// for compilers that support precompilation, includes "wx/wx.h".
#ifndef LISTBOX_H
#define LISTBOX_H

#include "wx/wxprec.h"
#include "headers.h"

// not all ports have support for EVT_CONTEXT_MENU yet, don't define
// USE_CONTEXT_MENU for those which don't
#if defined(__WXMOTIF__) || defined(__WXPM__) || defined(__WXX11__) || defined(__WXMGL__)
    #define USE_CONTEXT_MENU 0
#else
    #define USE_CONTEXT_MENU 1
#endif

class MyFrame;

class MyListCtrl: public wxListCtrl
{
public:
		
	int ColToSort;
	bool colSortToggle[64];
	int sortMode;

    MyListCtrl(wxWindow *parent, MyFrame* theFrame,
               const wxWindowID id,
               const wxPoint& pos,
               const wxSize& size,
               long style)
        : wxListCtrl(parent, id, pos, size, style),
          m_attr(*wxBLUE, *wxLIGHT_GREY, wxNullFont)
        {
            m_updated = -1;
			//SetScrollbars(1,1, w, h*2, 0, 0);
			//this->SetBackgroundColour(*wxStockGDI::GetColour(wxStockGDI::COLOUR_LIGHTGREY));
			//this->SetForegroundColour(*wxLIGHT_GREY);
			//this->setB
			//this->set
			ColToSort = 0;
			sortMode = 0;
			for( int i = 0; i < 64; i++ )
				colSortToggle[i] = true;
			m_frame = theFrame;

#ifdef __POCKETPC__
            EnableContextMenu();
#endif
        }

    // add one item to the listctrl in report mode
    void InsertItemInReportView(int i);

	void InsertItemInReportView(int i, wxString str1, wxString str2, wxString str3, wxString str4,wxString str5, wxString str6="");
	//void InitWithVirtualItems();

    void OnColClick(wxListEvent& event);
    void OnColRightClick(wxListEvent& event);
    void OnColBeginDrag(wxListEvent& event);
    void OnColDragging(wxListEvent& event);
    void OnColEndDrag(wxListEvent& event);
    void OnBeginDrag(wxListEvent& event);
    void OnBeginRDrag(wxListEvent& event);
    void OnBeginLabelEdit(wxListEvent& event);
    void OnEndLabelEdit(wxListEvent& event);
    void OnDeleteItem(wxListEvent& event);
    void OnDeleteAllItems(wxListEvent& event);
    void OnSelected(wxListEvent& event);
    void OnDeselected(wxListEvent& event);
    void OnListKeyDown(wxListEvent& event);
    void OnActivated(wxListEvent& event);
    void OnFocused(wxListEvent& event);
    void OnCacheHint(wxListEvent& event);

    void OnChar(wxKeyEvent& event);

#if USE_CONTEXT_MENU
    void OnContextMenu(wxContextMenuEvent& event);
#endif

    void OnRightClick(wxMouseEvent& event);

private:
    void ShowContextMenu(const wxPoint& pos);
   // wxLog *m_logOld;
    void SetColumnImage(int col, int image);

    void LogEvent(const wxListEvent& event, const wxChar *eventName);
    void LogColEvent(const wxListEvent& event, const wxChar *eventName);

    virtual wxString OnGetItemText(long item, long column) const;
    virtual int OnGetItemColumnImage(long item, long column) const;
    virtual wxListItemAttr *OnGetItemAttr(long item) const;

    wxListItemAttr m_attr;
	MyFrame *m_frame;
    long m_updated;



    //wxDECLARE_NO_COPY_CLASS(MyListCtrl);
    DECLARE_EVENT_TABLE()
};


enum
{
    LIST_ABOUT = wxID_ABOUT,
    LIST_QUIT = wxID_EXIT,

    LIST_LIST_VIEW = wxID_HIGHEST+1500,
    LIST_ICON_VIEW,
    LIST_ICON_TEXT_VIEW,
    LIST_SMALL_ICON_VIEW,
    LIST_SMALL_ICON_TEXT_VIEW,
    LIST_REPORT_VIEW,
    LIST_VIRTUAL_VIEW,
    LIST_SMALL_VIRTUAL_VIEW,
    LIST_SET_ITEMS_COUNT,

    LIST_DESELECT_ALL,
    LIST_SELECT_ALL,
    LIST_DELETE_ALL,
    LIST_DELETE,
    LIST_ADD,
    LIST_EDIT,
    LIST_SORT,
    LIST_FIND,
    LIST_SET_FG_COL,
    LIST_SET_BG_COL,
    LIST_TOGGLE_MULTI_SEL,
    LIST_TOGGLE_HEADER,
    LIST_TOGGLE_FIRST,
    LIST_SHOW_COL_INFO,
    LIST_SHOW_SEL_INFO,
    LIST_SHOW_VIEW_RECT,
#ifdef wxHAS_LISTCTRL_COLUMN_ORDER
    LIST_SET_COL_ORDER,
    LIST_GET_COL_ORDER,
#endif // wxHAS_LISTCTRL_COLUMN_ORDER
    LIST_GOTO,
    LIST_FOCUS_LAST,
    LIST_FREEZE,
    LIST_THAW,
    LIST_TOGGLE_LINES,
#ifdef __WXOSX__
    LIST_MAC_USE_GENERIC,
#endif

    LIST_CTRL                   = 1000
};






#endif