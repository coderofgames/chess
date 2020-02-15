#ifndef RENDER_TIMER_H
#define RENDER_TIMER_H
#include "wx/timer.h"
#include "NotationCanvas.h"


class BoardCanvas;

class RenderTimer : public wxTimer
{
    BoardCanvas* pane;
public:
    RenderTimer(BoardCanvas* pane);
    void Notify();
    void start();
};
#endif