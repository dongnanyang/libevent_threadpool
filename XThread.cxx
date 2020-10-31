#include"XThread.h"
#include<thread>
#include<iostream>
#include<event2/evnet.h>
#include<unistd.h>
using namespace std;

static void NotifyCB(int fd,short which,void*arg)
{
    XThread*t=(XThread*)arg;
    t->Notify(fd,which);
}

void XThread::Notify(int fd,short which)
{
    char buf[2]={0};
    int re=read(fd,buf,1);
    if(re<=0)
        return;
    cout<<id<<"thread"<<buf<<endl;
    tasks_mutex_.lock();
    if(tasks_.empty())
    {
        tasks_mutex_.unlock();
        return;
    }
    auto task=tasks_.front();
    tasks_.pop_front();
    tasks_mutex_.unlock();
    task->Init();
}

XThread::XThread()
{

}

XThread::~XThread()
{

}

void XThread::Start()
{
    Setup();
    thread th(&XThread::Main,this);

    th.detach();
}

void XThread::Main()
{
    cout<<id<<"XThread::Main() begin"<<endl;
    event_base_dispatch(base_);
    event_base_free(base_);
    cout<<id<<"XThread::Main() end"<<endl;
}

bool XThread::Setup()
{
    int fds[2];
    if(pipe(fds))
    {
        cerr<<"pipe failed!"<<endl;
        return false;
    }
    notify_send_fd_=fds[1];
    
    event_config*ev_config=evnet_config_new();
    event_config_set_flag(ev_config,EVENT_BASE_FLAG_NOLOCK);
    this->base_=event_base_new_with_config(ev_config);
    event_config_free(ev_config);
    if(!base_)
    {
        cerr<<"event_base_new_with_config failed in thread!"<<endl;
        return false;
    }

    event*ev=event_new(base_,fds[0],EV_READ|EV_PERSIST,NotifyCB,this);
    event_add(ev,0);
    return true;
}

void XThread::Activate()
{
    int re=write(notify_send_fd_,"c",1);
    if(re<=0)
    {
        return;
    }
}

void XThread::AddTask(XTask*t)
{
    if(!t)return;
    t->set_base(this->base_);
    tasks_mutex_.lock();
    tasks_.push_back(t);
    tasks_mutex_.unlock();
}