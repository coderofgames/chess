#ifndef APP_H
#define APP_H
// -- application --
#include "headers.h"

class MyApp : public wxApp
{
public:
    bool OnInit();

    // critical section protects access to all of the fields below
    wxCriticalSection m_critsect;

    // all the threads currently alive - as soon as the thread terminates, it's
    // removed from the array
    wxArrayThread m_threads;

    // semaphore used to wait for the threads to exit, see MyFrame::OnQuit()
    wxSemaphore m_semAllDone;

    // indicates that we're shutting down and all threads should exit
    bool m_shuttingDown;
};


DECLARE_APP(MyApp)
#endif