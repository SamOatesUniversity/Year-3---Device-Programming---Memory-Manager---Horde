#ifndef CTESTS_H
#define CTESTS_H

/*
    Manages the tests and results

    To do:

    - run with a provided pointer
    - check on aligned data

*/

#include <string>

class CTextOutput;
class IMemoryManager;

struct TTestResults
{
    bool passed;
    std::string report;
    unsigned int startTime;
    unsigned int endTime;
};

enum ETests
{
    eNotStarted=-2,
    eFinished,
    eBasicTest,             // Random allocation and deletion of memory
    eErrorTest,             // Does the MM handle error conditions e.g. deleting the wrong pointer
    eMergeFreeBlockTest,    // MM will only pass this if it can merge free blocks
    eStressTest,            // Stress and timing tests
    eNumTests,
};

// Test manager
class CTests
{
    private:
        CTextOutput *m_harnessTextOutput;
        CTextOutput *m_mmTextOutput;
        TTestResults *m_testResults;
        ETests m_currentTest;
        IMemoryManager *m_memoryMan;
        int m_testPercentage;
        int m_fails,m_passes;
        int m_testsFailed;
        void BasicTest(int percentage);
        void ErrorTest(int percentage);
        void MergeFreeBlockTest(int percentage);
        void StressTest(int percentage);
        unsigned int GetRandomValue(unsigned int min,unsigned int max);
        void CreateNewMM();
        void ShutdownMM();
        void FlushMMOutput();
        void InitTestResult(ETests test);
        void FinalReport();
        std::string GetTestDescription(ETests test);
    public:
        CTests(CTextOutput *harnessTextOutput,CTextOutput *mmTextOutput);
        ~CTests();
        void Start();
        bool Update();
};

#endif // CTESTS_H
