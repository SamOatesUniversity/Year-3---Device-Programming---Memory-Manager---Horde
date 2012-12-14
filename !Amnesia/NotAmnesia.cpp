#include "NotAmnesia.h"

IMemoryManager *CreateMemoryManagerInstance()
{
    return new CNotAmnesia();
}

CNotAmnesia::CNotAmnesia() :
    m_startPtr(nullptr),
    m_lastNugget(nullptr),
	m_freeNugget(nullptr),
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
		// see if we can merge any free nuggets to re-use
		if (m_freeNugget == nullptr)
			return nullptr;

		// see if any of our released nuggets happen to be bug enough to re-use
		MemoryNugget *nextFreeNugget = m_freeNugget;
		while (nextFreeNugget->prevNugget != nullptr)
		{
			if (nextFreeNugget->totalSize >= numBytes)
			{
				// fixup the free nuggets list
				MemoryNugget *next = nextFreeNugget->nextNugget;
				MemoryNugget *prev = nextFreeNugget->prevNugget;
				if (next != nullptr) next->prevNugget = prev;
				if (prev != nullptr) prev->nextNugget = next;

				// move the free nugget onto the allocated nuggets
				m_lastNugget->nextNugget = nextFreeNugget;
				nextFreeNugget->prevNugget = m_lastNugget;
				m_lastNugget = nextFreeNugget;

				#ifndef OPTIMIZED
					if (nextFreeNugget->totalSize > 10)
						strncpy_s((char*)(nextFreeNugget->ptr), nextFreeNugget->totalSize, "NotAmnesia", nextFreeNugget->totalSize);
				#endif

				return nextFreeNugget->ptr;
			}
			nextFreeNugget = nextFreeNugget->prevNugget;
		}

		MemoryNugget *firstFreeNugget = m_freeNugget;
		while (firstFreeNugget->prevNugget != nullptr)
			firstFreeNugget = firstFreeNugget->prevNugget;

		// we have already checked for a free correct sized nugget, so this will fail
		if (firstFreeNugget->nextNugget == nullptr)
			return nullptr;

		// try find a few nuggets next to each other that we can merge
		MemoryNugget *nextNugget = firstFreeNugget->nextNugget;
		while (firstFreeNugget->ptr + firstFreeNugget->totalSize == nextNugget->ptr)
		{
			if (static_cast<size_t>((nextNugget->ptr + nextNugget->totalSize) - firstFreeNugget->ptr) > numBytes)
			{
				// okay, we have 2 or more nuggets next to each other that combined have the same or more space available.
				while (nextNugget != firstFreeNugget)
				{
					MemoryNugget *currentNugget = nextNugget;

					MemoryNugget *next = currentNugget->nextNugget;
					MemoryNugget *prev = currentNugget->prevNugget;
					if (next != nullptr) next->prevNugget = prev;
					if (prev != nullptr) prev->nextNugget = next;

					nextNugget = prev;
					delete currentNugget;
				}

				MemoryNugget *next = firstFreeNugget->nextNugget;
				MemoryNugget *prev = firstFreeNugget->prevNugget;
				if (next != nullptr) next->prevNugget = prev;
				if (prev != nullptr) prev->nextNugget = next;

				if (next == nullptr && prev == nullptr)
					m_freeNugget = nullptr;

				firstFreeNugget->prevNugget = m_lastNugget;
				m_lastNugget->nextNugget = firstFreeNugget;
				firstFreeNugget->nextNugget = nullptr;
				m_lastNugget = firstFreeNugget;

				firstFreeNugget->totalSize = numBytes;

				#ifndef OPTIMIZED
					if (firstFreeNugget->totalSize > 10)
						strncpy_s((char*)(firstFreeNugget->ptr), firstFreeNugget->totalSize, "NotAmnesia", firstFreeNugget->totalSize);
				#endif

				return firstFreeNugget->ptr;
			}

			nextNugget = nextNugget->nextNugget;
		}

        return nullptr;
    }

    MemoryNugget *newNugget = new MemoryNugget();
    newNugget->prevNugget = newNugget->nextNugget = nullptr;
    newNugget->ptr = m_nextFreePtr;
    newNugget->totalSize = numBytes;

#ifndef OPTIMIZED
	newNugget->file = file;
	newNugget->line = line;
#endif

    m_nextFreePtr += newNugget->totalSize;
	m_amountAllocated += newNugget->totalSize;

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

	#ifndef OPTIMIZED
		if (newNugget->totalSize > 10)
			strncpy_s((char*)(newNugget->ptr), newNugget->totalSize, "NotAmnesia", newNugget->totalSize);
	#endif

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
   
	if (prev != nullptr) prev->nextNugget = next;
    if (next != nullptr) next->prevNugget = prev;

    if (m_lastNugget == nugget)
    {
        m_lastNugget = prev;
		if (m_lastNugget == nullptr)
		{
			m_nextFreePtr = m_startPtr;
			m_totalSize += m_amountAllocated;
		}
    }
	else
	{
		if (m_freeNugget == nullptr)
		{
			#ifndef OPTIMIZED
				if (nugget->totalSize > 10)
					strncpy_s((char*)(nugget->ptr), nugget->totalSize, "Amnesia", nugget->totalSize);
			#endif

			m_freeNugget = nugget;
			m_freeNugget->nextNugget = nullptr;
			m_freeNugget->prevNugget = nullptr;
		}
		else
		{
			#ifndef OPTIMIZED
				if (nugget->totalSize > 10)
					strncpy_s((char*)(nugget->ptr), nugget->totalSize, "Amnesia", nugget->totalSize);
			#endif

			m_freeNugget->nextNugget = nugget;
			nugget->prevNugget = m_freeNugget;
			nugget->nextNugget = nullptr;
			m_freeNugget = nugget;
		}
	}

	// if it's the last nugget, just delete it
	if (next == nullptr)
	{
		delete nugget;
	}
}

void CNotAmnesia::Shutdown()
{
	MemoryNugget *freeNugget = m_freeNugget;
	while (freeNugget != nullptr)
	{
		MemoryNugget *prevNugget = freeNugget->prevNugget;
		delete freeNugget;
		freeNugget = prevNugget;
	}

	MemoryNugget *nugget = m_lastNugget;
	while (nugget != nullptr)
	{
		MemoryNugget *prevNugget = nugget->prevNugget;
		delete nugget;
		nugget = prevNugget;
	}

	free(m_startPtr);
	m_startPtr = nullptr;
	m_totalSize = 0;
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
