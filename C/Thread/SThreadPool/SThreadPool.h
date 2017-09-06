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
    HANDLE              m_hTaskAppend; // 新任务添加事件,充当条件变量
    size_t              m_nMaxCacheTask; // 任务队列容量值

protected:
    int PopTask(STTaskInfo& oTask);
    static unsigned __stdcall RunThread(void* pPara);

public:
    // 启动线程池
    bool Start();
    // 暴力直接退出，不建议使用
    bool Stop();
    // 等待所有任务执行完毕，再退出
    bool WaitAll();
    // 添加任务至线程池（当队列中任务数太多时，添加任务失败，返回-1）
    int  AddTask(pTaskPro TaskPro, void* pTaskPara);
    // 获取当前缓冲队列中排队任务数
    int  GetCachedTaskNum();
    // 判断线程池是否启动
    bool IsRun();
};
