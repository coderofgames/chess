#ifndef ENGINE_LIST_DIALOG_H
#define ENGINE_LIST_DIALOG_H

#include "headers.h"

class MyPipePanel;

class EngineManager;

class EngineListModalDialog : public wxDialog
{
public:

	enum
	{
		ENGINE_LIST = wxID_HIGHEST + 350,
		ADD_ENGINE,
		REMOVE_ENGINE,
		EDIT_ENGINE,
			ID_WXBUTTON9,// = 1023,
			ID_WXBUTTON8,// = 1022,
			ID_WXBUTTON7,// = 1021,
			ID_WXLISTBOX2,// = 1020,
			ID_WXBUTTON6,// = 1008,
			ID_WXBUTTON5,// = 1006,
			ID_WXBUTTON4,// = 1005,
			ID_WXBUTTON3,// = 1004,
			ID_WXBUTTON2,// = 1003,
			ID_WXBUTTON1,// = 1002,
			ID_WXLISTBOX1,// = 1001,
			ID_WXSTATICBOX2,// = 1025,
			ID_WXSTATICBOX1,/// = 1009,
			ID_WXSTATICBOX3,
			ID_WXSTATICBOX4,
			ID_WXSTATICTEXT1,
			ID_WXSTATICTEXT2,
	};

	EngineListModalDialog(wxWindow * parent, MyPipePanel *pipePanel, wxWindowID id, const wxString & title,
					wxString* name,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );
	~EngineListModalDialog();

	void OnOk( wxCommandEvent & event );
	void OnCancel( wxCommandEvent & event );
	void OnAddEngine( wxCommandEvent & event );
	void OnDeleteEngine( wxCommandEvent & event );
	void OnEditEngine( wxCommandEvent & event );
	void OnShutdownEngine( wxCommandEvent & event );

	void OnEngineOptions( wxCommandEvent & event );

	void WxListBox1Selected( wxCommandEvent & event );
	void WxListBox2Selected( wxCommandEvent & event );

	MyFrame *m_pFrame;
	MyPipePanel *m_pipePanel;
	wxListBox *m_list;
	wxString * m_name;

	int selectedList;

	vector<string> paths;

		wxButton *WxButton9;
		wxButton *WxButton8;
		wxButton *WxButton7;
		wxListBox *WxListBox2;
		wxButton *WxButton6;
		wxButton *WxButton5;
		wxButton *WxButton4;
		wxButton *WxButton3;
		wxButton *WxButton2;
		wxButton *WxButton1;
		wxListBox *WxListBox1;
		wxStaticBox *WxStaticBox2;
		wxStaticBox *WxStaticBox1;
		wxStaticBox *WxStaticBox3;
		wxStaticBox *WxStaticBox4;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticText1;

	EngineManager *m_engineManager;

	DECLARE_EVENT_TABLE()
};

#endif