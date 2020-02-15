#ifndef NEW_GAME_DIALOG_H
#define NEW_GAME_DIALOG_H

#include "Headers.h"

class MyFrame;

class NewGameDialog : public wxDialog
{
public:

	enum
	{
		ADD_GAME_TO_LIST = wxID_HIGHEST + 450,
		ADD_GAME_CLEAR_LIST,
		
	};
		enum
		{
			////GUI Enum Control ID Start
			ID_WXRADIOBUTTON2 = 10033,
			ID_WXRADIOBUTTON1 = 10032,
			ID_WXSTATICBOX2 = 10027,
			ID_WXBUTTON3 = 10023,
			ID_WXSTATICTEXT3 = 10021,
			ID_WXEDIT3 = 10019,
			ID_WXSTATICTEXT2 = 10012,
			ID_WXSTATICTEXT1 = 10011,
			ID_WXEDIT2 = 10005,
			ID_WXEDIT1 = 10004,
			ID_WXBUTTON2 = 10002,
			ID_WXBUTTON1 = 10001,
			ID_WXSTATICBOX1 = 10025,
			////GUI Enum Control ID End
		//	ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

	NewGameDialog(MyFrame * parent, wxWindowID id, const wxString & title = wxT("New Game Dialog"),
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );
	~NewGameDialog();

	void OnOk( wxCommandEvent & event );
	void AddGameClearList( wxCommandEvent & event );
	void OnCancel( wxCommandEvent & event );

	void EditHeader(wxCommandEvent & event);

			wxRadioButton *WxRadioButton2;
		wxRadioButton *WxRadioButton1;
		wxStaticBox *WxStaticBox2;
		wxButton *WxButton3;
		wxStaticText *WxStaticText3;
		wxTextCtrl *WxEdit3;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticText1;
		wxTextCtrl *WxEdit2;
		wxTextCtrl *WxEdit1;
		wxButton *WxButton2;
		wxButton *WxButton1;
		wxStaticBox *WxStaticBox1;

	MyFrame *m_frame;
	string whiteText;
	string blackText;
	string eventText;

	PGN::Game game;
	bool editedHeader;
	int retCode;
	DECLARE_EVENT_TABLE()
};

#endif