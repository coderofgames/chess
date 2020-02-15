/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.cpp
// Purpose:     MDI sample
// Author:      Julian Smart
// Modified by: 2008-10-31 Vadim Zeitlin: big clean up
// Created:     04/01/98
// RCS-ID:      $Id: mdi.cpp 64940 2010-07-13 13:29:13Z VZ $
// Copyright:   (c) 1997 Julian Smart
//              (c) 2008 Vadim Zeitlin
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ===========================================================================
// declarations
// ===========================================================================

// ---------------------------------------------------------------------------
// headers
// ---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
    #include "wx/mdi.h"
#endif

#include "wx/toolbar.h"

#if !defined(__WXMSW__)
    #include "../sample.xpm"
    #include "chart.xpm"
#endif

#include "bitmaps/new.xpm"
#include "bitmaps/open.xpm"
#include "bitmaps/save.xpm"
#include "bitmaps/copy.xpm"
#include "bitmaps/cut.xpm"
#include "bitmaps/paste.xpm"
#include "bitmaps/print.xpm"
#include "bitmaps/help.xpm"


#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"

#include "wx/aui/aui.h"


#include "wx/stockitem.h"
#include "wx/msw/private.h"

#include <string.h>
#include "PgnLoad.h"

#include "wx/wxprec.h"
//#include "Clipboard.h"
#include "wx/clipbrd.h"
//#include "Book.h"
//#include "Objects.h"
//#include "UCIEngine.h"
//#include "ChessRules.h"
//#include "Repository.h"
//#include "ByteBoard.h"

//using namespace thc;

//Objects objs;


#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/app.h"
#include "wx/grid.h"
#include "wx/treectrl.h"
#include "wx/spinctrl.h"
#include "wx/artprov.h"
#include "wx/clipbrd.h"
#include "wx/image.h"
#include "wx/colordlg.h"
#include "wx/wxhtml.h"
#include "wx/imaglist.h"
#include "wx/dataobj.h"
#include "wx/dcclient.h"
#include "wx/bmpbuttn.h"
#include "wx/menu.h"
#include "wx/toolbar.h"
#include "wx/statusbr.h"
#include "wx/msgdlg.h"
#include "wx/textdlg.h"

#include "wx/aui/aui.h"
#include "../sample.xpm"

#include "wx/defs.h"
#include "wx/dcclient.h"
#include "wx/wfstream.h"

#if wxUSE_ZLIB
#include "wx/zstream.h"
#endif
//#include <gl/glew.h>
//#include "wx/glcanvas.h"
//#include "dxfrenderer.h"
//#include "Image.h"
//#include "ARB_Multisample.h"
//#include "Drawable.h"

#ifdef __WXMSW__
#include "wx/msw/private.h"
#endif

//#include "MyGLCanvas.h"


#include "ListBox.h"


#ifdef __DARWIN__
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

#include "wx/sstream.h"

#include "wx/richtext/richtextctrl.h"
#include "wx/richtext/richtextstyles.h"
#include "wx/richtext/richtextxml.h"
#include "wx/richtext/richtexthtml.h"
#include "wx/richtext/richtextformatdlg.h"
#include "wx/richtext/richtextsymboldlg.h"
#include "wx/richtext/richtextstyledlg.h"
#include "wx/richtext/richtextprint.h"


#include "wx/colordlg.h"
#include "wx/image.h"
#include "wx/artprov.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/filename.h"

#include "MyPipePanel.h"
#include "MyListPanel.h"

#include "VariationSelectionFrame.h"
#include "NotationFrame.h"
#include "OptionsFrame.h"

#include "clockthread.h"

#include "lcdwindow.h"
#include "GameClock.h"

#include "BoardCanvas.h"
#include "NotationCanvas.h"
#include "TreeCanvas.h"

#include "GameInfoDialog.h"
#include "EngineListDialog.h"

#include "PositionSetupFrame.h"

#include "MiscUtils.h"

#include "GameDataViewPanel.h"
#include "NewGameDialog.h"

#include "GameAnalyser.h"
#include "EngineManager.h"
#include "MoveDataCanvas.h"
//#include "ProcessReadThread.h"



//=============================================================================

class wxSizeReportCtrl;

// replace this 0 with 1 to build the sample using the generic MDI classes (you
// may also need to add src/generic/mdig.cpp to the build)
#if 0
    #include "wx/generic/mdig.h"
    #define wxMDIParentFrame wxGenericMDIParentFrame
    #define wxMDIChildFrame wxGenericMDIChildFrame
    #define wxMDIClientWindow wxGenericMDIClientWindow
#endif

#include "mdi.h"

IMPLEMENT_APP(MyApp)

// ---------------------------------------------------------------------------
// event tables
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(MDIParentFrame, wxMDIParentFrame)
    EVT_MENU(wxID_ABOUT, MDIParentFrame::OnAbout)
    EVT_MENU(wxID_NEW, MDIParentFrame::OnNewWindow)
	EVT_MENU(ID_TOGGLE_MAXIMIZE, MDIParentFrame::ToggleMaximize)
    EVT_MENU(MDI_FULLSCREEN, MDIParentFrame::OnFullScreen)
    EVT_MENU(wxID_EXIT, MDIParentFrame::OnQuit)

    EVT_MENU(wxID_CLOSE_ALL, MDIParentFrame::OnCloseAll)
	EVT_RIBBONBAR_TAB_LEFT_DCLICK(ID_RIBBON_BAR_TAB_LEFT_CLICKED, MDIParentFrame::RibbonTabDoubleClick)		
    EVT_CLOSE(MDIParentFrame::OnClose)
	
	EVT_RIBBONBUTTONBAR_CLICKED(ID_NEW_GAME, MDIParentFrame::OnNewGameButton)
	EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_NEW_GAME, MDIParentFrame::OnNewGameDropDown)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_SWAP_SIDES, MDIParentFrame::OnSwapSides)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_LOAD_MEDIA_FROM_DISK, MDIParentFrame::OnLoadMediaFromDisk)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_LOAD_MEDIA_FROM_URL, MDIParentFrame::OnLoadMediaFromURL)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DEFAULT_PROVIDER, MDIParentFrame::OnDefaultProvider)

EVT_RIBBONBUTTONBAR_CLICKED(ID_AUI_PROVIDER, MDIParentFrame::OnAUIProvider)
EVT_RIBBONBUTTONBAR_CLICKED(ID_MSW_PROVIDER, MDIParentFrame::OnMSWProvider)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SELECTION_EXPAND_H, MDIParentFrame::OnSelectionExpandHButton)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SELECTION_EXPAND_V, MDIParentFrame::OnSelectionExpandVButton)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SELECTION_CONTRACT, MDIParentFrame::OnSelectionContractButton)

//EVT_RIBBONBUTTONBAR_CLICKED(ID_EDIT_PGN_HEADER, MDIParentFrame::OnEditPGNHeader)
EVT_RIBBONTOOLBAR_CLICKED(ID_EDIT_PGN_HEADER, MDIParentFrame::OnEditPGNHeader)
EVT_RIBBONTOOLBAR_CLICKED(ID_FLIP_BOARD, MDIParentFrame::OnFlipBoard)
EVT_RIBBONBUTTONBAR_CLICKED(ID_EDIT_ANNOTATION, MDIParentFrame::OnEditAnnotation)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DELETE_REMAINING_MOVES, MDIParentFrame::OnDeleteRemainingMoves)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DELETE_VARIATION, MDIParentFrame::OnDeleteVariation)
EVT_RIBBONBUTTONBAR_CLICKED(ID_PROMOTE_VARIATION, MDIParentFrame::OnPromoteVariation)

EVT_RIBBONBUTTONBAR_CLICKED(ID_PLAY_THROUGH_GAME, MDIParentFrame::OnPlayThroughGame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_INFO, MDIParentFrame::OnInfo)
	
EVT_RIBBONBUTTONBAR_CLICKED(ID_CIRCLE, MDIParentFrame::OnCircleButton)
EVT_RIBBONBUTTONBAR_CLICKED(ID_CROSS, MDIParentFrame::OnCrossButton)
EVT_RIBBONBUTTONBAR_CLICKED(ID_TRIANGLE, MDIParentFrame::OnTriangleButton)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_SQUARE, MDIParentFrame::OnSquareButton)
EVT_RIBBONTOOLBAR_CLICKED(ID_SQUARE, MDIParentFrame::OnSquareButton)
//EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_SQUARE, MDIParentFrame::OnGameDropdown)

EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_CLOCK, MDIParentFrame::OnClockDropdown)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_SQUARE, MDIParentFrame::OnGameDropdown)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_TRIANGLE, MDIParentFrame::OnTriangleDropdown)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_POLYGON, MDIParentFrame::OnPolygonDropdown)
EVT_RIBBONGALLERY_HOVER_CHANGED(ID_PRIMARY_COLOUR, MDIParentFrame::OnHoveredColourChange)
EVT_RIBBONGALLERY_HOVER_CHANGED(ID_SECONDARY_COLOUR, MDIParentFrame::OnHoveredColourChange)
EVT_RIBBONGALLERY_SELECTED(ID_PRIMARY_COLOUR, MDIParentFrame::OnPrimaryColourSelect)
EVT_RIBBONGALLERY_SELECTED(ID_SECONDARY_COLOUR, MDIParentFrame::OnSecondaryColourSelect)
EVT_RIBBONTOOLBAR_CLICKED(wxID_NEW, MDIParentFrame::OnNew)
EVT_RIBBONTOOLBAR_CLICKED(ID_OPEN, MDIParentFrame::OnOpen)
EVT_RIBBONTOOLBAR_CLICKED(ID_SAVE, MDIParentFrame::OnSave)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(wxID_NEW, MDIParentFrame::OnNewDropdown)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_OPEN, MDIParentFrame::OnOpenDropdown)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_SAVE, MDIParentFrame::OnSaveDropdown)
EVT_RIBBONTOOLBAR_CLICKED(wxID_PRINT, MDIParentFrame::OnPrint)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(wxID_PRINT, MDIParentFrame::OnPrintDropdown)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(wxID_REDO, MDIParentFrame::OnRedoDropdown)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(wxID_UNDO, MDIParentFrame::OnUndoDropdown)
EVT_RIBBONTOOLBAR_CLICKED(ID_POSITION_LEFT, MDIParentFrame::OnPositionLeft)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_POSITION_LEFT, MDIParentFrame::OnPositionLeftDropdown)
EVT_RIBBONTOOLBAR_CLICKED(ID_POSITION_TOP, MDIParentFrame::OnPositionTop)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_POSITION_TOP, MDIParentFrame::OnPositionTopDropdown)
EVT_BUTTON(ID_PRIMARY_COLOUR, MDIParentFrame::OnColourGalleryButton)
EVT_BUTTON(ID_SECONDARY_COLOUR, MDIParentFrame::OnColourGalleryButton)
EVT_MENU(ID_POSITION_LEFT, MDIParentFrame::OnPositionLeftIcons)
EVT_MENU(ID_POSITION_LEFT_LABELS, MDIParentFrame::OnPositionLeftLabels)
EVT_MENU(ID_POSITION_LEFT_BOTH, MDIParentFrame::OnPositionLeftBoth)
EVT_MENU(ID_POSITION_TOP, MDIParentFrame::OnPositionTopLabels)
EVT_MENU(ID_POSITION_TOP_ICONS, MDIParentFrame::OnPositionTopIcons)
EVT_MENU(ID_POSITION_TOP_BOTH, MDIParentFrame::OnPositionTopBoth)
EVT_MENU(ID_POSITION_TOP_ICONS, MDIParentFrame::OnPositionTopIcons)
EVT_MENU(ID_LOAD_PGN, MDIParentFrame::OnLoadPGN)
EVT_MENU(ID_ADD_PGN, MDIParentFrame::OnAddPGN)
EVT_MENU(ID_PASTE_PGN, MDIParentFrame::OnPastePGN)
EVT_MENU(ID_MERGE_PGN_FROM_CLIPBOARD, MDIParentFrame::OnMergePGNFromClipboard)
EVT_MENU(ID_MERGE_PGN_FROM_DISK, MDIParentFrame::OnMergePGNFromDisk)
EVT_MENU(ID_COPY_PGN_TO_CLIPBOARD, MDIParentFrame::OnCopyPGNToClipBoard)
EVT_MENU(ID_COPY_FEN_TO_CLIPBOARD, MDIParentFrame::OnCopyFENToClipBoard)
EVT_MENU(ID_COPY_VARIATION_TO_CLIPBOARD, MDIParentFrame::OnCopyVariationToClipBoard)
EVT_MENU(ID_SAVE_GAME_LIST_AS_DATABASE, MDIParentFrame::OnSaveGameListAsDatabase)
EVT_MENU(ID_PASTE_FEN, MDIParentFrame::OnPasteFEN)
EVT_MENU(ID_SAVE_PGN, MDIParentFrame::OnSavePGN)

EVT_MENU(ID_NEW_GAME, MDIParentFrame::OnNewGame)
EVT_MENU(ID_SETUP_POSITION, MDIParentFrame::OnSetupPosition)

EVT_MENU(ID_CLOCK_SET_TIME_CONTROLS, MDIParentFrame::OnClockSetTimeControls)
EVT_MENU(ID_CLOCK_RESET, MDIParentFrame::OnClockReset)
EVT_MENU(ID_CLOCK_SET_TIME, MDIParentFrame::OnClockSetTime)


EVT_IDLE(MDIParentFrame::OnIdle)
END_EVENT_TABLE()

// Note that wxID_NEW and wxID_ABOUT commands get passed
// to the parent window for processing, so no need to
// duplicate event handlers here.
BEGIN_EVENT_TABLE(MyFrame, wxMDIChildFrame)
    EVT_MENU(wxID_CLOSE, MyFrame::OnClose)
    EVT_MENU(MDI_REFRESH, MyFrame::OnRefresh)
    EVT_MENU(MDI_CHANGE_TITLE, MyFrame::OnChangeTitle)
    EVT_MENU(MDI_CHANGE_POSITION, MyFrame::OnChangePosition)
    EVT_MENU(MDI_CHANGE_SIZE, MyFrame::OnChangeSize)

#if wxUSE_CLIPBOARD
    EVT_MENU(wxID_PASTE, MyFrame::OnPaste)
    EVT_UPDATE_UI(wxID_PASTE, MyFrame::OnUpdatePaste)
#endif // wxUSE_CLIPBOARD

	EVT_MENU(MyFrame::ID_LoadPGN, MyFrame::OnLoadPGN)
	EVT_MENU(MyFrame::ID_LoadPGNFromClipBoard, MyFrame::OnLoadPgnFromClipBoard)
	EVT_MENU(MyFrame::ID_MergePGNFromClipBoard, MyFrame::OnMergePGNFromClipBoard)
	EVT_MENU(MyFrame::ID_MergePGNFromDisk, MyFrame::OnMergePGNFromDisk)
	EVT_MENU(MyFrame::ID_CopyPGNToClipBoard, MyFrame::OnCopyPGNToClipBoard)
	EVT_MENU(MyFrame::ID_CopyFENToClipBoard, MyFrame::CopyFenToClipBoard)
	EVT_MENU(MyFrame::ID_SavePGN, MyFrame::OnSavePGN)
    EVT_SIZE(MyFrame::OnSize)
    EVT_MOVE(MyFrame::OnMove)

    EVT_CLOSE(MyFrame::OnCloseWindow)
   EVT_ERASE_BACKGROUND(MyFrame::OnEraseBackground)
 //   EVT_SIZE(MDIParentFrame::OnSize)
    EVT_MENU(MyFrame::ID_CreateTree, MyFrame::OnCreateTree)
    EVT_MENU(MyFrame::ID_CreateGrid, MyFrame::OnCreateGrid)
    EVT_MENU(MyFrame::ID_CreateText, MyFrame::OnCreateText)
    EVT_MENU(MyFrame::ID_CreateHTML, MyFrame::OnCreateHTML)
    EVT_MENU(MyFrame::ID_CreateSizeReport, MyFrame::OnCreateSizeReport)
    EVT_MENU(MyFrame::ID_CreateNotebook, MyFrame::OnCreateNotebook)
    EVT_MENU(MyFrame::ID_CreatePerspective, MyFrame::OnCreatePerspective)
    EVT_MENU(MyFrame::ID_CopyPerspectiveCode, MyFrame::OnCopyPerspectiveCode)
    EVT_MENU(ID_AllowFloating, MyFrame::OnManagerFlag)
    EVT_MENU(ID_TransparentHint, MyFrame::OnManagerFlag)
    EVT_MENU(ID_VenetianBlindsHint, MyFrame::OnManagerFlag)
    EVT_MENU(ID_RectangleHint, MyFrame::OnManagerFlag)
    EVT_MENU(ID_NoHint, MyFrame::OnManagerFlag)
    EVT_MENU(ID_HintFade, MyFrame::OnManagerFlag)
    EVT_MENU(ID_NoVenetianFade, MyFrame::OnManagerFlag)
    EVT_MENU(ID_TransparentDrag, MyFrame::OnManagerFlag)
    EVT_MENU(ID_LiveUpdate, MyFrame::OnManagerFlag)
    EVT_MENU(ID_AllowActivePane, MyFrame::OnManagerFlag)
    EVT_MENU(ID_NotebookTabFixedWidth, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookNoCloseButton, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookCloseButton, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookCloseButtonAll, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookCloseButtonActive, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAllowTabMove, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAllowTabExternalMove, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAllowTabSplit, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookScrollButtons, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookWindowList, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookArtGloss, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookArtSimple, MyFrame::OnNotebookFlag)
    EVT_MENU(ID_NotebookAlignTop,     MyFrame::OnTabAlignment)
    EVT_MENU(ID_NotebookAlignBottom,  MyFrame::OnTabAlignment)
    EVT_MENU(ID_NoGradient, MyFrame::OnGradient)
    EVT_MENU(ID_VerticalGradient, MyFrame::OnGradient)
    EVT_MENU(ID_HorizontalGradient, MyFrame::OnGradient)
    EVT_MENU(ID_Settings, MyFrame::OnSettings)
    EVT_MENU(ID_CustomizeToolbar, MyFrame::OnCustomizeToolbar)
    EVT_MENU(ID_GridContent, MyFrame::OnChangeContentPane)
    EVT_MENU(ID_TreeContent, MyFrame::OnChangeContentPane)
    EVT_MENU(ID_TextContent, MyFrame::OnChangeContentPane)
    EVT_MENU(ID_SizeReportContent, MyFrame::OnChangeContentPane)
    EVT_MENU(ID_HTMLContent, MyFrame::OnChangeContentPane)
    EVT_MENU(ID_NotebookContent, MyFrame::OnChangeContentPane)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
	EVT_MENU(ID_SampleItem +1, MyFrame::OnToolLeftClick)
	EVT_MENU(ID_SampleItem +2, MyFrame::OnToolLeftClick)
	EVT_MENU(ID_SampleItem +3, MyFrame::OnToolLeftClick)
	EVT_MENU(ID_SampleItem +4, MyFrame::OnToolLeftClick)
	EVT_MENU(ID_SampleItem +5, MyFrame::OnToolLeftClick)
    EVT_UPDATE_UI(ID_NotebookTabFixedWidth, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookNoCloseButton, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookCloseButton, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookCloseButtonAll, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookCloseButtonActive, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookAllowTabMove, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookAllowTabExternalMove, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookAllowTabSplit, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookScrollButtons, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NotebookWindowList, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_AllowFloating, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_TransparentHint, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_VenetianBlindsHint, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_RectangleHint, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NoHint, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_HintFade, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NoVenetianFade, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_TransparentDrag, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_LiveUpdate, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_NoGradient, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_VerticalGradient, MyFrame::OnUpdateUI)
    EVT_UPDATE_UI(ID_HorizontalGradient, MyFrame::OnUpdateUI)
    EVT_MENU_RANGE(MyFrame::ID_FirstPerspective, MyFrame::ID_FirstPerspective+1000,
                   MyFrame::OnRestorePerspective)
	//EVT_SET_FOCUS(MyFrame::OnSetFocus)
    EVT_AUITOOLBAR_TOOL_DROPDOWN(ID_DropDownToolbarItem, MyFrame::OnDropDownToolbarItem)
    EVT_AUI_PANE_CLOSE(MyFrame::OnPaneClose)
    EVT_AUINOTEBOOK_ALLOW_DND(wxID_ANY, MyFrame::OnAllowNotebookDnD)
    EVT_AUINOTEBOOK_PAGE_CLOSE(wxID_ANY, MyFrame::OnNotebookPageClose)
    EVT_AUINOTEBOOK_PAGE_CLOSED(wxID_ANY, MyFrame::OnNotebookPageClosed)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyFrame::EventHandler, wxEvtHandler)
    EVT_MENU(MDI_REFRESH, MyFrame::EventHandler::OnRefresh)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(wxSizeReportCtrl, wxControl)
    EVT_PAINT(wxSizeReportCtrl::OnPaint)
    EVT_SIZE(wxSizeReportCtrl::OnSize)
    EVT_ERASE_BACKGROUND(wxSizeReportCtrl::OnEraseBackground)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(SettingsPanel, wxPanel)
    EVT_SPINCTRL(ID_PaneBorderSize, SettingsPanel::OnPaneBorderSize)
    EVT_SPINCTRL(ID_SashSize, SettingsPanel::OnSashSize)
    EVT_SPINCTRL(ID_CaptionSize, SettingsPanel::OnCaptionSize)
    EVT_BUTTON(ID_BackgroundColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_SashColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_InactiveCaptionColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_InactiveCaptionGradientColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_InactiveCaptionTextColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_ActiveCaptionColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_ActiveCaptionGradientColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_ActiveCaptionTextColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_BorderColor, SettingsPanel::OnSetColor)
    EVT_BUTTON(ID_GripperColor, SettingsPanel::OnSetColor)
END_EVENT_TABLE()



#include "align_center.xpm"
#include "align_left.xpm"
#include "align_right.xpm"
#include "aui_style.xpm"
#include "auto_crop_selection.xpm"
#include "auto_crop_selection_small.xpm"
#include "icons/shlyap/3/circle.xpm"
#include "circle_small.xpm"
#include "colours.xpm"
#include "icons/shlyap/2/cross.xpm"
#include "empty.xpm"
#include "expand_selection_v.xpm"
#include "expand_selection_h.xpm"
#include "eye.xpm"
#include "icons/shlyap/2/hexagon.xpm"
#include "msw_style.xpm"
#include "position_left_small.xpm"
#include "position_top_small.xpm"
#include "ribbon.xpm"
#include "selection_panel.xpm"
#include "images/chessboard_icon.xpm"
#include "icons/shlyap/2/triangle.xpm"
// ===========================================================================
// implementation
// ===========================================================================

// ---------------------------------------------------------------------------
// MyApp
// ---------------------------------------------------------------------------

// Initialise this in OnInit, not statically
bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    // Create the main frame window

    MDIParentFrame *frame = new MDIParentFrame;

    frame->Show(true);

    return true;
}

// ---------------------------------------------------------------------------
// MDIParentFrame
// ---------------------------------------------------------------------------

// Define my frame constructor
MDIParentFrame::MDIParentFrame()
       : wxMDIParentFrame(NULL, wxID_ANY, "wxWidgets MDI Sample",
                          wxDefaultPosition, wxSize(500, 400))
{
	this->Maximize(true);
	bFullScreen = false;

	wxInitAllImageHandlers();

    m_ribbon = new wxRibbonBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS |wxRIBBON_BAR_SHOW_PANEL_MINIMISE_BUTTONS );

	
	wxBitmap bmpClipBoard;
	wxBitmap bmpGame;
	wxBitmap bmpOpen;
	wxBitmap bmpSave;
	wxBitmap bmpMerge;
	wxBitmap bmpFullScreen;

	wxBitmap bmpEditAnnotation;
	wxBitmap bmpEditPGNHeader;
	wxBitmap bmpDeleteRemainingMoves;
	wxBitmap bmpDeleteVariation;
	wxBitmap bmpPromoteVariation;

	
	wxBitmap bmpFlipBoard;

	wxBitmap bmpMediaFromDisk;
	wxBitmap movieFromWeb;

	wxBitmap info;
	wxBitmap swapSides;
	
	wxBitmap bmpClock;

	wxBitmap playThroughGame;

	/*
		ID_EDIT_PGN_HEADER,
		ID_EDIT_ANNOTATION,
		ID_DELETE_REMAINING_MOVES,
		ID_DELETE_VARIATION,
		ID_PROMOTE_VARIATION,
	*/
	//"icons\pixophilia\2"
	//"icons\shlyap\2\clipboard.png"
	bmpClipBoard.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/2/clipboard.png", wxBITMAP_TYPE_PNG);
	bmpFullScreen.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/2/full_screen.png", wxBITMAP_TYPE_PNG);



	bmpOpen.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/folder.png", wxBITMAP_TYPE_PNG);
	bmpSave.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/save.png", wxBITMAP_TYPE_PNG);

	bmpEditAnnotation.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/Bubble.png", wxBITMAP_TYPE_PNG);
	bmpEditPGNHeader.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/RenameDoc.png", wxBITMAP_TYPE_PNG);
	bmpDeleteRemainingMoves.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/MinusRed.png", wxBITMAP_TYPE_PNG);
	bmpDeleteVariation.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Remove.png", wxBITMAP_TYPE_PNG);
	bmpPromoteVariation.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Promote.png", wxBITMAP_TYPE_PNG);


	info.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/info.png", wxBITMAP_TYPE_PNG);

	bmpFlipBoard.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/3/update.png", wxBITMAP_TYPE_PNG);


	bmpMediaFromDisk.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/pixophilia/2/burn.png", wxBITMAP_TYPE_PNG);
	movieFromWeb.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/pixophilia/2/movie.png", wxBITMAP_TYPE_PNG);

	bmpClock.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/clock.png",wxBITMAP_TYPE_PNG);
	//bmpSetupPosition.LoadFile("icons/shlyap/2/full_screen.png", wxBITMAP_TYPE_PNG);

	//LoadFile("images/PieceThemes/Internet/bp.png", wxBITMAP_TYPE_PNG);
	
	swapSides.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/2/theater.png", wxBITMAP_TYPE_PNG);


	playThroughGame.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/star.png", wxBITMAP_TYPE_PNG);

	//m_ribbon->SetFont(wxFont(6, wxMODERN, wxNORMAL,wxNORMAL, false/*,"Arial Baltic"*/));
    //--------------------------------------------------------------------------------------
	// Analysis Page
	//--------------------------------------------------------------------------------------
    wxRibbonPage* home = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Analysis"), ribbon_xpm);
    {
		 wxRibbonPanel *toolbar_panel = new wxRibbonPanel(home, wxID_ANY, wxT("Toolbar"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        wxRibbonToolBar *toolbar = new wxRibbonToolBar(toolbar_panel, ID_MAIN_TOOLBAR);
      //  toolbar->AddTool(wxID_ANY, align_left_xpm);

       // toolbar->AddTool(wxID_ANY, align_center_xpm);
      //  toolbar->AddTool(wxID_ANY, align_right_xpm);
      //  toolbar->AddSeparator();
		toolbar->AddHybridTool(ID_OPEN, bmpOpen, "Open");
		toolbar->AddHybridTool(ID_SAVE, bmpSave, "Save");
		toolbar->AddHybridTool(wxID_NEW, bmpClipBoard, "Clipboard");

       // toolbar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER, wxSize(16, 15)));
       // toolbar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_OTHER, wxSize(16, 15)));
       // toolbar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_OTHER, wxSize(16, 15)));
      //  toolbar->AddSeparator();
     /*   toolbar->AddDropdownTool(wxID_UNDO, wxArtProvider::GetBitmap(wxART_UNDO, wxART_OTHER, wxSize(16, 15)));
        toolbar->AddDropdownTool(wxID_REDO, wxArtProvider::GetBitmap(wxART_REDO, wxART_OTHER, wxSize(16, 15)));
        toolbar->AddSeparator();
        toolbar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_REPORT_VIEW, wxART_OTHER, wxSize(16, 15)));
        toolbar->AddTool(wxID_ANY, wxArtProvider::GetBitmap(wxART_LIST_VIEW, wxART_OTHER, wxSize(16, 15)));
        toolbar->AddSeparator();
        toolbar->AddHybridTool(ID_POSITION_LEFT, position_left_xpm);
        toolbar->AddHybridTool(ID_POSITION_TOP, position_top_xpm);
        toolbar->AddSeparator();
		*/
      //  toolbar->AddHybridTool(wxID_PRINT, wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, wxSize(16, 15)));
        toolbar->SetRows(1, 3);
		//,wxNullBitmap, "Add Annotation"
        wxRibbonPanel *selection_panel = new wxRibbonPanel(home, wxID_ANY, wxT("Game"), wxBitmap(selection_panel_xpm),wxDefaultPosition, wxDefaultSize,wxRIBBON_PANEL_EXT_BUTTON);
      //  wxRibbonButtonBar *selection = new wxRibbonButtonBar(selection_panel);
		wxRibbonToolBar *toolbar2 = new wxRibbonToolBar(selection_panel, ID_MAIN_TOOLBAR2); 
		toolbar2->AddHybridTool(ID_SQUARE,wxBitmap(chessboard_icon_xpm));
		toolbar2->AddHybridTool(ID_EDIT_PGN_HEADER,bmpEditPGNHeader);
		toolbar2->AddHybridTool(ID_FLIP_BOARD,bmpFlipBoard);
		toolbar2->SetRows(1, 3);


		//selection->AddHybridButton(ID_SQUARE, wxT("New"), wxBitmap(chessboard_icon_xpm), "New Game Menu");
        //selection->AddButton(ID_EDIT_PGN_HEADER, wxT("Edit Header"),bmpEditPGNHeader, "Edit PGN Header");
        //selection->AddButton(ID_SELECTION_CONTRACT, wxT("FullScreen"), bmpFullScreen, "Fullscreen");

        wxRibbonPanel *shapes_panel = new wxRibbonPanel(home, wxID_ANY, wxT("Functions"), wxBitmap(circle_small_xpm));
        wxRibbonButtonBar *shapes = new wxRibbonButtonBar(shapes_panel);
        shapes->AddButton(ID_CIRCLE, wxT("Engine"), wxBitmap(circle_xpm), wxBitmap(circle_small_xpm), wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Add Engine");
        shapes->AddButton(ID_CROSS, wxT("Options"), wxBitmap(cross_xpm), wxNullBitmap,wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Show Options Panel");
        shapes->AddHybridButton(ID_TRIANGLE, wxT("Book"), wxBitmap(triangle_xpm));
        shapes->AddDropdownButton(ID_POLYGON, wxT("Wizard"), wxBitmap(hexagon_xpm), wxEmptyString);

		/*
		ID_EDIT_PGN_HEADER,
		ID_EDIT_ANNOTATION,
		ID_DELETE_REMAINING_MOVES,
		ID_DELETE_VARIATION,
		ID_PROMOTE_VARIATION,
		*/
		//shapes->SetHe
        wxRibbonPanel *nrp = new wxRibbonPanel(home, wxID_ANY, wxT("Edit PGN"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);
		wxRibbonButtonBar *editables = new wxRibbonButtonBar(nrp);
	    
        editables->AddButton(ID_EDIT_ANNOTATION, wxT("Annotate"), bmpEditAnnotation, "Add Annotation");
        editables->AddButton(ID_DELETE_REMAINING_MOVES, wxT("Delete Moves"), bmpDeleteRemainingMoves, "Delete Remaining Moves");	
		editables->AddButton(ID_DELETE_VARIATION, wxT("Delete Variation"), bmpDeleteVariation, "Delete Variation");	
		editables->AddButton(ID_PROMOTE_VARIATION, wxT("Promote Variation"), bmpPromoteVariation, "Promote Variation");	
			
		//::wxRibbonB
		//mClock = new wxLCDWindow(nrp, wxDefaultPosition, wxSize(80, 48));
		//m_clockTimer = new ClockTimer(mClock);
		//nrp->Hide();
		//wxRibbonToolBar *toolbar2 = new wxRibbonToolBar(nrp, wxID_ANY);
		//wxCheckBox *checkUseAnnotaionWindow = new wxCheckBox(nrp, wxID_ANY, "Game Annotation", wxPoint(0,0));
		//wxCheckBox *checkUseAnnotaionWindow2 = new wxCheckBox(nrp, wxID_ANY, "Game Annotation2",wxPoint(0,20));
		
		//checkUseAnnotaionWindow->SetBackgroundColour(wxColor(0,0,0,128));
		//checkUseAnnotaionWindow2->SetBackgroundColour(wxColor(255,255,255,128));
		//checkUseAnnotaionWindow->
		//checkUseAnnotaionWindow2->UseBgCol();
		//checkUseAnnotaionWindow->SetBackgroundColour(nrp->GetArtProvider()->GetColour(0));
		//checkUseAnnotaionWindow2->SetBackgroundColour(nrp->GetArtProvider()->GetColour(0));
		//toolbar2->Add(checkUseAnnotaionWindow);
		//toolbar2->AddChild(checkUseAnnotaionWindow2);
		//wxSizer *ribbonSizer = new wxBoxSizer(wxVERTICAL);
		//ribbonSizer->Add(checkUseAnnotaionWindow,0,wxEXPAND,5);
		//ribbonSizer->Add(checkUseAnnotaionWindow2,0,wxEXPAND,5);
		//nrp->SetSizer(ribbonSizer);
		//ribbonSizer->Fit(nrp);
		//m_clockTimer->start();

        wxRibbonPanel *rp2 = new wxRibbonPanel(home, wxID_ANY, wxT("Empty todo"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);
		wxRibbonButtonBar *rp2bb = new wxRibbonButtonBar(rp2);
		rp2bb->AddButton(ID_PLAY_THROUGH_GAME, "Run Game",playThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
		//gcp = new GameClockPanel(rp2);
		//gcp->Start();

        wxRibbonPanel *rp3 = new wxRibbonPanel(home, wxID_ANY, wxT("Empty todo"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);
		wxRibbonButtonBar *edge = new wxRibbonButtonBar(rp3);
       edge->AddButton(ID_SELECTION_CONTRACT, wxT("Toggle FullScreen"), bmpFullScreen, "Fullscreen");
	   edge->AddButton(ID_INFO, wxT("Info"), info, "Info");

	   

	//	m_clockTimer2->start();

    }

	//--------------------------------------------------------------------------------------
	// Game Page
	//--------------------------------------------------------------------------------------
	wxRibbonPage* scheme = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Game"), eye_xpm);
    {
        wxFont label_font(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
        m_bitmap_creation_dc.SetFont(label_font);

        
        m_ribbon->GetArtProvider()->GetColourScheme(&m_default_primary,
            &m_default_secondary, &m_default_tertiary);
        wxRibbonPanel *provider_panel = new wxRibbonPanel(scheme, wxID_ANY,
            wxT("Art"), wxNullBitmap,wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        wxRibbonButtonBar *provider_bar = new wxRibbonButtonBar(provider_panel, wxID_ANY);
        provider_bar->AddDropdownButton(ID_SQUARE, wxT("New"), wxBitmap(chessboard_icon_xpm), "New Game Menu");
        provider_bar->AddButton(ID_CIRCLE, wxT("Engine"), wxBitmap(circle_xpm), wxBitmap(circle_small_xpm), wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Add Engine");
        provider_bar->AddButton(ID_CROSS, wxT("Options"), wxBitmap(cross_xpm), wxNullBitmap,wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Show Options Panel");
  
        wxRibbonPanel *gameFunc = new wxRibbonPanel(scheme, wxID_ANY, wxT("Game Functions"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);

		wxRibbonButtonBar *game_functions = new wxRibbonButtonBar(gameFunc);
		game_functions->AddButton(ID_SWAP_SIDES, wxT("Swap Sides"), swapSides, wxNullBitmap, wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Swap Sides");
		game_functions->AddDropdownButton(ID_CLOCK, wxT("Clock"), bmpClock, "Clock functions");
		
		

		//provider_bar->AddHybridButton(ID_TRIANGLE, wxT("Book"), wxBitmap(triangle_xpm));
     //   provider_bar->AddDropdownButton(ID_POLYGON, wxT("Wizard"), wxBitmap(hexagon_xpm), wxEmptyString);
  
      /*  provider_bar->AddButton(ID_DEFAULT_PROVIDER, wxT("Default Provider"),
            wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(32, 32)));
        provider_bar->AddButton(ID_AUI_PROVIDER, wxT("AUI Provider"), aui_style_xpm);
        provider_bar->AddButton(ID_MSW_PROVIDER, wxT("MSW Provider"), msw_style_xpm);
        wxRibbonPanel *primary_panel = new wxRibbonPanel(scheme, wxID_ANY,
            wxT("Primary Colour"), colours_xpm);
        m_primary_gallery = PopulateColoursPanel(primary_panel,
            m_default_primary, ID_PRIMARY_COLOUR);
        wxRibbonPanel *secondary_panel = new wxRibbonPanel(scheme, wxID_ANY,
            wxT("Secondary Colour"), colours_xpm);
        m_secondary_gallery = PopulateColoursPanel(secondary_panel,
            m_default_secondary, ID_SECONDARY_COLOUR);*/
    }

	//--------------------------------------------------------------------------------------
	// Training
	//--------------------------------------------------------------------------------------
    wxRibbonPage *BatchAnalysisPage =new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Batch Analysis"), empty_xpm);
    {
		wxRibbonPanel *toolbar_panel21 = new wxRibbonPanel(BatchAnalysisPage, wxID_ANY, wxT("Toolbar"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        wxRibbonToolBar *toolbar21 = new wxRibbonToolBar(toolbar_panel21, ID_MAIN_TOOLBAR);

        toolbar21->AddHybridTool(wxID_PRINT, wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, wxSize(16, 15)));

		toolbar21->SetRows(1, 3);  
	}

	//--------------------------------------------------------------------------------------
	// Training
	//--------------------------------------------------------------------------------------
    wxRibbonPage *TrainingPage =new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Training"), empty_xpm);
    {
		wxRibbonPanel *toolbar_panel2 = new wxRibbonPanel(TrainingPage, wxID_ANY, wxT("Toolbar"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        wxRibbonToolBar *toolbar2 = new wxRibbonToolBar(toolbar_panel2, ID_MAIN_TOOLBAR);

        toolbar2->AddHybridTool(wxID_PRINT, wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, wxSize(16, 15)));

		toolbar2->SetRows(1, 3);  
	}

	//--------------------------------------------------------------------------------------
	// Opening Page
	//--------------------------------------------------------------------------------------
	wxRibbonPage *OpeningPage = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Openings"), empty_xpm);
	{
	}

	//--------------------------------------------------------------------------------------
	// Engines Page
	//--------------------------------------------------------------------------------------
	new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Engines"), empty_xpm);
	
	//--------------------------------------------------------------------------------------
	// Media Page
	//--------------------------------------------------------------------------------------
	wxRibbonPage *mediaPage = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Media"), empty_xpm);
	{
		wxRibbonPanel *mediaPanel1 = new wxRibbonPanel(mediaPage, wxID_ANY, wxT("Video"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);
		wxRibbonButtonBar *mediaButtons = new wxRibbonButtonBar(mediaPanel1);
	    
        mediaButtons->AddButton(ID_LOAD_MEDIA_FROM_DISK, wxT("Disk"), bmpMediaFromDisk, "Load Media");
        mediaButtons->AddButton(ID_LOAD_MEDIA_FROM_URL, wxT("URL"), movieFromWeb, "Enter URL (youtube)");	
		

        wxRibbonPanel *selection_panel2 = new wxRibbonPanel(mediaPage, wxID_ANY, wxT("Game"), wxBitmap(selection_panel_xpm),wxDefaultPosition, wxDefaultSize,wxRIBBON_PANEL_EXT_BUTTON);
      //  wxRibbonButtonBar *selection = new wxRibbonButtonBar(selection_panel);
		wxRibbonToolBar *toolbar3 = new wxRibbonToolBar(selection_panel2, ID_MAIN_TOOLBAR2); 
		toolbar3->AddHybridTool(ID_SQUARE,wxBitmap(chessboard_icon_xpm));
		toolbar3->AddHybridTool(ID_EDIT_PGN_HEADER,bmpEditPGNHeader);
		toolbar3->AddHybridTool(ID_FLIP_BOARD,bmpFlipBoard);
		toolbar3->SetRows(1, 3);

		wxRibbonPanel *mediaPanel2 = new wxRibbonPanel(mediaPage, wxID_ANY, wxT("Edit PGN"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);

		wxRibbonButtonBar *editables2 = new wxRibbonButtonBar(mediaPanel2);
	    
        editables2->AddButton(ID_EDIT_ANNOTATION, wxT("Annotate"), bmpEditAnnotation, "Add Annotation");
        editables2->AddButton(ID_DELETE_REMAINING_MOVES, wxT("Delete Moves"), bmpDeleteRemainingMoves, "Delete Remaining Moves");	
		editables2->AddButton(ID_DELETE_VARIATION, wxT("Delete Variation"), bmpDeleteVariation, "Delete Variation");	
		editables2->AddButton(ID_PROMOTE_VARIATION, wxT("Promote Variation"), bmpPromoteVariation, "Promote Variation");	



	}
	//--------------------------------------------------------------------------------------
	// Scripts Page
	//--------------------------------------------------------------------------------------
	new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Scripts"), empty_xpm);
	{
	}

	//--------------------------------------------------------------------------------------
	// Database Page
	//--------------------------------------------------------------------------------------
    new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Database"), empty_xpm);
	{
	}
    m_ribbon->Realize();

    //m_textWindow = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
    //    wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY |
    //    wxTE_LEFT | wxTE_BESTWRAP | wxBORDER_NONE);

    wxSizer *s = new wxBoxSizer(wxVERTICAL);

    s->Add(m_ribbon, 0, wxEXPAND);
    //s->Add(m_logwindow, 1, wxEXPAND);

 


	//this->SetBackgroundColour(*wxRED);
    // Make a menubar
#if wxUSE_MENUS
    // Associate the menu bar with the frame
    //SetMenuBar(CreateMainMenubar());


    // This shows that the standard window menu may be customized:
  //  wxMenu * const windowMenu = GetWindowMenu();
  /*  if ( windowMenu )
    {
        // we can change the labels of standard items (which also means we can
        // set up accelerators for them as they're part of the label)
        windowMenu->SetLabel(wxID_MDI_WINDOW_TILE_HORZ,
                             "&Tile horizontally\tCtrl-Shift-H");
        windowMenu->SetLabel(wxID_MDI_WINDOW_TILE_VERT,
                             "&Tile vertically\tCtrl-Shift-V");

        // we can also change the help string
        windowMenu->SetHelpString(wxID_MDI_WINDOW_CASCADE,
                                  "Arrange windows in cascade");

        // we can remove some items
        windowMenu->Delete(wxID_MDI_WINDOW_ARRANGE_ICONS);

        // and we can add completely custom commands -- but then we must handle
        // them ourselves, see OnCloseAll()
        windowMenu->AppendSeparator();
        windowMenu->Append(wxID_CLOSE_ALL, "&Close all windows\tCtrl-Shift-C",
                           "Close all open windows");

        SetWindowMenu(windowMenu);
    }*/
#endif // wxUSE_MENUS



#if wxUSE_STATUSBAR
    CreateStatusBar();
#endif // wxUSE_STATUSBAR


 //   m_textWindow = new wxTextCtrl(this, wxID_ANY, "A help window",
  //                                wxDefaultPosition, wxDefaultSize,
  //                                wxTE_MULTILINE | wxSUNKEN_BORDER);

#if wxUSE_TOOLBAR
   // CreateToolBar(wxNO_BORDER | wxTB_FLAT | wxTB_HORIZONTAL);
  //  InitToolBar(GetToolBar());
#endif // wxUSE_TOOLBAR

#ifdef wxUSE_ACCEL
    // Accelerators
    wxAcceleratorEntry entries[2];
    entries[0].Set(wxACCEL_CTRL, (int) 'a', ID_TOGGLE_MAXIMIZE);
    entries[1].Set(wxACCEL_CTRL, (int) 'X', wxID_EXIT);
    //entries[2].Set(wxACCEL_CTRL, (int) 'A', wxID_ABOUT);
    wxAcceleratorTable accel(2, entries);
    SetAcceleratorTable(accel);
	
#endif // wxUSE_ACCEL



    // connect it only now, after creating m_textWindow
    Connect(wxEVT_SIZE, wxSizeEventHandler(MDIParentFrame::OnSize));

	subframe = new MyFrame(this);
	subframe->Maximize(true);
    subframe->Show(true);

	//s->Add(subframe);

   SetSizer(s);
    
	m_ribbon->Refresh(true);
	
	SetIcon(wxICON(sample));


}

void MDIParentFrame::SetBarStyle(long style)
{
    m_ribbon->Freeze();
    m_ribbon->SetWindowStyleFlag(style);
    wxBoxSizer *pTopSize = reinterpret_cast<wxBoxSizer*>(GetSizer());
    wxRibbonToolBar *pToolbar = wxDynamicCast(FindWindow(ID_MAIN_TOOLBAR), wxRibbonToolBar);
    if(style & wxRIBBON_BAR_FLOW_VERTICAL)
    {
        m_ribbon->SetTabCtrlMargins(10, 10);
        pTopSize->SetOrientation(wxHORIZONTAL);
        if(pToolbar)
            pToolbar->SetRows(3, 5);
    }
    else
    {
        m_ribbon->SetTabCtrlMargins(50, 20);
        pTopSize->SetOrientation(wxVERTICAL);
        if(pToolbar)
            pToolbar->SetRows(2, 3);
    }
    m_ribbon->Realise();
    Layout();
    m_ribbon->Thaw();
}

MDIParentFrame::~MDIParentFrame()
{
    // and disconnect it to prevent accessing already deleted m_textWindow in
    // the size event handler if it's called during destruction
//	if( this->gcp )
	//	delete this->gcp;
    Disconnect(wxEVT_SIZE, wxSizeEventHandler(MDIParentFrame::OnSize));
}

class ColourClientData : public wxClientData
{
public:
    ColourClientData(const wxString& name, const wxColour& colour)
        : m_name(name), m_colour(colour) {}

    const wxString& GetName()   const {return m_name;}
    const wxColour& GetColour() const {return m_colour;}

private:
    wxString m_name;
    wxColour m_colour;
};

wxRibbonGallery* MDIParentFrame::PopulateColoursPanel(wxWindow* panel,
                                               wxColour def, int gallery_id)
{
    wxRibbonGallery *gallery = wxDynamicCast(panel->FindWindow(gallery_id), wxRibbonGallery);
    if(gallery)
        gallery->Clear();
    else
        gallery = new wxRibbonGallery(panel, gallery_id);
    wxMemoryDC& dc = m_bitmap_creation_dc;
    wxRibbonGalleryItem *def_item = 
        AddColourToGallery(gallery, wxT("Default"), dc, &def);
    gallery->SetSelection(def_item);
    AddColourToGallery(gallery, wxT("BLUE"), dc);
    AddColourToGallery(gallery, wxT("BLUE VIOLET"), dc);
    AddColourToGallery(gallery, wxT("BROWN"), dc);
    AddColourToGallery(gallery, wxT("CADET BLUE"), dc);
    AddColourToGallery(gallery, wxT("CORAL"), dc);
    AddColourToGallery(gallery, wxT("CYAN"), dc);
    AddColourToGallery(gallery, wxT("DARK GREEN"), dc);
    AddColourToGallery(gallery, wxT("DARK ORCHID"), dc);
    AddColourToGallery(gallery, wxT("FIREBRICK"), dc);
    AddColourToGallery(gallery, wxT("GOLD"), dc);
    AddColourToGallery(gallery, wxT("GOLDENROD"), dc);
    AddColourToGallery(gallery, wxT("GREEN"), dc);
    AddColourToGallery(gallery, wxT("INDIAN RED"), dc);
    AddColourToGallery(gallery, wxT("KHAKI"), dc);
    AddColourToGallery(gallery, wxT("LIGHT BLUE"), dc);
    AddColourToGallery(gallery, wxT("LIME GREEN"), dc);
    AddColourToGallery(gallery, wxT("MAGENTA"), dc);
    AddColourToGallery(gallery, wxT("MAROON"), dc);
    AddColourToGallery(gallery, wxT("NAVY"), dc);
    AddColourToGallery(gallery, wxT("ORANGE"), dc);
    AddColourToGallery(gallery, wxT("ORCHID"), dc);
    AddColourToGallery(gallery, wxT("PINK"), dc);
    AddColourToGallery(gallery, wxT("PLUM"), dc);
    AddColourToGallery(gallery, wxT("PURPLE"), dc);
    AddColourToGallery(gallery, wxT("RED"), dc);
    AddColourToGallery(gallery, wxT("SALMON"), dc);
    AddColourToGallery(gallery, wxT("SEA GREEN"), dc);
    AddColourToGallery(gallery, wxT("SIENNA"), dc);
    AddColourToGallery(gallery, wxT("SKY BLUE"), dc);
    AddColourToGallery(gallery, wxT("TAN"), dc);
    AddColourToGallery(gallery, wxT("THISTLE"), dc);
    AddColourToGallery(gallery, wxT("TURQUOISE"), dc);
    AddColourToGallery(gallery, wxT("VIOLET"), dc);
    AddColourToGallery(gallery, wxT("VIOLET RED"), dc);
    AddColourToGallery(gallery, wxT("WHEAT"), dc);
    AddColourToGallery(gallery, wxT("WHITE"), dc);
    AddColourToGallery(gallery, wxT("YELLOW"), dc);

    return gallery;
}

void MDIParentFrame::RibbonTabDoubleClick(wxRibbonBarEvent& event)
{
	if( !m_ribbon->ArePanelsShown())
	{
		m_ribbon->ShowPanels(true);
		Refresh();
	}
}

wxColour MDIParentFrame::GetGalleryColour(wxRibbonGallery *gallery,
                                   wxRibbonGalleryItem* item, wxString* name)
{
    ColourClientData *data = (ColourClientData*)gallery->GetItemClientObject(item);
    if(name != NULL)
        *name = data->GetName();
    return data->GetColour();
}


void MDIParentFrame::OnIdle(wxIdleEvent &event)
{
	static int pg = 1;
	int id = m_ribbon->GetActivePage();
	if( id!= pg)
	{
//	m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=Client Size Reporter;state=6293500;dir=3;layer=2;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=852;floaty=512;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=2;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=398;floaty=412;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=No Close Button;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Client Size Reporter;state=6293500;dir=2;layer=1;row=0;pos=1;prop=62069;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=194;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=1;floaty=193;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(3,0,1)=109|dock_size(2,1,0)=248|dock_size(3,2,0)=169|dock_size(4,3,0)=516|", true);
//	m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=Client Size Reporter;state=6293500;dir=2;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=426;floaty=381;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=4;row=0;pos=0;prop=86926;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=414;floaty=373;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=No Close Button;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Client Size Reporter;state=6293500;dir=3;layer=4;row=0;pos=1;prop=75143;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=530;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-5;floaty=143;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=3;layer=10;row=0;pos=270;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=260;floaty=201;floatw=256;floath=76|name=tb2;caption=Toolbar 2;state=2106096;dir=3;layer=10;row=0;pos=6;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-5;floaty=669;floatw=278;floath=62|dock_size(5,0,0)=34|dock_size(3,0,1)=68|dock_size(2,1,0)=248|dock_size(3,10,0)=44|dock_size(3,4,0)=169|dock_size(4,5,0)=501|",true);
		pg = id;
		switch( id )
		{
		case 0:
			{
				//subframe->m_mgr.LoadPerspective(subframe->perspective_all);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=419;floaty=371;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=3;layer=3;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=703;floaty=150;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,0,0)=338|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(2,2,0)=153|dock_size(3,3,0)=173|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=3;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=200;minh=200;maxw=-1;maxh=-1;floatx=948;floaty=509;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=2;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=398;floaty=412;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Game Clock;state=6293500;dir=2;layer=1;row=0;pos=1;prop=62069;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=194;floatw=96;floath=114|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=1;floaty=193;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(3,0,1)=109|dock_size(2,1,0)=248|dock_size(3,2,0)=169|dock_size(4,3,0)=516|dock_size(2,3,0)=268|", true);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=831;floaty=537;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1157;floaty=143;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=581;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=530|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,1,0)=134|dock_size(4,2,0)=302|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=714;floaty=534;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=83363;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1210;floaty=143;floatw=37;floath=54|name=moveData;caption=;state=2044;dir=2;layer=2;row=0;pos=1;prop=116637;bestw=3;besth=3;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1071;floaty=143;floatw=37;floath=38|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=5;floaty=580;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,2,0)=176|");
				subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=96958;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=959;floaty=146;floatw=37;floath=54|name=moveData;caption=Move Data;state=2044;dir=2;layer=1;row=0;pos=1;prop=103042;bestw=3;besth=3;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1148;floaty=370;floatw=37;floath=38|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=579;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,1,0)=611|dock_size(5,0,0)=22|dock_size(3,0,1)=147|dock_size(2,1,0)=186|");
				subframe->m_mgr.GetPane("test2").Hide();
				//
				subframe->gameMode = 0;
				subframe->board_canvas->SetGameMode(0);

				subframe->m_pipePanel[0]->SetAnalyzeMode();
				if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
					subframe->m_mgr.GetPane("Engine_Panel").Hide();
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
					subframe->m_mgr.GetPane("Engine_Panel_2").Hide();

			/*	subframe->m_mgr.GetPane(wxT("tb1")).Hide();
				subframe->m_mgr.GetPane(wxT("tb2")).Hide();
				subframe->m_mgr.GetPane(wxT("clock_pane")).Hide();*/
				subframe->m_mgr.Update();
			}
			break;
		case 1:
			{
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=426;floaty=381;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=4;row=0;pos=0;prop=86926;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=414;floaty=373;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Game Clock;state=6293500;dir=3;layer=4;row=0;pos=1;prop=75143;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=530;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-5;floaty=143;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=3;layer=10;row=0;pos=270;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=260;floaty=201;floatw=256;floath=76|name=tb2;caption=Toolbar 2;state=2106096;dir=3;layer=10;row=0;pos=6;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-5;floaty=669;floatw=278;floath=62|dock_size(5,0,0)=34|dock_size(3,0,1)=68|dock_size(2,1,0)=248|dock_size(3,10,0)=44|dock_size(3,4,0)=169|dock_size(4,5,0)=501|",true);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=644;floaty=186;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=430;floaty=298;floatw=116;floath=114|name=test5;caption=Tree Browser;state=2044;dir=2;layer=5;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=815;floaty=189;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293500;dir=1;layer=3;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=757;floaty=167;floatw=37;floath=54|name=test7;caption=Dummy1;state=6293502;dir=4;layer=4;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-4;floaty=185;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=2;layer=4;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=235;floaty=187;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(5,0,0)=22|dock_size(1,3,0)=39|dock_size(2,4,0)=599|dock_size(4,5,0)=302|dock_size(2,5,0)=67|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=881;floaty=407;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=2;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1338;floaty=214;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293500;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1224;floaty=143;floatw=37;floath=54|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=312;floaty=580;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=402|dock_size(5,0,0)=22|dock_size(4,2,0)=302|dock_size(1,0,1)=67|dock_size(3,0,1)=109|dock_size(2,1,0)=63|");
				subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=4;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=917;floaty=143;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=2;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=3;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=900;floaty=185;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293500;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1158;floaty=141;floatw=37;floath=54|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=578;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=660|dock_size(5,0,0)=22|dock_size(1,0,1)=39|dock_size(3,0,1)=55|dock_size(2,3,0)=84|dock_size(2,4,0)=302|");
				subframe->gameMode = 1;
				subframe->board_canvas->SetGameMode(1);
				subframe->m_mgr.GetPane("test2").Hide();
				subframe->m_mgr.GetPane("test5").Hide();
				//if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
				//	subframe->m_mgr.GetPane("Engine_Panel").Hide();
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
					subframe->m_mgr.GetPane("Engine_Panel_2").Hide();

				subframe->gameClock->PauseGame();
				subframe->m_mgr.Update();
				subframe->m_pipePanel[0]->SetGameMode();
				subframe->board_canvas->whoseMove == BoardCanvas::YOUR_MOVE;
				if( subframe->board_canvas->GetBoard()->mov == 0 || subframe->board_canvas->GetBoard()->mov == 2 )
				{
					if( !subframe->board_canvas->IsBoardFlipped() )
					{
						subframe->board_canvas->Flip();
						subframe->board_canvas->Refresh(false);
					}
					subframe->m_pipePanel[0]->SetEngineColor(WHITE);
				}
				else
				{
					if( subframe->board_canvas->IsBoardFlipped() )
					{
						subframe->board_canvas->Flip();
						subframe->board_canvas->Refresh(false);
					}
					subframe->m_pipePanel[0]->SetEngineColor(BLACK);
				}

			}
			break;
		case 2:
			{
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=14;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=296;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=12;row=0;pos=0;prop=112763;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=735;floaty=517;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=12;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=824;floaty=350;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=74477;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=8;row=0;pos=1;prop=93134;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1281;floaty=143;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=13;row=0;pos=0;prop=125523;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=302;floaty=146;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099196;dir=4;layer=8;row=0;pos=0;prop=94103;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=614;floaty=347;floatw=37;floath=54|name=test12;caption=Graphical Pane;state=23070716;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=785;floaty=143;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(4,8,0)=206|dock_size(2,2,0)=402|dock_size(3,12,0)=169|dock_size(4,13,0)=302|dock_size(4,14,0)=302|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=4;layer=14;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=296;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=12;row=0;pos=0;prop=112763;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=735;floaty=517;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=12;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=824;floaty=350;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=74477;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=14;row=0;pos=1;prop=93134;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=302;floaty=339;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=15;row=0;pos=0;prop=125523;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=157;floaty=143;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099196;dir=4;layer=14;row=0;pos=0;prop=94103;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=459;floaty=145;floatw=37;floath=54|name=test12;caption=Graphical Pane;state=23070716;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=785;floaty=143;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(2,2,0)=402|dock_size(3,12,0)=188|dock_size(4,14,0)=226|dock_size(4,15,0)=424|");
				subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=4;layer=14;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=296;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=4;layer=14;row=0;pos=2;prop=52906;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=9;floaty=534;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=12;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=824;floaty=350;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=74477;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=14;row=0;pos=1;prop=93134;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=302;floaty=339;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=19;row=0;pos=0;prop=125523;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-6;floaty=143;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099196;dir=4;layer=14;row=0;pos=0;prop=94103;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=459;floaty=145;floatw=37;floath=54|name=test12;caption=Graphical Pane;state=23070716;dir=1;layer=0;row=1;pos=0;prop=159857;bestw=600;besth=600;minw=300;minh=300;maxw=-1;maxh=-1;floatx=960;floaty=143;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(4,14,0)=330|dock_size(4,19,0)=553|dock_size(1,0,1)=419|");
				//subframe->m_mgr.LoadPerspective(subframe->perspective_all);
				subframe->m_mgr.GetPane(wxT("tb1")).Hide();
				subframe->m_mgr.GetPane(wxT("tb2")).Hide();
				subframe->m_mgr.GetPane("test2").Hide();
				//
				subframe->gameMode = 0;
				subframe->board_canvas->SetGameMode(0);

				subframe->m_pipePanel[0]->SetAnalyzeMode();
				if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
					subframe->m_mgr.GetPane("Engine_Panel").Hide();
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
					subframe->m_mgr.GetPane("Engine_Panel_2").Hide();
				subframe->m_mgr.Update();
			}
			break;
		case 3:
			{
				subframe->m_mgr.LoadPerspective(subframe->perspective_all);
				subframe->m_mgr.GetPane(wxT("tb1")).Hide();
				subframe->m_mgr.GetPane(wxT("tb2")).Hide();
				subframe->m_mgr.Update();
			}
			break;
		case 5:
			{
				subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=420;floaty=391;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=454;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2046;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2098940;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070718;dir=4;layer=1;row=0;pos=1;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=;state=1022;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,0,0)=182|dock_size(4,1,0)=408|dock_size(3,1,0)=119|dock_size(5,0,0)=82|dock_size(1,10,1)=44|dock_size(2,2,0)=202|");
				subframe->m_mgr.Update();
			}
			break;
		case 6:
			{
				subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=2;prop=149016;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293500;dir=2;layer=2;row=0;pos=1;prop=60108;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=419;floaty=533;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=124669;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=0;row=2;pos=0;prop=26315;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=153;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=2;layer=2;row=0;pos=0;prop=139892;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=722;floaty=528;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=4;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=419;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(2,2,0)=387|dock_size(4,0,2)=95|dock_size(3,0,4)=109|");
				
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
					subframe->m_mgr.GetPane(subframe->m_pipePanel[1]->GetPanelName()).Hide();

				subframe->m_mgr.Update();
			}
			break;
		case 8:
			{
				subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099196;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=0;floaty=359;floatw=96;floath=114|name=test5;caption=No Close Button;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1323;floaty=368;floatw=96;floath=114|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070718;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=25;floaty=25;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(1,0,0)=209|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(4,2,0)=339|dock_size(2,2,0)=82|",true);
			}
			break;
		}
	}
}

void MDIParentFrame::OnHoveredColourChange(wxRibbonGalleryEvent& evt)
{
    // Set the background of the gallery to the hovered colour, or back to the
    // default if there is no longer a hovered item.

    wxRibbonGallery *gallery = evt.GetGallery();
    wxRibbonArtProvider *provider = gallery->GetArtProvider();

    if(evt.GetGalleryItem() != NULL)
    {
        if(provider == m_ribbon->GetArtProvider())
        {
            provider = provider->Clone();
            gallery->SetArtProvider(provider);
        }
        provider->SetColour(wxRIBBON_ART_GALLERY_HOVER_BACKGROUND_COLOUR,
            GetGalleryColour(evt.GetGallery(), evt.GetGalleryItem(), NULL));
    }
    else
    {
        if(provider != m_ribbon->GetArtProvider())
        {
            gallery->SetArtProvider(m_ribbon->GetArtProvider());
            delete provider;
        }
    }
}

void MDIParentFrame::OnPrimaryColourSelect(wxRibbonGalleryEvent& evt)
{
    wxString name;
    wxColour colour = GetGalleryColour(evt.GetGallery(), evt.GetGalleryItem(), &name);
    AddText(wxT("Colour \"") + name + wxT("\" selected as primary."));
    wxColour secondary, tertiary;
    m_ribbon->GetArtProvider()->GetColourScheme(NULL, &secondary, &tertiary);
    m_ribbon->GetArtProvider()->SetColourScheme(colour, secondary, tertiary);
	
    ResetGalleryArtProviders();
    m_ribbon->Refresh();
}

void MDIParentFrame::OnSecondaryColourSelect(wxRibbonGalleryEvent& evt)
{
    wxString name;
    wxColour colour = GetGalleryColour(evt.GetGallery(), evt.GetGalleryItem(), &name);
    AddText(wxT("Colour \"") + name + wxT("\" selected as secondary."));
    wxColour primary, tertiary;
    m_ribbon->GetArtProvider()->GetColourScheme(&primary, NULL, &tertiary);
    m_ribbon->GetArtProvider()->SetColourScheme(primary, colour, tertiary);
    ResetGalleryArtProviders();
	SetBackgroundColour(colour);
	Refresh(true);
	     /*   wxColourDialog dlg(m_frame);
        dlg.SetTitle(_("Color Picker"));
        if (dlg.ShowModal() != wxID_OK)
            return;

        int var = 0;
        switch (event.GetId())
        {
            case ID_BackgroundColor:              var = wxAUI_DOCKART_BACKGROUND_COLOUR; break;
            case ID_SashColor:                    var = wxAUI_DOCKART_SASH_COLOUR; break;
            case ID_InactiveCaptionColor:         var = wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR; break;
            case ID_InactiveCaptionGradientColor: var = wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR; break;
            case ID_InactiveCaptionTextColor:     var = wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR; break;
            case ID_ActiveCaptionColor:           var = wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR; break;
            case ID_ActiveCaptionGradientColor:   var = wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR; break;
            case ID_ActiveCaptionTextColor:       var = wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR; break;
            case ID_BorderColor:                  var = wxAUI_DOCKART_BORDER_COLOUR; break;
            case ID_GripperColor:                 var = wxAUI_DOCKART_GRIPPER_COLOUR; break;
            default: return;
        }

        m_frame->GetDockArt()->SetColor(var, dlg.GetColourData().GetColour());
        m_frame->DoUpdate();
        UpdateColors();*/
	subframe->GetDockArt()->SetColor(wxAUI_DOCKART_BACKGROUND_COLOUR,colour);
	subframe->GetDockArt()->SetColor(wxAUI_DOCKART_SASH_COLOUR,primary);
	subframe->GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR,primary);
	subframe->GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR,primary);
	subframe->GetDockArt()->SetColor(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR,colour);
	subframe->GetDockArt()->SetColor(wxAUI_DOCKART_BORDER_COLOUR,colour);
	//subframe->GetToolBar()->SetBackgroundColour(colour);
    m_ribbon->Refresh();
}

void MDIParentFrame::ResetGalleryArtProviders()
{
    if(m_primary_gallery->GetArtProvider() != m_ribbon->GetArtProvider())
    {
        delete m_primary_gallery->GetArtProvider();
        m_primary_gallery->SetArtProvider(m_ribbon->GetArtProvider());
    }
    if(m_secondary_gallery->GetArtProvider() != m_ribbon->GetArtProvider())
    {
        delete m_secondary_gallery->GetArtProvider();
        m_secondary_gallery->SetArtProvider(m_ribbon->GetArtProvider());
    }
}

void MDIParentFrame::OnSelectionExpandHButton(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    //AddText(wxT("Expand selection horizontally button clicked."));

}

void MDIParentFrame::OnSelectionExpandVButton(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    AddText(wxT("Expand selection vertically button clicked."));
	wxCommandEvent evmt;
	subframe->OnInsertAnnotation(evmt);


}

void MDIParentFrame::OnSelectionContractButton(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
	if( bFullScreen )
	{
		this->ShowFullScreen(false);
		bFullScreen = false;
	}
	else
	{
		this->ShowFullScreen(true);
		bFullScreen = true;
	}
    AddText(wxT("Contract selection button clicked."));
}

void MDIParentFrame::OnCircleButton(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    AddText(wxT("Circle button clicked."));
	wxString cmd;
	wxString name;
	EngineListModalDialog dialog(this, subframe->m_pipePanel[0], wxID_ANY, "Engine List", &cmd);
	dialog.ShowModal();

    if ( cmd.empty() )
    {
		//Close(true);
		//	subframe->m_mgr.GetPane("Engine_Panel").Show(true);
		//	subframe->m_mgr.GetPane("Engine_Panel_2").Show(true);
		 return;
	}

    wxProcess *process = wxProcess::Open(cmd);
	 //long pid = wxExecute(cmd, 1, process);
	 //process->Redirect();
	//MyPipedProcess2 *process = new MyPipedProcess2(this, cmd, input);
    if ( !process )
    {
        wxLogError(wxT("Failed to launch the command."));
        return;
    }

    wxLogVerbose(wxT("PID of the new process: %ld"), process->GetPid());

    wxOutputStream *out = process->GetOutputStream();
    if ( !out )
    {
        wxLogError(wxT("Failed to connect to child stdin"));
        return;
    }
	//out->

    wxInputStream *in = process->GetInputStream();
    if ( !in )
    {
        wxLogError(wxT("Failed to connect to child stdout"));
        return;
    }

	//in->
	if( subframe->m_pipePanel[0]->EngineIsLoaded() )
	{
		if( subframe->m_pipePanel[1]->EngineIsLoaded () )
		{
			bool panelFound = false;
			for( int i = 0; i < MAX_NUM_PIPE_PANELS; i++ )
			{
				if( subframe->m_pipePanel[i]!= NULL )
				{
					if( !subframe->m_pipePanel[i]->EngineIsLoaded() )
					{
						// non null pipe panel without engine.
						subframe->m_mgr.GetPane(subframe->m_pipePanel[i]->GetPanelName()).Show(true);
						subframe->m_pipePanel[i]->LoadEngine(cmd,process);

						subframe->board_canvas->AddActiveEngine(subframe->m_pipePanel[i]);
						subframe->my_canvas->AddActiveEngine(subframe->m_pipePanel[i]);					
						// for pipe_panel 0, the string doesn't exist
						panelFound = true;
						break;
					}
				}
			}

			if( !panelFound )
			{
				wxCommandEvent cmdEvt;
				subframe->OnCreatePipePanel(cmdEvt);
				subframe->m_pipePanel[subframe->GetNumActivePipePanels()-1]->LoadEngine(cmd,process);
			
				subframe->board_canvas->AddActiveEngine(subframe->m_pipePanel[subframe->GetNumActivePipePanels()-1]);
				subframe->my_canvas->AddActiveEngine(subframe->m_pipePanel[subframe->GetNumActivePipePanels()-1]);	
			}
		}
		else
		{
			subframe->m_mgr.GetPane("Engine_Panel_2").Show(true);
			subframe->m_pipePanel[1]->LoadEngine(cmd,process);


			subframe->board_canvas->AddActiveEngine(subframe->m_pipePanel[1]);
			subframe->my_canvas->AddActiveEngine(subframe->m_pipePanel[1]);
		}
#ifdef USE_CANVAS_SET_FOCUS
		subframe->my_canvas->SetFocus();
#endif
	}
	else 
	{
		subframe->m_mgr.GetPane("Engine_Panel").Show(true);
		subframe->m_pipePanel[0]->LoadEngine(cmd,process);


		subframe->board_canvas->AddActiveEngine(subframe->m_pipePanel[0]);
		subframe->my_canvas->AddActiveEngine(subframe->m_pipePanel[0]);

#ifdef USE_CANVAS_SET_FOCUS
		subframe->my_canvas->SetFocus();
#endif
	}
	//subframe->m_mgr.Update();
}

void MDIParentFrame::OnCrossButton(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    AddText(wxT("Cross button clicked."));
	subframe->m_mgr.GetPane("test2").Show();//(true);
	subframe->m_mgr.Update();
}

void MDIParentFrame::OnTriangleButton(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    AddText(wxT("Triangle button clicked."));
}

void MDIParentFrame::OnTriangleDropdown(wxRibbonButtonBarEvent& evt)
{
    wxMenu menu;
    menu.Append(wxID_ANY, wxT("Equilateral"));
    menu.Append(wxID_ANY, wxT("Isosceles"));
    menu.Append(wxID_ANY, wxT("Scalene"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnClockDropdown(wxRibbonButtonBarEvent& evt)
{
	wxMenu menu;
    menu.Append(ID_CLOCK_SET_TIME_CONTROLS, wxT("Set Time Controls"));
    menu.Append(ID_CLOCK_RESET, wxT("Reset Clock"));
    menu.Append(ID_CLOCK_SET_TIME, wxT("Set Time"));

    evt.PopupMenu(&menu);
}


void MDIParentFrame::OnClockSetTimeControls(wxCommandEvent& WXUNUSED(evt))
{
	wxMessageBox("OnClockSetTimeControls");
}

void MDIParentFrame::OnClockReset(wxCommandEvent& WXUNUSED(evt))
{
	this->subframe->gameClock->ResetTime();
	
}

void MDIParentFrame::OnClockSetTime(wxCommandEvent& WXUNUSED(evt))
{
}

void MDIParentFrame::OnNewGame(wxCommandEvent& evt)
{
	wxCommandEvent cmd_evt;
	subframe->OnNewGame(cmd_evt);
	
}

void MDIParentFrame::OnSetupPosition(wxCommandEvent& evt)
{
	PositionSetupFrame *positionSetupFrame = new PositionSetupFrame(this->subframe);
	positionSetupFrame->Show(true);
}

void MDIParentFrame::OnPlayThroughGame(wxRibbonButtonBarEvent& evt)
{
	subframe->PlayThroughGame();
}


void MDIParentFrame::OnGameDropdown(wxRibbonToolBarEvent& evt)
{
	wxMenu menu;
    menu.Append(ID_NEW_GAME, wxT("New Game"));
    menu.Append(ID_SETUP_POSITION, wxT("Setup Position"));

    evt.PopupMenu(&menu);
}



void MDIParentFrame::OnNewGameButton(wxRibbonButtonBarEvent& evt)
{
		wxCommandEvent cmd_evt;
	subframe->OnNewGame(cmd_evt);

}

void MDIParentFrame::OnNewGameDropDown(wxRibbonButtonBarEvent& evt)
{
	wxMenu menu;
    menu.Append(ID_NEW_GAME, wxT("New Game"));
    menu.Append(ID_SETUP_POSITION, wxT("Setup Position"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnSwapSides(wxRibbonButtonBarEvent& evt)
{
	if( subframe->board_canvas->whoseMove == BoardCanvas::YOUR_MOVE)
	{
		subframe->board_canvas->whoseMove = BoardCanvas::WAIT_COMPUTER;
		if( subframe->m_pipePanel[0]->GetEngineColor() == WHITE )
			subframe->m_pipePanel[0]->SetEngineColor(BLACK);
		if( subframe->m_pipePanel[0]->GetEngineColor() == BLACK )
			subframe->m_pipePanel[0]->SetEngineColor(WHITE);
		if( subframe->board_canvas->activeEngines[0]->IsActive() )
			subframe->board_canvas->activeEngines[0]->PlayerMoves();

		if( subframe->gameClock->IsPaused() )
		{
			subframe->gameClock->UnPause();
		}

		subframe->board_canvas->Flip();
		subframe->board_canvas->Refresh(false);
	}
	else if( subframe->board_canvas->whoseMove == BoardCanvas::WAIT_COMPUTER)
	{
		subframe->board_canvas->whoseMove = BoardCanvas::YOUR_MOVE;
		if( subframe->m_pipePanel[0]->GetEngineColor() == WHITE )
			subframe->m_pipePanel[0]->SetEngineColor(BLACK);
		if( subframe->m_pipePanel[0]->GetEngineColor() == BLACK )
			subframe->m_pipePanel[0]->SetEngineColor(WHITE);

		if( subframe->gameClock->IsPaused() )
		{
			subframe->gameClock->UnPause();
		}

		subframe->board_canvas->Flip();
		subframe->board_canvas->Refresh(false);
	}

}

void MDIParentFrame::OnFlipBoard(wxRibbonToolBarEvent& evt)
{
	subframe->board_canvas->Flip();
	//m_ribbon->ShowPanels(false);
	//wxSizeEvent sze;
	//OnSize(sze);
}

void MDIParentFrame::ToggleMaximize(wxCommandEvent& evt)
{
//	subframe->board_canvas->Flip();
	if( m_ribbon->ArePanelsShown() == false )
	{
		m_ribbon->ShowPanels(true);
	}
	else
	{
		m_ribbon->ShowPanels(false);

	}

		wxSizeEvent sze;
		OnSize(sze);
}

void MDIParentFrame::OnSquareButton(wxRibbonToolBarEvent& WXUNUSED(evt))
{
    AddText(wxT("Square button clicked."));
	wxCommandEvent cmd_evt;
	subframe->OnNewGame(cmd_evt);
	
}

void MDIParentFrame::OnPolygonDropdown(wxRibbonButtonBarEvent& evt)
{
    wxMenu menu;
    menu.Append(wxID_ANY, wxT("Pentagon (5 sided)"));
    menu.Append(wxID_ANY, wxT("Hexagon (6 sided)"));
    menu.Append(wxID_ANY, wxT("Heptagon (7 sided)"));
    menu.Append(wxID_ANY, wxT("Octogon (8 sided)"));
    menu.Append(wxID_ANY, wxT("Nonagon (9 sided)"));
    menu.Append(wxID_ANY, wxT("Decagon (10 sided)"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnNew(wxRibbonToolBarEvent& WXUNUSED(evt))
{
    AddText(wxT("New button clicked."));
}

void MDIParentFrame::OnNewDropdown(wxRibbonToolBarEvent& evt)
{
    
	wxMenu menu;

    menu.Append(ID_PASTE_PGN, wxT("Paste PGN"));
    menu.Append(ID_MERGE_PGN_FROM_CLIPBOARD, wxT("Merge PGN from Clipboard"));
	
	menu.Append(ID_COPY_PGN_TO_CLIPBOARD, wxT("Copy PGN to Clipboard"));
	menu.Append(ID_COPY_FEN_TO_CLIPBOARD, wxT("Copy FEN to Clipboard"));
	menu.Append(ID_COPY_VARIATION_TO_CLIPBOARD, wxT("Copy Variation to Clipboard"));
	menu.Append(ID_PASTE_FEN, wxT("Paste FEN"));

	

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnOpen(wxRibbonToolBarEvent& evt)
{
}
void MDIParentFrame::OnOpenDropdown(wxRibbonToolBarEvent& evt)
{
	wxMenu menu;
	menu.Append(ID_LOAD_PGN, wxT("Load New PGN"));
	menu.Append(ID_ADD_PGN, wxT("Add PGN"));
	menu.Append(ID_MERGE_PGN_FROM_DISK, wxT("Merge PGN from Disk"));
	evt.PopupMenu(&menu);
}
void MDIParentFrame::OnSave(wxRibbonToolBarEvent& evt)
{
}
void MDIParentFrame::OnSaveDropdown(wxRibbonToolBarEvent& evt)
{
	wxMenu menu;
	menu.Append(ID_SAVE_PGN, wxT("Save PGN"));
	menu.Append(ID_SAVE_GAME_LIST_AS_DATABASE, wxT("Save Game List as PGN Database"));
	evt.PopupMenu(&menu);
}

void MDIParentFrame::OnPrint(wxRibbonToolBarEvent& WXUNUSED(evt))
{
    AddText(wxT("Print button clicked."));
	wxCommandEvent cmdEvt;
	subframe->OnCopyPerspectiveCode(cmdEvt);
}

void MDIParentFrame::OnPrintDropdown(wxRibbonToolBarEvent& evt)
{
    wxMenu menu;
    menu.Append(wxID_ANY, wxT("Print"));
    menu.Append(wxID_ANY, wxT("Preview"));
    menu.Append(wxID_ANY, wxT("Options"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnRedoDropdown(wxRibbonToolBarEvent& evt)
{
    wxMenu menu;
    menu.Append(wxID_ANY, wxT("Redo E"));
    menu.Append(wxID_ANY, wxT("Redo F"));
    menu.Append(wxID_ANY, wxT("Redo G"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnUndoDropdown(wxRibbonToolBarEvent& evt)
{
    wxMenu menu;
    menu.Append(wxID_ANY, wxT("Undo C"));
    menu.Append(wxID_ANY, wxT("Undo B"));
    menu.Append(wxID_ANY, wxT("Undo A"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnPositionTopLabels(wxCommandEvent& WXUNUSED(evt))
{
 //   SetBarStyle(wxRIBBON_BAR_DEFAULT_STYLE);
}

void MDIParentFrame::OnPositionTopIcons(wxCommandEvent& WXUNUSED(evt))
{
    //SetBarStyle((wxRIBBON_BAR_DEFAULT_STYLE &~wxRIBBON_BAR_SHOW_PAGE_LABELS)
      //  | wxRIBBON_BAR_SHOW_PAGE_ICONS);
}

void MDIParentFrame::OnLoadPGN(wxCommandEvent& evt)
{
	subframe->OnLoadPGN(evt);
}

void MDIParentFrame::OnAddPGN(wxCommandEvent& evt)
{
	subframe->OnAddPGN(evt);
}

void MDIParentFrame::OnPasteFEN(wxCommandEvent& evt)
{
	this->subframe->OnPasteFEN(evt);
}

void MDIParentFrame::OnPastePGN(wxCommandEvent& evt)
{
	this->subframe->OnLoadPgnFromClipBoard(evt);
}

void MDIParentFrame::OnMergePGNFromClipboard(wxCommandEvent& evt)
{
	this->subframe->OnMergePGNFromClipBoard(evt);
}

void MDIParentFrame::OnMergePGNFromDisk(wxCommandEvent& evt)
{
	this->subframe->OnMergePGNFromDisk(evt);
}

void MDIParentFrame::OnCopyPGNToClipBoard(wxCommandEvent& evt)
{
	this->subframe->OnCopyPGNToClipBoard(evt);
}

void MDIParentFrame::OnCopyFENToClipBoard(wxCommandEvent& evt)
{
	this->subframe->CopyFenToClipBoard(evt);
}

void MDIParentFrame::OnCopyVariationToClipBoard(wxCommandEvent& evt)
{
	this->subframe->OnCopyVariationToClipBoard(evt);
}

void MDIParentFrame::OnSavePGN(wxCommandEvent& evt)
{
	this->subframe->OnSavePGN(evt);
}

void MDIParentFrame::OnSaveGameListAsDatabase(wxCommandEvent& evt)
{
	this->subframe->OnSaveGameListAsDatabase(evt);
}


void MDIParentFrame::OnPositionTopBoth(wxCommandEvent& WXUNUSED(evt))
{
    //SetBarStyle(wxRIBBON_BAR_DEFAULT_STYLE | wxRIBBON_BAR_SHOW_PAGE_ICONS);
}

void MDIParentFrame::OnPositionLeftLabels(wxCommandEvent& WXUNUSED(evt))
{
    //SetBarStyle(wxRIBBON_BAR_DEFAULT_STYLE | wxRIBBON_BAR_FLOW_VERTICAL);
}

void MDIParentFrame::OnPositionLeftIcons(wxCommandEvent& WXUNUSED(evt))
{
    //SetBarStyle((wxRIBBON_BAR_DEFAULT_STYLE &~wxRIBBON_BAR_SHOW_PAGE_LABELS) |
    //    wxRIBBON_BAR_SHOW_PAGE_ICONS | wxRIBBON_BAR_FLOW_VERTICAL);
}

void MDIParentFrame::OnPositionLeftBoth(wxCommandEvent& WXUNUSED(evt))
{
   // SetBarStyle(wxRIBBON_BAR_DEFAULT_STYLE | wxRIBBON_BAR_SHOW_PAGE_ICONS |
    //    wxRIBBON_BAR_FLOW_VERTICAL);
}

void MDIParentFrame::OnPositionTop(wxRibbonToolBarEvent& evt)
{
    OnPositionTopLabels(evt);
}

void MDIParentFrame::OnPositionTopDropdown(wxRibbonToolBarEvent& evt)
{
    wxMenu menu;
    menu.Append(ID_POSITION_TOP, wxT("Top with Labels"));
    menu.Append(ID_POSITION_TOP_ICONS, wxT("Top with Icons"));
    menu.Append(ID_POSITION_TOP_BOTH, wxT("Top with Both"));
    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnPositionLeft(wxRibbonToolBarEvent& evt)
{
    OnPositionLeftIcons(evt);
}

void MDIParentFrame::OnPositionLeftDropdown(wxRibbonToolBarEvent& evt)
{
    wxMenu menu;
    menu.Append(ID_POSITION_LEFT, wxT("Left with Icons"));
    menu.Append(ID_POSITION_LEFT_LABELS, wxT("Left with Labels"));
    menu.Append(ID_POSITION_LEFT_BOTH, wxT("Left with Both"));
    evt.PopupMenu(&menu);
}

void MDIParentFrame::AddText(wxString msg)
{
    //m_logwindow->AppendText(msg);
    //m_logwindow->AppendText(wxT("\n"));
    m_ribbon->DismissExpandedPanel();
}



void MDIParentFrame::OnEditPGNHeader(wxRibbonToolBarEvent& evt)
{
	subframe->EditPGNHeader();
}
void MDIParentFrame::OnEditAnnotation(wxRibbonButtonBarEvent& evt)
{
	subframe->EditAnnotation();
}
void MDIParentFrame::OnDeleteRemainingMoves(wxRibbonButtonBarEvent& evt)
{
	subframe->DeleteRemainingMoves();
}
void MDIParentFrame::OnDeleteVariation(wxRibbonButtonBarEvent& evt)
{
	subframe->DeleteVariation();
}
void MDIParentFrame::OnPromoteVariation(wxRibbonButtonBarEvent& evt)
{
	subframe->PromoteVariation();
}

void MDIParentFrame::OnInfo(wxRibbonButtonBarEvent& evt)
{
	wxMessageBox("Sources Written by David Nash 2011");
}


wxRibbonGalleryItem* MDIParentFrame::AddColourToGallery(wxRibbonGallery *gallery,
                                 wxString colour, wxMemoryDC& dc,
                                 wxColour* value)
{
    wxRibbonGalleryItem* item = NULL;
    wxColour c;
    if (colour != wxT("Default"))
        c = wxColour(colour);
    if (!c.IsOk())
        c = *value;
    if(c.IsOk())
    {
        const int iWidth = 64;
        const int iHeight = 40;

        wxBitmap bitmap(iWidth, iHeight);
        dc.SelectObject(bitmap);
        wxBrush b(c);
        dc.SetPen(*wxBLACK_PEN);
        dc.SetBrush(b);
        dc.DrawRectangle(0, 0, iWidth, iHeight);

        colour = colour.Mid(0, 1) + colour.Mid(1).Lower();
        wxSize size = dc.GetTextExtent(colour);
        wxColour foreground = wxColour(~c.Red(), ~c.Green(), ~c.Blue());
        if(abs(foreground.Red() - c.Red()) + abs(foreground.Blue() - c.Blue())
            + abs(foreground.Green() - c.Green()) < 64)
        {
            // Foreground too similar to background - use a different
            // strategy to find a contrasting colour
            foreground = wxColour((c.Red() + 64) % 256, 255 - c.Green(),
                (c.Blue() + 192) % 256);
        }
        dc.SetTextForeground(foreground);
        dc.DrawText(colour, (iWidth - size.GetWidth() + 1) / 2,
            (iHeight - size.GetHeight()) / 2);
        dc.SelectObjectAsSource(wxNullBitmap);

        item = gallery->Append(bitmap, wxID_ANY);
        gallery->SetItemClientObject(item, new ColourClientData(colour, c));
    }
    return item;
}

void MDIParentFrame::OnColourGalleryButton(wxCommandEvent& evt)
{
    wxRibbonGallery *gallery = wxDynamicCast(evt.GetEventObject(), wxRibbonGallery);
    if(gallery == NULL)
        return;

    m_ribbon->DismissExpandedPanel();
    if(gallery->GetSelection())
        m_colour_data.SetColour(GetGalleryColour(gallery, gallery->GetSelection(), NULL));
    wxColourDialog dlg(this, &m_colour_data);
    if(dlg.ShowModal() == wxID_OK)
    {
        m_colour_data = dlg.GetColourData();
        wxColour clr = m_colour_data.GetColour();

        // Try to find colour in gallery
        wxRibbonGalleryItem *item = NULL;
        for(unsigned int i = 0; i < gallery->GetCount(); ++i)
        {
            item = gallery->GetItem(i);
            if(GetGalleryColour(gallery, item, NULL) == clr)
                break;
            else
                item = NULL;
        }

        // Colour not in gallery - add it
        if(item == NULL)
        {
            item = AddColourToGallery(gallery,
                clr.GetAsString(wxC2S_HTML_SYNTAX), m_bitmap_creation_dc,
                &clr);
            gallery->Realise();
        }

        // Set selection
        gallery->EnsureVisible(item);
        gallery->SetSelection(item);

        // Send an event to respond to the selection change
        wxRibbonGalleryEvent dummy(wxEVT_COMMAND_RIBBONGALLERY_SELECTED, gallery->GetId());
        dummy.SetEventObject(gallery);
        dummy.SetGallery(gallery);
        dummy.SetGalleryItem(item);
        ProcessWindowEvent(dummy);
    }
}

void MDIParentFrame::OnLoadMediaFromDisk(wxRibbonButtonBarEvent& evt)
{
    wxFileDialog dialog(this,
                        wxT("Open a Media File"));

//    dialog.SetFilterIndex(0);

    if (dialog.ShowModal() == wxID_OK)
    {

		wxURI uripath(dialog.GetPath());
		subframe->OnLoadMedia(uripath);
	}

}
void MDIParentFrame::OnLoadMediaFromURL(wxRibbonButtonBarEvent& evt)
{
	wxTextEntryDialog ted(this, "Enter the URL");
	if( ted.ShowModal() == wxID_OK )
	{
		wxString val = ted.GetValue();
		val.Replace("watch?v=", "v/",true);
		wxMessageBox(val, "Loading this location");
		wxURI uripath(val);
		subframe->OnLoadMedia(uripath);
	}

}

void MDIParentFrame::OnDefaultProvider(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    m_ribbon->DismissExpandedPanel();
    SetArtProvider(new wxRibbonDefaultArtProvider);
}

void MDIParentFrame::OnAUIProvider(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    m_ribbon->DismissExpandedPanel();
    SetArtProvider(new wxRibbonAUIArtProvider);
}

void MDIParentFrame::OnMSWProvider(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    m_ribbon->DismissExpandedPanel();
    SetArtProvider(new wxRibbonMSWArtProvider);
}

void MDIParentFrame::SetArtProvider(wxRibbonArtProvider *prov)
{
    m_ribbon->Freeze();
    m_ribbon->SetArtProvider(prov);

    prov->GetColourScheme(&m_default_primary, &m_default_secondary,
        &m_default_tertiary);
    PopulateColoursPanel(m_primary_gallery->GetParent(), m_default_primary,
        ID_PRIMARY_COLOUR);
    PopulateColoursPanel(m_secondary_gallery->GetParent(), m_default_secondary,
        ID_SECONDARY_COLOUR);

    m_ribbon->Realize();
    m_ribbon->Thaw();
    GetSizer()->Layout();
}


#if wxUSE_MENUS
/* static */
wxMenuBar *MDIParentFrame::CreateMainMenubar()
{
    wxMenu *menuFile = new wxMenu;

    menuFile->Append(wxID_NEW, "&New window\tCtrl-N", "Create a new child window");
    menuFile->AppendCheckItem(MDI_FULLSCREEN, "Show &full screen\tCtrl-F");
    menuFile->Append(wxID_EXIT, "&Exit\tAlt-X", "Quit the program");

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "&About\tF1");

    wxMenuBar *mbar = new wxMenuBar;
    mbar->Append(menuFile, "&File");
    mbar->Append(menuHelp, "&Help");

    return mbar;
}
#endif // wxUSE_MENUS

void MDIParentFrame::OnClose(wxCloseEvent& event)
{
    unsigned numChildren = MyFrame::GetChildrenCount();
    if ( event.CanVeto() && (numChildren > 0) )
    {
        wxString msg;
        msg.Printf("%d windows still open, close anyhow?", numChildren);
        if ( wxMessageBox(msg, "Please confirm",
                          wxICON_QUESTION | wxYES_NO) != wxYES )
        {
            event.Veto();

            return;
        }
    }

    event.Skip();
}

void MDIParentFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void MDIParentFrame::OnAbout(wxCommandEvent& WXUNUSED(event) )
{
    (void)wxMessageBox("wxWidgets 2.0 MDI Demo\n"
                       "Author: Julian Smart (c) 1997\n"
                       "Usage: mdi.exe", "About MDI Demo");
}

void MDIParentFrame::OnNewWindow(wxCommandEvent& WXUNUSED(event) )
{
	static bool first_time = true;

	if( first_time )
    {
		// create and show another child frame
		//MyFrame *subframe = new MyFrame(this);
		//subframe->Show(true);
		first_time = false;
	}
}

void MDIParentFrame::OnFullScreen(wxCommandEvent& event)
{
    ShowFullScreen(event.IsChecked());
}

void MDIParentFrame::OnCloseAll(wxCommandEvent& WXUNUSED(event))
{
    for ( wxWindowList::const_iterator i = GetChildren().begin();
          i != GetChildren().end();
          ++i )
    {
        if ( wxDynamicCast(*i, wxMDIChildFrame) )
            (*i)->Close();
    }
}

void MDIParentFrame::OnSize(wxSizeEvent& event)
{
    int w, h;
    GetClientSize(&w, &h);

	int rbW, rbH;
	if( this->m_ribbon->ArePanelsShown() )
	{
		this->m_ribbon->SetSize(w,120);
		GetClientWindow()->SetSize(0, 120, w, h-120);
	}
	else
	{
		m_ribbon->GetSize(&rbW, &rbH );
		GetClientWindow()->SetSize(0, rbH, w, h-rbH);
	}
   // m_textWindow->SetSize(0, 0, 200, h);


    // FIXME: On wxX11, we need the MDI frame to process this
    // event, but on other platforms this should not
    // be done.
#ifdef __WXUNIVERSAL__
    event.Skip();
#else
    wxUnusedVar(event);
#endif
}

#if wxUSE_TOOLBAR
void MDIParentFrame::InitToolBar(wxToolBar* toolBar)
{
    wxBitmap bitmaps[8];

    bitmaps[0] = wxBitmap( new_xpm );
    bitmaps[1] = wxBitmap( open_xpm );
    bitmaps[2] = wxBitmap( save_xpm );
    bitmaps[3] = wxBitmap( copy_xpm );
    bitmaps[4] = wxBitmap( cut_xpm );
    bitmaps[5] = wxBitmap( paste_xpm );
    bitmaps[6] = wxBitmap( print_xpm );
    bitmaps[7] = wxBitmap( help_xpm );

    toolBar->AddTool(wxID_NEW, "New", bitmaps[0], "New file");
    toolBar->AddTool(1, "Open", bitmaps[1], "Open file");
    toolBar->AddTool(2, "Save", bitmaps[2], "Save file");
    toolBar->AddSeparator();
    toolBar->AddTool(3, "Copy", bitmaps[3], "Copy");
    toolBar->AddTool(4, "Cut", bitmaps[4], "Cut");
    toolBar->AddTool(5, "Paste", bitmaps[5], "Paste");
    toolBar->AddSeparator();
    toolBar->AddTool(6, "Print", bitmaps[6], "Print");
    toolBar->AddSeparator();
    toolBar->AddTool(wxID_ABOUT, "About", bitmaps[7], "Help");

    toolBar->Realize();
}
#endif // wxUSE_TOOLBAR


/*
// This implements a tiny doodling program! Drag the mouse using the left
// button.
void MyCanvas::OnEvent(wxMouseEvent& event)
{
    wxClientDC dc(this);
    PrepareDC(dc);

    wxPoint pt(event.GetLogicalPosition(dc));

    static long xpos = -1;
    static long ypos = -1;

    if (xpos > -1 && ypos > -1 && event.Dragging())
    {
        dc.SetPen(*wxBLACK_PEN);
        dc.DrawLine(xpos, ypos, pt.x, pt.y);

        m_dirty = true;
    }

    xpos = pt.x;
    ypos = pt.y;
}
*/


// ---------------------------------------------------------------------------
// MyFrame
// ---------------------------------------------------------------------------

unsigned MyFrame::ms_numChildren = 0;

MyFrame::MyFrame(MDIParentFrame *parent)
       : wxMDIChildFrame
         (
            parent,
            wxID_ANY,
            wxString::Format("Child %u", ++ms_numChildren)
         )
{

	m_parent = parent;

	//SetDoubleBuffered(true);
	//this->Maximize(true);
	//this->Unset
    // tell wxAuiManager to manage this frame
  	//variationSelectionBox = NULL;
	//currentGame = NULL;
	//this->Maximize(true);
	
	//this->UseNativeStatusBar(true);



 // m_listCtrl = NULL;
    //m_logWindow = NULL;
//    m_smallVirtual = false;
 //   m_numListItems = 10;

    // Give it an icon
    //SetIcon(wxICON(sample));

    // Make an image list containing large icons



/*
    // Make a menubar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(LIST_ABOUT, wxT("&About"));
    menuFile->AppendSeparator();
    menuFile->Append(LIST_QUIT, wxT("E&xit\tAlt-X"));

    wxMenu *menuView = new wxMenu;
    menuView->Append(LIST_LIST_VIEW, wxT("&List view\tF1"));
    menuView->Append(LIST_REPORT_VIEW, wxT("&Report view\tF2"));
    menuView->Append(LIST_ICON_VIEW, wxT("&Icon view\tF3"));
    menuView->Append(LIST_ICON_TEXT_VIEW, wxT("Icon view with &text\tF4"));
    menuView->Append(LIST_SMALL_ICON_VIEW, wxT("&Small icon view\tF5"));
    menuView->Append(LIST_SMALL_ICON_TEXT_VIEW, wxT("Small icon &view with text\tF6"));
    menuView->Append(LIST_VIRTUAL_VIEW, wxT("&Virtual view\tF7"));
    menuView->Append(LIST_SMALL_VIRTUAL_VIEW, wxT("Small virtual vie&w\tF8"));
    menuView->AppendSeparator();
    menuView->Append(LIST_SET_ITEMS_COUNT, "Set &number of items");
#ifdef __WXOSX__
    menuView->AppendSeparator();
    menuView->AppendCheckItem(LIST_MAC_USE_GENERIC, wxT("Mac: Use Generic Control"));
#endif

    wxMenu *menuList = new wxMenu;
    menuList->Append(LIST_GOTO, wxT("&Go to item #3\tCtrl-3"));
    menuList->Append(LIST_FOCUS_LAST, wxT("&Make last item current\tCtrl-L"));
    menuList->Append(LIST_TOGGLE_FIRST, wxT("To&ggle first item\tCtrl-G"));
    menuList->Append(LIST_DESELECT_ALL, wxT("&Deselect All\tCtrl-D"));
    menuList->Append(LIST_SELECT_ALL, wxT("S&elect All\tCtrl-A"));
    menuList->AppendSeparator();
    menuList->Append(LIST_SHOW_COL_INFO, wxT("Show &column info\tCtrl-C"));
    menuList->Append(LIST_SHOW_SEL_INFO, wxT("Show &selected items\tCtrl-S"));
    menuList->Append(LIST_SHOW_VIEW_RECT, wxT("Show &view rect"));
#ifdef wxHAS_LISTCTRL_COLUMN_ORDER
    menuList->Append(LIST_SET_COL_ORDER, wxT("Se&t columns order\tShift-Ctrl-O"));
    menuList->Append(LIST_GET_COL_ORDER, wxT("Sho&w columns order\tCtrl-O"));
#endif // wxHAS_LISTCTRL_COLUMN_ORDER
    menuList->AppendSeparator();
    menuList->Append(LIST_SORT, wxT("Sor&t\tCtrl-T"));
    menuList->Append(LIST_FIND, "Test Find() performance");
    menuList->AppendSeparator();
    menuList->Append(LIST_ADD, wxT("&Append an item\tCtrl-P"));
    menuList->Append(LIST_EDIT, wxT("&Edit the item\tCtrl-E"));
    menuList->Append(LIST_DELETE, wxT("&Delete first item\tCtrl-X"));
    menuList->Append(LIST_DELETE_ALL, wxT("Delete &all items"));
    menuList->AppendSeparator();
    menuList->Append(LIST_FREEZE, wxT("Free&ze\tCtrl-Z"));
    menuList->Append(LIST_THAW, wxT("Tha&w\tCtrl-W"));
    menuList->AppendSeparator();
    menuList->AppendCheckItem(LIST_TOGGLE_LINES, wxT("Toggle &lines\tCtrl-I"));
    menuList->AppendCheckItem(LIST_TOGGLE_MULTI_SEL,
                              wxT("&Multiple selection\tCtrl-M"));
    menuList->Check(LIST_TOGGLE_MULTI_SEL, true);
    menuList->AppendCheckItem(LIST_TOGGLE_HEADER, "Toggle &header\tCtrl-H");
    menuList->Check(LIST_TOGGLE_HEADER, true);

    wxMenu *menuCol = new wxMenu;
    menuCol->Append(LIST_SET_FG_COL, wxT("&Foreground colour..."));
    menuCol->Append(LIST_SET_BG_COL, wxT("&Background colour..."));

    wxMenuBar *menubar = new wxMenuBar;
    menubar->Append(menuFile, wxT("&File"));
    menubar->Append(menuView, wxT("&View"));
    menubar->Append(menuList, wxT("&List"));
    menubar->Append(menuCol, wxT("&Colour"));
    SetMenuBar(menubar);
	*/

 //   m_panel = new wxPanel(this, wxID_ANY);
 //   m_logWindow = new wxTextCtrl(m_panel, wxID_ANY, wxEmptyString,
 //                                wxDefaultPosition, wxDefaultSize,
//                                 wxTE_READONLY | wxTE_MULTILINE | wxBORDER_THEME);

//	m_panel->Hide();
//	m_logWindow->Hide();

	m_gameTraverseTimer = NULL;
//    m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_logWindow));
	optionsFrame = new OptionsFrame(this);

	m_engineManager = new EngineManager();
	

	for( int i = 0; i < MAX_NUM_PIPE_PANELS; i++ )
		m_pipePanel[i] = NULL;

	m_pipePanel[0] = new MyPipePanel(this);
	m_pipePanel[1] = new MyPipePanel(this);

	numActivePipePanels = 2;

	m_pipePanel[0]->SetEngineManager(m_engineManager,0);
	m_pipePanel[1]->SetEngineManager(m_engineManager,1);


	m_listPanel = new MyListPanel(this);

    RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);



	m_analyser = new GameAnalyser(this, m_pipePanel[0]);
//	m_panelFolderList = new wxPanel(this, wxID_ANY);	
  //  m_folderList = new MyListCtrl(this, this, wxID_ANY,
  //                                  wxDefaultPosition, wxDefaultSize,
  //                                  wxLC_ICON|
  //                                  wxBORDER_THEME );

	m_folderList = new MyListPanel(this);

//	m_folderList->InitWithIconItems(true, false);

	// note these will swap places eventually
	pgnLoad.Startup();
	currentGame = pgnLoad.games[0];
	//currentGame->CreateEmptyHeader();
	currentGame->ParseHeader();
    
	
	

	 noteFrame = new NotationFrame(this, &my_canvas);
   //wxFrame *fr = new wxFrame(ctrl, wxID_ANY, "Notation");
   //wxPanel *panel1 = new wxPanel(this, wxID_ANY);
   //my_canvas = new MyCanvas(this,panel1);

	 //wxMessageBox("Setting scrollbars");
	my_canvas->SetScrollbars( 10, 10, 50, 1024 );	
	
	//wxMessageBox("Setting managed window");
	// tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);
	//m_mgr.SetArtProvider(::wxAuiDockArt::
	//m_mgr.SetArtProvider(::wxAui
    // set frame icon
//    SetIcon(wxIcon(sample_xpm));




    // set up default notebook style
    m_notebook_style = wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_EXTERNAL_MOVE| wxAUI_NB_WINDOWLIST_BUTTON|wxNO_BORDER;
    m_notebook_theme = 1;
/*
    // create menu
    wxMenuBar* mb = new wxMenuBar;
	//this->OnLoadPGN
    wxMenu* file_menu = new wxMenu;
	file_menu->Append(ID_LoadPGN, _("Load PGN"));
	file_menu->Append(ID_LoadPGNFromClipBoard, _("Load PGN From Clipboard"));
	file_menu->Append(ID_MergePGNFromClipBoard, _("Merge PGN From Clipboard"));
	file_menu->Append(ID_CopyPGNToClipBoard, _("Copy PGN To Clipboard"));
	file_menu->Append(ID_CopyFENToClipBoard, _("Copy FEN To Clipboard"));
	file_menu->Append(ID_SavePGN, _("Save PGN"));
    file_menu->Append(wxID_EXIT, _("Exit"));
	
    wxMenu* view_menu = new wxMenu;
    view_menu->Append(ID_CreateText, _("Create Text Control"));
    view_menu->Append(ID_CreateHTML, _("Create HTML Control"));
    view_menu->Append(ID_CreateTree, _("Create Tree"));
    view_menu->Append(ID_CreateGrid, _("Create Grid"));
    view_menu->Append(ID_CreateNotebook, _("Create Notebook"));
    view_menu->Append(ID_CreateSizeReport, _("Create Size Reporter"));
    view_menu->AppendSeparator();
    view_menu->Append(ID_GridContent, _("Use a Grid for the Content Pane"));
    view_menu->Append(ID_TextContent, _("Use a Text Control for the Content Pane"));
    view_menu->Append(ID_HTMLContent, _("Use an HTML Control for the Content Pane"));
    view_menu->Append(ID_TreeContent, _("Use a Tree Control for the Content Pane"));
    view_menu->Append(ID_NotebookContent, _("Use a wxAuiNotebook control for the Content Pane"));
    view_menu->Append(ID_SizeReportContent, _("Use a Size Reporter for the Content Pane"));

    wxMenu* options_menu = new wxMenu;
    options_menu->AppendRadioItem(ID_TransparentHint, _("Transparent Hint"));
    options_menu->AppendRadioItem(ID_VenetianBlindsHint, _("Venetian Blinds Hint"));
    options_menu->AppendRadioItem(ID_RectangleHint, _("Rectangle Hint"));
    options_menu->AppendRadioItem(ID_NoHint, _("No Hint"));
    options_menu->AppendSeparator();
    options_menu->AppendCheckItem(ID_HintFade, _("Hint Fade-in"));
    options_menu->AppendCheckItem(ID_AllowFloating, _("Allow Floating"));
    options_menu->AppendCheckItem(ID_NoVenetianFade, _("Disable Venetian Blinds Hint Fade-in"));
    options_menu->AppendCheckItem(ID_TransparentDrag, _("Transparent Drag"));
    options_menu->AppendCheckItem(ID_AllowActivePane, _("Allow Active Pane"));
    options_menu->AppendCheckItem(ID_LiveUpdate, _("Live Resize Update"));
    options_menu->AppendSeparator();
    options_menu->AppendRadioItem(ID_NoGradient, _("No Caption Gradient"));
    options_menu->AppendRadioItem(ID_VerticalGradient, _("Vertical Caption Gradient"));
    options_menu->AppendRadioItem(ID_HorizontalGradient, _("Horizontal Caption Gradient"));
    options_menu->AppendSeparator();
    options_menu->Append(ID_Settings, _("Settings Pane"));

    wxMenu* notebook_menu = new wxMenu;
    notebook_menu->AppendRadioItem(ID_NotebookArtGloss, _("Glossy Theme (Default)"));
    notebook_menu->AppendRadioItem(ID_NotebookArtSimple, _("Simple Theme"));
    notebook_menu->AppendSeparator();
    notebook_menu->AppendRadioItem(ID_NotebookNoCloseButton, _("No Close Button"));
    notebook_menu->AppendRadioItem(ID_NotebookCloseButton, _("Close Button at Right"));
    notebook_menu->AppendRadioItem(ID_NotebookCloseButtonAll, _("Close Button on All Tabs"));
    notebook_menu->AppendRadioItem(ID_NotebookCloseButtonActive, _("Close Button on Active Tab"));
    notebook_menu->AppendSeparator();
    notebook_menu->AppendRadioItem(ID_NotebookAlignTop, _("Tab Top Alignment"));
    notebook_menu->AppendRadioItem(ID_NotebookAlignBottom, _("Tab Bottom Alignment"));
    notebook_menu->AppendSeparator();
    notebook_menu->AppendCheckItem(ID_NotebookAllowTabMove, _("Allow Tab Move"));
    notebook_menu->AppendCheckItem(ID_NotebookAllowTabExternalMove, _("Allow External Tab Move"));
    notebook_menu->AppendCheckItem(ID_NotebookAllowTabSplit, _("Allow Notebook Split"));
    notebook_menu->AppendCheckItem(ID_NotebookScrollButtons, _("Scroll Buttons Visible"));
    notebook_menu->AppendCheckItem(ID_NotebookWindowList, _("Window List Button Visible"));
    notebook_menu->AppendCheckItem(ID_NotebookTabFixedWidth, _("Fixed-width Tabs"));

    m_perspectives_menu = new wxMenu;
    m_perspectives_menu->Append(ID_CreatePerspective, _("Create Perspective"));
    m_perspectives_menu->Append(ID_CopyPerspectiveCode, _("Copy Perspective Data To Clipboard"));
    m_perspectives_menu->AppendSeparator();
    m_perspectives_menu->Append(ID_FirstPerspective+0, _("Default Startup"));
    m_perspectives_menu->Append(ID_FirstPerspective+1, _("All Panes"));

    wxMenu* help_menu = new wxMenu;
    help_menu->Append(wxID_ABOUT, _("About..."));

    mb->Append(file_menu, _("File"));
    mb->Append(view_menu, _("View"));
    mb->Append(m_perspectives_menu, _("Perspectives"));
    mb->Append(options_menu, _("Options"));
    mb->Append(notebook_menu, _("Notebook"));
    mb->Append(help_menu, _("Help"));

    SetMenuBar(mb);
*/
    //CreateStatusBar();
   // GetStatusBar()->SetStatusText(_("Ready"));


    // min size for the frame itself isn't completely done.
    // see the end up wxAuiManager::Update() for the test
    // code. For now, just hard code a frame minimum size
	//wxMessageBox("Setting min size");
    SetMinSize(wxSize(800,600));






    // prepare a few custom overflow elements for the toolbars' overflow buttons
	//wxMessageBox("Setting up toolbars");
    wxAuiToolBarItemArray prepend_items;
    wxAuiToolBarItemArray append_items;
    wxAuiToolBarItem item;
    item.SetKind(wxITEM_SEPARATOR);
    append_items.Add(item);
    item.SetKind(wxITEM_NORMAL);
    item.SetId(ID_CustomizeToolbar);
    item.SetLabel(_("Customize..."));
    append_items.Add(item);




	
    // create some toolbars
    wxAuiToolBar* tb1 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
    tb1->SetToolBitmapSize(wxSize(48,48));
    tb1->AddTool(ID_SampleItem+1, wxT("Test"), wxArtProvider::GetBitmap(wxART_ERROR));
    tb1->AddSeparator();
	
    /*wxImage img(wxT("images/flipboard32.bmp"));
        if ( img.Ok() )
        {
            if ( img.GetWidth() > 32 && img.GetHeight() > 32 )
                img = img.GetSubImage(wxRect(0, 0, 32, 32));

			tb1->AddTool(ID_SampleItem+2, wxT("Test"), img);
		}
		else*/
		{
			tb1->AddTool(ID_SampleItem+2, wxT("Test"), wxArtProvider::GetBitmap(wxART_QUESTION));
		}

	//bmap.LoadFile(wxT("images/flipboard32.png"),wxBITMAP_TYPE_PNG);
   // tb1->AddTool(ID_SampleItem+2, wxT("Test"), img);// wxArtProvider::GetBitmap(wxART_QUESTION));
    tb1->AddTool(ID_SampleItem+3, wxT("Test"), wxArtProvider::GetBitmap(wxART_INFORMATION));
    tb1->AddTool(ID_SampleItem+4, wxT("Test"), wxArtProvider::GetBitmap(wxART_WARNING));
    tb1->AddTool(ID_SampleItem+5, wxT("Test"), wxArtProvider::GetBitmap(wxART_MISSING_IMAGE));
    tb1->SetCustomOverflowItems(prepend_items, append_items);
    tb1->Realize();


    wxAuiToolBar* tb2 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);



	//tb2->SetBackgroundColour(*wxGREY);
	//tb2->SetForegroundColour(*wxBLACK);
    tb2->SetToolBitmapSize(wxSize(16,16));

	wxBitmap beginBmp;
	beginBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/begin_small.png", wxBITMAP_TYPE_PNG);

	wxBitmap prevBmp;
	prevBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/back_small.png", wxBITMAP_TYPE_PNG);

	wxBitmap playBmp;
	playBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/play_small_green.png", wxBITMAP_TYPE_PNG);

	wxBitmap nextBmp;
	nextBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/forward_small.png", wxBITMAP_TYPE_PNG);

	wxBitmap endBmp;
	endBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/end_small.png", wxBITMAP_TYPE_PNG);

    wxBitmap tb2_bmp1 = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16,16));
    tb2->AddTool(ID_SampleItem+6, wxT("Test"), beginBmp);
    tb2->AddTool(ID_SampleItem+7, wxT("Test"), prevBmp);
    tb2->AddTool(ID_SampleItem+8, wxT("Test"), playBmp);
    tb2->AddTool(ID_SampleItem+9, wxT("Test"), nextBmp);
	tb2->AddTool(ID_SampleItem+10, wxT("Test"), endBmp);
    tb2->AddSeparator();
	wxSlider *slider = new wxSlider(tb2, wxID_ANY, 0,0,10);
	
	tb2->AddControl(slider,"slider");
	//tb2->AddChild(mClock);
	//tb2->SetLabel("toolbar");
	//tb2->AddControl(mClock);
	//tb2->AddControl(
 //   tb2->AddTool(ID_SampleItem+10, wxT("Test"), tb2_bmp1);
 //   tb2->AddTool(ID_SampleItem+11, wxT("Test"), tb2_bmp1);
 //   tb2->AddSeparator();
 //   tb2->AddTool(ID_SampleItem+12, wxT("Test"), tb2_bmp1);
 //   tb2->AddTool(ID_SampleItem+13, wxT("Test"), tb2_bmp1);
 //   tb2->AddTool(ID_SampleItem+14, wxT("Test"), tb2_bmp1);
 //   tb2->AddTool(ID_SampleItem+15, wxT("Test"), tb2_bmp1);
    tb2->SetCustomOverflowItems(prepend_items, append_items);
    tb2->Realize();

/*	
    wxAuiToolBar* tb3 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
    tb3->SetToolBitmapSize(wxSize(16,16));
    tb3->SetOwnBackgroundColour(*wxBLACK);
	 tb3->SetOwnForegroundColour(*wxBLACK);
	wxBitmap tb3_bmp1 = wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16));
    tb3->AddTool(ID_SampleItem+16, wxT("Check 1"), tb3_bmp1, wxT("Check 1"), wxITEM_CHECK);
    tb3->AddTool(ID_SampleItem+17, wxT("Check 2"), tb3_bmp1, wxT("Check 2"), wxITEM_CHECK);
    tb3->AddTool(ID_SampleItem+18, wxT("Check 3"), tb3_bmp1, wxT("Check 3"), wxITEM_CHECK);
    tb3->AddTool(ID_SampleItem+19, wxT("Check 4"), tb3_bmp1, wxT("Check 4"), wxITEM_CHECK);
    tb3->AddSeparator();
    tb3->AddTool(ID_SampleItem+20, wxT("Radio 1"), tb3_bmp1, wxT("Radio 1"), wxITEM_RADIO);
    tb3->AddTool(ID_SampleItem+21, wxT("Radio 2"), tb3_bmp1, wxT("Radio 2"), wxITEM_RADIO);
    tb3->AddTool(ID_SampleItem+22, wxT("Radio 3"), tb3_bmp1, wxT("Radio 3"), wxITEM_RADIO);
    tb3->AddSeparator();
    tb3->AddTool(ID_SampleItem+23, wxT("Radio 1 (Group 2)"), tb3_bmp1, wxT("Radio 1 (Group 2)"), wxITEM_RADIO);
    tb3->AddTool(ID_SampleItem+24, wxT("Radio 2 (Group 2)"), tb3_bmp1, wxT("Radio 2 (Group 2)"), wxITEM_RADIO);
    tb3->AddTool(ID_SampleItem+25, wxT("Radio 3 (Group 2)"), tb3_bmp1, wxT("Radio 3 (Group 2)"), wxITEM_RADIO);
    tb3->SetCustomOverflowItems(prepend_items, append_items);
    tb3->Realize();


    wxAuiToolBar* tb4 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE |
                                         wxAUI_TB_OVERFLOW |
                                         wxAUI_TB_TEXT |
                                         wxAUI_TB_HORZ_TEXT);
    tb4->SetToolBitmapSize(wxSize(16,16));
    wxBitmap tb4_bmp1 = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));
    tb4->SetOwnBackgroundColour(*wxBLACK);
	 tb4->SetOwnForegroundColour(*wxBLACK);
	 tb4->ShouldInheritColours();
	 //tb3->SetOwnForegroundColour(*wxBLACK);
	tb4->AddTool(ID_DropDownToolbarItem, wxT("Item 1"), tb4_bmp1);
    tb4->AddTool(ID_SampleItem+23, wxT("Item 2"), tb4_bmp1);
    tb4->AddTool(ID_SampleItem+24, wxT("Item 3"), tb4_bmp1);
    tb4->AddTool(ID_SampleItem+25, wxT("Item 4"), tb4_bmp1);
    tb4->AddSeparator();
    tb4->AddTool(ID_SampleItem+26, wxT("Item 5"), tb4_bmp1);
    tb4->AddTool(ID_SampleItem+27, wxT("Item 6"), tb4_bmp1);
    tb4->AddTool(ID_SampleItem+28, wxT("Item 7"), tb4_bmp1);
    tb4->AddTool(ID_SampleItem+29, wxT("Item 8"), tb4_bmp1);
    tb4->SetToolDropDown(ID_DropDownToolbarItem, true);
    tb4->SetCustomOverflowItems(prepend_items, append_items);
    tb4->Realize();


    wxAuiToolBar* tb5 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW | wxAUI_TB_VERTICAL);
    tb5->SetToolBitmapSize(wxSize(48,48));
    tb5->AddTool(ID_SampleItem+30, wxT("Test"), wxArtProvider::GetBitmap(wxART_ERROR));
    tb5->AddSeparator();
    tb5->AddTool(ID_SampleItem+31, wxT("Test"), wxArtProvider::GetBitmap(wxART_QUESTION));
    tb5->AddTool(ID_SampleItem+32, wxT("Test"), wxArtProvider::GetBitmap(wxART_INFORMATION));
    tb5->AddTool(ID_SampleItem+33, wxT("Test"), wxArtProvider::GetBitmap(wxART_WARNING));
    tb5->AddTool(ID_SampleItem+34, wxT("Test"), wxArtProvider::GetBitmap(wxART_MISSING_IMAGE));
    tb5->SetCustomOverflowItems(prepend_items, append_items);
    tb5->Realize();
	*/

//	wxMessageBox("toolbars done");

//	wxMessageBox("adding panes");
    // add a bunch of panes
    m_mgr.AddPane(m_folderList, wxAuiPaneInfo().
                  Name(wxT("test1")).Caption(wxT("Pane Caption")).
                  Top());

	//m_sampleFrame = new CLCDSampleFrame(wxT("Clock"), wxDefaultPosition, wxDefaultSize);

	//wxMessageBox("creating options frame");
	
	//wxMessageBox("done creating options frame");
    m_mgr.AddPane(optionsFrame, wxAuiPaneInfo().
                  Name(wxT("test2")).Caption(wxT("options")).MinSize(300,150).
                  Bottom().Position(1).
				  CloseButton(true).MaximizeButton(true).DestroyOnClose(false));

    m_mgr.AddPane(m_pipePanel[0], wxAuiPaneInfo().
                  Name(wxT("Engine_Panel")).Caption(wxT("Engine")).
                  Bottom().MinSize(300,150).
                  CloseButton(true).MaximizeButton(true).DestroyOnClose(false));
	m_pipePanel[0]->SetPanelName("Engine_Panel");
    m_mgr.AddPane(m_pipePanel[1], wxAuiPaneInfo().
                  Name(wxT("Engine_Panel_2")).Caption(wxT("Engine")).
                  Bottom().Position(1).MinSize(300,150).
                  CloseButton(true).MaximizeButton(true).DestroyOnClose(false));
	m_pipePanel[1]->SetPanelName("Engine_Panel_2");
    m_mgr.AddPane(m_listPanel, wxAuiPaneInfo().
                  Name(wxT("test4")).Caption(wxT("Pane Caption")).
                  Left());


	//wxMessageBox("Creating tree canvas");
	treeCanvas = new TreeCanvas(this);

    m_mgr.AddPane(treeCanvas, wxAuiPaneInfo().
                  Name(wxT("test5")).Caption(wxT("Tree Browser")).
                  Right().CloseButton(false).Hide());

moveDataCanvas = new MoveDataCanvas(this);
    m_mgr.AddPane(moveDataCanvas, wxAuiPaneInfo().
		Name(wxT("moveData")).CaptionVisible(false).
                  Right().CloseButton(false).Show());

	//wxMessageBox("Creating game clock");
gameClock = new GameClockPanel(this);//CreateSizeReportCtrl()
    m_mgr.AddPane(gameClock, wxAuiPaneInfo().
                  Name(wxT("clock_pane")).Caption(wxT("Game Clock")).
                  Right().Row(1).
                  CloseButton(true).MaximizeButton(true));

/*m_richTextCtrl = new wxRichTextCtrl(this, 16, wxEmptyString, wxDefaultPosition, wxSize(200, 200), wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS|
wxRE_READONLY);
   
m_richTextCtrl->SetDefaultStyle(wxRichTextAttr());
m_richTextCtrl->BeginParagraphSpacing(0, 20);
m_richTextCtrl->BeginBold();
    m_richTextCtrl->BeginFontSize(14);

    wxString lineBreak = (wxChar) 29;

   m_richTextCtrl->WriteText(wxString(wxT("Welcome to wxRichTextCtrl, a wxWidgets control")) + lineBreak + wxT("for editing and presenting styled text and images\n"));
    m_richTextCtrl->EndFontSize();
    //r.Newline();

    m_richTextCtrl->BeginItalic();
	m_richTextCtrl->BeginTextColour(wxColour(*wxRED));
    m_richTextCtrl->WriteText(wxT("by Julian Smart"));
    m_richTextCtrl->EndItalic();
	m_richTextCtrl->EndTextColour();

    m_richTextCtrl->EndBold();
    m_richTextCtrl->Newline();	

    wxRichTextAttr backgroundColourAttr;
    backgroundColourAttr.SetBackgroundColour(*wxGREEN);
    backgroundColourAttr.SetTextColour(wxColour(0, 0, 255));
    m_richTextCtrl->BeginStyle(backgroundColourAttr);
    m_richTextCtrl->WriteText(wxT(" And this blue on green bit."));
    m_richTextCtrl->EndStyle();
	*/
	//m_richTextCtrl->allow
	//wxMessageBox("Creating data view");
	m_dataView = new GameDataViewPanel(this);
	m_mgr.AddPane(m_dataView, wxAuiPaneInfo().
                  Name(wxT("test7")).Caption(wxT("Dummy1")).
                  Left().Layer(1).
                  CloseButton(true).MaximizeButton(true));
				  

  /*  m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().
                  Name(wxT("test8")).Caption(wxT("Tree Pane")).
                  Left().Layer(1).Position(2).
                  CloseButton(true).MaximizeButton(true));
				  */

	m_mediactrl = new wxMediaCtrl(this, wxID_ANY);
	//wxURI uripath("http://www.youtube.com/v/SQWY4UUZzhc");
	//OnLoadMedia(uripath);
	//m_mediactrl->Load
    m_mgr.AddPane(m_mediactrl, wxAuiPaneInfo().
                  Name(wxT("test9")).Caption(wxT("Media")).
                  BestSize(wxSize(200,100)).MinSize(wxSize(200,100)).
                  Bottom().Layer(1).
                  CloseButton(true).MaximizeButton(true));
				  
    wnd10 = CreateTextCtrl(wxT(""));
		wxFont fontWnd10(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD);
		wnd10->SetFont(fontWnd10);
		wnd10->SetForegroundColour(*wxBLUE);
    m_mgr.AddPane(wnd10, wxAuiPaneInfo().
                  Name(wxT("test10")).Caption(wxT("Game Annotation")).
                  Bottom().Layer(1).Position(1));
				  
   m_mgr.AddPane(m_analyser, wxAuiPaneInfo().
                  Name(wxT("test11")).Caption(wxT("Fixed Pane")).
                  Bottom().Layer(1).Position(2));
   //wxMessageBox(wxString("Hello"));
   
   //wxMessageBox("Creating board canvas");
    bool ok = m_mgr.AddPane(CreateBoardCanvas(), wxAuiPaneInfo().
		Name(wxT("test12")).Caption(wxT("Graphical Pane")).BestSize(wxSize(600,600)).MinSize(400,400).
                  Left().Layer(1).Position(1).DestroyOnClose(false).PinButton(true).
                  CloseButton(true).MaximizeButton(true));
	//wxMessageBox("Done creating board canvas");
	//wxMessageBox(wxString("Bye"));

	if( !ok )
		return;

	//wxMessageBox("Adding noteframe");
	m_mgr.AddPane(noteFrame, wxAuiPaneInfo().Name(wxT("notebook_content")).Caption(wxT("Notation"))
		.CenterPane().DestroyOnClose(false).CaptionVisible(true));
		
/*m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().
                  Name(wxT("notebook_content")).Caption(wxT("NoteBook")).
                  Left().Layer(1).Position(1).
                  CloseButton(true).MaximizeButton(true));

	    m_mgr.AddPane(CreateGL(), wxAuiPaneInfo().Name(wxT("test12")).Caption(wxT("Graphical Pane")).BestSize(wxSize(600,600)).MinSize(wxSize(600,600)).
		CenterPane().PaneBorder(true).Floatable(true).Movable(true).Dockable(true));
		*/
		//CreateGL();
	//wxMessageBox("Creating a bunch of optional center panes");
    m_mgr.AddPane(new SettingsPanel(this,this), wxAuiPaneInfo().
                  Name(wxT("settings")).Caption(wxT("Dock Manager Settings")).
                  Dockable(false).Float().Hide());

    // create some center panes

    m_mgr.AddPane(CreateGrid(), wxAuiPaneInfo().Name(wxT("grid_content")).
                  CenterPane().Hide());

    m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().Name(wxT("tree_content")).
                  CenterPane().Hide());

    m_mgr.AddPane(CreateSizeReportCtrl(), wxAuiPaneInfo().Name(wxT("sizereport_content")).
		CenterPane().CaptionVisible(true).Hide());

    m_mgr.AddPane(CreateTextCtrl(), wxAuiPaneInfo().Name(wxT("text_content")).
                  CenterPane().Hide());

    m_mgr.AddPane(CreateHTMLCtrl(), wxAuiPaneInfo().Name(wxT("html_content")).
                  CenterPane().Hide());

	//wxMessageBox("Done adding center panes");

	//wxMessageBox("Adding toolbars");
    // add the toolbars to the manager
    m_mgr.AddPane(tb1, wxAuiPaneInfo().
                  Name(wxT("tb1")).Caption(wxT("Big Toolbar")).
                  ToolbarPane().Top().Row(1).Position(2).
                  LeftDockable(false).RightDockable(false));

    m_mgr.AddPane(tb2, wxAuiPaneInfo().
                  Name(wxT("tb2")).Caption(wxT("Toolbar 2")).
                  ToolbarPane().Top().Row(1).
                  LeftDockable(false).RightDockable(false));

   /* m_mgr.AddPane(tb3, wxAuiPaneInfo().
                  Name(wxT("tb3")).Caption(wxT("Toolbar 3")).
                  ToolbarPane().Top().Row(1).Position(1).
                  LeftDockable(false).RightDockable(false));

    m_mgr.AddPane(tb4, wxAuiPaneInfo().
                  Name(wxT("tb4")).Caption(wxT("Sample Bookmark Toolbar")).
                  ToolbarPane().Top().Row(2).
                  LeftDockable(false).RightDockable(false));

    m_mgr.AddPane(tb5, wxAuiPaneInfo().
                  Name(wxT("tb5")).Caption(wxT("Sample Vertical Toolbar")).
                  ToolbarPane().Left().
                  GripperTop().
                  TopDockable(false).BottomDockable(false));

    m_mgr.AddPane(new wxButton(this, wxID_ANY, _("Test Button")),
                  wxAuiPaneInfo().Name(wxT("tb6")).
                  ToolbarPane().Top().Row(2).Position(1).
                  LeftDockable(false).RightDockable(false));
				  */
    // make some default perspectives

//    perspective_all = m_mgr.SavePerspective();

  /*  int i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
        if (!all_panes.Item(i).IsToolbar())
            all_panes.Item(i).Hide();
    m_mgr.GetPane(wxT("tb1")).Hide();
 //   m_mgr.GetPane(wxT("tb6")).Hide();

	/// BIG NOTE: THIS SETS WHICH PANES ARE VISIBLE AND WHERE THEY APPEAR - OVERIDES THE OTHER PANELS
    m_mgr.GetPane(wxT("test8")).Show().Right().Layer(0).Row(0).Position(0);
    m_mgr.GetPane(wxT("test10")).Show().Bottom().Layer(0).Row(0).Position(0);
	m_mgr.GetPane(wxT("test12")).Show().Left().Layer(0).Row(0).Position(0);  
//	m_mgr.GetPane(wxT("clock_pane")).Show().Right().Layer(0).Position(2); 
    m_mgr.GetPane(wxT("notebook_content")).Show();
    wxString perspective_default = m_mgr.SavePerspective();
	*/
	//wxMessageBox("Done adding toolbars");


	m_mgr.GetPane(wxT("notebook_content")).Show();
	m_mgr.GetPane(wxT("test5")).Show(true);
	perspective_all = m_mgr.SavePerspective();
    //m_perspectives.Add(perspective_default);
    m_perspectives.Add(perspective_all);

int i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
        if (!all_panes.Item(i).IsToolbar())
            all_panes.Item(i).Hide();

	//wxMessageBox("Saving perspectives");

    // "commit" all changes made to wxAuiManager
    m_mgr.Update();
	
//wxMessageBox("m_mgr.Update()");
//	m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=Client Size Reporter;state=6293500;dir=3;layer=2;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=852;floaty=512;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=2;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=398;floaty=412;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=No Close Button;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Client Size Reporter;state=6293500;dir=2;layer=1;row=0;pos=1;prop=62069;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=194;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=1;floaty=193;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(3,0,1)=109|dock_size(2,1,0)=248|dock_size(3,2,0)=169|dock_size(4,3,0)=516|", true);
//	m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=Client Size Reporter;state=6293500;dir=2;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=426;floaty=381;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=4;row=0;pos=0;prop=86926;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=414;floaty=373;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=No Close Button;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Client Size Reporter;state=6293500;dir=3;layer=4;row=0;pos=1;prop=75143;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=530;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-5;floaty=143;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=3;layer=10;row=0;pos=270;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=260;floaty=201;floatw=256;floath=76|name=tb2;caption=Toolbar 2;state=2106096;dir=3;layer=10;row=0;pos=6;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-5;floaty=669;floatw=278;floath=62|dock_size(5,0,0)=34|dock_size(3,0,1)=68|dock_size(2,1,0)=248|dock_size(3,10,0)=44|dock_size(3,4,0)=169|dock_size(4,5,0)=501|",true);

    // connect event handlers for the blue input window
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyFrame::OnKeyDown),
                        NULL, this);
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyFrame::OnKeyUp),
                        NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(MyFrame::OnChar),
                        NULL, this);
    // test that event handlers pushed on top of MDI children do work (this
    // used to be broken, see #11225)
	PushEventHandler(new EventHandler(ms_numChildren));
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
}

MyFrame::~MyFrame()
{
    PopEventHandler(true);
	m_mgr.UnInit();
    ms_numChildren--;
	if( m_gameTraverseTimer != NULL )
	{
		m_gameTraverseTimer->Stop(); 
		delete m_gameTraverseTimer;// = new TraverseGameTimer(this);
		m_gameTraverseTimer = NULL;
	}

	
}


void MyFrame::DoRefresh()
{
	board_canvas->Refresh(false);
	my_canvas->Refresh();
	treeCanvas->Refresh();
	moveDataCanvas->Refresh();
	//noteFrame->Refresh(false);

}


void MyFrame::LogEvent(const wxString& name, wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	
	switch ( keycode )
	{
		case WXK_BACK:break;
        case WXK_TAB:break;
       /* case WXK_RETURN:
			{

				if( pgnLoad.move_counter== 0)
					break;

				if( move_count == 2)
				{
					my_canvas->selectedMove++;
	
							
				}

				if( move_count == 2)
				{
					move_count = 0;
							
				}
				if( (my_canvas->selectedMove > currentGame->move_counter)
					|| ctrl->chess_board.postReset)
				{
					ctrl->chess_board.mov = 0;
					currentGame->ResetMoveList();
					
					ctrl->chess_board.SetInitialPosition();
					move_count = 0;
					this->my_canvas->selectedMove = 0;
					this->my_canvas->white_black_move = 0;
					
					ctrl->Refresh(true);
					this->my_canvas->Refresh(true);
					
					return;
				}				

				//MyChessMove mv = 
				if( move_count == 0 )
				{
					my_canvas->white_black_move = move_count;
					//ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->white );
					ctrl->chess_board.parseMove( currentGame->GetNextMove()->white );

				}
				else
				{
					my_canvas->white_black_move = move_count;
				//	ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->black );
					ctrl->chess_board.parseMove( currentGame->GetNextMove()->black );	
				

				}
				
				

			=
				//GetClientDC(
				//wxClientDC dc(this);
				//this->m_mgr.Update();
				//ctrl->Refresh();
				
				move_count++;
				my_canvas->Refresh(false);
				ctrl->Refresh(true);
				//Refresh();
			//	ctrl->chess_board.
			}break;*/
        case WXK_ESCAPE:break;
        case WXK_SPACE:break;
        case WXK_DELETE:break;
        case WXK_START:break;
        case WXK_LBUTTON:break;
        case WXK_RBUTTON:break;
        case WXK_CANCEL:break;
        case WXK_MBUTTON:break;
        case WXK_CLEAR:break;
        case WXK_SHIFT:break;
        case WXK_ALT:break;
        case WXK_CONTROL:break;
        case WXK_MENU:break;
        case WXK_PAUSE:break;
        case WXK_CAPITAL:break;
        case WXK_END:break;
        case WXK_HOME:break;
        case WXK_LEFT:
			{
				my_canvas->LogEvent(name, event);
			}break;
        case WXK_UP:break;
        case WXK_RIGHT:
			{
				// RIGHTKEY
				//this->m_canvas
			/*	my_canvas->selectedMove++;
				if( my_canvas->selectedMove > pgnLoad.moves.size() )
					my_canvas->selectedMove= 0;

				my_canvas->Refresh(false);
				
				*/
				my_canvas->LogEvent(name, event);
				break;
			}
        case WXK_DOWN:break;
        case WXK_SELECT:break;
        case WXK_PRINT:break;
        case WXK_EXECUTE:break;
        case WXK_SNAPSHOT:break;
        case WXK_INSERT:break;
        case WXK_HELP:break;
        case WXK_NUMPAD0:break;
        case WXK_NUMPAD1:break;
        case WXK_NUMPAD2:break;
        case WXK_NUMPAD3:break;
        case WXK_NUMPAD4:break;
        case WXK_NUMPAD5:break;
        case WXK_NUMPAD6:break;
        case WXK_NUMPAD7:break;
        case WXK_NUMPAD8:break;
        case WXK_NUMPAD9:break;
        case WXK_MULTIPLY:break;
        case WXK_ADD:
			{
				wxCommandEvent tgmEvt;
				m_parent->ToggleMaximize(tgmEvt);
				//this->parent
			}break;
        case WXK_SEPARATOR:break;
        case WXK_SUBTRACT:break;
        case WXK_DECIMAL:break;
        case WXK_DIVIDE:break;
        case WXK_F1:break;
        case WXK_F2:break;
        case WXK_F3:break;
        case WXK_F4:break;
        case WXK_F5:break;
        case WXK_F6:break;
        case WXK_F7:break;
        case WXK_F8:break;
        case WXK_F9:break;
        case WXK_F10:break;
        case WXK_F11:break;
        case WXK_F12:break;
        case WXK_F13:break;
        case WXK_F14:break;
        case WXK_F15:break;
        case WXK_F16:break;
        case WXK_F17:break;
        case WXK_F18:break;
        case WXK_F19:break;
        case WXK_F20:break;
        case WXK_F21:break;
        case WXK_F22:break;
        case WXK_F23:break;
        case WXK_F24:break;
        case WXK_NUMLOCK:break;
        case WXK_SCROLL:break;
        case WXK_PAGEUP:break;
        case WXK_PAGEDOWN:break;
        case WXK_NUMPAD_SPACE:break;
        case WXK_NUMPAD_TAB:break;
        case WXK_NUMPAD_ENTER:break;
        case WXK_NUMPAD_F1:break;
        case WXK_NUMPAD_F2:break;
        case WXK_NUMPAD_F3:break;
        case WXK_NUMPAD_F4:break;
        case WXK_NUMPAD_HOME:break;
        case WXK_NUMPAD_LEFT:break;
        case WXK_NUMPAD_UP:break;
        case WXK_NUMPAD_RIGHT:break;
        case WXK_NUMPAD_DOWN:break;
        case WXK_NUMPAD_PAGEUP:break;
        case WXK_NUMPAD_PAGEDOWN:break;
        case WXK_NUMPAD_END:break;
        case WXK_NUMPAD_BEGIN:break;
        case WXK_NUMPAD_INSERT:break;
        case WXK_NUMPAD_DELETE:break;
        case WXK_NUMPAD_EQUAL:break;
        case WXK_NUMPAD_MULTIPLY:break;
        case WXK_NUMPAD_ADD:break;
        case WXK_NUMPAD_SEPARATOR:break;
        case WXK_NUMPAD_SUBTRACT:break;
        case WXK_NUMPAD_DECIMAL:break;
        case WXK_NUMPAD_DIVIDE:break;

    default: break;

    }
}

void MyFrame::OnLoadMedia(wxURI &uripath)
{
	
	if( uripath.IsReference() )
	{
            if( !m_mediactrl->Load(uripath.GetPath()) )
            {
                wxMessageBox(wxT("Couldn't load Media file!"));
                //m_playlist->SetItem(nNewId, 0, wxT("E"));
            }

	}
	else
	{
            if( !m_mediactrl->Load(uripath) )
            {
                wxMessageBox(wxT("Couldn't load Media URL!"));
               // currentpage->m_playlist->SetItem(nNewId, 0, wxT("E"));
            }

	}
}

void MyFrame::OnSetFocus(wxFocusEvent &evt)
{
//	this->Refresh(true);
}

void MyFrame::CreateVariationSelectionBox(int white_or_black)
{
	if( variationSelectionBox == NULL )
	{
		variationSelectionBox = new VariationSelectionFrame(this);
	}
	variationSelectionBox->Show(true);
	variationSelectionBox->m_lbox->Clear();


	if( white_or_black == 0 )
	{
		if( currentGame->GetCurrentMove()->pVarWhite != NULL )
		{
			variationSelectionBox->m_lbox->Append(currentGame->GetCurrentMove()->white );
			//arr.Add( currentGame->GetCurrentMove()->black );
			PGN::Move* pMove = currentGame->GetCurrentMove()->pVarWhite;
			variationSelectionBox->m_bListsWhiteMoves = true;
			int count = 0;
			while( pMove )
			{
				char buf[20];
				sprintf(buf, "%d) %s.%s",count+1 ,pMove->info.c_str(), pMove->white.c_str());
				variationSelectionBox->m_lbox->Append(buf);
				//arr.Add(buf);
				count++;
				pMove = pMove->pVarWhite;
			}
			//success = true;
			//m_bListsWhiteMoves
		}

		variationSelectionBox->m_lbox->SetSelection(0);
		variationSelectionBox->SetFocus();
		//variationSelectionBox->HandleC
		
			
	}
	else if( currentGame->GetCurrentMove()->pVarBlack != NULL )
	{
		//success = true;
		variationSelectionBox->m_lbox->Append(currentGame->GetCurrentMove()->black );
		//arr.Add( currentGame->GetCurrentMove()->black );
		PGN::Move* pMove = currentGame->GetCurrentMove()->pVarBlack;
		variationSelectionBox->m_bListsWhiteMoves = false;
		int count = 0;
			
		while( pMove )
		{
			char buf[20];
			sprintf(buf, "%d) %s", count, pMove->black.c_str());
			variationSelectionBox->m_lbox->Append(buf);
		//	arr.Add(buf);
			count++;
			pMove = pMove->pVarBlack;
		}

		variationSelectionBox->m_lbox->SetSelection(0);
		variationSelectionBox->SetFocus();
		
		
	}

	
	/*if( success )
	{
		//int ret = 
		//wxGetSingleChoiceIndex(wxString("Choose a Variation"),
		//wxString("Variation Dialog"),
		//arr,
		//this, -1,-1, true, 150, 200);
		wxSingleChoiceDialog	dlg(	this,
		wxString("Choose a Variation"),
		wxString("Variation Dialog"),
		arr);	
		int ret = dlg.ShowModal();
		this->SelectMove(ret);
	}*/
}

void MyFrame::SelectMoveFromTreeAutoTraversal(int sel)
{
	//return;
	PGN::Move* pMove = currentGame->GetCurrentMove();
	int count = 0;

	bool whiteMove;
	int mov = board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	if( sel != 0 )
	{
		if( !pMove->displayVariations )
		{
			pMove->SetDisplayVariations(true);
			my_canvas->needsRefresh = true;
		}
	}

	if( this->my_canvas->white_black_move == WHITE )
		pMove = pMove->pVarWhite;
	else pMove = pMove->pVarBlack;	

	
	while( pMove )
	{
		if( pMove->HasBeenVisited() == false )
		{
			pMove->Visit();
			currentGame->pCurrMove = pMove;
			if( whiteMove )
			{
				board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
				my_canvas->white_black_move = BLACK;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
				{
					wnd10->Clear();
					wxString inpt ="";
					for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
					{
						inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
						//inpt+= " ";
					}
					wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
				}
			}
			else
			{
				board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				my_canvas->white_black_move = WHITE;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
				{
					wnd10->Clear();
					wxString inpt ="";
					for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
					{
						inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
						//inpt += " ";
					}
					wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
				}
			}
			for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
			{
				if( my_canvas->activeEngines[j]->IsActive() &&
					my_canvas->activeEngines[j]->IsAnalysing())
				{
					my_canvas->activeEngines[j]->KibitzStartsMove();
					my_canvas->activeEngines[j]->KibitzUserEndsMove();
				}
			}

			my_canvas->ScrollToCursor();
			DoRefresh();
			//my_canvas->Refresh();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			//treeCanvas->Refresh();
			
			return;
		}
		count++;
		if( this->my_canvas->white_black_move == WHITE )
			pMove = pMove->pVarWhite;
		else pMove = pMove->pVarBlack;
	}

	

			if( whiteMove )
			{
			//	currentGame->NextMove();
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
				my_canvas->white_black_move = BLACK;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
				{
					wnd10->Clear();
					wxString inpt ="";
					for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
					{
						inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
						//inpt+= " ";
					}
					wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
				}
			}
			else
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
				my_canvas->white_black_move = WHITE;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
				{
					wnd10->Clear();
					wxString inpt ="";
					for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
					{
						inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
						//inpt += " ";
					}
					wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
				}
			}
			for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
			{
				if( my_canvas->activeEngines[j]->IsActive() &&
					my_canvas->activeEngines[j]->IsAnalysing())
				{
					my_canvas->activeEngines[j]->KibitzStartsMove();
					my_canvas->activeEngines[j]->KibitzUserEndsMove();
				}
			}
			my_canvas->ScrollToCursor();
			//my_canvas->Refresh();
			DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			//treeCanvas->Refresh();
			
			return;
		

}

void MyFrame::SelectMoveFromTree(int sel)
{
	PGN::Move* pMove = treeCanvas->lastMovePtr;
	bool whiteMove;
	int mov = board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	PGN::Move* pRoot = NULL;

	if( pMove != NULL )
	{
		pRoot = pMove->GetVariationRoot();
	}

	if( pRoot != NULL )
	{
		int count = 1;
		if( whiteMove )
		{
			PGN::Move * pMovePrev = pMove;
			while(pMove)
			{
				if( count-1 == sel && (pMove->white != "") )
				{
					currentGame->pCurrMove = pMove;

					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					my_canvas->white_black_move = BLACK;
									//m_owner->board_canvas->clearFlag = true;
					board_canvas->moveForward = true;
					board_canvas->renderMoveNow = true;
				//	board_canvas->Refresh(false);
					if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wnd10->Clear();
						wxString inpt ="";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
							//inpt+= " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}
					
					for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
					{
						if( my_canvas->activeEngines[j]->IsActive() &&
							my_canvas->activeEngines[j]->IsAnalysing())
						{
							my_canvas->activeEngines[j]->KibitzStartsMove();
							my_canvas->activeEngines[j]->KibitzUserEndsMove();
						}
					}

					my_canvas->ScrollToCursor();
				//	my_canvas->Refresh();
					DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
					my_canvas->SetFocus();
#endif
					//treeCanvas->Refresh();
			
					return;
				}
				pMove = pMove->pVarWhite;
				count++;
			}
			if( count-1 == sel )
			{
				

				int color = pMovePrev->GetVariationRootColor();

				if( color != 0 )
				{
					currentGame->pCurrMove = pRoot;
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					my_canvas->white_black_move = BLACK;
					board_canvas->clearFlag = true;
					//board_canvas->moveForward = true;

					//board_canvas->Refresh(false);
					if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wnd10->Clear();
						wxString inpt ="";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
							//inpt+= " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}
				}
				else
				{
					if( pRoot->pPrev )
						currentGame->pCurrMove = pRoot->pPrev;
					else
						currentGame->pCurrMove = pRoot;
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					my_canvas->white_black_move = WHITE;
					board_canvas->clearFlag = true;
					//board_canvas->moveForward = true;
					//board_canvas->Refresh(false);
					if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wxString inpt ="";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
							//inpt += " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}
				}
					
				for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
				{
					if( my_canvas->activeEngines[j]->IsActive() &&
						my_canvas->activeEngines[j]->IsAnalysing())
					{
						my_canvas->activeEngines[j]->KibitzStartsMove();
						my_canvas->activeEngines[j]->KibitzUserEndsMove();
					}
				}

				my_canvas->ScrollToCursor();
				//my_canvas->Refresh();
				DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
				my_canvas->SetFocus();
#endif
				//treeCanvas->Refresh();
			
				return;
				
			}
		}
		else
		{
			PGN::Move * pMovePrev = pMove;
			while(pMove)
			{
				if( count-1 == sel && (pMove->black != "") )
				{
					currentGame->pCurrMove = pMove;

					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					my_canvas->white_black_move = WHITE;
					//board_canvas->clearFlag = true;
					board_canvas->moveForward = true;
					board_canvas->renderMoveNow = true;
					//board_canvas->Refresh(false);
					if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wnd10->Clear();
						wxString inpt ="";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
							//inpt += " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}
			
					for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
					{
						if( my_canvas->activeEngines[j]->IsActive() &&
							my_canvas->activeEngines[j]->IsAnalysing())
						{
							my_canvas->activeEngines[j]->KibitzStartsMove();
							my_canvas->activeEngines[j]->KibitzUserEndsMove();
						}
					}
					

					my_canvas->ScrollToCursor();
					//my_canvas->Refresh();
					DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
					my_canvas->SetFocus();
#endif
					//treeCanvas->Refresh();
			
					return;
				}
				pMove = pMove->pVarBlack;
				count++;
			}
			if( count-1 == sel )
			{

				
				int color = pMovePrev->GetVariationRootColor();

				if( color != 0 )
				{
					currentGame->pCurrMove = pRoot;
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					my_canvas->white_black_move = BLACK;
					board_canvas->clearFlag = true;
					//board_canvas->moveForward = true;
					//board_canvas->Refresh(false);
					if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wnd10->Clear();
						wxString inpt ="";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
							//inpt+= " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}
				}
				else
				{
					if( pRoot->pPrev )
						currentGame->pCurrMove = pRoot->pPrev;
					else
						currentGame->pCurrMove = pRoot;

					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					my_canvas->white_black_move = WHITE;
					board_canvas->clearFlag = true;
					//board_canvas->moveForward = true;
					//board_canvas->Refresh(false);
					if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wnd10->Clear();
						wxString inpt ="";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
							//inpt += " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}
				}


			
				for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
				{
					if( my_canvas->activeEngines[j]->IsActive() &&
						my_canvas->activeEngines[j]->IsAnalysing())
					{
						my_canvas->activeEngines[j]->KibitzStartsMove();
						my_canvas->activeEngines[j]->KibitzUserEndsMove();
					}
				}

				my_canvas->ScrollToCursor();
				//my_canvas->Refresh();
				DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
				my_canvas->SetFocus();
#endif
				//treeCanvas->Refresh();
			
				return;				
			}
		}
	}
	else
	{
		SelectMove(sel);
	}

}

void MyFrame::SelectMove(int sel)
{
	PGN::Move* pMove = currentGame->GetCurrentMove();
	int count = 0;

	bool whiteMove;
	int mov = board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	if( sel != 0 )
	{
		if( !pMove->displayVariations )
		{
			pMove->SetDisplayVariations(true);
			my_canvas->needsRefresh = true;
		}
	}

	
	while( pMove )
	{
		if( count == sel )
		{
			currentGame->pCurrMove = pMove;
			if( whiteMove )
			{
				board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
				my_canvas->white_black_move = BLACK;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
				{
					wxString inpt ="";
					for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
					{
						inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
						//inpt+= " ";
					}
					wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
				}
			}
			else
			{
				board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				my_canvas->white_black_move = WHITE;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
				{
					wxString inpt ="";
					for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
					{
						inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
						//inpt += " ";
					}
					wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
				}
			}
			for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
			{
				if( my_canvas->activeEngines[j]->IsActive() &&
					my_canvas->activeEngines[j]->IsAnalysing())
				{
					my_canvas->activeEngines[j]->KibitzStartsMove();
					my_canvas->activeEngines[j]->KibitzUserEndsMove();
				}
			}
			my_canvas->ScrollToCursor();
			//my_canvas->Refresh();
			DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			//treeCanvas->Refresh();
			
			return;
		}
		count++;
		if( this->my_canvas->white_black_move == WHITE )
			pMove = pMove->pVarWhite;
		else pMove = pMove->pVarBlack;
	}
}



void MyFrame::EditPGNHeader()
{
	GameInfoModalDialog dialog(this, this, wxID_ANY, "GameInfo");
	dialog.ShowModal();
}

void MyFrame::InsertAnnotationData(string &str)
{
		if( (board_canvas->GetBoard()->mov != WHITE) && (board_canvas->GetBoard()->mov != 2) )
		{

			currentGame->pCurrMove->annotation_white.text.clear();
			currentGame->pCurrMove->annotation_white.text.push_back("{");
			//vector<string> tokens;
			string inpt = str;

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(currentGame->pCurrMove->annotation_white.text));
			currentGame->pCurrMove->annotation_white.text.push_back("}");
			vector<string>::iterator strit = currentGame->pCurrMove->annotation_white.text.begin();
			
			while( strit < currentGame->pCurrMove->annotation_white.text.end())
			{
				strit=currentGame->pCurrMove->annotation_white.text.insert(strit, " ");
				strit++;
				strit++;
			}
			my_canvas->Refresh();
			UpdateAnnotationWindow();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			
		}
		else
		{
			currentGame->pCurrMove->annotation_black.text.clear();
			currentGame->pCurrMove->annotation_black.text.push_back("{");
			//vector<string> tokens;
			string inpt = str;

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(currentGame->pCurrMove->annotation_black.text));
			currentGame->pCurrMove->annotation_black.text.push_back("}");

			vector<string>::iterator strit = currentGame->pCurrMove->annotation_black.text.begin();
			while( strit < currentGame->pCurrMove->annotation_black.text.end())
			{
				strit=currentGame->pCurrMove->annotation_black.text.insert(strit, " ");
				strit++;
				strit++;
			}

			my_canvas->Refresh();
			UpdateAnnotationWindow();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
		}
		
    
}


void MyFrame::EditAnnotation()
{
	wxString toSet = "";
    wxTextEntryDialog dialog(this,
                             "Annotation Editor",
                             wxT("Please enter a string"),
                             toSet,
                             wxOK | wxCANCEL);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
		//s = SetSubstringInQuotes(s, dialog.GetValue());
		//m_list->SetString(m_list->GetSelection(),s);
		wxString outpt = dialog.GetValue();
		if( (board_canvas->GetBoard()->mov != WHITE) && (board_canvas->GetBoard()->mov != 2) )
		{

			currentGame->pCurrMove->annotation_white.text.clear();
			currentGame->pCurrMove->annotation_white.text.push_back("{");
			//vector<string> tokens;
			string inpt = outpt.data().AsChar();

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(currentGame->pCurrMove->annotation_white.text));
			currentGame->pCurrMove->annotation_white.text.push_back("}");
			vector<string>::iterator strit = currentGame->pCurrMove->annotation_white.text.begin();
			
			while( strit < currentGame->pCurrMove->annotation_white.text.end())
			{
				strit=currentGame->pCurrMove->annotation_white.text.insert(strit, " ");
				strit++;
				strit++;
			}
			my_canvas->Refresh();
			UpdateAnnotationWindow();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			
		}
		else
		{
			currentGame->pCurrMove->annotation_black.text.clear();
			currentGame->pCurrMove->annotation_black.text.push_back("{");
			//vector<string> tokens;
			string inpt = outpt.data().AsChar();

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(currentGame->pCurrMove->annotation_black.text));
			currentGame->pCurrMove->annotation_black.text.push_back("}");

			vector<string>::iterator strit = currentGame->pCurrMove->annotation_black.text.begin();
			while( strit < currentGame->pCurrMove->annotation_black.text.end())
			{
				strit=currentGame->pCurrMove->annotation_black.text.insert(strit, " ");
				strit++;
				strit++;
			}

			my_canvas->Refresh();
			UpdateAnnotationWindow();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
		}
		
    }
}


void MyFrame::DeleteRemainingMoves()
{
	bool stopKibitz = false;
	for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
	{
		if( my_canvas->activeEngines[j]->IsActive() &&
			my_canvas->activeEngines[j]->IsAnalysing())
		{
			my_canvas->activeEngines[j]->KibitzStops();
			stopKibitz = true;
				
		}
	}
	//this->OnPOpen(event);
	if( currentGame->pCurrMove->IsThisFirstMoveInVariationWithoutNext(board_canvas->GetBoard()->mov))
	{
		if( stopKibitz )
		{
			wxMessageBox("This variation has no remaining moves you should use delete variation instead, Delete Variation has been called! All Kibitzing has been stopped.","User Error"); 
		}
		else
		{
			wxMessageBox("This variation has no remaining moves you should use delete variation instead, Delete Variation has been called!","User Error"); 

		}
			currentGame->DeleteVariation();
			
		board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
	}
	else
	{
		currentGame->pCurrMove = currentGame->pCurrMove->DeleteRemainingMoves(board_canvas->GetBoard()->mov);
	}
	my_canvas->needsRefresh = true;
	my_canvas->Refresh();
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
	treeCanvas->Refresh();
}





void MyFrame::DeleteVariation()
{
//	WriteClipboardData();
	bool stopKibitz = false;
	for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
	{
		if( my_canvas->activeEngines[j]->IsActive() &&
			my_canvas->activeEngines[j]->IsAnalysing())
		{
			my_canvas->activeEngines[j]->KibitzStops();
			stopKibitz = true;
		}
	}

	if( currentGame->pCurrMove->GetVariationRoot() == NULL )
	{
		if( stopKibitz )
		{
			wxMessageBox("This is not a variation, perhaps use Delete Remaining Moves Instead. All Kibitzing has been stopped.","User Error");

		}
		else
		{
			wxMessageBox("This is not a variation, perhaps use Delete Remaining Moves Instead","User Error");
		}
	}
	else
	{//if( currentGame->pCurrMove->I
	// please do not read my work ...lol
		currentGame->DeleteVariation();
		my_canvas->needsRefresh = true;
		board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
		my_canvas->white_black_move = 1;
		//my_canvas->ResetBoard();
	//	my_canvas->Refresh();
	//	board_canvas->Refresh(false);
	//	treeCanvas->Refresh();
		DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
		my_canvas->SetFocus();
#endif
	}
}




void MyFrame::PromoteVariation()
{
	bool stopKibitz = false;
	for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
	{
		if( my_canvas->activeEngines[j]->IsActive() &&
			my_canvas->activeEngines[j]->IsAnalysing())
		{
			my_canvas->activeEngines[j]->KibitzStops();
			stopKibitz = true;
				
		}
	}
	if( currentGame->pCurrMove->GetVariationRoot() == NULL )
	{
		if( stopKibitz )
		{
			wxMessageBox("This is not a variation, promotion aborted. All Kibitzing has been stopped.","User Error");
		}
		else
		{
			wxMessageBox("This is not a variation, promotion aborted","User Error");
		}
	}
	else
	{
		PGN::Move* pVarRoot = currentGame->pCurrMove->GetVariationRoot();
		currentGame->pCurrMove->SetVariationRenderColor(0,0,0);
		currentGame->pCurrMove->PromoteVariation(currentGame->pCurrMove);
		pVarRoot->SetVariationRenderColor(0,0,255);
		my_canvas->needsRefresh = true;
		my_canvas->Refresh();
#ifdef USE_CANVAS_SET_FOCUS
		my_canvas->SetFocus();
#endif
		treeCanvas->Refresh();
	}
}


void MyFrame::OnToolLeftClick(wxCommandEvent& event)
{
    wxString str;
    str.Printf( _T("Clicked on tool %d\n"), event.GetId());
   
	//MessageBox(0,"","",0);
	if( event.GetId() == ID_SampleItem+1)
	{

	}
    if (event.GetId() == ID_SampleItem+2)
    {
        //DoEnablePrint();
		//MessageBox(0,"","",0);
		//this->ctrl->flip_board = !this->ctrl->flip_board;
		//this->ctrl->ResetProjectionMode();
		//this->ctrl->Refresh();
	//	board_canvas->Flip();
	//	board_canvas->Refresh(false);
	}
	else if( event.GetId() == ID_SampleItem+3)
	{

	}
	else if( event.GetId() == ID_SampleItem+4)
	{

	}
	else if( event.GetId() == ID_SampleItem + 5 )
	{

	}

    if (event.GetId() == wxID_CUT)
    {
        //DoToggleHelp();
    }

    if (event.GetId() == wxID_PRINT)
    {
        //DoDeletePrint();
    }
}

void MyFrame::OnClose(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnRefresh(wxCommandEvent& WXUNUSED(event))
{
 //   if ( m_canvas )
  //      m_canvas->Refresh();
}

void MyFrame::OnChangePosition(wxCommandEvent& WXUNUSED(event))
{
    Move(10, 10);
}

void MyFrame::OnChangeSize(wxCommandEvent& WXUNUSED(event))
{
    SetClientSize(100, 100);
}

void MyFrame::OnChangeTitle(wxCommandEvent& WXUNUSED(event))
{
#if wxUSE_TEXTDLG
    static wxString s_title = "Canvas Frame";

    wxString title = wxGetTextFromUser("Enter the new title for MDI child",
                                       "MDI sample question",
                                       s_title,
                                       GetParent()->GetParent());
    if ( !title )
        return;

    s_title = title;
    SetTitle(s_title);
#endif // wxUSE_TEXTDLG
}

void MyFrame::OnActivate(wxActivateEvent& event)
{
   // if ( event.GetActive() && m_canvas )
   //     m_canvas->SetFocus();
}

void MyFrame::OnMove(wxMoveEvent& event)
{
    // VZ: here everything is totally wrong under MSW, the positions are
    //     different and both wrong (pos2 is off by 2 pixels for me which seems
    //     to be the width of the MDI canvas border)
    wxPoint pos1 = event.GetPosition(),
            pos2 = GetPosition();
    wxLogStatus("position from event: (%d, %d), from frame (%d, %d)",
                pos1.x, pos1.y, pos2.x, pos2.y);

    event.Skip();
}

/*void MyFrame::OnSize(wxSizeEvent& event)
{
    // VZ: under MSW the size event carries the client size (quite
    //     unexpectedly) *except* for the very first one which has the full
    //     size... what should it really be? TODO: check under wxGTK
    wxSize size1 = event.GetSize(),
           size2 = GetSize(),
           size3 = GetClientSize();
    wxLogStatus("size from event: %dx%d, from frame %dx%d, client %dx%d",
                size1.x, size1.y, size2.x, size2.y, size3.x, size3.y);

    event.Skip();
}*/

void MyFrame::OnCloseWindow(wxCloseEvent& event)
{
    //if ( m_canvas && m_canvas->IsDirty() )
    {
        if ( wxMessageBox("Really close?", "Please confirm",
                          wxICON_QUESTION | wxYES_NO) != wxYES )
        {
            event.Veto();

            return;
        }
    }

    event.Skip();
}

#if wxUSE_CLIPBOARD

#include "wx/clipbrd.h"

void MyFrame::OnPaste(wxCommandEvent& WXUNUSED(event))
{
    wxClipboardLocker lock;
    wxTextDataObject data;
  //  m_canvas->SetText(wxTheClipboard->GetData(data)
  //                      ? data.GetText()
    //                    : wxString("No text on clipboard"));
}

void MyFrame::OnUpdatePaste(wxUpdateUIEvent& event)
{
   // wxClipboardLocker lock;
   // event.Enable( wxTheClipboard->IsSupported(wxDF_TEXT) );
}

#endif // wxUSE_CLIPBOARD


wxAuiDockArt* MyFrame::GetDockArt()
{
    return m_mgr.GetArtProvider();
}

void MyFrame::DoUpdate()
{
	 m_listPanel->DoSize();
	// m_panel->Refresh(true);
	my_canvas->Refresh(false);
    m_mgr.Update();
}

void MyFrame::OnEraseBackground(wxEraseEvent& event)
{
	//my_canvas->Refresh(false);
    event.Skip();
}

void MyFrame::OnSize(wxSizeEvent& event)
{
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	//this->m_listCtrl->Refresh(true);
    m_listPanel->DoSize();
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	event.Skip();
}

void MyFrame::OnSettings(wxCommandEvent& WXUNUSED(evt))
{
    // show the settings pane, and float it
    wxAuiPaneInfo& floating_pane = m_mgr.GetPane(wxT("settings")).Float().Show();

    if (floating_pane.floating_pos == wxDefaultPosition)
        floating_pane.FloatingPosition(GetStartPosition());

    m_mgr.Update();
}

void MyFrame::OnCustomizeToolbar(wxCommandEvent& WXUNUSED(evt))
{
    wxMessageBox(_("Customize Toolbar clicked"));
}

void MyFrame::OnGradient(wxCommandEvent& event)
{
    int gradient = 0;

    switch (event.GetId())
    {
        case ID_NoGradient:         gradient = wxAUI_GRADIENT_NONE; break;
        case ID_VerticalGradient:   gradient = wxAUI_GRADIENT_VERTICAL; break;
        case ID_HorizontalGradient: gradient = wxAUI_GRADIENT_HORIZONTAL; break;
    }

    m_mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE, gradient);
    m_mgr.Update();
}

void MyFrame::OnManagerFlag(wxCommandEvent& event)
{
    unsigned int flag = 0;

#if !defined(__WXMSW__) && !defined(__WXMAC__) && !defined(__WXGTK__)
    if (event.GetId() == ID_TransparentDrag ||
        event.GetId() == ID_TransparentHint ||
        event.GetId() == ID_HintFade)
    {
        wxMessageBox(wxT("This option is presently only available on wxGTK, wxMSW and wxMac"));
        return;
    }
#endif

    int id = event.GetId();

    if (id == ID_TransparentHint ||
        id == ID_VenetianBlindsHint ||
        id == ID_RectangleHint ||
        id == ID_NoHint)
    {
        unsigned int flags = m_mgr.GetFlags();
        flags &= ~wxAUI_MGR_TRANSPARENT_HINT;
        flags &= ~wxAUI_MGR_VENETIAN_BLINDS_HINT;
        flags &= ~wxAUI_MGR_RECTANGLE_HINT;
        m_mgr.SetFlags(flags);
    }

    switch (id)
    {
        case ID_AllowFloating: flag = wxAUI_MGR_ALLOW_FLOATING; break;
        case ID_TransparentDrag: flag = wxAUI_MGR_TRANSPARENT_DRAG; break;
        case ID_HintFade: flag = wxAUI_MGR_HINT_FADE; break;
        case ID_NoVenetianFade: flag = wxAUI_MGR_NO_VENETIAN_BLINDS_FADE; break;
        case ID_AllowActivePane: flag = wxAUI_MGR_ALLOW_ACTIVE_PANE; break;
        case ID_TransparentHint: flag = wxAUI_MGR_TRANSPARENT_HINT; break;
        case ID_VenetianBlindsHint: flag = wxAUI_MGR_VENETIAN_BLINDS_HINT; break;
        case ID_RectangleHint: flag = wxAUI_MGR_RECTANGLE_HINT; break;
        case ID_LiveUpdate: flag = wxAUI_MGR_LIVE_RESIZE; break;
    }

    if (flag)
    {
        m_mgr.SetFlags(m_mgr.GetFlags() ^ flag);
    }

    m_mgr.Update();
}

void MyFrame::OnNotebookPageClosed(wxAuiNotebookEvent& evt)
{
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();

    // selection should always be a valid index
    wxASSERT_MSG( ctrl->GetSelection() < (int)ctrl->GetPageCount(),
                  wxString::Format("Invalid selection %d, only %d pages left",
                                   ctrl->GetSelection(),
                                   (int)ctrl->GetPageCount()) );

    evt.Skip();
}

void MyFrame::OnNotebookFlag(wxCommandEvent& event)
{
    int id = event.GetId();

    if (id == ID_NotebookNoCloseButton ||
        id == ID_NotebookCloseButton ||
        id == ID_NotebookCloseButtonAll ||
        id == ID_NotebookCloseButtonActive)
    {
        m_notebook_style &= ~(wxAUI_NB_CLOSE_BUTTON |
                              wxAUI_NB_CLOSE_ON_ACTIVE_TAB |
                              wxAUI_NB_CLOSE_ON_ALL_TABS);

        switch (id)
        {
            case ID_NotebookNoCloseButton: break;
            case ID_NotebookCloseButton: m_notebook_style |= wxAUI_NB_CLOSE_BUTTON; break;
            case ID_NotebookCloseButtonAll: m_notebook_style |= wxAUI_NB_CLOSE_ON_ALL_TABS; break;
            case ID_NotebookCloseButtonActive: m_notebook_style |= wxAUI_NB_CLOSE_ON_ACTIVE_TAB; break;
        }
    }

    if (id == ID_NotebookAllowTabMove)
    {
        m_notebook_style ^= wxAUI_NB_TAB_MOVE;
    }
    if (id == ID_NotebookAllowTabExternalMove)
    {
        m_notebook_style ^= wxAUI_NB_TAB_EXTERNAL_MOVE;
    }
     else if (id == ID_NotebookAllowTabSplit)
    {
        m_notebook_style ^= wxAUI_NB_TAB_SPLIT;
    }
     else if (id == ID_NotebookWindowList)
    {
        m_notebook_style ^= wxAUI_NB_WINDOWLIST_BUTTON;
    }
     else if (id == ID_NotebookScrollButtons)
    {
        m_notebook_style ^= wxAUI_NB_SCROLL_BUTTONS;
    }
     else if (id == ID_NotebookTabFixedWidth)
    {
        m_notebook_style ^= wxAUI_NB_TAB_FIXED_WIDTH;
    }


    size_t i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
    {
        wxAuiPaneInfo& pane = all_panes.Item(i);
        if (pane.window->IsKindOf(CLASSINFO(wxAuiNotebook)))
        {
            wxAuiNotebook* nb = (wxAuiNotebook*)pane.window;

            if (id == ID_NotebookArtGloss)
            {
                nb->SetArtProvider(new wxAuiDefaultTabArt);
                m_notebook_theme = 0;
            }
             else if (id == ID_NotebookArtSimple)
            {
                nb->SetArtProvider(new wxAuiSimpleTabArt);
                m_notebook_theme = 1;
            }


            nb->SetWindowStyleFlag(m_notebook_style);
            nb->Refresh();
        }
    }


}


void MyFrame::OnUpdateUI(wxUpdateUIEvent& event)
{
    unsigned int flags = m_mgr.GetFlags();
	
    switch (event.GetId())
    {
        case ID_NoGradient:
            event.Check(m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_GRADIENT_TYPE) == wxAUI_GRADIENT_NONE);
            break;
        case ID_VerticalGradient:
            event.Check(m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_GRADIENT_TYPE) == wxAUI_GRADIENT_VERTICAL);
            break;
        case ID_HorizontalGradient:
            event.Check(m_mgr.GetArtProvider()->GetMetric(wxAUI_DOCKART_GRADIENT_TYPE) == wxAUI_GRADIENT_HORIZONTAL);
            break;
        case ID_AllowFloating:
            event.Check((flags & wxAUI_MGR_ALLOW_FLOATING) != 0);
            break;
        case ID_TransparentDrag:
            event.Check((flags & wxAUI_MGR_TRANSPARENT_DRAG) != 0);
            break;
        case ID_TransparentHint:
            event.Check((flags & wxAUI_MGR_TRANSPARENT_HINT) != 0);
            break;
        case ID_LiveUpdate:
            event.Check((flags & wxAUI_MGR_LIVE_RESIZE) != 0);
            break;
        case ID_VenetianBlindsHint:
            event.Check((flags & wxAUI_MGR_VENETIAN_BLINDS_HINT) != 0);
            break;
        case ID_RectangleHint:
            event.Check((flags & wxAUI_MGR_RECTANGLE_HINT) != 0);
            break;
        case ID_NoHint:
            event.Check(((wxAUI_MGR_TRANSPARENT_HINT |
                          wxAUI_MGR_VENETIAN_BLINDS_HINT |
                          wxAUI_MGR_RECTANGLE_HINT) & flags) == 0);
            break;
        case ID_HintFade:
            event.Check((flags & wxAUI_MGR_HINT_FADE) != 0);
            break;
        case ID_NoVenetianFade:
            event.Check((flags & wxAUI_MGR_NO_VENETIAN_BLINDS_FADE) != 0);
            break;

        case ID_NotebookNoCloseButton:
            event.Check((m_notebook_style & (wxAUI_NB_CLOSE_BUTTON|wxAUI_NB_CLOSE_ON_ALL_TABS|wxAUI_NB_CLOSE_ON_ACTIVE_TAB)) != 0);
            break;
        case ID_NotebookCloseButton:
            event.Check((m_notebook_style & wxAUI_NB_CLOSE_BUTTON) != 0);
            break;
        case ID_NotebookCloseButtonAll:
            event.Check((m_notebook_style & wxAUI_NB_CLOSE_ON_ALL_TABS) != 0);
            break;
        case ID_NotebookCloseButtonActive:
            event.Check((m_notebook_style & wxAUI_NB_CLOSE_ON_ACTIVE_TAB) != 0);
            break;
        case ID_NotebookAllowTabSplit:
            event.Check((m_notebook_style & wxAUI_NB_TAB_SPLIT) != 0);
            break;
        case ID_NotebookAllowTabMove:
            event.Check((m_notebook_style & wxAUI_NB_TAB_MOVE) != 0);
            break;
        case ID_NotebookAllowTabExternalMove:
            event.Check((m_notebook_style & wxAUI_NB_TAB_EXTERNAL_MOVE) != 0);
            break;
        case ID_NotebookScrollButtons:
            event.Check((m_notebook_style & wxAUI_NB_SCROLL_BUTTONS) != 0);
            break;
        case ID_NotebookWindowList:
            event.Check((m_notebook_style & wxAUI_NB_WINDOWLIST_BUTTON) != 0);
            break;
        case ID_NotebookTabFixedWidth:
            event.Check((m_notebook_style & wxAUI_NB_TAB_FIXED_WIDTH) != 0);
            break;
        case ID_NotebookArtGloss:
            event.Check(m_notebook_style == 0);
            break;
        case ID_NotebookArtSimple:
            event.Check(m_notebook_style == 1);
            break;

    }
}

void MyFrame::OnPaneClose(wxAuiManagerEvent& evt)
{
    if (evt.pane->name == wxT("test10"))
    {
        int res = wxMessageBox(wxT("Are you sure you want to close/hide the Annotation pane?"),
                               wxT("Question"),
                               wxYES_NO,
                               this);
        if (res != wxYES)
            evt.Veto();
    }
	/*if (evt.pane->name == wxT("Engine_Panel"))
	{
		//this->m_pipePanel
		
		if( this->m_pipePanel[0]->IsActive() )
		{
			if( this->m_pipePanel[0]->IsAnalysing() )
			{
				m_pipePanel[0]->SendStop();
				m_pipePanel[0]->DoClose();
			}
		}
		m_mgr.GetPane("Engine_Panel").Hide();
		m_mgr.Update();
		evt.Veto();
	}
	else if (evt.pane->name == wxT("Engine_Panel_2"))
	{
		//this->m_pipePanel
		
		if( this->m_pipePanel[1]->IsActive() )
		{
			if( this->m_pipePanel[1]->IsAnalysing() )
			{
				m_pipePanel[1]->SendStop();
				m_pipePanel[1]->DoClose();
			}
		}
		m_mgr.GetPane("Engine_Panel_2").Hide();
		m_mgr.Update();
		evt.Veto();
	}*/
}

void MyFrame::OnCreatePerspective(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog dlg(this, wxT("Enter a name for the new perspective:"),
                          wxT("wxAUI Test"));

    dlg.SetValue(wxString::Format(wxT("Perspective %u"), unsigned(m_perspectives.GetCount() + 1)));
    if (dlg.ShowModal() != wxID_OK)
        return;

    if (m_perspectives.GetCount() == 0)
    {
        m_perspectives_menu->AppendSeparator();
    }

    m_perspectives_menu->Append(ID_FirstPerspective + m_perspectives.GetCount(), dlg.GetValue());
    m_perspectives.Add(m_mgr.SavePerspective());
}

void MyFrame::OnCopyPerspectiveCode(wxCommandEvent& WXUNUSED(evt))
{
    wxString s = m_mgr.SavePerspective();

#if wxUSE_CLIPBOARD
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(s));
        wxTheClipboard->Close();
    }
#endif
}

void MyFrame::OnRestorePerspective(wxCommandEvent& evt)
{
    m_mgr.LoadPerspective(m_perspectives.Item(evt.GetId() - ID_FirstPerspective));
}

void MyFrame::OnNotebookPageClose(wxAuiNotebookEvent& evt)
{
    wxAuiNotebook* ctrl = (wxAuiNotebook*)evt.GetEventObject();
    if (ctrl->GetPage(evt.GetSelection())->IsKindOf(CLASSINFO(wxHtmlWindow)))
    {
        int res = wxMessageBox(wxT("Are you sure you want to close/hide this notebook page?"),
                       wxT("wxAUI"),
                       wxYES_NO,
                       this);
        if (res != wxYES)
            evt.Veto();
    }
}

void MyFrame::OnAllowNotebookDnD(wxAuiNotebookEvent& evt)
{
    // for the purpose of this test application, explicitly
    // allow all noteboko drag and drop events
    evt.Allow();
}

wxPoint MyFrame::GetStartPosition()
{
    static int x = 0;
    x += 20;
    wxPoint pt = ClientToScreen(wxPoint(0,0));
    return wxPoint(pt.x + x, pt.y + x);
}


void MyFrame::OnCreateTree(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().
                  Caption(wxT("Tree Control")).
                  Float().FloatingPosition(GetStartPosition()).
                  FloatingSize(wxSize(150,300)));
    m_mgr.Update();
}

void MyFrame::OnCreatePipePanel(wxCommandEvent& evt)
{
	wxString name = "Engine_Panel";
	int num = numActivePipePanels + 1;
	char buff[10];
	sprintf(buff, "_%d", num);
	name += buff;
	m_pipePanel[numActivePipePanels] = new MyPipePanel(this);
    m_mgr.AddPane( m_pipePanel[numActivePipePanels], wxAuiPaneInfo().
                  Name(name).Caption(wxT("Engine")).
                  Bottom().MinSize(300,150).
                  CloseButton(true).MaximizeButton(true).DestroyOnClose(false));
	m_pipePanel[numActivePipePanels]->SetPanelName(name);

	numActivePipePanels++;
	m_mgr.Update();
}

void MyFrame::OnCreateGrid(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateGrid(), wxAuiPaneInfo().
                  Caption(wxT("Grid")).
                  Float().FloatingPosition(GetStartPosition()).
                  FloatingSize(wxSize(300,200)));
    m_mgr.Update();

	   wxString text;
    wxStringOutputStream strStream(& text);
	wxRichTextHTMLHandler htmlHandler(wxT("HTML"), wxT("htm"), 
 wxRICHTEXT_TYPE_HTML); 
   // wxRichTextHTMLHandler htmlHandler;
    //htmlHandler.SetFlags(wxRICHTEXT_HANDLER_SAVE_IMAGES_TO_MEMORY);

	htmlHandler.SetFlags(wxRICHTEXT_HANDLER_SAVE_IMAGES_TO_MEMORY);
    wxArrayInt fontSizeMapping;
    fontSizeMapping.Add(7);
    fontSizeMapping.Add(9);
    fontSizeMapping.Add(11);
    fontSizeMapping.Add(12);
    fontSizeMapping.Add(14);
    fontSizeMapping.Add(22);
    fontSizeMapping.Add(100);

    htmlHandler.SetFontSizeMapping(fontSizeMapping);

    if (htmlHandler.SaveFile(& m_richTextCtrl->GetBuffer(), strStream))
    {
        this->html_ctrl->SetPage(text);
    }

    //boxSizer->Fit(& dialog);

   // dialog.ShowModal();

	
	//html_ctrl->Select(wxPoint(0,0));

    // Now delete the temporary in-memory images
    htmlHandler.DeleteTemporaryImages();


	//->html_ctrl->SetPage("<H>Hello World</H>");
	//this->html_ctrl->Connect(
	//this->html_ctrl->LoadPage("www.chess.com");
	//wxPoint p(0,0);
	//this->html_ctrl->Insert
}

void MyFrame::OnCreateHTML(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateHTMLCtrl(), wxAuiPaneInfo().
                  Caption(wxT("HTML Control")).
                  Float().FloatingPosition(GetStartPosition()).
                  FloatingSize(wxSize(300,200)));
    m_mgr.Update();
}

void MyFrame::OnPOpen(wxCommandEvent& WXUNUSED(event))
{
    wxString cmd = wxGetTextFromUser(wxT("Enter the command to launch: "),
                                     wxT("Engine Loader"));
    if ( cmd.empty() )
        return;

    wxProcess *process = wxProcess::Open(cmd);
	 //long pid = wxExecute(cmd, 1, process);
	 //process->Redirect();
	//MyPipedProcess2 *process = new MyPipedProcess2(this, cmd, input);
    if ( !process )
    {
        wxLogError(wxT("Failed to launch the command."));
        return;
    }

    wxLogVerbose(wxT("PID of the new process: %ld"), process->GetPid());

    wxOutputStream *out = process->GetOutputStream();
    if ( !out )
    {
        wxLogError(wxT("Failed to connect to child stdin"));
        return;
    }
	//out->

    wxInputStream *in = process->GetInputStream();
    if ( !in )
    {
        wxLogError(wxT("Failed to connect to child stdout"));
        return;
    }

	//in->

	if( m_pipePanel[0]->EngineIsLoaded() )
	{
		this->m_pipePanel[1]->LoadEngine(cmd,process);

		board_canvas->AddActiveEngine(m_pipePanel[1]);
		my_canvas->AddActiveEngine(m_pipePanel[1]);
#ifdef USE_CANVAS_SET_FOCUS
		my_canvas->SetFocus();
#endif
	}
	else
	{
		this->m_pipePanel[0]->LoadEngine(cmd,process);

		board_canvas->AddActiveEngine(m_pipePanel[0]);
		my_canvas->AddActiveEngine(m_pipePanel[0]);
#ifdef USE_CANVAS_SET_FOCUS
		my_canvas->SetFocus();
#endif
	}
}

void MyFrame::OnInsertAnnotation(wxCommandEvent &evt)
{

}

void MyFrame::NewGame(bool removeList)
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
		//	board_canvas->activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzStops();
		//	activeEngines[j]->KibitzUserNewGame();
			
		}
	}	
	my_canvas->ResetBoard();
	

	//board_canvas->ResetBoard();
	if( removeList )
	{
		currentGame = pgnLoad.NewGame();
	}
	else
	{
		currentGame = pgnLoad.AddGame();
	}

	my_canvas->move_count = 0;
//	treeCanvas->Refresh();
	board_canvas->ResetBoard();
	//board_canvas->whoseMove == BoardCanvas::YOUR_MOVE;

	DoRefresh();

	if( gameMode == 1 )
	{
		gameClock->ResetTime();
		gameClock->PauseGame();
	}
	
	RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);


	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())//&&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
	
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
}


void MyFrame::OnNewGame(wxCommandEvent& evt)
{
	NewGameDialog ng(this, wxID_ANY);// = new NewGameDialog(this);
	int res = ng.ShowModal();
	if( ng.retCode == NewGameDialog::ADD_GAME_TO_LIST )
	{
		NewGame(false);
	}
	else if( ng.retCode == NewGameDialog::ADD_GAME_CLEAR_LIST )
	{
		NewGame(true);
	}
}

void MyFrame::SetCurrentGame(int idx)
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
			
			
		}
	}

	if( idx >= pgnLoad.games.size() )
	{
		wxMessageBox(wxString("Error, List element out of bounds"));
	}
	for( int i = 0; i < pgnLoad.games.size(); i++ )
	{
		if( i == idx )
		{
			currentGame = pgnLoad.games[i];
			currentGame->pCurrMove = currentGame->pRoot;
			if( !currentGame->HasGeneratedFens() )
			{
				//currentGame->ResetMoveList();
				currentGame->Parse();
				//currentGame->FixMoves();
				currentGame->ParseHeader();
			}

			my_canvas->ResetBoard();
			board_canvas->ResetBoard();
			if( currentGame->pCurrMove->white == "" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
			}
			else if( currentGame->pCurrMove->info != "1" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
			}

			if( !currentGame->HasGeneratedFens() )
			{
				PGN::Move* pMove = currentGame->pCurrMove;
				while(pMove)
				{
					board_canvas->GetBoard()->parseMove(pMove->white);
					pMove->SetFenWhite(board_canvas->GetBoard()->Fen());
					pMove->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
					board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
						
					board_canvas->GetBoard()->parseMove(pMove->black);
					pMove->SetFenBlack(board_canvas->GetBoard()->Fen());
					pMove->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
					board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);

					pMove->PrepareAnnotations();
					CreateFenForVariation(pMove);
					board_canvas->GetBoard()->SetBoard( pMove->m_fenBlack);
					pMove = pMove->pNext;
				}
				currentGame->SetHasGeneratedFens();
			}

			currentGame->ResetMoveList();
			//currentGame->EnumerateVariations();
			board_canvas->ResetBoard();

			my_canvas->move_count = 0;
			my_canvas->Scroll(0,0);
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			break;
		}
	}

	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			//activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();		
		}
	}
}

void MyFrame::OnCopyVariationToClipBoard(wxCommandEvent& evt)
{
	string out = currentGame->pCurrMove->GetCurrentVariationAsString();

	if (wxTheClipboard->Open())
	{
	//	if (wxTheClipboard->IsSupported( wxDF_UNICODETEXT ))wxDF_TEXT
		{

			wxTheClipboard->SetData( new wxTextDataObject(out));
		}
		wxTheClipboard->Close();	
	}
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
}


void MyFrame::OnPasteFEN(wxCommandEvent& evt)
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}


	if (wxTheClipboard->Open())
	{
		//if (wxTheClipboard->IsSupported( wxDF_UNICODETEXT ))wxDF_TEXT
		{
			//data.
			wxTheClipboard->GetData( data );
		}
		wxTheClipboard->Close();
	}

	if( data.GetTextLength() == 0 )
	{
		wxMessageBox("No FEN in clipboard", "Error");
#ifdef USE_CANVAS_SET_FOCUS
		my_canvas->SetFocus();
#endif
		return;
	}

	my_canvas->ResetBoard();
	

	//board_canvas->ResetBoard();
	currentGame = pgnLoad.NewGame();
	my_canvas->move_count = 0;
	//treeCanvas->Refresh();
	
	RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);


	string fen = data.GetText().c_str();
	board_canvas->GetBoard()->SetBoard(fen);
	//board_canvas->Refresh(false);
	DoRefresh();
	currentGame->pBase->SetFenWhite(fen);
	currentGame->pBase->SetFenBlack(fen);
	if( fen != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
	{
		currentGame->gameBeginsAtStartPos = false;
	}

	vector<string> tokens;

	istringstream iss(fen);
		copy(istream_iterator<string>(iss),
			istream_iterator<string>(),
				back_inserter<vector<string> >(tokens));

	if( tokens[5] != "0" )
	{
		currentGame->pRoot->info = tokens[5];
	}
	else
	{
		char buf[3];
		itoa(board_canvas->GetBoard()->mov, buf, 10);
		currentGame->pRoot->info = buf;
	}

	// now add this to the header
	PGN::HeaderElem hdr;
	hdr.data = "";
	hdr.data += "[FEN \"";
	hdr.data += fen;
	hdr.data += "\"]";
	currentGame->vec_header.push_back(hdr);

	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();	
		}
	}
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
}


void MyFrame::SaveGameToOutputStream(PGN::Game *gameToSave)
{
}

void MyFrame::OnSaveGameListAsDatabase(wxCommandEvent& evt)
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}

	ostringstream oss;
	for( unsigned int i = 0; i < pgnLoad.games.size(); i++ )
	{
		pgnLoad.games[i]->GameToBuffer(oss);
		oss << endl << endl;
	}

    wxFileDialog dialog(this,
                        wxT("Save PGN Database"),
                        wxEmptyString,
                        wxT("filename"),
                        wxT("PGN files (*.pgn)|*.pgn|Document files (*.doc;*.ods)|*.doc;*.ods"),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(0);

    if (dialog.ShowModal() == wxID_OK)
    {
      //  wxLogMessage(wxT("%s, filter %d"),
      //               dialog.GetPath().c_str(), dialog.GetFilterIndex());
		//wxString strOutput = dialog.GetPath();// + dialog.GetFilename();
		//currentGame->SaveGame((char*)strOutput.c_str().AsChar());
		ofstream ofs;
		ofs.open(dialog.GetPath().data().AsChar());
		ofs << oss.str();
		
		//ofs.write
		ofs.close();
    }

#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
}

void MyFrame::OnSavePGN(wxCommandEvent& WXUNUSED(evt))
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

    wxFileDialog dialog(this,
                        wxT("Testing save file dialog"),
                        wxEmptyString,
                        wxT("filename"),
                        wxT("PGN files (*.pgn)|*.pgn|Document files (*.doc;*.ods)|*.doc;*.ods"),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(0);

    if (dialog.ShowModal() == wxID_OK)
    {
      //  wxLogMessage(wxT("%s, filter %d"),
      //               dialog.GetPath().c_str(), dialog.GetFilterIndex());
		wxString strOutput = dialog.GetPath();// + dialog.GetFilename();
		currentGame->SaveGame((char*)strOutput.c_str().AsChar());
    }
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
	
}

void MyFrame::OnMergePGNFromClipBoard(wxCommandEvent& WXUNUSED(evt))
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}	

	if (wxTheClipboard->Open())
	{
		//if (wxTheClipboard->IsSupported( wxDF_UNICODETEXT ))wxDF_TEXT
		{
				
			//data.
			wxTheClipboard->GetData( data );
			//m_textctrl->Clear();
			//m_textctrl->SetValue( data.GetText() );
			wnd10->Clear();
			wnd10->SetValue( data.GetText().wc_str());
		}
		wxTheClipboard->Close();
	}

//	pgnLoad.Reset();
	//	pgnLoad.LoadPgn(str);
	PGN::Game* newGame = pgnLoad.LoadPgnFromString(data.GetText().c_str().AsChar());
		
	newGame->Parse();

	newGame->ParseHeader();
	//pgnLoad.ParseGames();
		//pgnLoad.ParsePgn();
	//currentGame = pgnLoad.games[0];
	//currentGame->FixMoves();
	//currentGame->ParseHeader();

	try
	{
			currentGame->MergeGame(newGame);
	}
	catch(...)
	{
		wxMessageBox(wxT("Error Merging PGN"),wxT("Error"));
	}


	delete pgnLoad.games[pgnLoad.games.size()-1];
	pgnLoad.games.erase(pgnLoad.games.begin() + pgnLoad.games.size()-1);


	this->my_canvas->selectedMove = 0;
	currentGame->ResetMoveList();		
	//ctrl->chess_board.SetInitialPosition();
	//ctrl->Refresh(true);
	board_canvas->ResetBoard();
	if( currentGame->pCurrMove->white == "" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = BLACK;
	}
	else if( currentGame->pCurrMove->info != "1" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = WHITE;
	}
	//board_canvas->Refresh(true);
	my_canvas->move_count = 0;
	//this->my_canvas->Refresh(true);
	//DoRefresh();
	my_canvas->needsRefresh = true;

//	currentGame->ResetMoveList();

	PGN::Move* pMove = currentGame->pCurrMove;
	while(pMove)
	{
		if( pMove->white != "" )
		{
			board_canvas->GetBoard()->parseMove(pMove->white);
			pMove->SetFenWhite(board_canvas->GetBoard()->Fen());
			pMove->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
					board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
					
		}
		else board_canvas->GetBoard()->mov = 1;
		
		board_canvas->GetBoard()->parseMove(pMove->black);
		pMove->SetFenBlack(board_canvas->GetBoard()->Fen());
		pMove->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
		board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				
		//currentGame->NextMove();
		pMove->PrepareAnnotations();
		CreateFenForVariation(pMove);
		board_canvas->GetBoard()->SetBoard( pMove->getFenBlack());
		pMove = pMove->pNext;
	}

	currentGame->SetHasGeneratedFens();

	currentGame->ResetMoveList();
	//currentGame->EnumerateVariations();
	board_canvas->ResetBoard();
	if( currentGame->pCurrMove->white == "" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = BLACK;
	}
	else if( currentGame->pCurrMove->info != "1" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = WHITE;
	}
	my_canvas->move_count = 0;
	my_canvas->ResetBoard();

	DoRefresh();

	RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
#ifdef USE_CANVAS_SET_FOCUS        
    my_canvas->SetFocus();
#endif
}




void MyFrame::OnMergePGNFromDisk(wxCommandEvent& WXUNUSED(evt))
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}

    wxString path;
    wxString filename;
    wxArrayInt fileTypes;
	my_canvas->needsRefresh = true;

  //  wxString filter = wxRichTextBuffer::GetExtWildcard(false, false, & fileTypes);
    //if (!filter.empty())
    //    filter += wxT("|");
   // wxString filter = wxT("*.pgn");
	//filter += wxT("|");
	wxString filter= wxT("PGN files (*.pgn)|*.pgn|Document files (*.doc;*.ods)|*.doc;*.ods");

    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_OPEN);

	dialog.SetFilterIndex(0);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			//wxString foo( wxT("Hello world") );
			char buf[200];
			strcpy( buf, (const char*)path.mb_str(wxConvUTF8) );
			string str = buf;

			ifstream myfile (str);
			std::ostringstream out;
			string fileBuffer;
			if (myfile.is_open())
			{


				out << myfile.rdbuf();

				fileBuffer = out.str();
			}
			else 
			{
					wxMessageBox("Cannot Read File", "Error");
					return;
			}
//	pgnLoad.Reset();
		//	myfile.
			//	pgnLoad.LoadPgn(str);
			PGN::Game* newGame = pgnLoad.LoadPgnFromString(fileBuffer.c_str());
		
			newGame->Parse();

			newGame->ParseHeader();
			//pgnLoad.ParseGames();
				//pgnLoad.ParsePgn();
			//currentGame = pgnLoad.games[0];
			//currentGame->FixMoves();
			//currentGame->ParseHeader();

			try
			{
					currentGame->MergeGame(newGame);
			}
			catch(...)
			{
				wxMessageBox(wxT("Error Merging PGN"),wxT("Error"));
			}


			delete pgnLoad.games[pgnLoad.games.size()-1];
			pgnLoad.games.erase(pgnLoad.games.begin() + pgnLoad.games.size()-1);


			this->my_canvas->selectedMove = 0;
			currentGame->ResetMoveList();		
			//ctrl->chess_board.SetInitialPosition();
			//ctrl->Refresh(true);
			board_canvas->ResetBoard();
			if( currentGame->pCurrMove->white == "" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = BLACK;
			}
			else if( currentGame->pCurrMove->info != "1" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = WHITE;
			}
			//board_canvas->Refresh(true);
			my_canvas->move_count = 0;
			//this->my_canvas->Refresh(true);
			my_canvas->needsRefresh = true;

		//	currentGame->ResetMoveList();

			PGN::Move* pMove = currentGame->pCurrMove;
			while(pMove)
			{
				if( pMove->white != "" )
				{
					board_canvas->GetBoard()->parseMove(pMove->white);
					pMove->SetFenWhite(board_canvas->GetBoard()->Fen());
					pMove->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
							board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
					
				}
				else board_canvas->GetBoard()->mov = 1;
		
				board_canvas->GetBoard()->parseMove(pMove->black);
				pMove->SetFenBlack(board_canvas->GetBoard()->Fen());
				pMove->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
				board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				
				//currentGame->NextMove();
				pMove->PrepareAnnotations();
				CreateFenForVariation(pMove);
				board_canvas->GetBoard()->SetBoard( pMove->getFenBlack());
				pMove = pMove->pNext;
			}

			currentGame->SetHasGeneratedFens();

			currentGame->ResetMoveList();
			//currentGame->EnumerateVariations();
			board_canvas->ResetBoard();
			if( currentGame->pCurrMove->white == "" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = BLACK;
			}
			else if( currentGame->pCurrMove->info != "1" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = WHITE;
			}
			my_canvas->move_count = 0;
			my_canvas->ResetBoard();
			DoRefresh();
			RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
					//ctrl->chess_board.mov = 0;
					//renderTimer->Start();
#ifdef USE_CANVAS_SET_FOCUS        
		my_canvas->SetFocus();
#endif
		}
	}

	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}

void MyFrame::OnCopyPGNToClipBoard(wxCommandEvent& WXUNUSED(evt))
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}
	ostringstream oss;
	currentGame->GameToBuffer(oss);

	if (wxTheClipboard->Open())
	{
		
			if (wxTheClipboard->IsSupported( wxDataFormatId::wxDF_TEXT ))
			{

				wxTheClipboard->SetData( new wxTextDataObject(wxString(oss.str().c_str())));
			}
			wxTheClipboard->Close();
		
	}
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif

}

void MyFrame::CopyFenToClipBoard(wxCommandEvent& evt)
{
	if (wxTheClipboard->Open())
	{
	//	if (wxTheClipboard->IsSupported( wxDF_UNICODETEXT ))wxDF_TEXT
		{
			if( board_canvas->GetBoard()->mov == WHITE || board_canvas->GetBoard()->mov==2 )
				wxTheClipboard->SetData( new wxTextDataObject(currentGame->pCurrMove->m_fenBlack));
			else
				wxTheClipboard->SetData( new wxTextDataObject(currentGame->pCurrMove->m_fenWhite));

		}
		wxTheClipboard->Close();	
	}
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
}


void MyFrame::OnLoadPgnFromClipBoard(wxCommandEvent& WXUNUSED(evt))
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}
	if (wxTheClipboard->Open())
	{
		//if (wxTheClipboard->IsSupported( wxDF_UNICODETEXT ))//wxDF_TEXT
		{
				
			//data.
			wxTheClipboard->GetData( data );
			//m_textctrl->Clear();
			//m_textctrl->SetValue( data.GetText() );
			//wnd10->Clear();
			//wnd10->SetValue( data.GetText().wc_str());
		}
		wxTheClipboard->Close();
	}

	pgnLoad.Reset();
	//	pgnLoad.LoadPgn(str);
	currentGame = pgnLoad.LoadPgnFromString(data.GetText().c_str().AsChar());
		//pgnLoad.ParseGames();
		//pgnLoad.ParsePgn();
	//currentGame = pgnLoad.games[0];
	currentGame->Parse();
	//currentGame->FixMoves();
	currentGame->ParseHeader();

	this->my_canvas->selectedMove = 0;
			
	//ctrl->chess_board.SetInitialPosition();
	//ctrl->Refresh(true);
	board_canvas->ResetBoard();
	if( currentGame->pCurrMove->white == "" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = BLACK;
	}
	else if( currentGame->pCurrMove->info != "1" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = WHITE;
	}
	//board_canvas->Refresh(true);
	my_canvas->move_count = 0;
	//this->my_canvas->Refresh(true);

	PGN::Move* pMove = currentGame->pCurrMove;
	while(pMove)
	{
		if( pMove->white != "" )
		{
			board_canvas->GetBoard()->parseMove(pMove->white);
			pMove->SetFenWhite(board_canvas->GetBoard()->Fen());
			pMove->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
			board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
		}
		//else board_canvas->GetBoard()->mov = 1;

		board_canvas->GetBoard()->parseMove(pMove->black);
		pMove->SetFenBlack(board_canvas->GetBoard()->Fen());
		pMove->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
		board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				
		//currentGame->NextMove();
		pMove->PrepareAnnotations();
		if( pMove->pVarBlack || pMove->pVarWhite )
		{
			CreateFenForVariation(pMove);
			board_canvas->GetBoard()->SetBoard( pMove->getFenBlack());
		}
		pMove = pMove->pNext;
	}

	currentGame->SetHasGeneratedFens();

	currentGame->ResetMoveList();
	//currentGame->EnumerateVariations();
	board_canvas->ResetBoard();
	if( currentGame->pCurrMove->white == "" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = BLACK;
	}
	else if( currentGame->pCurrMove->info != "1" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = WHITE;
	}
	my_canvas->move_count = 0;
	my_canvas->Scroll(0,0);
	my_canvas->needsRefresh = true;
	DoRefresh();

#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
	RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);

	m_dataView->AppendGameList(&pgnLoad);
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
        
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}    

}


void MyFrame::LoadPGN(bool refreshList)
{
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStops();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}

    wxString path;
    wxString filename;
    wxArrayInt fileTypes;
	my_canvas->needsRefresh = true;

  //  wxString filter = wxRichTextBuffer::GetExtWildcard(false, false, & fileTypes);
    //if (!filter.empty())
    //    filter += wxT("|");
   // wxString filter = wxT("*.pgn");
	//filter += wxT("|");
	wxString filter= wxT("PGN files (*.pgn)|*.pgn|Document files (*.doc;*.ods)|*.doc;*.ods");

    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_OPEN);

	dialog.SetFilterIndex(0);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			//wxString foo( wxT("Hello world") );
			char buf[200];
			strcpy( buf, (const char*)path.mb_str(wxConvUTF8) );
			string str = buf;
			//string str = path.
            //int filterIndex = dialog.GetFilterIndex();
            //int fileType = (filterIndex < (int) fileTypes.GetCount())
            //               ? fileTypes[filterIndex]
            //               : wxRICHTEXT_TYPE_TEXT;
            //m_richTextCtrl->LoadFile(path, fileType);
			if( refreshList )
				pgnLoad.Reset();
			pgnLoad.LoadPgn(str);
			//pgnLoad.ParseGames();
			//pgnLoad.ParsePgn();
			if( refreshList )
				currentGame = pgnLoad.games[0];
			else 
				currentGame = pgnLoad.games[pgnLoad.games.size()-1];
			
			currentGame->Parse();
			currentGame->FixMoves();
			currentGame->ParseHeader();

			this->my_canvas->selectedMove = 0;
			
			//ctrl->chess_board.SetInitialPosition();
			//ctrl->Refresh(true);
			board_canvas->ResetBoard();
			if( currentGame->pCurrMove->white == "" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = BLACK;
			}
			else if( currentGame->pCurrMove->info != "1" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = WHITE;
			}
			//board_canvas->Refresh(true);
			my_canvas->move_count = 0;
			//this->my_canvas->Refresh(true);

			PGN::Move* pMove = currentGame->pCurrMove;
			while(pMove)
			{
				if( pMove->white != "" )
				{
					board_canvas->GetBoard()->parseMove(pMove->white);
					pMove->SetFenWhite(board_canvas->GetBoard()->Fen());
					pMove->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
					board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
				}
				else board_canvas->GetBoard()->mov = 1;

				board_canvas->GetBoard()->parseMove(pMove->black);
				pMove->SetFenBlack(board_canvas->GetBoard()->Fen());
				pMove->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
				board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				
				//currentGame->NextMove();
				pMove->PrepareAnnotations();
				CreateFenForVariation(pMove);
				board_canvas->GetBoard()->SetBoard( pMove->getFenBlack());
				pMove = pMove->pNext;
			}

			currentGame->SetHasGeneratedFens();

			currentGame->ResetMoveList();
			//currentGame->EnumerateVariations();
			board_canvas->ResetBoard();
			if( currentGame->pCurrMove->white == "" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = BLACK;
			}
			else if( currentGame->pCurrMove->info != "1" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
				//board_canvas->Refresh(false);
				my_canvas->white_black_move = WHITE;
			}
			my_canvas->move_count = 0;

			RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
			m_dataView->AppendGameList(&pgnLoad);
			my_canvas->Scroll(0,0);
			my_canvas->needsRefresh = true;
			DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
        }
    }
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}

void MyFrame::OnAddPGN(wxCommandEvent& evt)
{
	LoadPGN(false);
}

void MyFrame::OnLoadPGN(wxCommandEvent& WXUNUSED(event))
{
	LoadPGN(true);

}


void MyFrame::RecreateList(long flags, bool withText)
{
	m_listPanel->RecreateList(flags, withText);
}


void MyFrame::CreateFenForVariationWhite(PGN::Move* pMove, string fen_last)
{
	if( pMove->pVarWhite != 0 /*&& pMove->pVarWhite->white != ""*/)
	{
		PGN::Move* temp = pMove->pVarWhite;
		
		board_canvas->GetBoard()->SetBoard(fen_last);
		while( temp )
		{
			if( temp->pUp == NULL )
				board_canvas->GetBoard()->SetBoard(temp->pPrev->getFenBlack());

			board_canvas->GetBoard()->parseMove(temp->white);
			temp->SetFenWhite(board_canvas->GetBoard()->Fen());
			temp->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
			board_canvas->GetBoard()->SetBoard(temp->m_fenWhite);

			board_canvas->GetBoard()->parseMove(temp->black);
			temp->SetFenBlack(board_canvas->GetBoard()->Fen());
			temp->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
			board_canvas->GetBoard()->SetBoard(temp->m_fenBlack);
				
				//currentGame->NextMove();
			temp->PrepareAnnotations();
			if( temp->pVarWhite != 0 )
			{
				// find uppermost move
				if( temp->pUp != NULL )
					CreateFenForVariationWhite(temp, fen_last);
				else
					CreateFenForVariationWhite(temp, temp->pPrev->m_fenBlack);
			}
			if( temp->pVarBlack != 0 )
			{
				CreateFenForVariationBlack(temp, temp->m_fenWhite);
			}
		//	board_canvas->GetBoard()->SetBoard( temp->getFenBlack());
			temp = temp->pNext;
		}

	}
}
	
void MyFrame::CreateFenForVariationBlack(PGN::Move* pMove, string fen_last)
{
	if( pMove->pVarBlack != 0 /*&& pMove->pVarBlack->black != ""*/)
	{
		PGN::Move* temp = pMove->pVarBlack;
		board_canvas->GetBoard()->SetBoard(fen_last);
		bool first_move = true;
		while( temp )
		{
			if( temp->pUp == NULL )
				board_canvas->GetBoard()->SetBoard(temp->pPrev->getFenBlack());
			
			if( !first_move )
			{
				board_canvas->GetBoard()->parseMove(temp->white);
				temp->SetFenWhite(board_canvas->GetBoard()->Fen());
				temp->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
				board_canvas->GetBoard()->SetBoard(temp->m_fenWhite);
			}

			if( first_move )
			{
			//	board_canvas->GetBoard()->mov = 1;
				first_move = false;
			}

			
			board_canvas->GetBoard()->parseMove(temp->black);
			temp->SetFenBlack(board_canvas->GetBoard()->Fen());
			temp->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
			board_canvas->GetBoard()->SetBoard(temp->m_fenBlack);
				
				//currentGame->NextMove();
			temp->PrepareAnnotations();
			if( temp->pVarWhite != 0 )
			{
				if( temp->pUp == pMove )
				{}//	CreateFenForVariationWhite(temp, fen_last);
				else
				{
					CreateFenForVariationWhite(temp, temp->pPrev->m_fenBlack);
				}
			}
			if( temp->pVarBlack != 0 )
			{
				if( temp->pUp != NULL )
				{
					CreateFenForVariationBlack(temp, fen_last);
				}
				else
				{
					CreateFenForVariationBlack(temp, temp->m_fenWhite);
				}
			}
			board_canvas->GetBoard()->SetBoard( temp->getFenBlack());
			temp = temp->pNext;
		}
	}
}

void MyFrame::UpdateAnnotationWindow()
{
	wnd10->Clear();
	if( board_canvas->GetBoard()->mov == BLACK )
	{
		if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
		{
			wxString inpt = "";
			for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
			{
				inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
			//	inpt += " ";
			//	m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str()));
			}
			wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
		}
	}
	else
	{
		if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
		{
			wxString inpt = "";
			for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
			{
				inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
			//	inpt += " ";
			//	m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str()));
			}
			wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
		}
	}

	wnd10->Refresh();
}

void MyFrame::CreateFenForVariation(PGN::Move* pMove)
{
	if( pMove->pVarWhite != 0 /*&& pMove->pVarWhite->white != ""*/)
	{
		CreateFenForVariationWhite(pMove, pMove->pPrev->getFenBlack() );
	}
	if( pMove->pVarBlack != 0 )
	{
		CreateFenForVariationBlack(pMove, pMove->getFenWhite() );
	}

}

void MyFrame::OnCreateNotebook(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().
                  Caption(wxT("Notebook")).
                  Float().FloatingPosition(GetStartPosition()).
                  //FloatingSize(300,200).
                  CloseButton(true).MaximizeButton(true));
    m_mgr.Update();
}

void MyFrame::OnCreateText(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateTextCtrl(), wxAuiPaneInfo().
                  Caption(wxT("Text Control")).
                  Float().FloatingPosition(GetStartPosition()));
    m_mgr.Update();
}

void MyFrame::OnCreateSizeReport(wxCommandEvent& WXUNUSED(event))
{
    m_mgr.AddPane(CreateSizeReportCtrl(), wxAuiPaneInfo().
                  Caption(wxT("Client Size Reporter")).
                  Float().FloatingPosition(GetStartPosition()).
                  CloseButton(true).MaximizeButton(true));
    m_mgr.Update();
}



void MyFrame::OnChangeContentPane(wxCommandEvent& evt)
{
    m_mgr.GetPane(wxT("Graphical Pane")).Show(evt.GetId() == ID_GridContent);
    m_mgr.GetPane(wxT("text_content")).Show(evt.GetId() == ID_TextContent);
    m_mgr.GetPane(wxT("tree_content")).Show(evt.GetId() == ID_TreeContent);
    m_mgr.GetPane(wxT("sizereport_content")).Show(evt.GetId() == ID_SizeReportContent);
    m_mgr.GetPane(wxT("html_content")).Show(evt.GetId() == ID_HTMLContent);
    m_mgr.GetPane(wxT("notebook_content")).Show(evt.GetId() == ID_NotebookContent);
    m_mgr.Update();
}

void MyFrame::OnDropDownToolbarItem(wxAuiToolBarEvent& evt)
{
    if (evt.IsDropDownClicked())
    {
        wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());

        tb->SetToolSticky(evt.GetId(), true);

        // create the popup menu
        wxMenu menuPopup;

        wxBitmap bmp = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16,16));

        wxMenuItem* m1 =  new wxMenuItem(&menuPopup, 101, _("Drop Down Item 1"));
        m1->SetBitmap(bmp);
        menuPopup.Append(m1);

        wxMenuItem* m2 =  new wxMenuItem(&menuPopup, 101, _("Drop Down Item 2"));
        m2->SetBitmap(bmp);
        menuPopup.Append(m2);

        wxMenuItem* m3 =  new wxMenuItem(&menuPopup, 101, _("Drop Down Item 3"));
        m3->SetBitmap(bmp);
        menuPopup.Append(m3);

        wxMenuItem* m4 =  new wxMenuItem(&menuPopup, 101, _("Drop Down Item 4"));
        m4->SetBitmap(bmp);
        menuPopup.Append(m4);

        // line up our menu with the button
        wxRect rect = tb->GetToolRect(evt.GetId());
        wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
        pt = ScreenToClient(pt);


        PopupMenu(&menuPopup, pt);


        // make sure the button is "un-stuck"
        tb->SetToolSticky(evt.GetId(), false);
    }
}


void MyFrame::OnTabAlignment(wxCommandEvent &evt)
{
   size_t i, count;
    wxAuiPaneInfoArray& all_panes = m_mgr.GetAllPanes();
    for (i = 0, count = all_panes.GetCount(); i < count; ++i)
    {
        wxAuiPaneInfo& pane = all_panes.Item(i);
        if (pane.window->IsKindOf(CLASSINFO(wxAuiNotebook)))
        {
            wxAuiNotebook* nb = (wxAuiNotebook*)pane.window;

            if (evt.GetId() == ID_NotebookAlignTop)
                nb->SetWindowStyleFlag(nb->GetWindowStyleFlag()^wxAUI_NB_BOTTOM|wxAUI_NB_TOP);
           else if (evt.GetId() == ID_NotebookAlignBottom)
               nb->SetWindowStyleFlag(nb->GetWindowStyleFlag()^wxAUI_NB_TOP|wxAUI_NB_BOTTOM);
            nb->Refresh();
        }
    }
}

void MyFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_("wxAUI Demo\nAn advanced window management library for wxWidgets\n(c) Copyright 2005-2006, Kirix Corporation"), _("About wxAUI Demo"), wxOK, this);
}

wxTextCtrl* MyFrame::CreateTextCtrl(const wxString& ctrl_text)
{
    static int n = 0;

    wxString text;
    if (ctrl_text.Length() > 0)
        text = ctrl_text;
    //else
    //   text.Printf(wxT("This is text box %d"), ++n);

    return new wxTextCtrl(this,wxID_ANY, text,
                          wxPoint(0,0), wxSize(150,90),
                          wxNO_BORDER | wxTE_MULTILINE);
}


wxGrid* MyFrame::CreateGrid()
{
    wxGrid* grid = new wxGrid(this, wxID_ANY,
                              wxPoint(0,0),
                              wxSize(150,250),
                              wxNO_BORDER | wxWANTS_CHARS);
    grid->CreateGrid(50, 20);
    return grid;
}

wxTreeCtrl* MyFrame::CreateTreeCtrl()
{
    wxTreeCtrl* tree = new wxTreeCtrl(this, wxID_ANY,
                                      wxPoint(0,0), wxSize(160,250),
                                      wxTR_DEFAULT_STYLE | wxNO_BORDER);

    wxImageList* imglist = new wxImageList(16, 16, true, 2);
    imglist->Add(wxArtProvider::GetBitmap(wxART_FOLDER, wxART_OTHER, wxSize(16,16)));
    imglist->Add(wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16)));
    tree->AssignImageList(imglist);

    wxTreeItemId root = tree->AddRoot(wxT("wxAUI Project"), 0);
    wxArrayTreeItemIds items;



    items.Add(tree->AppendItem(root, wxT("Item 1"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 2"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 3"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 4"), 0));
    items.Add(tree->AppendItem(root, wxT("Item 5"), 0));


    int i, count;
    for (i = 0, count = items.Count(); i < count; ++i)
    {
        wxTreeItemId id = items.Item(i);
        tree->AppendItem(id, wxT("Subitem 1"), 1);
        tree->AppendItem(id, wxT("Subitem 2"), 1);
        tree->AppendItem(id, wxT("Subitem 3"), 1);
        tree->AppendItem(id, wxT("Subitem 4"), 1);
        tree->AppendItem(id, wxT("Subitem 5"), 1);
    }


    tree->Expand(root);

    return tree;
}

wxSizeReportCtrl* MyFrame::CreateSizeReportCtrl(int width, int height)
{
    wxSizeReportCtrl* ctrl = new wxSizeReportCtrl(this, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(width, height), &m_mgr);
    return ctrl;
}

BoardCanvas* MyFrame::CreateBoardCanvas()
{
	board_canvas = new BoardCanvas(this);
	return board_canvas;
}



wxHtmlWindow* MyFrame::CreateHTMLCtrl(wxWindow* parent)
{
    if (!parent)
        parent = this;

    wxHtmlWindow* ctrl = new wxHtmlWindow(parent, wxID_ANY,
                                   wxDefaultPosition,
                                   wxSize(400,300));
    ctrl->SetPage(GetIntroText());
	html_ctrl = ctrl;
    return ctrl;
}

wxAuiNotebook* MyFrame::CreateNotebook()
{
   // create the notebook off-window to avoid flicker
   wxSize client_size = GetClientSize();

   wxAuiNotebook* ctrl = new wxAuiNotebook(this, wxID_ANY,
                                    wxPoint(client_size.x, client_size.y),
                                    wxSize(430,200),
                                    m_notebook_style);

	ctrl->SetArtProvider(new wxAuiSimpleTabArt);
    m_notebook_theme = 1;
   wxBitmap page_bmp = wxArtProvider::GetBitmap(wxART_NORMAL_FILE, wxART_OTHER, wxSize(16,16));

//   ctrl->AddPage(CreateHTMLCtrl(ctrl), wxT("Welcome to wxAUI") , false, page_bmp);

   /*wxPanel *panel = new wxPanel( ctrl, wxID_ANY );
   wxFlexGridSizer *flex = new wxFlexGridSizer( 2 );
   flex->AddGrowableRow( 0 );
   flex->AddGrowableRow( 3 );
   flex->AddGrowableCol( 1 );
   flex->Add( 5,5 );   flex->Add( 5,5 );
   flex->Add( new wxStaticText( panel, -1, wxT("wxTextCtrl:") ), 0, wxALL|wxALIGN_CENTRE, 5 );

   flex->Add( new wxTextCtrl( panel, -1, wxT(""), wxDefaultPosition, wxSize(100,-1)),
                1, wxALL|wxALIGN_CENTRE, 5 );
   flex->Add( new wxStaticText( panel, -1, wxT("wxSpinCtrl:") ), 0, wxALL|wxALIGN_CENTRE, 5 );
   flex->Add( new wxSpinCtrl( panel, -1, wxT("5"), wxDefaultPosition, wxSize(100,-1),
                wxSP_ARROW_KEYS, 5, 50, 5 ), 0, wxALL|wxALIGN_CENTRE, 5 );
   flex->Add( 5,5 );   flex->Add( 5,5 );
   panel->SetSizer( flex );*/
  


//   ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some text"),
 //               wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER|wxTE_RICH) , wxT("wxTextCtrl 1"), false, page_bmp );

  // wxPanel *cp = new wxPanel(this);

   

   ctrl->AddPage(noteFrame,wxT("Game Notation"), true);//#

    ctrl->AddPage( m_listPanel, wxT("Game List"), false, page_bmp );
  // wxSize pn1sz(panel1->GetClientSize());

   //my_canvas->SetSize(pn1sz);
                //wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 2") );

   /*ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some more text"),
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 3") );

   ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some more text"),
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 4") );

   ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some more text"),
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 5") );

   ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some more text"),
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 6") );

   ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some more text"),
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 7 (longer title)") );

   ctrl->AddPage( new wxTextCtrl( ctrl, wxID_ANY, wxT("Some more text"),
                wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxNO_BORDER) , wxT("wxTextCtrl 8") );
				*/
   return ctrl;
}

wxString MyFrame::GetIntroText()
{
    const char* text =
        "<html><body>"
        "<h3>Welcome to wxAUI</h3>"
        "<br/><b>Overview</b><br/>"
        "<p>wxAUI is an Advanced User Interface library for the wxWidgets toolkit "
        "that allows developers to create high-quality, cross-platform user "
        "interfaces quickly and easily.</p>"
        "<p><b>Features</b></p>"
        "<p>With wxAUI, developers can create application frameworks with:</p>"
        "<ul>"
        "<li>Native, dockable floating frames</li>"
        "<li>Perspective saving and loading</li>"
        "<li>Native toolbars incorporating real-time, &quot;spring-loaded&quot; dragging</li>"
        "<li>Customizable floating/docking behavior</li>"
        "<li>Completely customizable look-and-feel</li>"
        "<li>Optional transparent window effects (while dragging or docking)</li>"
        "<li>Splittable notebook control</li>"
        "</ul>"
        "<p><b>What's new in 0.9.4?</b></p>"
        "<p>wxAUI 0.9.4, which is bundled with wxWidgets, adds the following features:"
        "<ul>"
        "<li>New wxAuiToolBar class, a toolbar control which integrates more "
        "cleanly with wxAuiFrameManager.</li>"
        "<li>Lots of bug fixes</li>"
        "</ul>"
        "<p><b>What's new in 0.9.3?</b></p>"
        "<p>wxAUI 0.9.3, which is now bundled with wxWidgets, adds the following features:"
        "<ul>"
        "<li>New wxAuiNotebook class, a dynamic splittable notebook control</li>"
        "<li>New wxAuiMDI* classes, a tab-based MDI and drop-in replacement for classic MDI</li>"
        "<li>Maximize/Restore buttons implemented</li>"
        "<li>Better hinting with wxGTK</li>"
        "<li>Class rename.  'wxAui' is now the standard class prefix for all wxAUI classes</li>"
        "<li>Lots of bug fixes</li>"
        "</ul>"
        "<p><b>What's new in 0.9.2?</b></p>"
        "<p>The following features/fixes have been added since the last version of wxAUI:</p>"
        "<ul>"
        "<li>Support for wxMac</li>"
        "<li>Updates for wxWidgets 2.6.3</li>"
        "<li>Fix to pass more unused events through</li>"
        "<li>Fix to allow floating windows to receive idle events</li>"
        "<li>Fix for minimizing/maximizing problem with transparent hint pane</li>"
        "<li>Fix to not paint empty hint rectangles</li>"
        "<li>Fix for 64-bit compilation</li>"
        "</ul>"
        "<p><b>What changed in 0.9.1?</b></p>"
        "<p>The following features/fixes were added in wxAUI 0.9.1:</p>"
        "<ul>"
        "<li>Support for MDI frames</li>"
        "<li>Gradient captions option</li>"
        "<li>Active/Inactive panes option</li>"
        "<li>Fix for screen artifacts/paint problems</li>"
        "<li>Fix for hiding/showing floated window problem</li>"
        "<li>Fix for floating pane sizing problem</li>"
        "<li>Fix for drop position problem when dragging around center pane margins</li>"
        "<li>LF-only text file formatting for source code</li>"
        "</ul>"
        "<p>See README.txt for more information.</p>"
        "</body></html>";

    return wxString::FromAscii(text);
}

#ifdef __WXOSX__

void MyFrame::OnToggleMacUseGeneric(wxCommandEvent& event)
{
    wxSystemOptions::SetOption(wxT("mac.listctrl.always_use_generic"), event.IsChecked());
}

#endif // __WXOSX__


void MyFrame::PlayThroughGame()
{
	if( m_gameTraverseTimer == NULL )
	{
		m_gameTraverseTimer = new TraverseGameTimer(this);
	}
	currentGame->PrepareForVisitor();
	m_gameTraverseTimer->start(); 
}



void MyFrame::IncMove()
{
	bool enterVariations = true;
	bool isMainLine = currentGame->pCurrMove->IsThisMainLine();
				/*if( m_owner->board_canvas->moveForward == false )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;
				}	*/			
				/*if( m_owner->variationSelectionBox &&
					m_owner->variationSelectionBox->IsVisible() )
					m_owner->variationSelectionBox->Close();
				*/
				if( board_canvas->IsAnimating() )
					return;

				bool bStartAnimation = true;

				if( pgnLoad.move_counter== 0)
				{
					wxMessageBox("Game has no moves! Load a Game first", "error");
					return;
				}

					bool treeUsed = false;

				if( (my_canvas->white_black_move == WHITE) && !(my_canvas->move_count == 0))
				{
					if( currentGame->pCurrMove->pNext &&
						currentGame->pCurrMove->pNext->white != "" &&
						!currentGame->pCurrMove->atEndBlack)
					{

							currentGame->NextMove();
							treeUsed = true;
							//treeUsed  = true;
						//		m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
							//	m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();
					}
					else 
					{
						if( currentGame->pRoot->white == "" )//&&
							//m_owner->currentGame->pCurrMove->pNext->white != "")
						{
							if( (currentGame->IsInBase()) ||
								(currentGame->pCurrMove->pNext->white != ""))
							{
								currentGame->NextMove();
								if( currentGame->pCurrMove->white != "" )
									my_canvas->white_black_move = WHITE;
								else
									my_canvas->white_black_move = BLACK;
							}
							else return;
							

						}
						//else return;
					}
				}
				/*else if( (white_black_moves == WHITE) && (move_count == 0) &&
					(m_owner->currentGame->pRoot->white == "") &&
					(!m_owner->currentGame->pRoot->black == ""))
				{

				}*/

			

				if( (my_canvas->white_black_move == WHITE) )
				{

					if( currentGame->GetCurrentMove()->pVarWhite && enterVariations)
					{
						bool traverseByTree = false;
						PGN::Move* temp = currentGame->GetCurrentMove()->pVarWhite;
						while( temp )
						{
							if( !temp->HasBeenVisited() )
								traverseByTree = true;
							temp = temp->pVarWhite;
						}

						//m_owner->CreateVariationSelectionBox(white_black_move);
						//SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
						//treeCanvas->SetSelection(0);
						if( traverseByTree )
						{
							SelectMoveFromTreeAutoTraversal(0);
					
							if( bStartAnimation )
								board_canvas->StartAnimation();
						//m_owner->treeCanvas->Refresh(true);
							return;
						}
					}

					if( currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					
					{
						if( currentGame->AtEnd())
						{
								if( currentGame->pCurrMove->atEndWhite )
								{
									if( my_canvas->white_black_move == BLACK && !treeUsed)
									{
										//white_black_move = WHITE;
										if( !isMainLine )
										{
											my_canvas->white_black_move = currentGame->pCurrMove->GetVariationRootColor();
											if( my_canvas->white_black_move == WHITE )
											{
												my_canvas->white_black_move = BLACK;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
											}
																					
										}
										//return;
									}
									//my_canvas->white_black_move = WHITE;
									my_canvas->move_count++;
									treeUsed = true;
								}
								else if( currentGame->pCurrMove->atEndBlack )
								{
									if( my_canvas->white_black_move == WHITE && !treeUsed)
									{
										//white_black_move = BLACK;
										if( !isMainLine )
										{
											my_canvas->white_black_move = currentGame->pCurrMove->GetVariationRootColor();
											if( my_canvas->white_black_move == WHITE )
											{
												my_canvas->white_black_move = BLACK;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
											}
										}
										treeUsed = true;
									}
						
								}
							
						

						}
						else
						{
							if( (treeCanvas->numSelectionsThisMove == 1) &&
								(treeCanvas->GetSelection() != 0) && enterVariations)
							{
								//SelectMoveFromTree(treeCanvas->GetSelection());
								//treeCanvas->SetSelection(0);
								SelectMoveFromTreeAutoTraversal(0);
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();
								
								my_canvas->move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								return;
							}
						}

					}
					else treeUsed = true;
				}

				if(my_canvas->white_black_move == BLACK && !treeUsed)
				{

					if( currentGame->GetCurrentMove()->pVarBlack && enterVariations) 
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						//SelectMoveFromTree(treeCanvas->GetSelection());
						//treeCanvas->SetSelection(0);
						SelectMoveFromTreeAutoTraversal(0);
						if( bStartAnimation )
							board_canvas->StartAnimation();

						my_canvas->move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
						return;
					}

					if(currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					{
						if( currentGame->AtEnd())
						{
								if( currentGame->pCurrMove->atEndWhite )
								{
									if( my_canvas->white_black_move == BLACK && !treeUsed )
									{
										//white_black_move = WHITE;
										if( !isMainLine )
										{
											my_canvas->white_black_move = currentGame->pCurrMove->GetVariationRootColor();
											if( my_canvas->white_black_move == WHITE )
											{
												my_canvas->white_black_move = BLACK;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
											}
										}

										treeUsed = true;
										//return;
									}
								//	my_canvas->white_black_move = WHITE;
								}
								else if( currentGame->pCurrMove->atEndBlack )
								{
									if( my_canvas->white_black_move == WHITE && !treeUsed)
									{
										//white_black_move = BLACK;
										if( !isMainLine )
										{
											my_canvas->white_black_move = currentGame->pCurrMove->GetVariationRootColor();
											if( my_canvas->white_black_move == WHITE )
											{
												my_canvas->white_black_move = BLACK;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
											}						
										}
										treeUsed = true;
										//return;
									}
						
								}
							}

						
						else
						{
							if( (treeCanvas->numSelectionsThisMove == 1) &&
								(treeCanvas->GetSelection() != 0) && enterVariations)
							{
								//SelectMoveFromTree(treeCanvas->GetSelection());
								//treeCanvas->SetSelection(0);
								SelectMoveFromTreeAutoTraversal(0);
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								my_canvas->move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								return;
							}
						}

					
					}
				}

				if( currentGame->IsInBase() )
				{
					board_canvas->pCurrMov=(MyChessMove*)0xffffff;

					currentGame->NextMove();
					if( currentGame->pCurrMove->white != "" )
						my_canvas->white_black_move = WHITE;
					else
						my_canvas->white_black_move = BLACK;
					if( currentGame->GetCurrentMove()->pVarWhite || 
						(currentGame->GetCurrentMove()->GetVariationRoot()!=NULL) && enterVariations)
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						//SelectMoveFromTree(treeCanvas->GetSelection());
						//treeCanvas->SetSelection(0);
						SelectMoveFromTreeAutoTraversal(0);
						if( bStartAnimation )
							board_canvas->StartAnimation();
						return;
					}
				}

				

				if( currentGame->AtEnd()  )
				{
					board_canvas->pCurrMov=(MyChessMove*)0xffffff;

					if( currentGame->pCurrMove->atEndWhite )
					{
						if( my_canvas->white_black_move == BLACK && !treeUsed)
						{
							//white_black_move = WHITE;
							if( !isMainLine )
							{
								my_canvas->white_black_move = currentGame->pCurrMove->GetVariationRootColor();
								if( my_canvas->white_black_move == WHITE )
								{
									my_canvas->white_black_move = BLACK;
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
								}
								else
								{
									my_canvas->white_black_move = WHITE;
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
								}
							}
							else 
							{
								this->m_gameTraverseTimer->Stop();
							}
						}
						
						my_canvas->white_black_move = WHITE;
					}
					else if( currentGame->pCurrMove->atEndBlack )
					{
						if( my_canvas->white_black_move == WHITE && !treeUsed)
						{
							//white_black_move = BLACK;
							if( !isMainLine )
							{
								my_canvas->white_black_move = currentGame->pCurrMove->GetVariationRootColor();
								if( my_canvas->white_black_move == WHITE )
								{
									my_canvas->white_black_move = BLACK;
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
								}
								else
								{
									my_canvas->white_black_move = WHITE;
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
								}
							}
							else
							{
								this->m_gameTraverseTimer->Stop();
							}
							//return;
						}
						
					}

					bStartAnimation = false;
					//break;
				}

				if( currentGame->pCurrMove->info == "" )
				{
					this->m_gameTraverseTimer->Stop();
				}
		

				//MyChessMove mv = 
				if( my_canvas->white_black_move == WHITE )
				{
					
					//ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->white );
					//m_owner->ctrl->chess_board.parseMove( m_owner->currentGame->GetNextMove()->white );
					board_canvas->GetBoard()->SetBoard( currentGame->CurrFenWhite() );
				
					my_canvas->white_black_move = BLACK;


					wnd10->Clear();
					if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wxString inpt ="";

						for( int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_white.text[aa].c_str());
						//	inpt+= " ";
							//m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str()));
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						wnd10->Refresh(true);
					}
				}
				else
				{
					
				//	ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->black );
					board_canvas->GetBoard()->SetBoard( currentGame->CurrFenBlack() );
					my_canvas->white_black_move = WHITE;
					
					wnd10->Clear();
					if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wxString inpt = "";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
						//	inpt += " ";
						//	m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str()));
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						wnd10->Refresh(true);
					}
					my_canvas->move_count++;
				}
				
				
				
				my_canvas->ScrollToCursor();

				//wxRect clear(XX,YY,w, h);
				//my_canvas->Refresh(/*, &clear*/ );
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				DoRefresh();
				if( bStartAnimation )
					board_canvas->StartAnimation();
				//treeCanvas->Refresh(true);

				for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
				{
					if( my_canvas->activeEngines[j]->IsActive() &&
						my_canvas->activeEngines[j]->IsAnalysing())
					{
						my_canvas->activeEngines[j]->KibitzStartsMove();
						my_canvas->activeEngines[j]->KibitzUserEndsMove();
					}
				}

				
				//Refresh();
			//	ctrl->chess_board.

}






// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    File_Quit = wxID_EXIT,
    File_About = wxID_ABOUT,

    MenuShow_First = wxID_HIGHEST,
    File_ShowDefault = MenuShow_First,
    File_ShowText,
    File_ShowLines,
    File_ShowBrushes,
    File_ShowPolygons,
    File_ShowMask,
    File_ShowOps,
    File_ShowRegions,
    File_ShowCircles,
    File_ShowSplines,
#if wxUSE_GRAPHICS_CONTEXT
    File_ShowAlpha,
#endif
    File_ShowGradients,
    MenuShow_Last = File_ShowGradients,

    File_Clip,
#if wxUSE_GRAPHICS_CONTEXT
    File_GraphicContext,
#endif

    MenuOption_First,

    MapMode_Text = MenuOption_First,
    MapMode_Lometric,
    MapMode_Twips,
    MapMode_Points,
    MapMode_Metric,

    UserScale_StretchHoriz,
    UserScale_ShrinkHoriz,
    UserScale_StretchVertic,
    UserScale_ShrinkVertic,
    UserScale_Restore,

    AxisMirror_Horiz,
    AxisMirror_Vertic,

    LogicalOrigin_MoveDown,
    LogicalOrigin_MoveUp,
    LogicalOrigin_MoveLeft,
    LogicalOrigin_MoveRight,
    LogicalOrigin_Set,
    LogicalOrigin_Restore,

#if wxUSE_COLOURDLG
    Colour_TextForeground,
    Colour_TextBackground,
    Colour_Background,
#endif // wxUSE_COLOURDLG
    Colour_BackgroundMode,
	SET_VARIATION_COLOR,
	SET_MOVE_COLOR,
	PROMOTE_VARIATION,
	DELETE_VARIATION,
	DELETE_REMAINING_MOVES,
	ADD_ANNOTATION,
    Colour_TextureBackgound,

    MenuOption_Last = Colour_TextureBackgound
};


// ----------------------------------------------------------------------------
// MyCanvas
// ----------------------------------------------------------------------------

// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them.
BEGIN_EVENT_TABLE(MyCanvas, wxScrolledWindow)
    EVT_PAINT  (MyCanvas::OnPaint)
	EVT_IDLE(MyCanvas::OnIdle)
	EVT_SIZE(MyCanvas::OnSize)
    EVT_LEFT_DOWN (MyCanvas::OnMouseMove)
	EVT_RIGHT_DOWN (MyCanvas::rightClick)
	EVT_MENU(SET_VARIATION_COLOR, MyCanvas::SetVariationColor)
	EVT_MENU(SET_MOVE_COLOR, MyCanvas::SetMoveColor)
	EVT_MENU(PROMOTE_VARIATION, MyCanvas::PromoteVariation)
	EVT_MENU(DELETE_VARIATION, MyCanvas::DeleteVariation)
	EVT_MENU(DELETE_REMAINING_MOVES, MyCanvas::DeleteRemainingMoves)
	EVT_MENU(ADD_ANNOTATION, MyCanvas::AddAnnotation)

END_EVENT_TABLE()



MyCanvas::MyCanvas( MyFrame* parent, wxWindow *owner):
 wxScrolledWindow(owner, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE | wxBORDER_THEME )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	SetDoubleBuffered(true);
	m_useContext = true;
    m_owner = parent;
    m_show = Show_Default;
   // m_smile_bmp = wxBitmap(smile_xpm);
    m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
	//this->EnableScrolling(false, true);
	//SetScrollbar(wxVERTICAL, 0, 16, 50);
	//wxSize sz(GetClientSize());
	//SetVirtualSize(sz.GetWidth(), sz.GetHeight()*4);
	bCheckVirtualSize = true;
	this->SetBackgroundColour(*wxWHITE);
	wxSize sz(GetClientSize());
       w = sz.GetWidth();
       h = sz.GetHeight();

	   needsRefresh = true;
	   MoveToCursor = false;
	
SetVirtualSize(sz.GetWidth(), sz.GetHeight()*100);
	scrollX = 0;
	scrollY = 1;
 
	Scroll(0,-10);
	  // SetSize(0, 40, w,h-40);
	  // this->gs_bmp4 = new wxBitmap;
	      /* 4 colour bitmap */
    //gs_bmp4->LoadFile("board.bmp", wxBITMAP_TYPE_BMP);
        /* init scrolled area size, scrolling speed, etc. */
       //SetScrollbars(1,1, w, h*2, 0, 0);
	move_count = 0;
#ifdef USE_CANVAS_SET_FOCUS
	SetFocus();
#endif
	   SetSizer(NULL,false);
    m_clip = true;
	selectedMove = 0;
	white_black_move = 0;
	      
	/*wxFont font = m_owner->optionsFrame->m_NotationOptions.notationFont;
		wxFont f1 = font;//m_owner->optionsFrame->m_NotationOptions.notationFont;
		wxFont f2 = font;//m_owner->optionsFrame->m_NotationOptions.notationFont;
		wxFont f3 = font;//m_owner->optionsFrame->m_NotationOptions.notationFont;
		f1.SetWeight(wxNORMAL);
		f2.SetWeight(wxBOLD);
		f3.SetStyle(wxITALIC);
		//f1.SetPixelSize();
	SetFonts(f1, f2, f3);*/
	fontSize = 10;

	fontBold = wxFont(fontSize, wxMODERN, wxNORMAL, wxBOLD, false);
	fontItalic = wxFont(fontSize, wxMODERN, wxFONTSTYLE_ITALIC, wxNORMAL, false);
	fontNormal = wxFont(fontSize, wxMODERN, wxNORMAL, wxNORMAL, false);
	



#if wxUSE_GRAPHICS_CONTEXT
    m_useContext = false;
#endif

    // connect event handlers for the blue input window
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyCanvas::OnKeyDown),
                        NULL, this);
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyCanvas::OnKeyUp),
                        NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(MyCanvas::OnChar),
                        NULL, this);
}

MyCanvas::MyCanvas(MyFrame *parent)
//:wxScrolledWindow( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER )
        : wxScrolledWindow(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                             wxVSCROLL | wxFULL_REPAINT_ON_RESIZE )
{
#ifdef BUFFER_DOUBLE
	SetExtraStyle(wxWS_EX_BUFFERED_DRAW);
#endif

	//SetDoubleBuffered(true);
	m_useContext = true;
    m_owner = parent;
    m_show = Show_Default;
   // m_smile_bmp = wxBitmap(smile_xpm);
    m_std_icon = wxArtProvider::GetIcon(wxART_INFORMATION);
	//this->EnableScrolling(false, true);
	//SetScrollbar(wxVERTICAL, 0, 16, 50);
	//wxSize sz(GetClientSize());
	//SetVirtualSize(sz.GetWidth(), sz.GetHeight()*4);
	bCheckVirtualSize = true;
	wxSize sz(GetClientSize());
       w = sz.GetWidth();
       h = sz.GetHeight();

	scrollX = 0;
	scrollY = 1;
	//wxPanel *panel = new wxPanel(this);

	//panel->SetSize(w,120);
	
	//SetSize(0, 120, w, h-120);
 
	  // this->gs_bmp4 = new wxBitmap;
	      /* 4 colour bitmap */
    //gs_bmp4->LoadFile("board.bmp", wxBITMAP_TYPE_BMP);
        /* init scrolled area size, scrolling speed, etc. */
       //SetScrollbars(1,1, w, h*2, 0, 0);
	move_count = 0;
	Scroll(0,0);
#ifdef USE_CANVAS_SET_FOCUS
	SetFocus();
#endif
	   SetSizer(NULL,false);
    m_clip = true;
	selectedMove = 0;
	white_black_move = 0;

#if wxUSE_GRAPHICS_CONTEXT
    m_useContext = false;
#endif

    // connect event handlers for the blue input window
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(MyCanvas::OnKeyDown),
                        NULL, this);
    Connect(wxEVT_KEY_UP, wxKeyEventHandler(MyCanvas::OnKeyUp),
                        NULL, this);
    Connect(wxEVT_CHAR, wxKeyEventHandler(MyCanvas::OnChar),
                        NULL, this);
}


void MyCanvas::SetFonts(wxFont norm, wxFont bold, wxFont italic)
{
	fontBold = bold;
	fontNormal = norm;
	fontItalic = italic;
	this->fontSize = bold.GetPointSize();
}

void MyCanvas::SetTextViewportBackground(wxColor col)
{
	this->SetBackgroundColour(col);
}


void MyCanvas::DrawTestBrushes(wxDC& dc)
{
    static const wxCoord WIDTH = 200;
    static const wxCoord HEIGHT = 80;

    wxCoord x = 10,
            y = 10;

    dc.SetBrush(wxBrush(*wxGREEN, wxSOLID));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Solid green"), x + 10, y + 10);

    y += HEIGHT;
    dc.SetBrush(wxBrush(*wxRED, wxCROSSDIAG_HATCH));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Hatched red"), x + 10, y + 10);

    y += HEIGHT;
//    dc.SetBrush(wxBrush(*gs_bmpMask));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Stipple mono"), x + 10, y + 10);

    y += HEIGHT;
//    dc.SetBrush(wxBrush(*gs_bmpNoMask));
    dc.DrawRectangle(x, y, WIDTH, HEIGHT);
    dc.DrawText(_T("Stipple colour"), x + 10, y + 10);
}

void MyCanvas::DrawTestPoly(wxDC& dc)
{
    wxBrush brushHatch(*wxRED, wxFDIAGONAL_HATCH);
    dc.SetBrush(brushHatch);

    wxPoint star[5];
    star[0] = wxPoint(100, 60);
    star[1] = wxPoint(60, 150);
    star[2] = wxPoint(160, 100);
    star[3] = wxPoint(40, 100);
    star[4] = wxPoint(140, 150);

    dc.DrawText(_T("You should see two (irregular) stars below, the left one ")
                _T("hatched"), 10, 10);
    dc.DrawText(_T("except for the central region and the right ")
                _T("one entirely hatched"), 10, 30);
    dc.DrawText(_T("The third star only has a hatched outline"), 10, 50);

    dc.DrawPolygon(WXSIZEOF(star), star, 0, 30);
    dc.DrawPolygon(WXSIZEOF(star), star, 160, 30, wxWINDING_RULE);

    wxPoint star2[10];
    star2[0] = wxPoint(0, 100);
    star2[1] = wxPoint(-59, -81);
    star2[2] = wxPoint(95, 31);
    star2[3] = wxPoint(-95, 31);
    star2[4] = wxPoint(59, -81);
    star2[5] = wxPoint(0, 80);
    star2[6] = wxPoint(-47, -64);
    star2[7] = wxPoint(76, 24);
    star2[8] = wxPoint(-76, 24);
    star2[9] = wxPoint(47, -64);
    int count[2] = {5, 5};

    dc.DrawPolyPolygon(WXSIZEOF(count), count, star2, 450, 150);
}

void MyCanvas::DrawTestLines( int x, int y, int width, wxDC &dc )
{
    dc.SetPen( wxPen( wxT("black"), width, wxSOLID) );
    dc.SetBrush( *wxRED_BRUSH );
    dc.DrawText(wxString::Format(wxT("Testing lines of width %d"), width), x + 10, y - 10);
    dc.DrawRectangle( x+10, y+10, 100, 190 );

    dc.DrawText(_T("Solid/dot/short dash/long dash/dot dash"), x + 150, y + 10);
    dc.SetPen( wxPen( wxT("black"), width, wxSOLID) );
    dc.DrawLine( x+20, y+20, 100, y+20 );
    dc.SetPen( wxPen( wxT("black"), width, wxDOT) );
    dc.DrawLine( x+20, y+30, 100, y+30 );
    dc.SetPen( wxPen( wxT("black"), width, wxSHORT_DASH) );
    dc.DrawLine( x+20, y+40, 100, y+40 );
    dc.SetPen( wxPen( wxT("black"), width, wxLONG_DASH) );
    dc.DrawLine( x+20, y+50, 100, y+50 );
    dc.SetPen( wxPen( wxT("black"), width, wxDOT_DASH) );
    dc.DrawLine( x+20, y+60, 100, y+60 );

    dc.DrawText(_T("Misc hatches"), x + 150, y + 70);
    dc.SetPen( wxPen( wxT("black"), width, wxBDIAGONAL_HATCH) );
    dc.DrawLine( x+20, y+70, 100, y+70 );
    dc.SetPen( wxPen( wxT("black"), width, wxCROSSDIAG_HATCH) );
    dc.DrawLine( x+20, y+80, 100, y+80 );
    dc.SetPen( wxPen( wxT("black"), width, wxFDIAGONAL_HATCH) );
    dc.DrawLine( x+20, y+90, 100, y+90 );
    dc.SetPen( wxPen( wxT("black"), width, wxCROSS_HATCH) );
    dc.DrawLine( x+20, y+100, 100, y+100 );
    dc.SetPen( wxPen( wxT("black"), width, wxHORIZONTAL_HATCH) );
    dc.DrawLine( x+20, y+110, 100, y+110 );
    dc.SetPen( wxPen( wxT("black"), width, wxVERTICAL_HATCH) );
    dc.DrawLine( x+20, y+120, 100, y+120 );

    dc.DrawText(_T("User dash"), x + 150, y + 140);
    wxPen ud( wxT("black"), width, wxUSER_DASH );
    wxDash dash1[6];
    dash1[0] = 8;  // Long dash  <---------+
    dash1[1] = 2;  // Short gap            |
    dash1[2] = 3;  // Short dash           |
    dash1[3] = 2;  // Short gap            |
    dash1[4] = 3;  // Short dash           |
    dash1[5] = 2;  // Short gap and repeat +
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+140, 100, y+140 );
    dash1[0] = 5;  // Make first dash shorter
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+150, 100, y+150 );
    dash1[2] = 5;  // Make second dash longer
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+160, 100, y+160 );
    dash1[4] = 5;  // Make third dash longer
    ud.SetDashes( 6, dash1 );
    dc.SetPen( ud );
    dc.DrawLine( x+20, y+170, 100, y+170 );
}

void MyCanvas::DrawDefault(wxDC& dc)
{
    // mark the origin
    dc.DrawCircle(0, 0, 10);

#if !defined(wxMAC_USE_CORE_GRAPHICS) || !wxMAC_USE_CORE_GRAPHICS
    // GetPixel and FloodFill not supported by Mac OS X CoreGraphics
    // (FloodFill uses Blit from a non-wxMemoryDC)
    //flood fill using brush, starting at 1,1 and replacing whatever colour we find there
    dc.SetBrush(wxBrush(wxColour(128,128,0), wxSOLID));

    wxColour tmpColour ;
    dc.GetPixel(1,1, &tmpColour);
    dc.FloodFill(1,1, tmpColour, wxFLOOD_SURFACE);
#endif

    dc.DrawCheckMark(5, 80, 15, 15);
    dc.DrawCheckMark(25, 80, 30, 30);
    dc.DrawCheckMark(60, 80, 60, 60);

    // this is the test for "blitting bitmap into DC damages selected brush" bug
    wxCoord rectSize = m_std_icon.GetWidth() + 10;
    wxCoord x = 100;
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxGREEN_BRUSH );
    dc.DrawRectangle(x, 10, rectSize, rectSize);
    dc.DrawBitmap(m_std_icon, x + 5, 15, true);
    x += rectSize + 10;
    dc.DrawRectangle(x, 10, rectSize, rectSize);
    dc.DrawIcon(m_std_icon, x + 5, 15);
    x += rectSize + 10;
    dc.DrawRectangle(x, 10, rectSize, rectSize);

    // test for "transparent" bitmap drawing (it intersects with the last
    // rectangle above)
    //dc.SetBrush( *wxTRANSPARENT_BRUSH );

  //  if (m_smile_bmp.Ok())
   //     dc.DrawBitmap(m_smile_bmp, x + rectSize - 20, rectSize - 10, true);

    dc.SetBrush( *wxBLACK_BRUSH );
    dc.DrawRectangle( 0, 160, 1000, 300 );

    // draw lines
    wxBitmap bitmap(20,70);
    wxMemoryDC memdc;
    memdc.SelectObject( bitmap );
    memdc.SetBrush( *wxBLACK_BRUSH );
    memdc.SetPen( *wxWHITE_PEN );
    memdc.DrawRectangle(0,0,20,70);
    memdc.DrawLine( 10,0,10,70 );

    // to the right
    wxPen pen = *wxRED_PEN;
    memdc.SetPen(pen);
    memdc.DrawLine( 10, 5,10, 5 );
    memdc.DrawLine( 10,10,11,10 );
    memdc.DrawLine( 10,15,12,15 );
    memdc.DrawLine( 10,20,13,20 );

/*
    memdc.SetPen(*wxRED_PEN);
    memdc.DrawLine( 12, 5,12, 5 );
    memdc.DrawLine( 12,10,13,10 );
    memdc.DrawLine( 12,15,14,15 );
    memdc.DrawLine( 12,20,15,20 );
*/

    // same to the left
    memdc.DrawLine( 10,25,10,25 );
    memdc.DrawLine( 10,30, 9,30 );
    memdc.DrawLine( 10,35, 8,35 );
    memdc.DrawLine( 10,40, 7,40 );

    // XOR draw lines
    dc.SetPen(*wxWHITE_PEN);
    memdc.SetLogicalFunction( wxINVERT );
    memdc.SetPen( *wxWHITE_PEN );
    memdc.DrawLine( 10,50,10,50 );
    memdc.DrawLine( 10,55,11,55 );
    memdc.DrawLine( 10,60,12,60 );
    memdc.DrawLine( 10,65,13,65 );

    memdc.DrawLine( 12,50,12,50 );
    memdc.DrawLine( 12,55,13,55 );
    memdc.DrawLine( 12,60,14,60 );
    memdc.DrawLine( 12,65,15,65 );

    memdc.SelectObject( wxNullBitmap );
    dc.DrawBitmap( bitmap, 10, 170 );
    wxImage image = bitmap.ConvertToImage();
    image.Rescale( 60,210 );
    bitmap = wxBitmap(image);
    dc.DrawBitmap( bitmap, 50, 170 );

    // test the rectangle outline drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRectangle(150, 170, 49, 29);
    dc.DrawRectangle(200, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(250, 210, 250, 170);
    dc.DrawLine(260, 200, 150, 200);

    // test the rectangle filled drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 170, 49, 29);
    dc.DrawRectangle(350, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(400, 170, 400, 210);
    dc.DrawLine(300, 200, 410, 200);

    // a few more tests of this kind
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 220, 1, 1);
    dc.DrawRectangle(310, 220, 2, 2);
    dc.DrawRectangle(320, 220, 3, 3);
    dc.DrawRectangle(330, 220, 4, 4);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(300, 230, 1, 1);
    dc.DrawRectangle(310, 230, 2, 2);
    dc.DrawRectangle(320, 230, 3, 3);
    dc.DrawRectangle(330, 230, 4, 4);

    // and now for filled rect with outline
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle(500, 170, 49, 29);
    dc.DrawRectangle(550, 170, 49, 29);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(600, 170, 600, 210);
    dc.DrawLine(500, 200, 610, 200);

    // test the rectangle outline drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxWHITE_PEN);
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRoundedRectangle(150, 270, 49, 29, 6);
    dc.DrawRoundedRectangle(200, 270, 49, 29, 6);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(250, 270, 250, 310);
    dc.DrawLine(150, 300, 260, 300);

    // test the rectangle filled drawing - there should be one pixel between
    // the rect and the lines
    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRoundedRectangle(300, 270, 49, 29, 6);
    dc.DrawRoundedRectangle(350, 270, 49, 29, 6);
    dc.SetPen(*wxWHITE_PEN);
    dc.DrawLine(400, 270, 400, 310);
    dc.DrawLine(300, 300, 410, 300);

    // Added by JACS to demonstrate bizarre behaviour.
    // With a size of 70, we get a missing red RHS,
    // and the height is too small, so we get yellow
    // showing. With a size of 40, it draws as expected:
    // it just shows a white rectangle with red outline.
    int totalWidth = 70;
    int totalHeight = 70;
    wxBitmap bitmap2(totalWidth, totalHeight);

    wxMemoryDC memdc2;
    memdc2.SelectObject(bitmap2);

    wxColour clr(255, 255, 0);
    wxBrush yellowBrush(clr, wxSOLID);
    memdc2.SetBackground(yellowBrush);
    memdc2.Clear();

    wxPen yellowPen(clr, 1, wxSOLID);

    // Now draw a white rectangle with red outline. It should
    // entirely eclipse the yellow background.
    memdc2.SetPen(*wxRED_PEN);
    memdc2.SetBrush(*wxWHITE_BRUSH);

    memdc2.DrawRectangle(0, 0, totalWidth, totalHeight);

    memdc2.SetPen(wxNullPen);
    memdc2.SetBrush(wxNullBrush);
    memdc2.SelectObject(wxNullBitmap);

    dc.DrawBitmap(bitmap2, 500, 270);

    // Repeat, but draw directly on dc
    // Draw a yellow rectangle filling the bitmap

    x = 600; int y = 270;
    dc.SetPen(yellowPen);
    dc.SetBrush(yellowBrush);
    dc.DrawRectangle(x, y, totalWidth, totalHeight);

    // Now draw a white rectangle with red outline. It should
    // entirely eclipse the yellow background.
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);

    dc.DrawRectangle(x, y, totalWidth, totalHeight);
}

int MyCanvas::DrawVariation(int &currPos, int *depth, int height,
							PGN::Move *pMove, wxDC& dc, wxSize *sz)
{
	int curr_pos = currPos;
	//dc.SetPen( *wxGREEN );
//	dc.SetTextForeground(*wxGREEN);

	dc.SetFont( fontNormal );
	if( pMove->pVarWhite != 0 )
	{
		CodeFold *codeFold = new CodeFold();
		codeFold->SetWhiteBlack(-1);

		codeFold->SetMovePtr(pMove);
		codeFold->SetRect( currPos,*depth,height,height );

		codeFold->SetType( CODE_FOLD_DISP_NODE );
		currLine->AddNode(codeFold);
		codeFold->SetLine(currLine);
					
				
		currPos += 20;

		if( pMove->IsTrueDisplayVariations() )
		{
			DrawVariationWhite(currPos, depth, height, pMove, dc, sz, 10);
			currPos = 0;
		
			m_dataPage.AddLine(currLine);
			currLine = new Line();
		}
		else 
		{
			codeFold->SetState(1);
		}


		//depth += height;

	}
	
	if( pMove->pVarBlack != 0 )
	{
		if(  pMove->pVarWhite == 0 )
		{
			CodeFold *codeFold = new CodeFold();
			codeFold->SetWhiteBlack(-1);

			codeFold->SetMovePtr(pMove);
			codeFold->SetRect( currPos,*depth,height,height );

			codeFold->SetType( CODE_FOLD_DISP_NODE );
			currLine->AddNode(codeFold);
			codeFold->SetLine(currLine);
			currPos += 20;

			//	depth += height;
			if( pMove->IsTrueDisplayVariations() )
			{
				DrawVariationBlack(currPos, depth, height, pMove, dc, sz, 10);
				currPos = 0;
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			}
			else codeFold->SetState(1);
			
		}
		else
		{
			//	depth += height;
			if( pMove->IsTrueDisplayVariations() )
			{
				DrawVariationBlack(currPos, depth, height, pMove, dc, sz, 10);
				currPos = 0;
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			}
			
		}


		
	}

	if( ((pMove->pVarWhite != 0) || (pMove->pVarBlack != 0)) && pMove->IsTrueDisplayVariations())
	{
		*depth += height;
	}

	dc.SetFont( fontBold );
	//dc.SetPen( *wxBLACK );
	//dc.SetTextForeground(*wxBLACK);

	return 0;
}

 int MyCanvas::DrawVariationBlack(int &currPos, int *depth, int height,
							PGN::Move *pMove, wxDC& dc, wxSize *sz, 
							int offset, char start, char end)
{

	int length;
	int descent;
	int localH;
	char buff2[512];
	//wxRichTextCtrl *m_richTextCtrl = m_owner->m_richTextCtrl;
	char annotationText[1024];
	bool renderInverse = false;
	int rectLeftOffset = 5;
	int rectRightOffset = 2;
	int varCount = 0;
	PGN::Move *pMoveLocal = pMove->pVarBlack;

	while( pMoveLocal )
	{
		PGN::Move *pNextIter = pMoveLocal;
		int lcurrPos = offset;
		*depth += height;
		m_dataPage.AddLine(currLine);
		currLine = new Line();
		bool firstMove = true;
		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxBOLD, false) );
		char varNum[10];
		varNum[0] = 'A'+varCount;//, varNum, 10);
		varNum[1] = '.';
		varNum[2] = 0;
	//	dc.DrawText(wxString(varNum), offset, depth);
		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, false) );
		lcurrPos+=10;
		
		while( pNextIter && 
			 ((pNextIter->white.length()>0)||
			  (pNextIter->black.length() > 0)||
			  (pNextIter->result.length()>0)))
		{

			if( firstMove )
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());

				}
				else
				{
					/*if( pNextIter->pUp->variation_count_black > 1 )
					{
						sprintf(buff2, "%s) %c%s...", pNextIter->variation_id, start, pNextIter->info.c_str());
					}
					else
					{*/
					//sprintf(buff2, "%c ", start );
					//dc.GetTextExtent( buff2, &length, &height, &descent );
					if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
					{
						dc.DrawText(wxString(start), lcurrPos, *depth);
					}
					currDataNode->SetWhiteBlack(BLACK);
					string startStr = ""; 
					startStr  += start;
					currDataNode->SetText( startStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					currLine->AddNode(currDataNode);
					//PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					//currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
						lcurrPos += 20;
					sprintf(buff2, "%s... ", pNextIter->info.c_str());
					currDataNode->SetWhiteBlack(BLACK);
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColorBlack();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);

					//}
					
					
				}
				firstMove = false;
			}
			else
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
					currDataNode->SetWhiteBlack(WHITE);
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColorWhite();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				}
				else
				{
					sprintf(buff2, "%s.%s ", pNextIter->info.c_str(),
						pNextIter->white.c_str());

					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColorWhite();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currDataNode->SetWhiteBlack(WHITE);
				}
			}
			dc.GetTextExtent( buff2, &length, &localH, &descent );


			renderInverse = false;
			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( (white_black_move == BLACK) ||((white_black_move==WHITE)&&(move_count==0)) )
				{
					renderInverse =true;
					ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}
			
			
			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
					dc.SetTextForeground(*wxBLACK);
				}
				else
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			//currDataNode->SetWhiteBlack(WHITE);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			currLine->AddNode(currDataNode);

			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();
			
			renderInverse = false;
			int indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;


			lcurrPos += length;

			//DrawAnnotationWhite(lcurrPos, depth, height,
			//	pNextIter, dc, sz, offset);

			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, "%s ", pNextIter->result.c_str());
					// ready to break
				}
				else if( pNextIter->black != "" )
				{
					sprintf(buff2, "%s ", pNextIter->black.c_str());
				}
				else
				{
				//	sprintf(buff2, " %c", end);
					// ready to break;
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, "%s ", pNextIter->result.c_str());
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
						sprintf(buff2, "%s ", pNextIter->black.c_str());
					else
					{
						sprintf(buff2, "%s ", pNextIter->black.c_str());
					}
				}
			}
			
			dc.GetTextExtent( buff2, &length, &localH, &descent );

			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( this->white_black_move == WHITE )
				{
					renderInverse =true;
					ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}

			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
					dc.SetTextForeground(*wxBLACK);
				}
				else
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			currDataNode->SetWhiteBlack(BLACK);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColorBlack();
			currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currLine->AddNode(currDataNode);
			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			renderInverse = false;

			lcurrPos += length;

			indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;

			//DrawAnnotationBlack(lcurrPos, depth, height,
			//	pNextIter, dc, sz, offset);
			
			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					currLine->AddNode(currDataNode);
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
				}
				else
				{
					dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
					{
						dc.DrawText(wxString(end), lcurrPos, *depth);
						currDataNode->SetWhiteBlack(BLACK);
						string endStr = ""; 
						endStr  += end;
						currDataNode->SetText( endStr.c_str() );
						currDataNode->SetMovePtr(pNextIter);
						currDataNode->SetRect( lcurrPos,*depth,length,height );
						if( start == '[' )
							currDataNode->SetStyle(TEXT_DISP_NORMAL);
						else currDataNode->SetStyle(TEXT_DISP_ITALIC);
						currDataNode->SetType( BRACKET_DISP_NODE );
						PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
						currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
						currLine->AddNode(currDataNode);
						currDataNode->SetLine(currLine);
						pNextIter->SetLine(currLine);
						currDataNode = new TextDisplayNode();
					}
					//else
					//	dc.DrawText(wxString(end), lcurrPos, depth);
				}
			}

				if( pNextIter->pVarWhite )
				{

					dc.SetFont( fontItalic );
					//dc.SetPen( *wxRED);
					//dc.SetTextForeground( *wxRED);
					int cpos = lcurrPos;
					DrawVariationWhite(cpos, depth, height,
							pNextIter, dc, sz, offset+10, '(',')');
					//lcurrPos = offset;
					//depth +=height;
					if( end == ')' )
					{
						dc.SetFont( fontItalic );
					}
					else
					{
						dc.SetFont( fontNormal );
					}
					//dc.SetPen( *wxGREEN );
					//dc.SetTextForeground( *wxGREEN);
					//*depth+= height;
					if( (pNextIter->pNext != NULL) && !(pNextIter->pVarBlack) && (pNextIter != pMoveLocal) && 
						(pNextIter->pNext->info!= ""))
					{
							*depth+= height;
						m_dataPage.AddLine(currLine);
						currLine = new Line();
					}
					lcurrPos = offset+30;
					
				}

			if( pNextIter != pMoveLocal )
			{


				if( pNextIter->pVarBlack )
				{
					dc.SetFont( fontItalic );
					//dc.SetTextForeground( *wxRED);

					int cpos = offset+10;
					DrawVariationBlack(cpos, depth, height,
							pNextIter, dc, sz, offset+10, '(',')');
					//lcurrPos = offset;
					//depth +=height;
					if( end == ')' )
					{
						dc.SetFont( fontItalic );
					}
					else
					{
						dc.SetFont( fontNormal );
					}
					//dc.SetTextForeground( *wxGREEN);
					//*depth+= height;
					if( pNextIter->pNext != NULL && (pNextIter->pNext->info!= ""))
					{
							*depth+= height;
							m_dataPage.AddLine(currLine);
						currLine = new Line();
					}
					lcurrPos = offset+30;
				}

				//if( pNextIter->pVarWhite || pNextIter->pVarBlack )
					
			}




			
			
			pNextIter = pNextIter->pNext;
		}
		lcurrPos = offset;

		//*depth +=height;
		varCount++;
		pMoveLocal = pMoveLocal->pVarBlack;
	
	}
	
//	currLine->AddNode(currDataNode);
//	m_dataPage.AddLine(currLine);
//	currLine = new Line();

	return 0;
}


 int MyCanvas::DrawVariationWhite(int &currPos, int *depth, int height,
							PGN::Move *pMove, wxDC& dc, wxSize *sz, 
							int offset, char start, char end)
{
	int length;
	int descent;
	int localH;
	char buff2[512];
	//wxRichTextCtrl *m_richTextCtrl = m_owner->m_richTextCtrl;
	char annotationText[1024];
	bool renderInverse = false;
	int rectLeftOffset = 5;
	int rectRightOffset = 2;
	int varCount = 0;
	
	PGN::Move *pMoveLocal = pMove->pVarWhite;


	while( pMoveLocal )
	{
		PGN::Move *pNextIter = pMoveLocal;
		int lcurrPos = offset;
		*depth += height;
		m_dataPage.AddLine(currLine);
		currLine = new Line();
		bool firstMove = true;

		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxBOLD, false) );
		char varNum[10];
		varNum[0] = 'A'+varCount;//, varNum, 10);
		varNum[1] = '.';
		varNum[2] = 0;
	//	dc.DrawText(wxString(varNum), offset, depth);
		//dc.SetFont( wxFont(10, wxMODERN, wxNORMAL, wxNORMAL, false) );
		lcurrPos+=10;
		
		while( pNextIter && 
			 ((pNextIter->white.length() > 0)||(pNextIter->result.length()>0)) )
		{
			
			if( firstMove )
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					/*if( pNextIter->pUp->variation_count_white > 1 )
					{
						sprintf(buff2, "%s) %c%s.%s", pNextIter->variation_id, start, pNextIter->info.c_str(),
								pNextIter->white.c_str());
					}
					else
					{*/
					//sprintf(buff2, "%c ", start );
					//dc.GetTextExtent( buff2, &length, &height, &descent );
					dc.DrawText(wxString(start), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(WHITE);
					string endStr = ""; 
					endStr  += start;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
					mvCol = pNextIter->GetRenderColorWhite();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					lcurrPos += 20;

						sprintf(buff2, "%s.%s ", pNextIter->info.c_str(),
								pNextIter->white.c_str());
					//}
				}
				firstMove = false;
			}
			else
			{
				if( (pNextIter->white.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					sprintf(buff2, "%s.%s ", pNextIter->info.c_str(),
						pNextIter->white.c_str());
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColorWhite();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				}
			}

			dc.GetTextExtent( buff2, &length, &localH, &descent );


			renderInverse = false;
			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( (white_black_move == BLACK) ||((white_black_move==WHITE)&&(move_count==0)) )
				{
					renderInverse =true;
					ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}
			
			
			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
					dc.SetBrush(*wxBLACK_BRUSH);
					//if( firstMove )
					//	dc.DrawRectangle(lcurrPos-rectLeftOffset+10,depth,length+rectRightOffset,height);
					//else
						dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
					dc.SetTextForeground(*wxBLACK);
				}
				else
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			currDataNode->SetWhiteBlack(WHITE);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			currLine->AddNode(currDataNode);
			//PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
			//currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			int indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;
	//		if( firstMove )
		//		firstMove = false;



			
			renderInverse = false;


			lcurrPos += length;

			//DrawAnnotationWhite(lcurrPos, depth, height,
			//	pNextIter, dc, sz, offset);


			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					sprintf(buff2, "%s ", pNextIter->black.c_str());
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					sprintf(buff2, " %s", pNextIter->result.c_str());
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
						sprintf(buff2, "%s ", pNextIter->black.c_str());
					else
						sprintf(buff2, "%s ", pNextIter->black.c_str());
				}
			}
			


			dc.GetTextExtent( buff2, &length, &localH, &descent );

			if( pNextIter == this->m_owner->currentGame->pCurrMove )
			{
				if( this->white_black_move == WHITE )
				{
					renderInverse =true;
					ScrollTest(*depth, sz);
				}
			}

			if (lcurrPos + length > sz->GetWidth()) 
			{
				if (lcurrPos > offset) 
				{ 
					*depth += height;//*1.5;
					lcurrPos = offset+30; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}

			if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
			{
				if( renderInverse )
				{
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.DrawRectangle(lcurrPos-rectLeftOffset,*depth,length+rectRightOffset,height);
					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
					dc.SetTextForeground(*wxBLACK);
				}
				else
					dc.DrawText(wxString(buff2), lcurrPos, *depth);
			}

			currDataNode->SetWhiteBlack(BLACK);
			currDataNode->SetText( buff2 );
			currDataNode->SetMovePtr(pNextIter);
			currDataNode->SetRect( lcurrPos,*depth,length,height );
			if( start == '[' )
				currDataNode->SetStyle(TEXT_DISP_NORMAL);
			else currDataNode->SetStyle(TEXT_DISP_ITALIC);
			
			PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColorBlack();
			currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currLine->AddNode(currDataNode);
			currDataNode->SetLine(currLine);
			pNextIter->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			renderInverse = false;
			indx = 0;
			while( buff2[indx] != 0 )
				buff2[indx++]=0;


			lcurrPos += length;
			
			//DrawAnnotationBlack(lcurrPos, depth, height,
			//	pNextIter, dc, sz, offset);

			if( pNextIter->pNext == NULL )
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
				}
				else
				{
					dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
				}
			}
			else
			{
				if( (pNextIter->black.length() == 0) &&
					(pNextIter->result.length()!=0))
				{
					dc.DrawText(wxString(end), lcurrPos, *depth);
					currDataNode->SetWhiteBlack(BLACK);
					string endStr = ""; 
					endStr  += end;
					currDataNode->SetText( endStr.c_str() );
					currDataNode->SetMovePtr(pNextIter);
					currDataNode->SetRect( lcurrPos,*depth,length,height );
					if( start == '[' )
						currDataNode->SetStyle(TEXT_DISP_NORMAL);
					else currDataNode->SetStyle(TEXT_DISP_ITALIC);
					currDataNode->SetType( BRACKET_DISP_NODE );
					PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
					currLine->AddNode(currDataNode);
					currDataNode->SetLine(currLine);
					pNextIter->SetLine(currLine);
					currDataNode = new TextDisplayNode();
				}
				else
				{
					if( pNextIter->pNext->white.length() == 0 )
					{
						dc.DrawText(wxString(end), lcurrPos, *depth);
						currDataNode->SetWhiteBlack(BLACK);
						string endStr = ""; 
						endStr  += end;
						currDataNode->SetText( endStr.c_str() );
						currDataNode->SetMovePtr(pNextIter);
						currDataNode->SetRect( lcurrPos,*depth,length,height );
						if( start == '[' )
							currDataNode->SetStyle(TEXT_DISP_NORMAL);
						else currDataNode->SetStyle(TEXT_DISP_ITALIC);
						currDataNode->SetType( BRACKET_DISP_NODE );
						PGN::RenderMoveColor *mvCol = pNextIter->GetRenderColor();
						currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
						currLine->AddNode(currDataNode);
						currDataNode->SetLine(currLine);
						pNextIter->SetLine(currLine);
						currDataNode = new TextDisplayNode();
					}
					//else
					//	dc.DrawText(wxString(end), lcurrPos, depth);
				}
			}

			if( pNextIter != pMoveLocal )
			{
				if( pNextIter->pVarWhite )
				{
										
					dc.SetFont( fontItalic );
					//dc.SetPen( *wxRED);
					//dc.SetTextForeground( *wxRED);
					int cpos = lcurrPos;
					DrawVariationWhite(cpos, depth, height,
							pNextIter, dc, sz, offset+10, '(',')');
					//lcurrPos = offset;
					//depth +=height;
					if( end == ')' )
					{
						dc.SetFont( fontItalic );
					}
					else
					{
						dc.SetFont( fontNormal );
					}
					//dc.SetTextForeground( *wxGREEN);
			//		*depth+= height;
					if( (pNextIter->pNext != NULL) && ((pNextIter->pNext->info!= "")) && (!pNextIter->pVarBlack))
					{
						*depth+= height;
						m_dataPage.AddLine(currLine);
						currLine = new Line();
					}
					lcurrPos = offset+30;
				}

				//if( pNextIter->pVarWhite || pNextIter->pVarBlack )
				//	lcurrPos = offset+10;
			}

				if( pNextIter->pVarBlack )
				{
					dc.SetFont( fontItalic );
					//dc.SetPen( *wxRED);
					//dc.SetTextForeground( *wxRED);
					int cpos = offset+10;
					DrawVariationBlack(cpos, depth, height,
							pNextIter, dc, sz, offset+10, '(',')');
					//lcurrPos = offset;
					//depth +=height;
					if( end == ')' )
					{
						dc.SetFont( fontItalic );
					}
					else
					{
						dc.SetFont( fontNormal );
					}
					//dc.SetTextForeground( *wxGREEN);
					//*depth+= height;
					if( pNextIter->pNext != NULL && ((pNextIter->pNext->info!= "")))
					{
						*depth+= height;
						m_dataPage.AddLine(currLine);
						currLine = new Line();
					}
					lcurrPos = offset+30;
					
				}

			pNextIter = pNextIter->pNext;
		}
		lcurrPos = offset;
		//*depth +=height;
		varCount++;

		pMoveLocal = pMoveLocal->pVarWhite;
	}
//	currLine->AddNode(currDataNode);
//	m_dataPage.AddLine(currLine);
//	currLine = new Line();
	
	return 0;
 }


 int MyCanvas::DrawAnnotationWhite(int &currPos, int *depth, int height,
	 PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset)
 {
	int buff5len=0;
	int buff5h, buff5ext;
	for( int aa = 0; aa < pMove->annotation_white.text.size(); aa++, currPos += buff5len )
	{
			
		dc.GetTextExtent( pMove->annotation_white.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
		if (currPos + buff5len > sz->GetWidth()) 
		{
			if (currPos > 0) 
			{ 
				// newline
				*depth += height;//*1.5;
				currPos = offset+10; 
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			} 
		}
		//dc.SetFont( f1 );
		dc.SetTextForeground(*wxBLUE);
		if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
		{
			dc.DrawText(wxString(pMove->annotation_white.text[aa].c_str()), currPos, *depth);
		}
						
	}

	dc.SetTextForeground(*wxBLACK);

	return 0;
 }

  int MyCanvas::DrawAnnotationBlack(int &currPos, int *depth, int height,
	 PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset)
 {
	int buff5len=0;
	int buff5h, buff5ext;
	for( int aa = 0; aa < pMove->annotation_black.text.size(); aa++, currPos += buff5len )
	{
			
		dc.GetTextExtent( pMove->annotation_black.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
		if (currPos + buff5len > sz->GetWidth()) 
		{
			if (currPos > 0) 
			{ 
				// newline
				*depth += height;//*1.5;
				currPos = offset+10; 
				m_dataPage.AddLine(currLine);
				currLine = new Line();
			} 
		}
		//dc.SetFont( f1 );
		dc.SetTextForeground(*wxBLUE);
		if( (*depth > YY) && (*depth < sz->GetHeight() + YY))
		{
			dc.DrawText(wxString(pMove->annotation_black.text[aa].c_str()), currPos, *depth);
			//wnd10->AppendText( wxString(pMove->annotation_black.text[aa].c_str()));
		}
						
	}

	dc.SetTextForeground(*wxBLACK);

	return 0;
 }

  /*
  |
  |_______________  depth- sz->GetHeight()  scrollY
  |
  |
  |______________   sz->GetHeight()
  |
  |   X             depth
  |
  |
  |________________ 

  depth > sz->GetHeight()

  */
void MyCanvas::ScrollTest(int depth, wxSize *sz)
{
	static bool scrolled = false;
	static bool scrolledTwice = false;

	int pixPerUnitX, pixPerUnitY;
	//this->GetScro
	GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);

	//scrollX = 0;
	//scrollY = 0;
	//int distanceToScroll = depth - sz->GetHeight();
	if(!scrolled)
	{
		if( (depth  > sz->GetHeight()) )
		{
		//if( scrollY < depth - sz->GetHeight() )
			this->Scroll(0, depth / pixPerUnitY );
			scrollY++;
			//this->SetScrollPos(0, scrollY);
			scrolled = true;
		}
	}

	else if( depth  > scrollY* sz->GetHeight() )
		{
			//if( !scrolledTwice )
		//if( scrollY < depth - sz->GetHeight() )
			//this->Scroll(0, sz->GetHeight() - scrollY);
			//scrollY += sz->GetHeight();
			{
				this->Scroll(0, depth / pixPerUnitY);
				//scrolledTwice = true;
				scrollY++;
			}
			
			//this->SetScrollPos(0, scrollY);
			
		}
	else if( depth < (scrollY-1) *sz->GetHeight() )
	{
			{
				this->Scroll(0, -depth / pixPerUnitY);
				//scrolledTwice = true;
				scrollY--;
			}
	}
	/*else if( depth < scrollY + sz->GetHeight() - 20)
	{
		this->Scroll(0, - depth + sz->GetHeight() + scrollY - 20);
		scrollY = -depth + sz->GetHeight() + scrollY - 20;
	}*/
	
}
/* 
 
 */
// dave NOTE: TEXTCALL
// wxBufferedPaintDC?
void MyCanvas::DrawText(wxDC& dc)
{
	wxSize sz(GetClientSize());

	w = sz.GetWidth();
	h = sz.GetHeight();
	//CalcUnscrolledPosition	(0,0,&xx,&yy );	

	this->ScrollWindow(0,0);
    // set underlined font for testing
    dc.SetFont( fontNormal );
	char* buff = new char[360];
	int i = 0;
	char buff2[360];
	char buff3[100];
	char buff4[100];
	char buff5[360];
	
	int xx = 0;
	int yy=0;
	int curLineLength = 0;

	//wxNativeFontInfo f1Info;
	//f1Info.
	//wxFONTWEIGHT_MAX
						//wxFONTSTYLE_ITALIC
	wxFont f1(fontBold);

	m_dataPage.Clear();

	currLine = new Line();
	currDataNode = new TextDisplayNode();

	
	int down = 10;
	PGN::Move* pMove;
	if( m_owner->currentGame != NULL )
	{
		pMove= this->m_owner->currentGame->pRoot;
		
		int length;
		int height;
		int descent;

		dc.SetFont( f1);
		dc.GetTextExtent( "Some Text", &length, &height, &descent );
		while ( pMove )
		{
		


			if( pMove == NULL )
				break;

			dc.SetFont( f1);



			sprintf(buff2, "%s. %s %s",pMove->info.c_str(),
				pMove->white.c_str(),
				pMove->black.c_str());


			


			
			if( pMove->atEndBlack && pMove->result != "" )
			{
				sprintf( buff3, " %s", pMove->result.c_str());
			}
			else
			{
				if( pMove->white == "" )
				{
					sprintf( buff3, "%s... ", pMove->info.c_str());
					PGN::RenderMoveColor *mvCol = pMove->GetRenderColorBlack();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				}
				else
				{
					sprintf( buff3, "%s.%s ", pMove->info.c_str(),
					pMove->white.c_str());
					PGN::RenderMoveColor *mvCol = pMove->GetRenderColorWhite();
					currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
				}
			}


			int buff3len, buff3h, buff3ext;
			dc.GetTextExtent( buff3, &buff3len, &buff3h, &buff3ext );

			line_height = buff3h;

			if (curLineLength + buff3len > sz.GetWidth()) 
			{
				if (curLineLength > 0) 
				{ 
					// newline
					down += height;//*1.5;
					curLineLength = 0; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 
			}

			bool renderInverse = false;
		
			if( pMove == this->m_owner->currentGame->pCurrMove )
			{
				if( (white_black_move == BLACK) ||((white_black_move==WHITE)&&(move_count==0)) )
				{
					renderInverse =true;
					ScrollTest(down, &sz);
				}
			}

		
			int rectLeftOffset = 5;
			int rectRightOffset = 2;

			if( (down > YY) && (down < sz.GetHeight() + YY))
			{
				if( renderInverse )
				{
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.DrawRectangle(curLineLength-rectLeftOffset,down,buff3len+rectRightOffset,buff3h);
					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(wxString(buff3), curLineLength, down);
			
				}
				else
					dc.DrawText(wxString(buff3), curLineLength, down);
			}

			currDataNode->SetWhiteBlack(WHITE);
			currDataNode->SetText( buff3 );
			currDataNode->SetMovePtr(pMove);
			currDataNode->SetRect( curLineLength,down,buff3len,buff3h );
			currDataNode->SetStyle(TEXT_DISP_BOLD);
			currLine->AddNode(currDataNode);
			//PGN::RenderMoveColor *mvCol = pMove->GetRenderColor();
			//currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			currDataNode->SetLine(currLine);
			pMove->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			renderInverse =false;

			curLineLength+= buff3len;
			/*int buff5len=0;
			int buff5h, buff5ext;
			for( int aa = 0; aa < pMove->annotation_white.text.size(); aa++, curLineLength += buff5len )
			{
			
				dc.GetTextExtent( pMove->annotation_white.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
				if (curLineLength + buff5len > sz.GetWidth()) 
				{
					if (curLineLength > 0) 
					{ 
						// newline
						down += height;//*1.5;
						curLineLength = 0; 
					} 
				}
				dc.SetFont( f1 );
				dc.SetTextForeground(*wxBLUE);
				dc.DrawText(wxString(pMove->annotation_white.text[aa].c_str()), curLineLength, down);
						
			}*/

			//DrawAnnotationWhite(curLineLength, &down, height,
			//	pMove, dc, &sz, 0);


	//		sprintf( buff5, "%s", pMove->text_white);

		
			//dc.GetTextExtent( buff5, &buff5len, &buff5h, &buff5ext );



			dc.SetFont( f1 );
			dc.SetTextForeground(*wxBLACK);

			int buff4len, buff4h, buff4ext;

			sprintf( buff4, "%s ",
				pMove->black.c_str());

			dc.GetTextExtent( buff4, &buff4len, &buff4h, &buff4ext );

		

			if( pMove == this->m_owner->currentGame->pCurrMove )
			{
				if( this->white_black_move == WHITE )
				{
					renderInverse =true;
					ScrollTest(down, &sz);
				}
			}

			//if( (pMove->pNext) && (pMove->pNext)->atEndBlack )
			//	renderInverse =true;



			if (curLineLength + buff4len > sz.GetWidth()) 
			{
				if (curLineLength > 0) 
				{ 
						// newline
					down += height;//*1.5;
					curLineLength = 0; 
					m_dataPage.AddLine(currLine);
					currLine = new Line();
				} 			
			}

			if( (down > YY) && (down < sz.GetHeight() + YY))
			{
				if( renderInverse )
				{
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.DrawRectangle(curLineLength-rectLeftOffset,down,buff4len+rectRightOffset,buff4h);
					dc.SetTextForeground(*wxWHITE);
					dc.DrawText(wxString(buff4), curLineLength, down);
				}
				else
					dc.DrawText(wxString(buff4), curLineLength, down);
			}

			currDataNode->SetWhiteBlack(BLACK);
			currDataNode->SetText( buff4 );
			currDataNode->SetMovePtr(pMove);
			currDataNode->SetRect( curLineLength,down,buff4len,buff4h );
			currLine->AddNode(currDataNode);
			currDataNode->SetLine(currLine);
			currDataNode->SetStyle(TEXT_DISP_BOLD);
			PGN::RenderMoveColor *mvCol = pMove->GetRenderColorBlack();
			currDataNode->SetColour(mvCol->r, mvCol->g, mvCol->b);
			pMove->SetLine(currLine);
			currDataNode = new TextDisplayNode();

			curLineLength += buff4len;

			/*for( int aa = 0; aa < pMove->annotation_black.text.size(); aa++, curLineLength += buff5len )
			{
			
				dc.GetTextExtent( pMove->annotation_black.text[aa].c_str(), &buff5len, &buff5h, &buff5ext );
				if (curLineLength + buff5len > sz.GetWidth()) 
				{
					if (curLineLength > 0) 
					{ 
						// newline
						down += height;//*1.5;
						curLineLength = 0; 
					} 
				}
				dc.SetFont( f1 );
				dc.SetTextForeground(*wxBLUE);
				dc.DrawText(wxString(pMove->annotation_black.text[aa].c_str()), curLineLength, down);
			
			
			}*/
			//DrawAnnotationBlack(curLineLength, &down, height,
			//	pMove, dc, &sz, 0);
//			curLineLength += buff5len;
		
		
			dc.SetTextForeground(*wxBLACK);

			DrawVariation( curLineLength, &down, height, pMove, dc, &sz );
		
			needsRefresh = false;
			pMove = pMove->pNext;
			i++;
		}
		currLine->AddNode(currDataNode);
		m_dataPage.AddLine(currLine);
		currLine = new Line();


		//m_dataPage.MergeEqualNodesInLines();
	}

}




static const struct
{
    const wxChar *name;
    int           rop;
} rasterOperations[] =
{
    { wxT("wxAND"),          wxAND           },
    { wxT("wxAND_INVERT"),   wxAND_INVERT    },
    { wxT("wxAND_REVERSE"),  wxAND_REVERSE   },
    { wxT("wxCLEAR"),        wxCLEAR         },
    { wxT("wxCOPY"),         wxCOPY          },
    { wxT("wxEQUIV"),        wxEQUIV         },
    { wxT("wxINVERT"),       wxINVERT        },
    { wxT("wxNAND"),         wxNAND          },
    { wxT("wxNO_OP"),        wxNO_OP         },
    { wxT("wxOR"),           wxOR            },
    { wxT("wxOR_INVERT"),    wxOR_INVERT     },
    { wxT("wxOR_REVERSE"),   wxOR_REVERSE    },
    { wxT("wxSET"),          wxSET           },
    { wxT("wxSRC_INVERT"),   wxSRC_INVERT    },
    { wxT("wxXOR"),          wxXOR           },
};

void MyCanvas::DrawImages(wxDC& dc)
{
    dc.DrawText(_T("original image"), 0, 0);
//    dc.DrawBitmap(*gs_bmpNoMask, 0, 20, 0);
    dc.DrawText(_T("with colour mask"), 0, 100);
//    dc.DrawBitmap(*gs_bmpWithColMask, 0, 120, true);
    dc.DrawText(_T("the mask image"), 0, 200);
//    dc.DrawBitmap(*gs_bmpMask, 0, 220, 0);
    dc.DrawText(_T("masked image"), 0, 300);
//    dc.DrawBitmap(*gs_bmpWithMask, 0, 320, true);

//    int cx = gs_bmpWithColMask->GetWidth(),
  //      cy = gs_bmpWithColMask->GetHeight();

    wxMemoryDC memDC;
    for ( size_t n = 0; n < WXSIZEOF(rasterOperations); n++ )
    {
        wxCoord x = 120 + 150*(n%4),
                y =  20 + 100*(n/4);

        dc.DrawText(rasterOperations[n].name, x, y - 20);
//        memDC.SelectObject(*gs_bmpWithColMask);
   //     dc.Blit(x, y, cx, cy, &memDC, 0, 0, rasterOperations[n].rop, true);
    }
}


#if wxUSE_GRAPHICS_CONTEXT
#ifdef __WXGTK20__
void MyCanvas::DrawAlpha(wxDC& no_dc)
#else
void MyCanvas::DrawAlpha(wxDC& dc)
#endif
{
#ifdef __WXGTK__
    wxGCDC dc( this );
    PrepareDC( dc );
#endif

    wxDouble margin = 20 ;
    wxDouble width = 180 ;
    wxDouble radius = 30 ;
    
    dc.SetPen( wxPen( wxColour( 128, 0, 0, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 255, 0, 0, 255),wxSOLID));
    
    wxRect r(margin,margin+width*0.66,width,width) ;
    
    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;
    
    dc.SetPen( wxPen( wxColour( 0, 0, 128, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 0, 0, 255, 255),wxSOLID));
    
    r.Offset( width * 0.8 , - width * 0.66 ) ;
    
    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;
    
    dc.SetPen( wxPen( wxColour( 128, 128, 0, 255 ),12, wxSOLID));
    dc.SetBrush( wxBrush( wxColour( 192, 192, 0, 255),wxSOLID));

    r.Offset( width * 0.8 , width *0.5 ) ;
    
    dc.DrawRoundedRectangle( r.x, r.y, r.width, r.width, radius ) ;
    
    dc.SetPen( *wxTRANSPARENT_PEN ) ;
    dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
    dc.DrawRoundedRectangle( 0 , margin + width / 2 , width * 3 , 100 , radius) ;
    
    dc.SetTextForeground( wxColour(255,255,0,128) );
    dc.SetFont( wxFont( 40, wxFONTFAMILY_SWISS, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL ) );
    dc.DrawText( wxT("Hello!"), 120, 80 );
}

#endif

void MyCanvas::DrawCircles(wxDC& dc)
{
    int x = 100,
        y = 100,
        r = 20;

    dc.SetPen( *wxRED_PEN );
    dc.SetBrush( *wxGREEN_BRUSH );

    dc.DrawText(_T("Some circles"), 0, y);
    dc.DrawCircle(x, y, r);
    dc.DrawCircle(x + 2*r, y, r);
    dc.DrawCircle(x + 4*r, y, r);

    y += 2*r;
    dc.DrawText(_T("And ellipses"), 0, y);
    dc.DrawEllipse(x - r, y, 2*r, r);
    dc.DrawEllipse(x + r, y, 2*r, r);
    dc.DrawEllipse(x + 3*r, y, 2*r, r);

    y += 2*r;
    dc.DrawText(_T("And arcs"), 0, y);
    dc.DrawArc(x - r, y, x + r, y, x, y);
    dc.DrawArc(x + 4*r, y, x + 2*r, y, x + 3*r, y);
    dc.DrawArc(x + 5*r, y, x + 5*r, y, x + 6*r, y);

    y += 2*r;
    dc.DrawEllipticArc(x - r, y, 2*r, r, 0, 90);
    dc.DrawEllipticArc(x + r, y, 2*r, r, 90, 180);
    dc.DrawEllipticArc(x + 3*r, y, 2*r, r, 180, 270);
    dc.DrawEllipticArc(x + 5*r, y, 2*r, r, 270, 360);
    
    // same as above, just transparent brush
    
    dc.SetPen( *wxRED_PEN );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );

    y += 2*r;
    dc.DrawText(_T("Some circles"), 0, y);
    dc.DrawCircle(x, y, r);
    dc.DrawCircle(x + 2*r, y, r);
    dc.DrawCircle(x + 4*r, y, r);

    y += 2*r;
    dc.DrawText(_T("And ellipses"), 0, y);
    dc.DrawEllipse(x - r, y, 2*r, r);
    dc.DrawEllipse(x + r, y, 2*r, r);
    dc.DrawEllipse(x + 3*r, y, 2*r, r);

    y += 2*r;
    dc.DrawText(_T("And arcs"), 0, y);
    dc.DrawArc(x - r, y, x + r, y, x, y);
    dc.DrawArc(x + 4*r, y, x + 2*r, y, x + 3*r, y);
    dc.DrawArc(x + 5*r, y, x + 5*r, y, x + 6*r, y);

    y += 2*r;
    dc.DrawEllipticArc(x - r, y, 2*r, r, 0, 90);
    dc.DrawEllipticArc(x + r, y, 2*r, r, 90, 180);
    dc.DrawEllipticArc(x + 3*r, y, 2*r, r, 180, 270);
    dc.DrawEllipticArc(x + 5*r, y, 2*r, r, 270, 360);
    
}

void MyCanvas::DrawSplines(wxDC& dc)
{
#if wxUSE_SPLINES
    dc.DrawText(_T("Some splines"), 10, 5);

    // values are hardcoded rather than randomly generated
    // so the output can be compared between native
    // implementations on platforms with different random
    // generators

    const int R = 300;
    const wxPoint center( R + 20, R + 20 );
    const int angles[7] = { 0, 10, 33, 77, 13, 145, 90 };
    const int radii[5] = { 100 , 59, 85, 33, 90 };
    const int n = 200;
    wxPoint pts[n];

    // background spline calculation
    unsigned int radius_pos = 0;
    unsigned int angle_pos = 0;
    int angle = 0;
    for ( int i = 0; i < n; i++ )
    {
        angle += angles[ angle_pos ];
        int r = R * radii[ radius_pos ] / 100;
        pts[ i ].x = center.x + (wxCoord)( r * cos( M_PI * angle / 180.0) );
        pts[ i ].y = center.y + (wxCoord)( r * sin( M_PI * angle / 180.0) );

        angle_pos++;
        if ( angle_pos >= WXSIZEOF(angles) ) angle_pos = 0;

        radius_pos++;
        if ( radius_pos >= WXSIZEOF(radii) ) radius_pos = 0;
    }

    // background spline drawing
    dc.SetPen(*wxRED_PEN);
    dc.DrawSpline(WXSIZEOF(pts), pts);

    // less detailed spline calculation
    wxPoint letters[4][5];
    // w
    letters[0][0] = wxPoint( 0,1); //  O           O
    letters[0][1] = wxPoint( 1,3); //   *         *
    letters[0][2] = wxPoint( 2,2); //    *   O   *
    letters[0][3] = wxPoint( 3,3); //     * * * *
    letters[0][4] = wxPoint( 4,1); //      O   O
    // x1
    letters[1][0] = wxPoint( 5,1); //  O*O
    letters[1][1] = wxPoint( 6,1); //     *
    letters[1][2] = wxPoint( 7,2); //      O
    letters[1][3] = wxPoint( 8,3); //       *
    letters[1][4] = wxPoint( 9,3); //        O*O
    // x2
    letters[2][0] = wxPoint( 5,3); //        O*O
    letters[2][1] = wxPoint( 6,3); //       *
    letters[2][2] = wxPoint( 7,2); //      O
    letters[2][3] = wxPoint( 8,1); //     *
    letters[2][4] = wxPoint( 9,1); //  O*O
    // W
    letters[3][0] = wxPoint(10,0); //  O           O
    letters[3][1] = wxPoint(11,3); //   *         *
    letters[3][2] = wxPoint(12,1); //    *   O   *
    letters[3][3] = wxPoint(13,3); //     * * * *
    letters[3][4] = wxPoint(14,0); //      O   O

    const int dx = 2 * R / letters[3][4].x;
    const int h[4] = { -R/2, 0, R/4, R/2 };

    for ( int m = 0; m < 4; m++ )
    {
        for ( int n = 0; n < 5; n++ )
        {
            letters[m][n].x = center.x - R + letters[m][n].x * dx;
            letters[m][n].y = center.y + h[ letters[m][n].y ];
        }

        dc.SetPen( wxPen( wxT("blue"), 1, wxDOT) );
        dc.DrawLines(5, letters[m]);
        dc.SetPen( wxPen( wxT("black"), 4, wxSOLID) );
        dc.DrawSpline(5, letters[m]);
    }

#else
    dc.DrawText(_T("Splines not supported."), 10, 5);
#endif
}

void MyCanvas::DrawGradients(wxDC& dc)
{
    static const int TEXT_HEIGHT = 15;

    // LHS: linear
    wxRect r(10, 10, 50, 50);
    dc.DrawText(_T("wxRIGHT"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxRIGHT);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("wxLEFT"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxLEFT);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("wxDOWN"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxDOWN);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("wxUP"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillLinear(r, *wxWHITE, *wxBLUE, wxUP);


    // RHS: concentric
    r = wxRect(200, 10, 50, 50);
    dc.DrawText(_T("Blue inside"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("White inside"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxWHITE, *wxBLUE);

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("Blue in top left corner"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE, wxPoint(0, 0));

    r.Offset(0, r.height + 10);
    dc.DrawText(_T("Blue in bottom right corner"), r.x, r.y);
    r.Offset(0, TEXT_HEIGHT);
    dc.GradientFillConcentric(r, *wxBLUE, *wxWHITE, wxPoint(r.width, r.height));
}

void MyCanvas::DrawRegions(wxDC& dc)
{
    dc.DrawText(_T("You should see a red rect partly covered by a cyan one ")
                _T("on the left"), 10, 5);
    dc.DrawText(_T("and 5 smileys from which 4 are partially clipped on the right"),
                10, 5 + dc.GetCharHeight());
    dc.DrawText(_T("The second copy should be identical but right part of it ")
                _T("should be offset by 10 pixels."),
                10, 5 + 2*dc.GetCharHeight());

    DrawRegionsHelper(dc, 10, true);
    DrawRegionsHelper(dc, 350, false);
}

void MyCanvas::DrawRegionsHelper(wxDC& dc, wxCoord x, bool firstTime)
{
    wxCoord y = 100;

    dc.DestroyClippingRegion();
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.SetPen( *wxTRANSPARENT_PEN );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.SetClippingRegion( x + 10, y + 10, 100, 270 );

    dc.SetBrush( *wxRED_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.SetClippingRegion( x + 10, y + 10, 100, 100 );

    dc.SetBrush( *wxCYAN_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

    dc.DestroyClippingRegion();

    wxRegion region(x + 110, y + 20, 100, 270);
#if !defined(__WXMOTIF__) && !defined(__WXMAC__)
    if ( !firstTime )
        region.Offset(10, 10);
#endif
    dc.SetClippingRegion(region);

    dc.SetBrush( *wxGREY_BRUSH );
    dc.DrawRectangle( x, y, 310, 310 );

   /* if (m_smile_bmp.Ok())
    {
        dc.DrawBitmap( m_smile_bmp, x + 150, y + 150, true );
        dc.DrawBitmap( m_smile_bmp, x + 130, y + 10,  true );
        dc.DrawBitmap( m_smile_bmp, x + 130, y + 280, true );
        dc.DrawBitmap( m_smile_bmp, x + 100, y + 70,  true );
        dc.DrawBitmap( m_smile_bmp, x + 200, y + 70,  true );
    }*/
}

void MyCanvas::OnPaint(wxPaintEvent &event)
{
	m_useContext = false;
    wxPaintDC pdc(this);
	wxSize sz(GetClientSize());
#if wxUSE_GRAPHICS_CONTEXT
     wxGCDC gdc( pdc ) ;
    wxDC &dc = m_useContext ? (wxDC&) gdc : (wxDC&) pdc ;
#else
    wxDC &dc = pdc ;
#endif

    PrepareDC(dc);

    m_owner->PrepareDC(dc);

  //  dc.SetBackgroundMode( m_owner->m_backgroundMode );
//    if ( m_owner->m_backgroundBrush.Ok() )
  //      dc.SetBackground( m_owner->m_backgroundBrush );
 //   if ( m_owner->m_colourForeground.Ok() )
 //       dc.SetTextForeground( m_owner->m_colourForeground );
  //  if ( m_owner->m_colourBackground.Ok() )
  //      dc.SetTextBackground( m_owner->m_colourBackground );

  /*  if ( m_owner->m_textureBackground) {
        if ( ! m_owner->m_backgroundBrush.Ok() ) {
            wxColour clr(0,128,0);
            wxBrush b(clr, wxSOLID);
            dc.SetBackground(b);
        }
    }*/

    /*if ( m_clip )
	{
		
		CalcUnscrolledPosition	(0,0,&XX,&YY );	
		dc.SetClippingRegion(XX, YY, sz.GetWidth(), sz.GetHeight());
	}*/
	//dc.
    dc.Clear();

/*    if ( m_owner->m_textureBackground )
    {
        dc.SetPen(*wxMEDIUM_GREY_PEN);
        for ( int i = 0; i < 200; i++ )
            dc.DrawLine(0, i*10, i*10, 0);
    }*/
	m_show = Show_Text;

	if( needsRefresh )
	{
		switch ( m_show )
		{
			case Show_Default:
				DrawDefault(dc);
				break;

			case Show_Circles:
				DrawCircles(dc);
				break;

			case Show_Splines:
				DrawSplines(dc);
				break;

			case Show_Regions:
				DrawRegions(dc);
				break;

			case Show_Text:
				DrawText(dc);
				Refresh(true);
				//DrawGraphics(gdc.GetGraphicsContext());
				break;

			case Show_Lines:
				DrawTestLines( 0, 100, 0, dc );
				DrawTestLines( 0, 320, 1, dc );
				DrawTestLines( 0, 540, 2, dc );
				DrawTestLines( 0, 760, 6, dc );
				break;

			case Show_Brushes:
				DrawTestBrushes(dc);
				break;

			case Show_Polygons:
				DrawTestPoly(dc);
				break;

			case Show_Mask:
				DrawImages(dc);
				break;

			case Show_Ops:
			  //  DrawWithLogicalOps(dc);
				break;
        
	#if wxUSE_GRAPHICS_CONTEXT
			case Show_Alpha:
				DrawAlpha(dc);
				break;
	#endif

			case Show_Gradient:
				DrawGradients(dc);
				break;

			default:
				break;
		}
	}
	else
	{
#ifdef USE_GRAPHICS_CONTEXT
wxGraphicsContext *gc = gdc.GetGraphicsContext();
		Scroll(0, 0);
		wxFont f3(10, wxMODERN, wxITALIC,wxNORMAL, false/*,"Arial Baltic"*/);
		wxFont f2(10, wxMODERN, wxNORMAL,wxBOLD, false/*,"Arial Baltic"*/);
		wxFont f1(10, wxMODERN, wxNORMAL,wxNORMAL, false/*,"Arial Baltic"*/);
		//wxGraphicsFont f1;
		gc->SetFont(f1, *wxBLACK);
		gc->SetPen(*wxBLACK);

		int fontHeight; 
		int fontWidth;
		int depth;

		wxSize textSize = dc.GetTextExtent("ABC");
		

		int lines_per_page = sz.GetHeight() / textSize.GetHeight();
		int pixPerUnitX;
		int pixPerUnitY;
		GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);
		
		int localXX, localYY;
		CalcUnscrolledPosition( 0,0, &localXX, &localYY);	
		
		int start = 0;
		int lineY = 0;
		// scroll here
		for( int i = 0; i < m_dataPage.lines.size(); i++ )
		{
			if( m_dataPage.lines[i] == m_owner->currentGame->pCurrMove->m_line )
			{
				start = i;
				lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
				break;
			}
		}
		if( lineY-localYY > sz.GetHeight() - textSize.GetHeight() )
		{
			Scroll(0, lineY/pixPerUnitY);
		}
		else if( localYY == 0 )
		{
			start = 0;
		}
		else 
		{
			for( int i = 0; i < m_dataPage.lines.size(); i++ )
			{
				if( m_dataPage.lines[i]->nodes[0]->m_rect.y == localYY )
				{
					start = i;
					//lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
					break;
				}
			}
		}

		for( int i = start; i < m_dataPage.lines.size(); i++ )
		{
			if( m_dataPage.lines[i]->nodes.size() > 0 )
			{
				if( m_dataPage.lines[i]->nodes[0]->m_rect.y > lineY + sz.GetHeight() )
				{
					break;
				}
			}
			for( int j = 0; j < m_dataPage.lines[i]->nodes.size(); j++ )
			{
				if( m_dataPage.lines[i]->nodes[j]->GetStyle() ==  TEXT_DISP_NORMAL )
				{
				//	dc.SetFont(f1);
					gc->SetFont(f1, *wxBLACK);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_BOLD ) 
				{
				//	dc.SetFont(f2);
					gc->SetFont(f2, *wxBLACK);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_ITALIC )
				{
				//	dc.SetFont(f3);
					gc->SetFont(f3, *wxBLACK);
				}

				if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == BLACK &&
					 (m_owner->board_canvas->GetBoard()->mov == WHITE || 
						m_owner->board_canvas->GetBoard()->mov == 2 )) &&
						(m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
/*					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack == WHITE && 
						(m_owner->board_canvas->GetBoard()->mov == WHITE || 
						m_owner->board_canvas->GetBoard()->mov == 2 )) &&
					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK && 
						(m_owner->board_canvas->GetBoard()->mov != BLACK)))) ||
					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack != WHITE && 
						(m_owner->board_canvas->GetBoard()->mov != WHITE || 
						m_owner->board_canvas->GetBoard()->mov != 2 )) &&
					((m_dataPage.lines[i]->nodes[j]->bWhiteBlack == BLACK && 
						(m_owner->board_canvas->GetBoard()->mov == BLACK)))))*/
				{
					int rectLeftOffset = 5;
					int rectRightOffset = 2;
					gc->SetBrush(*wxBLACK_BRUSH);
					gc->DrawRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height);

					gc->SetPen(*wxWHITE);
					gc->DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y);
					gc->SetPen(*wxBLACK);
				}
				else if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == WHITE &&
					 (m_owner->board_canvas->GetBoard()->mov == BLACK )) &&
					 (m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
				{
					int rectLeftOffset = 5;
					int rectRightOffset = 2;
					gc->SetBrush(*wxBLACK_BRUSH);
					gc->DrawRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height);

					gc->SetPen(*wxWHITE);
					gc->DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y);
					gc->SetPen(*wxBLACK);
				}
				else
				{
					gc->DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
						m_dataPage.lines[i]->nodes[j]->m_rect.x, 
						m_dataPage.lines[i]->nodes[j]->m_rect.y);
				}
			}
		}
#endif

#ifndef USE_GRAPHICS_CONTEXT
		wxGraphicsContext *gc = gdc.GetGraphicsContext();
		//Scroll(0, 0);
		int localXX, localYY;
		CalcUnscrolledPosition( 0,0, &localXX, &localYY);	
		
		wxFont tf3(this->fontSize, wxMODERN, wxNORMAL,wxNORMAL, false,"DiagramTTFritz");
		//wxFont tf2(10, wxMODERN, wxNORMAL,wxBOLD, false,"Times New Roman");
		//wxFont tf1(10, wxMODERN, wxNORMAL,wxNORMAL, false,"Times New Roman");
		dc.SetFont(fontNormal);

		int fontHeight; 
		int fontWidth;
		int depth;

		wxSize textSize = dc.GetTextExtent("A");
		

		int lines_per_page = sz.GetHeight() / textSize.GetHeight();
		int pixPerUnitX;
		int pixPerUnitY;
		GetScrollPixelsPerUnit(&pixPerUnitX, &pixPerUnitY);
		

		
		int start = 0;
		int lineY = 0;
		// scroll here


		if( MoveToCursor )
		{
			MoveToCursor = false;
			for( int i = 0; i < m_dataPage.lines.size(); i++ )
			{
				if( m_dataPage.lines[i] == m_owner->currentGame->pCurrMove->m_line &&
					m_dataPage.lines[i]->nodes.size() != 0 )

				{
					start = i;
					lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
					break;
				}
			}
			if( lineY-localYY > sz.GetHeight() - textSize.GetHeight()*2 )
			{

				Scroll(0, lineY/pixPerUnitY);
				/*for( int i = 0; i < m_dataPage.lines.size(); i++ )
				{
					if( (m_dataPage.lines[i]->nodes[0]->m_rect.y > localYY-10) && 
						(m_dataPage.lines[i]->nodes[0]->m_rect.y < localYY+10))
					{
						start = i;
						//lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
						break;
					}
				}*/

			}
			else if (lineY < localYY)
			{
				Scroll(0, 0);
				Scroll(0, lineY/pixPerUnitY);
				//start = 0;
			}
			else if( localYY == 0 )
			{
				start = 0;
			}	
			else
			{
				for( int i = 0; i < m_dataPage.lines.size(); i++ )
				{
					if(m_dataPage.lines[i]->nodes.size() == 0)
						continue;

					if( (m_dataPage.lines[i]->nodes[0]->m_rect.y > localYY-textSize.GetHeight()) && 
						(m_dataPage.lines[i]->nodes[0]->m_rect.y < localYY+textSize.GetHeight()))
					{
						start = i;
						//lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
						break;
					}
				}
			}
		}
		else
		{
			for( int i = 0; i < m_dataPage.lines.size(); i++ )
			{
				if(m_dataPage.lines[i]->nodes.size() == 0)
					continue;
				if( (m_dataPage.lines[i]->nodes[0]->m_rect.y >= localYY-textSize.GetHeight()) && 
					(m_dataPage.lines[i]->nodes[0]->m_rect.y <= localYY+textSize.GetHeight()))
				{
					start = i;
					lineY=m_dataPage.lines[i]->nodes[0]->m_rect.y;
					break;
				}
			}
		}
		for( int i = start; i < m_dataPage.lines.size(); i++ )
		{
			if( m_dataPage.lines[i]->nodes.size() > 0 )
			{
				if( m_dataPage.lines[i]->nodes[0]->m_rect.y > lineY + sz.GetHeight() )
				{
					break;
				}
			}
			else continue;

//			CodeFold *cfNode = NULL;
//			bool in_code_fold = false;
			for( int j = 0; j < m_dataPage.lines[i]->nodes.size(); j++ )
			{


				if( m_dataPage.lines[i]->nodes[j]->GetStyle() ==  TEXT_DISP_NORMAL )
				{
					dc.SetFont(fontNormal);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_BOLD ) 
				{
					dc.SetFont(fontBold);
				}
				else if( m_dataPage.lines[i]->nodes[j]->GetStyle() == TEXT_DISP_ITALIC )
				{
					dc.SetFont(fontItalic);
				}


				if( m_dataPage.lines[i]->nodes[j]->GetType() == CODE_FOLD_DISP_NODE )
				{
					if( m_dataPage.lines[i]->nodes[j]->GetState() == 1 )
					{
						dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
						dc.SetBrush( wxBrush( wxColour( 100, 255, 100, 128),wxSOLID));
						//cfNode = (CodeFold*)m_dataPage.lines[i]->nodes[j];
						//in_code_fold = true;
					}
					else
					{
						dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
						dc.SetBrush( wxBrush( wxColour( 255, 100, 100, 128),wxSOLID));
						//cfNode = NULL;
						//in_code_fold = false;
					}

					dc.DrawRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x + m_dataPage.lines[i]->nodes[j]->m_rect.height/4,
									m_dataPage.lines[i]->nodes[j]->m_rect.y + m_dataPage.lines[i]->nodes[j]->m_rect.height/4,
									m_dataPage.lines[i]->nodes[j]->m_rect.height/2,
									m_dataPage.lines[i]->nodes[j]->m_rect.height/2

									);
				}
				else if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == BLACK &&
					 (m_owner->board_canvas->GetBoard()->mov == WHITE || 
						m_owner->board_canvas->GetBoard()->mov == 2 )) &&
						(m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
				{
					int rectLeftOffset = textSize.GetWidth()/4;
					int rectRightOffset = 0;//textSize.GetWidth()/4;
					//dc.SetBrush(*wxBLACK_BRUSH);
					//dc.DrawRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.y,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.height);
					dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
					dc.SetBrush( wxBrush( wxColour( 100, 100, 255, 128),wxSOLID));
					dc.DrawRoundedRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height,2);
					//dc.SetPen( *wxTRANSPARENT_PEN ) ;
					//dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
					//dc.DrawRoundedRectangle( m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset+1,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.y+1,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset-2,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.height-2,1 ) ;

					dc.SetTextForeground(*wxWHITE);
					if( m_owner->optionsFrame->m_NotationOptions.usePiecesInNotation == true )
					{
						int twidth;
						int theight;
						wxPoint firstPoint = wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
							m_dataPage.lines[i]->nodes[j]->m_rect.y);
						if( m_dataPage.lines[i]->nodes[j]->text.length() > 0 )
						switch( m_dataPage.lines[i]->nodes[j]->text[0] )
						{
							
						case 'N':
							{
								wxFont oldFont = dc.GetFont();
								dc.SetFont(tf3);
								dc.GetTextExtent("n", &twidth, &theight);
								dc.DrawText("n", firstPoint);
								int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
								dc.SetFont(oldFont);
								dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
									wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
											m_dataPage.lines[i]->nodes[j]->m_rect.y));
							}
							break;
						case 'B':
							{
								wxFont oldFont = dc.GetFont();
								dc.SetFont(tf3);
								dc.GetTextExtent("l", &twidth, &theight);
								dc.DrawText("l", firstPoint);
								int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
								dc.SetFont(oldFont);
								dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
									wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
											m_dataPage.lines[i]->nodes[j]->m_rect.y));
							}
							break;
						case 'R':
							{
								wxFont oldFont = dc.GetFont();
								dc.SetFont(tf3);
								dc.GetTextExtent("r", &twidth, &theight);
								dc.DrawText("r", firstPoint);
								int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
								dc.SetFont(oldFont);
								dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
									wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
											m_dataPage.lines[i]->nodes[j]->m_rect.y));
							}
							break;
						case 'Q':
							{
								wxFont oldFont = dc.GetFont();
								dc.SetFont(tf3);
								dc.GetTextExtent("q", &twidth, &theight);
								dc.DrawText("q", firstPoint);
								int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
								dc.SetFont(oldFont);
								dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
									wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
											m_dataPage.lines[i]->nodes[j]->m_rect.y));
							}
							break;
						case 'K':
							{
								wxFont oldFont = dc.GetFont();
								dc.SetFont(tf3);
								dc.GetTextExtent("k", &twidth, &theight);
								dc.DrawText("k", firstPoint);
								int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
								dc.SetFont(oldFont);
								dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
									wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
											m_dataPage.lines[i]->nodes[j]->m_rect.y));
							}
							break;
						default:
							{
								dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
									wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
									m_dataPage.lines[i]->nodes[j]->m_rect.y));
							}
							break;
						};
					}
					else
					{
						dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
							wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
							m_dataPage.lines[i]->nodes[j]->m_rect.y));
					}
					dc.SetTextForeground(*wxBLACK);
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.SetPen(*wxBLACK_PEN);
				}
				else if( (m_dataPage.lines[i]->nodes[j]->pMove == m_owner->currentGame->pCurrMove)&&
					(m_dataPage.lines[i]->nodes[j]->bWhiteBlack == WHITE &&
					 (m_owner->board_canvas->GetBoard()->mov == BLACK )) &&
					 (m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE))
				{
					int rectLeftOffset = textSize.GetWidth()/4;
					int rectRightOffset = 0;//textSize.GetWidth()/4;
					//dc.SetBrush(*wxBLACK_BRUSH);
					dc.SetPen( wxPen( wxColour( 0, 0, 128, 128 ),1, wxSOLID));
					dc.SetBrush( wxBrush( wxColour( 100, 100, 255, 128),wxSOLID));
					//dc.DrawRoundedRectangle( r2.x, r2.y, r2.width, r2.height, 2 ) ;
					dc.DrawRoundedRectangle(m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.y,
						m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset,
						m_dataPage.lines[i]->nodes[j]->m_rect.height,2);

					//dc.SetPen( *wxTRANSPARENT_PEN ) ;
					//dc.SetBrush( wxBrush( wxColour(255,255,128,128) ) );
					//dc.DrawRoundedRectangle( m_dataPage.lines[i]->nodes[j]->m_rect.x-rectLeftOffset+1,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.y+1,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.width+rectRightOffset-2,
					//	m_dataPage.lines[i]->nodes[j]->m_rect.height-2,1 ) ;

					dc.SetTextForeground(*wxWHITE);
					if( m_owner->optionsFrame->m_NotationOptions.usePiecesInNotation == true )
					{
						int twidth;
						int theight;
						wxPoint firstPoint = wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
							m_dataPage.lines[i]->nodes[j]->m_rect.y);
						if( m_dataPage.lines[i]->nodes[j]->text.length() > 0 )
						{
							char c = m_dataPage.lines[i]->nodes[j]->text[0]; 
							int findN = m_dataPage.lines[i]->nodes[j]->text.find("N");
							int findR = m_dataPage.lines[i]->nodes[j]->text.find("R");
							int findB = m_dataPage.lines[i]->nodes[j]->text.find("B");
							int findQ = m_dataPage.lines[i]->nodes[j]->text.find("Q");
							int findK = m_dataPage.lines[i]->nodes[j]->text.find("K");
								
							if( findN > 0 )
								c='N';
							else if( findR > 0 )
								c = 'R';
							else if( findB > 0 )
								c = 'B';
							else if( findQ > 0 )
								c = 'Q';
							else if( findK > 0 )
								c = 'K';

							switch( c )
							{
							
							case 'N':
								{
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findN), firstPoint);
									dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findN), &twidth, &theight);
									wxFont oldFont = dc.GetFont();
									dc.SetFont(tf3);
									firstPoint.x += twidth;
									dc.GetTextExtent("N", &twidth, &theight);
									
									dc.DrawText("N", firstPoint);
									int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
									dc.SetFont(oldFont);
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findN+1,textLen-1), 
										wxPoint(firstPoint.x+twidth, 
												m_dataPage.lines[i]->nodes[j]->m_rect.y));
								}
								break;
							case 'B':
								{
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findB), firstPoint);
									dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findB), &twidth, &theight);
									wxFont oldFont = dc.GetFont();
									dc.SetFont(tf3);
									firstPoint.x += twidth;
									dc.GetTextExtent("L", &twidth, &theight);
									
									dc.DrawText("L", firstPoint);
									int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
									dc.SetFont(oldFont);
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findB+1,textLen-1), 
										wxPoint(firstPoint.x+twidth, 
												m_dataPage.lines[i]->nodes[j]->m_rect.y));								}
								break;
							case 'R':
								{
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findR), firstPoint);
									dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findR), &twidth, &theight);
									wxFont oldFont = dc.GetFont();
									dc.SetFont(tf3);
									firstPoint.x += twidth;
									dc.GetTextExtent("R", &twidth, &theight);
									
									dc.DrawText("R", firstPoint);
									int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
									dc.SetFont(oldFont);
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findR+1,textLen-1), 
										wxPoint(firstPoint.x+twidth, 
												m_dataPage.lines[i]->nodes[j]->m_rect.y));								}
								break;
							case 'Q':
								{
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findQ), firstPoint);
									dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findQ), &twidth, &theight);
									wxFont oldFont = dc.GetFont();
									dc.SetFont(tf3);
									firstPoint.x += twidth;
									dc.GetTextExtent("Q", &twidth, &theight);
									
									dc.DrawText("Q", firstPoint);
									int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
									dc.SetFont(oldFont);
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findQ+1,textLen-1), 
										wxPoint(firstPoint.x+twidth, 
												m_dataPage.lines[i]->nodes[j]->m_rect.y));
								}
								break;
							case 'K':
								{
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findK), firstPoint);
									dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findK), &twidth, &theight);
									wxFont oldFont = dc.GetFont();
									dc.SetFont(tf3);
									firstPoint.x += twidth;
									dc.GetTextExtent("K", &twidth, &theight);
									
									dc.DrawText("K", firstPoint);
									int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
									dc.SetFont(oldFont);
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findK+1,textLen-1), 
										wxPoint(firstPoint.x+twidth, 
												m_dataPage.lines[i]->nodes[j]->m_rect.y));
								}
								break;

							default:
								{
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
										wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
										m_dataPage.lines[i]->nodes[j]->m_rect.y));
								}
								break;
							};
						}
					}
					else
					{
						dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
							wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
							m_dataPage.lines[i]->nodes[j]->m_rect.y));
					}
					dc.SetTextForeground(*wxBLACK);
					dc.SetBrush(*wxBLACK_BRUSH);
					dc.SetPen(*wxBLACK_PEN);
				}
				else
				{
					dc.SetTextForeground(m_dataPage.lines[i]->nodes[j]->GetColor());
					if( m_owner->optionsFrame->m_NotationOptions.usePiecesInNotation == true )
					{
						int twidth;
						int theight;
						wxPoint firstPoint = wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
							m_dataPage.lines[i]->nodes[j]->m_rect.y);
						if( m_dataPage.lines[i]->nodes[j]->text.length() > 0 ) 
						{
							char c;
							int findN = m_dataPage.lines[i]->nodes[j]->text.find("N");
							int findR = m_dataPage.lines[i]->nodes[j]->text.find("R");
							int findB = m_dataPage.lines[i]->nodes[j]->text.find("B");
							int findQ = m_dataPage.lines[i]->nodes[j]->text.find("Q");
							int findK = m_dataPage.lines[i]->nodes[j]->text.find("K");
							if( m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK  )
							{
								c = m_dataPage.lines[i]->nodes[j]->text[0]; 

								
								if( findN > 0 )
									c='N';
								else if( findR > 0 )
									c = 'R';
								else if( findB > 0 )
									c = 'B';
								else if( findQ > 0 )
									c = 'Q';
								else if( findK > 0 )
									c = 'K';
							}
							else
							{
								c = m_dataPage.lines[i]->nodes[j]->text[0];
							}

							switch( c)
							{
							
							case 'N':
								{

									if( m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK )
									{
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findN), firstPoint);
										dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findN), &twidth, &theight);
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										firstPoint.x += twidth;
										dc.GetTextExtent("N", &twidth, &theight);
									
										dc.DrawText("N", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findN+1,textLen-1), 
											wxPoint(firstPoint.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));

									}
									else
									{
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										dc.GetTextExtent("n", &twidth, &theight);
										dc.DrawText("n", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
											wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));
									}

								}
								break;
							case 'B':
								{
									if( m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK )
									{
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findB), firstPoint);
										dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findB), &twidth, &theight);
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										firstPoint.x += twidth;
										dc.GetTextExtent("L", &twidth, &theight);
									
										dc.DrawText("L", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findB+1,textLen-1), 
											wxPoint(firstPoint.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));

									}
									else
									{
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										dc.GetTextExtent("l", &twidth, &theight);
										dc.DrawText("l", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
											wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));
									}
								}
								break;
							case 'R':
								{
									if( m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK )
									{
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findR), firstPoint);
										dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findR), &twidth, &theight);
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										firstPoint.x += twidth;
										dc.GetTextExtent("R", &twidth, &theight);
									
										dc.DrawText("R", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findR+1,textLen-1), 
											wxPoint(firstPoint.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));

									}
									else
									{
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										dc.GetTextExtent("r", &twidth, &theight);
										dc.DrawText("r", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
											wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));
									}
								}
								break;
							case 'Q':
								{
									if( m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK )
									{
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findQ), firstPoint);
										dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findQ), &twidth, &theight);
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										firstPoint.x += twidth;
										dc.GetTextExtent("Q", &twidth, &theight);
									
										dc.DrawText("Q", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findQ+1,textLen-1), 
											wxPoint(firstPoint.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));

									}
									else
									{
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										dc.GetTextExtent("q", &twidth, &theight);
										dc.DrawText("q", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
											wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));
									}
								}
								break;
							case 'K':
								{
									if( m_dataPage.lines[i]->nodes[j]->bWhiteBlack != BLACK )
									{
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(0,findK), firstPoint);
										dc.GetTextExtent(m_dataPage.lines[i]->nodes[j]->text.substr(0,findK), &twidth, &theight);
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										firstPoint.x += twidth;
										dc.GetTextExtent("K", &twidth, &theight);
									
										dc.DrawText("K", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(findK+1,textLen-1), 
											wxPoint(firstPoint.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));

									}
									else
									{
										wxFont oldFont = dc.GetFont();
										dc.SetFont(tf3);
										dc.GetTextExtent("k", &twidth, &theight);
										dc.DrawText("k", firstPoint);
										int textLen = m_dataPage.lines[i]->nodes[j]->text.length();
										dc.SetFont(oldFont);
										dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.substr(1,textLen-1), 
											wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x+twidth, 
													m_dataPage.lines[i]->nodes[j]->m_rect.y));
									}
								}
								break;

							default:
								{
									dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
										wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
										m_dataPage.lines[i]->nodes[j]->m_rect.y));
								}
								break;

							};
						}
					}
					else
					{
						
						dc.DrawText(m_dataPage.lines[i]->nodes[j]->text.c_str(), 
							wxPoint(m_dataPage.lines[i]->nodes[j]->m_rect.x, 
							m_dataPage.lines[i]->nodes[j]->m_rect.y));
						
					}
					dc.SetTextForeground(*wxBLACK);
					dc.SetPen(*wxBLACK_PEN);
				
					
				}

			}
		}
#endif
	}

	//event.RequestMore() ;
}


#if wxUSE_GRAPHICS_CONTEXT

const int BASE  = 80.0;
const int BASE2 = BASE/2;
const int BASE4 = BASE/4;

static inline double DegToRad(double deg) { return (deg * M_PI) / 180.0; }


// modeled along Robin Dunn's GraphicsContext.py sample

void MyCanvas::DrawGraphics(wxGraphicsContext* gc)
{
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    gc->SetFont(font,*wxBLACK);

   gc->PushState();
   // gc->Translate(60, 400);
    gc->DrawText("Scaled smiley inside a square", 0, 0);
    gc->DrawRectangle(BASE2, BASE2, 500, 500);
    gc->DrawBitmap(*gs_bmp4, BASE2, BASE2, 500, 500);
    gc->PopState();

    // make a path that contains a circle and some lines, centered at 0,0
    wxGraphicsPath path = gc->CreatePath() ;
    path.AddCircle( 0, 0, BASE2 );
    path.MoveToPoint(0, -BASE2);
    path.AddLineToPoint(0, BASE2);
    path.MoveToPoint(-BASE2, 0);
    path.AddLineToPoint(BASE2, 0);
    path.CloseSubpath();
    path.AddRectangle(-BASE4, -BASE4/2, BASE2, BASE4);

    // Now use that path to demonstrate various capbilites of the grpahics context
    gc->PushState(); // save current translation/scale/other state
    gc->Translate(60, 75); // reposition the context origin

    gc->SetPen(wxPen("navy", 1));
    gc->SetBrush(wxBrush("pink"));

	//gc->DrawA
    for( int i = 0 ; i < 3 ; ++i )
    {
        wxString label;
        switch( i )
        {
            case 0 :
                label = "StrokePath";
                break;
            case 1 :
                label = "FillPath";
                break;
            case 2 :
                label = "DrawPath";
                break;
        }
        wxDouble w, h;
        gc->GetTextExtent(label, &w, &h, NULL, NULL);
        gc->DrawText(label, -w/2, -BASE2-h-4);
        switch( i )
        {
            case 0 :
                gc->StrokePath(path);
                break;
            case 1 :
                gc->FillPath(path);
                break;
            case 2 :
                gc->DrawPath(path);
                break;
        }
        gc->Translate(2*BASE, 0);
    }

    gc->PopState(); // restore saved state
    gc->PushState(); // save it again
    gc->Translate(60, 200); // offset to the lower part of the window

    gc->DrawText("Scale", 0, -BASE2);
    gc->Translate(0, 20);

    gc->SetBrush(wxBrush(wxColour(178,  34,  34, 128)));// 128 == half transparent
    for( int i = 0 ; i < 8 ; ++i )
    {
        gc->Scale(1.08, 1.08); // increase scale by 8%
        gc->Translate(5,5);
        gc->DrawPath(path);
    }

    gc->PopState(); // restore saved state
    gc->PushState(); // save it again
    gc->Translate(400, 200);

    gc->DrawText("Rotate", 0, -BASE2);

    // Move the origin over to the next location
    gc->Translate(0, 75);

    // draw our path again, rotating it about the central point,
    // and changing colors as we go
    for ( int angle = 0 ; angle < 360 ; angle += 30 )
    {
        gc->PushState(); // save this new current state so we can
        //  pop back to it at the end of the loop
        wxImage::RGBValue val = wxImage::HSVtoRGB(wxImage::HSVValue(float(angle)/360, 1, 1));
        gc->SetBrush(wxBrush(wxColour(val.red, val.green, val.blue, 64)));
        gc->SetPen(wxPen(wxColour(val.red, val.green, val.blue, 128)));

        // use translate to artfully reposition each drawn path
        gc->Translate(1.5 * BASE2 * cos(DegToRad(angle)),
                     1.5 * BASE2 * sin(DegToRad(angle)));

        // use Rotate to rotate the path
        gc->Rotate(DegToRad(angle));

        // now draw it
        gc->DrawPath(path);
        gc->PopState();
    }
    gc->PopState();

 
}
#endif // wxUSE_GRAPHICS_CONTEXT

void MyCanvas::OnIdle(wxIdleEvent &event)
{
	//wxPaintEvent evt;

/*	if( m_owner->board_canvas->IsAnimating() )
	{
		m_owner->board_canvas->OnIdle(event);
	}
*/	//evt.Skip();
	//OnPaintruet(evt);
	//event.RequestMore(false);
}

void MyCanvas::OnMouseMove(wxMouseEvent &event)
{
#if wxUSE_STATUSBAR
    wxClientDC dc(this);
    PrepareDC(dc);
    m_owner->PrepareDC(dc);


    wxString str;
   // str.Printf( wxT("Current mouse position: %d,%d"), (int)x, (int)y );
   // m_owner->SetStatusText( str );
#else
    wxUnusedVar(event);
#endif // wxUSE_STATUSBAR
    wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );

	for( int i = 0; i < m_dataPage.lines.size(); i++ )
	{
		if( m_dataPage.lines[i]->nodes.size() == 0 )
			continue;
		
		if(((y > m_dataPage.lines[i]->nodes[0]->m_rect.y) &&
		   (y < m_dataPage.lines[i]->nodes[0]->m_rect.y + m_dataPage.lines[i]->nodes[0]->m_rect.GetHeight())))
		{
			for( int j = 0; j < m_dataPage.lines[i]->nodes.size(); j++ )
			{
				if( x > m_dataPage.lines[i]->nodes[j]->m_rect.x &&
					(x< m_dataPage.lines[i]->nodes[j]->m_rect.x + 
					m_dataPage.lines[i]->nodes[j]->m_rect.GetWidth()))
				{
					if( m_dataPage.lines[i]->nodes[j]->GetType() == TEXT_DISP_NODE )
					{
						this->m_owner->currentGame->pCurrMove = m_dataPage.lines[i]->nodes[j]->pMove;
					
						this->white_black_move = m_dataPage.lines[i]->nodes[j]->bWhiteBlack;
						if( this->white_black_move == 1 )
						{
							m_owner->board_canvas->GetBoard()->SetBoard(m_owner->currentGame->pCurrMove->m_fenBlack );
							this->white_black_move = 0;
						}
						else
						{
							m_owner->board_canvas->GetBoard()->SetBoard(m_owner->currentGame->pCurrMove->m_fenWhite );
							this->white_black_move = 1;
						}
					
						//Refresh();
						
						//m_owner->board_canvas->StopAnimating();
						m_owner->board_canvas->clearFlag = true;
						//m_owner->board_canvas->Refresh(false);
						
						//m_owner->treeCanvas->Refresh();
						m_owner->DoRefresh();
						for( int j = 0; j < activeEngines.size(); j++ )
						{
							if( activeEngines[j]->IsActive() &&
								activeEngines[j]->IsAnalysing())
							{
								activeEngines[j]->KibitzStartsMove();
								activeEngines[j]->KibitzUserEndsMove();
							}
						}
						//SetFocus();
						break;
					}
					else if(m_dataPage.lines[i]->nodes[j]->GetType() == CODE_FOLD_DISP_NODE)
					{
						if( m_dataPage.lines[i]->nodes[j]->GetState() == 1 )
						{
							m_dataPage.lines[i]->nodes[j]->SetState(0);
							m_dataPage.lines[i]->nodes[j]->pMove->SetDisplayVariations(true);
							this->needsRefresh = true;
						}
						else if( !m_owner->currentGame->pCurrMove->MoveIsVariationRoot(m_dataPage.lines[i]->nodes[j]->pMove) )
						{
							m_dataPage.lines[i]->nodes[j]->SetState(1);
							m_dataPage.lines[i]->nodes[j]->pMove->SetDisplayVariations(false);
							this->needsRefresh = true;
						}
						Refresh();
						//SetFocus();
						break;
					}
				}
			}
		}
	}
#ifdef USE_CANVAS_SET_FOCUS
	SetFocus();
#endif
}

void MyCanvas::ResetBoard()
{
	m_owner->currentGame->ResetMoveList();
					
	m_owner->board_canvas->ResetBoard();//->chess_board.SetInitialPosition();
					
	move_count = 0;
	this->selectedMove = 0;
	this->white_black_move = 0;
					
//	m_owner->ctrl->Refresh(true);
	needsRefresh = true;
	m_owner->board_canvas->Refresh(false);
	this->Refresh(true);
}


void MyCanvas::LogEvent(const wxString& name, wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	
	switch ( keycode )
	{
		case WXK_BACK:break;
        case WXK_TAB:break;
        case WXK_RETURN:break;
        case WXK_ESCAPE:break;
        case WXK_SPACE:break;
        case WXK_DELETE:break;
        case WXK_START:break;
        case WXK_LBUTTON:break;
        case WXK_RBUTTON:break;
        case WXK_CANCEL:break;
        case WXK_MBUTTON:break;
        case WXK_CLEAR:break;
        case WXK_SHIFT:break;
        case WXK_ALT:break;
        case WXK_CONTROL:break;
        case WXK_MENU:break;
        case WXK_PAUSE:break;
        case WXK_CAPITAL:break;
        case WXK_END:break;
        case WXK_HOME:break;
		case WXK_UP:
			{
				m_owner->treeCanvas->DecrementSelection();
			}
			break;
        case WXK_DOWN:
			{
				m_owner->treeCanvas->IncrementSelection();
			}
			break;
        case WXK_LEFT:
			{
				if( m_owner->board_canvas->IsAnimating() )
					break;

				/*if( m_owner->board_canvas->moveForward == true )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;
				}*/
				
				bool isMainLine = m_owner->currentGame->pCurrMove->IsThisMainLine();

				bool bStartAnimation = true;
				

				

				if( m_owner->pgnLoad.move_counter== 0)
				{
					m_owner->treeCanvas->SetSelection(0);
					break;
				}

				if( (white_black_move == BLACK) && (move_count > 0))
				{
					if(m_owner->currentGame->pCurrMove->pUp != NULL )
					{
						if( isMainLine )
						{
							m_owner->currentGame->PrevMove();
							m_owner->currentGame->PrevMove();
						}
						else
						{
							if( m_owner->treeCanvas->GetSelection()!= 0 &&
								m_owner->treeCanvas->GetSelection() == m_owner->treeCanvas->numSelectionsThisMove)
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
							}
							else
							{
								m_owner->currentGame->PrevMove();
								m_owner->currentGame->PrevMove();
							}
						}
						
					}
					else
					{
						m_owner->currentGame->PrevMove();
						//white_black_move = WHITE;
					}
				}

				if( (white_black_move == WHITE) && (move_count>0))
				{
					if( m_owner->currentGame->pCurrMove->white == "" )
					{
						if( isMainLine )
						{
							m_owner->currentGame->PrevMove();
						}
						else
						{
							if( m_owner->treeCanvas->GetSelection()!= 0 &&
								m_owner->treeCanvas->GetSelection() == m_owner->treeCanvas->numSelectionsThisMove)
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
							}
							else
							{
								m_owner->currentGame->PrevMove();
								//m_owner->currentGame->PrevMove();
							}
						}

					}
				}


				if( m_owner->currentGame->IsInBase() )
				{
					//m_owner->currentGame->NextMove();
					white_black_move = BLACK;
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;
					bStartAnimation = false;
				}
				/*if( (selectedMove ==0 ))// m_owner->currentGame->move_counter)
					//|| m_owner->board_canvas->GetBoard()->postReset)
				{
					//m_owner->ctrl->chess_board.mov = 0;
					//PGN::Move *pMove = m_owner->currentGame->pRoot;
					//while(pMove->pNext)
					//	pMove = pMove->pNext;
					
					//ResetBoard();

					
					return;
				}		*/		

				//MyChessMove mv = 
				if( white_black_move == WHITE )
				{
					//white_black_move = move_count;
					//ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->white );
					//m_owner->ctrl->chess_board.parseMove( m_owner->currentGame->GetNextMove()->white );
					//if( m_owner->currentGame->pCurrMove->hasFen )
					
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenWhite() );
					white_black_move = BLACK;
					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wxString inpt ="";

						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str());
							//inpt+= " ";
							//m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str()));
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}
					//else
					//{
					//	m_owner->board_canvas->GetBoard()->parseMove(m_owner->currentGame->GetPreviousMove()->white);
					//	m_owner->currentGame->pCurrMove->SetFen( m_owner->board_canvas->GetBoard()->Fen());
					//}
				}
				else
				{
					//white_black_move = move_count;
				//	ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->black );
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenBlack() );
					white_black_move = WHITE;
					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wxString inpt = "";
						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str());
							//inpt += " ";
						//	m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str()));
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}
				}
				
				m_owner->treeCanvas->SetSelection(0);
				ScrollToCursor();
				wxRect clear(XX,YY,w, h);
				//Refresh(/*, &clear*/ );
				//m_owner->board_canvas->clearFlag = true;
				m_owner->board_canvas->moveForward = false;
				m_owner->board_canvas->renderMoveNow = true;
				//m_owner->board_canvas->Refresh(false);
				
				if( bStartAnimation )
					m_owner->board_canvas->BackAnimation();
				//m_owner->treeCanvas->Refresh(true);
				m_owner->DoRefresh();

				//if( m_owner->currentGame->


				for( int j = 0; j < activeEngines.size(); j++ )
				{
					if( activeEngines[j]->IsActive() &&
						activeEngines[j]->IsAnalysing())
					{
						activeEngines[j]->KibitzStartsMove();
						activeEngines[j]->KibitzUserEndsMove();
					}
				}

				
				//Refresh();
			//	ctrl->chess_board.
			}break;
 
        case WXK_RIGHT:
			{

				/*if( m_owner->board_canvas->moveForward == false )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;
				}	*/			
				/*if( m_owner->variationSelectionBox &&
					m_owner->variationSelectionBox->IsVisible() )
					m_owner->variationSelectionBox->Close();
				*/
				if( m_owner->board_canvas->IsAnimating() )
					break;

				bool bStartAnimation = true;

				if( m_owner->pgnLoad.move_counter== 0)
					break;

					bool treeUsed = false;

				if( (white_black_move == WHITE) && !(move_count == 0))
				{
					if( m_owner->currentGame->pCurrMove->pNext &&
						m_owner->currentGame->pCurrMove->pNext->white != "" &&
						!m_owner->currentGame->pCurrMove->atEndBlack)
					{
						m_owner->currentGame->NextMove();
						//		m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
							//	m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();
					}
					else 
					{
						if( m_owner->currentGame->pRoot->white == "" )//&&
							//m_owner->currentGame->pCurrMove->pNext->white != "")
						{
							if( (m_owner->currentGame->IsInBase()) ||
								(m_owner->currentGame->pCurrMove->pNext->white != ""))
							{
								m_owner->currentGame->NextMove();
								if( m_owner->currentGame->pCurrMove->white != "" )
									white_black_move = WHITE;
								else
									white_black_move = BLACK;
							}
							else break;
							

						}
						else break;
					}
				}
				/*else if( (white_black_moves == WHITE) && (move_count == 0) &&
					(m_owner->currentGame->pRoot->white == "") &&
					(!m_owner->currentGame->pRoot->black == ""))
				{

				}*/

			

				if( (white_black_move == WHITE) )
				{

					if( m_owner->currentGame->GetCurrentMove()->pVarWhite)
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
						m_owner->treeCanvas->SetSelection(0);
					
						if( bStartAnimation )
							m_owner->board_canvas->StartAnimation();
						//m_owner->treeCanvas->Refresh(true);
						break;
					}

					if( m_owner->currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					
					{
						if( m_owner->currentGame->AtEnd())
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								(m_owner->treeCanvas->GetSelection() != 0))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								break;
							}
							else if( m_owner->currentGame->pCurrMove->white != "" )
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								
								if( bStartAnimation )
								{
									//if( 
									m_owner->board_canvas->StartAnimation();
								}

								white_black_move = WHITE;

								treeUsed = true;
								move_count++; // ??
								//break;
							}

						}
						else
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								(m_owner->treeCanvas->GetSelection() != 0))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								break;
							}
						}

					}
					else treeUsed = true;
				}

				if(white_black_move == BLACK && !treeUsed)
				{

					if( m_owner->currentGame->GetCurrentMove()->pVarBlack ) 
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
						m_owner->treeCanvas->SetSelection(0);
					
						if( bStartAnimation )
							m_owner->board_canvas->StartAnimation();

						move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
						break;
					}

					if(m_owner->currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					{
						if( m_owner->currentGame->AtEnd())
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								(m_owner->treeCanvas->GetSelection() != 0))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								break;
							}
							else if( m_owner->currentGame->pCurrMove->white != "" )
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								if( bStartAnimation )
								{
									if( m_owner->currentGame->pCurrMove->atEndWhite == true )
									{
										//m_owner->board_canvas->StartAnimation();
									}
									else
									{
										m_owner->board_canvas->StartAnimation();
									}
									
								}

								//white_black_move = WHITE;

								move_count++; // ??
							}

						}
						else
						{
							if( (m_owner->treeCanvas->numSelectionsThisMove == 1) &&
								(m_owner->treeCanvas->GetSelection() != 0))
							{
								m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
								m_owner->treeCanvas->SetSelection(0);
					
								//if( bStartAnimation )
								//	m_owner->board_canvas->StartAnimation();

								move_count++; // ??
						//m_owner->treeCanvas->Refresh(true);
								break;
							}
						}

					
					}
				}

				if( m_owner->currentGame->IsInBase() )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;

					m_owner->currentGame->NextMove();
					if( m_owner->currentGame->pCurrMove->white != "" )
						white_black_move = WHITE;
					else
						white_black_move = BLACK;
					if( m_owner->currentGame->GetCurrentMove()->pVarWhite || 
						(m_owner->currentGame->GetCurrentMove()->GetVariationRoot()!=NULL))
					{
						//m_owner->CreateVariationSelectionBox(white_black_move);
						m_owner->SelectMoveFromTree(m_owner->treeCanvas->GetSelection());
						m_owner->treeCanvas->SetSelection(0);
						if( bStartAnimation )
							m_owner->board_canvas->StartAnimation();
						break;
					}
				}

				if( m_owner->currentGame->AtEnd() )
				{
					m_owner->board_canvas->pCurrMov=(MyChessMove*)0xffffff;

					if( m_owner->currentGame->pCurrMove->atEndWhite )
					{
						if( white_black_move == BLACK )
						{
							//white_black_move = WHITE;
							break;
						}
						white_black_move = WHITE;
					}
					else if( m_owner->currentGame->pCurrMove->atEndBlack )
					{
						if( white_black_move == WHITE && !treeUsed)
						{
							//white_black_move = BLACK;
							break;
						}
						
					}

					bStartAnimation = false;
					//break;
				}
		

				//MyChessMove mv = 
				if( white_black_move == WHITE )
				{
					
					//ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->white );
					//m_owner->ctrl->chess_board.parseMove( m_owner->currentGame->GetNextMove()->white );
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenWhite() );
				
					white_black_move = BLACK;


					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wxString inpt ="";

						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_white.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str());
						//	inpt+= " ";
							//m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_white.text[aa].c_str()));
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}
				}
				else
				{
					
				//	ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->black );
					m_owner->board_canvas->GetBoard()->SetBoard( m_owner->currentGame->CurrFenBlack() );
					white_black_move = WHITE;
					
					m_owner->wnd10->Clear();
					if( m_owner->currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wxString inpt = "";
						for( int aa = 0; aa < m_owner->currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str());
						//	inpt += " ";
						//	m_owner->wnd10->AppendText( wxString(m_owner->currentGame->pCurrMove->annotation_black.text[aa].c_str()));
						}
						m_owner->wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
						m_owner->wnd10->Refresh(true);
					}
					move_count++;
				}
				
				
				
				ScrollToCursor();

				wxRect clear(XX,YY,w, h);
				//Refresh(/*, &clear*/ );
				m_owner->board_canvas->moveForward = true;
				m_owner->board_canvas->renderMoveNow = true;
				//m_owner->board_canvas->Refresh(false);
				if( bStartAnimation )
					m_owner->board_canvas->StartAnimation();
				//m_owner->treeCanvas->Refresh(true);
				m_owner->DoRefresh();

				for( int j = 0; j < activeEngines.size(); j++ )
				{
					if( activeEngines[j]->IsActive() &&
						activeEngines[j]->IsAnalysing())
					{
						activeEngines[j]->KibitzStartsMove();
						activeEngines[j]->KibitzUserEndsMove();
					}
				}

				
				//Refresh();
			//	ctrl->chess_board.
			}break;

        case WXK_SELECT:break;
        case WXK_PRINT:break;
        case WXK_EXECUTE:break;
        case WXK_SNAPSHOT:break;
        case WXK_INSERT:break;
        case WXK_HELP:break;
        case WXK_NUMPAD0:break;
        case WXK_NUMPAD1:break;
        case WXK_NUMPAD2:break;
        case WXK_NUMPAD3:break;
        case WXK_NUMPAD4:break;
        case WXK_NUMPAD5:break;
        case WXK_NUMPAD6:break;
        case WXK_NUMPAD7:break;
        case WXK_NUMPAD8:break;
        case WXK_NUMPAD9:break;
        case WXK_MULTIPLY:break;
        case WXK_ADD:break;
        case WXK_SEPARATOR:break;
        case WXK_SUBTRACT:break;
        case WXK_DECIMAL:break;
        case WXK_DIVIDE:break;
        case WXK_F1:
			{
				wxCommandEvent tgmEvt;
				m_owner->m_parent->ToggleMaximize(tgmEvt);
			}
			break;
        case WXK_F2:break;
        case WXK_F3:break;
        case WXK_F4:break;
        case WXK_F5:break;
        case WXK_F6:break;
        case WXK_F7:break;
        case WXK_F8:break;
        case WXK_F9:break;
        case WXK_F10:break;
        case WXK_F11:break;
        case WXK_F12:break;
        case WXK_F13:break;
        case WXK_F14:break;
        case WXK_F15:break;
        case WXK_F16:break;
        case WXK_F17:break;
        case WXK_F18:break;
        case WXK_F19:break;
        case WXK_F20:break;
        case WXK_F21:break;
        case WXK_F22:break;
        case WXK_F23:break;
        case WXK_F24:break;
        case WXK_NUMLOCK:break;
        case WXK_SCROLL:break;
        case WXK_PAGEUP:break;
        case WXK_PAGEDOWN:break;
        case WXK_NUMPAD_SPACE:break;
        case WXK_NUMPAD_TAB:break;
        case WXK_NUMPAD_ENTER:break;
        case WXK_NUMPAD_F1:break;
        case WXK_NUMPAD_F2:break;
        case WXK_NUMPAD_F3:break;
        case WXK_NUMPAD_F4:break;
        case WXK_NUMPAD_HOME:break;
        case WXK_NUMPAD_LEFT:break;
        case WXK_NUMPAD_UP:break;
        case WXK_NUMPAD_RIGHT:break;
        case WXK_NUMPAD_DOWN:break;
        case WXK_NUMPAD_PAGEUP:break;
        case WXK_NUMPAD_PAGEDOWN:break;
        case WXK_NUMPAD_END:break;
        case WXK_NUMPAD_BEGIN:break;
        case WXK_NUMPAD_INSERT:break;
        case WXK_NUMPAD_DELETE:break;
        case WXK_NUMPAD_EQUAL:break;
        case WXK_NUMPAD_MULTIPLY:break;
        case WXK_NUMPAD_ADD:break;
        case WXK_NUMPAD_SEPARATOR:break;
        case WXK_NUMPAD_SUBTRACT:break;
        case WXK_NUMPAD_DECIMAL:break;
        case WXK_NUMPAD_DIVIDE:break;

    default: break;

    }
}


  /*  EVT_MOTION (MyCanvas::OnMouseMove)
	EVT_LEFT_DOWN(MyCanvas::mouseDown)
	EVT_RIGHT_DOWN(MyCanvas::rightClick)
	EVT_LEFT_UP(MyCanvas::mouseReleased)
	EVT_MOUSEWHEEL(MyCanvas::mouseWheelMoved)
END_EVENT_TABLE()*/
void MyCanvas::mouseDown(wxMouseEvent& event)
{
}
void MyCanvas::mouseWheelMoved(wxMouseEvent& event)
{
}
void MyCanvas::mouseReleased(wxMouseEvent& event)
{
}
void MyCanvas::rightClick(wxMouseEvent& evt)
{
	wxMenu menu;
	menu.Append(PROMOTE_VARIATION, "Promote Variation", "Moves the Current Variation up in the Tree");
	menu.Append(DELETE_VARIATION, "Delete Variation", "Completely Deletes the Current Variation, this action cannot be undone");
	menu.Append(DELETE_REMAINING_MOVES, "Delete Remaining Moves", "Deletes all Moves after the current Move");
	menu.Append(ADD_ANNOTATION, "Add Annotation To This Move", "Annotates the Current Move");
	menu.Append(SET_VARIATION_COLOR, "Set Variation Colour", "Sets the Colour of the Current Variation");
	menu.Append(SET_MOVE_COLOR, "Set Move Colour", "Sets the Colour of the Current Move");
	//menu.

	
    PopupMenu(&menu);
}


void MyCanvas::SetMoveColor(wxCommandEvent& WXUNUSED(evt))
{
	wxColourDialog colorDlg(this);
	
	if( colorDlg.ShowModal() == wxID_OK )
	{
		wxColourData colData = colorDlg.GetColourData();
	
		if( m_owner->board_canvas->GetBoard()->mov == 1 )
		{
			m_owner->currentGame->pCurrMove->SetRenderColorWhite(colData.GetColour().Red(),
										colData.GetColour().Green(), colData.GetColour().Blue());
		}
		else
		{
			m_owner->currentGame->pCurrMove->SetRenderColorBlack(colData.GetColour().Red(),
										colData.GetColour().Green(), colData.GetColour().Blue());
		}
		this->needsRefresh = true;
		Refresh(true);
	}
}

void MyCanvas::SetVariationColor(wxCommandEvent& WXUNUSED(evt))
{
	wxColourDialog colorDlg(this);
	
	if( colorDlg.ShowModal() == wxID_OK )
	{
		wxColourData colData = colorDlg.GetColourData();
		
		m_owner->currentGame->pCurrMove->SetVariationRenderColor(colData.GetColour().Red(),
									colData.GetColour().Green(), colData.GetColour().Blue());

		this->needsRefresh = true;
		Refresh(true);
	}
}

void MyCanvas::PromoteVariation(wxCommandEvent& evt)
{
	m_owner->PromoteVariation();
}

void MyCanvas::DeleteVariation(wxCommandEvent& evt)
{
	m_owner->DeleteVariation();
}

void MyCanvas::DeleteRemainingMoves(wxCommandEvent& evt)
{
	m_owner->DeleteRemainingMoves();
}

void MyCanvas::AddAnnotation(wxCommandEvent& evt)
{
	m_owner->EditAnnotation();
}




TraverseGameTimer::TraverseGameTimer(MyFrame *frame) : wxTimer()
{
    m_frame = frame;
}
 
void TraverseGameTimer::Notify()
{
    //pane->Refresh(true);
	//if( pane->gameState == BoardCanvas::GameState::SELECTED_PIECE )
	m_frame->IncMove();
	//pane->Notified = true;
}
 
void TraverseGameTimer::start()
{
    wxTimer::Start(15000);
}