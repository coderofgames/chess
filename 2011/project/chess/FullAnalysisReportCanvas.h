#ifndef FULL_ANALYSIS_REPORT_CANVAS_H
#define FULL_ANALYSIS_REPORT_CANVAS_H

#include "headers.h"
#include <vector>

using std::vector;

class PGN::ResultCache;

class FullAnalysisReportCanvas : public wxScrolledWindow
{
public:

	FullAnalysisReportCanvas(MyFrame *parent);
	~FullAnalysisReportCanvas();


	void OnPaint(wxPaintEvent &event);






	MyFrame *m_frame;

	wxFont m_font;
	wxFont m_font2;

	wxBitmap m_bmp;




	DECLARE_EVENT_TABLE()
};

#endif