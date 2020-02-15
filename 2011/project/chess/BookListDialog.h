#ifndef BOOK_LIST_DIALOG_H
#define BOOK_LIST_DIALOG_H

#include "headers.h"


class MyFrame;


class BookListDialog : public wxDialog
{
public:

	enum
	{
		ID_WXBUTTON8 = wxID_HIGHEST+4770,
		ID_WXBUTTON7,
		ID_WXBUTTON4,
		ID_WXBUTTON2,
		ID_WXLISTBOX1,
		ID_WXSTATICBOX1,
	};

	BookListDialog(wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );
	~BookListDialog();

	void OnOk( wxCommandEvent & event );
	void OnCancel( wxCommandEvent & event );
	void OnAddBook( wxCommandEvent & event );
	void OnRemoveBook( wxCommandEvent & event );

		wxButton *WxButton8;
		wxButton *WxButton7;
		wxButton *WxButton4;
		wxButton *WxButton2;
		wxListBox *WxListBox1;
		wxStaticBox *WxStaticBox1;

	vector<string> paths;

	MyFrame* m_frame;

	DECLARE_EVENT_TABLE()
};

#endif