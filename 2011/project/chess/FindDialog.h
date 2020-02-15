#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H



#include "headers.h"

class MyFrame;

class FindDialog : public wxDialog
{
public:
	FindDialog(MyFrame* pFrame, wxWindow * parent, wxWindowID id, const wxString & title, int *sel,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(300,350),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~FindDialog(){}

	// ok and cancel
	void OnOk( wxCommandEvent & evt );
	void OnCancel( wxCommandEvent & evt );
	void OnClose( wxCloseEvent & evt );

	

		wxButton *WxButton8;
		wxButton *WxButton7;
		wxListBox *WxListBox1;
	MyFrame* m_pFrame;

	string stringSelection;
	int *m_selection;

	DECLARE_EVENT_TABLE()
};

#endif