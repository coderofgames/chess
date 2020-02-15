#ifndef MY_LIST_PANEL_H
#define MY_LIST_PANEL_H

#include "headers.h"


class MyFrame;
class MyListCtrl;

class MyListPanel : public wxPanel
{
public:

	MyListPanel(MyFrame *parent);
	~MyListPanel();





    void OnListView(wxCommandEvent& event);
    void OnReportView(wxCommandEvent& event);
    void OnIconView(wxCommandEvent& event);
    void OnIconTextView(wxCommandEvent& event);
    void OnSmallIconView(wxCommandEvent& event);
    void OnSmallIconTextView(wxCommandEvent& event);
    void OnVirtualView(wxCommandEvent& event);
    void OnSmallVirtualView(wxCommandEvent& event);

    void OnSetItemsCount(wxCommandEvent& event);


    void OnGoTo(wxCommandEvent& event);
    void OnFocusLast(wxCommandEvent& event);
    void OnToggleFirstSel(wxCommandEvent& event);
    void OnDeselectAll(wxCommandEvent& event);
    void OnSelectAll(wxCommandEvent& event);
    void OnAdd(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnDeleteAll(wxCommandEvent& event);
    void OnSort(wxCommandEvent& event);
    void OnSetFgColour(wxCommandEvent& event);
    void OnSetBgColour(wxCommandEvent& event);
    void OnToggleMultiSel(wxCommandEvent& event);
    void OnShowColInfo(wxCommandEvent& event);
    void OnShowSelInfo(wxCommandEvent& event);
    void OnShowViewRect(wxCommandEvent& event);
#ifdef wxHAS_LISTCTRL_COLUMN_ORDER
    void OnSetColOrder(wxCommandEvent& event);
    void OnGetColOrder(wxCommandEvent& event);
#endif // wxHAS_LISTCTRL_COLUMN_ORDER
    void OnFreeze(wxCommandEvent& event);
    void OnThaw(wxCommandEvent& event);
    void OnToggleLines(wxCommandEvent& event);
    void OnToggleHeader(wxCommandEvent& event);
#ifdef __WXOSX__
    void OnToggleMacUseGeneric(wxCommandEvent& event);
#endif // __WXOSX__
    void OnFind(wxCommandEvent& event);

    void OnUpdateUIEnableInReport(wxUpdateUIEvent& event);
    void OnUpdateToggleMultiSel(wxUpdateUIEvent& event);
    void OnUpdateToggleHeader(wxUpdateUIEvent& event);

	bool CheckNonVirtual() const;
	void InitWithListItems();
	void RecreateList(long flags, bool withText = true);

	void InitWithVirtualItems();
	void InitWithIconItems(bool withText, bool sameIcon);
	void InitWithReportItems(bool pgnLoaded);

	void DoSize();


    // number of items to initialize list/report view with
    int m_numListItems;
	bool m_smallVirtual;
	wxImageList *m_imageListNormal;
    wxImageList *m_imageListSmall;

	MyFrame *m_parent;
	MyListCtrl *m_listCtrl;
	wxTextCtrl *m_logWindow;

	wxSizer *m_sizer;

	DECLARE_EVENT_TABLE()

};


#endif