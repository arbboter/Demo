#include "SThreadPool.h"
#include <iostream>
#include <process.h>


/** CSThreadPool方法 **/
CSThreadPool::CSThreadPool(int nThrNum)
{
    m_nThrNum = nThrNum;
    m_bInit = false;
    InitializeCriticalSection(&m_csTask);
}


CSThreadPool::~CSThreadPool(void)
{
    DeleteCriticalSection(&m_csTask);
}

int CSThreadPool::GetCachedTaskNum()
{
    int nRet = 0;
    if (m_dqTask.size() > 0)
    {
        EnterCriticalSection(&m_csTask);
        nRet = m_dqTask.size();
        LeaveCriticalSection(&m_csTask);
    }
    return nRet;
}

bool CSThreadPool::IsRun()
{
    return m_bInit;
}

int CSThreadPool::PopTask(STTaskInfo& oTask)
{
    int nRet = -1;
    if(m_bInit)
    {
        // 轮询获取任务
        while(nRet != 0)
        {
            if(m_dqTask.size() > 0)
            {
                EnterCriticalSection(&m_csTask);
                if (m_dqTask.size() > 0)
                {
                    oTask = m_dqTask.back();
                    m_dqTask.pop_back();
                    nRet = 0;
                }
                LeaveCriticalSection(&m_csTask);
            }
            else
            {
                //printf("[%-5d] no more task。\n", GetCurrentThreadId());
                WaitForSingleObject(m_hTaskAppend, INFINITE);
                // Sleep(1000);
            }
        }
    }
    return nRet;
}

unsigned __stdcall CSThreadPool::RunThread(void* pPara)
{
    int nRet = 0;
    CSThreadPool* pThis = (CSThreadPool*)pPara;

    STTaskInfo oTask;
    while(pThis->m_bInit)
    {
        // 同步获取任务
        if(pThis->PopTask(oTask) == 0)
        {
            oTask.RunTask(oTask.pPara);
        }
    }

    return 0;
}

bool CSThreadPool::Start()
{
    if(m_bInit)
    {
        return m_bInit;
    }

    // 修正线程数
    if(m_nThrNum < 0)
    {
        m_nThrNum = 8;
    }

    // 初始化参数
    m_bInit = true;
    m_hTaskAppend = CreateEvent(NULL, FALSE, FALSE, NULL);

    HANDLE hCurHandle = NULL;
    for (int i=0; i<m_nThrNum; i++)
    {
        hCurHandle = (HANDLE)_beginthreadex(NULL, 0, RunThread, this, 0, NULL);
        m_vecThrHandle.push_back(hCurHandle);
    }

    return m_bInit;
}

bool CSThreadPool::Stop()
{
    if(m_bInit)
    {
        m_bInit = false;
        for(size_t i=0; i<m_vecThrHandle.size(); i++)
        {
            WaitForSingleObject(m_vecThrHandle[i], INFINITE);
        }
        m_vecThrHandle.clear();
        m_dqTask.clear();
        CloseHandle(m_hTaskAppend);
        m_hTaskAppend = INVALID_HANDLE_VALUE;
    }
    return m_bInit;
}

void CSThreadPool::AddTask(pTaskPro TaskPro, void* pTaskPara)
{
    if(m_bInit)
    {
        STTaskInfo oTask;
        oTask.RunTask = TaskPro;
        oTask.pPara = pTaskPara;
        EnterCriticalSection(&m_csTask);
        m_dqTask.push_front(oTask);
        SetEvent(m_hTaskAppend);
        LeaveCriticalSection(&m_csTask);
    }
}
