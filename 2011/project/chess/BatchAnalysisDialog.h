#ifndef BATCH_ANALYSIS_DIALOG_H
#define BATCH_ANALYSIS_DIALOG_H

#include "headers.h"

class MyFrame;

class BatchAnalysisDialog : public wxDialog
{
public:
	BatchAnalysisDialog(MyFrame* pFrame, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(300,350),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~BatchAnalysisDialog();


	void OnOk( wxCommandEvent & event );

	void OnCancel( wxCommandEvent & event );


	MyFrame* m_pFrame;


	DECLARE_EVENT_TABLE()
};



#endif