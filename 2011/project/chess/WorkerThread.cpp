#include "WorkerThread.h"

#include "mdi.h"

// ----------------------------------------------------------------------------
// MyWorkerThread
// ----------------------------------------------------------------------------

// define this symbol to 1 to test if the YieldFor() call in the wxProgressDialog::Update
// function provokes a race condition in which the second wxThreadEvent posted by
// MyWorkerThread::Entry is processed by the YieldFor() call of wxProgressDialog::Update
// and results in the destruction of the progress dialog itself, resulting in a crash later.
#define TEST_YIELD_RACE_CONDITION       0

MyWorkerThread::MyWorkerThread(MyFrame *frame)
        : wxThread()
{
    m_frame = frame;
    m_count = 0;
}

void MyWorkerThread::OnExit()
{
}

wxThread::ExitCode MyWorkerThread::Entry()
{
#if TEST_YIELD_RACE_CONDITION
    if ( TestDestroy() )
        return NULL;

    wxThreadEvent event( wxEVT_COMMAND_THREAD, WORKER_EVENT );

    event.SetInt( 50 );
    wxQueueEvent( m_frame, event.Clone() );

    event.SetInt(-1);
    wxQueueEvent( m_frame, event.Clone() );
#else
	int currNum = 0;
    for ( m_count = 0; !m_frame->Cancelled() && (m_count < 100); m_count++ )
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;

        // create any type of command event here
        wxThreadEvent event( wxEVT_COMMAND_THREAD, MyFrame::WORKER_EVENT );
        //if( m_count / 10 > currNum )
		{
		//	currNum = m_count / 10;
			event.SetInt( m_count );

		}
        // send in a thread-safe way
        wxQueueEvent( m_frame, event.Clone() );

        wxMilliSleep(20);
    }

    wxThreadEvent event( wxEVT_COMMAND_THREAD, MyFrame::WORKER_EVENT );
    event.SetInt(-1); // that's all
    wxQueueEvent( m_frame, event.Clone() );
#endif

    return NULL;
}