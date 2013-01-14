#ifndef CDEFAULTMEMORYMANAGER_H
#define CDEFAULTMEMORYMANAGER_H

#include <list>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "IMemoryManager.h"

#ifndef nullptr
    #define nullptr 0
#endif

// we only really care about the file name, and not its full path, so strip it down.
#define FILE_NAME(FILE) (strrchr(FILE, '\\') ? strrchr(FILE, '\\') + 1 : FILE)

// We can remove most debug information and change how memory is allocated, by using this flag
// vastly improving performance.
#define OPTIMIZED

// We have to allocated memory for our memory nuggets using a NEW and DELETE define.
// The reason for this, is that malloc will not instanciate an instance of std::string
// resulting in a crash.
#ifndef OPTIMIZED
	#define NEW(TYPE)	new TYPE; 
	#define DELETE(OBJECT)	delete OBJECT;
#else
	#define NEW(TYPE)	static_cast<TYPE*>(malloc(sizeof(TYPE)))
	#define DELETE(OBJECT)	free(OBJECT);
#endif

class CNotAmnesia : public IMemoryManager
{
    private:

	//! A doubly linked list of memory nuggets, which are used
	//! to represent memory allocations
    struct MemoryNugget
    {
        size_t                                      totalSize;									//! The total size of the memory nugget
        unsigned char								*ptr;										//! The address the memory nugget represents

#ifndef OPTIMIZED
        std::string                                 file;										//! The file where the memory nugget was created
        int                                         line;										//! The line of the file where the memory nugget was created
#endif

        MemoryNugget                                *nextNugget;								//! The previous memory nugget in the linked list
        MemoryNugget                                *prevNugget;								//! The next memory nugget in the linked list
    };

    private:

        unsigned char                               *m_startPtr;                                //! The start point of the allocated memory (nullptr if uninitialized)
        unsigned char                               *m_nextFreePtr;                             //! A pointer of the next cached free memory address

        MemoryNugget                                *m_lastNugget;                              //! A pointer to the last allocated memory nugget
		MemoryNugget                                *m_firstNugget;                             //! A pointer to the first allocated memory nugget

		MemoryNugget                                *m_freeNugget;                              //! A pointer to the last free memory nugget

#ifndef OPTIMIZED
		int											m_noofNuggets;								//! The number of allocated memory nuggets
		int											m_noofFreeNuggets;							//! The number of nuggets that have been moved to the free list of memory nuggets
		std::list<std::string>						m_statusTextList;							//! A list of text output to be displayed
#endif

        size_t                                      m_totalSize;                                //! The total size of the allocated memory
        size_t                                      m_amountAllocated;                          //! The amount of memory we have allocated into memory nuggets

    private:

													//! Find a memory nugget based upon a given memory address
													//! \return THe memory nugget that represents the given address, or nullptr if no memory nugget was found
        MemoryNugget                                *FindMemoryNugget(
                                                        unsigned char* address							//!< The address to find
                                                    );

													//! Try to merge memory nuggets together to create a larger memory nugget
													//! \return A memory nugget created from merging other nuggets, or nullptr on fail
		MemoryNugget								*MergeMemoryNuggets(
														size_t requiredSize						//!< The size we are looking to create a memory nugget
													);

													//! Pushes a memory nugget to the end of the allocated nugget list
		void										PushNuggetToAllocatedList(
														MemoryNugget *nugget
													);

    public:
                                                    //! Class constructor
                                                    CNotAmnesia();

                                                    //! Class destructor
        virtual                                     ~CNotAmnesia();

                                                    //! Initializes the memory manger, allocating a given number of bytes
													//! \return True on success, false on fail
        virtual bool			                    Initialise(
														size_t numBytes,						//!< The number of bytes to allocate
														void *preallocatedBuffer = nullptr		//!< A pre-allocated buffer, or nullptr to create a new buffer
													);

                                                    //! Allocate memory from our buffer, storing it in a memory nugget
													//! \return The address of the allocated memory
        virtual void                                *Allocate(
														size_t numBytes,						//!< The number of bytes to allocate
														const char* file,						//!< The file where the allocation was called
														int line								//!< The line of the file where the allocation was called
													);

                                                    //! Allocate memory from our buffer, aligned to a given alignment, storing it in a memory nugget
													//! \return The address of the allocated memory
        virtual void                                *AllocateAligned(
														size_t numBytes,
														size_t alignment,
														const char* file, 
														int line
													);

                                                    //! Release a given memory address, moving the memory nugget to the list of free memory nuggets for reuse
        virtual void                                Release(
														void* address							//!< The memory address to release
													);

													//! Release the memory pointed to by address originally allocated via AllocateAligned
		virtual void								ReleaseAligned(
														void* address
													);

                                                    //! Release all memory and destroy all memory nuggets
        virtual void                                Shutdown();

                                                    //! NOT IMPLEMENTED
        virtual const char* const                   GetTextLine();

};

#endif // CDEFAULTMEMORYMANAGER_H
