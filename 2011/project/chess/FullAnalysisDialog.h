#ifndef FULL_ANALYSIS_DIALOG
#define FULL_ANALYSIS_DIALOG

#include "headers.h"

class MyFrame;

class FullAnalysisDialog : public wxDialog
{
public:
	FullAnalysisDialog(MyFrame* pFrame, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(300,350),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~FullAnalysisDialog();


	void OnOk( wxCommandEvent & event );

	void OnCancel( wxCommandEvent & event );

	wxCheckBox *WxCheckBox16;
	wxCheckBox *WxCheckBox15;
	wxButton *WxButton11;
	wxButton *WxButton10;
	wxButton *WxButton9;
	wxButton *WxButton8;
	wxStaticText *WxStaticText9;
	wxChoice *WxChoice6;
	wxStaticBox *WxStaticBox5;
	wxStaticText *WxStaticText8;
	wxChoice *WxChoice5;
	wxButton *WxButton7;
	wxButton *WxButton6;
	wxButton *WxButton5;
	wxButton *WxButton4;
	wxStaticText *WxStaticText7;
	wxChoice *WxChoice4;
	wxStaticText *WxStaticText6;
	wxChoice *WxChoice3;
	wxFilePickerCtrl *WxFilePickerCtrl1;
	wxCheckBox *WxCheckBox14;
	wxCheckBox *WxCheckBox13;
	wxButton *WxButton3;
	wxStaticText *WxStaticText5;
	wxChoice *WxChoice2;
	wxCheckBox *WxCheckBox12;
	wxCheckBox *WxCheckBox11;
	wxCheckBox *WxCheckBox10;
	wxCheckBox *WxCheckBox9;
	wxStaticText *WxStaticText4;
	wxSpinCtrl *WxSpinCtrl3;
	wxCheckBox *WxCheckBox8;
	wxCheckBox *WxCheckBox7;
	wxCheckBox *WxCheckBox6;
	wxCheckBox *WxCheckBox3;
	wxCheckBox *WxCheckBox5;
	wxCheckBox *WxCheckBox4;
	wxCheckBox *WxCheckBox2;
	wxButton *WxButton2;
	wxButton *WxButton1;
	wxCheckBox *WxCheckBox1;
	wxStaticText *WxStaticText3;
	wxSpinCtrl *WxSpinCtrl2;
	wxStaticText *WxStaticText2;
	wxSpinCtrl *WxSpinCtrl1;
	wxStaticText *WxStaticText1;
	wxChoice *WxChoice1;
	wxStaticBox *WxStaticBox2;
	wxStaticBox *WxStaticBox4;
	wxStaticBox *WxStaticBox1;
	wxStaticBox *WxStaticBox3;

	MyFrame* m_pFrame;


	DECLARE_EVENT_TABLE()
};

#endif