/*

    Changes in Test harness Version 0.7
    - Changed basic test memory limit to 900 to allow storage of data alongside memory blocks
    - Removed the pause mode by default as it effects the timing
    Changes in Test harness Version 0.6
    - Fixed a problem with the Error Test (a missing else)
    - Added a virtual destructor to the interface
    Changes in Test harness Version 0.5
    - Changed the basic test so second half uses a provided buffer
    - Changed the Stress Test so it leaves two pointers unfreed
    - Changed the error test so it does some aligned memory tests
        - it now tests alignment for: 2,4,8,16,32,64,128
    - Changed the random
*/

#include "Tests.h"
#include "TextOutput.h"
#include "IMemoryManager.h"

#include <assert.h>
#include <sstream>
#include <math.h>
#include <time.h>

#include <windows.h>
// Enable this line to pause between tests (added 0.5)
//#define PAUSE_BETWEEN_TESTS

int noofLoops = 0;
template <class T>
std::string ToString(const T& t)
{
    std::ostringstream sstr;
    sstr << t;
    return sstr.str();
}

CTests::CTests(CTextOutput *harnessTextOutput,CTextOutput *mmTextOutput) : m_harnessTextOutput(harnessTextOutput),
    m_mmTextOutput(mmTextOutput), m_testResults(0), m_currentTest(eNotStarted), m_memoryMan(0),m_testPercentage(0),
    m_fails(0),m_passes(0), m_testsFailed(0), m_paused(false)
{
    m_testResults=new TTestResults[eNumTests];

    // Need consistant rands so tests fair to all
    //srand(2707); // v0.1
    srand(5707); // v0.5 jiggle
}

CTests::~CTests()
{
    delete []m_testResults;
}

std::string CTests::GetTestDescription(ETests test)
{
    switch(test)
    {
        case eBasicTest:
            return (" Basic Test");
        break;
        case eErrorTest:
            return (" Error Cond. Test");
        break;
        case eMergeFreeBlockTest:
            return (" Merge Block Test");
        break;
        case eStressTest:
            return (" Stress Test");
        break;
        default:
             return(" Error: unknown test");
        break;
    }

    return "";
}

// Returns false when finished
bool CTests::Update()
{
    FlushMMOutput();

    if (m_paused)
        return true;

    switch(m_currentTest)
    {
        case eNotStarted:
            return true;
        case eFinished:
            return false;
        case eBasicTest:
            BasicTest(m_testPercentage);
        break;
        case eErrorTest:
            ErrorTest(m_testPercentage);
        break;
        case eMergeFreeBlockTest:
            MergeFreeBlockTest(m_testPercentage);
        break;
        case eStressTest:
            StressTest(m_testPercentage);
        break;
        default:
             m_harnessTextOutput->AddLine("Error: unknown test");
            return false;
        break;
    }

    m_testPercentage++;

    if (m_testPercentage>=100)
    {
        ShutdownMM();

        m_testResults[m_currentTest].report=("Test: "+ToString((int)m_currentTest+1)+GetTestDescription(m_currentTest)+" : "+ToString(m_passes)+
                                           " passed and "+ToString(m_fails)+" failed");

        m_harnessTextOutput->SetStatusText(m_testResults[m_currentTest].report);

        m_testResults[m_currentTest].endTime=SDL_GetTicks();

        m_testResults[m_currentTest].passed=true;
        if (m_fails>0)
        {
            m_testsFailed++;
            m_testResults[m_currentTest].passed=false;
        }

        m_currentTest=(ETests)(m_currentTest+1);

        if (m_currentTest>=eNumTests)
        {
            m_currentTest=eFinished;
            FinalReport();
			/*m_currentTest = eBasicTest;
			noofLoops++;
			if (noofLoops % 100 == 0)
				srand(time(NULL));*/
        }
        else
        {
            InitTestResult(m_currentTest);
            #if defined(PAUSE_BETWEEN_TESTS)
                m_paused=true;
            #endif
        }

        m_testPercentage=0;
        m_passes=0;
        m_fails=0;
    }
    else
        m_harnessTextOutput->SetStatusText("Test: "+ToString((int)m_currentTest+1)+" "+ToString(m_testPercentage)+" %");

    return true;
}

void CTests::FinalReport()
{
    // Clear
    for (int i=0;i<10;i++)
        m_harnessTextOutput->AddLine(" ");
    m_harnessTextOutput->SetStatusText("Finished. "+ToString(eNumTests)+" tests taken "+ToString(m_testsFailed)+" failed.");
    for (int i=0;i<eNumTests;i++)
    {
       //m_harnessTextOutput->AddLine("Test: "+ToString(i+1)+" "+ m_testResults[i].report);
       m_harnessTextOutput->AddLine(m_testResults[i].report);
        m_harnessTextOutput->AddLine("Time taken: "+ToString(m_testResults[i].endTime-m_testResults[i].startTime)+" ms");
    }

    m_harnessTextOutput->AddLine("DONE");
}

void CTests::FlushMMOutput()
{
 // Check for MM output
    if (m_memoryMan)
    {
        bool done=false;
        while (!done)
        {
            const char* const output=m_memoryMan->GetTextLine();
            if (output==0)
                done=true;
            else
                m_mmTextOutput->AddLine(output);
        }
    }
}

void CTests::Start()
{
    #if defined(PAUSE_BETWEEN_TESTS)
    if (m_paused)
        m_paused=false;
    #endif

    if (m_currentTest>eFinished)
    {
       // m_harnessTextOutput->AddLine("Already running");
        return;
    }

    m_currentTest=eBasicTest;
    m_testPercentage=0;
    m_testsFailed=0;
    m_harnessTextOutput->SetStatusText("Tests Started");

    InitTestResult(m_currentTest);
}

void CTests::InitTestResult(ETests test)
{
    m_testResults[test].passed=false;
    m_testResults[test].startTime=SDL_GetTicks();
    m_testResults[test].endTime=0;
    m_testResults[test].report="Not run";
}

unsigned int CTests::GetRandomValue(unsigned int min,unsigned int max)
{
    // Note RAND_MAX is 32767
    float rnd=rand()/(float)RAND_MAX;

    float res=((max-min)*rnd)+min;

    return (unsigned int)res;
}

void CTests::CreateNewMM()
{
    assert(!m_memoryMan);
    m_memoryMan=CreateMemoryManagerInstance();
    m_mmTextOutput->SetStatusText("Created");
}

void CTests::ShutdownMM()
{
    assert(m_memoryMan);
    m_memoryMan->Shutdown();
    FlushMMOutput();
    delete m_memoryMan;
    m_memoryMan=0;
    m_mmTextOutput->SetStatusText("Destroyed");
    FlushMMOutput();
}

/*
    Allocate and delete a load of random blocks
*/
void CTests::BasicTest(int percentage)
{
    static unsigned char *preallocatedMemory=0;
    if (percentage==0)
    {
        CreateNewMM();
        bool result=m_memoryMan->Initialise(1024*1024*10);
        if (!result)
        {
            m_harnessTextOutput->AddLine("Failed to initialise");
            m_fails++;
        }
    }
    else if (percentage==50)
    {
        ShutdownMM();
        CreateNewMM();

        preallocatedMemory=new unsigned char[2048];
        memset(preallocatedMemory,0xcc,2048);

        bool result=m_memoryMan->Initialise(1024,preallocatedMemory);// 10 K
        if (!result)
        {
            m_harnessTextOutput->AddLine("Failed to initialise");
            m_fails++;
        }
        else
            m_passes++;
        return;

    }
    else if (percentage==100)
    {
        // Check no overflow
        if (preallocatedMemory[1024]!=0xcc)
        {
            m_harnessTextOutput->AddLine("Overwrote my buffer!");
            m_fails++;
        }
        else
            m_passes++;

        return;
    }

    size_t size=GetRandomValue(4,900);

    m_harnessTextOutput->AddLine("Attempting to allocate: "+ToString(size));

    void *pnter=m_memoryMan->Allocate(size,__FILE__,__LINE__);
    if (pnter==0)
    {
        m_harnessTextOutput->AddLine("Failed to allocate: "+ToString(size));
        m_fails++;
    }
    else
    {
       // Write to it
       memset(pnter,0xbb,size);
        // Release it
       m_memoryMan->Release(pnter);
       m_passes++;
    }

}

/*
    Create error conditions, should fail but not crash!
*/
void CTests::ErrorTest(int percentage)
{
    if (percentage==0)
    {
        CreateNewMM();
        bool result=m_memoryMan->Initialise(0); // stupid number - should fail!
        if (result)
        {
            m_fails++;
        }
        else
        {
            m_harnessTextOutput->AddLine("Failed to initialise. This is correct.");
            m_passes++;
        }
    }
    else
    if (percentage==1)
    {
        ShutdownMM();
        CreateNewMM();
        bool result=m_memoryMan->Initialise(1024*10);// 10 K
        if (!result)
        {
            m_harnessTextOutput->AddLine("Failed to initialise");
            m_fails++;
        }
        else
            m_passes++;
    }
    else
    if ((percentage>=2) && (percentage<=7)) // 0.5 - aligned memory tests 4,8,16,32,64,128
    {

        size_t alignment=pow(2,percentage);
        size_t size=GetRandomValue(7,500);

//        m_harnessTextOutput->AddLine("Alignment: "+ToString(alignment));

        void *pnter=m_memoryMan->AllocateAligned(size,alignment,__FILE__,__LINE__);
        if (pnter==0)
        {
            m_harnessTextOutput->AddLine("Failed to allocate: "+ToString(size));
            m_fails++;
        }
        else
        {
            // Write to it
            memset(pnter,0xbb,size);

            // Check alignment
            size_t remainder=((unsigned int)pnter)%alignment;
            if (remainder!=0)
            {
                m_harnessTextOutput->AddLine("Incorrectly Aligned: "+ToString(pnter));

                m_harnessTextOutput->AddLine("Alignment: "+ToString(alignment)+" remainder: "+ToString(remainder));

                m_fails++;
            }
            else
            {
                m_passes++;
                m_harnessTextOutput->AddLine("Correct Alignment: "+ToString(alignment));
            }

            m_memoryMan->ReleaseAligned(pnter);
        }
    }
    else
    {

        size_t size=3;
        void *pnter=m_memoryMan->Allocate(size,__FILE__,__LINE__);
        if (pnter==0)
        {
            m_harnessTextOutput->AddLine("Failed to allocate: "+ToString(size));
            m_fails++;
        }
        else
        {
            // Write to it
            memset(pnter,0xbb,size);

 //           m_harnessTextOutput->AddLine("Delete wrong address");
            m_memoryMan->Release((unsigned char*)pnter+7); // wrong address
            // Correct address
            m_memoryMan->Release(pnter);
   //         m_harnessTextOutput->AddLine("Double delete");
            // Double release
            m_memoryMan->Release(pnter);

            m_passes++;
        }
    }
}

/*
    This test checks to see if MM is merging free blocks correctly
*/
void CTests::MergeFreeBlockTest(int percentage)
{
    if (percentage==0)
    {
//        ShutdownMM();
        CreateNewMM();
        bool result=m_memoryMan->Initialise(19800);
        if (!result)
        {
            m_harnessTextOutput->AddLine("Failed to initialise");
            m_fails++;
        }
    }

    // Allocate 200 , 100 times = 20000, free 2 so total needed is 19800, last allocation is of 300
    // Remember Allocation 50 and 51 and delete both on 90 so on 99 should be enough room
    {
        static void *allPointers[100];

        size_t size=200;
        if (percentage==99)
            size=300; // MM will need to merge blocks to get this to work
        void *pnter=m_memoryMan->Allocate(size,__FILE__,__LINE__);
        allPointers[percentage]=pnter;
        if (pnter==0)
        {
            m_harnessTextOutput->AddLine("Failed to allocate: "+ToString(size));
            m_fails++;
        }
        else
        {
           // Write to it
           memset(pnter,0xbb,size);
           m_passes++;
        }

        if (percentage==90)
        {
            m_harnessTextOutput->AddLine("Deleting two blocks together");
            m_memoryMan->Release(allPointers[50]);
            m_memoryMan->Release(allPointers[51]);
        }

        // Now release all
        if (percentage==99)
        {
            m_harnessTextOutput->AddLine("Releasing Blocks, should be no leaks");
            for (int i=0;i<100;i++)
            {
                if (i!=50 && i!=51)
                    m_memoryMan->Release(allPointers[i]);
            }
        }
    }
}

/*
    Stress test
    0.5 - should leave two unfreed blocks
*/
void CTests::StressTest(int percentage)
{
    if (percentage==0)
    {
//        ShutdownMM();
        CreateNewMM();
        bool result=m_memoryMan->Initialise(1024*1024*64); // 64MB
        if (!result)
        {
            m_harnessTextOutput->AddLine("Failed to initialise");
            m_fails++;
        }
    }

    // Each time we allocate 1000 pointers and delete them
    static void *allPointers[1000];

    bool failed=false;
    for (int i=0;i<1000;i++)
    {
        size_t size=GetRandomValue(2,1800);
        void *pnter=m_memoryMan->Allocate(size,__FILE__,__LINE__);
        allPointers[i]=pnter;
        if (pnter==0)
        {
            m_harnessTextOutput->AddLine("Failed to allocate: "+ToString(size));
            failed=true;
        }
        else
        {
            // Write to it
            memset(pnter,0xbb,size);
        }
    }

    if (failed)
        m_fails++;
    else
        m_passes++;

    // Release all but 2 for leak testing when percentage is 95 (in v0.5)
    for (int i=0;i<1000;i++)
    {
        if (!(percentage==95 && (i==476 || i==854)))
            m_memoryMan->Release(allPointers[i]);
    }

    if (percentage==95)
        m_harnessTextOutput->AddLine("Should be two memory leaks");
}
