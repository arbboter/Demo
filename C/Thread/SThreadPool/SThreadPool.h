#pragma once
#include <Windows.h>
#include <vector>
#include <deque>
#include <utility>

using namespace std;

// 抽象出的任务函数
typedef int ( *pTaskPro)(void* pPara);

class CSThreadPool
{
public:
    CSThreadPool(int nThrNum);
    ~CSThreadPool(void);

    /* 任务信息 */
    typedef struct __task_info__
    {
        pTaskPro    RunTask;
        void*       pPara;
    }STTaskInfo;

protected:
    bool                m_bInit;
    int                 m_nThrNum;
    vector<HANDLE>      m_vecThrHandle;
    deque<STTaskInfo>   m_dqTask;
    CRITICAL_SECTION    m_csTask;
    HANDLE              m_hTaskAppend; // 新任务添加事件

protected:
    int PopTask(STTaskInfo& oTask);
    static unsigned __stdcall RunThread(void* pPara);

public:
    bool Start();
    bool Stop();
    void AddTask(pTaskPro TaskPro, void* pTaskPara);
    int  GetCachedTaskNum();
    bool IsRun();
};
