#pragma once
#include <Windows.h>
#include <vector>
#include <deque>
#include <utility>

using namespace std;

// �������������
typedef int ( *pTaskPro)(void* pPara);

class CSThreadPool
{
public:
    CSThreadPool(int nThrNum);
    ~CSThreadPool(void);

    /* ������Ϣ */
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
    HANDLE              m_hTaskAppend; // ����������¼�,�䵱��������
    size_t              m_nMaxCacheTask; // �����������ֵ

protected:
    int PopTask(STTaskInfo& oTask);
    static unsigned __stdcall RunThread(void* pPara);

public:
    // �����̳߳�
    bool Start();
    // ����ֱ���˳���������ʹ��
    bool Stop();
    // �ȴ���������ִ����ϣ����˳�
    bool WaitAll();
    // ����������̳߳أ���������������̫��ʱ���������ʧ�ܣ�����-1��
    int  AddTask(pTaskPro TaskPro, void* pTaskPara);
    // ��ȡ��ǰ����������Ŷ�������
    int  GetCachedTaskNum();
    // �ж��̳߳��Ƿ�����
    bool IsRun();
};
