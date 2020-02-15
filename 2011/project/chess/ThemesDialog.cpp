#include "ThemesDialog.h"


BEGIN_EVENT_TABLE(ThemesModalDialog, wxDialog)

	
END_EVENT_TABLE()

ThemesModalDialog::ThemesModalDialog(MyFrame* pFrame, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{

}

ThemesModalDialog::~ThemesModalDialog()
{
}