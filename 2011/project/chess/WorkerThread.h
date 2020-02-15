#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include "headers.h"

class MyFrame;

// ----------------------------------------------------------------------------
// a worker thread
// ----------------------------------------------------------------------------

class MyWorkerThread : public wxThread
{
public:
    MyWorkerThread(MyFrame *frame);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit();

public:
    MyFrame *m_frame;
    unsigned m_count;
};

#endif