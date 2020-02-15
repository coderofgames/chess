#ifndef PERSONALITY_LIST_DIALOG_H
#define PERSONALITY_LIST_DIALOG_H

#include "headers.h"

class MyFrame;
class MyPipePanel;
class UCIEngineOptionsDialog;
class UCIEnginePersonality;

class EnginePersonalityDialog : public wxDialog
{
public:

		enum
		{
			////GUI Enum Control ID Start
			ID_WXBUTTON3 = wxID_HIGHEST + 3460,
			ID_WXBUTTON1 ,
			ID_WXBUTTON8 ,
			ID_WXBUTTON7 ,
			ID_WXBUTTON4 ,
			ID_WXBUTTON2 ,
			ID_WXLISTBOX1,
			ID_WXSTATICBOX1 ,
			////GUI Enum Control ID End
			//ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

	EnginePersonalityDialog(wxWindow* parent, MyFrame * frame, wxString &engineName, UCIEnginePersonality* pEnginePersonality, MyPipePanel* pp,wxWindowID id, const wxString & title = wxT("Engine Personality Dialog"),
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );

	~EnginePersonalityDialog();

	void OnOk( wxCommandEvent & event );
	//void AddGameClearList( wxCommandEvent & event );
	void OnCancel( wxCommandEvent & event );


	void OnEditName( wxCommandEvent & event );
	//void EditHeader(wxCommandEvent & event);

	wxButton *WxButton3;
		wxButton *WxButton1;
		wxButton *WxButton8;
		wxButton *WxButton7;
		wxButton *WxButton4;
		wxButton *WxButton2;
		wxListBox *WxListBox1;
		wxStaticBox *WxStaticBox1;


	MyPipePanel* m_pipePanel;
	string newPersonalityFileName;
	vector<string> m_elems;
	MyFrame *m_frame;
	wxString m_engineName;
	UCIEnginePersonality *m_enginePersonality;

	DECLARE_EVENT_TABLE()
};

#endif