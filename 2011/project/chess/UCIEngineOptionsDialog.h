#ifndef UCI_ENGINE_OPTIONS_H
#define UCI_ENGINE_OPTIONS_H


#include "headers.h"


class MyPipePanel;

#include "EnginePersonality.h"


class EngineOptionsModalDialog : public wxDialog
{
public:
	EngineOptionsModalDialog(wxWindow * parent, MyPipePanel *pp, string & optionsString,
				wxWindowID id, const wxString & title,  
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );

	~EngineOptionsModalDialog();

	void OnPropertyGridChange(wxPropertyGridEvent& event);
    void OnPropertyGridChanging(wxPropertyGridEvent& event);

	void OnCancel( wxCommandEvent & event );
	void OnClose(wxCloseEvent& event);
	void OnOk(wxCommandEvent& event);

	void OnLoadPersonality(wxCommandEvent& event);

	void FindAndSetOptionValue( wxString name, wxString value);

	wxPropertyGrid* m_pg;
	vector< UCIOption* > optionsMap;
	//EnginePersonality m_ep;

	string outPutString;

	MyPipePanel *m_pPipePanel;

	DECLARE_EVENT_TABLE()
};

#endif