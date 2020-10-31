#include"XThreadPool.hpp"
#include"XThread.hpp"
#include<time.h>
#include<stdlib.h>
#include<iostream>
#include<unistd.h>
using namespace std;

void XThreadPool::Init(int threadCount)
{
    this->thread_Count_=threadCount;
    last_Thread_=-1;
    for(int i=0;i<thread_Conut_;++i)
    {
        XThread*t=new XThread();
        t->id=i+1;
        t->Start();
        threads_.push_back(t);
        usleep(1000);
    }
}

void XThreadPool::Dispatch(XTask*task)
{
    if(!task) return;
    int tid=(last_Thread_+1)%thread_Conut_;
    last_Thread_=tid;
    XThread*t=threads_[tid];
    t->AddTask(task);
}