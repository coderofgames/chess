#include "ClockThread.h"
#include "lcdwindow.h"



ClockTimer::ClockTimer(wxLCDWindow *clock) : wxTimer()
{
	mClock = clock;
}

void ClockTimer::Notify()
{
	static wxString now;
	if( now != ::wxNow() )
	{
		now = ::wxNow();
		mClock->SetValue( now.Mid( 11, 8 ) );
	}


}

void ClockTimer::start()
{
	wxTimer::Start(100);
}