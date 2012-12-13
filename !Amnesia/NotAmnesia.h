#ifndef CDEFAULTMEMORYMANAGER_H
#define CDEFAULTMEMORYMANAGER_H

#include <list>
#include <string>
#include <stdlib.h>

#include "IMemoryManager.h"

#ifndef nullptr
    #define nullptr 0
#endif

class CNotAmnesia : public IMemoryManager
{
    private:

    struct MemoryNugget
    {
        size_t                                      totalSize;
        void                                        *ptr;

        std::string                                 file;
        int                                         line;

        MemoryNugget                                *nextNugget;
        MemoryNugget                                *prevNugget;
    };

    private:

        unsigned char                               *m_startPtr;                                //! The start point of the allocated memory (nullptr if uninitialized)
        unsigned char                               *m_nextFreePtr;                             //!

        MemoryNugget                                *m_lastNugget;                              //!

        size_t                                      m_totalSize;                                //! The size of the allocated memory
        size_t                                      m_amountAllocated;                          //! The size of the allocated memory

    private:

        MemoryNugget                                *FindMemeoryNugget(
                                                        void *address
                                                    );

    public:
                                                    //!
                                                    CNotAmnesia();

                                                    //!
        virtual                                     ~CNotAmnesia();

                                                    //!
        bool                                        Initialise(size_t numBytes, void *preallocatedBuffer=0);

                                                    //!
        void                                        *Allocate(size_t numBytes, const char* file, int line);

                                                    //!
        void                                        *AllocateAligned(size_t numBytes,size_t alignment,const char* file, int line);

                                                    //!
        void                                        Release(void* address);

                                                    //!
        void                                        Shutdown();

                                                    //!
        const char* const                           GetTextLine();

};

#endif // CDEFAULTMEMORYMANAGER_H
