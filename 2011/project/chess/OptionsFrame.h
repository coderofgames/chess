#ifndef OPTIONS_FRAME_H
#define OPTIONS_FRAME_H

#include "headers.h"


class wxAdvImageFileProperty : public wxFileProperty
{
    WX_PG_DECLARE_PROPERTY_CLASS(wxAdvImageFileProperty)
public:

    wxAdvImageFileProperty( const wxString& label = wxPG_LABEL,
                            const wxString& name = wxPG_LABEL,
                            const wxString& value = wxEmptyString );
    virtual ~wxAdvImageFileProperty ();

    virtual void OnSetValue();  // Override to allow image loading.

    virtual bool IntToValue( wxVariant& variant, int number, int argFlags = 0 ) const;
    virtual bool OnEvent( wxPropertyGrid* propgrid, wxWindow* primary, wxEvent& event );
    virtual wxSize OnMeasureImage( int item ) const;
    virtual void OnCustomPaint( wxDC& dc,
                                const wxRect& rect, wxPGPaintData& paintdata );

    void LoadThumbnails( size_t n );

protected:
    wxImage*    m_pImage; // Temporary thumbnail data.

    static wxPGChoices ms_choices;

    int m_index; // Index required for choice behaviour.
};


class OptionsFrame : public wxScrolledWindow
{
public:

	OptionsFrame(MyFrame* parent);
	~OptionsFrame();


	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnPropertyGridChange(wxPropertyGridEvent& event);
	void OnPropertyGridChanging(wxPropertyGridEvent& event);	

	wxSizer *sizer;
	wxPropertyGridManager* m_pg;
	MyFrame* m_parent;


	bool LoadOptionsFile();

	class GameOptions
	{
	public:

		GameOptions()
		{
			alwaysPromoteToQueen = false;
			highlightsAtEnd = false;
			gameEndDisplayTypes = HIGHLIGHT_PCS;
			showMessageBoxAtGameEnd = false;
			MoveEntryStyle = DND;
			useSnaps = false;
			
			
			showEngineOutput = false;
			
			animate = false;
			animationSpeed = 0;
		};
		enum GameEndDisplayTypes {HIGHLIGHT_KING, HIGHLIGHT_PCS}; 
		enum MoveEntryStyle {DND, TWO_CLICK};
		
		bool alwaysPromoteToQueen;	//- always promote to Queen
		bool highlightsAtEnd;
		GameEndDisplayTypes gameEndDisplayTypes;	//- show message when game ends (choice)
		bool showMessageBoxAtGameEnd;
		int MoveEntryStyle;	//- move entry
		bool useSnaps;	//- snap pieces to good square?
		
		bool showEngineOutput;	//- show engine output
		bool animate;			//- animate
		int animationSpeed; //- animation speed
	};

	GameOptions m_GameOptions;

	class ThemeOptions
	{
	public:
		ThemeOptions()
		{
			ribbonPrimary = wxColor(0,0,255,255);
			ribbonPrimary = wxColor(128,128,255,255);
		}
		wxColor ribbonPrimary;
		wxColor ribbonSecondary;
	};

	ThemeOptions m_ThemeOptions;


	class BoardOptions
	{
	public:
		BoardOptions()
		{
			showCoordinates = true;
			showLines = true;
			useBorderBitmap = false;

			showArrows = false;
			arrowStyle = 0;
		pieceType = 0;
			borderStyle = 0;
			showMouseCircle = false;
			fasterMouse = false;
			useBoardBitmaps = false;
			useBackgroundImage = false;
			showLastMoveRect = true;
			background = wxColour(145,156,156,255);
			colWhite = wxColour(255,  255,  255, 255);
			colBlack = wxColour(150,  150,  255, 255);
			d3dModeRecreateDevice = false;
			
		}

		~BoardOptions()
		{
		/*	whiteBmp.Clear();
			blackBmp.Clear();

			backgroundImage.Clear();*/
		}

		bool showCoordinates;	//- show coordinates
		bool showLines;
		bool showArrows;
		int arrowStyle;	//- arrow style

		bool useBoardBitmaps;
		bool useBorderBitmap;
		bool useBackgroundImage;
		wxColour colWhite;
		wxColour colBlack;
	//	wxString whiteBmp;	//- graphics
	//	wxString blackBmp;	//- colors
		wxColour background;
		wxColour border;
		wxColour arrow;
		wxColour colText;

		int pieceType; //- piece type
		int borderStyle;			//- border style
			
	//	wxString backgroundImage;	//- background image
		bool showMouseCircle;	//- mouse
		bool fasterMouse;

		bool showLastMoveRect;

		bool d3dModeRecreateDevice;

		wxColour lastMoveRectColor;
	};

	BoardOptions m_BoardOptions;

	class NotationOptions
	{
	public:
		NotationOptions()
		{
			showAnnotationInSepWindow = true;

			AnnotateTextColor = *wxBLUE;
			dynamicClassifyECO = false;
			//notationFont = wxFont(10, wxMODERN, wxNORMAL, wxBOLD, false);
			updateMoveFast = false;
		};

		~NotationOptions()
		{
			//notationFont.FreeResource(true);
		}
	
		//- font
		wxColor textColor;	//- colors
		wxColor AnnotateTextColor;	//- colors
			
		wxColor backGroundColor;	//- selector style
		bool showAnnotationInSepWindow;
		bool usePiecesInNotation;
		bool dynamicClassifyECO;
		bool updateMoveFast;
		//- show annotation in sep window
		//- pieces in notation (font etc) 
	};

	NotationOptions m_NotationOptions;

	class TreePaneOptions
	{
	public:
		TreePaneOptions()
		{

			showScore = false;

		}
			//- appearance

		bool showScore;			//- move score
			//- navigation options



	};

	TreePaneOptions m_TreePaneOptions;


	class GeneralAppOptions
	{
	public:
		GeneralAppOptions()
		{
			useRibbonBar = true;
			hideRibbonBar = false;
			showActiveTab = false;
			showToolTips = true;
			piecesInNotation = false;
			autosavePeriod = 5;
			autosave = true;
		}
		bool showGeneralToolbars;		//	- toolbars?
			//- show window (x)
		bool useRibbonBar;	//- ribbon bar or menu
		bool hideRibbonBar;	//- hide ribbon bar
		bool showActiveTab;	//- show active tab
		bool showToolTips;	//- show tool tips
		bool piecesInNotation;	//- pieces in notation (font etc) 
		bool updateScoreGraphOnKeyUp;
		int autosavePeriod;
		bool autosave;
	
	};

	GeneralAppOptions m_GeneralAppOptions;


	class EnginePanelOptions
	{
	public:
		EnginePanelOptions()
		{
			displayCustomEngineButtons = false;
			showBarInNotation = true;
			notationType = 0;
			showDepth = false;
		};
		//- engine panel settings
		bool displayCustomEngineButtons;//	- display custom engine buttons
		bool showBarInNotation;//	- show bar in notation
		int notationType; // choice	- use LAN/SAN
		bool showDepth;//	- depth for each move
	};

	EnginePanelOptions m_EnginePanelOptions;

private:
	DECLARE_EVENT_TABLE()
};



#endif