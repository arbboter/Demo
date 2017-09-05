#include <stdio.h>
#include "SThreadPool.h"
#include <time.h>
#include <cmath>

int CalcScore(void* pPara)
{
    int* pTaskId = (int*)pPara;
    
    Sleep(rand()%100);

    // 任务完成提示
    printf("\r[%-5d]task finished : %d", GetCurrentThreadId(), *pTaskId);

    delete pTaskId;
    pTaskId = NULL;

    return 0;
}


int main()
{
    CSThreadPool oPool(8);

    srand(time(NULL));
    if(oPool.Start())
    {
        string strName;
        int nNum = 0;
        while(1)
        {
            int* pTaskId = new int(nNum);
            oPool.AddTask(CalcScore, pTaskId);
            if(++nNum%10 == 0)
            {
                int nTask = oPool.GetCachedTaskNum();
                if(nTask > 500)
                {
                    printf("\n任务数量太多，休息片刻：%d\n", nTask);
                    Sleep(20000);
                }
                if(nTask < 500)
                {
                    Sleep(10);
                }
                else if(nTask < 1000)
                {
                    Sleep(100);
                }
                else
                {
                    Sleep(10000);
                }
            }
        }
    }
    
    return 0;
}
