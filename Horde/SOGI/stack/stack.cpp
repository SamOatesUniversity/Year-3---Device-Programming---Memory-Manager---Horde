// Copyright 2007 Edd Dawson.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifdef WIN32

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <stdexcept>
#include <sstream>

#include "stack.h"

#include <windows.h>
#include <imagehlp.h>
#include <bfd.h> // link against libbfd and libiberty
#include <psapi.h> // link against psapi
#include <cxxabi.h>

namespace
{
    const char * const unknown_function = "[unknown function]";
    const char * const unknown_module = "[unknown module]";

    std::string demangle(const char *name)
    {
        if (!name)
            return unknown_function;

        int status = 0;
        char *d = 0;
        std::string ret = name;
        try
        {
            if ((d = abi::__cxa_demangle(name, 0, 0, &status)))
                ret = d;
        }
        catch (const std::bad_alloc &) {  }

        std::free(d);
        return ret;
    }
    // Derive from this to disallow copying of your class.
    // c.f. boost::noncopyable
    class uncopyable
    {
        protected:
            uncopyable() { }

        private:
            uncopyable(const uncopyable &);
            uncopyable &operator= (const uncopyable &);
    };

    // Provides a means to translate a program counter offset in to the name of the corresponding function.
    class bfd_context : uncopyable
    {
        public:
            struct find_data
            {
                std::string func;
                int line;
                std::string file;
                asymbol **symbol_table;
                bfd_vma counter;
            };

        public:
            bfd_context() :
                abfd_(0),
                sec_(0),
                symbol_table_(0)
            {
                char procname[MAX_PATH];
                GetModuleFileNameA(NULL, procname, sizeof procname);

                bfd_init();
                abfd_ = bfd_openr(procname, 0);
                if (!abfd_)
                    throw std::runtime_error("Failed to parse object data for the executable");

                char **formats = 0;
                bool b1 = bfd_check_format(abfd_, bfd_object);
                bool b2 = bfd_check_format_matches(abfd_, bfd_object, &formats);
                bool b3 = bfd_get_file_flags(abfd_) & HAS_SYMS;

                if (!(b1 && b2 && b3))
                {
                    bfd_close(abfd_);
                    free(formats);
                    throw std::runtime_error("Failed to parse object data for the executable");
                }
                free(formats);

                // Load symbol table
                unsigned dummy = 0;
                if (bfd_read_minisymbols(abfd_, FALSE, reinterpret_cast<void **>(&symbol_table_), &dummy) == 0 &&
                    bfd_read_minisymbols(abfd_, TRUE, reinterpret_cast<void **>(&symbol_table_), &dummy) < 0)
                {
                    free(symbol_table_);
                    bfd_close(abfd_);
                    throw std::runtime_error("Failed to parse object data for the executable");
                }
            }

            ~bfd_context()
            {
                free(symbol_table_);
                bfd_close(abfd_);
            }

            find_data get_function_data(DWORD offset)
            {
                find_data data;
                data.symbol_table = symbol_table_;
                data.counter = offset;

                bfd_map_over_sections(abfd_, &find_function_name_in_section, &data);

                return data;
            }

        private:
            static void find_function_name_in_section(bfd *abfd, asection *sec, void *opaque_data)
            {
                assert(sec);
                assert(opaque_data);
                find_data &data = *static_cast<find_data *>(opaque_data);

                if (!data.func.empty()) return; // already found it

                if (!(bfd_get_section_flags(abfd, sec) & SEC_ALLOC)) return;

                bfd_vma vma = bfd_get_section_vma(abfd, sec);
                if (data.counter < vma || vma + bfd_get_section_size(sec) <= data.counter) return;

                const char *func = 0;
                const char *file = 0;
                unsigned line = 0;

                if (bfd_find_nearest_line(abfd, sec, data.symbol_table, data.counter - vma, &file, &func, &line) && func)
                    data.func = demangle(func);

                data.file = file;
                data.line = line;
            }

        private:
            bfd *abfd_;
            asection *sec_;
            asymbol **symbol_table_;
    };

    // g++ spouts warnings if you use {0} to initialize PODs. So we use this instead:
    const struct
    {
        template<typename POD>
        operator POD () const { POD p; std::memset(&p, 0, sizeof p); return p; }
    }
    empty_pod = { };

    // Wraps a FARPROC. Implicitly convertible to any kind of pointer-to-function.
    // Avoids having reinterpret casts all over the place.
    struct auto_cast_function_ptr
    {
        auto_cast_function_ptr(FARPROC f) : fptr_(f) { }

        template<typename FuncPtr>
        operator FuncPtr() const { return reinterpret_cast<FuncPtr>(fptr_); }

        FARPROC fptr_;
    };

    // A wrapper around a DLL. Can dynamically get function pointers with the function() function!
    class windows_dll : uncopyable
    {
        public:
            explicit windows_dll(const std::string &libname) :
                name_(libname),
                lib_(LoadLibrary(name_.c_str()))
            {
                if (!lib_) throw std::runtime_error("Failed to load dll " + name_);
            }

            ~windows_dll() { FreeLibrary(lib_); }

            const std::string &name() const { return name_; }

            auto_cast_function_ptr function(const char *func_name) const
            {
                FARPROC proc = GetProcAddress(lib_, func_name);
                if (!proc) throw std::runtime_error(std::string("failed to load function ") + func_name + " from library " + name_);

                return proc;
            }

        private:
            std::string name_;
            HMODULE lib_;
    };

    // An object that makes sure debugging symbols are available
    class symbol_context : uncopyable
    {
        public:
            symbol_context()
            {
                if (!SymInitialize(GetCurrentProcess(), 0, TRUE))
                    throw std::runtime_error("Failed to initialize symbol context");
            }
            ~symbol_context() { SymCleanup(GetCurrentProcess()); }
    };

    // A simple Windows mutex class. Use a lock object to lock the mutex for the duration of a scope.
    class mutex : uncopyable
    {
        public:
            mutex() { InitializeCriticalSection(&cs_); }
            ~mutex() { DeleteCriticalSection(&cs_); }

        private:
            friend class lock;
            void lock() { EnterCriticalSection(&cs_); }
            void unlock() { LeaveCriticalSection(&cs_); }

            CRITICAL_SECTION cs_;
    }
    g_fill_frames_mtx;

    // A lock for the mutex
    class lock : uncopyable
    {
        public:
            lock(mutex &m) : m_(m) { m.lock(); }
            ~lock() { m_.unlock(); }
        private:
            mutex &m_;
    };


    void fill_frames(std::list<dbg::stack_frame> &frames, dbg::stack::depth_type limit)
    {
        lock lk(g_fill_frames_mtx);

        symbol_context sc;
        bfd_context bfdc;

        STACKFRAME frame = empty_pod;
        CONTEXT context = empty_pod;
        context.ContextFlags = CONTEXT_FULL;

        windows_dll kernel32("kernel32.dll");
        void (WINAPI *RtlCaptureContext_)(CONTEXT*) = kernel32.function("RtlCaptureContext");

        RtlCaptureContext_(&context);

        frame.AddrPC.Offset = context.Eip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Esp;
        frame.AddrStack.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Ebp;
        frame.AddrFrame.Mode = AddrModeFlat;

        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();

        bool skip = true;
        bool has_limit = limit != 0;
        char symbol_buffer[sizeof(IMAGEHLP_SYMBOL) + 255];
        char module_name_raw[MAX_PATH];
        const DWORD machine = IMAGE_FILE_MACHINE_I386;

        while(StackWalk(machine, process, thread, &frame, &context, 0, SymFunctionTableAccess, SymGetModuleBase, 0))
        {
            if (skip)
            {
                skip = false;
                continue;
            }

            if (has_limit && limit-- == 0) break;

            IMAGEHLP_SYMBOL *symbol = reinterpret_cast<IMAGEHLP_SYMBOL *>(symbol_buffer);
            symbol->SizeOfStruct = (sizeof *symbol) + 255;
            symbol->MaxNameLength = 254;

            DWORD module_base = SymGetModuleBase(process, frame.AddrPC.Offset);
            std::string module_name = unknown_module;
            if (module_base && GetModuleFileNameA(reinterpret_cast<HINSTANCE>(module_base), module_name_raw, MAX_PATH))
                module_name = ((strrchr(module_name_raw, '\\') ?: __FILE__ - 1) + 1);

            bfd_context::find_data funcdata = bfdc.get_function_data(frame.AddrPC.Offset);

            const size_t found = funcdata.file.find_last_of("/\\");
            std::stringstream functionLocation;
            if (funcdata.line > 0)
                functionLocation << funcdata.file.substr(found+1) << " (" << funcdata.line << ")";
            else if(!funcdata.file.empty())
                functionLocation << funcdata.file.substr(found+1);

            if (funcdata.func.empty())
            {

                DWORD dummy = 0;
                if (SymGetSymFromAddr(process, frame.AddrPC.Offset, &dummy, symbol))
                {
                    funcdata.func = symbol->Name;
                    module_name = functionLocation.str();
                }
                else if (funcdata.line != 0)
                {
                    funcdata.func = functionLocation.str();
                }
                else
                {
                    funcdata.func = unknown_function;
                    module_name = functionLocation.str();
                }
            }
            else
            {
                module_name = functionLocation.str();
            }

            dbg::stack_frame f(reinterpret_cast<const void *>(frame.AddrPC.Offset), funcdata.func, module_name);
            frames.push_back(f);
        }
    }
} // close anonymous namespace



namespace dbg
{
    stack_frame::stack_frame(const void *instruction, const std::string &function, const std::string &module) :
        instruction(instruction),
        function(function),
        module(module)
    {
    }

    std::ostream &operator<< (std::ostream &out, const stack_frame &frame)
    {
        return out << frame.instruction << ": " << frame.function << " in " << frame.module;
    }

    stack::stack(depth_type limit)
    {
        fill_frames(frames_, limit);
    }

    stack::const_iterator stack::begin() const
    {
        return frames_.begin();
    }

    stack::const_iterator stack::end() const
    {
        return frames_.end();
    }

    stack::depth_type stack::depth() const
    {
        return frames_.size();
    }

} // close namespace dbg

#endif
