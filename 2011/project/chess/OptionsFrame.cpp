#include "OptionsFrame.h"
#include "mdi.h"
#include "BoardCanvas.h"
#include "MiscUtils.h"
#include "MyPipePanel.h"
#include "EngineAnalysisCanvas.h"
#include "NotationFrame.h"
#include "MoveDataCanvas.h"
#include "MyListPanel.h"
#include "ListBox.h"

class wxMyImageInfo;

WX_DECLARE_OBJARRAY(wxMyImageInfo, wxArrayMyImageInfo);

class wxMyImageInfo
{
public:
    wxString    m_path;
    wxBitmap*   m_pThumbnail1; // smaller thumbnail
    wxBitmap*   m_pThumbnail2; // larger thumbnail

    wxMyImageInfo ( const wxString& str )
    {
        m_path = str;
        m_pThumbnail1 = (wxBitmap*) NULL;
        m_pThumbnail2 = (wxBitmap*) NULL;
    }
    virtual ~wxMyImageInfo()
    {
        if ( m_pThumbnail1 )
            delete m_pThumbnail1;
        if ( m_pThumbnail2 )
            delete m_pThumbnail2;
    }

};


#include <wx/arrimpl.cpp>
WX_DEFINE_OBJARRAY(wxArrayMyImageInfo);

wxArrayMyImageInfo  g_myImageArray;


// Preferred thumbnail height.
#define PREF_THUMBNAIL_HEIGHT       64


wxPGChoices wxAdvImageFileProperty::ms_choices;

WX_PG_IMPLEMENT_PROPERTY_CLASS(wxAdvImageFileProperty,wxFileProperty,
                               wxString,const wxString&,ChoiceAndButton)


wxAdvImageFileProperty::wxAdvImageFileProperty( const wxString& label,
                                                          const wxString& name,
                                                          const wxString& value)
    : wxFileProperty(label,name,value)
{
    m_wildcard = wxPGGetDefaultImageWildcard();

    m_index = -1;

    m_pImage = (wxImage*) NULL;

    // Only show names.
    m_flags &= ~(wxPG_PROP_SHOW_FULL_FILENAME);
}

wxAdvImageFileProperty::~wxAdvImageFileProperty ()
{
    // Delete old image
    wxDELETE(m_pImage);
}

void wxAdvImageFileProperty::OnSetValue()
{
    wxFileProperty::OnSetValue();

    // Delete old image
    wxDELETE(m_pImage);

    wxString imagename = GetValueAsString(0);

    if ( imagename.length() )
    {
        wxFileName filename = GetFileName();
        size_t prevCount = g_myImageArray.GetCount();
        int index = ms_choices.Index(imagename);

        // If not in table, add now.
        if ( index == wxNOT_FOUND )
        {
            ms_choices.Add( imagename );
            g_myImageArray.Add( new wxMyImageInfo( filename.GetFullPath() ) );

            index = g_myImageArray.GetCount() - 1;
        }

        // If no thumbnail ready, then need to load image.
        if ( !g_myImageArray[index].m_pThumbnail2 )
        {
            // Load if file exists.
            if ( filename.FileExists() )
                m_pImage = new wxImage( filename.GetFullPath() );
        }

        m_index = index;

        wxPropertyGrid* pg = GetGrid();
        wxWindow* control = pg->GetEditorControl();

        if ( pg->GetSelection() == this && control )
        {
            wxString name = GetValueAsString(0);

            if ( g_myImageArray.GetCount() != prevCount )
            {
                wxASSERT( g_myImageArray.GetCount() == (prevCount+1) );

                // Add to the control's array.
                // (should be added to own array earlier)

                if ( control )
                    GetEditorClass()->InsertItem(control, name, -1);
            }

            if ( control )
               GetEditorClass()->UpdateControl(this, control);
        }
    }
    else
        m_index = -1;
}

bool wxAdvImageFileProperty::IntToValue( wxVariant& variant, int number, int WXUNUSED(argFlags) ) const
{
    wxASSERT( number >= 0 );
    return StringToValue( variant, ms_choices.GetLabel(number), wxPG_FULL_VALUE );
}

bool wxAdvImageFileProperty::OnEvent( wxPropertyGrid* propgrid, wxWindow* primary,
   wxEvent& event )
{
 //   if ( propgrid->IsMainButtonEvent(event) )
    {
        return wxFileProperty::OnEvent(propgrid,primary,event);
    }
    return false;
}

wxSize wxAdvImageFileProperty::OnMeasureImage( int item ) const
{
    if ( item == -1 )
        return wxPG_DEFAULT_IMAGE_SIZE;

    return wxSize(PREF_THUMBNAIL_HEIGHT,PREF_THUMBNAIL_HEIGHT);
}

void wxAdvImageFileProperty::LoadThumbnails( size_t index )
{
    wxMyImageInfo& mii = g_myImageArray[index];

    if ( !mii.m_pThumbnail2 )
    {
        wxFileName filename = GetFileName();

        if ( !m_pImage || !m_pImage->Ok() ||
             filename != mii.m_path
           )
        {
            if ( m_pImage )
                delete m_pImage;
            m_pImage = new wxImage( mii.m_path );
        }

        if ( m_pImage && m_pImage->Ok() )
        {
            int im_wid = m_pImage->GetWidth();
            int im_hei = m_pImage->GetHeight();
            if ( im_hei > PREF_THUMBNAIL_HEIGHT )
            {
                // TNW = (TNH*IW)/IH
                im_wid = (PREF_THUMBNAIL_HEIGHT*m_pImage->GetWidth())/m_pImage->GetHeight();
                im_hei = PREF_THUMBNAIL_HEIGHT;
            }

            m_pImage->Rescale( im_wid, im_hei );

            mii.m_pThumbnail2 = new wxBitmap( *m_pImage );

            wxSize cis = GetParentState()->GetGrid()->GetImageSize();
            m_pImage->Rescale ( cis.x, cis.y );

            mii.m_pThumbnail1 = new wxBitmap( *m_pImage );

        }

        wxDELETE(m_pImage);
    }
}

void wxAdvImageFileProperty::OnCustomPaint( wxDC& dc,
                                                 const wxRect& rect,
                                                 wxPGPaintData& pd )
{
    int index = m_index;
    if ( pd.m_choiceItem >= 0 )
        index = pd.m_choiceItem;

    //wxLogDebug(wxT("%i"),index);

    if ( index >= 0 )
    {
        LoadThumbnails(index);

        // Is this a measure item call?
        if ( rect.x < 0 )
        {
            // Variable height
            //pd.m_drawnHeight = PREF_THUMBNAIL_HEIGHT;
            wxBitmap* pBitmap = (wxBitmap*)g_myImageArray[index].m_pThumbnail2;
            if ( pBitmap )
                pd.m_drawnHeight = pBitmap->GetHeight();
            else
                pd.m_drawnHeight = 16;
            return;
        }

        // Draw the thumbnail

        wxBitmap* pBitmap;

        if ( pd.m_choiceItem >= 0 )
            pBitmap = (wxBitmap*)g_myImageArray[index].m_pThumbnail2;
        else
            pBitmap = (wxBitmap*)g_myImageArray[index].m_pThumbnail1;

        if ( pBitmap )
        {
            dc.DrawBitmap ( *pBitmap, rect.x, rect.y, FALSE );

            // Tell the caller how wide we drew.
            pd.m_drawnWidth = pBitmap->GetWidth();

            return;
        }
    }

    // No valid file - just draw a white box.
    dc.SetBrush ( *wxWHITE_BRUSH );
    dc.DrawRectangle ( rect );
}



BEGIN_EVENT_TABLE(OptionsFrame, wxScrolledWindow)
	EVT_SIZE(OptionsFrame::OnSize)
	EVT_PAINT  (OptionsFrame::OnPaint)
	EVT_PG_CHANGED( wxID_HIGHEST+1111, OptionsFrame::OnPropertyGridChange )
END_EVENT_TABLE()

OptionsFrame::OptionsFrame(MyFrame *parent) : wxScrolledWindow((wxWindow*)parent)//, wxID_ANY, "Options",
					 //wxDefaultPosition, wxDefaultSize,
                      //       wxVSCROLL | wxFULL_REPAINT_ON_RESIZE)
{
	//SetSize(400,400);
//	sizer = new wxBoxSizer(wxVERTICAL);
	m_parent = parent;
    //m_pg = new wxPropertyGrid(this,wxID_HIGHEST+1000,wxDefaultPosition,wxSize(400,400),
     //                   wxPG_SPLITTER_AUTO_CENTER |
      //                  wxPG_BOLD_MODIFIED );
	//m_NotationOptions.notationFont.FreeResource(true);
	//ofstream of(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar()) + string("/settings/options.pts"),ios::binary);
	/*ifstream ifs(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar()) + string("/settings/options.pts"),ios::binary);
	if( ifs.is_open() )
	{
		ifs.read((char *)&m_GameOptions, sizeof(GameOptions));
		ifs.read((char *)&m_ThemeOptions, sizeof(ThemeOptions));//]=-0=p[
		ifs.read((char *)&m_BoardOptions, sizeof(BoardOptions));
		ifs.read((char *)&m_NotationOptions, sizeof(NotationOptions));
		ifs.read((char *)&m_TreePaneOptions, sizeof(TreePaneOptions));
		ifs.read((char *)&m_GeneralAppOptions, sizeof(GeneralAppOptions));
		ifs.read((char *)&m_EnginePanelOptions, sizeof(EnginePanelOptions));
		ifs.close();
	}*/

	if( LoadOptionsFile() == false )
	{
		wxMessageBox("Error Loading Options File","Error");
	}
	

    m_pg =
        new wxPropertyGridManager(this,
                                  // Don't change this into wxID_ANY in the sample, or the
                                  // event handling will obviously be broken.
                                  wxID_HIGHEST+1111, /*wxID_ANY*/
                                  wxDefaultPosition,
                                  wxSize(100, 100), // FIXME: wxDefaultSize gives assertion in propgrid.
                                                    // But calling SetInitialSize in manager changes the code
                                                    // order to the grid gets created immediately, before SetExtraStyle
                                                    // is called.
                                  wxPG_SPLITTER_AUTO_CENTER |
                        wxPG_BOLD_MODIFIED );

	wxPropertyGridPage *pPage = m_pg->AddPage(wxT("Game Settings"));
	//m_pg->AddPage(wxT("Game"));


		//m_pg->SetPropertyAttributeAll(
	pPage->SetPropertyAttributeAll(wxPG_BOOL_USE_CHECKBOX,true);
	pPage->SetPropertyAttributeAll(wxString("MotionSpin"),true);

	//wxIntProperty *intProp = new wxIntProperty("Int Prop", wxPG_LABEL);
	//pPage->Append(intProp );
	//m_pg->SetPropertyAttribute(name.c_str(), wxPG_ATTR_MIN, (long)minVal );
	//m_pg->SetPropertyAttribute(name.c_str(), wxPG_ATTR_MAX, (long)maxVal );
	//m_pg->SetPropertyValue(name.c_str(),(long)defaultVal);
//	pPage->SetPropertyEditor( "Int Prop", wxPGEditor_SpinCtrl );
	pPage->Append( new wxPropertyCategory(wxT("Game"),wxT("GameCategory")) );
	{
		wxBoolProperty *boolProp = new wxBoolProperty("Always Promote To Queen", wxPG_LABEL);
		pPage->Append(boolProp );
		pPage->SetPropertyValue("Always Promote To Queen",this->m_GameOptions.alwaysPromoteToQueen);
		
		wxBoolProperty *boolProp2 = new wxBoolProperty("Show Highlights At End", wxPG_LABEL);
		pPage->Append(boolProp2 );
		pPage->SetPropertyValue("Show Highlights At End",this->m_GameOptions.highlightsAtEnd);
		
		wxPGChoices soc;
		soc.Add( "Highlight King", 0 );
		soc.Add( "Highlight Attackers", 1 );
	//	soc.Add( "Blue", 2 );
	//	soc.Add( "Summer Flame", 3 );
		wxEnumProperty *choiceProp = new wxEnumProperty("Highlight Type",wxPG_LABEL,soc); 

		pPage->Append(choiceProp );
		pPage->SetPropertyValue("Highlight Type",(long)this->m_GameOptions.highlightsAtEnd);
		//pPage->SetPropertyValue("Highlight Type","Highlight King");

		wxBoolProperty *boolProp3 = new wxBoolProperty("Show MessageBox at End", wxPG_LABEL);
		pPage->Append(boolProp3 );
		pPage->SetPropertyValue("Show MessageBox at End",this->m_GameOptions.showMessageBoxAtGameEnd);

		wxPGChoices soc2;
		soc2.Add( "Drag And Drop", 0 );
		soc2.Add( "Two Clicks", 1 );
	//	soc.Add( "Blue", 2 );
	//	soc.Add( "Summer Flame", 3 );
		wxEnumProperty *choiceProp2 = new wxEnumProperty("Move Entry Style",wxPG_LABEL,soc2);
		pPage->Append(choiceProp2 );
		pPage->SetPropertyValue("Move Entry Style",this->m_GameOptions.MoveEntryStyle);

		//pPage->SetPropertyValue("Move Entry Style","Drag And Drop");

		wxBoolProperty *boolProp4 = new wxBoolProperty("Use Snaps", wxPG_LABEL);
		pPage->Append(boolProp4 );
		pPage->SetPropertyValue("Use Snaps",this->m_GameOptions.useSnaps);





		wxBoolProperty *boolProp7 = new wxBoolProperty("Show Engine Ouput", wxPG_LABEL);
		pPage->Append(boolProp7 );
		pPage->SetPropertyValue("Show Engine Ouput",this->m_GameOptions.showEngineOutput);


		wxBoolProperty *boolProp9 = new wxBoolProperty("Animate", wxPG_LABEL);
		pPage->Append(boolProp9 );
		pPage->SetPropertyValue("Animate",this->m_GameOptions.animate);

		wxIntProperty *intProp = new wxIntProperty("Animation Speed", wxPG_LABEL);
		pPage->Append(intProp );
		long minSpeed = 1;
		long maxSpeed = 12;
		long defaultSpeed = 6;
		pPage->SetPropertyAttribute("Animation Speed", wxPG_ATTR_MIN, (long)minSpeed );
		pPage->SetPropertyAttribute("Animation Speed", wxPG_ATTR_MAX, (long)maxSpeed );
		//pPage->SetPropertyValue("Animation Speed",(long)defaultSpeed);
		pPage->SetPropertyValue("Animation Speed",this->m_GameOptions.animationSpeed);

		pPage->SetPropertyEditor( "Animation Speed", wxPGEditor_SpinCtrl );

	}
	pPage->Append( new wxPropertyCategory(wxT("Theme"),wxT("ThemeCategory")) );
	{
	
		wxPGChoices soc;
		soc.Add( "Default", 0 );
		soc.Add( "Green", 1 );
		soc.Add( "Blue", 2 );
		soc.Add( "Summer Flame", 3 );
		wxEnumProperty *choiceProp = new wxEnumProperty("presets",wxPG_LABEL,soc);
		

		pPage->Append(choiceProp );
		pPage->SetPropertyValue("presets",0);
		//pPage->SetPropertyValue("presets","Default");
	}
	pPage->Append( new wxPropertyCategory(wxT("Board"),wxT("BoardCategory")) );
	{
		wxBoolProperty *boolProp = new wxBoolProperty("Show Coordinates", wxPG_LABEL);
		pPage->Append(boolProp );
		//pPage->SetPropertyValue("Show Coordinates",true);
		pPage->SetPropertyValue("Show Coordinates",this->m_BoardOptions.showCoordinates);

		pPage->Append( new wxSystemColourProperty(wxT("Coordinate Color"),wxPG_LABEL, *wxWHITE ));
		pPage->SetPropertyValue("Coordinate Color",(this->m_BoardOptions.colText));
		

		wxPGChoices soc;
		soc.Add( "HightLights", 0 );
		soc.Add( "Outline Squares", 1 );
		soc.Add( "Show Arrows", 2 );
		soc.Add( "Arrows And Highlights", 3 );
		soc.Add( "Arrows And Outlines", 4 );
		soc.Add( "Highlights And Outlines", 5 );
		soc.Add( "None", 6 );

		wxEnumProperty *choiceProp = new wxEnumProperty("HightLight Moves",wxPG_LABEL,soc); 

		pPage->Append(choiceProp );
		//pPage->SetPropertyValue("HightLight Moves","Outline Squares");
		pPage->SetPropertyValue("HightLight Moves",(long)this->m_BoardOptions.showLastMoveRect);

		pPage->Append( new wxSystemColourProperty(wxT("Highlight Color1"),wxPG_LABEL, *wxRED ));
		pPage->Append( new wxSystemColourProperty(wxT("Highlight Color2"),wxPG_LABEL, *wxRED ));
		pPage->Append( new wxSystemColourProperty(wxT("Outline Color1"),wxPG_LABEL, *wxRED ));
		//pPage->SetPropertyValue("Outline Color1",WXVARIANT(this->m_BoardOptions.lastMoveRectColor));
		pPage->Append( new wxSystemColourProperty(wxT("Outline Color2"),wxPG_LABEL, *wxRED ));

		wxBoolProperty *boolProp54 = new wxBoolProperty("Show Arrows", wxPG_LABEL);
		pPage->Append(boolProp54 );
		pPage->SetPropertyValue("Show Arrows",this->m_BoardOptions.showArrows);
		pPage->Append( new wxSystemColourProperty(wxT("Arrow Color"),wxPG_LABEL, *wxRED ));
		pPage->SetPropertyValue("Arrow Color",(this->m_BoardOptions.arrow));

		
		wxPGChoices soc2;
		soc2.Add( "Thin", 0 );
		soc2.Add( "Fat", 1 );
		soc2.Add( "Simple", 2 );


		wxEnumProperty *choiceProp2 = new wxEnumProperty("Arrow Style",wxPG_LABEL,soc2); 

		pPage->Append(choiceProp2 );
		//pPage->SetPropertyValue("Arrow Style","Thin");
		pPage->SetPropertyValue("Arrow Style",(long)this->m_BoardOptions.arrowStyle);



		wxBoolProperty *boolProp5 = new wxBoolProperty("Use Board Bitmaps", wxPG_LABEL);
		pPage->Append(boolProp5 );
		pPage->SetPropertyValue("Use Board Bitmaps",this->m_BoardOptions.useBoardBitmaps);

		/*			showLines = true;
			useBorderBitmap = false;
			background
		*/

		wxBoolProperty *boolProp6 = new wxBoolProperty("Use Border Bitmap", wxPG_LABEL);
		pPage->Append(boolProp6 );
		//pPage->SetPropertyValue("Use Border Bitmap",false);
		pPage->SetPropertyValue("Use Border Bitmap",this->m_BoardOptions.useBorderBitmap);


		wxBoolProperty *boolProp8 = new wxBoolProperty("Use Background Image", wxPG_LABEL);
		pPage->Append(boolProp8 );
		//pPage->SetPropertyValue("Use Background Image",false);
		pPage->SetPropertyValue("Use Border Bitmap",this->m_BoardOptions.useBackgroundImage);

		wxBoolProperty *boolProp7 = new wxBoolProperty("Show Lines", wxPG_LABEL);
		pPage->Append(boolProp7 );
		//pPage->SetPropertyValue("Show Lines",true);
		pPage->SetPropertyValue("Show Lines",this->m_BoardOptions.showLines);

		pPage->Append( new wxSystemColourProperty(wxT("Background"),wxPG_LABEL, *wxBLUE ));
		pPage->SetPropertyValue("Background",WXVARIANT(this->m_BoardOptions.background));

		pPage->Append( new wxSystemColourProperty(wxT("Board Light Squares"),wxPG_LABEL, *wxBLUE ));
		pPage->SetPropertyValue("Board Light Squares",(this->m_BoardOptions.colWhite));
		pPage->Append( new wxSystemColourProperty(wxT("Board Dark Squares"),wxPG_LABEL, *wxBLUE ));
		pPage->SetPropertyValue("Board Dark Squares",(this->m_BoardOptions.colBlack));
		pPage->Append( new wxSystemColourProperty(wxT("Border Color"),wxPG_LABEL, *wxBLUE ));
		pPage->SetPropertyValue("Border Color",(this->m_BoardOptions.border));

		wxDirProperty *path = new wxDirProperty("White Squares Bitmap","");
		wxDirProperty *imgWhite = new wxDirProperty(wxT("White Squares Bitmap"),wxPG_LABEL);
		pPage->Append(imgWhite );


		wxDirProperty *imgBlack = new wxDirProperty(wxT("Black Squares Bitmap"),wxPG_LABEL);
		
		pPage->Append(imgBlack );	

		

		wxImageFileProperty *path3 = new wxImageFileProperty("Background Image",wxPG_LABEL);
		pPage->Append(path3 );


		wxImageFileProperty *path4 = new wxImageFileProperty("Border Image",wxPG_LABEL);
		pPage->Append(path4 );
		//pPage->SetPropertyValue("Background Image",this->m_BoardOptions.bo);

		wxPGChoices soc3;
		soc3.Add( "Alpha", 0 );
		soc3.Add( "Book", 1 );
		soc3.Add( "Fritz", 2 );
		soc3.Add( "Merida", 3);

		wxEnumProperty *choiceProp3 = new wxEnumProperty("Piece Style",wxPG_LABEL,soc3); 

		pPage->Append(choiceProp3 );
		pPage->SetPropertyValue("Piece Style",(long)this->m_BoardOptions.pieceType);


		wxPGChoices soc4;
		soc4.Add( "Plain", 0 );
		soc4.Add( "Pattern1", 1 );
		soc4.Add( "Pattern2", 2 );

		wxEnumProperty *choiceProp4 = new wxEnumProperty("Border Style",wxPG_LABEL,soc4); 

		pPage->Append(choiceProp4 );
	//	pPage->SetPropertyValue("Border Style","Plain");
		pPage->SetPropertyValue("Border Style",(long)this->m_BoardOptions.borderStyle);
//	int arrowStyle;	//- arrow style
//		wxString whiteBmp;	//- graphics
	//	wxString blackBmp;	//- colors
//		int pieceType; //- piece type
//		int borderStyle;			//- border style
			
//		wxString backgroundImage;	//- background image
//		bool showMouseCircle;	//- mouse

				wxBoolProperty *boolProp3 = new wxBoolProperty("Show Mouse In Cirle", wxPG_LABEL);
		pPage->Append(boolProp3 );
		pPage->SetPropertyValue("Show Mouse In Cirle",this->m_BoardOptions.showMouseCircle);

		wxBoolProperty *boolProp4 = new wxBoolProperty("Faster Mouse", wxPG_LABEL);
		pPage->Append(boolProp4 );
		pPage->SetPropertyValue("Faster Mouse",this->m_BoardOptions.fasterMouse);
	}
	pPage->Append( new wxPropertyCategory(wxT("Notation"),wxT("NotationCategory")) );
	{
		//wxFontDataProperty *pfont;

		pPage->Append( new wxFontProperty(wxT("Font"),wxPG_LABEL) );
		pPage->SetPropertyHelpString ( wxT("Font"), wxT("Editing this will change font used in the notation.") );

		pPage->Append( new wxSystemColourProperty(wxT("Text Color"),wxPG_LABEL, *wxBLACK ));
		pPage->SetPropertyValue("Text Color",WXVARIANT(this->m_NotationOptions.textColor));
		pPage->Append( new wxSystemColourProperty(wxT("Background Color"),wxPG_LABEL, *wxWHITE ));
		pPage->SetPropertyValue("Background Color",WXVARIANT(this->m_NotationOptions.backGroundColor));
		pPage->Append( new wxSystemColourProperty(wxT("Annotation Text Color"),wxPG_LABEL, *wxBLACK ));
		pPage->SetPropertyValue("Annotation Text Color",WXVARIANT(this->m_NotationOptions.AnnotateTextColor));	
		//	bool roundedBorderSelectionRect;
		//bool showAnnotationInSepWindow;
		//bool usePiecesInNotation;



		wxBoolProperty *boolProp2 = new wxBoolProperty("Show Annotation In Separate Window", wxPG_LABEL);
		pPage->Append(boolProp2 );
		pPage->SetPropertyValue("Show Annotation In Separate Window",this->m_NotationOptions.showAnnotationInSepWindow);


		wxBoolProperty *boolProp5 = new wxBoolProperty("Wait For Animation", wxPG_LABEL);
		pPage->Append(boolProp5 );
		pPage->SetPropertyValue("Wait For Animation",this->m_NotationOptions.updateMoveFast);
	
	
	}
	pPage->Append( new wxPropertyCategory(wxT("TreeView"),wxT("TreeViewCategory")) );
	{

		wxBoolProperty *boolProp2 = new wxBoolProperty("Show Score", wxPG_LABEL);
		pPage->Append(boolProp2 );
		pPage->SetPropertyValue("Show Score",this->m_TreePaneOptions.showScore);


	}
	pPage->Append( new wxPropertyCategory(wxT("General App"),wxT("GeneralAppCategory")) );
	{


		wxBoolProperty *boolProp6 = new wxBoolProperty("Show Active Tab", wxPG_LABEL);
		pPage->Append(boolProp6 );
		pPage->SetPropertyValue("Show Active Tab",false);

		wxBoolProperty *boolProp7 = new wxBoolProperty("Show Tooltips", wxPG_LABEL);
		pPage->Append(boolProp7 );
		pPage->SetPropertyValue("Show Tooltips",true);

		wxBoolProperty *boolProp8 = new wxBoolProperty("Lazy Update", wxPG_LABEL);
		pPage->Append(boolProp8 );
		pPage->SetPropertyValue("Lazy Update",this->m_GeneralAppOptions.updateScoreGraphOnKeyUp);


		wxIntProperty *intProp = new wxIntProperty("Autosave Frequency (minutes)", wxPG_LABEL);
		pPage->Append(intProp );
		long minSpeed = 1;
		long maxSpeed = 30;
		long defaultSpeed = 5;
		pPage->SetPropertyAttribute("Autosave Frequency (minutes)", wxPG_ATTR_MIN, (long)minSpeed );
		pPage->SetPropertyAttribute("Autosave Frequency (minutes)", wxPG_ATTR_MAX, (long)maxSpeed );
		//pPage->SetPropertyValue("Animation Speed",(long)defaultSpeed);
		pPage->SetPropertyValue("Autosave Frequency (minutes)",this->m_GeneralAppOptions.autosavePeriod);

		pPage->SetPropertyEditor( "Autosave Frequency (minutes)", wxPGEditor_SpinCtrl );
	
		wxBoolProperty *boolProp9 = new wxBoolProperty("Use Autosave", wxPG_LABEL);
		pPage->Append(boolProp9 );
		pPage->SetPropertyValue("Use Autosave",this->m_GeneralAppOptions.autosave);
		//wxBoolProperty *boolProp8 = new wxBoolProperty("Use Pieces In Notation", wxPG_LABEL);
		//pPage->Append(boolProp8 );
	}
	pPage->Append( new wxPropertyCategory(wxT("Engine Panel"),wxT("EnginePanelCategory")) );
	{
		wxBoolProperty *boolProp = new wxBoolProperty("Display Engine Custom Buttons", wxPG_LABEL);
		pPage->Append(boolProp );
		pPage->SetPropertyValue("Display Engine Custom Buttons",this->m_EnginePanelOptions.displayCustomEngineButtons);

		wxBoolProperty *boolProp2 = new wxBoolProperty("Show Info Bar Above Engine Output", wxPG_LABEL);
		pPage->Append(boolProp2 );
		pPage->SetPropertyValue("Show Info Bar Above Engine Output",this->m_EnginePanelOptions.showBarInNotation);
		//bool displayCustomEngineButtons;//	- display custom engine buttons
		//bool showBarInNotation;//	- show bar in notation
		//int notationType; // choice	- use LAN/SAN
		//bool showDepth;//	- depth for each move
	//pPage->SetPropertyValue("Notation Type","Standard Algebra Notation");

		wxBoolProperty *boolProp3 = new wxBoolProperty("Show Depth in Output String", wxPG_LABEL);
		pPage->Append(boolProp3 );
		pPage->SetPropertyValue("Show Depth in Output String",(long)this->m_EnginePanelOptions.showDepth);


	}
	
	
	
}


OptionsFrame::~OptionsFrame()
{
	delete m_pg;
//	delete sizer;
}


void OptionsFrame::OnSize(wxSizeEvent& event)
{
//	sizer->Fit(this);
	//DoSize();
	//Update();
	//Refresh(true);
	m_pg->SetSize(GetClientSize());
}


void OptionsFrame::OnPaint(wxPaintEvent& event)
{
	//sizer->Fit(this);
	//DoSize();
	//Update();
	//Refresh(true);
}


void OptionsFrame::OnPropertyGridChange(wxPropertyGridEvent& event)
{
    wxPGProperty* property = event.GetProperty();

    const wxString& name = property->GetName();

    // Properties store values internally as wxVariants, but it is preferred
    // to use the more modern wxAny at the interface level
    wxAny value = property->GetValue();

    // Don't handle 'unspecified' values
    if ( value.IsNull() )
        return;

	if( name == "Font" )
	{
        wxFont font = wxANY_AS(value, wxFont);
        wxASSERT( font.Ok() );

        m_pg->SetFont( font );
		wxFont f1 = font;
		wxFont f2 = font;
		wxFont f3 = font;
		f1.SetWeight(wxNORMAL);
		f2.SetWeight(wxBOLD);
		f3.SetStyle(wxITALIC);
		m_parent->my_canvas->SetFonts(f1, f2, f3);
		//m_parent->moveDataCanvas->m_font = f2;
		m_parent->SetFont(f2);
		m_parent->noteFrame->m_font = f2;
		//m_parent->m_listPanel->SetFont(f2);
		m_parent->m_listPanel->m_listCtrl->SetFont(f2);
		for( unsigned int i = 0; i < m_parent->numActivePipePanels; i++ )
		{
			m_parent->m_pipePanel[i]->GetAnalysisCanvas()->fontRich = f1;
			m_parent->m_pipePanel[i]->GetAnalysisCanvas()->fontSize = f1.GetPointSize();
			m_parent->m_pipePanel[i]->SetGUIFont(f1);
			
		}
		//this->m_NotationOptions.notationFont = font;
		
	}
	else if( name == "Always Promote To Queen" )
	{
		if( value.As<bool>())
		{
			this->m_GameOptions.alwaysPromoteToQueen = true;
		}
		else 
		{
			this->m_GameOptions.alwaysPromoteToQueen = false;
		}
	}
	else if( name == "Show Highlights At End" )
	{
		if( value.As<bool>())
		{
			this->m_GameOptions.highlightsAtEnd = true;
		}
		else 
		{
			this->m_GameOptions.highlightsAtEnd = false;
		}
	}
	else if( name == "Highlight Type" )
	{
		
	}
	else if( name == "Show MessageBox at End" )
	{
		if( value.As<bool>())
		{
			this->m_GameOptions.showMessageBoxAtGameEnd = true;
		}
		else 
		{
			this->m_GameOptions.showMessageBoxAtGameEnd = false;
		}
	}
	else if( name == "Move Entry Style" )
	{
		
	}
	else if( name == "Use Snaps" )
	{
		
		if( value.As<bool>())
		{
			this->m_GameOptions.useSnaps = true;
		}
		else 
		{
			this->m_GameOptions.useSnaps = false;
		}
	}	
	else if( name == "Show Engine Ouput")
	{
		if( value.As<bool>())
		{
			this->m_GameOptions.showEngineOutput = true;
		}
		else 
		{
			this->m_GameOptions.showEngineOutput = false;	
		}
	}
	else if( name =="Animate" )
	{
		if( value.As<bool>())
		{
			this->m_GameOptions.animate= true;
		}
		else 
		{
			this->m_GameOptions.animate = false;	
		}
	}
	else if( name == "Animation Speed")
	{
		this->m_GameOptions.animationSpeed = value.As<int>();
		this->m_parent->board_canvas->SetAnimationSpeed(m_GameOptions.animationSpeed);
	}
	else if( name == "presets")
	{

	}
	else if( name =="Show Coordinates" )
	{
		if( value.As<bool>())
		{
			this->m_BoardOptions.showCoordinates= true;
		}
		else 
		{
			this->m_BoardOptions.showCoordinates = false;	
		}
	}
	else if( name =="Coordinate Color" )
	{
		if( this->m_BoardOptions.showCoordinates== true && 
			this->m_BoardOptions.useBorderBitmap == false )
		{
			wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
			m_BoardOptions.colText =  cpv.m_colour;
			m_parent->board_canvas->colText = m_BoardOptions.colText;
			m_parent->board_canvas->newMaterials = true;
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name =="HightLight Moves" )
	{
		int intVal = value.As<int>();//.c_str().AsChar();
		if( intVal == 1)
		{
			m_BoardOptions.showLastMoveRect = true;
		}
		else 
		{
			m_BoardOptions.showLastMoveRect = false;
		}
		//..
	}
	else if( name == "Highlight Color1")
	{
		wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
        m_pg->GetGrid()->SetCellTextColour( cpv.m_colour );
	}
	else if( name == "Highlight Color2")
	{
		wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
		m_pg->GetGrid()->SetCellBackgroundColour( cpv.m_colour );
	}
	else if( name == "Outline Color1")
	{
		wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
        m_pg->GetGrid()->SetCellTextColour( cpv.m_colour );
	}	
	else if( name =="Outline Color2" )
	{
		if( m_BoardOptions.showLastMoveRect == true )
		{
			wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
			m_parent->board_canvas->SetLastMoveRectColour(cpv.m_colour);
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name == "Show Arrows" )
	{
		
		if( value.As<bool>())
		{
			this->m_BoardOptions.showArrows = true;
		}
		else 
		{
			this->m_BoardOptions.showArrows = false;
		}
	}
	else if( name =="Arrow Color" )
	{
		//if( m_BoardOptions.showArrows == true )
		{
			wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
			m_BoardOptions.arrow =  cpv.m_colour;
			m_parent->board_canvas->colArrow = m_BoardOptions.arrow;
			m_parent->board_canvas->newMaterials = true;
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name == "Arrow Style" )
	{
 
	}
	else if( name == "Use Board Bitmaps" )
	{
 
		if( value.As<bool>())
		{
			this->m_BoardOptions.useBoardBitmaps= true;
			m_parent->board_canvas->LoadDefaultDarkSquares();
			m_parent->board_canvas->LoadDefaultWhiteSquares();
			m_parent->board_canvas->newMaterials = true;
			m_parent->board_canvas->MyRefresh(true);
		}
		else 
		{
			m_parent->board_canvas->newMaterials = true;
			this->m_BoardOptions.useBoardBitmaps = false;	
			m_parent->board_canvas->colBlack = this->m_BoardOptions.colBlack;
			m_parent->board_canvas->colWhite = this->m_BoardOptions.colWhite;
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name == "Background" )
	{
		if( m_BoardOptions.useBackgroundImage == false )
		{
			wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
			m_BoardOptions.background =  cpv.m_colour;
			m_parent->board_canvas->SetBackgroundCol(m_BoardOptions.background);
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name == "Use Border Bitmap" )
	{
 
		if( value.As<bool>())
		{
			this->m_BoardOptions.useBorderBitmap= true;
			m_parent->board_canvas->MyRefresh(true);
		}
		else 
		{
			this->m_BoardOptions.useBorderBitmap = false;	
		}
	}
	else if( name == "Use Background Image" )
	{
 
		if( value.As<bool>())
		{
			this->m_BoardOptions.useBackgroundImage= true;
			m_parent->board_canvas->LoadDefaultBackground();
			m_parent->board_canvas->MyRefresh(true);
		}
		else 
		{
			this->m_BoardOptions.useBackgroundImage = false;	
		}
	}
	else if( name == "Show Lines" )
	{
 
		if( value.As<bool>())
		{
			this->m_BoardOptions.showLines= true;
			m_parent->board_canvas->MyRefresh(true);
		}
		else 
		{
			this->m_BoardOptions.showLines = false;	
		}
	}
	else if( name == "Border Color" )
	{
		if( m_BoardOptions.useBorderBitmap == false )
		{
			wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
			m_BoardOptions.border =  cpv.m_colour;
			m_parent->board_canvas->colBorder = m_BoardOptions.border;
			m_parent->board_canvas->newMaterials = true;
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name == "Board Dark Squares" )
	{
		if( m_BoardOptions.useBoardBitmaps == false )
		{
			wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
			m_BoardOptions.colBlack =  cpv.m_colour;
			m_parent->board_canvas->colBlack = m_BoardOptions.colBlack;
			m_parent->board_canvas->newMaterials = true;
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name == "Board Light Squares" )
	{
 		if( m_BoardOptions.useBoardBitmaps == false )
		{
			wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
			m_BoardOptions.colWhite =  cpv.m_colour;
			m_parent->board_canvas->colWhite = m_BoardOptions.colWhite;
			m_parent->board_canvas->newMaterials = true;
			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name =="White Squares Bitmap" )
	{
 		if( m_BoardOptions.useBoardBitmaps == true )
		{
			wxString cpv = wxANY_AS(value, wxString);
			m_parent->board_canvas->SetBoardWhiteSquare(cpv);
		}
	}
	else if( name == "Black Squares Bitmap")
	{
 		if( m_BoardOptions.useBoardBitmaps == true )
		{
			wxString cpv = wxANY_AS(value, wxString);
			m_parent->board_canvas->SetBoardBlackSquare(cpv);
		}
	}
	else if( name =="Background Image" )
	{
		if( m_BoardOptions.useBackgroundImage == true )
		{
			wxString cpv = wxANY_AS(value, wxString);
			m_parent->board_canvas->SetBackgroundImage(cpv);

			m_parent->board_canvas->MyRefresh(true);
		}
	}
	else if( name =="Border Image" )
	{
		if( m_BoardOptions.useBorderBitmap == true )
		{
			wxString cpv = wxANY_AS(value, wxString);
			m_parent->board_canvas->SetBorderImage(cpv);
			m_parent->board_canvas->MyRefresh(true);
		}
	}	
	else if( name == "Piece Style")
	{
		//wxAny value = property->GetValue();
		int cpv = value.As<int>();
	//value.As<wxString>().c_str().AsChar();
		//outPutString += name.c_str().AsChar();
		if( cpv == 0 )
		{
			wxString texName = "Alpha";
			m_parent->board_canvas->ChangeTextures(texName);
		}
		else if( cpv == 1 )
		{
			wxString texName = "Book";
			m_parent->board_canvas->ChangeTextures(texName);
		}
		else if( cpv == 2 )
		{
			wxString texName = "Fritz";
			m_parent->board_canvas->ChangeTextures(texName);
		}
		else if( cpv == 3 )
		{
			wxString texName = "Merida";
			m_parent->board_canvas->ChangeTextures(texName);
		}
	}
	else if( name == "Border Style" )
	{
		 
	}
	else if( name == "Show Mouse In Cirle")
	{
		if( value.As<bool>())
		{
			this->m_BoardOptions.showMouseCircle= true;
		}
		else 
		{
			this->m_BoardOptions.showMouseCircle = false;	
		}
	}
	else if( name == "Faster Mouse" )
	{
		if( value.As<bool>())
		{
			this->m_BoardOptions.fasterMouse= true;
		}
		else 
		{
			this->m_BoardOptions.fasterMouse = false;
		}
	}		
	else if( name =="Text Color" )
	{
		wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
        m_pg->GetGrid()->SetCellTextColour( cpv.m_colour );
	}
	else if( name == "Background Color")
	{
		
	}
	else if( name =="Selection Border" )
	{

		
	}
	else if(name == "Show Annotation In Separate Window")
	{
		if( value.As<bool>())
		{
			this->m_NotationOptions.showAnnotationInSepWindow= true;
			m_parent->m_mgr.GetPane("test10").Show();
			m_parent->m_mgr.Update();
		}
		else 
		{
			this->m_NotationOptions.showAnnotationInSepWindow = false;	
			m_parent->m_mgr.GetPane("test10").Hide();
			m_parent->m_mgr.Update();
		}
		m_parent->my_canvas->needsRefresh = true;
		m_parent->my_canvas->Refresh(true);
		//this->
	}	
	else if( name == "Annotation Text Color")
	{
		wxColourPropertyValue cpv = wxANY_AS(value, wxColourPropertyValue);
		this->m_NotationOptions.AnnotateTextColor = cpv.m_colour;
		m_parent->my_canvas->SetAnnotateTextColor(cpv.m_colour);
	}
	else if( name == "Wait For Animation")
	{
		if( value.As<bool>())
		{
			this->m_NotationOptions.updateMoveFast= false;
		}
		else 
		{
			this->m_NotationOptions.updateMoveFast = true;
		}
	}	
	else if( name =="Show Score" )
	{
		if( value.As<bool>())
		{
			this->m_TreePaneOptions.showScore= true;
		}
		else 
		{
			this->m_TreePaneOptions.showScore = false;
		}
	}	
	else if( name == "Show Active Tab")
	{
		if( value.As<bool>())
		{
			this->m_GeneralAppOptions.showActiveTab= true;
		}
		else 
		{
			this->m_GeneralAppOptions.showActiveTab = false;
		}
	}

	else if( name =="Show Tooltips" )
	{
		if( value.As<bool>())
		{
			this->m_GeneralAppOptions.showToolTips= true;
		}
		else 
		{
			this->m_GeneralAppOptions.showToolTips = false;
		}

	}
	else if( name =="Lazy Update" )
	{
		if( value.As<bool>())
		{
			this->m_GeneralAppOptions.updateScoreGraphOnKeyUp= true;
		}
		else 
		{
			this->m_GeneralAppOptions.updateScoreGraphOnKeyUp = false;
		}

	}
	else if( name == "Autosave Frequency (minutes)" )
	{
		this->m_GeneralAppOptions.autosavePeriod = value.As<int>();
		//this->m_parent->board_canvas->SetAnimationSpeed(m_GameOptions.animationSpeed);
		m_parent->SetAutosaveFrequency( this->m_GeneralAppOptions.autosavePeriod );
	}
	else if( name == "Use Autosave" )
	{
		if( value.As<bool>())
		{
			this->m_GeneralAppOptions.autosave= true;
			m_parent->ActivateAutosave();
		}
		else 
		{
			this->m_GeneralAppOptions.autosave = false;
			m_parent->DeactivateAutosave();
		}
	}
	else if( name == "Display Engine Custom Buttons")
	{
		if( value.As<bool>())
		{
			this->m_EnginePanelOptions.displayCustomEngineButtons = true;
		}
		else 
		{
			this->m_EnginePanelOptions.displayCustomEngineButtons = false;
		}
	}
	else if( name =="Show Info Bar Above Engine Output" )
	{

		if( value.As<bool>())
		{
			this->m_EnginePanelOptions.showBarInNotation = true;
		}
		else 
		{
			this->m_EnginePanelOptions.showBarInNotation = false;
		}
	}
	else if( name == "Notation Type" )
	{
	
		if( value.As<bool>())
		{
			this->m_EnginePanelOptions.notationType = true;
		}
		else 
		{
			this->m_EnginePanelOptions.notationType = false;
		}
	}

	else if( name == "Show Depth in Output String")
	{

		if( value.As<bool>())
		{
			this->m_EnginePanelOptions.showDepth = true;
		}
		else 
		{
			this->m_EnginePanelOptions.showDepth = false;
		}
	}

	ofstream of(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar()) + string("/settings/options.pts"));

	of << "GameOptions" <<endl;
	of << (WXVARIANT(m_GameOptions.alwaysPromoteToQueen)).GetString()<<endl;
	of << (WXVARIANT(m_GameOptions.animate)).GetString()<<endl;
	of << (WXVARIANT(m_GameOptions.animationSpeed)).GetString()<<endl;
	of << (WXVARIANT(m_GameOptions.highlightsAtEnd)).GetString()<<endl;
	of << (WXVARIANT(m_GameOptions.MoveEntryStyle)).GetString()<<endl;
	//
	of << (WXVARIANT(m_GameOptions.showEngineOutput)).GetString()<<endl;
	of << (WXVARIANT(m_GameOptions.showMessageBoxAtGameEnd)).GetString()<<endl;
	of << (WXVARIANT(m_GameOptions.useSnaps)).GetString()<<endl;

	of << "ThemeOptions" <<endl;
	of << (int)m_ThemeOptions.ribbonPrimary.Red()<< " " <<(int)m_ThemeOptions.ribbonPrimary.Green() << " " 
		<< (int)m_ThemeOptions.ribbonPrimary.Blue()<< " " <<(int)m_ThemeOptions.ribbonPrimary.Alpha() <<endl;
		
	
	of << (int)m_ThemeOptions.ribbonSecondary.Red()<< " " <<(int)m_ThemeOptions.ribbonSecondary.Green() << " " 
		<< (int)m_ThemeOptions.ribbonSecondary.Blue()<< " " <<(int)m_ThemeOptions.ribbonSecondary.Alpha() <<endl;
	//of << m_ThemeOptions.ribbonPrimary.Green()<<" ";
	//of << m_ThemeOptions.ribbonPrimary.Blue()<<" ";
	//of << m_ThemeOptions.ribbonPrimary.Alpha()<<endl;
	

	of<< "BoardOptions" <<endl;
	of << (WXVARIANT(m_BoardOptions.showArrows)).GetString()<<endl;
	of << (int)m_BoardOptions.arrow.Red()<< " " << (int)m_BoardOptions.arrow.Green()<< 
		" " << (int)m_BoardOptions.arrow.Blue()<< " " <<(int)m_BoardOptions.arrow.Alpha()<<endl;
	
	of << (WXVARIANT(m_BoardOptions.useBorderBitmap)).GetString()<<endl;
	of << (WXVARIANT(m_BoardOptions.useBoardBitmaps)).GetString()<<endl;
	of << (WXVARIANT(m_BoardOptions.showLastMoveRect)).GetString()<<endl;
	of << (WXVARIANT(m_BoardOptions.useBackgroundImage)).GetString()<<endl;
	of << (WXVARIANT(m_BoardOptions.borderStyle)).GetString()<<endl;
	of << (WXVARIANT(m_BoardOptions.fasterMouse)).GetString()<<endl;
	of << (int)m_BoardOptions.lastMoveRectColor.Red()<< " " << (int)m_BoardOptions.lastMoveRectColor.Green()<< 
		" " << (int)m_BoardOptions.lastMoveRectColor.Blue()<< " " <<(int)m_BoardOptions.lastMoveRectColor.Alpha()<<endl;
	
	of << (int)m_BoardOptions.colWhite.Red()<<" "<<(int)m_BoardOptions.colWhite.Green()<<" "<<
		 (int)m_BoardOptions.colWhite.Blue()<< " " <<(int)m_BoardOptions.colWhite.Alpha() <<endl;

	of << (int)m_BoardOptions.colBlack.Red()<< " " <<(int)m_BoardOptions.colBlack.Green() << " " 
		<< (int)m_BoardOptions.colBlack.Blue()<< " " <<(int)m_BoardOptions.colBlack.Alpha() <<endl;
	
	of << (int)m_BoardOptions.border.Red()<< " " <<(int)m_BoardOptions.border.Green() << " " 
		<< (int)m_BoardOptions.border.Blue()<< " " <<(int)m_BoardOptions.border.Alpha() <<endl;	
	
	of << (int)m_BoardOptions.background.Red()<< " " <<(int)m_BoardOptions.background.Green() << " " 
		<< (int)m_BoardOptions.background.Blue()<< " " <<(int)m_BoardOptions.background.Alpha() <<endl;

	of << (WXVARIANT(m_BoardOptions.pieceType)).GetString()<<endl;
	of << (WXVARIANT(m_BoardOptions.showCoordinates)).GetString()<<endl;
	of << (int)m_BoardOptions.colText.Red()<< " " <<(int)m_BoardOptions.colText.Green() << " " 
		<< (int)m_BoardOptions.colText.Blue()<< " " <<(int)m_BoardOptions.colText.Alpha() <<endl;
	
	of << (WXVARIANT(m_BoardOptions.showLines)).GetString()<<endl;
	of << (WXVARIANT(m_BoardOptions.showMouseCircle)).GetString()<<endl;
	
	of << "NotationOptions" <<endl;
	of << (WXVARIANT(m_NotationOptions.showAnnotationInSepWindow)).GetString()<<endl;

	of << (int)m_NotationOptions.backGroundColor.Red()<< " " <<(int)m_NotationOptions.backGroundColor.Green() << " " 
		<< (int)m_NotationOptions.backGroundColor.Blue()<< " " <<(int)m_NotationOptions.backGroundColor.Alpha() <<endl;
	
	of << (int)m_NotationOptions.AnnotateTextColor.Red()<< " " <<(int)m_NotationOptions.AnnotateTextColor.Green() << " " 
		<< (int)m_NotationOptions.AnnotateTextColor.Blue()<< " " <<(int)m_NotationOptions.AnnotateTextColor.Alpha() <<endl;
	
	of << (WXVARIANT(m_NotationOptions.updateMoveFast)).GetString()<<endl;

	of<< "TreePaneOptions" <<endl;
	of << (WXVARIANT(m_TreePaneOptions.showScore)).GetString()<<endl;

	of << "GeneralAppOptions" <<endl;
	of << (WXVARIANT(m_GeneralAppOptions.showActiveTab)).GetString()<<endl;
	of << (WXVARIANT(m_GeneralAppOptions.showToolTips)).GetString()<<endl;
	of << (WXVARIANT(m_GeneralAppOptions.updateScoreGraphOnKeyUp)).GetString()<<endl;
	of << (WXVARIANT(m_GeneralAppOptions.autosavePeriod)).GetString()<<endl;
	of << (WXVARIANT(m_GeneralAppOptions.autosave)).GetString()<<endl;


	of << "EnginePanelOptions" <<endl;
	of << (WXVARIANT(m_EnginePanelOptions.displayCustomEngineButtons)).GetString()<<endl;
	of << (WXVARIANT(m_EnginePanelOptions.showBarInNotation)).GetString()<<endl;
	of << (WXVARIANT(m_EnginePanelOptions.showDepth)).GetString()<<endl;

	of << "End" << endl;
	//of << m_ThemeOptions.ribbonSecondary.Green()<<" ";
//	of << m_ThemeOptions.ribbonSecondary.Blue()<<" ";
	//of << m_ThemeOptions.ribbonSecondary.Alpha()<<endl;
/*	of.write((char *)&m_GameOptions, sizeof(GameOptions));
	of.write((char *)&m_ThemeOptions, sizeof(ThemeOptions));
	of.write((char *)&m_BoardOptions, sizeof(BoardOptions));
	of.write((char *)&m_NotationOptions, sizeof(NotationOptions));
	of.write((char *)&m_TreePaneOptions, sizeof(TreePaneOptions));
	of.write((char *)&m_GeneralAppOptions, sizeof(GeneralAppOptions));
	of.write((char *)&m_EnginePanelOptions, sizeof(EnginePanelOptions));
	of.eof();*/
	of.close();




	this->m_parent->my_canvas->SetFocus();
}


void OptionsFrame::OnPropertyGridChanging(wxPropertyGridEvent& event)
{
}


inline wxColour GetColorFromString(string str)
{
	vector<string> vecStrings;
	
	istringstream iss(str);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
			back_inserter<vector<string> >(vecStrings));

	if( vecStrings.size () == 4 )
	{
		return wxColour( atoi(vecStrings[0].c_str() ),
						atoi(vecStrings[1].c_str() ), 
						atoi(vecStrings[2].c_str() ), 
						atoi(vecStrings[3].c_str() ));
	}
	else
	{
		return wxColour(255,255,255,255);
	}
}

bool OptionsFrame::LoadOptionsFile()
{
	ifstream ifs(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar()) + string("/settings/options.pts"));
	
	if( !ifs.is_open() )
	{
		return false;
	}

	while( ifs.good() )
	{
		
		string line;
	
		getline (ifs,line);
		if( line == "GameOptions")
		{
			string line2 = "";

			getline (ifs,line2);
			m_GameOptions.alwaysPromoteToQueen = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_GameOptions.animate = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_GameOptions.animationSpeed = atoi(line2.c_str());			
			getline (ifs,line2);
			m_GameOptions.highlightsAtEnd = (bool)atoi(line2.c_str());			
			getline (ifs,line2);
			m_GameOptions.MoveEntryStyle = atoi(line2.c_str());			
			getline (ifs,line2);

			m_GameOptions.showEngineOutput = (bool)atoi(line2.c_str());			
			getline (ifs,line2);
			m_GameOptions.showMessageBoxAtGameEnd = (bool)atoi(line2.c_str());			
			getline (ifs,line2);
			m_GameOptions.useSnaps = (bool)atoi(line2.c_str());			
			
		}
		getline (ifs,line);
		if( line == "ThemeOptions" )
		{
			string line2;
			getline (ifs,line2);
			m_ThemeOptions.ribbonPrimary= GetColorFromString(line2);
			getline (ifs,line2);
			m_ThemeOptions.ribbonPrimary= GetColorFromString(line2);
			//of << "Color Primary = " <<m_ThemeOptions.ribbonPrimary.GetRGBA()<<endl;
			//of << m_ThemeOptions.ribbonPrimary.Green()<<" ";
		//of << m_ThemeOptions.ribbonPrimary.Blue()<<" ";
			//of << m_ThemeOptions.ribbonPrimary.Alpha()<<endl;
			//of << "Color Secondary = " <<m_ThemeOptions.ribbonSecondary.GetRGBA()<<endl;
		}
		getline (ifs,line);
		if( line == "BoardOptions" )
		{
			string line2;
			getline (ifs,line2);
			m_BoardOptions.showArrows = (bool)atoi(line2.c_str());			
			getline (ifs,line2);
			m_BoardOptions.arrow = GetColorFromString(line2);			
			getline (ifs,line2);
			m_BoardOptions.useBorderBitmap = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_BoardOptions.useBoardBitmaps = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_BoardOptions.showLastMoveRect = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_BoardOptions.useBackgroundImage = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_BoardOptions.borderStyle = atoi(line2.c_str());
			getline (ifs,line2);
			m_BoardOptions.fasterMouse = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_BoardOptions.lastMoveRectColor = GetColorFromString(line2);
			getline (ifs,line2);
			m_BoardOptions.colWhite = GetColorFromString(line2);
			getline (ifs,line2);
			m_BoardOptions.colBlack = GetColorFromString(line2);
			getline (ifs,line2);
			m_BoardOptions.border = GetColorFromString(line2);
			getline (ifs,line2);
			m_BoardOptions.background = GetColorFromString(line2);
			getline (ifs,line2);
			m_BoardOptions.pieceType = atoi(line2.c_str());	
			getline (ifs,line2);
			m_BoardOptions.showCoordinates = (bool)atoi(line2.c_str());	
			getline (ifs,line2);
			m_BoardOptions.colText = GetColorFromString(line2);
			getline (ifs,line2);
			m_BoardOptions.showLines = (bool)atoi(line2.c_str());	
			getline (ifs,line2);
			m_BoardOptions.showMouseCircle = (bool)atoi(line2.c_str());	
			
		}

		getline (ifs,line);
		if( line == "NotationOptions" )	
		{
			string line2;
			getline (ifs,line2);
			m_NotationOptions.showAnnotationInSepWindow = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_NotationOptions.backGroundColor= GetColorFromString(line2);
			getline (ifs,line2);
			m_NotationOptions.AnnotateTextColor= GetColorFromString(line2);
			getline (ifs,line2);
			m_NotationOptions.updateMoveFast= (bool)atoi(line2.c_str());
			
		}

		getline (ifs,line);
		if( line == "TreePaneOptions" )
		{
			string line2;
			getline (ifs,line2);
			m_TreePaneOptions.showScore = (bool)atoi(line2.c_str());
		}

		getline (ifs,line);
		if( line == "GeneralAppOptions")
		{
			string line2;
			getline (ifs,line2);
			m_GeneralAppOptions.hideRibbonBar = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_GeneralAppOptions.showActiveTab = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_GeneralAppOptions.showToolTips = (bool)atoi(line2.c_str());

			getline (ifs,line2);
			m_GeneralAppOptions.updateScoreGraphOnKeyUp =(bool)atoi(line2.c_str());


			getline (ifs,line2);
			m_GeneralAppOptions.autosavePeriod =atoi(line2.c_str());

			getline (ifs,line2);
			m_GeneralAppOptions.autosave = (bool)atoi(line2.c_str());

		}

		getline (ifs,line);
		if( line == "EnginePanelOptions" )
		{
			string line2;
			getline (ifs,line2);
			m_EnginePanelOptions.displayCustomEngineButtons = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_EnginePanelOptions.showBarInNotation = (bool)atoi(line2.c_str());
			getline (ifs,line2);
			m_EnginePanelOptions.showDepth = (bool)atoi(line2.c_str());

			getline (ifs,line2);
			m_GeneralAppOptions.updateScoreGraphOnKeyUp =(bool)atoi(line2.c_str());

		}
	}
	ifs.close();

	return true;
}
