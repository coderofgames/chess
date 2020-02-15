#ifndef THEMES_DIALOG_H
#define THEMES_DIALOG_H

#include "headers.h"

class ThemesModalDialog : public wxDialog
{
public:
	ThemesModalDialog(MyFrame* pFrame, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );
	~ThemesModalDialog();


	DECLARE_EVENT_TABLE()
};

#endif