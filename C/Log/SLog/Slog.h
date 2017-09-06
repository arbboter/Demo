#ifndef __SLOG_H_20161031__
#define __SLOG_H_20161031__

#include <string>
#include <vector>
#include <stdarg.h>
#include <Windows.h>

using namespace std;

class CSlog
{
public:
    enum LV
    {
        LV_DEBUG    = 0,
        LV_INFO     = 1,
        LV_WARN     = 2,
        LV_ERROR    = 3,
        LV_FATAL    = 4,

        LV_MAX,
    };

    enum FILE_MODE
    {
        FM_SELF,        // ��־�ļ����洢����ȼ�����־
        FM_TREE,        // ��־�ļ��洢����ȼ������ϵ���־
        FM_ALL,         // ������־���洢����ǰ�趨����־�ȼ��ļ���
    };

    enum DATE_FMT
    {
        DATE_FMT_LOG_MSEC,
        DATE_FMT_LOG_SEC,
        DATE_FMT_DATE,
    };
    
    // ��־��Ϣ
    typedef struct _log_info_
    {
        unsigned int    nTid;           // �߳�ID
        unsigned int    nPid;           // ����ID
        unsigned int    nLine;          // �к�
        int             nLogLv;         // ��־�Ǽ�
        string          strFile;        // �ļ���
        string          strFunc;        // ������
        string          strTag;         // ��־�ȼ���ǩ��
        string          strMsg;         // ��־��Ϣ
        string          strDateTime;    // ��־ʱ��
    }LogInfo;

public:
    // ��·����ȡ�ļ���
    string GetFileName(const string& strPath);
    // �ļ��Ƿ����
    bool FileExist(const string& strFile);
    // ��ȡ�ļ���С
    size_t GetFileSize(const string& strFile);
    // �����༶Ŀ¼
    int MakeMultiPath(const string& strPath);
    // ��ȡ·����Ԫ���ָ�·��
    int GetPathUnit(const string& strPath, vector<string>& vecPath);
    // ��ʽ���ַ�����string
    string Formate(const char * pFmt, ...);
    string Formate(const char * pFmt, va_list va);

    // �ַ����ָ� 
    static int Split(const string& strSrc, const string& strDim, vector<string>& vecItems);
    // ��ȡ��ǰʱ��
    static string GetDateTime(const DATE_FMT& fmt);

public:
    // �����ṩ�Ĳ�����������
    void    SetCfg(const LV& lv, const string& strName, const string& strPath="");
    void    SetFileMode(const FILE_MODE& fm);
    void    SetFileRoll(const size_t& nMaxFileSize, const int& nMaxFileNum);
    static  CSlog*  Inst();

public:
    // ������־��Ϣ
    void    LogFormate(LogInfo& oInfo, const LV& lv, const int nLine, const char* pFunc, const char* pFile, const char* pFmt, ...);
    string  BuildInfo(const LogInfo& oInfo);
    string  BuildHeader(const string& strTag);
    // д��־
    size_t  WriteLog(const LV& lv, const string& strLog);

protected:
    // ��־����
    bool    RollFile(const LV& lv, int nStart=0);
    // ��ȡд���ļ�
    FILE*   ObtainFile(const LV& lv);
    // ��ȡ��־�ļ���
    string GetName(const LV& lv, int nIdx);
    int    CloseFile(const LV& lv);
    void   Lock(int nLv);
    void   Unlock(int nLv);


private:
    CSlog()
    {
        for (int i=0; i<=LV_MAX; i++)
        {
            m_fpLog[i] = NULL;
            InitializeCriticalSection(&m_csFile[i]);
        }

        Init();
        m_fmLog = FM_SELF;
        m_bEnableTS = true;
    }

    ~CSlog(void)
    {
        Uninit();

        for (int i=0; i<=LV_MAX; i++)
        {
            m_fpLog[i] = NULL;
            DeleteCriticalSection(&m_csFile[i]);
        }
    }

    bool Init();
    bool Uninit();

private:
    static CSlog*  m_pInst;     // ��־���

protected:
    CRITICAL_SECTION    m_csFile[LV_MAX+1];         // ��־�ļ���
    FILE*               m_fpLog[LV_MAX+1];          // ��־�ļ����
    LV                  m_lvLog;                    // ��־�ȼ�
    string              m_strLogPath;               // ��־�ļ�·��
    string              m_strAppName;               // ��־�������ƣ���־�ļ����а������ֶ�
    size_t              m_nMaxFileSize;             // ������־�ļ����ֵ
    int                 m_nMaxFileNum;              // ������־�ļ������
    FILE_MODE           m_fmLog;                    // ��־�ļ�ģʽ
    bool                m_bEnableTS;                // �Ƿ������̰߳�ȫ
};

#define SLogInst        (CSlog::Inst())


#define LOGLv(lv,fmt, ...)  do{\
                                CSlog::LogInfo __LOGLv_lg__;\
                                string __LOGLv_strLog__;\
                                SLogInst->LogFormate(__LOGLv_lg__, lv, __LINE__, __FUNCTION__, __FILE__, fmt, ##__VA_ARGS__);\
                                __LOGLv_strLog__ = SLogInst->BuildInfo(__LOGLv_lg__);\
                                SLogInst->WriteLog(lv, __LOGLv_strLog__);\
                            }while(0);

#define LOGLvH(lv,tag)      do{\
                                string __LOGLv_strLog__;\
                                __LOGLv_strLog__ = SLogInst->BuildHeader(tag);\
                                SLogInst->WriteLog(lv, __LOGLv_strLog__);\
                             }while(0);

// ��ʽ����־�ӿ�
#define LOGD(fmt,...)   LOGLv(CSlog::LV_DEBUG,fmt,  ##__VA_ARGS__)
#define LOGI(fmt,...)   LOGLv(CSlog::LV_INFO ,fmt,  ##__VA_ARGS__)
#define LOGW(fmt,...)   LOGLv(CSlog::LV_WARN ,fmt,  ##__VA_ARGS__)
#define LOGE(fmt,...)   LOGLv(CSlog::LV_ERROR,fmt,  ##__VA_ARGS__)
#define LOGF(fmt,...)   LOGLv(CSlog::LV_FATAL,fmt,  ##__VA_ARGS__)

// ��־�ֶα�ǩ��Ĭ�ϱ�ǩ��ǰ������
#define LOGDH()   LOGLvH(CSlog::LV_DEBUG,__FUNCTION__)
#define LOGIH()   LOGLvH(CSlog::LV_INFO ,__FUNCTION__)
#define LOGWH()   LOGLvH(CSlog::LV_WARN ,__FUNCTION__)
#define LOGEH()   LOGLvH(CSlog::LV_ERROR,__FUNCTION__)
#define LOGFH()   LOGLvH(CSlog::LV_FATAL,__FUNCTION__)

// ��־�ֶα�ǩ
#define LOGDHT(tag)   LOGLvH(CSlog::LV_DEBUG,tag)
#define LOGIHT(tag)   LOGLvH(CSlog::LV_INFO ,tag)
#define LOGWHT(tag)   LOGLvH(CSlog::LV_WARN ,tag)
#define LOGEHT(tag)   LOGLvH(CSlog::LV_ERROR,tag)
#define LOGFHT(tag)   LOGLvH(CSlog::LV_FATAL,tag)

#endif
