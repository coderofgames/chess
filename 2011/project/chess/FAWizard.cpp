#include "FAWizard.h"
#include "mdi.h"
#include "MyPipePanel.h"




	
// ----------------------------------------------------------------------------
// wxChoiceBoxPage
// ----------------------------------------------------------------------------	
wxChoiceBoxPage::wxChoiceBoxPage(wxWizard *parent) : wxWizardPageSimple(parent)
//wxValidationPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
	m_parent = parent;

    m_bitmap = wxBitmap(wiztest2_xpm);

	wxArrayString arrayListFor_comboBox1;
	arrayListFor_comboBox1.Add("Time");
	arrayListFor_comboBox1.Add("Depth");
	arrayListFor_comboBox1.Add("Time AND Depth");
	arrayListFor_comboBox1.Add("Time OR Depth");
    m_comboBox1 = new wxComboBox(this, wxID_ANY, "",wxPoint(5,5), wxDefaultSize, arrayListFor_comboBox1 );//, wxT("&Check me"));
	m_comboBox1->SetValue("Time");

	m_staticText1 = new wxStaticText(this,wxID_ANY,"Search Type");

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *HorizSizer = new wxBoxSizer(wxHORIZONTAL);
	HorizSizer->Add(m_comboBox1, 0, wxALL, 5);
	HorizSizer->Add(m_staticText1, 0, wxALL, 5);

	m_spinControl1 = new wxSpinCtrl(this, wxID_ANY, "");
	m_staticText2 = new wxStaticText(this,wxID_ANY,"Time");
	wxBoxSizer *HorizSizer2 = new wxBoxSizer(wxHORIZONTAL);
	HorizSizer2->Add(m_spinControl1, 0, wxALL, 5);
	HorizSizer2->Add(m_staticText2, 0, wxALL, 5);

	m_spinControl2 = new wxSpinCtrl(this, wxID_ANY, "");
	m_staticText3 = new wxStaticText(this,wxID_ANY,"Depth");

	wxBoxSizer *HorizSizer3 = new wxBoxSizer(wxHORIZONTAL);
	HorizSizer3->Add(m_spinControl2, 0, wxALL, 5);
	HorizSizer3->Add(m_staticText3, 0, wxALL, 5);

	performQuickScan = new wxCheckBox(this, wxID_ANY, "Preliminary Quick Scan");
	goDeeperIntoPromisingMoves = new wxCheckBox(this, wxID_ANY, "Go Deeper into Promising Lines");
	
	reAnalyzeUnclearMoves = new wxCheckBox( this, wxID_ANY, "Re-analyze unclear moves");
	clearHashInBetweenPhases = new wxCheckBox( this, wxID_ANY, "Clear Hash In-between phases");;



    mainSizer->Add(
        HorizSizer,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        HorizSizer2,
        0,
        wxALL,
        5
    );

    mainSizer->Add(
        HorizSizer3,
        0,
        wxALL,
        5
    );

    mainSizer->Add(
		this->performQuickScan,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
		this->goDeeperIntoPromisingMoves,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
		this->reAnalyzeUnclearMoves,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
		this->clearHashInBetweenPhases,
        0,
        wxALL,
        5
    );



	

		
    SetSizerAndFit(mainSizer);
};
	
	
bool wxChoiceBoxPage::TransferDataFromWindow()
{
	wxString value =  m_comboBox1->GetValue();
	if( value == "Time" )
	{
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.go_command = MyFrame::FullAnalysisSettings::GO_TIME;
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.timePerPlySeconds = this->m_spinControl1->GetValue();
	}
	else if( value == "Depth")
	{
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.go_command = MyFrame::FullAnalysisSettings::GO_DEPTH;
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.depthPerPly = this->m_spinControl2->GetValue();
	}
	else if( value == "Depth AND Time")
	{
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.go_command = MyFrame::FullAnalysisSettings::GO_DEPTH_AND_TIME;
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.timePerPlySeconds = this->m_spinControl1->GetValue();
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.depthPerPly = this->m_spinControl2->GetValue();
	}
	else if (value == "Depth OR Time")
	{
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.go_command = MyFrame::FullAnalysisSettings::GO_DEPTH_OR_TIME;
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.timePerPlySeconds = this->m_spinControl1->GetValue();
		((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.depthPerPly = this->m_spinControl2->GetValue();
	}

	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.performQuickScan = this->performQuickScan->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.goDeeperIntoPromisingMoves = this->goDeeperIntoPromisingMoves->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.reAnalyzeUnclearMoves = this->reAnalyzeUnclearMoves->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.clearHashInBetweenPhases = this->clearHashInBetweenPhases->GetValue();	
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.guiControlsTime = true;//this->guiControlsTime->GetValue();	
	
	return true;
}

enum
{
	ID_WIZARD_LOAD = wxID_HIGHEST + 2344,
	ID_WIZARD_OPTIONS,
};

BEGIN_EVENT_TABLE(wxEngineBoxPage, wxPanel)
    EVT_BUTTON(ID_WIZARD_LOAD, wxEngineBoxPage::OnBtnLoad)
	EVT_BUTTON(ID_WIZARD_OPTIONS, wxEngineBoxPage::OnBtnOptions)
END_EVENT_TABLE()


void wxEngineBoxPage::OnBtnLoad(wxCommandEvent& WXUNUSED(event))
{
	wxRibbonButtonBarEvent evt;
	((MyWizard*)m_parent)->m_frame->m_parent->OnEngineLoadBtn(evt);

	wxArrayString arrayStringForComboBox1;
//	wxArrayString arrayStringForComboBox2;
//	wxArrayString arrayStringForComboBox3;
//	wxArrayString arrayStringForComboBox4;
    
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( ((MyWizard*)m_parent)->m_frame->m_pipePanel[jj] &&
			((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->IsActive() )
			if( ((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName()!= "")	
			{
				arrayStringForComboBox1.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox2.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox3.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox4.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			}
	}	

	m_comboBox1->Clear();
	m_comboBox1->Set( arrayStringForComboBox1 );
}

void wxEngineBoxPage::OnBtnOptions(wxCommandEvent& WXUNUSED(event))
{
	wxString value = m_comboBox1->GetValue();
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( ((MyWizard*)m_parent)->m_frame->m_pipePanel[jj] &&
			((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->IsActive() )
		{
			if( ((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName() == value)	
			{
				wxCommandEvent evt;
				//arrayStringForComboBox1.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
				((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->OnBtnEngineOptions(evt);

					return;
			//	arrayStringForComboBox2.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox3.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox4.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			}
		}
	}	
}

// ----------------------------------------------------------------------------
// wxEngineBoxPage
// ----------------------------------------------------------------------------	
wxEngineBoxPage::wxEngineBoxPage(wxWizard *parent) : wxWizardPageSimple(parent)
//wxValidationPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
	m_parent = parent;
    m_bitmap = wxBitmap(wiztest2_xpm);

	wxArrayString arrayStringForComboBox1;
//	wxArrayString arrayStringForComboBox2;
//	wxArrayString arrayStringForComboBox3;
//	wxArrayString arrayStringForComboBox4;
    
	for( int jj = 0 ; jj < MAX_NUM_PIPE_PANELS; jj++ )
	{

		if( ((MyWizard*)m_parent)->m_frame->m_pipePanel[jj] &&
			((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->IsActive() )
			if( ((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName()!= "")	
			{
				arrayStringForComboBox1.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox2.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox3.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			//	arrayStringForComboBox4.Add(((MyWizard*)m_parent)->m_frame->m_pipePanel[jj]->GetEngineName());
			}
	}	
	m_comboBox1 = new wxComboBox(this, wxID_ANY, "",wxPoint(5,5), wxDefaultSize, arrayStringForComboBox1 );//, wxT("&Check me"));
//	m_comboBox2 = new wxComboBox(this, wxID_ANY, "",wxPoint(5,5), wxDefaultSize, arrayStringForComboBox2 );//, 
//	m_comboBox3 = new wxComboBox(this, wxID_ANY, "",wxPoint(5,5), wxDefaultSize, arrayStringForComboBox3 );//, 
//	m_comboBox4 = new wxComboBox(this, wxID_ANY, "",wxPoint(5,5), wxDefaultSize, arrayStringForComboBox4 );//, 

	
	m_staticText1 = new wxStaticText(this,wxID_ANY,"Engine1");
//	m_staticText2 = new wxStaticText(this,wxID_ANY,"Engine2");
//	m_staticText3 = new wxStaticText(this,wxID_ANY,"Engine3");
//	m_staticText4 = new wxStaticText(this,wxID_ANY,"Engine4");

	load1 = new wxButton(this,ID_WIZARD_LOAD, "Load");
//	load2 = new wxButton(this,wxID_ANY, "Load");
//	load3 = new wxButton(this,wxID_ANY, "Load");
//	load4 = new wxButton(this,wxID_ANY, "Load");

	options1 = new wxButton(this,ID_WIZARD_OPTIONS, "Options");
//	options2 = new wxButton(this,wxID_ANY, "Options");
//	options3 = new wxButton(this,wxID_ANY, "Options");
//	options4 = new wxButton(this,wxID_ANY, "Options");



    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *HorizSizer = new wxBoxSizer(wxHORIZONTAL);
/*	wxBoxSizer *HorizSizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *HorizSizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *HorizSizer4 = new wxBoxSizer(wxHORIZONTAL);
*/
	HorizSizer->Add(m_comboBox1, 0, wxALL, 5);
	HorizSizer->Add(m_staticText1, 0, wxALL, 5);
	HorizSizer->Add(load1, 0, wxALL, 5);
	HorizSizer->Add(options1, 0, wxALL, 5);

/*	HorizSizer2->Add(m_comboBox2, 0, wxALL, 5);
	HorizSizer2->Add(m_staticText2, 0, wxALL, 5);
	HorizSizer2->Add(load2, 0, wxALL, 5);
	HorizSizer2->Add(options2, 0, wxALL, 5);

	HorizSizer3->Add(m_comboBox3, 0, wxALL, 5);
	HorizSizer3->Add(m_staticText3, 0, wxALL, 5);
	HorizSizer3->Add(load3, 0, wxALL, 5);
	HorizSizer3->Add(options3, 0, wxALL, 5);

	HorizSizer4->Add(m_comboBox4, 0, wxALL, 5);
	HorizSizer4->Add(m_staticText4, 0, wxALL, 5);
	HorizSizer4->Add(load4, 0, wxALL, 5);
	HorizSizer4->Add(options4, 0, wxALL, 5);
	*/



    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
                            wxT("Engine Management Options\n")
                            wxT("each Engine will analyze for 1 pass\n")),
        0,
        wxALL,
        5
    );

    mainSizer->Add(
        HorizSizer,
        0,
        wxALL,
        5
    );
 /*   mainSizer->Add(
        HorizSizer2,
        0,
        wxALL,
        5
    );

    mainSizer->Add(
        HorizSizer3,
        0,
        wxALL,
        5
    );

    mainSizer->Add(
        HorizSizer4,
        0,
        wxALL,
        5
    );*/


    SetSizerAndFit(mainSizer);
}

bool wxEngineBoxPage::TransferDataFromWindow()
{
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.engine1 = this->m_comboBox1->GetValue();
//	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.engine2 = this->m_comboBox2->GetValue();
//	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.engine3 = this->m_comboBox3->GetValue();
//	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.engine4 = this->m_comboBox4->GetValue();
	
	return true;
}

// ----------------------------------------------------------------------------
// wxBookPage
// ----------------------------------------------------------------------------	
wxBookPage::wxBookPage(wxWizard *parent) : wxWizardPageSimple(parent)
//wxValidationPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
	m_parent = parent;
    m_bitmap = wxBitmap(wiztest2_xpm);

    //   m_comboBox1 = new wxComboBox(this, wxID_ANY, "",wxPoint(5,5), wxDefaultSize );//, wxT("&Check me"));

//	m_staticText1 = new wxStaticText(this,wxID_ANY,"Search Type");

	markBookLines = new wxCheckBox(this, wxID_ANY, "Mark Book Moves");
	analyzeBookLines = new wxCheckBox(this, wxID_ANY,"Analyze Book Moves");
	classifyECO = new wxCheckBox(this, wxID_ANY, "Classify ECO");;
	openingBook = new wxComboBox(this, wxID_ANY, "");
	load = new wxButton(this,wxID_ANY, "Load");

	//textBook = new wxStaticText(this, wxID_ANY,"Book File");
	
	maxPly = new wxSpinCtrl(this,wxID_ANY);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *HorizSizer = new wxBoxSizer(wxHORIZONTAL);
	HorizSizer->Add(openingBook, 0, wxALL, 5);
	//
	HorizSizer->Add(load, 0, wxALL, 5);
		
	
	wxBoxSizer *HorizSizer2 = new wxBoxSizer(wxHORIZONTAL);

	HorizSizer2->Add(maxPly, 0, wxALL, 5);
	HorizSizer2->Add(new wxStaticText(this, wxID_ANY,"Max Ply"), 0, wxALL, 5);

	


    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
                            wxT("Opening Book / Database Options\n")
                            wxT("Set Analysis Behavior for Book moves\n")),
        0,
        wxALL,
        5
    );

    mainSizer->Add(
        markBookLines,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        analyzeBookLines,
        0,
        wxALL,
        5
    );

		
    mainSizer->Add(
        classifyECO,
        0,
        wxALL,
        5
    );

/*    mainSizer->Add(
        textBook,
        0,
        wxALL,
        5
    );*/

    mainSizer->Add(
        HorizSizer,
        0,
        wxALL,
        5
    );

    mainSizer->Add(
        HorizSizer2,
        0,
        wxALL,
        5
    );


    SetSizerAndFit(mainSizer);
}

bool wxBookPage::TransferDataFromWindow()
{
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.bookName = this->openingBook->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.classifyECO = this->classifyECO->GetValue();	
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.markBookMoves = this->markBookLines->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.analyzeBookLines = this->analyzeBookLines->GetValue();	
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.plyForBookLines = this->maxPly->GetValue();	

	return true;
}

// ----------------------------------------------------------------------------
// wxAdditionalOptionsPage
// ----------------------------------------------------------------------------	
wxAdditionalOptionsPage::wxAdditionalOptionsPage(wxWizard *parent) : wxWizardPageSimple(parent)
//wxValidationPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
	m_parent = parent;
    m_bitmap = wxBitmap(wiztest2_xpm);

	generatePVs = new wxCheckBox(this,wxID_ANY, "Write Variations to Notation");
	numPVs = new wxSpinCtrl(this,wxID_ANY, "MultiPV");
	numPVs->SetValue(3);
	numPVsText = new wxStaticText(this, wxID_ANY, "Variation Per Move");

		
	
	markBlunders = new wxCheckBox(this, wxID_ANY, "Find Blunders");
	findInnacuracies = new wxCheckBox(this, wxID_ANY, "Find Innacuracies");
	markUnclearMoves = new wxCheckBox(this, wxID_ANY, "Find Unclear Moves");
	markForcedMoves = new wxCheckBox(this, wxID_ANY, "Find Forced Moves");

	colorCodeMatches = new wxCheckBox(this, wxID_ANY, "Color Code Moves In Notation");


    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *HorizSizer = new wxBoxSizer(wxHORIZONTAL);
	//HorizSizer->Add(generatePVs, 0, wxALL, 5);
		
	HorizSizer->Add(numPVs, 0, wxALL, 5);
	HorizSizer->Add(numPVsText, 0, wxALL, 5);
	//
//	HorizSizer->Add(load, 0, wxALL, 5);
		
	



    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
                            wxT("Additional Options\n")
                            wxT("These options determine the type of data to be obtained\n")),
        0,
        wxALL,
        5
    );


	mainSizer->Add(
        generatePVs,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        HorizSizer,
        0,
        wxALL,
        5
    );


    mainSizer->Add(
		this->markBlunders,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
		this->findInnacuracies,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        markUnclearMoves,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
		this->markForcedMoves,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
		this->colorCodeMatches,
        0,
        wxALL,
        5
    );
  
		
    //  mainSizer->Add(
    //     classifyECO,
    //     0,
    //     wxALL,
    //     5
    // );

/*    mainSizer->Add(
        textBook,
        0,
        wxALL,
        5
    );*/




    SetSizerAndFit(mainSizer);
}

bool wxAdditionalOptionsPage::TransferDataFromWindow()
{
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.generateAlternativeLines = this->generatePVs->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.numLines = this->numPVs->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.findBlunders = this->markBlunders->GetValue();	
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.findForcedMoves = this->markForcedMoves->GetValue();	
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.findUnclearMoves = this->markUnclearMoves->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.findInnacuracies = this->findInnacuracies->GetValue();	
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.colorCodeMatches = this->colorCodeMatches->GetValue();	
	
	return true;
}

// ----------------------------------------------------------------------------
// wxLogPage
// ----------------------------------------------------------------------------	
wxLogPage::wxLogPage(wxWizard *parent) : wxWizardPageSimple(parent)
//wxValidationPage(wxWizard *parent) : wxWizardPageSimple(parent)
{
	m_parent = parent;
    m_bitmap = wxBitmap(wiztest2_xpm);

	m_saveAllAnalysis = new wxCheckBox(this,wxID_ANY, "Save All Analysis");
	m_createTables = new wxCheckBox(this, wxID_ANY, "Create Tables");
	m_createGraphs = new wxCheckBox(this,wxID_ANY , "Create Graphs");
	m_saveDepthScores = new wxCheckBox(this, wxID_ANY, "Save Depth Scores");

	m_writeScoresToAnnotation = new wxCheckBox(this, wxID_ANY, "Write Scores To Annotation");
	m_writeScoresToAnnotation->SetValue(true);
	m_writeDepthToAnnotation = new wxCheckBox( this, wxID_ANY, "Write Depth To Annotation");
	m_writeDepthToAnnotation->SetValue(true);
	m_writeTimeToAnnotation = new wxCheckBox(this, wxID_ANY, "Write Time to Annotation");
	m_writeTimeToAnnotation->SetValue(true);

	m_insertTextAnalysis = new wxCheckBox(this, wxID_ANY, "Insert Text Analysis");

	m_writeToLog = new wxCheckBox(this, wxID_ANY, "Write To Log");
	m_pureLog = new wxCheckBox(this, wxID_ANY, "Write Pure Log");



    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(
        new wxStaticText(this, wxID_ANY,
                            wxT("Options for managing \n")
                            wxT("analysis data\n")),
        0,
        wxALL,
        5
    );

    mainSizer->Add(
        m_saveAllAnalysis,
        0,
        wxALL,
        5
    );
		
			
			

    mainSizer->Add(
        m_createTables,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        m_createGraphs,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        m_saveDepthScores,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        m_writeScoresToAnnotation,
        0,
        wxALL,
        5
    );
			
			
			
			
			
    mainSizer->Add(
        m_writeDepthToAnnotation,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        m_writeTimeToAnnotation,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        m_insertTextAnalysis,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        m_writeToLog,
        0,
        wxALL,
        5
    );
    mainSizer->Add(
        m_pureLog,
        0,
        wxALL,
        5
    );








    SetSizerAndFit(mainSizer);
}

bool wxLogPage::TransferDataFromWindow()
{
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.storeAll = this->m_saveAllAnalysis->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.createTables = this->m_createTables->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.createGraphs = this->m_createGraphs->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.saveDepthScores = this->m_saveDepthScores->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.writeToAnnotation = this->m_writeScoresToAnnotation->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.writeDepthToAnnotation = this->m_writeDepthToAnnotation->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.writeTimeToAnnotation = this->m_writeTimeToAnnotation->GetValue();

	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.insertTextAnalysis = this->m_insertTextAnalysis->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.writeToLog = this->m_writeToLog->GetValue();
	((MyWizard*)this->m_parent)->m_frame->m_fullAnalysisSettings.writePureLog = this->m_pureLog->GetValue();
    
	
	return true;
}



// ----------------------------------------------------------------------------
// MyWizard
// ----------------------------------------------------------------------------

MyWizard::MyWizard(MyFrame *frame, bool useSizer)
{
	m_frame = frame;

    SetExtraStyle(wxWIZARD_EX_HELPBUTTON);

    Create(frame,wxID_ANY,wxT("Absolutely Useless Wizard"),
                   wxBitmap(wiztest_xpm),wxDefaultPosition,
                   wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
    SetIcon(wxICON(sample));

    // Allow the bitmap to be expanded to fit the page height
   // if (frame->GetMenuBar()->IsChecked(Wizard_ExpandBitmap))
        SetBitmapPlacement(wxWIZARD_VALIGN_CENTRE);

    // Enable scrolling adaptation
   // if (frame->GetMenuBar()->IsChecked(Wizard_LargeWizard))
        SetLayoutAdaptationMode(wxDIALOG_ADAPTATION_MODE_ENABLED);

    // a wizard page may be either an object of predefined class
    m_page1 = new wxWizardPageSimple(this);

    /* wxStaticText *text = */ new wxStaticText(m_page1, wxID_ANY,
             wxT("This wizard can help you to\nsetup Full Analysis.\n")
             wxT("\n")
             wxT("The next pages will present you\nwith some options."),
             wxPoint(5,5)
        );


	wxChoiceBoxPage *page2 = new wxChoiceBoxPage(this);


    // ... or a derived class
	wxEngineBoxPage *page3 = new wxEngineBoxPage(this);

	wxBookPage *page4 = new wxBookPage(this);
    //wxRadioboxPage *page3 = new wxRadioboxPage(this);
    //wxValidationPage *page4 = new wxValidationPage(this);
	//wxChoiceBoxPage *page4 = new wxChoiceBoxPage(this);

	wxAdditionalOptionsPage *page5 = new wxAdditionalOptionsPage(this);
	wxLogPage *page6 = new wxLogPage(this);
	
    // set the page order using a convenience function - could also use
    // SetNext/Prev directly as below
    //wxWizardPageSimple::Chain(page3, page4);

    // this page is not a wxWizardPageSimple, so we use SetNext/Prev to insert
    // it into the chain of pages
    //, m_page1, page3);
	//wxWizardPageSimple::Chain(m_page1, page2);
    m_page1->SetNext(page2);
	page2->SetPrev(m_page1);
	page2->SetNext(page3);
	page3->SetPrev(page2);

	page3->SetNext(page4);
	page4->SetPrev(page3);

	page4->SetNext(page5);
	page5->SetPrev(page4);

	page5->SetNext(page6);
	page6->SetPrev(page5);

//	page6->SetNext(page7);
//	page7->SetPrev(page6);
	

    if ( useSizer )
    {
        // allow the wizard to size itself around the pages
        GetPageAreaSizer()->Add(m_page1);
    }
}