#include "NotAmnesia.h"

IMemoryManager *CreateMemoryManagerInstance()
{
    return new CNotAmnesia();
}

CNotAmnesia::CNotAmnesia() :
    m_startPtr(nullptr),
    m_lastNugget(nullptr),
    m_totalSize(0),
    m_amountAllocated(0)
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
    // cant allocate zero bytes
    if (numBytes == 0)
        return false;

    m_totalSize = numBytes;
    m_startPtr = (preallocatedBuffer == nullptr) ? static_cast<unsigned char*>(malloc(numBytes)) : static_cast<unsigned char*>(preallocatedBuffer);
    m_nextFreePtr = m_startPtr;

    return m_startPtr != nullptr;
}

void *CNotAmnesia::Allocate(
        size_t numBytes,
        const char* file,
        int line
    )
{
    if (m_totalSize - m_amountAllocated < numBytes)
    {
        return nullptr;
    }

    MemoryNugget *newNugget = new MemoryNugget();
    newNugget->prevNugget = newNugget->nextNugget = nullptr;
    newNugget->ptr = m_nextFreePtr;
    newNugget->totalSize = numBytes;

    m_nextFreePtr += newNugget->totalSize;

    if (m_lastNugget == nullptr)
    {
        // first nugget created
        m_lastNugget = newNugget;
    }
    else
    {
        // add a nugget to our bi-directional list
        newNugget->prevNugget = m_lastNugget;
        m_lastNugget->nextNugget = newNugget;
        m_lastNugget = newNugget;
    }

    return newNugget->ptr;
}

void *CNotAmnesia::AllocateAligned(
        size_t numBytes,
        size_t alignment,
        const char* file,
        int line
    )
{
    return nullptr;
}

void CNotAmnesia::Release(
        void* address
    )
{
    // cant free a nullptr
    if (address == nullptr)
        return;

    // Not our memory to release
    if (address < m_startPtr)
        return;

    // Not our memory to release
    if (address > (m_startPtr + m_totalSize))
        return;

    MemoryNugget *const nugget = FindMemeoryNugget(address);

    // didn't find a nugget for the address specified
    if (nugget == nullptr)
        return;

    // fixup our linked list
    MemoryNugget *next = nugget->nextNugget;
    MemoryNugget *prev = nugget->prevNugget;
    if (prev != nullptr)
    {
        prev->nextNugget = next;
        next->prevNugget = prev;
    }

    if (m_lastNugget == nugget)
    {
        m_lastNugget = prev;
    }

    // release the memory ptr and the nugget
    free(address);
    delete nugget;
}

void CNotAmnesia::Shutdown()
{

}

const char* const CNotAmnesia::GetTextLine()
{
    return nullptr;
}

CNotAmnesia::MemoryNugget *CNotAmnesia::FindMemeoryNugget(
        void *address
    )
{
    MemoryNugget *currentNugget = m_lastNugget;
    while (currentNugget != nullptr)
    {
        if (currentNugget->ptr == address)
            return currentNugget;

        currentNugget = currentNugget->prevNugget;
    }

    return nullptr;
}
