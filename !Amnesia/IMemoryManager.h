#if !defined(IMemoryManager_h)
#define IMemoryManager_h

// Memory manager interfaceclass
class IMemoryManager
{
public:
    // If preallocatedBuffer is 0 (NULL) MM should allocate numBytes memory
    // Otherwise MM should use the provided preallocatedBuffer
    // Note: in either case the MM should delete[] the memory
    virtual bool Initialise(size_t numBytes, void *preallocatedBuffer=0)=0;

    // Allocate memory of size numBytes and return a pointer to it
    // file and line provide the file and line the allocation was requested on and can be used
    // for debugging e.g. outputting memory leaks and where the allocation occurred in Shutdown
    virtual void *Allocate(size_t numBytes, const char* file, int line)=0;

    // Release the memory pointed to by address originally allocated via Allocate
    virtual void Release(void* address)=0;

    // As above but memory pointer must be aligned to alignment bytes
    // User should release via ReleaseAligned
    // Note alignment is always a power of 2 and never more than 128
    virtual void *AllocateAligned(size_t numBytes,size_t alignment,const char* file, int line)=0;

    // Release the memory pointed to by address originally allocated via AllocateAligned
    virtual void ReleaseAligned(void* address)=0;

    // Shutdown MM and report any stats and leaks etc.
    virtual void Shutdown()=0;

    // Allows the MM to output debug text to the test harness
    // Return 0 if no new text to be output. Caller will call repeatadly until 0 is returned i.e. this is a pop
    // Note: caller will only copy the text i.e. it will not delete it or modify it or hold onto it
    // It is expected this will be used to output any leaks or other debugging information and status text
    virtual const char* const GetTextLine()=0;
};

// Function to create a MM instance, define in your MM
// Caller will delete the pointer when done with it
IMemoryManager *CreateMemoryManagerInstance();

#endif
