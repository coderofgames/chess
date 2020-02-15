#include "BatchAnalysisDialog.h"

#include "mdi.h"

BEGIN_EVENT_TABLE(BatchAnalysisDialog, wxDialog)
	EVT_BUTTON(wxID_OK, BatchAnalysisDialog::OnOk)


END_EVENT_TABLE()


BatchAnalysisDialog::BatchAnalysisDialog(MyFrame* pFrame,  wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{


	m_pFrame = pFrame;

}

BatchAnalysisDialog::~BatchAnalysisDialog()
{
}

void BatchAnalysisDialog::OnOk( wxCommandEvent & event )
{

	Close(true);
}




void BatchAnalysisDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}