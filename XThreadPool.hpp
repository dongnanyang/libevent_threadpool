#ifndef _XTHREAD_POOL_H_
#define _XTHREAD_POOL_H_
#include<vector>
#include<thread>
#include"XTask.h"
class XThreadPool
{
    public:
        XThreadPool()
        {

        }
    public:
        virtual ~XThreadPool()
        {

        }
    public:
        static XThreadPool*GetInstance()
        {
            static XThreadPool xc;
            return &xc;
        }
        void Init(int threadCount);
        void Dispatch(XTask*task);
    private:
        int thread_Conut_=0;
        int last_Thread_=-1;
        std::vector<XThread*>threads_;
};
#endif