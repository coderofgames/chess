#include "EngineLoadingScreen.h"


BEGIN_EVENT_TABLE(EngineLoadingModalDialog, wxDialog)

	
END_EVENT_TABLE()


EngineLoadingModalDialog::EngineLoadingModalDialog(wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{

}

EngineLoadingModalDialog::~EngineLoadingModalDialog()
{
}