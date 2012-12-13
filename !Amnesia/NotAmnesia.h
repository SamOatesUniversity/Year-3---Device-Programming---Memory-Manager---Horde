#ifndef CDEFAULTMEMORYMANAGER_H
#define CDEFAULTMEMORYMANAGER_H

#include <list>
#include <string>
#include "IMemoryManager.h"

class CNotAmnesia : public IMemoryManager
{
    private:

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
                                                    void Shutdown();

                                                    //!
        const char* const                           GetTextLine();

};

#endif // CDEFAULTMEMORYMANAGER_H
