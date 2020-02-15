#ifndef FA_WIZARD_H
#define FA_WIZARD_H


#include "Headers.h"
#include "wx/wizard.h"

#include "wiztest.xpm"
#include "wiztest2.xpm"


// ids for menu items
enum
{
    Wizard_About = wxID_ABOUT,
    Wizard_Quit = wxID_EXIT,
    Wizard_RunModal = wxID_HIGHEST+2753,

    Wizard_RunNoSizer,
    Wizard_RunModeless,

    Wizard_LargeWizard,
    Wizard_ExpandBitmap
};


// ----------------------------------------------------------------------------
// our wizard
// ----------------------------------------------------------------------------
class MyFrame;

class MyWizard : public wxWizard
{
public:
    MyWizard(MyFrame *frame, bool useSizer = true);

    wxWizardPage *GetFirstPage() const { return m_page1; }

	MyFrame *m_frame;
private:
    wxWizardPageSimple  *m_page1;

	
};

// ----------------------------------------------------------------------------
// some pages for our wizard
// ----------------------------------------------------------------------------

// This shows how to simply control the validity of the user input by just
// overriding TransferDataFromWindow() - of course, in a real program, the
// check wouldn't be so trivial and the data will be probably saved somewhere
// too.
//

class wxChoiceBoxPage : public wxWizardPageSimple
{
public:

	wxChoiceBoxPage(wxWizard *parent);


    virtual bool TransferDataFromWindow();

private:
    wxCheckBox *m_checkbox;
	wxComboBox *m_comboBox1;
	//wxComboBox m_comboBox2;
	wxSpinCtrl *m_spinControl1;
	wxSpinCtrl *m_spinControl2;
	wxStaticText *m_staticText1;
	wxStaticText *m_staticText2;
	wxStaticText *m_staticText3;

	wxCheckBox *performQuickScan;
	wxCheckBox *goDeeperIntoPromisingMoves;
	wxCheckBox *reAnalyzeUnclearMoves;
	wxCheckBox *clearHashInBetweenPhases;

	wxCheckBox* guiControlsTime;
	wxWizard *m_parent;
};


class wxEngineBoxPage : public wxWizardPageSimple
{
public:

	wxEngineBoxPage(wxWizard *parent);

    virtual bool TransferDataFromWindow();


	void OnBtnLoad(wxCommandEvent& WXUNUSED(event));
	void OnBtnOptions(wxCommandEvent& WXUNUSED(event));

private:
    wxCheckBox *m_checkbox;

	wxComboBox *m_comboBox1;
	//wxComboBox *m_comboBox2;
	//wxComboBox *m_comboBox3;
	//wxComboBox *m_comboBox4;

	wxStaticText *m_staticText1;
	//wxStaticText *m_staticText2;
	//wxStaticText *m_staticText3;
	//wxStaticText *m_staticText4;

	//wxComboBox m_comboBox2;
	wxButton *load1;
	//wxButton *load2;
	//wxButton *load3;
	//wxButton *load4;

	wxButton *options1;
	//wxButton *options2;
	//wxButton *options3;
	//wxButton *options4;

	wxWizard *m_parent;

	DECLARE_EVENT_TABLE()
};


class wxLogPage : public wxWizardPageSimple
{
public:

	wxLogPage(wxWizard *parent);

    virtual bool TransferDataFromWindow();

private:
    wxCheckBox *m_saveAllAnalysis;
	wxCheckBox *m_createTables;
	wxCheckBox *m_createGraphs;
	wxCheckBox *m_saveDepthScores;

	wxCheckBox *m_writeScoresToAnnotation;
	wxCheckBox *m_writeDepthToAnnotation;
	wxCheckBox *m_writeTimeToAnnotation;

	wxCheckBox *m_insertTextAnalysis;

	wxCheckBox *m_writeToLog;
	wxCheckBox *m_pureLog;


	wxWizard *m_parent;

};


class wxBookPage : public wxWizardPageSimple
{
public:

	wxBookPage(wxWizard *parent);
	
    virtual bool TransferDataFromWindow();

private:
 /*   wxCheckBox *m_checkbox;
	wxComboBox *m_comboBox1;
	//wxComboBox m_comboBox2;
	wxSpinCtrl *m_spinControl1;
	wxSpinCtrl *m_spinControl2;
	wxStaticText *m_staticText1;
	wxStaticText *m_staticText2;
	wxStaticText *m_staticText3;
	*/
	wxCheckBox *markBookLines;
	wxCheckBox *analyzeBookLines;
	wxCheckBox *classifyECO;
	wxComboBox *openingBook;
	wxStaticText* textBook;
	wxButton *load;
	wxSpinCtrl *maxPly;

	wxWizard *m_parent;
};


class wxAdditionalOptionsPage : public wxWizardPageSimple
{
public:

	wxAdditionalOptionsPage(wxWizard *parent) ;

    virtual bool TransferDataFromWindow();

private:
 /*   wxCheckBox *m_checkbox;
	wxComboBox *m_comboBox1;
	//wxComboBox m_comboBox2;
	wxSpinCtrl *m_spinControl1;
	wxSpinCtrl *m_spinControl2;
	wxStaticText *m_staticText1;
	wxStaticText *m_staticText2;
	wxStaticText *m_staticText3;
	*/


//	wxCheckBox *saveLog;
//	wxCheckBox *savePureLog;

	wxCheckBox *generatePVs;
	wxSpinCtrl *numPVs;
	wxStaticText *numPVsText;

	wxCheckBox *markUnclearMoves;
	wxCheckBox *markBlunders;
	wxCheckBox *findInnacuracies;
	wxCheckBox *markForcedMoves;

	wxCheckBox *colorCodeMatches;

	wxWizard *m_parent;


//	wxComboBox *openingBook;
//	wxStaticText* textBook;
//	wxButton *load;
//	wxSpinCtrl *maxPly;
};


// It also shows how to use a different bitmap for one of the pages.
class wxValidationPage : public wxWizardPageSimple
{
public:
    wxValidationPage(wxWizard *parent) : wxWizardPageSimple(parent)
    {
        m_bitmap = wxBitmap(wiztest2_xpm);

        m_checkbox = new wxCheckBox(this, wxID_ANY, wxT("&Check me"));

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(
            new wxStaticText(this, wxID_ANY,
                             wxT("You need to check the checkbox\n")
                             wxT("below before going to the next page\n")),
            0,
            wxALL,
            5
        );

        mainSizer->Add(
            m_checkbox,
            0, // No stretching
            wxALL,
            5 // Border
        );
        SetSizerAndFit(mainSizer);
    }

    virtual bool TransferDataFromWindow()
    {
        if ( !m_checkbox->GetValue() )
        {
            wxMessageBox(wxT("Check the checkbox first!"), wxT("No way"),
                         wxICON_WARNING | wxOK, this);

            return false;
        }

        return true;
    }

private:
    wxCheckBox *m_checkbox;
};

// This is a more complicated example of validity checking: using events we may
// allow to return to the previous page, but not to proceed. It also
// demonstrates how to intercept [Cancel] button press.
class wxRadioboxPage : public wxWizardPageSimple
{
public:
    // directions in which we allow the user to proceed from this page
    enum
    {
        Forward, Backward, Both, Neither
    };

    wxRadioboxPage(wxWizard *parent) : wxWizardPageSimple(parent)
    {
        // should correspond to the enum above
        //        static wxString choices[] = { "forward", "backward", "both", "neither" };
        // The above syntax can cause an internal compiler error with gcc.
        wxString choices[4];
        choices[0] = wxT("forward");
        choices[1] = wxT("backward");
        choices[2] = wxT("both");
        choices[3] = wxT("neither");

        m_radio = new wxRadioBox(this, wxID_ANY, wxT("Allow to proceed:"),
                                 wxDefaultPosition, wxDefaultSize,
                                 WXSIZEOF(choices), choices,
                                 1, wxRA_SPECIFY_COLS);
        m_radio->SetSelection(Both);

        wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
        mainSizer->Add(
            m_radio,
            0, // No stretching
            wxALL,
            5 // Border
        );

        SetSizerAndFit(mainSizer);
    }

    // wizard event handlers
    void OnWizardCancel(wxWizardEvent& event)
    {
        if ( wxMessageBox(wxT("Do you really want to cancel?"), wxT("Question"),
                          wxICON_QUESTION | wxYES_NO, this) != wxYES )
        {
            // not confirmed
            event.Veto();
        }
    }

    void OnWizardPageChanging(wxWizardEvent& event)
    {
        int sel = m_radio->GetSelection();

        if ( sel == Both )
            return;

        if ( event.GetDirection() && sel == Forward )
            return;

        if ( !event.GetDirection() && sel == Backward )
            return;

        wxMessageBox(wxT("You can't go there"), wxT("Not allowed"),
                     wxICON_WARNING | wxOK, this);

        event.Veto();
    }

private:
    wxRadioBox *m_radio;

    DECLARE_EVENT_TABLE()
};



#endif