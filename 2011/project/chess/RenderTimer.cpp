#include "RenderTimer.h"
#include "BoardCanvas.h"

RenderTimer::RenderTimer(BoardCanvas* pane) : wxTimer()
{
    RenderTimer::pane = pane;
}
 
void RenderTimer::Notify()
{
    //pane->Refresh(true);
	//if( pane->gameState == BoardCanvas::GameState::SELECTED_PIECE )
	if( pane->needsRefresh )
		pane->Refresh();
	//pane->Notified = true;
}
 
void RenderTimer::start()
{
    wxTimer::Start(20);
}