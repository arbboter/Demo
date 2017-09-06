#include "SThreadPool.h"
#include <iostream>
#include <process.h>


/** CSThreadPool���� **/
CSThreadPool::CSThreadPool(int nThrNum)
{
    m_nThrNum = nThrNum;
    m_bInit = false;
    InitializeCriticalSection(&m_csTask);

    // ������������Ĭ��ֵ
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
    // ��ѯ��ȡ����
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

            // ����ȡ�����Ƿ�ɹ�
            if (nRet == 0)
            {
                break;
            }
        }
        else
        {
            // �����̳߳�ʱ��Ϊ��ֹ�˴�һֱ�ȴ�����Ҫ�����¼�
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
        // ͬ����ȡ����
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

    // �����߳���
    if (m_nThrNum < 0)
    {
        m_nThrNum = 8;
    }

    // ��ʼ������
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
            // �����������������ִ�У����⴦�����޵ȴ����ܽ���
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
    // �ȴ�������������ʱ������ֹ�̳߳��е��߳���
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
