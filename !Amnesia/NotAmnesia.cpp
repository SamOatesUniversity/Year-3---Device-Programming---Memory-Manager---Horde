#include "NotAmnesia.h"

IMemoryManager *CreateMemoryManagerInstance()
{
    return new CNotAmnesia();
}

CNotAmnesia::CNotAmnesia()
{
}

CNotAmnesia::~CNotAmnesia()
{
}

bool CNotAmnesia::Initialise(
        size_t numBytes,
        void *preallocatedBuffer
    )
{
    return true;
}

void *CNotAmnesia::Allocate(
        size_t numBytes,
        const char* file,
        int line
    )
{
    return 0;
}

void *CNotAmnesia::AllocateAligned(
        size_t numBytes,
        size_t alignment,
        const char* file,
        int line
    )
{
    return 0;
}

void CNotAmnesia::Release(void* address)
{

}

const char* const CNotAmnesia::GetTextLine()
{
    return 0;
}

void CNotAmnesia::Shutdown()
{

}
