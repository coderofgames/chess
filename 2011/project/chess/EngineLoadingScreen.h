#ifndef ENGINE_LOADING_SCREEN_H
#define ENGINE_LOADING_SCREEN_H


#include "headers.h"

class EngineLoadingModalDialog : public wxDialog
{
public:
	EngineLoadingModalDialog(wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );
	~EngineLoadingModalDialog();


	DECLARE_EVENT_TABLE()
};


#endif