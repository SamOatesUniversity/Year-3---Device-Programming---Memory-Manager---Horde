#ifndef CDEFAULTMEMORYMANAGER_H
#define CDEFAULTMEMORYMANAGER_H

/*

    A default memory manager using the OS built in memory allocation methods

    This does nothing clever, just used to test the harness

    It fails some of the tests.

*/


#include <list>
#include <string>

#include "IMemoryManager.h"

typedef unsigned char BYTE;

// Memory block data
class CMemoryBlock
{
    public:
        size_t m_actualSize;
        size_t m_allocatedSize;
        void *m_returnedPointer;
        void *m_actualPointer;    // May differ from above if aligned
       // bool m_allocated;

        // For debug purposes
        std::string *m_file;
        int m_line;

        CMemoryBlock():  m_actualSize(0), m_allocatedSize(0),m_returnedPointer(0),m_actualPointer(0),m_file(0),m_line(0){};
        ~CMemoryBlock()
        {
            delete m_file;
        }
};

// A default MM
class CDefaultMemoryManager : public IMemoryManager
{
    private:
        std::list<CMemoryBlock*> m_memoryBlocks;
        size_t m_totalMemory;
        BYTE *m_memoryStartAddress;
        BYTE *m_nextFreeMemoryAddress;
        std::list<std::string> m_statusTextList;
        char *m_textBuffer;
    public:
        CDefaultMemoryManager();
        virtual ~CDefaultMemoryManager();

        // Pure virtuals from parent that must be implemented:
        bool Initialise(size_t numBytes, void *preallocatedBuffer=0);
        void *Allocate(size_t numBytes, const char* file, int line);
        void *AllocateAligned(size_t numBytes,size_t alignment,const char* file, int line);
        void Release(void* address);
        const char* const GetTextLine();
        void Shutdown();
    private:
        size_t GetMemoryAvailable() const;
        void AddStatusText(const std::string &text);
};

#endif // CDEFAULTMEMORYMANAGER_H
