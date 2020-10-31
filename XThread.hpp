#ifndef _XTHREAD_H_
#define _XTHREAD_H_
#include<event2/evnet.h>
#include<list>
#include<mutex>
#include<XTask.h>
class XThread
{
public:
    XThred();
public:
    virtual~XThread();
public:
    void Notify(int fd,short which);
    void Start();
    void Main();
    bool Setup();
    void Activatie();
    void AddTask(XTask*t);
    int id=0;
private:
    int notify_send_fd_=0;
    struct event_base*base_=0;
    std::list<XTask*>tasks_;
    std::mutex tasks_mutex_;
};
#endif