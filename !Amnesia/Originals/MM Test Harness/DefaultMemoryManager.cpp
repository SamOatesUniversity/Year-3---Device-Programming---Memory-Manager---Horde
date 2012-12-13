#include "DefaultMemoryManager.h"
#include <string.h>
#include <sstream>

template <class T>
std::string ToString(const T& t)
{
    std::ostringstream sstr;
    sstr << t;
    return sstr.str();
}

// Function to create our MM instance
IMemoryManager *CreateMemoryManagerInstance()
{
    return new CDefaultMemoryManager();
}

const int kTextBufferSize=1024;

CDefaultMemoryManager::CDefaultMemoryManager()
{
    m_textBuffer=new char[kTextBufferSize];
}

CDefaultMemoryManager::~CDefaultMemoryManager()
{
    for (std::list<CMemoryBlock*>::iterator p=m_memoryBlocks.begin();p!=m_memoryBlocks.end();++p)
        delete (*p);
    m_memoryBlocks.clear();

    delete []m_memoryStartAddress;
    delete []m_textBuffer;
}

// If preallocatedBuffer is 0 (NULL) MM should allocate numBytes memory
// Otherwise MM should use the provided preallocatedBuffer
bool CDefaultMemoryManager::Initialise(size_t numBytes, void *preallocatedBuffer)
{
    m_totalMemory=numBytes;
    m_memoryStartAddress=(BYTE*)preallocatedBuffer;
    if (m_memoryStartAddress==0)
    {
        m_memoryStartAddress=new BYTE[numBytes];

        if (!m_memoryStartAddress)
        {
            AddStatusText("Could not allocate memory of: "+ToString(numBytes)+" bytes.");
            return false;
        }
    }

    m_nextFreeMemoryAddress=m_memoryStartAddress;

    return true;
}

// Allocate memory of size numBytes and return a pointer to it
// file and line provide the file and line the allocation was requested on and can be used
// for debugging e.g. outputting memory leaks and where the allocation occurred
void *CDefaultMemoryManager::Allocate(size_t numBytes, const char* file, int line)
{
    // Let us see if we can service the request
    // simplistic solutions only here! e.g. we do not reuse blocks
    if (GetMemoryAvailable()<numBytes)
    {
        AddStatusText("No room available to allocate memory of: "+ToString(numBytes)+" bytes.");
        return 0;
    }

    CMemoryBlock* newBlock=new CMemoryBlock();
    newBlock->m_actualPointer=m_nextFreeMemoryAddress;
    newBlock->m_returnedPointer=newBlock->m_actualPointer;
    newBlock->m_actualSize=numBytes;
    newBlock->m_allocatedSize=newBlock->m_actualSize;
    if (file)
    {
        newBlock->m_file=new std::string(file);
        newBlock->m_line=line;
    }

    m_memoryBlocks.push_back(newBlock);

    m_nextFreeMemoryAddress+=newBlock->m_actualSize;

    AddStatusText("Allocated memory of: "+ToString(numBytes)+" bytes.");

    return newBlock->m_returnedPointer;
}

// As above but memory pointer must be aligned to alignment bytes. Note alignment is
// always a power of 2
void *CDefaultMemoryManager::AllocateAligned(size_t numBytes,size_t alignment,const char* file, int line)
{
    // Dont want to give away how to do this :)
    AddStatusText("Unsuported by this MM");
    return 0;
}

// Release the memory pointer to by address
void CDefaultMemoryManager::Release(void* address)
{
    for (std::list<CMemoryBlock*>::iterator p=m_memoryBlocks.begin();p!=m_memoryBlocks.end();++p)
    {
        if ((*p)->m_returnedPointer==address)
        {
            delete (*p);
            m_memoryBlocks.erase(p);
            return;
        }
    }

    AddStatusText("Release: Unknown Memory Address: "+ToString(address));
}

// Allows the MM to output debug text to the test harness
// Return 0 if no new text to be output
// It is expected this will be used to output any leaks or other debugging information
const char* const CDefaultMemoryManager::GetTextLine()
{
    if (!m_statusTextList.empty())
    {
        strncpy(m_textBuffer,m_statusTextList.front().c_str(),kTextBufferSize);
        m_statusTextList.pop_front();
        return m_textBuffer;
    }
    return 0;
}

// Shutdown MM and report any stats and leaks etc.
void CDefaultMemoryManager::Shutdown()
{
    AddStatusText("MM Shutting Down");
    for (std::list<CMemoryBlock*>::iterator p=m_memoryBlocks.begin();p!=m_memoryBlocks.end();++p)
    {
        CMemoryBlock* b=(*p);
        AddStatusText("Unfreed block: "+ToString(b->m_returnedPointer)+" : "+ToString(b->m_allocatedSize));
        AddStatusText("File: "+*b->m_file+" at line:"+ToString(b->m_line));
    }
    AddStatusText("MM Finished");
}

void CDefaultMemoryManager::AddStatusText(const std::string &text)
{
    m_statusTextList.push_back(text);
}

size_t CDefaultMemoryManager::GetMemoryAvailable() const
{
    size_t used=m_nextFreeMemoryAddress-m_memoryStartAddress;

    return m_totalMemory-used;
}

