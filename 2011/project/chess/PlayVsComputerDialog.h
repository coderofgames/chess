#ifndef PLAY_VS_COMPUTER_DIALOG_H
#define PLAY_VS_COMPUTER_DIALOG_H

#include "Headers.h"

class MyFrame;

class PlayVsComputerDialog : public wxDialog
{
public:

	enum
	{
		ADD_GAME_TO_LIST = wxID_HIGHEST + 450,
		ADD_GAME_CLEAR_LIST,
		KEEP_CURRENT,
		
	};
		enum
		{
			////GUI Enum Control ID Start
			ID_WXCHECKBOX1 = wxID_HIGHEST+9000,
			ID_WXRADIOBUTTON3 ,
			ID_WXCOMBO2 ,
			ID_WXCOMBO1 ,
			ID_WXRADIOBUTTON2 ,
			ID_WXRADIOBUTTON1 ,
			ID_WXSTATICBOX2 ,
			ID_WXSTATICTEXT2 ,
			ID_WXSTATICTEXT1 ,
			ID_WXBUTTON2 ,
			ID_WXBUTTON1 ,
			ID_WXSTATICBOX1 ,
			ID_WXBUTTON3,
			ID_WXBUTTON4,
			ID_WXCOMBOBOX1,
			ID_WXBUTTON5,
			ID_WXSPINCTRL1,
			ID_WXSTATICTEXT3,
			////GUI Enum Control ID End
		//	ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

	PlayVsComputerDialog(MyFrame * parent, wxWindowID id, const wxString & title = wxT("New Game Dialog"),
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxDefaultSize,
	              long style = wxDEFAULT_DIALOG_STYLE );
	~PlayVsComputerDialog();

	void OnOk( wxCommandEvent & event );
	void AddGameClearList( wxCommandEvent & event );
	void OnCancel( wxCommandEvent & event );
	void OnLoadEngine( wxCommandEvent & event );
	void OnConfigureEngine( wxCommandEvent & event );
	void OnSetTimeControl( wxCommandEvent & event );
	//void OnSetTimeControl( wxCommandEvent & event );

		wxCheckBox *WxCheckBox1;
		wxRadioButton *WxRadioButton3;
		wxComboBox *WxCombo2;
		wxComboBox *WxCombo1;
		wxRadioButton *WxRadioButton2;
		wxRadioButton *WxRadioButton1;
		wxStaticBox *WxStaticBox2;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticText1;
		wxButton *WxButton2;
		wxButton *WxButton1;
		wxButton *WxButton3;
		wxButton *WxButton4;
		wxButton *WxButton5;
		wxStaticBox *WxStaticBox1;
		wxComboBox *WxComboBox1;
		wxStaticText* WxStaticText3;
		wxSpinCtrl* WxSpinCtrl1;

	MyFrame *m_frame;
	string whiteText;
	string blackText;
	string eventText;

	int numGames;

	bool timeConfigured;
	

	PGN::Game game;
	bool editedHeader;
	int retCode;
	DECLARE_EVENT_TABLE()
};

#endif