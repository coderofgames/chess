#ifndef __CLOCKTHREAD_H__
#define __CLOCKTHREAD_H__

#include "headers.h"
#include "lcdwindow.h"




class ClockTimer : public wxTimer
{
    wxLCDWindow* mClock;
public:
    ClockTimer(wxLCDWindow* p);
    void Notify();
    void start();
	
};

#endif // __CLOCKTHREAD_H__
