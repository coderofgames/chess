#ifndef TIME_CONTROL_SETTINGS_DIALOG
#define TIME_CONTROL_SETTINGS_DIALOG



#include "headers.h"





class TimeControlDialog : public wxDialog
{
public:
	TimeControlDialog(MyFrame* pFrame, PGN::Game* game, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(300,350),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~TimeControlDialog();


	void OnOk( wxCommandEvent & event );
	//void OnEditElem( wxCommandEvent & event );
	//void OnAddElem( wxCommandEvent & event );
	//void OnDeleteElem( wxCommandEvent &event );
	void OnCancel( wxCommandEvent & event );


	MyFrame* m_pFrame;
	//PGN::Game* pGame;
	//wxListBox *m_list;

	//bool edited;

		wxStaticText *WxStaticText10;
		wxStaticText *WxStaticText8;
		wxStaticText *WxStaticText7;
		wxStaticText *WxStaticText6;
		wxStaticText *WxStaticText5;
		wxStaticText *WxStaticText4;
		wxStaticText *WxStaticText3;
		wxStaticText *WxStaticText2;
		wxStaticText *WxStaticText1;
		wxComboBox *WxComboBox5;
		wxComboBox *WxComboBox4;
		wxComboBox *WxComboBox3;
		wxComboBox *WxComboBox7;
		wxComboBox *WxComboBox2;
		wxSpinCtrl *WxSpinCtrl7;
		wxSpinCtrl *WxSpinCtrl6;
		wxSpinCtrl *WxSpinCtrl5;
		wxSpinCtrl *WxSpinCtrl4;
		wxRadioButton *WxRadioButton2;
		wxRadioButton *WxRadioButton1;
		wxComboBox *WxComboBox1;
		wxSpinCtrl *WxSpinCtrl3;
		wxSpinCtrl *WxSpinCtrl2;
		wxSpinCtrl *WxSpinCtrl1;
		wxButton *WxButton2;
		wxStaticBox *WxStaticBox1;
		wxButton *WxButton1;
		wxStaticBox *WxStaticBox2;

		int start_minutes;
		int start_seconds;
		int start_inc_seconds;
		int start_inc_millis;

		int time_control_one_num_moves;
		
		int time_control_one_minutes;
		int time_control_one_seconds;
		int time_control_one_inc_seconds;
		int time_control_one_inc_millis;

		bool bFischer;
		int res;
		

	DECLARE_EVENT_TABLE()
};



#endif