#ifndef ENGINE_MATCH_DIALOG_H
#define ENGINE_MATCH_DIALOG_H



#include "headers.h"

class MyFrame;

class EngineMatchDialog : public wxDialog
{
public:
	EngineMatchDialog(MyFrame* pFrame, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(300,350),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~EngineMatchDialog();

	// ok and cancel
	void OnOk( wxCommandEvent & event );
	void OnCancel( wxCommandEvent & event );

	// other events
	void OnEngine1Load( wxCommandEvent & event );
	void OnEngine1Configure( wxCommandEvent & event );

	void OnEngine2Load( wxCommandEvent & event );
	void OnEngine2Configure( wxCommandEvent & event );

	void OnTimeControlConfigure( wxCommandEvent & event );

	void OnRadio( wxCommandEvent &event );
	void OnRadioButton1( wxCommandEvent &event );
	void OnRadioButton2( wxCommandEvent &event );
	void OnRadioButton3( wxCommandEvent &event );

	MyFrame* m_pFrame;

		//wxCheckBox *WxCheckBox3;
		wxStaticText *WxStaticText6;
		wxComboBox *WxComboBox3;
		wxSpinCtrl *WxSpinCtrl4;
		wxStaticText *WxStaticText5;
		wxFilePickerCtrl *WxFilePickerCtrl1;
	wxButton *WxButton7;
	wxChoice *WxChoice1;
	wxFilePickerCtrl *WxFilePickerCtrl2;
	wxButton *WxButton6;
	wxButton *WxButton5;
	wxCheckBox *WxCheckBox3;
	wxButton *WxButton4;
	wxButton *WxButton3;
	wxCheckBox *WxCheckBox2;
	wxButton *WxButton2;
	wxButton *WxButton1;
	wxCheckBox *WxCheckBox1;
	wxStaticText *WxStaticText3;
	wxStaticText *WxStaticText1;
	wxSpinCtrl *WxSpinCtrl3;
	wxSpinCtrl *WxSpinCtrl2;
	wxRadioButton *WxRadioButton3;
	wxRadioButton *WxRadioButton2;
	wxRadioButton *WxRadioButton1;
	wxStaticText *WxStaticText2;
	wxSpinCtrl *WxSpinCtrl1;
	wxComboBox *WxComboBox2;
	wxComboBox *WxComboBox1;
	wxStaticBox *WxStaticBox1;
	wxStaticBox *WxStaticBox2;
	wxStaticBox *WxStaticBox3;
	wxStaticText *WxStaticText4;
	wxTextCtrl *WxEdit1;
	wxCheckBox *WxCheckBox4;

	int timeControlType;
	int timeControlID;
	wxString engine1;
	wxString engine2;
	wxString eventName;
	int numGames;
	bool swapSidesBetweenGames;
	bool playSamePositionForEachColor;
	bool logEngineOutput;
	int timeInMilliSecs;
	int timeInSeconds;
	int timeInMinutes;
	int incrementMillis;
	int incrementSeconds;
	int incrementMinutes;

	bool timeConfigured;

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

	int fixedDepth;

	int retID;

	int plyForBook;
	bool writeIntoTree;

	MyPipePanel* loadedEngine1;
	MyPipePanel* loadedEngine2;

	DECLARE_EVENT_TABLE()
};


#endif