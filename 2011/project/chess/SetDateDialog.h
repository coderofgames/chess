#ifndef SET_DATE_DIALOG_H
#define SET_DATE_DIALOG_H

#include "headers.h"

class SetDateDialog : public wxDialog
{
public:
	SetDateDialog(wxWindow * parent,  wxWindowID id, const wxString & title, wxString text,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(450,200),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~SetDateDialog();


	void OnOk( wxCommandEvent & event );
	void OnCancel( wxCommandEvent & event );

	wxString GetString();

	wxDatePickerCtrl *dateCtrl;

	wxString m_text;

	DECLARE_EVENT_TABLE()
};


#endif