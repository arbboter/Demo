#include "SThreadPool.h"
#include <iostream>
#include <process.h>


/** CSThreadPool方法 **/
CSThreadPool::CSThreadPool(int nThrNum)
{
    m_nThrNum = nThrNum;
    m_bInit = false;
    InitializeCriticalSection(&m_csTask);

    // 设置任务容量默认值
    m_nMaxCacheTask = 40960;
}


CSThreadPool::~CSThreadPool(void)
{
    DeleteCriticalSection(&m_csTask);
}

int CSThreadPool::GetCachedTaskNum()
{
    int nRet = 0;

    EnterCriticalSection(&m_csTask);
    nRet = m_dqTask.size();
    LeaveCriticalSection(&m_csTask);

    return nRet;
}

bool CSThreadPool::IsRun()
{
    return m_bInit;
}

int CSThreadPool::PopTask(STTaskInfo& oTask)
{
    int nRet = -1;
    // 轮询获取任务
    while (m_bInit)
    {
        if (m_dqTask.size() > 0)
        {
            EnterCriticalSection(&m_csTask);
            if (m_dqTask.size() > 0)
            {
                oTask = m_dqTask.back();
                m_dqTask.pop_back();
                nRet = 0;
            }
            LeaveCriticalSection(&m_csTask);

            // 检查获取任务是否成功
            if (nRet == 0)
            {
                break;
            }
        }
        else
        {
            // 结束线程池时，为防止此处一直等待，需要重置事件
            WaitForSingleObject(m_hTaskAppend, INFINITE);
        }
    }
    return nRet;
}

unsigned __stdcall CSThreadPool::RunThread(void* pPara)
{
    int nRet = 0;
    CSThreadPool* pThis = (CSThreadPool*)pPara;

    STTaskInfo oTask;
    while (pThis->m_bInit)
    {
        // 同步获取任务
        if (pThis->PopTask(oTask) == 0)
        {
            oTask.RunTask(oTask.pPara);
        }
    }

    return 0;
}

bool CSThreadPool::Start()
{
    if (m_bInit)
    {
        return m_bInit;
    }

    // 修正线程数
    if (m_nThrNum < 0)
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
    if (m_bInit)
    {
        m_bInit = false;
        HANDLE* pHandle = new HANDLE[m_vecThrHandle.size()];
        for(size_t i=0; i<m_vecThrHandle.size(); i++)
        {
            pHandle[i] = m_vecThrHandle[i];
            // 触发阻塞的任务继续执行，以免处于无限等待不能结束
            SetEvent(m_hTaskAppend);
        }
        WaitForMultipleObjects(m_vecThrHandle.size(), pHandle, TRUE, INFINITE);

        delete[] pHandle;
        pHandle = NULL;

        m_vecThrHandle.clear();
        m_dqTask.clear();
        CloseHandle(m_hTaskAppend);
        m_hTaskAppend = INVALID_HANDLE_VALUE;
    }
    return m_bInit;
}

bool CSThreadPool::WaitAll()
{
    // 等待队列中无任务时才能终止线程池中的线程数
    while (GetCachedTaskNum() > 0)
    {
        Sleep(1000);
    }

    return Stop();
}

int CSThreadPool::AddTask(pTaskPro TaskPro, void* pTaskPara)
{
    int nRet = -1;
    if (m_bInit)
    {
        STTaskInfo oTask;
        oTask.RunTask = TaskPro;
        oTask.pPara = pTaskPara;
        EnterCriticalSection(&m_csTask);
        if (m_dqTask.size() < m_nMaxCacheTask)
        {
            m_dqTask.push_front(oTask);
            nRet = 0;
        }
        SetEvent(m_hTaskAppend);
        LeaveCriticalSection(&m_csTask);
    }
    return nRet;
}
