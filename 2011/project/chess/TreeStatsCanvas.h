#ifndef TREE_STATS_CANVAS_H
#define TREE_STATS_CANVAS_H

#include "headers.h"
#include <vector>

using std::vector;

class PGN::ResultCache;

class TreeStatsCanvas : public wxScrolledWindow
{
public:

	TreeStatsCanvas(MyFrame *parent);
	~TreeStatsCanvas();


	void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);


	void OnMouseMove(wxMouseEvent &event){}
	void OnMouseLeftClick(wxMouseEvent &event);

	void SortResultCaches();

	MyFrame *m_frame;

	wxFont m_font;
	wxFont m_font2;

	wxBitmap m_bmp;

	vector< PGN::ResultCache* > vecResults;
	vector< PGN::ResultCacheExtraStats* > vecResultsExtraStats;

	bool m_bUsingExtraStats;

	DECLARE_EVENT_TABLE()
};

#endif