#include "NotAmnesia.h"

/*
*	\brief Create a new instance of our memory manager
*/
IMemoryManager *CreateMemoryManagerInstance()
{
    return new CNotAmnesia();
}

/*
*	\brief	Class constructor
*/
CNotAmnesia::CNotAmnesia() :
    m_startPtr(nullptr),
    m_lastNugget(nullptr),
	m_firstNugget(nullptr),
	m_freeNugget(nullptr),
    m_totalSize(0),
    m_amountAllocated(0)
{
#ifndef OPTIMIZED
	m_noofNuggets = 0;
	m_noofFreeNuggets = 0;
#endif
}

/*
*	\brief	Class Destructor
*/
CNotAmnesia::~CNotAmnesia()
{
}

/*
*	\brief	Initializes the memory manger, allocating a given number of bytes
*/
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

	if (m_startPtr != nullptr)
	{
		memset(m_startPtr, 0xFEFEFEFE, numBytes);
	}

    return m_startPtr != nullptr;
}

/*
*	\brief	Allocate memory from our buffer, storing it in a memory nugget
*/
void *CNotAmnesia::Allocate(
        size_t numBytes,
        const char* file,
        int line
    )
{
	MemoryNugget *newNugget = nullptr;

	// see if we can re-use a free nugget
	if (m_freeNugget != nullptr)
	{
		newNugget = MergeMemoryNuggets(numBytes);
		if (newNugget != nullptr)
		{
			#ifndef OPTIMIZED
			CheckForBufferOverflow(newNugget);
			memset(newNugget->ptr, 0x00000000, newNugget->totalSize);
			#endif

			return newNugget->ptr;
		}
	}

	// tried to allocate more than what we have
	if (m_totalSize - m_amountAllocated < numBytes)
	{
		return nullptr;
	}

	if (newNugget == nullptr)
	{
		// no free nuggets available, so create a new one
		newNugget = A_NEW(MemoryNugget);
		if (newNugget == nullptr)
			return nullptr;

		newNugget->ptr = m_nextFreePtr;
		newNugget->totalSize = numBytes;
		m_nextFreePtr += newNugget->totalSize;
		m_amountAllocated += newNugget->totalSize;
	}

#ifndef OPTIMIZED
	newNugget->file = file;
	newNugget->line = line;
#endif

	PushNuggetToAllocatedList(newNugget);

#ifndef OPTIMIZED
	CheckForBufferOverflow(newNugget);
	memset(newNugget->ptr, 0x00000000, newNugget->totalSize);
#endif

    return newNugget->ptr;
}

/*
*	\brief	Allocate memory from our buffer, aligned to a given alignment, storing it in a memory nugget
*/
void *CNotAmnesia::AllocateAligned(
        size_t numBytes,
        size_t alignment,
        const char* file,
        int line
    )
{
	size_t alignOffset = alignment - (reinterpret_cast<size_t>(m_nextFreePtr) % alignment);

	// if the pointer isn't already aligned, make a padded nugget,
	// to push the address to an aligned value. Put the padded nugget
	// on to the list of free nuggets
	if (alignOffset != alignment)
	{
		MemoryNugget *const paddingNugget = A_NEW(MemoryNugget);
		if (paddingNugget == nullptr)
			return nullptr;

		paddingNugget->ptr = m_nextFreePtr;
		paddingNugget->totalSize = alignOffset;
		m_nextFreePtr += paddingNugget->totalSize;
		m_amountAllocated += paddingNugget->totalSize;

		#ifndef OPTIMIZED
			paddingNugget->file = file;
			paddingNugget->line = line;
		#endif

		if (m_freeNugget == nullptr)
		{
			#ifndef OPTIMIZED
				m_noofFreeNuggets++;
			#endif

			m_freeNugget = paddingNugget;
			m_freeNugget->nextNugget = nullptr;
			m_freeNugget->prevNugget = nullptr;
		}
		else
		{
			#ifndef OPTIMIZED
				m_noofFreeNuggets++;
			#endif

			m_freeNugget->nextNugget = paddingNugget;
			paddingNugget->prevNugget = m_freeNugget;
			paddingNugget->nextNugget = nullptr;
			m_freeNugget = paddingNugget;
		}
	}

	// The memory address should now be aligned to the given alignment
	MemoryNugget *const newNugget = A_NEW(MemoryNugget);
	if (newNugget == nullptr)
		return nullptr;

	newNugget->ptr = m_nextFreePtr;
	newNugget->totalSize = numBytes;
	m_nextFreePtr += newNugget->totalSize;
	m_amountAllocated += newNugget->totalSize;

	#ifndef OPTIMIZED
		newNugget->file = file;
		newNugget->line = line;
	#endif

	PushNuggetToAllocatedList(newNugget);

#ifndef OPTIMIZED
	CheckForBufferOverflow(newNugget);
	memset(newNugget->ptr, 0x00000000, newNugget->totalSize);
#endif

    return newNugget->ptr;
}

/*
*	\brief	Release a given memory address, moving the memory nugget to the list of free memory nuggets for reuse
*/
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

	// we have no nuggets so we wont be able to find it
	if (m_lastNugget == nullptr)
		return;

    MemoryNugget *const nugget = FindMemoryNugget(static_cast<unsigned char*>(address));

    // didn't find a nugget for the address specified
    if (nugget == nullptr)
        return;

#ifndef OPTIMIZED
	memset(nugget->ptr, 0xCDCDCDCD, nugget->totalSize);
#endif

    // fix up our linked list
    MemoryNugget *next = nugget->nextNugget;
    MemoryNugget *prev = nugget->prevNugget;

	if (prev != nullptr) prev->nextNugget = next;
    if (next != nullptr) next->prevNugget = prev;

    if (m_lastNugget == nugget)
    {
        m_lastNugget = prev;
		if (m_lastNugget == nullptr)
		{
			m_firstNugget = nullptr;

			if (m_freeNugget == nullptr)
			{
				m_freeNugget = nugget;
				m_freeNugget->nextNugget = nullptr;
				m_freeNugget->prevNugget = nullptr;
			}
			else
			{
				m_freeNugget->nextNugget = nugget;
				nugget->prevNugget = m_freeNugget;
				nugget->nextNugget = nullptr;
				m_freeNugget = nugget;
			}
		}
    }
	else
	{
		if (m_firstNugget == nugget)
		{
			m_firstNugget = next;
		}

		if (m_freeNugget == nullptr)
		{
			#ifndef OPTIMIZED
				m_noofFreeNuggets++;
			#endif

			m_freeNugget = nugget;
			m_freeNugget->nextNugget = nullptr;
			m_freeNugget->prevNugget = nullptr;
		}
		else
		{
			#ifndef OPTIMIZED
				m_noofFreeNuggets++;
			#endif

			m_freeNugget->nextNugget = nugget;
			nugget->prevNugget = m_freeNugget;
			nugget->nextNugget = nullptr;
			m_freeNugget = nugget;
		}
	}

	#ifndef OPTIMIZED
		m_noofNuggets--;
	#endif
}

/*
*	\brief	Called when releasing an aligned memory address. JUst call the normal release.
*/
void CNotAmnesia::ReleaseAligned(
		void* address
	)
{
	Release(address);
}

/*
*	\brief	Release all memory and destroy all memory nuggets
*/
void CNotAmnesia::Shutdown()
{
	MemoryNugget *freeNugget = m_freeNugget;
	while (freeNugget != nullptr)
	{
		MemoryNugget *const prevNugget = freeNugget->prevNugget;
		A_DELETE(freeNugget);
		freeNugget = prevNugget;
	}
	m_freeNugget = nullptr;

#ifndef OPTIMIZED
	int noofLeakedNuggets = 0;
#endif

	MemoryNugget *nugget = m_lastNugget;
	while (nugget != nullptr)
	{
#ifndef OPTIMIZED
		noofLeakedNuggets++;
		std::stringstream buf;
		buf << "Memory leak detected in " << FILE_NAME(nugget->file.c_str()) << " (" << nugget->line << ")";
		m_statusTextList.push_back(buf.str().c_str());
#endif
		nugget = nugget->prevNugget;
	}

#ifndef OPTIMIZED
	if (noofLeakedNuggets != 0)
	{
		m_statusTextList.push_front("Memory Leaks Detected");
		if (noofLeakedNuggets != 2)
			m_statusTextList.push_front("More than 2 leaks, this should not happed");
	}
#endif

	nugget = m_lastNugget;
	while (nugget != nullptr)
	{
		MemoryNugget *const prevNugget = nugget->prevNugget;
		A_DELETE(nugget);
		nugget = prevNugget;
	}
	m_lastNugget = nullptr;

	A_DELETE(m_startPtr);
	m_startPtr = nullptr;

	m_totalSize = 0;
	m_amountAllocated = 0;

	#ifndef OPTIMIZED
		m_noofNuggets = 0;
		m_noofFreeNuggets = 0;
	#endif
}

/*
*	\brief	Get a text line from our list of debug text
*/
const char* const CNotAmnesia::GetTextLine()
{
#ifndef OPTIMIZED
    if (!m_statusTextList.empty())
    {
		static const int bufferSize = 1024;
		static char textBuffer[bufferSize];
#ifdef WIN32
        strncpy_s(textBuffer, m_statusTextList.front().c_str(), bufferSize);
#else
		strncpy(textBuffer, m_statusTextList.front().c_str(), bufferSize);
#endif
        m_statusTextList.pop_front();
        return textBuffer;
    }
#endif

    return nullptr;
}

/*
*	\brief	Find a memory nugget based upon a given memory address
*/
CNotAmnesia::MemoryNugget *CNotAmnesia::FindMemoryNugget(
        unsigned char* address												//!< The memory address of the nugget we are looking for
    )
{
	if (m_lastNugget->ptr == address) return m_lastNugget;		// looking for the last nugget, so just return that
	if (m_firstNugget->ptr == address) return m_firstNugget;	// looking for the first nugget, so just return that

	// work out if we are close to the last or fist memory nugget, then spin through the list from there
	if ((m_lastNugget->ptr - address) < (address - m_firstNugget->ptr))
	{
		MemoryNugget *currentNugget = m_lastNugget;
		while (currentNugget != nullptr)
		{
			if (currentNugget->ptr == address)
				return currentNugget;

			currentNugget = currentNugget->prevNugget;
		}
	}
	else
	{
		MemoryNugget *currentNugget = m_firstNugget;
		while (currentNugget != nullptr)
		{
			if (currentNugget->ptr == address)
				return currentNugget;

			currentNugget = currentNugget->nextNugget;
		}
	}

	// couldn't find the nugget represented by the given pointer
    return nullptr;
}

/*
*	\brief	Try to merge memory nuggets together to create a larger memory nugget
*/
CNotAmnesia::MemoryNugget *CNotAmnesia::MergeMemoryNuggets(
		size_t requiredSize											//!< The size we want the merged nugget to be
	)
{
	// see if we can merge any free nuggets to re-use
	if (m_freeNugget == nullptr)
		return nullptr;

	// see if any of our released nuggets happen to be bug enough to re-use
	MemoryNugget *nextFreeNugget = m_freeNugget;
	do
	{
		if (nextFreeNugget->totalSize >= requiredSize)
		{
			// fix up the free nuggets list
			MemoryNugget *next = nextFreeNugget->nextNugget;
			MemoryNugget *prev = nextFreeNugget->prevNugget;
			if (next != nullptr) next->prevNugget = prev;
			if (prev != nullptr) prev->nextNugget = next;

			if (nextFreeNugget == m_freeNugget)
				m_freeNugget = m_freeNugget->prevNugget;

			// move the free nugget onto the allocated nuggets
			if (m_lastNugget == nullptr)
			{
				m_lastNugget = nextFreeNugget;
				m_lastNugget->nextNugget = m_lastNugget->prevNugget = nullptr;
			}
			else
			{
				nextFreeNugget->nextNugget = nullptr;
				m_lastNugget->nextNugget = nextFreeNugget;
				nextFreeNugget->prevNugget = m_lastNugget;
				m_lastNugget = nextFreeNugget;
			}

			if (m_firstNugget == nullptr)
			{
				m_firstNugget = nextFreeNugget;
			}

			#ifndef OPTIMIZED
			m_noofFreeNuggets--;
			m_noofNuggets++;
			memset(nextFreeNugget->ptr, 0xcd, nextFreeNugget->totalSize);
			#endif

			return nextFreeNugget;
		}
		nextFreeNugget = nextFreeNugget->prevNugget;
	}
	while (nextFreeNugget != nullptr && nextFreeNugget->prevNugget != nullptr);

	// if there is only one nugget in the list, just resize it
	if (m_freeNugget->prevNugget == nullptr)
	{
		MemoryNugget *const nugget = m_freeNugget;

		const size_t sizeIncrease = requiredSize - nugget->totalSize;
		nugget->totalSize = requiredSize;

		m_nextFreePtr += sizeIncrease;
		m_totalSize += sizeIncrease;

		// move the nugget over
		m_freeNugget = nullptr;
		PushNuggetToAllocatedList(nugget);

		return nugget;
	}

	// spin back to find the first nugget in the list
	MemoryNugget *firstFreeNugget = m_freeNugget;
	while (firstFreeNugget->prevNugget != nullptr)
		firstFreeNugget = firstFreeNugget->prevNugget;

	// try find a few nuggets next to each other that we can merge
	while (firstFreeNugget != nullptr && firstFreeNugget != m_freeNugget)
	{
		MemoryNugget *nextNugget = firstFreeNugget->nextNugget;
		while (nextNugget != nullptr && nextNugget != m_freeNugget)
		{
			while (nextNugget != nullptr && firstFreeNugget->ptr + firstFreeNugget->totalSize == nextNugget->ptr)
			{
				size_t mergedNuggetSize = static_cast<size_t>((nextNugget->ptr + nextNugget->totalSize) - firstFreeNugget->ptr);
				{
					// we have 2 or more nuggets next to each other that combined have the same or more space available.
					while (nextNugget != firstFreeNugget && nextNugget != m_freeNugget)
					{
						MemoryNugget *const currentNugget = nextNugget;
						if (currentNugget == nullptr)
							continue;

						MemoryNugget *next = currentNugget->nextNugget;
						MemoryNugget *prev = currentNugget->prevNugget;
						if (next != nullptr) next->prevNugget = prev;
						if (prev != nullptr) prev->nextNugget = next;

						#ifndef OPTIMIZED
						memset(nextNugget->ptr, 0xcd, nextNugget->totalSize);
						#endif

						nextNugget = prev;
						A_DELETE(currentNugget);
					}

					firstFreeNugget->totalSize = mergedNuggetSize;

					if (mergedNuggetSize > requiredSize)
					{
						MemoryNugget *next = firstFreeNugget->nextNugget;
						MemoryNugget *prev = firstFreeNugget->prevNugget;
						if (next != nullptr) next->prevNugget = prev;
						if (prev != nullptr) prev->nextNugget = next;

						if (next == nullptr && prev == nullptr)
							m_freeNugget = nullptr;

						if (m_lastNugget == nullptr)
						{
							m_lastNugget = firstFreeNugget;
							m_lastNugget->nextNugget = m_lastNugget->prevNugget = nullptr;
						}
						else
						{
							firstFreeNugget->prevNugget = m_lastNugget;
							m_lastNugget->nextNugget = firstFreeNugget;
							firstFreeNugget->nextNugget = nullptr;
							m_lastNugget = firstFreeNugget;
						}

						#ifndef OPTIMIZED
						m_noofFreeNuggets--;
						m_noofNuggets++;
						memset(firstFreeNugget->ptr, 0xcd, firstFreeNugget->totalSize);
						#endif
						
						return firstFreeNugget;
					}
				}

				if (nextNugget != nullptr)
					nextNugget = nextNugget->nextNugget;
			}

			if (nextNugget != nullptr)
				nextNugget = nextNugget->nextNugget;
		}

		firstFreeNugget = firstFreeNugget->nextNugget;
	}

	return nullptr;
}

/*
*	\brief	Pushes a memory nugget to the end of the allocated nugget list
*/
void CNotAmnesia::PushNuggetToAllocatedList(
		MemoryNugget *nugget					//!< The nugget to add to the list
	)
{
	nugget->prevNugget = nugget->nextNugget = nullptr;

	if (m_lastNugget == nullptr)
	{
		// first nugget created
		m_firstNugget = m_lastNugget = nugget;
	}
	else
	{
		// add a nugget to our bi-directional list
		nugget->prevNugget = m_lastNugget;
		m_lastNugget->nextNugget = nugget;
		m_lastNugget = nugget;
	}

#ifndef OPTIMIZED
	m_noofNuggets++;
#endif
}

/*
*	\brief	Check the ptr of a nugget whilst allocating for buffer overflows
*/
void CNotAmnesia::CheckForBufferOverflow(
		MemoryNugget *nugget					//!< The nugget to check for buffer overflows
	)
{
	// pointing at unallocated memory, this is fine
	if ((*nugget->ptr) == 0xFE)
		return;

	// pointing at released memory, this is fine
	if ((*nugget->ptr) == 0xCD)
		return;

	// pointing at allocated memory, we shouldn't be allocating this!
	ASSERT((*nugget->ptr) != 0x00, "This pointer is pointing at already allocated memory");

	// pointing at some data, that isn't expected! most likely caused by a buffer overflow
	ASSERT((*nugget->ptr) == 0xFE || (*nugget->ptr) == 0xCD, "Buffer Overflow Detected!");
}
