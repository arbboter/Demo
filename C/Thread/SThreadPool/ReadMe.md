## Windows 简易线程池 ##
* 2017年9月5日 版本说明：通过简单的线程池管理类CSThreadPool以及线程任务指针函数实现
```cpp
// 使用示例
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
    
    if(oPool.Start())
    {
        string strName;
        int nNum = 0;
        while(++nNum < 100)
        {
            int* pTaskId = new int(nNum);
            oPool.AddTask(CalcScore, pTaskId);
            Sleep(10);
        }
        oPool.Stop();
    }
    return 0;
}
```

* 2017年9月6日 
> 1.优化结构设计
> 2.添加说明
> 3.修复已知问题

