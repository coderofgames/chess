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
#include "ScoreGraph.h"


#include "FullAnalysisDialog.h"
#include "EngineMatchDialog.h"
#include "D3D9Board.h"
//#include "ProcessReadThread.h"
#ifdef DrawText
#undef DrawText
#include "headers.h"
#endif

#include "TimeControlSettingsDialog.h"
#include "BookListDialog.h"

#include "PlayVsComputerDialog.h"
#include "FAWizard.h"

#include "ECO_FilterDialog.h"

#include "TreeStatsCanvas.h"

#include "GridFrame.h"

#include "ScorePanel.h"

#include "VariationBoardFrame.h"

#include "MatchResultsPanel.h"

#include "FindDialog.h"

#include "WorkerThread.h"

#if wxUSE_BUSYINFO
    #include "wx/busyinfo.h"
#endif // wxUSE_BUSYINFO

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
		 /*   void OnClipboardBtn(wxRibbonButtonBarEvent& evt);
	void OnClipboardDropdownBtn(wxRibbonButtonBarEvent& evt);
	void OnOpenBtn(wxRibbonButtonBarEvent& evt);
	void OnOpenDropdownBtn(wxRibbonButtonBarEvent& evt);
	void OnSaveBtn(wxRibbonButtonBarEvent& evt);
	void OnSaveDropdownBtn(wxRibbonButtonBarEvent& evt);*/
	EVT_RIBBONBUTTONBAR_CLICKED(ID_SHOW_RESULTS, MDIParentFrame::OnShowResults)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_RESIGN, MDIParentFrame::OnPlayerResign)

	EVT_RIBBONBUTTONBAR_CLICKED(ID_FULL_ANALYSIS, MDIParentFrame::OnFullAnalysis)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_STOP_ENGINES_PLAYING, MDIParentFrame::OnStopEnginesPlayingBtn)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_AJOURN_GAME, MDIParentFrame::AjournGame)
			
		
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHOW_VARIATION_BOARD, MDIParentFrame::OnVariationBoard)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHOW_GRAPH, MDIParentFrame::OnShowGraph)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_CLIPBOARD_BTN, MDIParentFrame::OnClipboardBtn)
	EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_CLIPBOARD_BTN, MDIParentFrame::OnClipboardDropdownBtn)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_SWAP_SIDES, MDIParentFrame::OnSwapSides)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_LOAD_MEDIA_FROM_DISK, MDIParentFrame::OnLoadMediaFromDisk)
	EVT_RIBBONBUTTONBAR_CLICKED(ID_LOAD_MEDIA_FROM_URL, MDIParentFrame::OnLoadMediaFromURL)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DEFAULT_PROVIDER, MDIParentFrame::OnDefaultProvider)

EVT_RIBBONBUTTONBAR_CLICKED(ID_AUI_PROVIDER, MDIParentFrame::OnAUIProvider)
EVT_RIBBONBUTTONBAR_CLICKED(ID_MSW_PROVIDER, MDIParentFrame::OnMSWProvider)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SELECTION_EXPAND_H, MDIParentFrame::OnSelectionExpandHButton)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SELECTION_EXPAND_V, MDIParentFrame::OnInserAnnotationBtn)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SELECTION_CONTRACT, MDIParentFrame::OnToggleFullScreenButton)

EVT_RIBBONBUTTONBAR_CLICKED(ID_EDIT_PGN_HEADER_BTN, MDIParentFrame::OnEditPGNHeaderBtn)
EVT_RIBBONTOOLBAR_CLICKED(ID_EDIT_PGN_HEADER, MDIParentFrame::OnEditPGNHeader)
EVT_RIBBONTOOLBAR_CLICKED(ID_FLIP_BOARD, MDIParentFrame::OnFlipBoard)
EVT_RIBBONBUTTONBAR_CLICKED(ID_FLIP_BOARD_BTN, MDIParentFrame::OnFlipBoardBtn)


EVT_RIBBONBUTTONBAR_CLICKED(ID_EDIT_ANNOTATION, MDIParentFrame::OnEditAnnotation)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DELETE_REMAINING_MOVES, MDIParentFrame::OnDeleteRemainingMoves)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DELETE_VARIATION, MDIParentFrame::OnDeleteVariation)
EVT_RIBBONBUTTONBAR_CLICKED(ID_PROMOTE_VARIATION, MDIParentFrame::OnPromoteVariation)

EVT_RIBBONBUTTONBAR_CLICKED(ID_STOP_PLAY_THROUGH_GAME, MDIParentFrame::OnStopPlayThroughGame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_PLAY_THROUGH_GAME, MDIParentFrame::OnPlayThroughGame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_INFO, MDIParentFrame::OnInfo)

EVT_RIBBONBUTTONBAR_CLICKED(ID_COPY_GAME_LIST_GAME, MDIParentFrame::OnCopyGame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_CHECK_ALL, MDIParentFrame::OnCheckAll)
EVT_RIBBONBUTTONBAR_CLICKED(ID_UNCHECK_ALL, MDIParentFrame::OnUnCheckAll)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DATAVIEW_DELETE_ALL_SELECTED_GAMES, MDIParentFrame::OnDeleteAllSelectedGames)

EVT_RIBBONBUTTONBAR_CLICKED(ID_REMOVE_GAME_FROM_DB, MDIParentFrame::OnRemoveGameFromDB)
EVT_RIBBONBUTTONBAR_CLICKED(ID_ADD_GAME_TO_DB, MDIParentFrame::OnAddGameToDB)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_DB, MDIParentFrame::)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_SAVE_DB, MDIParentFrame::OnSaveDB)

EVT_RIBBONBUTTONBAR_CLICKED(ID_COMPRESS_DB, MDIParentFrame::OnCompressDB)
EVT_RIBBONBUTTONBAR_CLICKED(ID_EDIT_MODE, MDIParentFrame::OnEditMode)
EVT_RIBBONBUTTONBAR_CLICKED(ID_ENGINES_MATCH, MDIParentFrame::OnEngineMatchBtn)
			

EVT_RIBBONBUTTONBAR_CLICKED(ID_PLAY_THROUGH_GAME_LIST, MDIParentFrame::OnPlayThroughGameList)
EVT_RIBBONBUTTONBAR_CLICKED(ID_CIRCLE, MDIParentFrame::OnEngineLoadBtn)
EVT_RIBBONBUTTONBAR_CLICKED(ID_CROSS, MDIParentFrame::OnOptions)
EVT_RIBBONBUTTONBAR_CLICKED(ID_TRIANGLE, MDIParentFrame::OnTriangleButton)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_SQUARE, MDIParentFrame::OnSquareButton)
EVT_RIBBONTOOLBAR_CLICKED(ID_SQUARE, MDIParentFrame::OnSquareButton)
//EVT_RIBBONBUTTONBAR_CLICKED(ID_NEW_GAME_BTN, MDIParentFrame::O)
//EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_SQUARE, MDIParentFrame::OnGameDropdown)

EVT_RIBBONBUTTONBAR_CLICKED(ID_APPLY_FILTER_ON_SUBSET, MDIParentFrame::ApplyFilterOnSubset)
EVT_RIBBONBUTTONBAR_CLICKED(ID_CLEAR_DB_FILTER, MDIParentFrame::OnClearSearchFilter)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SEARCH_FILTER, MDIParentFrame::OnApplySearchFilter)
EVT_RIBBONBUTTONBAR_CLICKED(ID_MERGE_ALL_GAMES, MDIParentFrame::OnMergeAll)
EVT_RIBBONBUTTONBAR_CLICKED(ID_MERGE_SELECTED_GAMES, MDIParentFrame::OnMergeSelected)
EVT_RIBBONBUTTONBAR_CLICKED(ID_MOVE_UP, MDIParentFrame::OnMoveUp)
EVT_RIBBONBUTTONBAR_CLICKED(ID_MOVE_DOWN, MDIParentFrame::OnMoveDown)

EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_FIND_DROPDOWN, MDIParentFrame::OnDatabaseFindDropdown)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_FILTER_DROPDOWN, MDIParentFrame::OnDatabaseFilterDropdown)

EVT_RIBBONBUTTONBAR_CLICKED(ID_SHOW_FILTERS, MDIParentFrame::OnDatabaseShowFilters)

EVT_RIBBONBUTTONBAR_CLICKED(ID_POSITION_SEARCH, MDIParentFrame::OnDatabasePositionSearch)
	
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
EVT_RIBBONBUTTONBAR_CLICKED(ID_OPEN_BTN, MDIParentFrame::OnOpenBtn)//
EVT_RIBBONTOOLBAR_CLICKED(ID_SAVE, MDIParentFrame::OnSave)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_BTN, MDIParentFrame::OnSaveBtn)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(wxID_NEW, MDIParentFrame::OnNewDropdown)
EVT_RIBBONBUTTONBAR_CLICKED(ID_NEW_GAME_BTN, MDIParentFrame::OnNewGameButton)//
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_NEW_GAME_BTN, MDIParentFrame::OnNewGameDropDown)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_OPEN, MDIParentFrame::OnOpenDropdown)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_OPEN_BTN, MDIParentFrame::OnOpenDropdownBtn)
EVT_RIBBONTOOLBAR_DROPDOWN_CLICKED(ID_SAVE, MDIParentFrame::OnSaveDropdown)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_SAVE_BTN, MDIParentFrame::OnSaveDropdownBtn)
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

EVT_MENU(ID_LOAD_DB, MDIParentFrame::OnLoadDB)
EVT_MENU(ID_LOAD_PGN, MDIParentFrame::OnLoadPGN)
EVT_MENU(ID_ADD_PGN, MDIParentFrame::OnAddPGN)

EVT_MENU(ID_OPEN_DB_NEW, MDIParentFrame::OnOpenDB_New)


EVT_MENU(ID_LOAD_DB_VM, MDIParentFrame::OnLoadDB_VM)
EVT_MENU(ID_LOAD_PGN_VM, MDIParentFrame::OnLoadPGN_VM)
EVT_MENU(ID_ADD_PGN_VM, MDIParentFrame::OnAddPGN_VM)


EVT_MENU(ID_LOAD_SINGLE_GAME, MDIParentFrame::OnLoadIndividualGame)
EVT_MENU(ID_LOAD_GAME_LIST_TO_ACTIVE, MDIParentFrame::OnLoadGameListToActive)
EVT_MENU(ID_PASTE_PGN, MDIParentFrame::OnPastePGN)
EVT_MENU(ID_MERGE_PGN_FROM_CLIPBOARD, MDIParentFrame::OnMergePGNFromClipboard)
EVT_MENU(ID_MERGE_PGN_FROM_DISK, MDIParentFrame::OnMergePGNFromDisk)
EVT_MENU(ID_COPY_PGN_TO_CLIPBOARD, MDIParentFrame::OnCopyPGNToClipBoard)
EVT_MENU(ID_COPY_FEN_TO_CLIPBOARD, MDIParentFrame::OnCopyFENToClipBoard)
EVT_MENU(ID_COPY_VARIATION_TO_CLIPBOARD, MDIParentFrame::OnCopyVariationToClipBoard)
EVT_MENU(ID_SAVE_GAME_LIST_AS_DATABASE, MDIParentFrame::OnSaveGameListAsDatabase)
EVT_MENU(ID_PASTE_FEN, MDIParentFrame::OnPasteFEN)
EVT_MENU(ID_SAVE_PGN, MDIParentFrame::OnSavePGN)
EVT_MENU(ID_EXPORT_DB, MDIParentFrame::OnSaveDBInfo)
EVT_MENU(ID_LOAD_BOOK, MDIParentFrame::OnLoadBook)
EVT_MENU(ID_SHOW_BOOK, MDIParentFrame::OnShowBookWindow)
EVT_MENU(ID_SHOW_CLOCK_WINDOW, MDIParentFrame::OnShowClock)


EVT_MENU(ID_NEW_GAME, MDIParentFrame::OnNewGame)
EVT_MENU(ID_SETUP_POSITION, MDIParentFrame::OnSetupPosition)


EVT_MENU(ID_CLOCK_SET_TIME_CONTROLS, MDIParentFrame::OnClockSetTimeControls)
EVT_MENU(ID_CLOCK_RESET, MDIParentFrame::OnClockReset)
EVT_MENU(ID_CLOCK_SET_TIME, MDIParentFrame::OnClockSetTime)


// DB SORT EVENTS
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_SORT_DB, MDIParentFrame::OnSort)
EVT_MENU(ID_SORT_BY_NUMBER, MDIParentFrame::SortByNumber)
EVT_MENU(ID_SORT_BY_WNAME, MDIParentFrame::SortByWhiteName)
EVT_MENU(ID_SORT_BY_BNAME, MDIParentFrame::SortByBlackName)
EVT_MENU(ID_SORT_BY_WELO, MDIParentFrame::SortByWhiteElo)
EVT_MENU(ID_SORT_BY_BELO, MDIParentFrame::SortByBlackElo)
EVT_MENU(ID_SORT_BY_ECO, MDIParentFrame::SortByECO)
EVT_MENU(ID_SORT_BY_EVENT, MDIParentFrame::SortByEvent)
EVT_MENU(ID_SORT_BY_SITE, MDIParentFrame::SortBySite)
EVT_MENU(ID_SORT_BY_DATE, MDIParentFrame::SortByDate)
EVT_MENU(ID_SORT_BY_RESULT, MDIParentFrame::SortByResult)
EVT_MENU(ID_SORT_BY_MOVES, MDIParentFrame::SortByMoves)
// DB FIND/ FILTER
EVT_MENU(ID_FIND_NAME, MDIParentFrame::OnDatabaseFindName)
EVT_MENU(ID_FILTER_DATE, MDIParentFrame::OnDatabaseFilterDate)
EVT_MENU(ID_FILTER_EVENT, MDIParentFrame::OnDatabaseFilterEvent)
EVT_MENU(ID_FILTER_SITE, MDIParentFrame::OnDatabaseFilterSite)
EVT_MENU(ID_FILTER_ELO, MDIParentFrame::OnDatabaseFilterElo)
EVT_MENU(ID_FILTER_ELO_WHITE, MDIParentFrame::OnDatabaseFilterEloWhite)
EVT_MENU(ID_FILTER_ELO_BLACK, MDIParentFrame::OnDatabaseFilterEloBlack)
EVT_MENU(ID_FILTER_ECO, MDIParentFrame::OnDatabaseFilterEco)
EVT_MENU(ID_FIND_NAME_WHITE, MDIParentFrame::OnDatabaseFilterNameWhite)
EVT_MENU(ID_FIND_NAME_BLACK, MDIParentFrame::OnDatabaseFilterNameBlack)


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


	EVT_THREAD(WORKER_EVENT, MyFrame::OnWorkerEvent)
	EVT_IDLE(MyFrame::OnIdle)
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
       : wxMDIParentFrame(NULL, wxID_ANY, "Chess Machine",
                          wxDefaultPosition, wxSize(500, 400))
{

	this->Maximize(true);
	bFullScreen = false;

	wxSystemOptions::SetOption("msw.display.directdraw", true);

	wxInitAllImageHandlers();

	wxIcon icon;
	icon.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/images/app_icon.png", wxBITMAP_TYPE_PNG,24,24);
	
    m_ribbon = new wxRibbonBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
		wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_LABELS | wxRIBBON_BAR_SHOW_PANEL_EXT_BUTTONS |wxRIBBON_BAR_SHOW_PANEL_MINIMISE_BUTTONS );

	m_ribbon->GetArtProvider()->SetColourScheme(THEME_COLOR_C, THEME_COLOR_B, THEME_COLOR_A);
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
	wxBitmap engineMatch;
	
	wxBitmap bmpClock;

	wxBitmap playThroughGame;
	wxBitmap stopPlayThroughGame;

	wxBitmap barGraph;
	wxBitmap bmpStopMatch;

	wxBitmap exclamationBmp;
	wxBitmap doubleExclamationBmp;
	wxBitmap exclamationQuestionBmp;
	wxBitmap questionBmp;
	wxBitmap questionExclamationBmp;
	wxBitmap doubleQuestionBmp;

	wxBitmap loadDB;
	wxBitmap removeGameFromDB;
	wxBitmap addGameToDB;

	wxBitmap resignBmp;
	wxBitmap trophy;

	wxBitmap fullAnalysis;
	wxBitmap showStats;

	wxBitmap checkAll;
	wxBitmap copyGame;

	wxBitmap clearSearchFilters;

	wxBitmap sortDB;

	wxBitmap uncheckAll;

	wxBitmap moveUp;
	wxBitmap moveDown;

	wxBitmap ajournGame;

	wxBitmap showVariationBoard;

	wxBitmap findBmp;
	wxBitmap filterBmp;

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
	engineMatch.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Run.png", wxBITMAP_TYPE_PNG);

	playThroughGame.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/star.png", wxBITMAP_TYPE_PNG);
	stopPlayThroughGame.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Stop All.png", wxBITMAP_TYPE_PNG);
	
	barGraph.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/3d bar chart.png", wxBITMAP_TYPE_PNG);
	
	resignBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Erase.png", wxBITMAP_TYPE_PNG);
	
	bmpStopMatch.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Stop sign.png", wxBITMAP_TYPE_PNG);
	//m_ribbon->SetFont(wxFont(6, wxMODERN, wxNORMAL,wxNORMAL, false/*,"Arial Baltic"*/));
    
	exclamationBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/Exclamation.png", wxBITMAP_TYPE_PNG);
	doubleExclamationBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/DoubleExclamation.png", wxBITMAP_TYPE_PNG);
	exclamationQuestionBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/ExclamationQuestion.png", wxBITMAP_TYPE_PNG);
	questionBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/QuestionExclamation.png", wxBITMAP_TYPE_PNG);
	questionExclamationBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/Question.png", wxBITMAP_TYPE_PNG);
	doubleQuestionBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/DoubleQuestion.png", wxBITMAP_TYPE_PNG);	

	loadDB.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Database.png", wxBITMAP_TYPE_PNG);
	removeGameFromDB.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Delete.png", wxBITMAP_TYPE_PNG);
	addGameToDB.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Add.png", wxBITMAP_TYPE_PNG);
	
	trophy.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Goals_32.png", wxBITMAP_TYPE_PNG);

	showStats.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/Statistics.png", wxBITMAP_TYPE_PNG);

	
	fullAnalysis.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/shlyap/1/Search.png", wxBITMAP_TYPE_PNG);
	
	//removeGameFromDB.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/Exclamation.png", wxBITMAP_TYPE_PNG);
	//addGameToDB.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/Glyphs/Exclamation.png", wxBITMAP_TYPE_PNG);
	checkAll.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/yes.png", wxBITMAP_TYPE_PNG);
	copyGame.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/copy.png", wxBITMAP_TYPE_PNG);
	

		clearSearchFilters.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/Remove Document.png", wxBITMAP_TYPE_PNG);

	sortDB.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Up-down.png", wxBITMAP_TYPE_PNG);
	uncheckAll.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Undo.png", wxBITMAP_TYPE_PNG);

	moveUp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Up.png", wxBITMAP_TYPE_PNG);
	moveDown.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/Down.png", wxBITMAP_TYPE_PNG);


	ajournGame.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/pixophilia/1/bell.png", wxBITMAP_TYPE_PNG);
	
	showVariationBoard.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/images/variation.png", wxBITMAP_TYPE_PNG);
	
	findBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/find.png", wxBITMAP_TYPE_PNG);

	filterBmp.LoadFile( wxStandardPaths::Get().GetResourcesDir()+"/icons/Free Aha Soft/filter.png", wxBITMAP_TYPE_PNG);

	//--------------------------------------------------------------------------------------
	// Analysis Page
	//--------------------------------------------------------------------------------------
    wxRibbonPage* home = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Analysis"), ribbon_xpm);
    {
		wxRibbonPanel *toolbar_panel = new wxRibbonPanel(home, wxID_ANY, wxT("Toolbar"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        
		{
			wxRibbonButtonBar *OpenSaveClipBoard = new wxRibbonButtonBar(toolbar_panel);
			OpenSaveClipBoard->AddDropdownButton(ID_OPEN_BTN, "Open", bmpOpen);
			OpenSaveClipBoard->AddDropdownButton(ID_SAVE_BTN, "Save", bmpSave);
			OpenSaveClipBoard->AddDropdownButton(ID_CLIPBOARD_BTN, "Clipboard", bmpClipBoard);
			/*wxRibbonToolBar *toolbar = new wxRibbonToolBar(toolbar_panel, ID_MAIN_TOOLBAR);
			toolbar->AddHybridTool(ID_OPEN, bmpOpen, "Open");
			toolbar->AddHybridTool(ID_SAVE, bmpSave, "Save");
			toolbar->AddHybridTool(wxID_NEW, bmpClipBoard, "Clipboard");      	
			toolbar->SetRows(1, 3);*/
		}
		wxRibbonPanel *selection_panel = new wxRibbonPanel(home, wxID_ANY, wxT("Game"), wxBitmap(selection_panel_xpm),wxDefaultPosition, wxDefaultSize,wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		
		{
			wxRibbonButtonBar *GamePanelBtns = new wxRibbonButtonBar(selection_panel);
			GamePanelBtns->AddDropdownButton(ID_NEW_GAME_BTN, "New", wxBitmap(chessboard_icon_xpm));
			GamePanelBtns->AddButton(ID_EDIT_PGN_HEADER_BTN, "Edit Header", bmpEditPGNHeader);
			GamePanelBtns->AddButton(ID_FLIP_BOARD_BTN, "Flip Board", bmpFlipBoard);
			/*wxRibbonToolBar *toolbar2 = new wxRibbonToolBar(selection_panel, ID_MAIN_TOOLBAR2); 
			toolbar2->AddHybridTool(ID_SQUARE,wxBitmap(chessboard_icon_xpm));
			toolbar2->AddHybridTool(ID_EDIT_PGN_HEADER,bmpEditPGNHeader);
			toolbar2->AddHybridTool(ID_FLIP_BOARD,bmpFlipBoard);
			toolbar2->SetRows(1, 2);*/
		}

        wxRibbonPanel *shapes_panel = new wxRibbonPanel(home, wxID_ANY, wxT("Functions"), wxBitmap(circle_small_xpm), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        {
			wxRibbonButtonBar *shapes = new wxRibbonButtonBar(shapes_panel);
			shapes->AddButton(ID_CIRCLE, wxT("Engine"), wxBitmap(circle_xpm), wxBitmap(circle_small_xpm), wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Add Engine");
			shapes->AddButton(ID_CROSS, wxT("Options"), wxBitmap(cross_xpm), wxNullBitmap,wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Show Options Panel");
			shapes->AddHybridButton(ID_TRIANGLE, wxT("Book"), wxBitmap(triangle_xpm));
			//shapes->AddDropdownButton(ID_POLYGON, wxT("Wizard"), wxBitmap(hexagon_xpm), wxEmptyString);
			shapes->AddButton(ID_SHOW_GRAPH, "Show Graph", barGraph);
		}
		//new wxRibbonPanel(home, wxID_ANY, wxT("Empty todo"), wxNullBitmap, wxDefaultPosition, wxSize(60,40), wxRIBBON_PANEL_NO_AUTO_MINIMISE);
      
		//shapes->SetHe
        wxRibbonPanel *nrp = new wxRibbonPanel(home, wxID_ANY, wxT("Edit PGN"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *editables = new wxRibbonButtonBar(nrp);
	    
			editables->AddButton(ID_EDIT_ANNOTATION, wxT("Edit Annotation"), bmpEditAnnotation, "Add Annotation");
			editables->AddButton(ID_DELETE_REMAINING_MOVES, wxT("Delete Moves"), bmpDeleteRemainingMoves, "Delete Remaining Moves");	
			editables->AddButton(ID_DELETE_VARIATION, wxT("Delete Variation"), bmpDeleteVariation, "Delete Variation");	
			editables->AddButton(ID_PROMOTE_VARIATION, wxT("Promote Variation"), bmpPromoteVariation, "Promote Variation");	
			//editables->SetAutoLayout(false);
			//wxRibbonPanel *primary_panel = new wxRibbonPanel(scheme, wxID_ANY,
            //wxT("Primary Colour"), colours_xpm);
			
		}
		//nrp->Mini

		/*wxRibbonPanel *glyphs = new wxRibbonPanel(home, wxID_ANY, wxT("Empty todo"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			m_primary_gallery = PopulateColoursPanel(glyphs,
            m_default_primary, ID_PRIMARY_COLOUR);
		}*/


        wxRibbonPanel *rp2 = new wxRibbonPanel(home, wxID_ANY, wxT("View"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *rp2bb = new wxRibbonButtonBar(rp2);
			rp2bb->AddButton(ID_PLAY_THROUGH_GAME, "Run Game",playThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			rp2bb->AddButton(ID_FULL_ANALYSIS, "Full Analysis",fullAnalysis,wxNullBitmap,wxNullBitmap,wxNullBitmap);

			//gcp = new GameClockPanel(rp2);
			rp2bb->AddButton(ID_STOP_PLAY_THROUGH_GAME, "Stop Game",stopPlayThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
		}

        wxRibbonPanel *rp4 = new wxRibbonPanel(home, wxID_ANY, wxT("Extras"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *rp2bb = new wxRibbonButtonBar(rp4);
			rp2bb->AddButton(ID_SHOW_VARIATION_BOARD, "Var Board",showVariationBoard,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			//rp2bb->AddButton(ID_FULL_ANALYSIS, "Full Analysis",fullAnalysis,wxNullBitmap,wxNullBitmap,wxNullBitmap);

			//gcp = new GameClockPanel(rp2);
			//rp2bb->AddButton(ID_STOP_PLAY_THROUGH_GAME, "Stop Game",stopPlayThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
		}

		
		
		//gcp->Start();
		new wxRibbonPanel(home, wxID_ANY, wxT("Empty todo"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        
		wxRibbonPanel *rp3 = new wxRibbonPanel(home, wxID_ANY, wxT("About"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *edge = new wxRibbonButtonBar(rp3);
			edge->AddButton(ID_SELECTION_CONTRACT, wxT("Toggle FullScreen"), bmpFullScreen, "Fullscreen");
			edge->AddButton(ID_INFO, wxT("Info"), info, "Info");
		}


	   


    }

	//--------------------------------------------------------------------------------------
	// Game Page
	//--------------------------------------------------------------------------------------
	wxRibbonPage* scheme = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Game"), ribbon_xpm );
    {


        wxFont label_font(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_LIGHT);
        m_bitmap_creation_dc.SetFont(label_font);

        
        m_ribbon->GetArtProvider()->GetColourScheme(&m_default_primary,
            &m_default_secondary, &m_default_tertiary);

        wxRibbonPanel *provider_panel = new wxRibbonPanel(scheme, wxID_ANY,
            wxT("Art"), wxNullBitmap,wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        {
			wxRibbonButtonBar *provider_bar = new wxRibbonButtonBar(provider_panel, wxID_ANY);
			provider_bar->AddDropdownButton(ID_NEW_GAME_BTN, wxT("New"), wxBitmap(chessboard_icon_xpm), "New Game Menu");
			provider_bar->AddButton(ID_CIRCLE, wxT("Engine"), wxBitmap(circle_xpm), wxBitmap(circle_small_xpm), wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Add Engine");
			provider_bar->AddButton(ID_CROSS, wxT("Options"), wxBitmap(cross_xpm), wxNullBitmap,wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Show Options Panel");
		}
        
		wxRibbonPanel *gameFunc = new wxRibbonPanel(scheme, wxID_ANY, wxT("Game Functions"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *game_functions = new wxRibbonButtonBar(gameFunc);
			game_functions->AddButton(ID_SWAP_SIDES, wxT("Swap Sides"), swapSides, wxNullBitmap, wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Swap Sides");
			game_functions->AddButton(ID_RESIGN, wxT("Resign"), resignBmp, wxNullBitmap, wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Resign");
			game_functions->AddButton(ID_FLIP_BOARD_BTN, "Flip Board", bmpFlipBoard);
			game_functions->AddDropdownButton(ID_CLOCK, wxT("Clock"), bmpClock, "Clock functions");
			game_functions->AddButton(ID_ENGINES_MATCH, "Engine Match", engineMatch);
			game_functions->AddButton(ID_STOP_ENGINES_PLAYING, "Stop Match", bmpStopMatch);
			game_functions->AddButton(ID_AJOURN_GAME, "Ajourn Game", ajournGame);

			game_functions->AddButton(ID_SHOW_GRAPH, "Show Graph", barGraph);
			game_functions->AddButton(ID_SHOW_RESULTS, "Results", showStats);
		}
    }

	//--------------------------------------------------------------------------------------
	// Batch Analysis
	//--------------------------------------------------------------------------------------
    wxRibbonPage *BatchAnalysisPage =new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Active Games"), ribbon_xpm);
    {
		wxRibbonPanel *toolbar_panel21 = new wxRibbonPanel(BatchAnalysisPage, wxID_ANY, wxT("Toolbar"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        {
			wxRibbonButtonBar *OpenSaveClipBoard = new wxRibbonButtonBar(toolbar_panel21);
			OpenSaveClipBoard->AddDropdownButton(ID_OPEN_BTN, "Open", bmpOpen);
			OpenSaveClipBoard->AddDropdownButton(ID_SAVE_BTN, "Save", bmpSave);
			OpenSaveClipBoard->AddDropdownButton(ID_CLIPBOARD_BTN, "Clipboard", bmpClipBoard);
		}

		wxRibbonPanel *toolbar_panel23 = new wxRibbonPanel(BatchAnalysisPage, wxID_ANY, wxT("Game"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);

		{
			wxRibbonButtonBar *GamePanelBtns = new wxRibbonButtonBar(toolbar_panel23);
			GamePanelBtns->AddDropdownButton(ID_NEW_GAME_BTN, "New", wxBitmap(chessboard_icon_xpm));
			GamePanelBtns->AddButton(ID_EDIT_PGN_HEADER_BTN, "Edit Header", bmpEditPGNHeader);
			GamePanelBtns->AddButton(ID_FLIP_BOARD_BTN, "Flip Board", bmpFlipBoard);
			/*wxRibbonToolBar *toolbar2 = new wxRibbonToolBar(selection_panel, ID_MAIN_TOOLBAR2); 
			toolbar2->AddHybridTool(ID_SQUARE,wxBitmap(chessboard_icon_xpm));
			toolbar2->AddHybridTool(ID_EDIT_PGN_HEADER,bmpEditPGNHeader);
			toolbar2->AddHybridTool(ID_FLIP_BOARD,bmpFlipBoard);
			toolbar2->SetRows(1, 2);*/
		}

		wxRibbonPanel *toolbar_panel22 = new wxRibbonPanel(BatchAnalysisPage, wxID_ANY, wxT("Toolbar"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *playThroughBtns = new wxRibbonButtonBar(toolbar_panel22);

			playThroughBtns->AddButton(ID_PLAY_THROUGH_GAME, "Run Game",playThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_STOP_PLAY_THROUGH_GAME, "Stop Game",stopPlayThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_PLAY_THROUGH_GAME_LIST, wxT("Play Through List"), playThroughGame );
			playThroughBtns->AddButton(ID_MERGE_ALL_GAMES, "Merge",bmpPromoteVariation,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_MERGE_SELECTED_GAMES, "Merge Selected",bmpPromoteVariation,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			
			playThroughBtns->AddButton(ID_REMOVE_GAME_FROM_DB, "Delete",removeGameFromDB,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_CHECK_ALL, "Select All",checkAll,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_UNCHECK_ALL, "Deselect",uncheckAll,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_DATAVIEW_DELETE_ALL_SELECTED_GAMES, "Delete Selections",removeGameFromDB,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			
			playThroughBtns->AddButton(ID_COPY_GAME_LIST_GAME, "Copy Game", copyGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_MOVE_UP, "Move Up", moveUp,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			playThroughBtns->AddButton(ID_MOVE_DOWN, "Move Down", moveDown,wxNullBitmap,wxNullBitmap,wxNullBitmap);	
		}
		

	}



	//--------------------------------------------------------------------------------------
	// Opening Page
	//--------------------------------------------------------------------------------------
	wxRibbonPage *OpeningPage = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Openings"), ribbon_xpm);
	{
        wxRibbonPanel *shapes_panel = new wxRibbonPanel(OpeningPage, wxID_ANY, wxT("Functions"), wxBitmap(circle_small_xpm), wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        {
			wxRibbonButtonBar *shapes = new wxRibbonButtonBar(shapes_panel);
			//shapes->AddButton(ID_CIRCLE, wxT("Engine"), wxBitmap(circle_xpm), wxBitmap(circle_small_xpm), wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Add Engine");
			shapes->AddButton(ID_CROSS, wxT("Options"), wxBitmap(cross_xpm), wxNullBitmap,wxNullBitmap,wxNullBitmap,wxRIBBON_BUTTON_NORMAL, "Show Options Panel");
			shapes->AddHybridButton(ID_TRIANGLE, wxT("Book"), wxBitmap(triangle_xpm));
			shapes->AddButton(ID_SEARCH_FILTER, "Search",fullAnalysis,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			shapes->AddButton(ID_APPLY_FILTER_ON_SUBSET, "Filter Selection",fullAnalysis,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			//shapes->AddDropdownButton(ID_POLYGON, wxT("Wizard"), wxBitmap(hexagon_xpm), wxEmptyString);
			//shapes->AddButton(ID_SHOW_GRAPH, "Show Graph", barGraph);
		}
	}


	
	//--------------------------------------------------------------------------------------
	// Media Page
	//--------------------------------------------------------------------------------------
	wxRibbonPage *mediaPage = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Media"), ribbon_xpm);
	{
		wxRibbonPanel *mediaPanel1 = new wxRibbonPanel(mediaPage, wxID_ANY, wxT("Video"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *mediaButtons = new wxRibbonButtonBar(mediaPanel1);
			mediaButtons->AddButton(ID_LOAD_MEDIA_FROM_DISK, wxT("Disk"), bmpMediaFromDisk, "Load Media");
			mediaButtons->AddButton(ID_LOAD_MEDIA_FROM_URL, wxT("URL"), movieFromWeb, "Enter URL (youtube)");	
		}

        wxRibbonPanel *selection_panel2 = new wxRibbonPanel(mediaPage, wxID_ANY, wxT("Game"), wxBitmap(selection_panel_xpm),wxDefaultPosition, wxDefaultSize,wxRIBBON_PANEL_EXT_BUTTON);
		{
			wxRibbonToolBar *toolbar3 = new wxRibbonToolBar(selection_panel2, ID_MAIN_TOOLBAR2); 
			toolbar3->AddHybridTool(ID_SQUARE,wxBitmap(chessboard_icon_xpm));
			toolbar3->AddHybridTool(ID_EDIT_PGN_HEADER,bmpEditPGNHeader);
			toolbar3->AddHybridTool(ID_FLIP_BOARD,bmpFlipBoard);
			toolbar3->SetRows(1, 3);
		}

		wxRibbonPanel *mediaPanel2 = new wxRibbonPanel(mediaPage, wxID_ANY, wxT("Edit PGN"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_EXT_BUTTON);
		{
			wxRibbonButtonBar *editables2 = new wxRibbonButtonBar(mediaPanel2);
	    
			editables2->AddButton(ID_EDIT_ANNOTATION, wxT("Annotate"), bmpEditAnnotation, "Add Annotation");
			editables2->AddButton(ID_DELETE_REMAINING_MOVES, wxT("Delete Moves"), bmpDeleteRemainingMoves, "Delete Remaining Moves");	
			editables2->AddButton(ID_DELETE_VARIATION, wxT("Delete Variation"), bmpDeleteVariation, "Delete Variation");	
			editables2->AddButton(ID_PROMOTE_VARIATION, wxT("Promote Variation"), bmpPromoteVariation, "Promote Variation");	
		}


	}


	//--------------------------------------------------------------------------------------
	// Database Page
	//--------------------------------------------------------------------------------------
    wxRibbonPage *database_page = new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Database"), ribbon_xpm);
	{
		wxRibbonPanel *rp2db = new wxRibbonPanel(database_page, wxID_ANY, wxT("Autoplay"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *rp2bbdb = new wxRibbonButtonBar(rp2db);
			rp2bbdb->AddButton(ID_PLAY_THROUGH_GAME, "Run Game",playThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			//gcp = new GameClockPanel(rp2);
			rp2bbdb->AddButton(ID_STOP_PLAY_THROUGH_GAME, "Stop Game",stopPlayThroughGame,wxNullBitmap,wxNullBitmap,wxNullBitmap);
		}

		wxRibbonPanel *rp3db = new wxRibbonPanel(database_page, wxID_ANY, wxT("Functions"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		{
			wxRibbonButtonBar *rp3bbdb = new wxRibbonButtonBar(rp3db);
			rp3bbdb->AddDropdownButton(ID_SAVE_DB, "DataBase", loadDB);
//			rp3bbdb->AddButton(ID_ADD_GAME_TO_DB, "Add Game",addGameToDB,wxNullBitmap,wxNullBitmap,wxNullBitmap);
	//		rp3bbdb->AddButton(ID_REMOVE_GAME_FROM_DB, "Delete Game",removeGameFromDB,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			rp3bbdb->AddButton(ID_SEARCH_FILTER, "Search",fullAnalysis,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			rp3bbdb->AddButton(ID_CLEAR_DB_FILTER, "Clear Filters",clearSearchFilters,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			rp3bbdb->AddDropdownButton(ID_SORT_DB, "Sort",sortDB,"Sort Functions");
			rp3bbdb->AddDropdownButton(ID_FIND_DROPDOWN, "Find", findBmp);
			rp3bbdb->AddDropdownButton(ID_FILTER_DROPDOWN, "Filter", filterBmp);

			rp3bbdb->AddButton(ID_SHOW_FILTERS, "Show Filters",clearSearchFilters,wxNullBitmap,wxNullBitmap,wxNullBitmap);
			
			rp3bbdb->AddButton(ID_POSITION_SEARCH, "Position",wxBitmap(chessboard_icon_xpm),wxNullBitmap,wxNullBitmap,wxNullBitmap);

				
			
			/*
		ID_FILTER_DATE,
		ID_FILTER_EVENT,
		ID_FILTER_SITE,
		ID_FILTER_ELO,
		ID_FILTER_ELO_WHITE,
		ID_FILTER_ELO_BLACK,
		ID_FILTER_ECO,
		ID_FIND_NAME_WHITE,
		ID_FIND_NAME_BLACK,
		ID_SHOW_FILTERS,
		*/
			//game_functions->AddDropdownButton(ID_CLOCK, wxT("Clock"), bmpClock, "Clock functions");
		}


		

		/*
			ID_REMOVE_GAME_FROM_DB,
		ID_ADD_GAME_TO_DB,
		ID_SAVE_DB,
		ID_EXPORT_DB,
		ID_COMPRESS_DB,
		ID_EDIT_MODE,
		*/
	}

	//--------------------------------------------------------------------------------------
	// Training
	//--------------------------------------------------------------------------------------
    wxRibbonPage *TrainingPage =new wxRibbonPage(m_ribbon, wxID_ANY, wxT("Training"), ribbon_xpm);
    {
		wxRibbonPanel *toolbar_panel2 = new wxRibbonPanel(TrainingPage, wxID_ANY, wxT("Toolbar"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_NO_AUTO_MINIMISE);
        {
			wxRibbonToolBar *toolbar2 = new wxRibbonToolBar(toolbar_panel2, ID_MAIN_TOOLBAR);
			toolbar2->AddHybridTool(wxID_PRINT, wxArtProvider::GetBitmap(wxART_PRINT, wxART_OTHER, wxSize(16, 15)));
			toolbar2->SetRows(1, 3);  
		}
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


	wxCommandEvent tgmEvt;
	ToggleMaximize(tgmEvt);
	ToggleMaximize(tgmEvt);
	
	SetIcon(icon);


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
        AddColourToGallery(gallery, wxT("!"), dc, &def);
    gallery->SetSelection(def_item);
    AddColourToGallery(gallery, wxT("!!"), dc, &def);
    AddColourToGallery(gallery, wxT("!?"), dc, &def);
    AddColourToGallery(gallery, wxT("?"), dc, &def);
    AddColourToGallery(gallery, wxT("??"), dc, &def);
    AddColourToGallery(gallery, wxT("?!"), dc, &def);
    //AddColourToGallery(gallery, wxT("CYAN"), dc);
    /*AddColourToGallery(gallery, wxT("DARK GREEN"), dc);
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
	*/
    return gallery;
}

void MDIParentFrame::ApplyFilterOnSubset(wxRibbonButtonBarEvent& evt)
{
	subframe->ApplyFilterOnSubset();
}

void MDIParentFrame::OnApplySearchFilter(wxRibbonButtonBarEvent& evt)
{
	subframe->ApplySearchFilter();
}

void MDIParentFrame::OnClearSearchFilter(wxRibbonButtonBarEvent& evt)
{
	subframe->ClearSearchFilter();
}

void MDIParentFrame::OnDatabaseFindName(wxCommandEvent& evt)
{
	subframe->FindName();
}

void MDIParentFrame::OnDatabaseFilterDate(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterDate();
}
void MDIParentFrame::OnDatabaseFilterEvent(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterEvent();
}
void MDIParentFrame::OnDatabaseFilterSite(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterSite();
}
void MDIParentFrame::OnDatabaseFilterElo(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterElo();
}
void MDIParentFrame::OnDatabaseFilterEloWhite(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterEloWhite();
}
void MDIParentFrame::OnDatabaseFilterEloBlack(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterEloBlack();
}
void MDIParentFrame::OnDatabaseFilterEco(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterEco();
}
void MDIParentFrame::OnDatabaseFilterNameWhite(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterNameWhite();
}
void MDIParentFrame::OnDatabaseFilterNameBlack(wxCommandEvent& evt)
{
	subframe->OnDatabaseFilterNameBlack();
}

void MDIParentFrame::OnDatabaseShowFilters(wxRibbonButtonBarEvent& evt)
{
	subframe->OnDatabaseShowFilters();
}

void MDIParentFrame::OnDatabaseFindDropdown(wxRibbonButtonBarEvent& evt)
{

    wxMenu menu;
    menu.Append(ID_FIND_NAME, wxT("Find Player"));
    menu.Append(ID_FIND_NAME_WHITE, wxT("Find Player White"));
	menu.Append(ID_FIND_NAME_BLACK, wxT("Find Player Black"));

    //menu.Append(wxID_ANY, wxT("Scalene"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnDatabaseFilterDropdown(wxRibbonButtonBarEvent& evt)
{


    wxMenu menu;
    menu.Append(ID_FILTER_DATE, wxT("Filter Dates"));
    menu.Append(ID_FIND_NAME_WHITE, wxT("Filter Events"));
	menu.Append(ID_FIND_NAME_BLACK, wxT("Filter Sites"));
	menu.Append(ID_FIND_NAME_BLACK, wxT("Filter Elo White"));
	menu.Append(ID_FIND_NAME_BLACK, wxT("Filter Elo Black"));
	menu.Append(ID_FIND_NAME_BLACK, wxT("Filter ECO"));
    //menu.Append(wxID_ANY, wxT("Scalene"));

    evt.PopupMenu(&menu);
}


//		ID_MERGE_ALL_GAMES,
void MDIParentFrame::OnMergeAll(wxRibbonButtonBarEvent& evt)
{
	subframe->OnMergeAll();
}

void MDIParentFrame::OnMergeSelected(wxRibbonButtonBarEvent& evt)
{
	subframe->OnMergeSelected();
}

void MDIParentFrame::OnMoveUp(wxRibbonButtonBarEvent& evt)
{
	subframe->OnMoveUp();
}
void MDIParentFrame::OnMoveDown(wxRibbonButtonBarEvent& evt)
{
	subframe->OnMoveDown();
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

wxString default_perspectives[7] = 
{

	"layout2|name=test1;caption=Tree Stats;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6326268;dir=2;layer=2;row=0;pos=1;prop=125599;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1213;floaty=158;floatw=400;floath=480|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2066;floaty=580;floatw=318;floath=190|name=test4;caption=Pane Caption;state=2046;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=74401;bestw=250;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2106;floaty=242;floatw=268;floath=240|name=moveData;caption=;state=2099196;dir=2;layer=2;row=0;pos=2;prop=82504;bestw=250;besth=100;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2091;floaty=242;floatw=268;floath=140|name=scoreGraph;caption=;state=2099198;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=125;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Active Games;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=129;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=20973564;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=variationBoard;caption=Variation Board;state=2099196;dir=2;layer=2;row=0;pos=3;prop=117496;bestw=300;besth=300;minw=-1;minh=-1;maxw=300;maxh=300;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=matchResults;caption=Results;state=2099198;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=200;minw=100;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=166|dock_size(4,1,0)=551|dock_size(5,0,0)=24|dock_size(2,2,0)=304|dock_size(3,0,1)=166|",
	"layout2|name=test1;caption=Tree Stats;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6326269;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=593;floaty=52;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=3;pos=0;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1480;floaty=580;floatw=318;floath=190|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2067;floaty=581;floatw=318;floath=190|name=test4;caption=Pane Caption;state=2045;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=579;floaty=279;floatw=118;floath=120|name=test5;caption=Tree Browser;state=2045;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=250;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=895;floaty=225;floatw=268;floath=240|name=moveData;caption=;state=2099198;dir=1;layer=2;row=0;pos=1;prop=100000;bestw=250;besth=100;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2337;floaty=238;floatw=268;floath=140|name=scoreGraph;caption=;state=2099196;dir=3;layer=0;row=2;pos=0;prop=100000;bestw=300;besth=125;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2339;floaty=238;floatw=318;floath=165|name=clock_pane;caption=Game Clock;state=6293500;dir=1;layer=2;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2146;floaty=191;floatw=43;floath=60|name=test7;caption=Active Games;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=129;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=20973564;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1365;floaty=235;floatw=43;floath=60|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=variationBoard;caption=Variation Board;state=2099198;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=300;minw=-1;minh=-1;maxw=300;maxh=300;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=matchResults;caption=Results;state=2099198;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=200;minw=100;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=24|dock_size(1,2,0)=41|dock_size(3,0,1)=142|dock_size(3,0,2)=97|dock_size(3,0,3)=150|dock_size(4,3,0)=539|",
	"layout2|name=test1;caption=Tree Stats;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6326270;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1123;floaty=68;floatw=400;floath=480|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2045;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=297;floaty=273;floatw=595;floath=348|name=test5;caption=Tree Browser;state=2045;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=250;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=721;floaty=68;floatw=268;floath=390|name=moveData;caption=;state=2099198;dir=2;layer=0;row=0;pos=1;prop=140476;bestw=250;besth=100;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=scoreGraph;caption=;state=2099198;dir=2;layer=0;row=0;pos=1;prop=59524;bestw=300;besth=125;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Active Games;state=6293500;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=100;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1365;floaty=409;floatw=118;floath=71|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=129;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=20973564;dir=1;layer=2;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1373;floaty=476;floatw=43;floath=60|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=variationBoard;caption=Variation Board;state=2099198;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=300;minw=-1;minh=-1;maxw=300;maxh=300;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=matchResults;caption=Results;state=2099198;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=200;minw=100;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=24|dock_size(1,2,0)=306|dock_size(4,3,0)=699|",
//	"layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=2;prop=149016;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293500;dir=2;layer=2;row=0;pos=1;prop=60108;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=419;floaty=533;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=124669;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=0;row=2;pos=0;prop=26315;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=153;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=2;layer=2;row=0;pos=0;prop=139892;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=722;floaty=528;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=4;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=419;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(2,2,0)=387|dock_size(4,0,2)=95|dock_size(3,0,4)=109|",
	//"layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=2;prop=149016;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293500;dir=2;layer=2;row=0;pos=1;prop=60108;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=419;floaty=533;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=124669;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=0;row=2;pos=0;prop=26315;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=153;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=2;layer=2;row=0;pos=0;prop=139892;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=722;floaty=528;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=4;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=419;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(2,2,0)=387|dock_size(4,0,2)=95|dock_size(3,0,4)=109|",
	"layout2|name=test1;caption=Tree Stats;state=2099196;dir=4;layer=4;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1734;floaty=202;floatw=407;floath=195|name=test2;caption=options;state=6326271;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1123;floaty=68;floatw=400;floath=480|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2044;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1940;floaty=204;floatw=118;floath=120|name=test5;caption=Tree Browser;state=2044;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=250;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2123;floaty=468;floatw=268;floath=419|name=moveData;caption=;state=2099196;dir=4;layer=2;row=0;pos=1;prop=100000;bestw=250;besth=100;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2147;floaty=199;floatw=268;floath=140|name=scoreGraph;caption=;state=2099198;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=125;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Active Games;state=6293502;dir=4;layer=1;row=1;pos=1;prop=100000;bestw=100;besth=31;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=129;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=20973564;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1374;floaty=198;floatw=43;floath=60|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=variationBoard;caption=Variation Board;state=2099198;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=300;minw=-1;minh=-1;maxw=300;maxh=300;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=matchResults;caption=Results;state=2099198;dir=2;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=200;minw=100;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=24|dock_size(1,0,1)=237|dock_size(4,2,0)=249|dock_size(4,3,0)=383|dock_size(4,4,0)=349|",
	"layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6326269;dir=3;layer=3;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=676;floaty=0;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=250;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1075;floaty=142;floatw=266;floath=234|name=moveData;caption=;state=2044;dir=2;layer=2;row=0;pos=1;prop=100000;bestw=250;besth=100;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=814;floaty=146;floatw=266;floath=134|name=scoreGraph;caption=;state=2044;dir=3;layer=0;row=3;pos=0;prop=17874;bestw=300;besth=125;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=293;floaty=383;floatw=316;floath=159|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Active Games;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293500;dir=2;layer=0;row=2;pos=0;prop=89052;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=8;floaty=344;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=4;pos=0;prop=153540;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=6;floaty=425;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=20973564;dir=4;layer=1;row=0;pos=0;prop=139534;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-5;floaty=143;floatw=37;floath=54|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=151|dock_size(5,0,0)=22|dock_size(2,2,0)=194|dock_size(3,0,3)=42|dock_size(3,0,4)=40|dock_size(2,0,2)=355|dock_size(4,1,0)=453|",
	"layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6326271;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=100;floaty=100;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-4;floaty=186;floatw=116;floath=114|name=test5;caption=Tree Browser;state=2044;dir=4;layer=0;row=1;pos=0;prop=100000;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1113;floaty=328;floatw=37;floath=54|name=moveData;caption=;state=2044;dir=4;layer=0;row=2;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1115;floaty=326;floatw=37;floath=54|name=scoreGraph;caption=;state=2044;dir=4;layer=0;row=2;pos=1;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1113;floaty=327;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Active Games;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=8;floaty=143;floatw=37;floath=54|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(1,0,1)=353|dock_size(4,2,0)=825|dock_size(4,0,1)=58|dock_size(4,0,2)=101|",
};

void MDIParentFrame::OnIdle(wxIdleEvent &event)
{
	static int pg = 1;

	if( subframe->AutosaveScheduled  )
	{
		subframe->DoAutosave();
	}

	int id = m_ribbon->GetActivePage();
/*	static wxString saved_perspectives[7] = { default_perspectives[0], 
				default_perspectives[1], default_perspectives[2], default_perspectives[3],
				default_perspectives[4], default_perspectives[5], default_perspectives[6],
	};//default_perspectives[7], default_perspectives[8]};
	*/
	static wxString saved_perspectives[6] = { default_perspectives[0], 
				default_perspectives[1], default_perspectives[2], default_perspectives[3],
				default_perspectives[4], default_perspectives[5]//, default_perspectives[6],
	};//default_perspectives[7], default_perspectives[8]};
	static bool first_time = true;
	subframe->optionsFrame->m_BoardOptions.d3dModeRecreateDevice = true;

	if( id!= pg)
	{
		subframe->board_canvas->clearFlag = true;
		for( int p = 0; p < subframe->numActivePipePanels; p++ )
		{
			if( subframe->m_pipePanel[p]->IsActive() &&
				subframe->m_pipePanel[p]->IsAnalysing())
				subframe->m_pipePanel[p]->KibitzStops();
		}

		if( first_time == true )
		{
			first_time = false;
		}
		else
		{
			saved_perspectives[pg] = subframe->m_mgr.SavePerspective();
		}
//	m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=Client Size Reporter;state=6293500;dir=3;layer=2;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=852;floaty=512;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=2;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=398;floaty=412;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=No Close Button;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Client Size Reporter;state=6293500;dir=2;layer=1;row=0;pos=1;prop=62069;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=194;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=1;floaty=193;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(3,0,1)=109|dock_size(2,1,0)=248|dock_size(3,2,0)=169|dock_size(4,3,0)=516|", true);
//	m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=Client Size Reporter;state=6293500;dir=2;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=426;floaty=381;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=4;row=0;pos=0;prop=86926;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=414;floaty=373;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=No Close Button;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Client Size Reporter;state=6293500;dir=3;layer=4;row=0;pos=1;prop=75143;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=530;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-5;floaty=143;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=3;layer=10;row=0;pos=270;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=260;floaty=201;floatw=256;floath=76|name=tb2;caption=Toolbar 2;state=2106096;dir=3;layer=10;row=0;pos=6;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-5;floaty=669;floatw=278;floath=62|dock_size(5,0,0)=34|dock_size(3,0,1)=68|dock_size(2,1,0)=248|dock_size(3,10,0)=44|dock_size(3,4,0)=169|dock_size(4,5,0)=501|",true);
		pg = id;
		switch( id )
		{
		case 0: // SET ANALYSIS VIEW
			{
				//subframe->m_mgr.LoadPerspective(subframe->perspective_all);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=419;floaty=371;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=3;layer=3;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=703;floaty=150;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,0,0)=338|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(2,2,0)=153|dock_size(3,3,0)=173|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=3;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=200;minh=200;maxw=-1;maxh=-1;floatx=948;floaty=509;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=2;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=398;floaty=412;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Game Clock;state=6293500;dir=2;layer=1;row=0;pos=1;prop=62069;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=194;floatw=96;floath=114|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=1;floaty=193;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(3,0,1)=109|dock_size(2,1,0)=248|dock_size(3,2,0)=169|dock_size(4,3,0)=516|dock_size(2,3,0)=268|", true);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=831;floaty=537;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1157;floaty=143;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=581;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=530|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,1,0)=134|dock_size(4,2,0)=302|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=714;floaty=534;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=83363;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1210;floaty=143;floatw=37;floath=54|name=moveData;caption=;state=2044;dir=2;layer=2;row=0;pos=1;prop=116637;bestw=3;besth=3;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1071;floaty=143;floatw=37;floath=38|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=5;floaty=580;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,2,0)=176|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=96958;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=959;floaty=146;floatw=37;floath=54|name=moveData;caption=Move Data;state=2044;dir=2;layer=1;row=0;pos=1;prop=103042;bestw=3;besth=3;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1148;floaty=370;floatw=37;floath=38|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=579;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,1,0)=611|dock_size(5,0,0)=22|dock_size(3,0,1)=147|dock_size(2,1,0)=186|");
				
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=6;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=408;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=3;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=722;floaty=369;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=3;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1071;floaty=521;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1352;floaty=143;floatw=37;floath=54|name=moveData;caption=;state=1020;dir=2;layer=2;row=0;pos=1;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1308;floaty=223;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2;floaty=582;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=299;floaty=147;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,2,0)=191|dock_size(3,3,0)=169|dock_size(4,5,0)=536|dock_size(4,6,0)=302|");
				subframe->m_mgr.LoadPerspective(saved_perspectives[id]);
				subframe->m_mgr.GetPane("test2").Hide();
				subframe->m_mgr.GetPane("test4").Hide();
				//
				subframe->gameMode = 0;
				subframe->enginesMatch = false;
				subframe->board_canvas->SetGameMode(0);
				
				//subframe->m_mgr.GetPane("test2").Hide();
				subframe->m_mgr.GetPane("scoreGraph").Hide();
				// important to stop animating when changing tabs
				subframe->StopAllAnimation();

				subframe->m_pipePanel[0]->SetAnalyzeMode();
				subframe->m_pipePanel[1]->SetAnalyzeMode();

				for( int i = 0; i < subframe->board_canvas->activeEngines.size(); i++)
				{
					subframe->board_canvas->activeEngines[i]->SetAnalyzeMode();
				}
				
				if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel").Show();
				}
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Show();
				}

				if( subframe->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
				{
					subframe->m_mgr.GetPane("test10").Hide();
				}

				for( int i = 0; i < subframe->numActivePipePanels; i++ )
				{
					if( subframe->m_pipePanel[i] && 
						subframe->m_pipePanel[i]->IsActive() )
					{
						subframe->m_pipePanel[i]->SetPanelNameIntoCaption();
						subframe->m_pipePanel[i]->SetGoMode(GO_INFINITE);
						subframe->m_mgr.GetPane(subframe->m_pipePanel[i]->GetPanelName()).Show(true);
						//subframe->m_pipePanel[i]->SetAnalyzeMode();
					}
				}

				subframe->m_mgr.GetPane("moveData").Hide();

				
			/*	subframe->m_mgr.GetPane(wxT("tb1")).Hide();
				subframe->m_mgr.GetPane(wxT("tb2")).Hide();
				subframe->m_mgr.GetPane(wxT("clock_pane")).Hide();*/
				subframe->m_mgr.Update();

				subframe->treeCanvas->MyRefresh();


			}
			break;
		case 1:
			{
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=426;floaty=381;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=4;row=0;pos=0;prop=86926;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=414;floaty=373;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Game Clock;state=6293500;dir=3;layer=4;row=0;pos=1;prop=75143;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=530;floatw=96;floath=114|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-5;floaty=143;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=3;layer=10;row=0;pos=270;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=260;floaty=201;floatw=256;floath=76|name=tb2;caption=Toolbar 2;state=2106096;dir=3;layer=10;row=0;pos=6;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-5;floaty=669;floatw=278;floath=62|dock_size(5,0,0)=34|dock_size(3,0,1)=68|dock_size(2,1,0)=248|dock_size(3,10,0)=44|dock_size(3,4,0)=169|dock_size(4,5,0)=501|",true);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=644;floaty=186;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099199;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=430;floaty=298;floatw=116;floath=114|name=test5;caption=Tree Browser;state=2044;dir=2;layer=5;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=815;floaty=189;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293500;dir=1;layer=3;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=757;floaty=167;floatw=37;floath=54|name=test7;caption=Dummy1;state=6293502;dir=4;layer=4;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-4;floaty=185;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=2;layer=4;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=235;floaty=187;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(5,0,0)=22|dock_size(1,3,0)=39|dock_size(2,4,0)=599|dock_size(4,5,0)=302|dock_size(2,5,0)=67|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=881;floaty=407;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=2;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1338;floaty=214;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293500;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1224;floaty=143;floatw=37;floath=54|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=312;floaty=580;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=402|dock_size(5,0,0)=22|dock_size(4,2,0)=302|dock_size(1,0,1)=67|dock_size(3,0,1)=109|dock_size(2,1,0)=63|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=4;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=917;floaty=143;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=2;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=3;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=900;floaty=185;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293500;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1158;floaty=141;floatw=37;floath=54|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=578;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=660|dock_size(5,0,0)=22|dock_size(1,0,1)=39|dock_size(3,0,1)=55|dock_size(2,3,0)=84|dock_size(2,4,0)=302|");
				subframe->m_mgr.LoadPerspective(saved_perspectives[id]);
				// important to stop animating when changing tabs
				subframe->StopAllAnimation();
				subframe->m_mgr.GetPane("test4").Hide();
				subframe->gameMode = 1;
				subframe->board_canvas->SetGameMode(1);
				subframe->m_mgr.GetPane("test2").Hide();
				subframe->m_mgr.GetPane("test5").Hide();
				//if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
				//	subframe->m_mgr.GetPane("Engine_Panel").Hide();
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Show();
				}

				if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel").Show();
				}
				//if( subframe->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
				{
					subframe->m_mgr.GetPane("test10").Hide();
				}

				subframe->m_mgr.GetPane("scoreGraph").Hide();

				if( subframe->m_mgr.GetPane("moveData").IsShown())
				{
					subframe->m_mgr.GetPane("moveData").Hide();
				}

				//subframe->m_mgr.GetPane("scoreGraph").Hide();

				subframe->gameClock->PauseGame();
				subframe->m_mgr.Update();
				subframe->m_pipePanel[0]->SetGameMode();
				subframe->m_pipePanel[1]->SetGameMode();
				subframe->board_canvas->whoseMove == BaseGraphicsBoard::YOUR_MOVE;
				if( subframe->board_canvas->GetBoard()->mov == 0 || subframe->board_canvas->GetBoard()->mov == 2 )
				{
					if( !subframe->board_canvas->IsBoardFlipped() )
					{
						subframe->board_canvas->Flip();
						subframe->board_canvas->MyRefresh(false);
					}
					subframe->m_pipePanel[0]->SetEngineColor(BLACK);
					subframe->m_pipePanel[1]->SetEngineColor(WHITE);
				}
				else
				{
					if( subframe->board_canvas->IsBoardFlipped() )
					{
						subframe->board_canvas->Flip();
						subframe->board_canvas->MyRefresh(false);
					}
					subframe->m_pipePanel[0]->SetEngineColor(WHITE);
					subframe->m_pipePanel[1]->SetEngineColor(BLACK);
				}

				bool bFindOpponent = false;
				if( !subframe->m_pipePanel[0]->IsActive())
				{
					bFindOpponent = true;
				}
				else
				{
					subframe->playersOpponent = subframe->m_pipePanel[0];
				}

				for( int i = 0; i < subframe->numActivePipePanels; i++ )
				{
					if( subframe->m_pipePanel[i] && 
						subframe->m_pipePanel[i]->IsActive() )
					{
						if( bFindOpponent )
						{
							subframe->playersOpponent = subframe->m_pipePanel[i];
							bFindOpponent = false;
						}
						subframe->m_pipePanel[i]->SetPanelNameIntoCaption();
						subframe->m_pipePanel[i]->SetGoMode(GO_TIME);
						subframe->m_pipePanel[i]->SetTimeMode(subframe->gameClock->m_timeMode);
						subframe->m_mgr.GetPane(subframe->m_pipePanel[i]->GetPanelName()).Show(true);
					}
				}

				subframe->treeCanvas->MyRefresh();

				static bool first_time_page_2 = true;

				if( first_time_page_2 )
				{
					wxCommandEvent tgmEvt;
					ToggleMaximize(tgmEvt);
					ToggleMaximize(tgmEvt);

					first_time_page_2 = false;
				}
			}
			break;
		case 2:
			{
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=14;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=296;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=12;row=0;pos=0;prop=112763;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=735;floaty=517;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=12;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=824;floaty=350;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=74477;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=8;row=0;pos=1;prop=93134;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1281;floaty=143;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=13;row=0;pos=0;prop=125523;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=302;floaty=146;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099196;dir=4;layer=8;row=0;pos=0;prop=94103;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=614;floaty=347;floatw=37;floath=54|name=test12;caption=Graphical Pane;state=23070716;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=785;floaty=143;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(4,8,0)=206|dock_size(2,2,0)=402|dock_size(3,12,0)=169|dock_size(4,13,0)=302|dock_size(4,14,0)=302|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=4;layer=14;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=296;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=12;row=0;pos=0;prop=112763;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=735;floaty=517;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=12;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=824;floaty=350;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=74477;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=14;row=0;pos=1;prop=93134;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=302;floaty=339;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=15;row=0;pos=0;prop=125523;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=157;floaty=143;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099196;dir=4;layer=14;row=0;pos=0;prop=94103;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=459;floaty=145;floatw=37;floath=54|name=test12;caption=Graphical Pane;state=23070716;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=785;floaty=143;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(2,2,0)=402|dock_size(3,12,0)=188|dock_size(4,14,0)=226|dock_size(4,15,0)=424|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=4;layer=14;row=0;pos=1;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=296;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=4;layer=14;row=0;pos=2;prop=52906;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=9;floaty=534;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=12;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=824;floaty=350;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=74477;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=14;row=0;pos=1;prop=93134;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=302;floaty=339;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=19;row=0;pos=0;prop=125523;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-6;floaty=143;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099196;dir=4;layer=14;row=0;pos=0;prop=94103;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=459;floaty=145;floatw=37;floath=54|name=test12;caption=Graphical Pane;state=23070716;dir=1;layer=0;row=1;pos=0;prop=159857;bestw=600;besth=600;minw=300;minh=300;maxw=-1;maxh=-1;floatx=960;floaty=143;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(4,14,0)=330|dock_size(4,19,0)=553|dock_size(1,0,1)=419|");
				//subframe->m_mgr.LoadPerspective(subframe->perspective_all);
				
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=0;floaty=0;floatw=683;floath=728|name=Engine_Panel;caption=Engine;state=6293500;dir=4;layer=1;row=0;pos=4;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1771;floaty=342;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=4;layer=1;row=0;pos=3;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1895;floaty=147;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=1;row=0;pos=1;prop=110555;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1471;floaty=7;floatw=189;floath=164|name=moveData;caption=;state=1020;dir=4;layer=1;row=0;pos=2;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1465;floaty=168;floatw=206;floath=204|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293500;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=0;floaty=0;floatw=683;floath=728|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2099196;dir=4;layer=1;row=0;pos=0;prop=89445;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1504;floaty=435;floatw=282;floath=290|name=test12;caption=Graphical Pane;state=23070716;dir=1;layer=0;row=1;pos=0;prop=100000;bestw=400;besth=400;minw=300;minh=300;maxw=-1;maxh=-1;floatx=1;floaty=281;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(1,0,1)=419|dock_size(4,1,0)=302|dock_size(4,2,0)=648|dock_size(4,3,0)=302|");
				subframe->m_mgr.LoadPerspective(saved_perspectives[id]);
				subframe->m_mgr.GetPane(wxT("tb1")).Hide();
				subframe->m_mgr.GetPane(wxT("tb2")).Hide();
				subframe->m_mgr.GetPane("test2").Hide();
				subframe->m_mgr.GetPane("test4").Hide();
				subframe->m_mgr.GetPane("test5").Hide();
				//
				// important to stop animating when changing tabs
				subframe->StopAllAnimation();

				subframe->gameMode = 0;
				subframe->board_canvas->SetGameMode(0);
				subframe->enginesMatch = false;

				for( int i = 0; i < subframe->board_canvas->activeEngines.size(); i++)
				{
					subframe->board_canvas->activeEngines[i]->SetAnalyzeMode();
				}
				subframe->m_mgr.GetPane("Engine_Panel").Hide();
				subframe->m_mgr.GetPane("Engine_Panel_2").Hide();
				
				
				/*if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel").Show();
				}
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Show();
				}*/

				//				if( !subframe->m_mgr.GetPane("moveData").IsShown())
				{
					subframe->m_mgr.GetPane("moveData").Hide();
				}
				subframe->m_mgr.Update();

				for( int i = 0; i < subframe->numActivePipePanels; i++ )
				{
					if( subframe->m_pipePanel[i] && 
						subframe->m_pipePanel[i]->IsActive() )
					{
						subframe->m_pipePanel[i]->SetPanelNameIntoCaption();
						subframe->m_pipePanel[i]->SetGoMode(GO_INFINITE);
						subframe->m_mgr.GetPane(subframe->m_pipePanel[i]->GetPanelName()).Show(true);
					}
				}

				subframe->treeCanvas->MyRefresh();
				static bool first_time_page_3 = true;

				if( first_time_page_3 )
				{
					wxCommandEvent tgmEvt;
					ToggleMaximize(tgmEvt);
					ToggleMaximize(tgmEvt);

					first_time_page_3 = false;
				}
			}
			break;
		case 3:
			{
				//subframe->m_mgr.LoadPerspective(subframe->perspective_all);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=419;floaty=371;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=3;layer=3;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=703;floaty=150;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,0,0)=338|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(2,2,0)=153|dock_size(3,3,0)=173|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=2;layer=3;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=200;minh=200;maxw=-1;maxh=-1;floatx=948;floaty=509;floatw=96;floath=114|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=2;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=398;floaty=412;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=137931;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1325;floaty=189;floatw=96;floath=114|name=clock_pane;caption=Game Clock;state=6293500;dir=2;layer=1;row=0;pos=1;prop=62069;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=194;floatw=96;floath=114|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=3;floaty=658;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=3;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=1;floaty=193;floatw=416;floath=485|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=150;floaty=150;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(3,0,1)=109|dock_size(2,1,0)=248|dock_size(3,2,0)=169|dock_size(4,3,0)=516|dock_size(2,3,0)=268|", true);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=831;floaty=537;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1157;floaty=143;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=581;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=530|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,1,0)=134|dock_size(4,2,0)=302|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=714;floaty=534;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=83363;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1210;floaty=143;floatw=37;floath=54|name=moveData;caption=;state=2044;dir=2;layer=2;row=0;pos=1;prop=116637;bestw=3;besth=3;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1071;floaty=143;floatw=37;floath=38|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=5;floaty=580;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,2,0)=176|");
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=1;row=0;pos=0;prop=96958;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=959;floaty=146;floatw=37;floath=54|name=moveData;caption=Move Data;state=2044;dir=2;layer=1;row=0;pos=1;prop=103042;bestw=3;besth=3;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1148;floaty=370;floatw=37;floath=38|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1;floaty=579;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=2;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(4,1,0)=611|dock_size(5,0,0)=22|dock_size(3,0,1)=147|dock_size(2,1,0)=186|");
				
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293500;dir=4;layer=6;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=408;floaty=144;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293500;dir=3;layer=3;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=722;floaty=369;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=3;row=0;pos=1;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1071;floaty=521;floatw=400;floath=485|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1352;floaty=143;floatw=37;floath=54|name=moveData;caption=;state=1020;dir=2;layer=2;row=0;pos=1;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1308;floaty=223;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=1;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=2;floaty=582;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=5;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=299;floaty=147;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(5,0,0)=22|dock_size(3,0,1)=109|dock_size(2,2,0)=191|dock_size(3,3,0)=169|dock_size(4,5,0)=536|dock_size(4,6,0)=302|");
				subframe->m_mgr.LoadPerspective(saved_perspectives[id]);
				subframe->m_mgr.GetPane("test2").Hide();
				//subframe->m_mgr.GetPane("test5").Hide();
				//subframe->m_mgr.GetPane("test4").Hide();
				//
				subframe->gameMode = 0;
				subframe->enginesMatch = false;
				subframe->board_canvas->SetGameMode(0);
				subframe->m_mgr.GetPane("scoreGraph").Hide();

				// important to stop animating when changing tabs
				subframe->StopAllAnimation();

				subframe->m_pipePanel[0]->SetAnalyzeMode();
				subframe->m_pipePanel[1]->SetAnalyzeMode();

				for( int i = 0; i < subframe->board_canvas->activeEngines.size(); i++)
				{
					subframe->board_canvas->activeEngines[i]->SetAnalyzeMode();
				}
				
				if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel").Show();
				}
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Hide();
				}
				else
				{
					subframe->m_mgr.GetPane("Engine_Panel_2").Show();
				}

				if( subframe->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
				{
					subframe->m_mgr.GetPane("test10").Hide();
				}

				for( int i = 0; i < subframe->numActivePipePanels; i++ )
				{
					if( subframe->m_pipePanel[i] && 
						subframe->m_pipePanel[i]->IsActive() )
					{
						subframe->m_pipePanel[i]->SetPanelNameIntoCaption();
						subframe->m_pipePanel[i]->SetGoMode(GO_INFINITE);
						//subframe->m_pipePanel[i]->SetAnalyzeMode();
						subframe->m_mgr.GetPane(subframe->m_pipePanel[i]->GetPanelName()).Show(true);
					}
				}

				if( !subframe->m_mgr.GetPane("moveData").IsShown())
				{
					subframe->m_mgr.GetPane("moveData").Show();
				}



			/*	subframe->m_mgr.GetPane(wxT("tb1")).Hide();
				subframe->m_mgr.GetPane(wxT("tb2")).Hide();
				subframe->m_mgr.GetPane(wxT("clock_pane")).Hide();*/
				subframe->m_mgr.Update();

				subframe->treeCanvas->MyRefresh();

				static bool first_time_page_4 = true;

				if( first_time_page_4 )
				{
					wxCommandEvent tgmEvt;
					ToggleMaximize(tgmEvt);
					ToggleMaximize(tgmEvt);

					first_time_page_4 = false;
				}
			}
			break;
		case 4:
			{
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=2;prop=149016;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293500;dir=2;layer=2;row=0;pos=1;prop=60108;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=419;floaty=533;floatw=400;floath=485|name=Engine_Panel_2;caption=Engine;state=6293500;dir=3;layer=0;row=0;pos=0;prop=124669;bestw=490;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099198;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test5;caption=Tree Browser;state=2044;dir=4;layer=0;row=2;pos=0;prop=26315;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=153;floatw=37;floath=54|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test9;caption=Min Size 200x100;state=6293500;dir=2;layer=2;row=0;pos=0;prop=139892;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=722;floaty=528;floatw=400;floath=485|name=test10;caption=Game Annotation;state=2099196;dir=3;layer=0;row=4;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=414;floaty=419;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2098942;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(3,0,0)=169|dock_size(4,1,0)=409|dock_size(5,0,0)=22|dock_size(2,2,0)=387|dock_size(4,0,2)=95|dock_size(3,0,4)=109|");
				subframe->m_mgr.LoadPerspective(saved_perspectives[id]);
				if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
					subframe->m_mgr.GetPane(subframe->m_pipePanel[1]->GetPanelName()).Hide();

				
				subframe->m_mgr.GetPane("test4").Hide();
				// important to stop animating when changing tabs
				subframe->StopAllAnimation();

				subframe->gameMode = 0;
				subframe->board_canvas->SetGameMode(0);
				subframe->enginesMatch = false;

				for( int i = 0; i < subframe->board_canvas->activeEngines.size(); i++)
				{
					subframe->board_canvas->activeEngines[i]->SetAnalyzeMode();
				}

				if( subframe->optionsFrame->m_NotationOptions.showAnnotationInSepWindow == false )
				{
					subframe->m_mgr.GetPane("test10").Hide();
				}

				//if( !subframe->m_mgr.GetPane("moveData").IsShown())
				{
					subframe->m_mgr.GetPane("moveData").Hide();
				}
				subframe->m_mgr.Update();

				for( int i = 0; i < subframe->numActivePipePanels; i++ )
				{
					if( subframe->m_pipePanel[i] && 
						subframe->m_pipePanel[i]->IsActive() )
					{
						subframe->m_pipePanel[i]->SetPanelNameIntoCaption();
						subframe->m_pipePanel[i]->SetGoMode(GO_INFINITE);
						subframe->m_pipePanel[i]->SetAnalyzeMode();
						subframe->m_mgr.GetPane(subframe->m_pipePanel[i]->GetPanelName()).Show(true);
					}
				}

				subframe->treeCanvas->MyRefresh();

				static bool first_time_page_5 = true;

				if( first_time_page_5 )
				{
					wxCommandEvent tgmEvt;
					ToggleMaximize(tgmEvt);
					ToggleMaximize(tgmEvt);

					first_time_page_5 = false;
				}
			}
			break;
		case 5:
			{
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099196;dir=1;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=506;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=2;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=0;floaty=359;floatw=96;floath=114|name=test5;caption=No Close Button;state=2044;dir=2;layer=2;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1323;floaty=368;floatw=96;floath=114|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070718;dir=4;layer=1;row=0;pos=0;prop=100000;bestw=600;besth=600;minw=400;minh=400;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=notebook_content;caption=;state=1020;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=430;besth=200;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=25;floaty=25;floatw=495;floath=219|name=grid_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106096;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106096;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(1,0,0)=209|dock_size(5,0,0)=34|dock_size(1,10,1)=44|dock_size(4,2,0)=339|dock_size(2,2,0)=82|",true);
				//subframe->m_mgr.LoadPerspective("layout2|name=test1;caption=Pane Caption;state=2099198;dir=1;layer=0;row=0;pos=1;prop=100000;bestw=0;besth=0;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test2;caption=options;state=6293501;dir=4;layer=10;row=0;pos=0;prop=100000;bestw=300;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=1775;floaty=86;floatw=400;floath=485|name=Engine_Panel;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=0;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=Engine_Panel_2;caption=Engine;state=6293502;dir=3;layer=0;row=0;pos=2;prop=100000;bestw=618;besth=150;minw=300;minh=150;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test4;caption=Pane Caption;state=2099196;dir=4;layer=9;row=0;pos=0;prop=100000;bestw=100;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=302;floaty=150;floatw=116;floath=114|name=test5;caption=Tree Browser;state=2044;dir=2;layer=0;row=0;pos=0;prop=106666;bestw=3;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=1088;floaty=237;floatw=37;floath=54|name=moveData;caption=;state=1020;dir=2;layer=0;row=0;pos=1;prop=93334;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=clock_pane;caption=Game Clock;state=6293502;dir=2;layer=0;row=1;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test7;caption=Dummy1;state=6293502;dir=1;layer=0;row=0;pos=0;prop=46788;bestw=100;besth=29;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=6;floaty=143;floatw=116;floath=63|name=test9;caption=Media;state=6293502;dir=3;layer=1;row=0;pos=0;prop=100000;bestw=200;besth=100;minw=200;minh=100;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test10;caption=Game Annotation;state=2099198;dir=3;layer=8;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=849;floaty=594;floatw=149;floath=124|name=test11;caption=Fixed Pane;state=2099198;dir=3;layer=1;row=0;pos=1;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=test12;caption=Graphical Pane;state=23070716;dir=1;layer=0;row=1;pos=0;prop=153212;bestw=600;besth=600;minw=300;minh=300;maxw=-1;maxh=-1;floatx=959;floaty=143;floatw=416;floath=485|name=notebook_content;caption=Notation;state=1792;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=20;besth=20;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=settings;caption=Dock Manager Settings;state=2099139;dir=4;layer=0;row=0;pos=0;prop=100000;bestw=479;besth=185;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=grid_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=233;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tree_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=160;besth=250;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=sizereport_content;caption=;state=1794;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=80;besth=80;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=text_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=133;besth=90;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=html_content;caption=;state=770;dir=5;layer=0;row=0;pos=0;prop=100000;bestw=383;besth=300;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb1;caption=Big Toolbar;state=2106098;dir=1;layer=10;row=1;pos=264;prop=100000;bestw=240;besth=42;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|name=tb2;caption=Toolbar 2;state=2106098;dir=1;layer=10;row=1;pos=0;prop=100000;bestw=262;besth=28;minw=-1;minh=-1;maxw=-1;maxh=-1;floatx=-1;floaty=-1;floatw=-1;floath=-1|dock_size(2,0,0)=39|dock_size(5,0,0)=22|dock_size(4,9,0)=850|dock_size(1,0,1)=419|");
				subframe->m_mgr.LoadPerspective(saved_perspectives[id]);
				subframe->gameMode = 0;
				subframe->board_canvas->SetGameMode(0);
				subframe->enginesMatch = false;

				// important to stop animating when changing tabs
				subframe->StopAllAnimation();

				subframe->m_mgr.GetPane("test2").Hide();
				subframe->m_mgr.GetPane("test5").Hide();
				subframe->m_mgr.GetPane("moveData").Hide();
				subframe->m_mgr.GetPane("scoreGraph").Hide();

				subframe->m_mgr.Update();
				for( int i = 0; i < subframe->board_canvas->activeEngines.size(); i++)
				{
					
					subframe->board_canvas->activeEngines[i]->SetAnalyzeMode();
				}

				for( int i = 0; i < subframe->numActivePipePanels; i++ )
				{
					if( subframe->m_pipePanel[i] && 
						subframe->m_pipePanel[i]->IsActive() )
					{
						subframe->m_pipePanel[i]->SetPanelNameIntoCaption();
						subframe->m_pipePanel[i]->SetGoMode(GO_INFINITE);
					}
				}

				subframe->treeCanvas->MyRefresh();

				static bool first_time_page_6 = true;

				if( first_time_page_6 )
				{
					wxCommandEvent tgmEvt;
					ToggleMaximize(tgmEvt);
					ToggleMaximize(tgmEvt);

					first_time_page_6 = false;
				}
			}
			break;
		case 6:
			{
				subframe->m_mgr.LoadPerspective(subframe->perspective_all);
				subframe->m_mgr.GetPane(wxT("tb1")).Hide();
				subframe->m_mgr.GetPane(wxT("tb2")).Hide();
				

				// important to stop animating when changing tabs
				subframe->StopAllAnimation();
				
				subframe->gameMode = 0;
				subframe->board_canvas->SetGameMode(0);
				subframe->enginesMatch = false;

				for( int i = 0; i < subframe->board_canvas->activeEngines.size(); i++)
				{
					subframe->board_canvas->activeEngines[i]->SetAnalyzeMode();
				}
				subframe->m_mgr.Update();

				for( int i = 0; i < subframe->numActivePipePanels; i++ )
				{
					if( subframe->m_pipePanel[i] && 
						subframe->m_pipePanel[i]->IsActive() )
					{
						subframe->m_pipePanel[i]->SetPanelNameIntoCaption();
						subframe->m_pipePanel[i]->SetGoMode(GO_INFINITE);
					}
				}
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

void MDIParentFrame::OnInserAnnotationBtn(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    AddText(wxT("Expand selection vertically button clicked."));
	wxCommandEvent evmt;
	subframe->OnInsertAnnotation(evmt);
}

void MDIParentFrame::OnToggleFullScreenButton(wxRibbonButtonBarEvent& WXUNUSED(evt))
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

void MDIParentFrame::OnEngineLoadBtn(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
    //AddText(wxT("Circle button clicked."));
	wxString cmd;
	wxString name;
	EngineListModalDialog dialog(this, subframe->m_pipePanel[0], wxID_ANY, "Engine List", &cmd);
	dialog.ShowModal();
	if( (dialog.selectedList == 1) || (dialog.selectedList == -1) )
	{
		return;
	}

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

void MDIParentFrame::OnOptions(wxRibbonButtonBarEvent& WXUNUSED(evt))
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
    menu.Append(ID_LOAD_BOOK, wxT("Open"));
    menu.Append(ID_SHOW_BOOK, wxT("Show Window"));
    //menu.Append(wxID_ANY, wxT("Scalene"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnLoadBook(wxCommandEvent& evt)
{
	BookListDialog dlg(subframe, wxID_ANY, "Book List");
	if( dlg.ShowModal() == wxID_OK )
	{
		if( !subframe->m_mgr.GetPane("moveData").IsShown() )
		{
			subframe->m_mgr.GetPane("moveData").Show();
			subframe->m_mgr.Update();
		}
	}
}

void MDIParentFrame::OnShowBookWindow(wxCommandEvent& evt)
{
	if( subframe->m_mgr.GetPane("moveData").IsShown() )
	{
		subframe->m_mgr.GetPane("moveData").Hide();
	}
	else
	{
		subframe->m_mgr.GetPane("moveData").Show();
	}
	subframe->m_mgr.Update();
}


void MDIParentFrame::OnClockDropdown(wxRibbonButtonBarEvent& evt)
{

	
	wxMenu menu;
    menu.Append(ID_CLOCK_SET_TIME_CONTROLS, wxT("Set Time Controls"));
    menu.Append(ID_CLOCK_RESET, wxT("Reset Clock"));
    menu.Append(ID_CLOCK_SET_TIME, wxT("Set Time"));
	menu.Append(ID_SHOW_CLOCK_WINDOW, wxT("Show Clock"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnShowClock(wxCommandEvent& evt)
{
	if( !subframe->m_mgr.GetPane("clock_pane").IsShown())
	{
		subframe->m_mgr.GetPane("clock_pane").Show();
	}
}

// TIME_CONTROL
void MDIParentFrame::OnClockSetTimeControls(wxCommandEvent& evt)
{
	//wxMessageBox("OnClockSetTimeControls");
	TimeControlDialog dlg(subframe, NULL, this, wxID_ANY,"Time Control Dialog");
	dlg.ShowModal();
	if(  dlg.res== wxID_OK )
	{
		subframe->gameClock->SetTimeControl(0,dlg.start_seconds,dlg.start_inc_millis,1);
		if( dlg.bFischer == false )
		{
			subframe->gameClock->UseFideTimeControl();
			subframe->gameClock->SetNumMovesBeforeTimeControl(dlg.time_control_one_num_moves);
			subframe->gameClock->SetNumMillisAfterTimeControl( dlg.time_control_one_seconds*1000);
			subframe->gameClock->SetIncrementAfterTimeControl( dlg.time_control_one_inc_millis );
		}
	}

	
	//wxMenu menu;
	//menu.AddItem(
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

void MDIParentFrame::OnDatabasePositionSearch(wxRibbonButtonBarEvent& evt)
{
	subframe->PositionSearch();
}



void MDIParentFrame::OnSetupPosition(wxCommandEvent& evt)
{
	subframe->StopAllAnimation();
	for( int i = 0; i < subframe->numActivePipePanels; i++ )
	{
		if( subframe->m_pipePanel[i]->IsActive() && 
			subframe->m_pipePanel[i]->IsAnalysing() )
		{
			subframe->m_pipePanel[i]->KibitzStops();
		}
	}
	PositionSetupFrame *positionSetupFrame = new PositionSetupFrame(this->subframe, false);

	positionSetupFrame->Show(true);
	//delete positionSetupFrame;
}

void MDIParentFrame::OnPlayThroughGame(wxRibbonButtonBarEvent& evt)
{
	subframe->playThroughGameList = false;
	subframe->PlayThroughGame();
}

void MDIParentFrame::OnStopPlayThroughGame(wxRibbonButtonBarEvent& evt)
{
	subframe->StopPlayThroughGame();
}


void MDIParentFrame::OnPlayThroughGameList(wxRibbonButtonBarEvent& evt)
{
	subframe->ResetPlayThroughData();
	subframe->OnPlayThroughGameList(0);
}

void MDIParentFrame::OnCheckAll(wxRibbonButtonBarEvent& evt)
{
	subframe->OnCheckAll();
}

void MDIParentFrame::OnUnCheckAll(wxRibbonButtonBarEvent& evt)
{
	subframe->OnUnCheckAll();
}

void MDIParentFrame::OnDeleteAllSelectedGames(wxRibbonButtonBarEvent& evt)
{
	subframe->DeleteAllSelectedGames();
}


void MDIParentFrame::OnCopyGame(wxRibbonButtonBarEvent& evt)
{
	subframe->CopyGame();
}

void MDIParentFrame::OnRemoveGameFromDB(wxRibbonButtonBarEvent& evt)
{
	subframe->OnRemoveGameFromDB();
}

void MDIParentFrame::OnAddGameToDB(wxRibbonButtonBarEvent& evt)
{
	//subframe->OnAddGameToDB();
	wxCommandEvent evt2;
	subframe->OnLoadPGN(evt2);
}

void MDIParentFrame::OnSaveDBInfo(wxCommandEvent& evt)
{
	subframe->OnSaveDB();
}

void MDIParentFrame::OnSaveDB(wxRibbonButtonBarEvent& evt)
{
	//subframe->OnSaveDB();
	wxMenu menu;

	if( subframe->pgnLoad.SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM )
	{
		menu.Append(ID_LOAD_PGN, wxT("Load New PGN in RAM mode"));
	}
	if( (subframe->pgnLoad.SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM) &&
		(subframe->pgnLoad.SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_RAM))
	{
		menu.Append(ID_LOAD_PGN_VM, wxT("Load New PGN in Virtual Mode"));
	}
	if( (subframe->pgnLoad.SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM) )
	{
		menu.Append(ID_ADD_PGN, wxT("Add PGN in RAM mode"));
	}
	if((subframe->pgnLoad.SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_VM))
	{
		menu.Append(ID_ADD_PGN_VM, wxT("Add PGN in Virtual mode"));
	}
	if( (subframe->pgnLoad.SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_VM) &&
		(subframe->pgnLoad.SORTABLE_INDEX_MODE != SORTABLE_GAME_INDEX_MODE_RAM))
	{
		menu.Append(ID_OPEN_DB_NEW, wxT("Open a Database"));
	}
	if( (subframe->pgnLoad.SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_VM) )
	{
		menu.Append(ID_EXPORT_DB, wxT("Export Database"));
	}
   // menu.Append(ID_CLOCK_SET_TIME, wxT("Set Time"));

    evt.PopupMenu(&menu);
}

void MDIParentFrame::OnOpenDB_New(wxCommandEvent& evt)
{
	subframe->OnOpenDB_New();
}

void MDIParentFrame::OnLoadDB(wxCommandEvent& evt)
{
	wxCommandEvent ev2;
	subframe->OnLoadDB(ev2);
}
/*
		ID_SORT_DB,
		ID_SORT_BY_NUMBER,
		ID_SORT_BY_WNAME,
		ID_SORT_BY_BNAME,
		ID_SORT_BY_WELO,
		ID_SORT_BY_BELO,
		ID_SORT_BY_ECO,
		ID_SORT_BY_EVENT,
		ID_SORT_BY_SITE,
		ID_SORT_BY_DATE,
		ID_SORT_BY_RESULT,
		ID_SORT_BY_MOVES,
*/
void MDIParentFrame::OnSort(wxRibbonButtonBarEvent& evt)
{
	wxMenu menu;
    menu.Append(ID_SORT_BY_NUMBER, wxT("Sort By Number"));
	menu.Append(ID_SORT_BY_WNAME, wxT("Sort By White Name"));
    menu.Append(ID_SORT_BY_BNAME, wxT("Sort By Black Name"));
	menu.Append(ID_SORT_BY_WELO, wxT("Sort By White Elo"));
	menu.Append(ID_SORT_BY_BELO, wxT("Sort By Black Elo"));
	menu.Append(ID_SORT_BY_ECO, wxT("Sort By ECO"));
	menu.Append(ID_SORT_BY_EVENT, wxT("Sort By Event"));
	menu.Append(ID_SORT_BY_SITE, wxT("Sort By Site"));
	menu.Append(ID_SORT_BY_DATE, wxT("Sort By Date"));
	menu.Append(ID_SORT_BY_RESULT, wxT("Sort By Result"));
	menu.Append(ID_SORT_BY_MOVES, wxT("Sort By Moves"));

	 evt.PopupMenu(&menu);
}
void MDIParentFrame::RemoveEmptyGamesFromDB(wxCommandEvent& evt)
{
	subframe->RemoveEmptyGamesFromDB();
}
void MDIParentFrame::SortByMoves(wxCommandEvent& evt)
{
	subframe->SortByMoves();
}
void MDIParentFrame::SortByNumber(wxCommandEvent& evt)
{
	subframe->SortByNumber();
}
void MDIParentFrame::SortByWhiteName(wxCommandEvent& evt)
{
	subframe->SortByWhiteName();
}
void MDIParentFrame::SortByBlackName(wxCommandEvent& evt)
{
	subframe->SortByBlackName();
}
void MDIParentFrame::SortByWhiteElo(wxCommandEvent& evt)
{
	subframe->SortByWhiteElo();
}
void MDIParentFrame::SortByBlackElo(wxCommandEvent& evt)
{
	subframe->SortByBlackElo();
}
void MDIParentFrame::SortByECO(wxCommandEvent& evt)
{
	subframe->SortByECO();
}
void MDIParentFrame::SortByEvent(wxCommandEvent& evt)
{
	subframe->SortByEvent();
}
void MDIParentFrame::SortBySite(wxCommandEvent& evt)
{
	subframe->SortBySite();
}
void MDIParentFrame::SortByDate(wxCommandEvent& evt)
{
	subframe->SortByDate();
}
void MDIParentFrame::SortByResult(wxCommandEvent& evt)
{
	subframe->SortByResult();
}

void MDIParentFrame::OnLoadIndividualGame(wxCommandEvent &evt)
{
	subframe->OnLoadIndividualGame();
}

void MDIParentFrame::OnLoadGameListToActive(wxCommandEvent &evt)
{
	subframe->OnLoadGameListToActive();
}

void MDIParentFrame::OnLoadPGN_VM(wxCommandEvent& evt)
{
	subframe->OnLoadPGN_VM();
}
void MDIParentFrame::OnAddPGN_VM(wxCommandEvent& evt)
{
	subframe->OnAddPGN_VM();
}
void MDIParentFrame::OnLoadDB_VM(wxCommandEvent& evt)
{
	subframe->OnLoadDB_VM();
}

void MDIParentFrame::OnExportDB(wxRibbonButtonBarEvent& evt)
{
	subframe->OnExportDB();
}

void MDIParentFrame::OnCompressDB(wxRibbonButtonBarEvent& evt)
{
	subframe->OnCompressDB();
}

void MDIParentFrame::OnEditMode(wxRibbonButtonBarEvent& evt)
{
	subframe->OnEditMode();
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

void MDIParentFrame::OnEngineMatchBtn(wxRibbonButtonBarEvent& evt)
{

	if( subframe->enginesMatch == true )
		return;
	

	EngineMatchDialog dialog(this->subframe,this,wxID_ANY,"Engine Match Dialog");
	int ret = dialog.ShowModal();
	int retID = dialog.retID;
	if( retID == 1 )
	{
		subframe->m_engineVsEngine.eventName = dialog.eventName.c_str();
		subframe->m_engineVsEngine.engine1Name = dialog.engine1;
		subframe->m_engineVsEngine.engine2Name = dialog.engine2;

		subframe->m_engineVsEngine.fixedDepth = dialog.fixedDepth;
		subframe->m_engineVsEngine.incrementMillis = dialog.incrementMillis;
		subframe->m_engineVsEngine.incrementMinutes = dialog.incrementMinutes;
		subframe->m_engineVsEngine.incrementSeconds = dialog.incrementSeconds;
		subframe->m_engineVsEngine.timeInMilliSecs = dialog.timeInMilliSecs;
		subframe->m_engineVsEngine.timeInSeconds = dialog.timeInSeconds;
		subframe->m_engineVsEngine.timeInMinutes = dialog.timeInMinutes;
		subframe->m_engineVsEngine.numGames = dialog.numGames;
		subframe->m_engineVsEngine.playSamePositionForEachColor = dialog.playSamePositionForEachColor;
		subframe->m_engineVsEngine.timeControlType = dialog.timeControlType;
		subframe->m_engineVsEngine.timeControlID = dialog.timeControlID;
		subframe->m_engineVsEngine.logEngineOutput = dialog.logEngineOutput;
		subframe->m_engineVsEngine.timeControlType = dialog.timeControlType;
		subframe->m_engineVsEngine.numGamesPlayed = 0;
		subframe->m_engineVsEngine.swapSidesBetweenGames = dialog.swapSidesBetweenGames;
		subframe->m_engineVsEngine.plyForBook = dialog.plyForBook;
		//dialog.
		if( dialog.timeConfigured == true )
		{
			subframe->gameClock->SetTimeControl(0,dialog.start_seconds,dialog.start_inc_millis,1);
		
			if( dialog.bFischer == false )
			{
				subframe->gameClock->UseFideTimeControl();
				subframe->gameClock->SetNumMovesBeforeTimeControl(dialog.time_control_one_num_moves);
				subframe->gameClock->SetNumMillisAfterTimeControl( dialog.time_control_one_seconds*1000);
				subframe->gameClock->SetIncrementAfterTimeControl( dialog.time_control_one_inc_millis );
			}
		}

		subframe->m_engineVsEngine.writeAllToSameGame = dialog.writeIntoTree;
	}
	else
	{
		return;
	}

	//---------------------------------------------------------------
	// first locate the listed engine if available...
	if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
	{
		wxMessageBox("Load 2 engines first");
		return;
	}
	if( subframe->m_pipePanel[1]->EngineIsLoaded() == false )
	{
		wxMessageBox("Load 2 engines first");
		return;
	}

	

	// first locate the listed engine if available...
	for( int i = 0; i < subframe->numActivePipePanels; i++ )
	{
		if( subframe->m_pipePanel[i] && subframe->m_pipePanel[i]->IsActive() )
		{
			if( subframe->m_engineVsEngine.engine1Name == subframe->m_pipePanel[i]->GetEngineName())
				subframe->m_engineVsEngine.engine01 = subframe->m_pipePanel[i];

			if( subframe->m_engineVsEngine.engine2Name == subframe->m_pipePanel[i]->GetEngineName())
				subframe->m_engineVsEngine.engine02 = subframe->m_pipePanel[i];
		}
	}

	if( ((subframe->m_engineVsEngine.engine02 == NULL)|| (!subframe->m_engineVsEngine.engine02->IsActive()) ) ||
		((subframe->m_engineVsEngine.engine01 == NULL)|| (!subframe->m_engineVsEngine.engine01->IsActive()) ) )
	{
		wxMessageBox("Load 2 engines first");
		return;
	}
	//---------------------------------------------------------------
	if( subframe->m_engineVsEngine.openings.size() > 0 )
	{
		for( int i = 0; i< subframe->m_engineVsEngine.openings.size(); i++ )
		{
			delete subframe->m_engineVsEngine.openings[i];
			
		}
		subframe->m_engineVsEngine.openings.clear();
	}
	if( subframe->currentGame->pCurrMove != subframe->currentGame->pRoot )
	{
		PGN::Game *gameOpening = new PGN::Game();
		gameOpening->CopyGame(subframe->currentGame);
		subframe->m_engineVsEngine.openings.push_back(gameOpening);
	}

	//engine located, now proceed
	subframe->enginesMatch = true;

	subframe->currentGame->WriteEventNameIntoHeader(subframe->m_engineVsEngine.eventName);

	if( subframe->m_engineVsEngine.list_engine_games.size() != 0 )
		subframe->m_engineVsEngine.list_engine_games.clear();

	int tickMillis;

	if( subframe->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::HYPER_BULLET )
	{
		if( dialog.timeConfigured == false )
			subframe->gameClock->SetTimeControl(0,10,0, 1);

		subframe->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(1);
		subframe->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		subframe->m_engineVsEngine.engine01->SetTimeMode(HYPER_BULLET);
		subframe->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(1);
		subframe->m_engineVsEngine.engine02->SetTimeMode(HYPER_BULLET);
		subframe->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
		
	}
	else if( subframe->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::BULLET )
	{
		if( dialog.timeConfigured == false )
			subframe->gameClock->SetTimeControl(1,0,0, 10);

		subframe->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(50);
		subframe->m_engineVsEngine.engine01->SetTimeMode(BULLET);
		subframe->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		subframe->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(50);
		subframe->m_engineVsEngine.engine02->SetTimeMode(BULLET);
		subframe->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( subframe->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::BLITZ )
	{
		if( dialog.timeConfigured == false )
			subframe->gameClock->SetTimeControl(5,0,3000, 50);

		subframe->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine01->SetTimeMode(BLITZ);
		subframe->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		subframe->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine02->SetTimeMode(BLITZ);
		subframe->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( subframe->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::RAPID )
	{
		if( dialog.timeConfigured == false )
			subframe->gameClock->SetTimeControl(30,0,0, 100);

		subframe->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine01->SetTimeMode(RAPID);
		subframe->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		subframe->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine02->SetTimeMode(RAPID);
		subframe->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( subframe->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::STANDARD )
	{
		if( dialog.timeConfigured == false )
			subframe->gameClock->SetTimeControl(60,0,0, 100);

		subframe->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine01->SetTimeMode(STANDARD);
		subframe->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		subframe->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine02->SetTimeMode(STANDARD);
		subframe->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( subframe->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::LONG_TIME )
	{
		if( dialog.timeConfigured == false )
			subframe->gameClock->SetTimeControl(90,0,0, 100);

		subframe->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine01->SetTimeMode(LONG_TIME);
		subframe->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		subframe->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		subframe->m_engineVsEngine.engine02->SetTimeMode(LONG_TIME);
		subframe->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}

	subframe->m_engineVsEngine.list_engine_games.push_back(subframe->currentGame);
	subframe->m_engineVsEngine.Setup();
	subframe->board_canvas->whoseMove = BaseGraphicsBoard::WAIT_COMPUTER;
	subframe->m_engineVsEngine.engine01->SetGameMode();
	subframe->m_engineVsEngine.engine02->SetGameMode();

	subframe->currentGame->SetHasGeneratedFens();

	if( subframe->board_canvas->GetBoard()->mov != 1 )
		subframe->m_engineVsEngine.m_MCTS->SetStartNode(subframe->currentGame->pCurrMove, 0 ); 
	else
		subframe->m_engineVsEngine.m_MCTS->SetStartNode(subframe->currentGame->pCurrMove, 1 ); 
	
	
	subframe->gameClock->StartGame();
	subframe->gameClock->UnPause();

	if( subframe->board_canvas->GetBoard()->mov != 1 )
		subframe->m_engineVsEngine.engine01->PlayerMoves();
	else
		subframe->m_engineVsEngine.engine02->PlayerMoves();

	subframe->m_mgr.GetPane("Engine_Panel").Show();
	subframe->m_mgr.GetPane("Engine_Panel_2").Show();

	subframe->currentGame->whiteName = subframe->m_engineVsEngine.engine01->GetEngineName();
	subframe->currentGame->blackName = subframe->m_engineVsEngine.engine02->GetEngineName();
	
	for( int i = 0; i < subframe->currentGame->vec_header.size(); i++ )
	{
		if( subframe->currentGame->vec_header[i].data.find("White ")!=-1)
		{
			wxString str = subframe->currentGame->vec_header[i].data;
			SetSubstringInQuotes(str, subframe->m_engineVsEngine.engine01->GetEngineName());
			subframe->currentGame->vec_header[i].data = str;
		}
		if( subframe->currentGame->vec_header[i].data.find("Black ")!=-1) 
		{
			wxString str = subframe->currentGame->vec_header[i].data;
			SetSubstringInQuotes(str, subframe->m_engineVsEngine.engine02->GetEngineName());
			subframe->currentGame->vec_header[i].data = str;

		}
	}
	subframe->noteFrame->Refresh();
}

void MDIParentFrame::OnSwapSides(wxRibbonButtonBarEvent& evt)
{
	if( subframe->m_pipePanel[0]->EngineIsLoaded() == false )
	{
		if( !(subframe->playersOpponent) || 
			(subframe->playersOpponent->EngineIsLoaded() == false))
		{
			wxMessageBox("Load an engine first");
			return;
		}
	}

	for( int i = 0; i < subframe->board_canvas->activeEngines.size(); i++ )
	{
		if( subframe->board_canvas->activeEngines[i]->IsActive() )
		{
			subframe->board_canvas->activeEngines[i]->KibitzStops();
		}
	}

	if( !subframe->enginesMatch )
	{
		if( subframe->board_canvas->whoseMove == BaseGraphicsBoard::YOUR_MOVE)
		{
			subframe->board_canvas->whoseMove = BaseGraphicsBoard::WAIT_COMPUTER;
			if( subframe->playersOpponent->GetEngineColor() == WHITE )
				subframe->playersOpponent->SetEngineColor(BLACK);
			if( subframe->playersOpponent->GetEngineColor() == BLACK )
				subframe->playersOpponent->SetEngineColor(WHITE);
			if( subframe->board_canvas->activeEngines.size() > 0 )
				if( subframe->playersOpponent->IsActive() )
					subframe->playersOpponent->PlayerMoves();

			if( subframe->gameClock->IsPaused() )
			{
				subframe->gameClock->UnPause();
			}

			subframe->board_canvas->Flip();
			subframe->board_canvas->MyRefresh(false);
		}
		else if( subframe->board_canvas->whoseMove == BaseGraphicsBoard::WAIT_COMPUTER)
		{
			subframe->board_canvas->whoseMove = BaseGraphicsBoard::YOUR_MOVE;
			if( subframe->playersOpponent->GetEngineColor() == WHITE )
				subframe->playersOpponent->SetEngineColor(BLACK);
			if( subframe->playersOpponent->GetEngineColor() == BLACK )
				subframe->playersOpponent->SetEngineColor(WHITE);

			if( subframe->gameClock->IsPaused() )
			{
				subframe->gameClock->UnPause();
			}

			subframe->board_canvas->Flip();
			subframe->board_canvas->MyRefresh(false);
		}
	}
	

}


void MDIParentFrame::OnFlipBoardBtn(wxRibbonButtonBarEvent& evt)
{
	subframe->board_canvas->Flip();
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

void MDIParentFrame::AjournGame(wxRibbonButtonBarEvent& evt)
{
	subframe->AjournGame();
}

void MDIParentFrame::OnStopEnginesPlayingBtn(wxRibbonButtonBarEvent& evt)
{
	if( subframe->enginesMatch == false )
		return;

	for( int i = 0; i < subframe->numActivePipePanels; i++ )
	{
		if( subframe->m_pipePanel[i]->IsActive() )
		{
			subframe->m_pipePanel[i]->KibitzStops();
		}
	}

	if( subframe->m_engineVsEngine.engine01 &&subframe->m_engineVsEngine.engine01->IsActive())
		subframe->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);

	if( subframe->m_engineVsEngine.engine02 && subframe->m_engineVsEngine.engine02->IsActive())
		subframe->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);

	subframe->gameClock->Stop();
	subframe->enginesMatch = false;
}



void MDIParentFrame::OnShowGraph(wxRibbonButtonBarEvent& evt)
{
	if( subframe->m_mgr.GetPane("scoreGraph").IsShown())
		subframe->m_mgr.GetPane("scoreGraph").Hide();
	else
		subframe->m_mgr.GetPane("scoreGraph").Show();

	subframe->m_mgr.Update();
}

void MDIParentFrame::OnVariationBoard(wxRibbonButtonBarEvent& evt)
{
	if( subframe->m_mgr.GetPane("variationBoard").IsShown())
		subframe->m_mgr.GetPane("variationBoard").Hide();
	else
		subframe->m_mgr.GetPane("variationBoard").Show();

	subframe->m_mgr.Update();
}

// button bar start menus.
void MDIParentFrame::OnClipboardBtn(wxRibbonButtonBarEvent& evt)
{
}
void MDIParentFrame::OnClipboardDropdownBtn(wxRibbonButtonBarEvent& evt)
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
void MDIParentFrame::OnOpenBtn(wxRibbonButtonBarEvent& evt)
{
}
void MDIParentFrame::OnOpenDropdownBtn(wxRibbonButtonBarEvent& evt)
{
	wxMenu menu;
		
	int id = m_ribbon->GetActivePage();
	
	menu.Append(ID_LOAD_SINGLE_GAME,wxT("Load Single PGN game"));
	if( id == 2 )
	{
		menu.Append(ID_LOAD_GAME_LIST_TO_ACTIVE, wxT("Load PGN Game List"));
	}
	menu.Append(ID_MERGE_PGN_FROM_DISK, wxT("Merge PGN from Disk (single game RAM)"));
	evt.PopupMenu(&menu);
}
void MDIParentFrame::OnSaveBtn(wxRibbonButtonBarEvent& evt)
{
}
void MDIParentFrame::OnSaveDropdownBtn(wxRibbonButtonBarEvent& evt)
{
	wxMenu menu;
	menu.Append(ID_SAVE_PGN, wxT("Save Game"));
	menu.Append(ID_SAVE_GAME_LIST_AS_DATABASE, wxT("Save Game List as PGN Database"));
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

	menu.Append(ID_LOAD_PGN, wxT("Load New PGN in RAM mode"));
	menu.Append(ID_LOAD_PGN_VM, wxT("Load New PGN in Virtual Mode"));
	menu.Append(ID_ADD_PGN, wxT("Add PGN in RAM mode"));
	menu.Append(ID_ADD_PGN_VM, wxT("Add PGN in Virtual mode"));
		menu.Append(ID_MERGE_PGN_FROM_DISK, wxT("Merge PGN from Disk (Single Game, RAM)"));
//	}

	evt.PopupMenu(&menu);
}
void MDIParentFrame::OnSave(wxRibbonToolBarEvent& evt)
{
}
void MDIParentFrame::OnSaveDropdown(wxRibbonToolBarEvent& evt)
{
	int id = m_ribbon->GetActivePage();

	if( id == 0 )
	{
		wxMenu menu;
		menu.Append(ID_SAVE_PGN, wxT("Save Game"));
		menu.Append(ID_SAVE_GAME_LIST_AS_DATABASE, wxT("Save Game to Database"));
		evt.PopupMenu(&menu);
	}
	else if( id == 4 )
	{
		wxMenu menu;
		menu.Append(ID_SAVE_PGN, wxT("Save Selected Game"));
		menu.Append(ID_SAVE_GAME_LIST_AS_DATABASE, wxT("Save Game List as PGN Database"));
		evt.PopupMenu(&menu);
	}
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

void MDIParentFrame::OnShowResults(wxRibbonButtonBarEvent& WXUNUSED(evt))
{

	subframe->ShowResults();
}

void MDIParentFrame::OnPlayerResign(wxRibbonButtonBarEvent& WXUNUSED(evt))
{
	subframe->PlayerResign();
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


// button bar version
void MDIParentFrame::OnEditPGNHeaderBtn(wxRibbonButtonBarEvent& evt)
{
	subframe->EditPGNHeader();
}

// toolbar version
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
	wxMessageBox("Sources Written by David Nash 2011, Copyrighted, All rights reserved.");
}


wxRibbonGalleryItem* MDIParentFrame::AddColourToGallery(wxRibbonGallery *gallery,
                                 wxString colour, wxMemoryDC& dc,
                                 wxColour* value)
{
    wxRibbonGalleryItem* item = NULL;
    wxColour c = *wxWHITE;

    if(c.IsOk())
    {
        const int iWidth = 16;
        const int iHeight = 20;

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
//#ifdef DrawText
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


void MDIParentFrame::OnFullAnalysis(wxRibbonButtonBarEvent& evt)
{
	subframe->playThroughGameList = false;
	subframe->OnFullAnalysis();
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
            "pre-release"
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

	enginesMatch = false;

	numActivePipePanels = 2;

	m_pipePanel[0]->SetEngineManager(m_engineManager,0);
	m_pipePanel[1]->SetEngineManager(m_engineManager,1);


	m_listPanel = new MyListPanel(this);
	this->selectedDBItem = 0;
	listHasBeenSorted = false;

	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	//RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	m_listPanel->RecreateList(wxLC_REPORT | wxLC_VIRTUAL);


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
	my_canvas->SetScrollbars( 10, 10, 50, 10024 );	
	
	//wxMessageBox("Setting managed window");
	// tell wxAuiManager to manage this frame
    m_mgr.SetManagedWindow(this);
	//m_mgr.SetArtProvider(::wxAuiDockArt::
	//m_mgr.SetArtProvider(::wxAui
    // set frame icon
//    SetIcon(wxIcon(sample_xpm));
	//wxLog_SetActiveTarget(wx.LogStderr()) ;
	wxLog *logger=new wxLogStream(&cout);
	wxLog::SetActiveTarget(logger);


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
    /*wxAuiToolBarItemArray prepend_items;
    wxAuiToolBarItemArray append_items;
    wxAuiToolBarItem item;
    item.SetKind(wxITEM_SEPARATOR);
    append_items.Add(item);
    item.SetKind(wxITEM_NORMAL);
    item.SetId(ID_CustomizeToolbar);
    item.SetLabel(_("Customize..."));
    append_items.Add(item);
	*/



	
    // create some toolbars
/*    wxAuiToolBar* tb1 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
    tb1->SetToolBitmapSize(wxSize(48,48));
    tb1->AddTool(ID_SampleItem+1, wxT("Test"), wxArtProvider::GetBitmap(wxART_ERROR));
    tb1->AddSeparator();
	*/
    /*wxImage img(wxT("images/flipboard32.bmp"));
        if ( img.Ok() )
        {
            if ( img.GetWidth() > 32 && img.GetHeight() > 32 )
                img = img.GetSubImage(wxRect(0, 0, 32, 32));

			tb1->AddTool(ID_SampleItem+2, wxT("Test"), img);
		}
		else*/
		/*{
			tb1->AddTool(ID_SampleItem+2, wxT("Test"), wxArtProvider::GetBitmap(wxART_QUESTION));
		}*/

	//bmap.LoadFile(wxT("images/flipboard32.png"),wxBITMAP_TYPE_PNG);
   // tb1->AddTool(ID_SampleItem+2, wxT("Test"), img);// wxArtProvider::GetBitmap(wxART_QUESTION));
    /*tb1->AddTool(ID_SampleItem+3, wxT("Test"), wxArtProvider::GetBitmap(wxART_INFORMATION));
    tb1->AddTool(ID_SampleItem+4, wxT("Test"), wxArtProvider::GetBitmap(wxART_WARNING));
    tb1->AddTool(ID_SampleItem+5, wxT("Test"), wxArtProvider::GetBitmap(wxART_MISSING_IMAGE));
    tb1->SetCustomOverflowItems(prepend_items, append_items);
    tb1->Realize();


    wxAuiToolBar* tb2 = new wxAuiToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                         wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);
										 */


	//tb2->SetBackgroundColour(*wxGREY);
	//tb2->SetForegroundColour(*wxBLACK);
  /*  tb2->SetToolBitmapSize(wxSize(16,16));
  */
/*	wxBitmap beginBmp;
	beginBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/begin_small.png", wxBITMAP_TYPE_PNG);

	wxBitmap prevBmp;
	prevBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/back_small.png", wxBITMAP_TYPE_PNG);

	wxBitmap playBmp;
	playBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/play_small_green.png", wxBITMAP_TYPE_PNG);

	wxBitmap nextBmp;
	nextBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/forward_small.png", wxBITMAP_TYPE_PNG);

	wxBitmap endBmp;
	endBmp.LoadFile(wxStandardPaths::Get().GetResourcesDir()+"/icons/dev icons sekkumu/end_small.png", wxBITMAP_TYPE_PNG);
*/
    /*wxBitmap tb2_bmp1 = wxArtProvider::GetBitmap(wxART_QUESTION, wxART_OTHER, wxSize(16,16));
    tb2->AddTool(ID_SampleItem+6, wxT("Test"), beginBmp);
    tb2->AddTool(ID_SampleItem+7, wxT("Test"), prevBmp);
    tb2->AddTool(ID_SampleItem+8, wxT("Test"), playBmp);
    tb2->AddTool(ID_SampleItem+9, wxT("Test"), nextBmp);
	tb2->AddTool(ID_SampleItem+10, wxT("Test"), endBmp);
    tb2->AddSeparator();
	wxSlider *slider = new wxSlider(tb2, wxID_ANY, 0,0,10);
	
	tb2->AddControl(slider,"slider");*/
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
    /*tb2->SetCustomOverflowItems(prepend_items, append_items);
    tb2->Realize();
	*/
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
	tsc = new TreeStatsCanvas(this);
    m_mgr.AddPane(tsc, wxAuiPaneInfo().
                  Name(wxT("test1")).Caption(wxT("Tree Stats")).
                  Top());

	//m_sampleFrame = new CLCDSampleFrame(wxT("Clock"), wxDefaultPosition, wxDefaultSize);

	//wxMessageBox("creating options frame");
	
	//wxMessageBox("done creating options frame");
    m_mgr.AddPane(optionsFrame, wxAuiPaneInfo().
		Name(wxT("test2")).Caption(wxT("options")).MinSize(300,150).GripperTop(true).Float().
                  /*Bottom().Position(1).*/
				  CloseButton(true).MaximizeButton(true).DestroyOnClose(false));

    m_mgr.AddPane(m_pipePanel[0], wxAuiPaneInfo().
                  Name(wxT("Engine_Panel")).Caption(wxT("Engine")).
                  Bottom().BestSize(300,150).
                  CloseButton(true).MaximizeButton(true).DestroyOnClose(false));
	m_pipePanel[0]->SetPanelName("Engine_Panel");
    m_mgr.AddPane(m_pipePanel[1], wxAuiPaneInfo().
                  Name(wxT("Engine_Panel_2")).Caption(wxT("Engine")).
                  Bottom().Position(1).BestSize(300,150).
                  CloseButton(true).MaximizeButton(true).DestroyOnClose(false));
	m_pipePanel[1]->SetPanelName("Engine_Panel_2");
    
	
	m_mgr.AddPane(m_listPanel, wxAuiPaneInfo().
                  Name(wxT("test4")).CloseButton(false).Caption(wxT("Pane Caption")).
                  Left().Show(true));


	//wxMessageBox("Creating tree canvas");
	treeCanvas = new TreeCanvas(this);

    m_mgr.AddPane(treeCanvas, wxAuiPaneInfo().
                  Name(wxT("test5")).Caption(wxT("Tree Browser")).BestSize(250,200).
                  Right().CloseButton(false).Hide());
	
moveDataCanvas = new MoveDataCanvas(this);
    m_mgr.AddPane(moveDataCanvas, wxAuiPaneInfo().
		Name(wxT("moveData")).BestSize(250,100).//CaptionVisible(false).
                  Right().CloseButton(true).DestroyOnClose(false).Show());
	
	//m_scoreGraph = new ScoreGraph(this, this);
	m_scoreGraphPanel = new ScorePanel(this);
	//m_scoreGraph->Hide();
    m_mgr.AddPane(m_scoreGraphPanel, wxAuiPaneInfo().
		Name(wxT("scoreGraph")).BestSize(300,125).//CaptionVisible(false).
		Right().CloseButton(true).DestroyOnClose(false).Hide());
				  
	//wxMessageBox("Creating game clock");
gameClock = new GameClockPanel(this);//CreateSizeReportCtrl()
    m_mgr.AddPane(gameClock, wxAuiPaneInfo().
                  Name(wxT("clock_pane")).Caption(wxT("Game Clock")).
                  Right().Row(1).
				  CloseButton(true).DestroyOnClose(false).MaximizeButton(true));

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
                  Name(wxT("test7")).Caption(wxT("Active Games")).
                  Left().Layer(1).
                  CloseButton(true).MaximizeButton(true));
				  

  /*  m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().
                  Name(wxT("test8")).Caption(wxT("Tree Pane")).
                  Left().Layer(1).Position(2).
                  CloseButton(true).MaximizeButton(true));
				  */

	m_mediactrl = new wxMediaCtrl(this, wxID_ANY);
	m_mediactrl->ShowPlayerControls();

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
   board_canvas = new D3D9Canvas(this);
   //((D3D9Canvas*)board_canvas)->Ini
 /*  bool ok = m_mgr.AddPane(CreateBoardCanvas(), wxAuiPaneInfo().
		Name(wxT("test12")).Caption(wxT("Graphical Pane")).BestSize(wxSize(600,600)).
                  Left().Layer(1).Position(1).DestroyOnClose(false).PinButton(true).
                  CloseButton(true).MaximizeButton(true));*/
	//wxMessageBox("Done creating board canvas");
	//wxMessageBox(wxString("Bye"));
   m_graphicsModeSetting = GRAPHICS_MODE::D3D_GRAPHICS;

bool ok = m_mgr.AddPane((D3D9Canvas*)board_canvas, wxAuiPaneInfo().
		Name(wxT("test12")).Caption(wxT("Graphical Pane")).
                  Left().Layer(1).Position(1).DestroyOnClose(false).PinButton(true).
                  CloseButton(false).MaximizeButton(true));

if( optionsFrame->m_BoardOptions.useBoardBitmaps == false )
{
	board_canvas->colBlack = optionsFrame->m_BoardOptions.colBlack;
	board_canvas->colWhite = optionsFrame->m_BoardOptions.colWhite;
	board_canvas->newMaterials = true;
}
				  
	if( !ok )
		return;

	//wxMessageBox("Adding noteframe");
	m_mgr.AddPane(noteFrame, wxAuiPaneInfo().Name(wxT("notebook_content")).Caption(wxT("Notation"))
		.CenterPane().DestroyOnClose(false).CaptionVisible(true));
		

	m_variationBoardFrame = new VariationBoardFrame(this);

   m_mgr.AddPane(m_variationBoardFrame, wxAuiPaneInfo().
	   Name(wxT("variationBoard")).Caption(wxT("Variation Board")).BestSize(wxSize(300,300))./*MinSize(wxSize(300,300)).*/MaxSize(wxSize(300,300)).
	   Right().Layer(2).CloseButton(true).MaximizeButton(false));//.Fixed());


   match_results = new MatchResultsPanel(this);
   m_mgr.AddPane(match_results, wxAuiPaneInfo().
	   Name(wxT("matchResults")).Caption(wxT("Results")).
	   Right().BestSize(wxSize(300,200)).MinSize(wxSize(100,100)).CloseButton(true).MaximizeButton(false).Hide());
  	
   
  
   //m_m
/*m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().
                  Name(wxT("notebook_content")).Caption(wxT("NoteBook")).
                  Left().Layer(1).Position(1).
                  CloseButton(true).MaximizeButton(true));

	    m_mgr.AddPane(CreateGL(), wxAuiPaneInfo().Name(wxT("test12")).Caption(wxT("Graphical Pane")).BestSize(wxSize(600,600)).MinSize(wxSize(600,600)).
		CenterPane().PaneBorder(true).Floatable(true).Movable(true).Dockable(true));
		*/
		//CreateGL();
	//wxMessageBox("Creating a bunch of optional center panes");
    //m_mgr.AddPane(new SettingsPanel(this,this), wxAuiPaneInfo().
    //              Name(wxT("settings")).Caption(wxT("Dock Manager Settings")).
     //             Dockable(false).Float().Hide());

    // create some center panes

    //m_mgr.AddPane(CreateGrid(), wxAuiPaneInfo().Name(wxT("grid_content")).
     //             CenterPane().Hide());

  //  m_mgr.AddPane(CreateTreeCtrl(), wxAuiPaneInfo().Name(wxT("tree_content")).
   //               CenterPane().Hide());

    //m_mgr.AddPane(CreateSizeReportCtrl(), wxAuiPaneInfo().Name(wxT("sizereport_content")).
//		CenterPane().CaptionVisible(true).Hide());

    //m_mgr.AddPane(CreateTextCtrl(), wxAuiPaneInfo().Name(wxT("text_content")).
     //             CenterPane().Hide());

    //m_mgr.AddPane(CreateHTMLCtrl(), wxAuiPaneInfo().Name(wxT("html_content")).
    //              CenterPane().Hide());

	//wxMessageBox("Done adding center panes");

	//wxMessageBox("Adding toolbars");
    // add the toolbars to the manager
    /*m_mgr.AddPane(tb1, wxAuiPaneInfo().
                  Name(wxT("tb1")).Caption(wxT("Big Toolbar")).
                  ToolbarPane().Top().Row(1).Position(2).
                  LeftDockable(false).RightDockable(false));

    m_mgr.AddPane(tb2, wxAuiPaneInfo().
                  Name(wxT("tb2")).Caption(wxT("Toolbar 2")).
                  ToolbarPane().Top().Row(1).
                  LeftDockable(false).RightDockable(false));
				  */
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
	//m_mgr.GetPane(wxT("test4")).Hide();
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
	
	LoadECOClassifier();

	polyGlotBookName = "Large_2500.bin";
	m_mgr.GetPane("moveData").Caption("Book: Large_2500.bin");
	string polyGlotBookPath = wxStandardPaths::Get().GetResourcesDir() +"/Books/polyglot/" + polyGlotBookName;
	polyglotBook = fopen(polyGlotBookPath.c_str(), "r+b");
	
	if( !polyglotBook )
	{
		wxMessageBox("Failed to load Default Book", "Application Error");
	}
	else
	{
	//	wxMessageBox("Success opening Default Book", "Application Error");
	}


	m_engineVsEngine.m_MCTS = new MCTS_Analysis(this);

	fullAnalysisMode = false;

	enginesMatch = false;

	engineVsHumanMatch = false;


	pgnViewMode = FILE_IN_RAM;


	m_autoSaveTimer = NULL;

	this->ActivateAutosave();

	this->AutosaveScheduled = false;


// wxCriticalSectionLocker lock(m_csCancelled);

//	m_cancelled = false;

	//OnToggleD3DMode();

    // wxColour sash = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_SASH_COLOUR);
     //   m_sash_color->SetBitmapLabel(CreateColorBitmap(sash));

#ifdef THEME_BUILD
	GetDockArt()->SetColor(wxAUI_DOCKART_BORDER_COLOUR, *wxLIGHT_GREY);
	GetDockArt()->SetColor(wxAUI_DOCKART_BACKGROUND_COLOUR, THEME_COLOR_B);
    GetDockArt()->SetColor(wxAUI_DOCKART_SASH_COLOUR, THEME_COLOR_C);
#else
	GetDockArt()->SetColor(wxAUI_DOCKART_SASH_COLOUR, wxColour( 200, 200, 255, 128));
#endif
	GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR, THEME_COLOR_C);
GetDockArt()->SetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR, wxColour(100,100,100,128));
m_mgr.GetArtProvider()->SetMetric(wxAUI_DOCKART_GRADIENT_TYPE,wxAUI_GRADIENT_HORIZONTAL);//, gradient);

//              case ID_InactiveCaptionColor:         var = wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR; break;
  //          case ID_InactiveCaptionGradientColor: var = wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR; break;
	
        GetDockArt()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,2);
DoUpdate();
//	m_pipePanel[0]->ActivateDataLogging();

	//optionsFrame->m_BoardOptions.d3dModeRecreateDevice = true;

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
	if( polyglotBook )
	{
		fclose(polyglotBook );
	}
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
//	board_canvas->Refresh(false);
	my_canvas->Refresh();


	if(  !(optionsFrame->m_GeneralAppOptions.updateScoreGraphOnKeyUp) )
	{
		
		if( m_mgr.GetPane("scoreGraph").IsShown() )
		{
			//this->m_scoreGraph->Refresh();
			this->m_scoreGraphPanel->Refresh();
		}
		this->DoLookupFromBook();

		this->moveDataCanvas->Refresh();
		
	}


	treeCanvas->MyRefresh();
	//moveDataCanvas->Refresh();
	//this->noteFrame->Refresh();
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
	if( m_gameTraverseTimer )
		m_gameTraverseTimer->Stop();
	board_canvas->bAnimating = false;	

	if( uripath.IsReference() )
	{
            if( !m_mediactrl->Load(uripath.GetPath()) )
            {
                wxMessageBox(wxT("Couldn't load Media file!"));
                //m_playlist->SetItem(nNewId, 0, wxT("E"));
            }
			else
			{
				//m_mediactrl->Play();
			}
			

	}
	else
	{
            if( !m_mediactrl->Load(uripath) )
            {
                wxMessageBox(wxT("Couldn't load Media URL!"));
               // currentpage->m_playlist->SetItem(nNewId, 0, wxT("E"));
            }
			//m_mediactrl->GetBestSize();

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

	if( !pMove->IsThisMainLine() )
	{
		if( !pMove->IsTrueDisplayVariations() )
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

			if( variation_auto_fold )
			{
				if( !pMove->IsThisBranchOfMove(currentGame->pCurrMove))
				{
					currentGame->pCurrMove->SetDisplayVariations(false);
				}
			}

			currentGame->pCurrMove = pMove;

			if( variation_auto_fold )
			{
				currentGame->pCurrMove->SetDisplayVariations(true);
				if( currentGame->pCurrMove->pNext )
				{
					if( currentGame->pCurrMove->pNext->pVarWhite )
					{
						currentGame->pCurrMove->pNext->SetDisplayVariations(true);
						my_canvas->needsRefresh = true;
						my_canvas->Refresh(false);
					}
				}
				if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
				{
					my_canvas->needsRefresh = true;
					my_canvas->Refresh(false);
				}
			}
			if( whiteMove )
			{
				if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
				//DoLookupFromBook();
				this->bookLookupCount = 0;
				my_canvas->white_black_move = BLACK;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;

				// updata thee progress data
				if( m_dataView->IsShown() )
				{
					m_dataView->SetSelectedItemProgress((int) 100*((float)my_canvas->move_count/(float)currentPlayGamePly));
					m_dataView->Refresh(true);
				}

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
				if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
				//DoLookupFromBook();
				this->bookLookupCount = 0;
				my_canvas->white_black_move = WHITE;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;

					// updata thee progress data
				if( m_dataView->IsShown() )
				{
					m_dataView->SetSelectedItemProgress((int) 100*((float)my_canvas->move_count/(float)currentPlayGamePly));
					m_dataView->Refresh(true);
				}

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
			//if( !board_canvas->bAnimating )
			{	
				for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
				{
					if( my_canvas->activeEngines[j]->IsActive() &&
						my_canvas->activeEngines[j]->IsAnalysing())
					{
						my_canvas->activeEngines[j]->KibitzStartsMove();
						if( fullAnalysisMode )
						{
							my_canvas->activeEngines[j]->CopyAllToNotation();
						}
						my_canvas->activeEngines[j]->KibitzUserEndsMove();
					}
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
				if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
				
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
				//DoLookupFromBook();
				this->bookLookupCount = 0;
				my_canvas->white_black_move = BLACK;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;

				// updata thee progress data
				if( m_dataView->IsShown() )
				{
					m_dataView->SetSelectedItemProgress((int) 100*((float)my_canvas->move_count/(float)currentPlayGamePly));
					m_dataView->Refresh(true);
				}
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
				
				if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
				
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
				//DoLookupFromBook();
				this->bookLookupCount = 0;
				my_canvas->white_black_move = WHITE;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;

				// updata thee progress data
				if( m_dataView->IsShown() )
				{
					m_dataView->SetSelectedItemProgress((int) 100*((float)my_canvas->move_count/(float)currentPlayGamePly));
					m_dataView->Refresh(true);
				}
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

			//if( !board_canvas->bAnimating )
			{
				for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
				{
					if( my_canvas->activeEngines[j]->IsActive() &&
						my_canvas->activeEngines[j]->IsAnalysing())
					{
						my_canvas->activeEngines[j]->KibitzStartsMove();
						if( fullAnalysisMode )
						{
							my_canvas->activeEngines[j]->CopyAllToNotation();
						}
						my_canvas->activeEngines[j]->KibitzUserEndsMove();
					}
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
	PGN::Move* pMove;
	if( this->gameMode == 0 ) 
		pMove = treeCanvas->lastMovePtr;
	else 
	{
		// SET TREE PANE
		if( m_mgr.GetPane("test5").IsShown() )
		{
			pMove = treeCanvas->lastMovePtr;
		}
		else pMove = currentGame->pCurrMove;
	}

	bool whiteMove;
	int mov = board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	PGN::Move* pRoot = NULL;

	if( pMove != NULL )
	{
		pRoot = pMove->GetVariationRoot();
	}

	if( ( (treeCanvas->selectedElem->pMove != NULL) && 
		  (treeCanvas->selectedElem->pMove->pUp != NULL)))
	{
		if( !pMove->IsTrueDisplayVariations() )
		{
			pMove->SetDisplayVariations(true);
			my_canvas->needsRefresh = true;
		}
	}

	if( pRoot != NULL )
	{
		PGN::Move * pMovePrev = pMove;
		if( whiteMove )
		{
			this->bookLookupCount = 0;
			int col = pMove->GetVariationRootColor();
			if( treeCanvas->selectedElem->pMove != NULL )
			{
				if( variation_auto_fold )
				{
					if( !treeCanvas->selectedElem->pMove->IsThisBranchOfMove(currentGame->pCurrMove))
					{
						currentGame->pCurrMove->SetDisplayVariations(false);
					}
				}
				currentGame->pCurrMove = treeCanvas->selectedElem->pMove;

				if( variation_auto_fold )
				{
					currentGame->pCurrMove->SetDisplayVariations(true);
					if( currentGame->pCurrMove->pNext )
					{
						if( currentGame->pCurrMove->pNext->pVarWhite )
						{
							currentGame->pCurrMove->pNext->SetDisplayVariations(true);
							my_canvas->needsRefresh = true;
							my_canvas->Refresh(false);
						}
					}
					if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
					{
						my_canvas->needsRefresh = true;
						my_canvas->Refresh(false);
					}
				}
			}
			else
			{
				bool bInsert = false;
				string pgnMove = treeCanvas->selectedElem->bookMove;

					int white_black = 0;
					if( board_canvas->GetBoard()->mov == 0 || board_canvas->GetBoard()->mov == 2 )
					{
						white_black = 0;
					}
					else
					{

						white_black = 1;
					}
					//if( (pgnMove != currentGame->pCurrMove->pPrev->black) && ( pgnMove != currentGame->pCurrMove->white ) )
					if( treeCanvas->selectedElem->white_black != white_black )
					{
						bInsert = true;
					}

					if( bInsert )
					{
						board_canvas->GetBoard()->parseMove(pgnMove);
						string fen = board_canvas->GetBoard()->Fen();
					//MyChessMove mv = *board_canvas->GetBoard()->GetLastMove();
					


					
						
						PGN::MoveInserter mv(pgnMove,white_black, fen, board_canvas->GetBoard()->GetLastMove());
						board_canvas->GetBoard()->SetBoard(fen);
						int hits = -1;

						currentGame->pCurrMove = currentGame->pCurrMove->InsertMove(&mv, &hits); 
						this->my_canvas->needsRefresh = true;
						DoLookupFromBook();
					}
			}

			if( currentGame->pCurrMove == pRoot )
			{
				if( col != 0 )
				{
					//currentGame->pCurrMove = pRoot;

				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
					
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					//DoLookupFromBook();
					my_canvas->white_black_move = BLACK;
					board_canvas->clearFlag = true;

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
					//board_canvas->moveForward = true;
				}
				else
				{
						
					if( pRoot->pPrev )
					{
						if( variation_auto_fold )
						{
							if( !pRoot->pPrev->IsThisBranchOfMove(currentGame->pCurrMove))
							{
								currentGame->pCurrMove->SetDisplayVariations(false);
							}
						}
						currentGame->pCurrMove = pRoot->pPrev;
						if( variation_auto_fold )
						{
							currentGame->pCurrMove->SetDisplayVariations(true);
							if( currentGame->pCurrMove->pNext )
							{
								if( currentGame->pCurrMove->pNext->pVarWhite  )
								{
									currentGame->pCurrMove->pNext->SetDisplayVariations(true);
								}
							}
							if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
							{
								my_canvas->needsRefresh = true;
								my_canvas->Refresh(false);
							}
						}
					}
					else
					{
						if( variation_auto_fold )
						{
							if( !pRoot->IsThisBranchOfMove(currentGame->pCurrMove))
							{
								currentGame->pCurrMove->SetDisplayVariations(false);
							}
						}
						currentGame->pCurrMove = pRoot;
						if( variation_auto_fold )
						{
							currentGame->pCurrMove->SetDisplayVariations(true);
							if( currentGame->pCurrMove->pNext )
							{
								if( currentGame->pCurrMove->pNext->pVarWhite  )
								{
									currentGame->pCurrMove->pNext->SetDisplayVariations(true);
									my_canvas->needsRefresh = true;
									my_canvas->Refresh(false);
								}	
							}
							if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
							{
								my_canvas->needsRefresh = true;
								my_canvas->Refresh(false);
							}
						}
					}

					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
					
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					//DoLookupFromBook();
					my_canvas->white_black_move = WHITE;
					board_canvas->clearFlag = true;

					board_canvas->moveForward = true;
					board_canvas->renderMoveNow = true;
				//	board_canvas->Refresh(false);
					if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wnd10->Clear();
						wxString inpt ="";
						for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
						{
							inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
							//inpt+= " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}
					//board_canvas->moveForward = true;
				}
			}
			else
			{
			
				if( (optionsFrame->m_GameOptions.animate == true) && (gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
					
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
				//DoLookupFromBook();
				my_canvas->white_black_move = BLACK;

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
							//m_owner->board_canvas->clearFlag = true;
			}


			/*if( !currentGame->pCurrMove->IsThisMainLine() )
			{
				PGN::Move* pVarRoot = currentGame->pCurrMove->GetVariationRoot();
				if( !pVarRoot->IsTrueDisplayVariations() )
				{
					pVarRoot->SetDisplayVariations(true);
					my_canvas->needsRefresh = true;
				
				}
			
			}*/			


			my_canvas->ScrollToCursor();
		//	my_canvas->Refresh();
			DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			//treeCanvas->Refresh();
			

		}
		else
		{
			//PGN::Move * pMovePrev = pMove;
			this->bookLookupCount = 0;
			int col = pMovePrev->GetVariationRootColor();

			if( treeCanvas->selectedElem->pMove != NULL )
			{
				if( variation_auto_fold )
				{
					if( !treeCanvas->selectedElem->pMove->IsThisBranchOfMove(currentGame->pCurrMove))
					{
						currentGame->pCurrMove->SetDisplayVariations(false);
					}
				}
				currentGame->pCurrMove = treeCanvas->selectedElem->pMove;
				if( variation_auto_fold )
				{
					currentGame->pCurrMove->SetDisplayVariations(true);
					if( currentGame->pCurrMove->pNext )
					{
						if( currentGame->pCurrMove->pNext->pVarWhite )
						{
							currentGame->pCurrMove->pNext->SetDisplayVariations(true);
							my_canvas->needsRefresh = true;
							my_canvas->Refresh(false);
						}
					}
					if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
					{
						my_canvas->needsRefresh = true;
						my_canvas->Refresh(false);
					}
				}
			}
			else
			{
				bool bInsert = false;
				string pgnMove = treeCanvas->selectedElem->bookMove;

					int white_black = 0;
					if( board_canvas->GetBoard()->mov == 0 || board_canvas->GetBoard()->mov == 2 )
					{
						white_black = 0;

					}
					else
					{

						white_black = 1;
					}

				//if( (pgnMove != currentGame->pCurrMove->pPrev->black) && ( pgnMove != currentGame->pCurrMove->white ) )
				if( treeCanvas->selectedElem->white_black != white_black)
					{
						bInsert = true;
					}				
				if( bInsert )
				{
					board_canvas->GetBoard()->parseMove(pgnMove);
					string fen = board_canvas->GetBoard()->Fen();
					//MyChessMove mv = *board_canvas->GetBoard()->GetLastMove();
					

						
					PGN::MoveInserter mv(pgnMove,white_black, fen, board_canvas->GetBoard()->GetLastMove());
					board_canvas->GetBoard()->SetBoard(fen);
					int hits = -1;
					currentGame->pCurrMove = currentGame->pCurrMove->InsertMove(&mv, &hits); 
					this->my_canvas->needsRefresh = true;
					DoLookupFromBook();
				}
			}
				//	currentGame->pCurrMove = pMove;

			if( currentGame->pCurrMove == pRoot )
			{

				if( col != 0 )
				{
					currentGame->pCurrMove = pRoot;
					

				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					//DoLookupFromBook();
					my_canvas->white_black_move = BLACK;
					board_canvas->clearFlag = true;

					//board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					//DoLookupFromBook();

					//my_canvas->white_black_move = WHITE;
			
					//board_canvas->clearFlag = true;
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
							//inpt += " ";
						}
						wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
					}	
				}
				else
				{
					if( pRoot->pPrev )
					{
						if( variation_auto_fold )
						{
							if( !pRoot->pPrev->IsThisBranchOfMove(currentGame->pCurrMove) )
							{
								currentGame->pCurrMove->SetDisplayVariations(false);
							}
							//my_canvas->needsRefresh = true;
						}
						
						currentGame->pCurrMove = pRoot->pPrev;
						if( variation_auto_fold )
						{
							currentGame->pCurrMove->SetDisplayVariations(true);
							if( currentGame->pCurrMove->pNext )
							{
								if( currentGame->pCurrMove->pNext->pVarWhite )
								{
									currentGame->pCurrMove->pNext->SetDisplayVariations(true);
									my_canvas->needsRefresh = true;
									my_canvas->Refresh(false);
								}
							}
							if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
							{
								my_canvas->needsRefresh = true;
								my_canvas->Refresh(false);
							}
						}
					}
					else
					{
						if( variation_auto_fold )
						{
							if( !pRoot->IsThisBranchOfMove(currentGame->pCurrMove) )
							{
								currentGame->pCurrMove->SetDisplayVariations(false);
							}
							//my_canvas->needsRefresh = true;
						}
						currentGame->pCurrMove = pRoot;
						if( variation_auto_fold )
						{
							currentGame->pCurrMove->SetDisplayVariations(true);
							if( currentGame->pCurrMove->pNext )
							{
								if( currentGame->pCurrMove->pNext->pVarWhite )
								{
									currentGame->pCurrMove->pNext->SetDisplayVariations(true);
									my_canvas->needsRefresh = true;
									my_canvas->Refresh(false);
								}
							}	
							if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
							{
								my_canvas->needsRefresh = true;
								my_canvas->Refresh(false);
							}
						}
					}

				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					//DoLookupFromBook();
					my_canvas->white_black_move = WHITE;
					board_canvas->clearFlag = true;

					//board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					//DoLookupFromBook();

					//my_canvas->white_black_move = WHITE;
			
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
				}
			}
			else
			{
				if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
			
			
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
				//DoLookupFromBook();

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
			}

				
			
					
			/*if( !currentGame->pCurrMove->IsThisMainLine() )
			{
				PGN::Move* pVarRoot = currentGame->pCurrMove->GetVariationRoot();
				if( !pVarRoot->IsTrueDisplayVariations() )
				{
					pVarRoot->SetDisplayVariations(true);
					my_canvas->needsRefresh = true;
				
				}
			
			}*/			
			

					

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
/*	if( pRoot != NULL )
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
					if( (optionsFrame->m_GameOptions.animate == true) && (gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
					
					
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					DoLookupFromBook();
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

				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
					
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					DoLookupFromBook();
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


				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
					
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					DoLookupFromBook();
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

				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
					
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					DoLookupFromBook();
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
					

				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					DoLookupFromBook();
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
					

				
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
					DoLookupFromBook();
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
	}*/
	else
	{
		SelectMove(sel);
	}

}

void MyFrame::SelectMove(int sel)
{
	PGN::Move* pMove;
	if( currentGame->pCurrMove == currentGame->pBase )
	{

		pMove = currentGame->pRoot;
	}
	else
	{
		pMove= currentGame->GetCurrentMove();
	}
	
	int count = 0;

	bool whiteMove;
	int mov = board_canvas->GetBoard()->mov;

	if( mov == 1 ) whiteMove = false;
	else whiteMove = true;

	if( sel != 0 )
	{
		if( !pMove->IsTrueDisplayVariations() )
		{
			pMove->SetDisplayVariations(true);
			my_canvas->needsRefresh = true;
		}
	}

	
	/*while( pMove )
	{
		if( count == sel )
		{
			currentGame->pCurrMove = pMove;
		*/	
			if( whiteMove )
			{
				this->bookLookupCount = 0;
				if( treeCanvas->selectedElem->pMove != NULL )
				{
					if( variation_auto_fold )
					{
						if( !treeCanvas->selectedElem->pMove->IsThisBranchOfMove(currentGame->pCurrMove))
						{
							currentGame->pCurrMove->SetDisplayVariations(false);
						}
						//my_canvas->needsRefresh = true;
					}
					currentGame->pCurrMove = treeCanvas->selectedElem->pMove;
					if( variation_auto_fold )
					{
						currentGame->pCurrMove->SetDisplayVariations(true);
						if( currentGame->pCurrMove->pNext )
						{
							if( currentGame->pCurrMove->pNext->pVarWhite )
							{
								currentGame->pCurrMove->pNext->SetDisplayVariations(true);
								my_canvas->needsRefresh = true;
								my_canvas->Refresh(false);
							}
						}
						if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
						{
							my_canvas->needsRefresh = true;
							my_canvas->Refresh(false);
						}
					}
				}
				else
				{
					bool bInsert = false;
					string pgnMove = treeCanvas->selectedElem->bookMove;

					int white_black = 0;
					if( board_canvas->GetBoard()->mov == 0 || board_canvas->GetBoard()->mov == 2 )
					{
						white_black = 0;

					}
					else
					{

						white_black = 1;
					}
					//if( (pgnMove != currentGame->pCurrMove->pPrev->black) && ( pgnMove != currentGame->pCurrMove->white ) )
					if( treeCanvas->selectedElem->white_black != white_black )
					{
						bInsert = true;
					}
					//string pgnMove = treeCanvas->selectedElem->bookMove;
					if( bInsert )
					{
						board_canvas->GetBoard()->parseMove(pgnMove);
						string fen = board_canvas->GetBoard()->Fen();
					//MyChessMove mv = *board_canvas->GetBoard()->GetLastMove();
					

						
						PGN::MoveInserter mv(pgnMove,white_black, fen, board_canvas->GetBoard()->GetLastMove());
						board_canvas->GetBoard()->SetBoard(fen);
						int hits = -1;
						currentGame->pCurrMove = currentGame->pCurrMove->InsertMove(&mv, &hits); 
						this->my_canvas->needsRefresh = true;
						DoLookupFromBook();
					}
				}
				
				if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
				
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
				//DoLookupFromBook();
				my_canvas->white_black_move = BLACK;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
				{
					wxString inpt ="";
					wnd10->Clear();
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
				this->bookLookupCount = 0;
				if( treeCanvas->selectedElem->pMove != NULL )
				{
					if( variation_auto_fold )
					{
						if( !treeCanvas->selectedElem->pMove->IsThisBranchOfMove(currentGame->pCurrMove))
						{
							currentGame->pCurrMove->SetDisplayVariations(false);
						}
					//	my_canvas->needsRefresh = true;
					}
					currentGame->pCurrMove = treeCanvas->selectedElem->pMove;
					if( variation_auto_fold )
					{
						currentGame->pCurrMove->SetDisplayVariations(true);
						if( currentGame->pCurrMove->pNext )
						{
							if( currentGame->pCurrMove->pNext->pVarWhite )
							{
								currentGame->pCurrMove->pNext->SetDisplayVariations(true);
								my_canvas->needsRefresh = true;
								my_canvas->Refresh(false);
							}
						}					
						if( currentGame->pCurrMove->pVarWhite || currentGame->pCurrMove->pVarBlack )
						{
							my_canvas->needsRefresh = true;
							my_canvas->Refresh(false);
						}
					}
				}
				else
				{
					bool bInsert = false;
					string pgnMove = treeCanvas->selectedElem->bookMove;

					int white_black = 0;
					if( board_canvas->GetBoard()->mov == 0 || board_canvas->GetBoard()->mov == 2 )
					{
						white_black = 0;

					}
					else
					{

						white_black = 1;
					}
					//if( (pgnMove != currentGame->pCurrMove->pPrev->black) && ( pgnMove != currentGame->pCurrMove->white ) )
					if( treeCanvas->selectedElem->white_black != white_black)
					{
						bInsert = true;
					}
					//string pgn = board_canvas->->EPN_Move_to_PGN_Move(epn, promote, promotePieceID);
					//string pgnMove = treeCanvas->selectedElem->bookMove;
					if( bInsert )
					{
						board_canvas->GetBoard()->parseMove(pgnMove);
						string fen = board_canvas->GetBoard()->Fen();
						//MyChessMove mv = *board_canvas->GetBoard()->GetLastMove();
						

						
						PGN::MoveInserter mv(pgnMove,white_black, fen, board_canvas->GetBoard()->GetLastMove());
						board_canvas->GetBoard()->SetBoard(fen);
						int hits = -1;
						currentGame->pCurrMove = currentGame->pCurrMove->InsertMove(&mv, &hits); 
						this->my_canvas->needsRefresh = true;
						DoLookupFromBook();
					}
				}
				
				if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
					board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
				//DoLookupFromBook();
				my_canvas->white_black_move = WHITE;
				//	board_canvas->clearFlag = true;
				board_canvas->moveForward = true;
				board_canvas->renderMoveNow = true;
				//board_canvas->Refresh(false);
				if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
				{
					wxString inpt ="";
					wnd10->Clear();
					for( int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
					{
						inpt+= wxString(currentGame->pCurrMove->annotation_black.text[aa].c_str());
						//inpt += " ";
					}
					wnd10->AppendText(GetSubstringInCurlyBraces(inpt) );
				}
			}

			/*if( !board_canvas->bAnimating )
			{
				for( int j = 0; j < my_canvas->activeEngines.size(); j++ )
				{
					if( my_canvas->activeEngines[j]->IsActive() &&
						my_canvas->activeEngines[j]->IsAnalysing())
					{
						my_canvas->activeEngines[j]->KibitzStartsMove();
						my_canvas->activeEngines[j]->KibitzUserEndsMove();
					}
				}
			}*/
			my_canvas->ScrollToCursor();
			//my_canvas->Refresh();
			DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
			//treeCanvas->Refresh();
			
			return;
		//}
	/*	count++;
		if( this->my_canvas->white_black_move == WHITE )
			pMove = pMove->pVarWhite;
		else pMove = pMove->pVarBlack;
	}*/
}



void MyFrame::EditPGNHeader()
{
	StopAllAnimation();	

	GameInfoModalDialog dialog(this, this->currentGame,this, wxID_ANY, "GameInfo");
	dialog.ShowModal();
	this->noteFrame->Refresh();
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
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
	StopAllAnimation();

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
			my_canvas->needsRefresh = true;
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
			my_canvas->needsRefresh = true;
			UpdateAnnotationWindow();
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
		}
		
    }
}


void MyFrame::DeleteRemainingMoves()
{
	StopAllAnimation();

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
		if( currentGame->pCurrMove == currentGame->pBase )
		{
			delete currentGame->pRoot;
			currentGame->pRoot = new PGN::Move();
			currentGame->pRoot->info = "1"; // default
			currentGame->pBase->pNext = currentGame->pRoot;
			currentGame->pRoot->pPrev = currentGame->pBase;
			if( currentGame->gameBeginsAtStartPos == false )
			{
				vector<string> vec_fen;
				istringstream iss(currentGame->pBase->getFenBlack());
				copy(istream_iterator<string>(iss),
					istream_iterator<string>(),
						back_inserter<vector<string> >(vec_fen));
					
				if( vec_fen[1] != "w" )
				{
					/*currentGame->pRoot->x = 1;*/
					currentGame->pRoot->SetMoveFlag( MOVE_FLAG_X );
					
				}
				//currentGame->pRoot->SetFen
			}
			//currentGame->pCurrMove = currentGame->pCurrMove->DeleteRemainingMoves(board_canvas->GetBoard()->mov);
		}
		else
		{
			currentGame->pCurrMove = currentGame->pCurrMove->DeleteRemainingMoves(board_canvas->GetBoard()->mov);
		}
	}
	my_canvas->needsRefresh = true;
	my_canvas->Refresh();
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
	treeCanvas->MyRefresh();
}





void MyFrame::DeleteVariation()
{
	StopAllAnimation();

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

void MyFrame::StopAllAnimation()
{
	if( m_gameTraverseTimer )
		m_gameTraverseTimer->Stop();
	board_canvas->bAnimating = false;	
}


void MyFrame::PromoteVariation()
{
	StopAllAnimation();

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
		currentGame->pCurrMove->SetVariationRenderColor(0,0,0, this->m_moveColorPallete);
		currentGame->pCurrMove->PromoteVariation(currentGame->pCurrMove);
		pVarRoot->SetVariationRenderColor(0,0,255,this->m_moveColorPallete);
		if( currentGame->pRoot->pUp )
		{
			currentGame->pRoot = currentGame->pBase->pNext;
		}
		my_canvas->needsRefresh = true;
		my_canvas->Refresh();
#ifdef USE_CANVAS_SET_FOCUS
		my_canvas->SetFocus();
#endif
		treeCanvas->MyRefresh();
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

void MyFrame::OnActivate(wxActivateEvent& WXUNUSED(event))
{
   // if ( event.GetActive() && m_canvas )
   //     m_canvas->SetFocus();
}

void MyFrame::OnMove(wxMoveEvent& event)
{
	StopAllAnimation();
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

void MyFrame::OnUpdatePaste(wxUpdateUIEvent& WXUNUSED(event))
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
	StopAllAnimation();
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

void MyFrame::OnCreatePipePanel(wxCommandEvent& WXUNUSED(evt))
{
	wxString name = "Engine_Panel";
	int num = numActivePipePanels + 1;
	char buff[10];
	sprintf(buff, "_%d", num);
	name += buff;
	m_pipePanel[numActivePipePanels] = new MyPipePanel(this);
    m_mgr.AddPane( m_pipePanel[numActivePipePanels], wxAuiPaneInfo().
                  Name(name).Caption(wxT("Engine")).
                  Bottom().BestSize(300,150).
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

void MyFrame::OnInsertAnnotation(wxCommandEvent &WXUNUSED(evt))
{

}

void MyFrame::NewGame(bool removeList)
{
	StopAllAnimation();
	
	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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

	this->m_dataView->AppendGameList(&pgnLoad);

	if( gameMode == 1 )
	{
		gameClock->ResetTime();
		gameClock->PauseGame();
	}
	
	


	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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

	if( this->gameMode == 0 )
	{
		NewGameDialog ng(this, wxID_ANY);// = new NewGameDialog(this);
		ng.ShowModal();
		if( ng.retCode == NewGameDialog::ADD_GAME_TO_LIST )
		{
			NewGame(false);
			if( ng.editedHeader == true )
			{
				currentGame->CopyHeader(&(ng.game));
			
			}
			else
			{
				currentGame->SetHeaderConvenienceFunction(ng.whiteText, ng.blackText, ng.eventText);
			}
			currentGame->ParseHeader();
			noteFrame->Refresh();
			//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
			RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

			this->m_dataView->AppendGameList(&pgnLoad);
		}
		else if( ng.retCode == NewGameDialog::ADD_GAME_CLEAR_LIST )
		{
			NewGame(true);
			if( ng.editedHeader == true )
			{
				currentGame->CopyHeader(&(ng.game));
			}
			else
			{
				currentGame->SetHeaderConvenienceFunction(ng.whiteText, ng.blackText, ng.eventText);
			}
			currentGame->ParseHeader();
			noteFrame->Refresh();
			//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
			RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

			this->m_dataView->AppendGameList(&pgnLoad);
		}
	}
	else
	{
		PlayVsComputerDialog ng(this, wxID_ANY);// = new NewGameDialog(this);
		ng.ShowModal();
	//	if( res == wxID_OK )
		{
			bool OK = false;
			if( ng.retCode == PlayVsComputerDialog::ADD_GAME_TO_LIST )
			{
				NewGame(false);
				if( ng.editedHeader == true )
				{
					currentGame->CopyHeader(&(ng.game));
			
				}
				else
				{
					currentGame->SetHeaderConvenienceFunction(ng.whiteText, ng.blackText, ng.eventText);
				}
				currentGame->ParseHeader();
				noteFrame->Refresh();
				//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
				RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

				this->m_dataView->AppendGameList(&pgnLoad);
				OK = true;
			}
			else if( ng.retCode == PlayVsComputerDialog::ADD_GAME_CLEAR_LIST )
			{
				NewGame(true);
				if( ng.editedHeader == true )
				{
					currentGame->CopyHeader(&(ng.game));
				}
				else
				{
					currentGame->SetHeaderConvenienceFunction(ng.whiteText, ng.blackText, ng.eventText);
				}
				currentGame->ParseHeader();
				noteFrame->Refresh();
				//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
				RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

				this->m_dataView->AppendGameList(&pgnLoad);
				OK = true;
			}
			else if( ng.retCode == PlayVsComputerDialog::KEEP_CURRENT )
			{
				currentGame->SetHeaderConvenienceFunction(ng.whiteText, ng.blackText, ng.eventText);
			
				currentGame->ParseHeader();
				noteFrame->Refresh();
				//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
				RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

				this->m_dataView->AppendGameList(&pgnLoad);
				OK = true;
			}

			if( OK )
			{
				bool pipePanelFound = false;
				for( int i = 0; i < this->numActivePipePanels; i++ )
				{
					if( m_pipePanel[i]->GetEngineName() == ng.whiteText )
					{
						engineVsHumanMatch = true;
						pipePanelFound = true;
						this->playersOpponent = m_pipePanel[i];
						this->playersOpponent->SetEngineColor(WHITE);
						this->playersOpponent->SetGoMode(GO_TIME);
						if( board_canvas->GetBoard()->mov != 1 )
						{
							this->board_canvas->whoseMove = BaseGraphicsBoard::WAIT_COMPUTER;
							this->playersOpponent->PlayerMoves();
						}
						else
						{
							this->board_canvas->whoseMove = BaseGraphicsBoard::YOUR_MOVE;
						}					
						this->board_canvas->Flip();
						this->gameClock->Start();

						break;
					}
					else if( m_pipePanel[i]->GetEngineName() == ng.blackText )
					{
						engineVsHumanMatch = true;
						pipePanelFound = true;
						this->playersOpponent = m_pipePanel[i];
						this->playersOpponent->SetEngineColor(BLACK);
						this->playersOpponent->SetGoMode(GO_TIME);
						if( board_canvas->GetBoard()->mov != 1 )
						{
							this->board_canvas->whoseMove = BaseGraphicsBoard::YOUR_MOVE;
						}
						else
						{
							this->board_canvas->whoseMove = BaseGraphicsBoard::WAIT_COMPUTER;

							this->playersOpponent->PlayerMoves();
						
						}
						this->gameClock->Start();
						break;
					}


				}
				if( !pipePanelFound )
				{
					wxMessageBox("Error Engine Not Found", "Error");
				}
			}
		}
	}
}

void MyFrame::SetCurrentGame(int idx)
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
			
			
		}
	}

	if( pgnLoad.pgn_load_mode == 0 )
	{
	// different for Virtual Memory ... or not
		if( idx >= pgnLoad.games.size() )
		{
			wxMessageBox(wxString("Error, List element out of bounds"));	
		}
	}
	else if( pgnLoad.pgn_load_mode == 1 )
	{
		if( idx >= pgnLoad.virtualGameCount )
		{
			wxMessageBox(wxString("Error, List element out of bounds"));
		}
	}
	else  if( pgnLoad.pgn_load_mode == 2 )
	{
		if( idx >= pgnLoad.rec_count )
		{
			wxMessageBox(wxString("Error, List element out of bounds"));
		}
	}
	else if( pgnLoad.pgn_load_mode == 3 )
	{
		if( idx >= pgnLoad.virtualGameCount )
		{
			wxMessageBox(wxString("Error, List element out of bounds"));
		}
	}

	if( pgnLoad.pgn_load_mode == 0 )
	{
		for( int i = 0; i < pgnLoad.games.size(); i++ )
		{
			if( i == idx )
			{
				//PGN::Game* lastCurrGame = currentGame;
				currentGame = pgnLoad.games[i];
				currentGame->pCurrMove = currentGame->pRoot;
				if( !currentGame->HasGeneratedFens() )
				{
					//currentGame->ResetMoveList();
					// NOTE: RECOVER PGN LOAD ERROR
					if( currentGame->Parse() == 0 )
					{
						wxMessageBox(currentGame->GetParseError(), "Error");
						wxMessageBox("Cleaning Up");
					
						if( i-1 > 0 )
						{
							currentGame = pgnLoad.EraseGameAtIndex(i);
							SetCurrentGame(i-1);
							//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
							RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
						}
						else if( i+1 < pgnLoad.games.size() )
						{
							//NewGame(true);
							currentGame = pgnLoad.EraseGameAtIndex(i);
							SetCurrentGame(i+1);
							//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
							RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
						}
						else
						{
							NewGame(true);
							//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
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
						return;
					//	NewGame(false);
					
					}
					//currentGame->FixMoves();
					currentGame->ParseHeader();
				}

				if( currentGame->gameBeginsAtStartPos == false )
				{
					my_canvas->ResetBoard();
					board_canvas->GetBoard()->SetBoard(currentGame->pBase->m_fenBlack);
				}
				else
				{
					my_canvas->ResetBoard();
					board_canvas->ResetBoard();
				}
				if( currentGame->pCurrMove->white == "" )
				{
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
				}
				else if( currentGame->pCurrMove->info != "1" )
				{
					board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
				}

				//int plyCount = 0;
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

				board_canvas->GetBoard()->SetBoard(currentGame->pBase->m_fenBlack);
				board_canvas->MyRefresh(false);
				my_canvas->white_black_move = WHITE;

				currentGame->ResetMoveList();
				//currentGame->EnumerateVariations();
				//board_canvas->ResetBoard();
				this->noteFrame->Refresh();
				my_canvas->move_count = 0;
				my_canvas->Scroll(0,0);
	#ifdef USE_CANVAS_SET_FOCUS
				my_canvas->SetFocus();
	#endif
				break;
			}
		}
	}
	else if( (pgnLoad.pgn_load_mode == 1) || (pgnLoad.pgn_load_mode == 3))
	{

		PGN::Game* pGame;
		if( pgnLoad.SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
		{
			pGame = pgnLoad.GetRAMGame(idx);
		}
		else
		{
			pGame = pgnLoad.GetVMGame(idx);
		}

		bool bFoundGame = false;
		PGN::Game* pFoundGame = 0;
		int foundGameIndex = 0;
		for( int i = 0; i < pgnLoad.games.size(); i++ )
		{
			int numMatches = 0;
			if( pGame->vec_header.size() == pgnLoad.games[i]->vec_header.size() )
			{
				for( int j = 0; j < pGame->vec_header.size(); j++ )
				{
					if( pGame->vec_header[j].data == pgnLoad.games[i]->vec_header[j].data )
					{
						numMatches++;
					}
				}

				if( numMatches == pGame->vec_header.size() )
				{
					PGN::Move* tempA = pgnLoad.games[i]->pRoot->GetLastMove();
					PGN::Move* tempB = pGame->pRoot->GetLastMove();
					if( (tempA->m_fenWhite == tempB->m_fenWhite) &&
						(tempA->m_fenBlack == tempB->m_fenBlack))
					{
						bFoundGame = true;
						pFoundGame = pgnLoad.games[i];
						foundGameIndex = i;
						break;
					}
				}
			}
		}

	
		if( bFoundGame == false )
		{
			if( !pGame->Parse() )
			{
				wxMessageBox(currentGame->GetParseError(), "Error");
				wxMessageBox("Cleaning Up");

				{
					NewGame(true);
					//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
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
				return;
			}

			pgnLoad.games.push_back(pGame);

			currentGame = pGame;

			currentGame->pCurrMove = currentGame->pRoot;

			currentGame->ParseHeader();

			if( currentGame->gameBeginsAtStartPos == false )
			{
				my_canvas->ResetBoard();
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->m_fenBlack);
			}
			else
			{
				my_canvas->ResetBoard();
				board_canvas->ResetBoard();
			}

			if( currentGame->pCurrMove->white == "" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
			}
			else if( currentGame->pCurrMove->info != "1" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
			}
			else
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
			}

			//int plyCount = 0;
			//if( !currentGame->HasGeneratedFens() )
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

			board_canvas->GetBoard()->SetBoard(currentGame->pBase->m_fenBlack);
			board_canvas->MyRefresh(false);
			my_canvas->white_black_move = WHITE;
			currentGame->ResetMoveList();
			//currentGame->EnumerateVariations();
			//board_canvas->ResetBoard();
			this->m_dataView->AppendGameList(&pgnLoad);
			this->noteFrame->Refresh();
			my_canvas->move_count = 0;
			my_canvas->white_black_move = WHITE;
			my_canvas->Scroll(0,0);
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
		}
		else
		{
			delete pGame;

			currentGame = pFoundGame;

			currentGame->ResetMoveList();

			currentGame->pCurrMove = currentGame->pRoot;
			//currentGame->EnumerateVariations();
			//board_canvas->ResetBoard();
			this->m_dataView->SelectItem(foundGameIndex);
			this->noteFrame->Refresh();
			my_canvas->move_count = 0;
			my_canvas->Scroll(0,0);
	#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif

		}
	}
	else if( pgnLoad.pgn_load_mode == 2 )
	{
		PGN::Game* pGame = pgnLoad.LoadPGNFromDatabaseSelection(idx);

		bool bFoundGame = false;
		PGN::Game* pFoundGame = 0;
		int foundGameIndex = 0;
		for( int i = 0; i < pgnLoad.games.size(); i++ )
		{
			int numMatches = 0;
			if( pGame->vec_header.size() == pgnLoad.games[i]->vec_header.size() )
			{
				for( int j = 0; j < pGame->vec_header.size(); j++ )
				{
					if( pGame->vec_header[j].data == pgnLoad.games[i]->vec_header[j].data )
					{
						numMatches++;
					}
				}

				if( numMatches == pGame->vec_header.size() )
				{
					PGN::Move* tempA = pgnLoad.games[i]->pRoot->GetLastMove();
					PGN::Move* tempB = pGame->pRoot->GetLastMove();
					if( (tempA->m_fenWhite == tempB->m_fenWhite) &&
						(tempA->m_fenBlack == tempB->m_fenBlack))
					{
						bFoundGame = true;
						pFoundGame = pgnLoad.games[i];
						foundGameIndex = i;
						break;
					}
				}
			}
		}

	
		if( bFoundGame == false )
		{
			if( !pGame->Parse() )
			{
				wxMessageBox(currentGame->GetParseError(), "Error");
				wxMessageBox("Cleaning Up");

				{
					NewGame(true);
					//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
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
				return;
			}

			pgnLoad.games.push_back(pGame);

			currentGame = pGame;

			currentGame->pCurrMove = currentGame->pRoot;

			//currentGame->ParseHeader();

			if( currentGame->gameBeginsAtStartPos == false )
			{
				my_canvas->ResetBoard();
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->m_fenBlack);
			}
			else
			{
				my_canvas->ResetBoard();
				board_canvas->ResetBoard();
			}
			if( currentGame->pCurrMove->white == "" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenWhite());
			}
			else if( currentGame->pCurrMove->info != "1" )
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
			}

			//int plyCount = 0;
			//if( !currentGame->HasGeneratedFens() )
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

			board_canvas->GetBoard()->SetBoard(currentGame->pBase->m_fenBlack);
			board_canvas->MyRefresh(false);
			my_canvas->white_black_move = WHITE;
			currentGame->ResetMoveList();
			//currentGame->EnumerateVariations();
			//board_canvas->ResetBoard();
			this->m_dataView->AppendGameList(&pgnLoad);
			this->noteFrame->Refresh();
			my_canvas->move_count = 0;
			
			my_canvas->Scroll(0,0);
#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
#endif
		}
		else
		{
			delete pGame;

			currentGame = pFoundGame;

			currentGame->ResetMoveList();

			currentGame->pCurrMove = currentGame->pRoot;
			//currentGame->EnumerateVariations();
			//board_canvas->ResetBoard();
			this->m_dataView->SelectItem(foundGameIndex);
			this->noteFrame->Refresh();
			my_canvas->move_count = 0;
			my_canvas->Scroll(0,0);
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
			//activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();		
		}
	}
}

void MyFrame::OnCopyVariationToClipBoard(wxCommandEvent& WXUNUSED(evt))
{
	StopAllAnimation();

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

#define ERROR_MESSAGE wxMessageBox

void MyFrame::OnPasteFEN(wxCommandEvent& evt)
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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
	noteFrame->Refresh();
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);


	string fen = data.GetText().c_str();

	if( fen.length() == 0 || fen.length() > 100 )
	{
		ERROR_MESSAGE( "Error, Ivalid FEN Length!");
		return;
	}

	vector<string> vecFen;
		
	istringstream iss(fen);
		
	copy(istream_iterator<string>(iss), istream_iterator<string>(),
		back_inserter<vector<string> >(vecFen));

	int extraQueensCount[2]={0,0};
	int extraRooksCount[2]={0,0};
	int extraKnightsCount[2]={0,0};
	int extraBishopsCount[2]={0,0};
	int bishopsCount[2] = {0,0};
	int knightsCount[2] = {0,0};
	int rooksCount[2] = {0,0};
	int queensCount[2] = {0,0};
	int KingsCount[2]={0,0};
	int pawnsCount[2] = {0,0};

	int forwardSlashCount = 0;

	int rank = 0;
	int file = 0;
	bool badChar = false;
	for( unsigned int i = 0; i < vecFen[0].length(); i++ )
	{
		char c = vecFen[0][i];

		if( c== '/' )
		{
			rank++;
			file = 0;
		}
		else if( (c >= '1') && (c <= '8') )
		{
			file += c-'0';
		}
		else if( isalpha(c) )
		{
			if( c == 'K' )
				KingsCount[WHITE]++;
			else if( c == 'k' )
				KingsCount[BLACK]++;
			else if( c == 'Q' )
				queensCount[WHITE]++;
			else if( c == 'q' )
				queensCount[BLACK]++;
			else if( c == 'B' )
				bishopsCount[WHITE]++;
			else if( c == 'b' )
				bishopsCount[BLACK]++;
			else if( c == 'N' )
				knightsCount[WHITE]++;
			else if( c == 'n' )
				knightsCount[BLACK]++;
			else if( c == 'R' )
				rooksCount[WHITE]++;
			else if( c == 'r' )
				rooksCount[BLACK]++;
			else if( c == 'P' )
				pawnsCount[WHITE]++;
			else if( c == 'p' )
				pawnsCount[BLACK]++;
			else 
			{
				badChar = true;
				break;
			}


			file++;
				
		}
		else 
		{
			badChar = true;
			break;
		}
	}

	if( badChar )
	{
		ERROR_MESSAGE( "Error, Bad Char found in first part of FEN string, board cleared!");
		return;
	}

	if( rank > 7 )
	{
		ERROR_MESSAGE( "Error, too many ranks (/) detected in FEN string");
		return;
	}

	for( int col = 0; col < 2; col++ )
	{
		if( pawnsCount[col] > 8 )
		{
			ERROR_MESSAGE( "Error, Too many Pawns!");
			return;
		}

		if( KingsCount[col] == 0 )
		{
			ERROR_MESSAGE( "Error, Both Sides Must Have Kings!");
			return;
		}
		if( KingsCount[col] != 1)
		{
			ERROR_MESSAGE( "Error, Too many Kings!");
			return;
		}

		if( queensCount[col] > 1 )
		{
			extraQueensCount[col] = queensCount[col]-1;
		}

		if( bishopsCount[col] > 2 )
		{
			extraBishopsCount[col] = bishopsCount[col]-2;
		}
		if( knightsCount[col] > 2 )
		{
			extraKnightsCount[col] = knightsCount[col]-2;
		}

		if( rooksCount[col] > 2 )
		{
			extraRooksCount[col] = rooksCount[col]-2;
		}

		if( extraRooksCount[col] + extraKnightsCount[col] + 
			extraBishopsCount[col] + extraQueensCount[col] + pawnsCount[col] > 8 )
		{
			ERROR_MESSAGE( "Fantasy Position, Please use Position Setup instead!");
			return;
		}
	}

	if( vecFen.size() == 1)
	{
		ERROR_MESSAGE( "Error, Not Enough Substrings In FEN!");
		return;
	}
		

	bool fixMovNums = false;
	if( vecFen.size() > 6 )
	{
		ERROR_MESSAGE( "Error, Too many Substrings in FEN!");
		return; 
	}
	else
	{
		if( vecFen[1].length() > 1 )
		{
			ERROR_MESSAGE( "Error, FEN Format Length Error in 2nd Substring!");
			return;
		}
			
		if( vecFen.size() == 6 )
		{
			if( vecFen[2].length() > 4 || vecFen[2].length() == 0)
			{
				ERROR_MESSAGE( "Error, FEN Format Length Error in 3rd Substring!");
				return;
			}

			if( vecFen[2][0] != '-' )
			{
				for( unsigned int v=0;v<4; v++){
					if( vecFen[2].length() > v && 
						vecFen[2][v] != 'K' &&
						vecFen[2][v] != 'Q' &&
						vecFen[2][v] != 'k' &&
						vecFen[2][v] != 'q')
					{
						ERROR_MESSAGE( "Error, FEN Format error in Castling Rights String!");
						return;
					}
				}
			}


			if( vecFen[3].length() > 2 || vecFen[3].length() == 0 )
			{
				ERROR_MESSAGE( "Error, FEN Format Length Error in empassant string!");
				return;
			}
			else
			{
				if( vecFen[3][0] != '-' )
				{
					if( vecFen[3].length() != 2 )
					{
						ERROR_MESSAGE( "Error, FEN Format Length Error in empassant string!");
						return;
					}

					if( !(vecFen[3][0] >= 'a' && 
							vecFen[3][0] <= 'h' && 
							vecFen[3][1] >= '1' && 
							vecFen[3][1] <= '8'))
					{
						ERROR_MESSAGE( "Error, FEN Format Error in empassant string!");
						return;
					}
				}
			}
				
			if( vecFen[4].length() > 0 && vecFen[4].length() <= 2)
			{
				int fiftyMov = atoi(vecFen[4].c_str());
				if( fiftyMov > 100  || fiftyMov < 0 )
				{
					ERROR_MESSAGE( "Error, bad 50 move string, setting defaults!");
					fixMovNums = true;
				}
			}
			if( vecFen[5].length() > 4 || vecFen[4].length() < 1 )
			{
				ERROR_MESSAGE( "Error, move number string was corrupted, setting defaults!");
				fixMovNums = true;
			}
		}
				
	}
	// hopefully this will fix loading on buggy fens.
	if( vecFen.size() == 4 )
	{
		vecFen.push_back("1");
		vecFen.push_back("25");
	}

	if( fixMovNums )
	{
		if( vecFen.size() ==6 )
		{
			vecFen[4] = "1";
			vecFen[5] = "25";
		}
		else
		{
			ERROR_MESSAGE( "Error, Failed to set default move numbers, try another FEN!");
			return;
		}
	}


	// we got this far it must be ok.



	board_canvas->GetBoard()->SetBoard(fen);
	//board_canvas->Refresh(false);
	DoRefresh();
	currentGame->pBase->SetFenWhite(fen);
	currentGame->pBase->SetFenBlack(fen);

	//currentGame->pRoot->info = "1";
	if( fen != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
	{
		currentGame->gameBeginsAtStartPos = false;
	}

	vector<string> tokens;

	istringstream iss2(fen);
		copy(istream_iterator<string>(iss2),
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

	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

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

#undef ERROR_MESSAGE

void MyFrame::SaveGameToOutputStream(PGN::Game *gameToSave)
{
}

void MyFrame::OnSaveGameListAsDatabase(wxCommandEvent& evt)
{
	StopAllAnimation();
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
                        wxT("PGN files (*.pgn)|*.pgn"),
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
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}
	wxString defaultFile = "filename";
	if( currentGame->whiteName != "" && currentGame->blackName != "" )
	{
		defaultFile = currentGame->whiteName;
		defaultFile+= "_vs_";
		defaultFile+= currentGame->blackName;
		//defaultFile+= ".pgn";
	}
    wxFileDialog dialog(this,
                        wxT("Save file dialog"),
                        wxEmptyString,
                        defaultFile,
                        wxT("PGN files (*.pgn)|*.pgn"),
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


// this function needs nothing to do with VM mode, the
// merge game will be the game in RAM not the VM game
void MyFrame::OnMergePGNFromClipBoard(wxCommandEvent& WXUNUSED(evt))
{
	StopAllAnimation();
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
					
	// NOTE: RECOVER PGN LOAD ERROR
	if( newGame->Parse() == 0 )
	{
		wxMessageBox(newGame->GetParseError(), "Error");
		if( pgnLoad.games.size() > 2 )
		{
			SetCurrentGame(pgnLoad.games.size()-2);
			pgnLoad.EraseGameAtIndex(pgnLoad.games.size()-1);
		}
		else if( pgnLoad.games.size() == 2 )
		{
			pgnLoad.EraseGameAtIndex(1);
			SetCurrentGame(0);
		}
		else
		{
			NewGame(true);
		}
		return;
	}

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


//	delete pgnLoad.games[pgnLoad.games.size()-1];
//	pgnLoad.games.erase(pgnLoad.games.begin() + pgnLoad.games.size()-1);


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

	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
#ifdef USE_CANVAS_SET_FOCUS        
    my_canvas->SetFocus();
#endif
}




void MyFrame::OnMergePGNFromDisk(wxCommandEvent& WXUNUSED(evt))
{
	StopAllAnimation();
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
	wxString filter= wxT("PGN files (*.pgn)|*.pgn");

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
		
			// NOTE: RECOVER PGN LOAD ERROR
			if( newGame->Parse() == 0 )
			{
				wxMessageBox(newGame->GetParseError(), "Error");
				if( pgnLoad.games.size() > 2 )
				{
					
					SetCurrentGame(pgnLoad.games.size()-2);
					pgnLoad.EraseGameAtIndex(pgnLoad.games.size()-1);
				}
				else if( pgnLoad.games.size() == 2 )
				{
					pgnLoad.EraseGameAtIndex(1);
					SetCurrentGame(0);
				}
				else
				{
					NewGame(true);
				}
				//NewGame(true);
				return;
			}

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


		//	delete pgnLoad.games[pgnLoad.games.size()-1];
		//	pgnLoad.games.erase(pgnLoad.games.begin() + pgnLoad.games.size()-1);


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
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
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
	StopAllAnimation();
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
	StopAllAnimation();
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

//-----------------------------------------------------------------
//-----------------------------------------------------------------
void MyFrame::OnLoadPgnFromClipBoard(wxCommandEvent& WXUNUSED(evt))
{
	StopAllAnimation();
	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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

	//pgnLoad.Reset();
	//	pgnLoad.LoadPgn(str);
	PGN::Game * prevCurrentGame = currentGame;
	currentGame = pgnLoad.LoadPgnFromString(data.GetText().c_str().AsChar());

	if( currentGame == NULL )
	{
		wxMessageBox("Error on paste PGN");
		//pgnLoad.Reset();
		currentGame = prevCurrentGame;
		//NewGame(true);
		return;
	}
		//pgnLoad.ParseGames();
		//pgnLoad.ParsePgn();
	//currentGame = pgnLoad.games[0];
	// NOTE: RECOVER PGN LOAD ERROR
	if( currentGame->Parse() == 0 )
	{
		wxMessageBox(currentGame->GetParseError(), "Error");
		pgnLoad.RemoveLastGame();
		if( pgnLoad.games.size() > 2 )
		{
			SetCurrentGame(pgnLoad.games.size()-2);
			pgnLoad.EraseGameAtIndex(pgnLoad.games.size()-1);
		}
		else if( pgnLoad.games.size() == 2 )
		{
			pgnLoad.EraseGameAtIndex(1);
			SetCurrentGame(0);
		}
		else
		{
			NewGame(true);
		}
		return;
	}
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
	noteFrame->Refresh();
	DoRefresh();

#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

	m_dataView->AppendGameList(&pgnLoad);
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
        
	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}    

}


void MyFrame::OnLoadIndividualGame()
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStops();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}
			
	int load_mode = pgnLoad.pgn_load_mode;
	pgnLoad.pgn_load_mode = 0;

	wxString path;
    wxString filename;
	wxString filter= wxT("PGN files (*.pgn)|*.pgn");

    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_OPEN);

	dialog.SetFilterIndex(0);

	string str;
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			//wxString foo( wxT("Hello world") );
			char buf[200];
			strcpy( buf, (const char*)path.mb_str(wxConvUTF8) );
			str = buf;

		}
		else return;
    }


	pgnLoad.load_mode_refresh_list = false;
	
	pgnLoad.LoadPgn(str, this);

	pgnLoad.load_mode_refresh_list = true;

	//pgnLoad.pgn_load_mode = load_mode;

	currentGame = pgnLoad.games[pgnLoad.games.size()-1];

	if( currentGame->Parse() == 0 )
	{
		wxMessageBox(currentGame->GetParseError(), "Error");

		if( pgnLoad.games.size() > 2 )
		{
			SetCurrentGame(pgnLoad.games.size()-2);
			//pgnLoad.EraseGameAtIndex(pgnLoad.games.size()-1);
			pgnLoad.RemoveLastGame();
		}
		else if( pgnLoad.games.size() == 2 )
		{
			//pgnLoad.EraseGameAtIndex(1);
			SetCurrentGame(0);
			pgnLoad.RemoveLastGame();
		}
		else
		{
			NewGame(true);
		}
		
		return;
	}

	if( !GenerateFensCurrentGame() )
	{
		return;
	}

//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	if( pgnLoad.pgn_load_mode != 0 )
	{
		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	}


	pgnLoad.pgn_load_mode = load_mode;

	m_dataView->AppendGameList(&pgnLoad);
	my_canvas->Scroll(0,0);
	my_canvas->needsRefresh = true;
	noteFrame->Refresh();
	DoRefresh();

#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
  	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}      
	
}

void MyFrame::OnLoadGameListToActive()
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStops();
	//		activeEngines[j]->KibitzUserNewGame();
			
		}
	}
			
	int load_mode = pgnLoad.pgn_load_mode;
	pgnLoad.pgn_load_mode = 0;

	wxString path;
    wxString filename;
	wxString filter= wxT("PGN files (*.pgn)|*.pgn");

    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_OPEN);

	dialog.SetFilterIndex(0);

	string str;
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			//wxString foo( wxT("Hello world") );
			char buf[200];
			strcpy( buf, (const char*)path.mb_str(wxConvUTF8) );
			str = buf;

		}
		else return;
    }


	pgnLoad.load_mode_refresh_list = false;
	
	pgnLoad.LoadPgn(str, this);

	pgnLoad.load_mode_refresh_list = true;

	
	my_canvas->move_count = 0;
	currentGame->pCurrMove = currentGame->pRoot;
	board_canvas->ResetBoard();

	for( int i = 0; i < pgnLoad.games.size(); i++ )
	{
		currentGame = pgnLoad.games[i];

		if( !currentGame->HasGeneratedFens() )
		{

			if( currentGame->Parse() == 0 )
			{
				wxMessageBox(wxString::Format( "Error in game %d: %s", i, currentGame->GetParseError()), "Error");

				if( pgnLoad.games.size() > 2 )
				{
					SetCurrentGame(0);
					pgnLoad.EraseGameAtIndex(i);
					//pgnLoad.RemoveLastGame();
				}
				else
				{
					NewGame(true);
				}
		
				break;
			}

		
			if( !GenerateFensCurrentGame() )
			{
				wxMessageBox(wxString::Format("Error in Parse for game at Index %d, Clearing list and returning", i));
				//pgnLoad.EraseGameAtIndex(i);
				NewGame(true);
				return;
			}
		}
	}

	currentGame = pgnLoad.games[0];



		while( !currentGame->HasGeneratedFens() && currentGame->Parse() == 0 )
		{
			wxMessageBox(wxString::Format( "Error in game %d: %s", 0, currentGame->GetParseError()), "Error");

			if( pgnLoad.games.size() > 2 )
			{
				
				pgnLoad.EraseGameAtIndex(0);
				SetCurrentGame(0);
				//pgnLoad.RemoveLastGame();
			}
			else
			{
				NewGame(true);
			}
		
			
		}

		
		if( !currentGame->HasGeneratedFens() && !GenerateFensCurrentGame() )
		{
			wxMessageBox(wxString::Format("Error in Parse for game at Index %d, Clearing list and returning", 0));
			//pgnLoad.EraseGameAtIndex(i);
			NewGame(true);
			return;
		}
	
//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	if( pgnLoad.pgn_load_mode != 0 )
	{
		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	}

	pgnLoad.pgn_load_mode = load_mode;

	m_dataView->AppendGameList(&pgnLoad);
	my_canvas->Scroll(0,0);
	my_canvas->needsRefresh = true;
	noteFrame->Refresh();
	DoRefresh();

#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
  	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}      
}

bool MyFrame::GenerateFensCurrentGame()
{

	
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

	if( currentGame->pBase->m_fenBlack != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
		if( board_canvas->GetBoard()->mov != BLACK )
		{
			my_canvas->white_black_move = BLACK;
		}
		else
		{
			my_canvas->white_black_move = WHITE;
		}
	}
	//board_canvas->Refresh(true);
	my_canvas->move_count = 0;
	//this->my_canvas->Refresh(true);
	if( !currentGame->HasGeneratedFens() )
	{
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
	}
	currentGame->ResetMoveList();
	//currentGame->EnumerateVariations();
	board_canvas->ResetBoard();
	if( currentGame->pCurrMove->white == "" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenBlack());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = WHITE;
	}
	else if( currentGame->pCurrMove->info != "1" )
	{
		board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
		//board_canvas->Refresh(false);
		my_canvas->white_black_move = BLACK;
	}
	my_canvas->move_count = 0;

	return true;
}

void MyFrame::OpenDB(string filename)
{
	pgnLoad.Reset();
	pgnLoad.pgn_load_mode = 2;

	if( !pgnLoad.OpenSQLDatabase(filename) )
	{
		wxMessageBox("Error Loading DB");

		return;
	}

	PGN::Game *pGame = pgnLoad.LoadPGNFromDatabaseSelection(1);


	bool bFoundGame = false;
	PGN::Game* pFoundGame = 0;
	int foundGameIndex = 0;
	for( int i = 0; i < pgnLoad.games.size(); i++ )
	{
		int numMatches = 0;
		if( pGame->vec_header.size() == pgnLoad.games[i]->vec_header.size() )
		{
			for( int j = 0; j < pGame->vec_header.size(); j++ )
			{
				if( pGame->vec_header[j].data == pgnLoad.games[i]->vec_header[j].data )
				{
					numMatches++;
				}
			}

			if( numMatches == pGame->vec_header.size() )
			{
				bFoundGame = true;
				pFoundGame = pgnLoad.games[i];
				foundGameIndex = i;
				break;
			}
		}
	}

			
	if( bFoundGame == false )
	{
		currentGame = pGame;
		currentGame->Parse();
		currentGame->pCurrMove = currentGame->pRoot;
		pgnLoad.games.push_back(currentGame);
	}
	else
	{
		currentGame = pFoundGame;
		m_dataView->SelectItem(foundGameIndex);
		currentGame->pCurrMove = currentGame->pRoot;


	}

	if( !currentGame->HasGeneratedFens() )
	{
			
		if( ! currentGame->Parse() )
		{
			wxMessageBox("Error Parsing First Game!");
			delete pGame;
			pgnLoad.CloseSQLDatabase();
		}

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

		if( currentGame->pBase->m_fenBlack != "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
		{
			board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
			if( board_canvas->GetBoard()->mov != BLACK )
			{
				my_canvas->white_black_move = BLACK;
			}
			else
			{
				my_canvas->white_black_move = WHITE;
			}
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
			board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->getFenBlack());
			//board_canvas->Refresh(false);
			my_canvas->white_black_move = WHITE;
		}
		else if( currentGame->pCurrMove->info != "1" )
		{
			board_canvas->GetBoard()->SetBoard(currentGame->pBase->getFenBlack());
			//board_canvas->Refresh(false);
			my_canvas->white_black_move = BLACK;
		}
		my_canvas->move_count = 0;


	}

//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	m_dataView->AppendGameList(&pgnLoad);
	my_canvas->Scroll(0,0);
	my_canvas->needsRefresh = true;
	noteFrame->Refresh();
	DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}

void MyFrame::OnLoadDB(wxCommandEvent &evt)
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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
	wxString filter= wxT("PGN files (*.pgn)|*.pgn|Database files(*.db)|*.db|Compressed Files (*.pgc3)|*.pgc3|Data Info Files (*.dat)|*.dat");

    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_OPEN);

	dialog.SetFilterIndex(2);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			//wxString foo( wxT("Hello world") );
			char buf[200];
			strcpy( buf, (const char*)path.mb_str(wxConvUTF8) );
			string str = buf;

			if(str.find(".pgn")!= -1)
			{
				this->OpenPGN(str,true);
				return;
			}
			else if(str.find(".pgc3")!= -1)
			{
				this->OpenPGN(str,true);
				return;
			}
			else
			{
				this->OpenPGN(str,true);
				return;
			}

			return;
		}
	}
}

void MyFrame::OpenPGN(string str,bool refreshList)
{
	if( refreshList )
		pgnLoad.Reset();
			
				


	// Determine File Size.
	// If Size > MAX_FILE_SIZE_IN_RAM
	// Index VM PGN
	// Set m_PGN_MODE to VIRTUAL_MEMORY_MODE
	// Set list view mode to VM PGN MODE
	// 

	wxFileName fn(str);
	if( fn.GetSize().ToDouble() > 100000 )
	{
		pgnLoad.pgn_load_mode = 1;
	}
	else
	{
		pgnLoad.pgn_load_mode = 0;
	}

	if(str.find(".pgnc2")!= -1)
	{
		pgnLoad.pgn_load_mode = 3;
	}

	if( pgnLoad.pgn_load_mode == 2 )
	{
		pgnLoad.OpenSQLDatabase(str);
		return;
	}
	
	if( !refreshList )
	{
		pgnLoad.load_mode_refresh_list = false;
	}
	pgnLoad.LoadPgn(str, this);

	pgnLoad.load_mode_refresh_list = true;
	//pgnLoad.ParseGames();
	//pgnLoad.ParsePgn();
	if( pgnLoad.pgn_load_mode == 0 )
	{
		if( refreshList )
			currentGame = pgnLoad.games[0];
		else 
			currentGame = pgnLoad.games[pgnLoad.games.size()-1];
	}
	else
	{
				

		PGN::Game* pGame = pgnLoad.GetRAMGame(0);

		bool bFoundGame = false;
		PGN::Game* pFoundGame = 0;
		int foundGameIndex = 0;
		for( int i = 0; i < pgnLoad.games.size(); i++ )
		{
			int numMatches = 0;
			if( pGame->vec_header.size() == pgnLoad.games[i]->vec_header.size() )
			{
				for( int j = 0; j < pGame->vec_header.size(); j++ )
				{
					if( pGame->vec_header[j].data == pgnLoad.games[i]->vec_header[j].data )
					{
						numMatches++;
					}
				}

				if( numMatches == pGame->vec_header.size() )
				{
					bFoundGame = true;
					pFoundGame = pgnLoad.games[i];
					foundGameIndex = i;
					break;
				}
			}
		}

		if( bFoundGame == false )
		{
			currentGame = pGame;
			currentGame->Parse();
			currentGame->pCurrMove = currentGame->pRoot;
			pgnLoad.games.push_back(currentGame);
		}
		else
		{
			currentGame = pFoundGame;
			m_dataView->SelectItem(foundGameIndex);
			currentGame->pCurrMove = currentGame->pRoot;
		}
	}
	//currentGame->Parse();
	// NOTE: RECOVER PGN LOAD ERROR
	if( pgnLoad.pgn_load_mode == 0 )
	{
		if( currentGame->Parse() == 0 )
		{
			wxMessageBox(currentGame->GetParseError(), "Error");
			if( refreshList )
			{
				NewGame(true);
			}
			else
			{
				if( pgnLoad.games.size() > 2 )
				{
					SetCurrentGame(pgnLoad.games.size()-2);
					//pgnLoad.EraseGameAtIndex(pgnLoad.games.size()-1);
					pgnLoad.RemoveLastGame();
				}
				else if( pgnLoad.games.size() == 2 )
				{
					//pgnLoad.EraseGameAtIndex(1);
					SetCurrentGame(0);
					pgnLoad.RemoveLastGame();
				}
				else
				{
					NewGame(true);
				}
			}
			return;
		}
	}
	GenerateFensCurrentGame();

//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	if( pgnLoad.pgn_load_mode != 0 )
	{
		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	}
	m_dataView->AppendGameList(&pgnLoad);
	my_canvas->Scroll(0,0);
	my_canvas->needsRefresh = true;
	noteFrame->Refresh();
	DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
        
}


void MyFrame::LoadPGN(bool refreshList)
{
	

	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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
	wxString filter= wxT("PGN files (*.pgn)|*.pgn|Dat files(*.dat)|*.dat|Compressed Files (*.pgcn2)|*.pgnc2");

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

			if( str.find(".dat")!=-1 )
			{
				OpenDB(str);
				//return;
			}
			else
			{
				OpenPGN(str, refreshList);
			}
		}
    }


	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}

void MyFrame::OnAddPGN(wxCommandEvent& WXUNUSED(evt))
{
	LoadPGN(false);
}

void MyFrame::OnLoadPGN(wxCommandEvent& WXUNUSED(event))
{


	LoadPGN(true);
}


void MyFrame::OpenDB_VM(string filename,bool refreshList)
{
	OpenPGN_VM(filename, refreshList);
}

void MyFrame::OpenPGN_VM(string str, bool refreshList)
{

/*   MyWorkerThread *thread = new MyWorkerThread(this);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxMessageBox(wxT("Can't create thread!"));
        return;
    }

    m_dlgProgress = new wxProgressDialog
                        (
                         wxT("Progress dialog"),
                         wxT("Wait until the thread terminates or press [Cancel]"),
                         100,
                         this,
                         wxPD_CAN_ABORT |
                         wxPD_APP_MODAL |
                         wxPD_ELAPSED_TIME |
                         wxPD_ESTIMATED_TIME |
                         wxPD_REMAINING_TIME
                        );

    // thread is not running yet, no need for crit sect
    m_cancelled = false;

    thread->Run();*/

	
	string newInputStr = str;

	if( refreshList )
		pgnLoad.Reset();
			
				


	// Determine File Size.
	// If Size > MAX_FILE_SIZE_IN_RAM
	// Index VM PGN
	// Set m_PGN_MODE to VIRTUAL_MEMORY_MODE
	// Set list view mode to VM PGN MODE
	// 

	wxFileName fn(newInputStr);
	if( fn.GetSize().ToDouble() > 1000 )
	{
		pgnLoad.pgn_load_mode = 1;
	}
	else
	{
		pgnLoad.pgn_load_mode = 0;
	}

	if(str.find(".pgnc2")!= -1)
	{
		pgnLoad.pgn_load_mode = 3;
	}

	if(str.find(".dat")!= -1)
	{
		pgnLoad.pgn_load_mode = 3;
		int newFormatPos = newInputStr.find_last_of("Index.dat");
		newInputStr = newInputStr.substr(0, newFormatPos);
	}



	
	if( !refreshList )
	{
		pgnLoad.load_mode_refresh_list = false;
	}
	pgnLoad.LoadPgnVM(newInputStr, this);

	if( pgnLoad.error_games.size() > 0 )
	{
		for( int i = 0; i < pgnLoad.error_games.size(); i++ )
		{
			wxMessageBox(wxString::Format("Error in game %d", pgnLoad.error_games[i]));
		}
	}

	pgnLoad.load_mode_refresh_list = true;
	//pgnLoad.ParseGames();
	//pgnLoad.ParsePgn();
	if( pgnLoad.pgn_load_mode == 0 )
	{
		if( refreshList )
			currentGame = pgnLoad.games[0];
		else 
			currentGame = pgnLoad.games[pgnLoad.games.size()-1];
	}
	else
	{
				

		PGN::Game* pGame = pgnLoad.GetVMGame(0);

		bool bFoundGame = false;


		if( bFoundGame == false )
		{
			currentGame = pGame;
			currentGame->Parse();
			currentGame->pCurrMove = currentGame->pRoot;
			pgnLoad.games.push_back(currentGame);
		}

	}
	//currentGame->Parse();
	// NOTE: RECOVER PGN LOAD ERROR
	if( pgnLoad.pgn_load_mode == 0 )
	{
		if( currentGame->Parse() == 0 )
		{
			wxMessageBox(currentGame->GetParseError(), "Error");
			if( refreshList )
			{
				NewGame(true);
			}
			else
			{
				if( pgnLoad.games.size() > 2 )
				{
					SetCurrentGame(pgnLoad.games.size()-2);
					//pgnLoad.EraseGameAtIndex(pgnLoad.games.size()-1);
					pgnLoad.RemoveLastGame();
				}
				else if( pgnLoad.games.size() == 2 )
				{
					//pgnLoad.EraseGameAtIndex(1);
					SetCurrentGame(0);
					pgnLoad.RemoveLastGame();
				}
				else
				{
					NewGame(true);
				}
			}
			return;
		}
	}
	GenerateFensCurrentGame();

	//wxCriticalSectionLocker lock(m_csCancelled);

	m_cancelled = true;

//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	if( pgnLoad.pgn_load_mode != 0 )
	{
		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	}
	m_dataView->AppendGameList(&pgnLoad);
	my_canvas->Scroll(0,0);
	my_canvas->needsRefresh = true;
	noteFrame->Refresh();
	DoRefresh();
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
        
}

void MyFrame::OnLoadPGN_VM()
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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
	wxString filter= wxT("PGN files (*.pgn)|*.pgn|Database files(*.db)|*.db|Compressed Files (*.pgcn2)|*.pgnc2");

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

			if( str.find(".pgnc2")!=-1 )
			{
				OpenDB_VM(str, true);
				//return;
			}
			else
			{
				OpenPGN_VM(str,true);
			}
		}
    }
	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}
void MyFrame::OnAddPGN_VM()
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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
	wxString filter= wxT("PGN files (*.pgn)|*.pgn|Compressed Files (*.pgcn2)|*.pgnc2");

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

			if( str.find(".pgnc2")!=-1 )
			{
				OpenDB_VM(str, false);
				//return;
			}
			else
			{
				OpenPGN_VM(str,false);
			}
		}
    }
	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}

void MyFrame::OnLoadDB_VM()
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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
	wxString filter= wxT("PGN files (*.pgn)|*.pgn|Database files(*.db)|*.db|Compressed Files (*.pgcn2)|*.pgnc2");

    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_OPEN);

	dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			//wxString foo( wxT("Hello world") );
			char buf[200];
			strcpy( buf, (const char*)path.mb_str(wxConvUTF8) );
			string str = buf;

			if( str.find(".pgnc2")!=-1 )
			{
				OpenDB_VM(str, true);
				//return;
			}
			else
			{
				OpenPGN_VM(str,true);
			}
		}
    }
	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}

void MyFrame::RecreateGameList()
{
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}

void MyFrame::RecreateList(long flags, bool withText)
{
	m_listPanel->RecreateList(flags, withText);
	m_mgr.Update();
}


void MyFrame::CreateFenForVariationWhite(PGN::Move* pMove, string fen_last)
{


	//long 
		
	
			
	//board_canvas->GetBoard()->parseMove(pMove->white);
		//	time_02 = sw2.Time();
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

			//if( temp->black == "" )
				//break;
			
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
	return (BoardCanvas*)board_canvas;
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

   ctrl->AddPage(CreateHTMLCtrl(ctrl), wxT("Match Results") , false, page_bmp);

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

   

//   ctrl->AddPage(m_scoreGraph,wxT("Game Notation"), true);//#

    //ctrl->AddPage( m_listPanel, wxT("Game List"), false, page_bmp );
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
	wxString table = "<html><body>";
	table += "<h3>Tournament Results</h3>";
	table += "<table>";
	table += "<tr>";
	table += "<td><b>Event Name: </b></td>";
	table += wxString::Format("<td>%s</td>", this->m_engineVsEngine.eventName.c_str() );
	table += "</tr>";
	table += "<tr>";
	table += "<td><b>Time Controls: </b></td>";	
	table += wxString::Format("<td>%d Minutes, %d Seconds, %d Increment (ms)</td>", this->gameClock->SetMinutes, this->gameClock->SetSeconds, this->gameClock->increment );
	table += "</tr>";
	table += "<tr>";
	table += "<td><b>Number Of Games: </b></td>";	
	table += wxString::Format("<td>%d</td>", this->m_engineVsEngine.numGames);
	table += "</tr>";
	table += "<tr>";
	table += "<td><b>Swap Sides Between Games: </b></td>";	
	table += this->m_engineVsEngine.swapSidesBetweenGames ? "<td>true</td>" : "<td>false</td>";
	table += "</tr>";
	table += "<tr><td> </td></tr>";
	table += "<tr></tr>";
	table += "</table>";

	table += "<table border=\"1\" BGCOLOR=\"#000033\">";
	table += "<tr>";
	
	table += "<th bgcolor=\"#FFFF33\"><font COLOR=\"#cccc01\"><b>Name</b></th>";
	for( int i = 0; i < this->m_engineVsEngine.list_engine_games.size(); i++ )
	{
		table += wxString::Format("<th bgcolor=\"#0000FF\"><font COLOR=\"#cccc01\"><b> %d </b></th>", i+1 );
	}
	table += "<th bgcolor=\"#FF0000\">Total</th>";
	table += "</tr>";
	table += "<tr bgcolor=\"#FFFFFF\">";
	table += wxString::Format("<td><b>%s</b></td>", this->m_engineVsEngine.engine01->GetEngineName().c_str().AsChar() );
	int total = 0;
	for( int i = 0; i < this->m_engineVsEngine.list_engine_games.size(); i++ )
	{
		string result = this->m_engineVsEngine.list_engine_games[i]->result;
		if( result == ""  )
		{
			table += "<td>*</td>";//wxString::Format("<th>%s</th>",  );
		}
		else if (result == "1/2-1/2")
		{
			table += "<td>1/2</td>";
		}
		else if (result == "1-0")
		{
			if( m_engineVsEngine.swapSidesBetweenGames == false )
			{
				table += "<td>1</td>";
				total++;
			}
			else
			{
				if( i % 2 == 0 )
				{
					table += "<td>0</td>";
				}
				else
				{
					table += "<td>1</td>";
					total++;
				}
			}
		}
		else if (result == "0-1")
		{
			if( m_engineVsEngine.swapSidesBetweenGames == false )
			{
				table += "<td>0</td>";
			}
			else
			{
				if( i % 2 == 0 )
				{
					table += "<td>1</td>";
					total++;
				}
				else
				{
					table += "<td>0</td>";
				}
			}
		}
	}
	table += wxString::Format("<th>%d</th>", total );
	table += "</tr>";
	total = 0;

	table += "<tr bgcolor=\"#FFFFFF\">";
	table += wxString::Format("<td><b>%s</b></td>", this->m_engineVsEngine.engine02->GetEngineName().c_str().AsChar() );
	
	for( int i = 0; i < this->m_engineVsEngine.list_engine_games.size(); i++ )
	{
		string result = this->m_engineVsEngine.list_engine_games[i]->result;
		if( result == ""  )
		{
			table += "<td>*</td>";//wxString::Format("<th>%s</th>",  );
		}
		else if (result == "1/2-1/2")
		{
			table += "<td>1/2</td>";
		}
		else if (result == "1-0")
		{
			if( m_engineVsEngine.swapSidesBetweenGames == false )
			{
				table += "<td>0</td>";
			}
			else
			{
				if( i % 2 == 0 )
				{
					table += "<td>1</td>";
					total++;
				}
				else
				{
					table += "<td>0</td>";
				}
			}
		}
		else if (result == "0-1")
		{
			if( m_engineVsEngine.swapSidesBetweenGames == false )
			{
				table += "<td>1</td>";
				total++;
			}
			else
			{
				if( i % 2 == 0 )
				{
					table += "<td>0</td>";
				}
				else
				{
					table += "<td>1</td>";
					total++;
				}
			}
		}
	}
	table += wxString::Format("<th>%d</th>", total );
	table += "</tr>";
	table += "</table>";
	table += "</body></html>";


	/*const char* text = 
		"<html><body>"
		"<h3>Tournament Results</h3>"
		"<table border=\"1\">"
		"<tr>"
		"<th>Header 1</th>"
		"<th>Header 2</th>"
		"</tr>"
		"<tr>"
		"<td>row 1, cell 1</td>"
		"<td>row 1, cell 2</td>"
		"</tr>"
		"<tr>"
		"<td>row 2, cell 1</td>"
		"<td>row 2, cell 2</td>"
		"</tr>"
		"</table>"
		"</body></html>";*/
    /*const char* text =
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
        "</body></html>";*/

    return table;//wxString::FromAscii(text);
}

#ifdef __WXOSX__

void MyFrame::OnToggleMacUseGeneric(wxCommandEvent& event)
{
    wxSystemOptions::SetOption(wxT("mac.listctrl.always_use_generic"), event.IsChecked());
}

#endif // __WXOSX__


void MyFrame::OnToggleD3DMode()
{
	BaseGraphicsBoard *pNewBoard;

	if( m_graphicsModeSetting == GRAPHICS_MODE::D3D_GRAPHICS )
	{
		m_graphicsModeSetting = GRAPHICS_MODE::GC_GRAPHICS;
		((D3D9Canvas*)board_canvas)->StopTimer();

		pNewBoard = new BoardCanvas(this, board_canvas);
		//pNewBoard->PerformCopy(this->board_canvas);
		
		if( !board_canvas->IsBoardFlipped() )
			pNewBoard->Flip();

		
		m_mgr.DetachPane((D3D9Canvas*)board_canvas);
		
		delete (D3D9Canvas*)board_canvas;
		board_canvas = pNewBoard;
		board_canvas->ChangingGraphicsMode = false;

		m_mgr.AddPane((BoardCanvas*)board_canvas, wxAuiPaneInfo().
		Name(wxT("test12")).Caption(wxT("Graphical Pane")).
                  Left().Layer(1).Position(1).DestroyOnClose(false).PinButton(true).
                  CloseButton(true).MaximizeButton(true));

		m_mgr.Update();
	}
	else
	{
		m_graphicsModeSetting = GRAPHICS_MODE::D3D_GRAPHICS;
		((BoardCanvas*)board_canvas)->StopTimer();

		pNewBoard = new D3D9Canvas(this,board_canvas);
		//pNewBoard->PerformCopy(this->board_canvas);
		
		if( !board_canvas->IsBoardFlipped() )
			pNewBoard->Flip();

		
		m_mgr.DetachPane((BoardCanvas*)board_canvas);
		
		delete (BoardCanvas*)board_canvas;
		board_canvas = pNewBoard;
		board_canvas->ChangingGraphicsMode = false;

		m_mgr.AddPane((D3D9Canvas*)board_canvas, wxAuiPaneInfo().
		Name(wxT("test12")).Caption(wxT("Graphical Pane")).
                  Left().Layer(1).Position(1).DestroyOnClose(false).PinButton(true).
                  CloseButton(true).MaximizeButton(true));

		m_mgr.Update();
	}
}


void MyFrame::StartEnginesAfterMoveChange()
{
	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() &&
			m_pipePanel[i]->IsAnalysing()&&
			!m_pipePanel[i]->AnalyzingHasStarted())
		{
			m_pipePanel[i]->KibitzUserEndsMove();
			
		}
	}
}

void MyFrame::StopEnginesBeforeMoveChange()
{
	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() &&
			m_pipePanel[i]->IsAnalysing() &&
			m_pipePanel[i]->AnalyzingHasStarted())
		{
			m_pipePanel[i]->KibitzStartsMove();
		}
	}
}

void MyFrame::KickStartEnginesAfterMoveChange()
{
	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() &&
			m_pipePanel[i]->IsAnalysing()&&
			!m_pipePanel[i]->AnalyzingHasStarted())
		{
			m_pipePanel[i]->KibitzUserEndsMove();
			
		}
		else if(m_pipePanel[i]->IsActive() &&
			m_pipePanel[i]->IsAnalysing()&&
			m_pipePanel[i]->AnalyzingHasStarted())
		{
			m_pipePanel[i]->KibitzStartsMove();
			m_pipePanel[i]->KibitzUserEndsMove();
		}
	}
}

void MyFrame::DeleteAllSelectedGames()
{
	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() && m_pipePanel[i]->IsAnalysing())
		{
			m_pipePanel[i]->KibitzStartsMove();
		}
	}
	StopAllAnimation();

	wxVariant boolVar;
	int dataSize = pgnLoad.games.size();
	for( int i = dataSize-1; i > -1; i-- )
	{
		// the bool is in column zero
		m_dataView->lc->GetStore()->GetValueByRow(boolVar, i, 0);
		if( boolVar )
		{
			pgnLoad.EraseGameAtIndex(i);

			//m_dataView->AppendGameList(pgnLoad);
		}
	}

	if( pgnLoad.games.size() == 0 )
	{
		pgnLoad.NewGame();
	}

	currentGame = pgnLoad.games[0];

	currentGame->pCurrMove = currentGame->pRoot;
	this->my_canvas->white_black_move = 0;
	this->my_canvas->move_count = 0;
	this->my_canvas->needsRefresh = true;
	board_canvas->MyRefresh(true);
	this->noteFrame->Refresh();
	this->treeCanvas->MyRefresh(true);
	this->moveDataCanvas->Refresh();

	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() )
		{
		m_pipePanel[i]->KibitzUserNewGame();
		}
	}
	this->m_dataView->AppendGameList(&pgnLoad);
}

void MyFrame::DeleteGameAtIndex(int i)
{

	// if data view is active

	//if( this->
	//if( listHasBeenSorted == false )
	{
		if( i < pgnLoad.games.size() )
		{
			pgnLoad.EraseGameAtIndex(i);
		}
	}
	
	if( pgnLoad.games.size() > 0 )
	{
		currentGame = pgnLoad.games[0];
	}
	else
	{
		NewGame(false);
	}



	//this->m_dataView->AppendGameList(&pgnLoad);
}

void MyFrame::OnEraseElementFromDB()
{
	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() && m_pipePanel[i]->IsAnalysing())
		{
			m_pipePanel[i]->KibitzStartsMove();
		}
	}
	StopAllAnimation();
	// if data view is active

	//if( this->
	//if( listHasBeenSorted == false )
	{
		if( this->m_dataView->selectedRow < pgnLoad.games.size() )
		{
			pgnLoad.EraseGameAtIndex(this->m_dataView->selectedRow);
		}
	}
	
	if( pgnLoad.games.size() > 0 )
	{
		currentGame = pgnLoad.games[0];
	}
	else
	{
		NewGame(false);
	}

	currentGame->pCurrMove = currentGame->pRoot;
	this->my_canvas->white_black_move = 0;
	this->my_canvas->move_count = 0;
	this->my_canvas->needsRefresh = true;
	board_canvas->MyRefresh(true);
	this->noteFrame->Refresh();
	this->treeCanvas->MyRefresh(true);
	this->moveDataCanvas->Refresh();

	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() )
		{
		m_pipePanel[i]->KibitzUserNewGame();
		}
	}

	this->m_dataView->AppendGameList(&pgnLoad);
	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	//RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}

void MyFrame::OnRemoveGameFromDB()
{
	OnEraseElementFromDB();
}

void MyFrame::OnAddGameToDB()
{
}

void MyFrame::OnOpenDB_New()
{
	StopAllAnimation();

	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
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


	wxString filter= wxT("DAT files (*.dat)|*.dat");

    wxFileDialog dialog(this,
        _("Choose a filename"),
        path,
        filename,
        filter,
        wxFD_OPEN);

	dialog.SetFilterIndex(1);

    if (dialog.ShowModal() == wxID_OK)
    {
        wxString path = dialog.GetPath();

        if (!path.empty())
        {
			//wxString foo( wxT("Hello world") );
			char buf[200];
			strcpy( buf, (const char*)path.mb_str(wxConvUTF8) );
			string str = buf;

			string newInputStr = str;

			//if( refreshList )
			//	pgnLoad.Reset();
			
				


			// Determine File Size.
			// If Size > MAX_FILE_SIZE_IN_RAM
			// Index VM PGN
			// Set m_PGN_MODE to VIRTUAL_MEMORY_MODE
			// Set list view mode to VM PGN MODE
			// 



			if(str.find(".dat")!= -1)
			{
				pgnLoad.pgn_load_mode = 3;
				int newFormatPos = newInputStr.find_last_of("_");
				newInputStr = newInputStr.substr(0, newFormatPos);
				newInputStr += ".dat";
			}



	

			pgnLoad.load_mode_refresh_list = true;
			
			pgnLoad.LoadPgnVM(newInputStr, this);

			pgnLoad.load_mode_refresh_list = true;
			//pgnLoad.ParseGames();
			//pgnLoad.ParsePgn();

			{
				

				PGN::Game* pGame = pgnLoad.GetVMGame(0);

				bool bFoundGame = false;
				PGN::Game* pFoundGame = 0;
				int foundGameIndex = 0;
				for( int i = 0; i < pgnLoad.games.size(); i++ )
				{
					int numMatches = 0;
					if( pGame->vec_header.size() == pgnLoad.games[i]->vec_header.size() )
					{
						for( int j = 0; j < pGame->vec_header.size(); j++ )
						{
							if( pGame->vec_header[j].data == pgnLoad.games[i]->vec_header[j].data )
							{
								numMatches++;
							}
						}

						if( numMatches == pGame->vec_header.size() )
						{
							bFoundGame = true;
							pFoundGame = pgnLoad.games[i];
							foundGameIndex = i;
							break;
						}
					}
				}

				if( bFoundGame == false )
				{
					currentGame = pGame;
					currentGame->Parse();
					currentGame->pCurrMove = currentGame->pRoot;
					pgnLoad.games.push_back(currentGame);
				}
				else
				{
					currentGame = pFoundGame;
					m_dataView->SelectItem(foundGameIndex);
					currentGame->pCurrMove = currentGame->pRoot;
				}
			}
			//currentGame->Parse();
			// NOTE: RECOVER PGN LOAD ERROR

			GenerateFensCurrentGame();


			RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
			
			m_dataView->AppendGameList(&pgnLoad);
			my_canvas->Scroll(0,0);
			my_canvas->needsRefresh = true;
			noteFrame->Refresh();
			DoRefresh();
		#ifdef USE_CANVAS_SET_FOCUS
			my_canvas->SetFocus();
		#endif
			//ctrl->chess_board.mov = 0;
			//renderTimer->Start();
		}
    }
	for( unsigned int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive())// &&
			//board_canvas->activeEngines[j]->IsAnalysing())
		{
	//		activeEngines[j]->KibitzStartsMove();
			board_canvas->activeEngines[j]->KibitzUserNewGame();
			
		}
	}
}

void MyFrame::OnSaveDB()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}
	wxString defaultFile = "filename";
	if( currentGame->whiteName != "" && currentGame->blackName != "" )
	{
		defaultFile = currentGame->whiteName;
		defaultFile+= "_vs_";
		defaultFile+= currentGame->blackName;
		//defaultFile+= ".pgn";
	}
    wxFileDialog dialog(this,
                        wxT("Save file dialog"),
                        wxEmptyString,
                        defaultFile,
                        wxT("PGN files (*.pgn)|*.pgn"),
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(0);

    if (dialog.ShowModal() == wxID_OK)
    {
      //  wxLogMessage(wxT("%s, filter %d"),
      //               dialog.GetPath().c_str(), dialog.GetFilterIndex());
		wxString strOutput = dialog.GetPath();// + dialog.GetFilename();
		string nameToDB = string(((char*)strOutput.c_str().AsChar()));
		if( !pgnLoad.SaveDB(nameToDB ) )
		{
			wxMessageBox("Can't save DB, Error opening file");
		}
    }
#ifdef USE_CANVAS_SET_FOCUS
	my_canvas->SetFocus();
#endif
}

void MyFrame::OnExportDB()
{
}
void MyFrame::OnCompressDB()
{
}
void MyFrame::OnEditMode()
{
}

void MyFrame::FindName()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

    wxTextEntryDialog dialog(this,
                             wxT("Find Name\n")
                             wxT("Enter the name that you would like to search for"),
                             wxT("Please enter a string"),
                             wxT("Default value"),
                             wxOK | wxCANCEL);
	
    if (dialog.ShowModal() == wxID_OK)
    {
        //wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
		string nameRef = dialog.GetValue();

		this->find_possibilities.clear();
		pgnLoad.headersListData.playerNames.FindSimilarStrings(this->find_possibilities,nameRef );

		int sel=0;
		FindDialog myFindDialog(this, this, wxID_ANY, "Find Options", &sel);

		

		myFindDialog.ShowModal();

		if( sel > this->find_possibilities.size() )
		{
			return;
		}

		int id_to_search_for = this->find_possibilities[ sel ].id;

		/*for( int vv = 0; vv < this->find_possibilities.size(); vv++ )
		{
			if( this->find_possibilities[vv].str == myFindDialog.stringSelection )
			{
				id_to_search_for = this->find_possibilities[vv].id;
				break;
			}
		}*/

		pgnLoad.FindName(this->find_possibilities[ sel ].str, id_to_search_for);

		

		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

    }
}

void MyFrame::OnDatabaseFilterDate()
{

}
void MyFrame::OnDatabaseFilterEvent()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

    wxTextEntryDialog dialog(this,
                             wxT("Find Event\n")
                             wxT("Enter the name of the Event that you would like to search for"),
                             wxT("Please enter a string"),
                             wxT("Default value"),
                             wxOK | wxCANCEL);
	
    if (dialog.ShowModal() == wxID_OK)
    {
        //wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
		string nameRef = dialog.GetValue();

		this->find_possibilities.clear();
		pgnLoad.headersListData.eventNames.FindSimilarStrings(this->find_possibilities,nameRef );

		int sel=0;
		FindDialog myFindDialog(this, this, wxID_ANY, "Find Options", &sel);

		

		myFindDialog.ShowModal();

		if( sel > this->find_possibilities.size() )
		{
			return;
		}

		int id_to_search_for = this->find_possibilities[ sel ].id;

		/*for( int vv = 0; vv < this->find_possibilities.size(); vv++ )
		{
			if( this->find_possibilities[vv].str == myFindDialog.stringSelection )
			{
				id_to_search_for = this->find_possibilities[vv].id;
				break;
			}
		}*/

		pgnLoad.FilterEvent(this->find_possibilities[ sel ].str, id_to_search_for);

		

		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

    }
}
void MyFrame::OnDatabaseFilterSite()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

    wxTextEntryDialog dialog(this,
                             wxT("Find Site\n")
                             wxT("Enter the name of the Site that you would like to search for"),
                             wxT("Please enter a string"),
                             wxT("Default value"),
                             wxOK | wxCANCEL);
	
    if (dialog.ShowModal() == wxID_OK)
    {
        //wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
		string nameRef = dialog.GetValue();

		this->find_possibilities.clear();
		pgnLoad.headersListData.eventNames.FindSimilarStrings(this->find_possibilities,nameRef );

		int sel=0;
		FindDialog myFindDialog(this, this, wxID_ANY, "Find Options", &sel);

		

		myFindDialog.ShowModal();

		if( sel > this->find_possibilities.size() )
		{
			return;
		}

		int id_to_search_for = this->find_possibilities[ sel ].id;

		/*for( int vv = 0; vv < this->find_possibilities.size(); vv++ )
		{
			if( this->find_possibilities[vv].str == myFindDialog.stringSelection )
			{
				id_to_search_for = this->find_possibilities[vv].id;
				break;
			}
		}*/

		pgnLoad.FilterSite(this->find_possibilities[ sel ].str, id_to_search_for);

		

		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

    }
}
void MyFrame::OnDatabaseFilterElo()
{
}
void MyFrame::OnDatabaseFilterEloWhite()
{
}
void MyFrame::OnDatabaseFilterEloBlack()
{
}
void MyFrame::OnDatabaseFilterEco()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

    ECO_FilterDialog dialog(this, wxID_ANY,
                             wxT("ECO Filter"));
	
    if (dialog.ShowModal() == wxID_OK)
    {
        //wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
		string name1 = dialog.eco1;
		string name2 = dialog.eco2;

		int id_to_search_for = 0;

		pgnLoad.headersListData.ecoNames.FindSubstring(name1,id_to_search_for);




		/*for( int vv = 0; vv < this->find_possibilities.size(); vv++ )
		{
			if( this->find_possibilities[vv].str == myFindDialog.stringSelection )
			{
				id_to_search_for = this->find_possibilities[vv].id;
				break;
			}
		}*/

		pgnLoad.FilterEco(name1, id_to_search_for);

		

		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

    }
}
void MyFrame::OnDatabaseFilterNameWhite()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

    wxTextEntryDialog dialog(this,
                             wxT("Find Name\n")
                             wxT("Enter the name that you would like to search for"),
                             wxT("Please enter a string"),
                             wxT("Default value"),
                             wxOK | wxCANCEL);
	
    if (dialog.ShowModal() == wxID_OK)
    {
        //wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
		string nameRef = dialog.GetValue();

		this->find_possibilities.clear();
		pgnLoad.headersListData.playerNames.FindSimilarStrings(this->find_possibilities,nameRef );

		int sel=0;
		FindDialog myFindDialog(this, this, wxID_ANY, "Find Options", &sel);

		

		myFindDialog.ShowModal();

		if( sel > this->find_possibilities.size() )
		{
			return;
		}

		int id_to_search_for = this->find_possibilities[ sel ].id;

		/*for( int vv = 0; vv < this->find_possibilities.size(); vv++ )
		{
			if( this->find_possibilities[vv].str == myFindDialog.stringSelection )
			{
				id_to_search_for = this->find_possibilities[vv].id;
				break;
			}
		}*/

		pgnLoad.FindNameWhite(this->find_possibilities[ sel ].str, id_to_search_for);

		

		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

    }
}
void MyFrame::OnDatabaseFilterNameBlack()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

    wxTextEntryDialog dialog(this,
                             wxT("Find Name\n")
                             wxT("Enter the name that you would like to search for"),
                             wxT("Please enter a string"),
                             wxT("Default value"),
                             wxOK | wxCANCEL);
	
    if (dialog.ShowModal() == wxID_OK)
    {
        //wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
		string nameRef = dialog.GetValue();

		this->find_possibilities.clear();
		pgnLoad.headersListData.playerNames.FindSimilarStrings(this->find_possibilities,nameRef );

		int sel=0;
		FindDialog myFindDialog(this, this, wxID_ANY, "Find Options", &sel);

		

		myFindDialog.ShowModal();

		if( sel > this->find_possibilities.size() )
		{
			return;
		}

		int id_to_search_for = this->find_possibilities[ sel ].id;

		/*for( int vv = 0; vv < this->find_possibilities.size(); vv++ )
		{
			if( this->find_possibilities[vv].str == myFindDialog.stringSelection )
			{
				id_to_search_for = this->find_possibilities[vv].id;
				break;
			}
		}*/

		pgnLoad.FindNameBlack(this->find_possibilities[ sel ].str, id_to_search_for);

		

		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

    }
}

void MyFrame::OnDatabaseShowFilters()
{
	const wxString str = wxT("Filters");
	// TEMP
	m_mgr.AddPane(new GridFrame(this), wxAuiPaneInfo().
                //  Caption(str).
                  Float().FloatingPosition(GetStartPosition()).
                  //FloatingSize(300,200).
				  CloseButton(true).MaximizeButton(true).DestroyOnClose(true));

	m_mgr.Update();
}

void MyFrame::ClearSearchFilter()
{
	pgnLoad.ClearFilters();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);

}

void MyFrame::ApplyFilterOnSubset()
{

/*MyWorkerThread *thread = new MyWorkerThread(this);

    if ( thread->Create() != wxTHREAD_NO_ERROR )
    {
        wxMessageBox(wxT("Can't create thread!"));
        return;
    }

    m_dlgProgress = new wxProgressDialog
                        (
                         wxT("Progress dialog"),
                         wxT("Wait until the thread terminates or press [Cancel]"),
                         100,
                         this,
                         wxPD_CAN_ABORT |
                         wxPD_APP_MODAL |
                         wxPD_ELAPSED_TIME |
                         wxPD_ESTIMATED_TIME |
                         wxPD_REMAINING_TIME
                        );

    // thread is not running yet, no need for crit sect
    m_cancelled = false;

    thread->Run();

	return;*/
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}
	
	for( int i = 0; i < tsc->vecResults.size(); i++ )
	{
		delete tsc->vecResults[i];
		tsc->vecResults[i] = NULL;
	}
	if( tsc->vecResults.size() != 0 )
		tsc->vecResults.clear();
	
	
	ChessBoard brd;

	if( currentGame->pCurrMove != currentGame->pBase )
	{
		vector< unsigned short int > moves_vec;
		currentGame->pCurrMove->GetCompressedMoveIntegersFromStart(this->board_canvas->GetBoard()->mov,&moves_vec); 
	//string str = currentGame->pCurrMove->GetSanStringsFromStartWithSpace(this->board_canvas->GetBoard()->mov );
	

		int black_white = WHITE;
		if( board_canvas->GetBoard()->mov == BLACK )
		{
			black_white = BLACK;
		}


		
		pgnLoad.IntFilter_FilteredRAMGamesIntoResultCache(&moves_vec,tsc->vecResults, black_white, brd); 
		
	//pgnLoad.StringFilterRAMGamesIntoResultCache(str, tsc->vecResults, black_white);
	}
	else
	{
		//if( pgnLoad.AreFiltersClear() )
		
			pgnLoad.IntFilterRAMGamesIntoResultCache_StartPos( tsc->vecResults, brd);
		

	}

	tsc->SortResultCaches();
	tsc->Refresh();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}

void MyFrame::ApplySearchFilter()
{
/*	ECO_FilterDialog dlg(this);
	
	dlg.ShowModal();
	if( dlg.Ok == true )
	{
		wxMessageBox(dlg.eco1);
		wxMessageBox(dlg.eco2);

		pgnLoad.FilterRAMGamesByECO( dlg.eco1 );
		RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	}
*/

	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}
	
	for( int i = 0; i < tsc->vecResults.size(); i++ )
	{
		delete tsc->vecResults[i];
		tsc->vecResults[i] = NULL;
	}
	if( tsc->vecResults.size() != 0 )
		tsc->vecResults.clear();
	
	
	ChessBoard brd;

	if( currentGame->pCurrMove != currentGame->pBase )
	{
		vector< unsigned short int > moves_vec;
		currentGame->pCurrMove->GetCompressedMoveIntegersFromStart(this->board_canvas->GetBoard()->mov,&moves_vec); 
	//string str = currentGame->pCurrMove->GetSanStringsFromStartWithSpace(this->board_canvas->GetBoard()->mov );
	

		int black_white = WHITE;
		if( board_canvas->GetBoard()->mov == BLACK )
		{
			black_white = BLACK;
		}

		// for apply filter on subset ... maybe add a filter type and perform this function
		if( pgnLoad.AreFiltersClear() )
		{
			pgnLoad.IntFilterRAMGamesIntoResultCache(&moves_vec,tsc->vecResults, black_white, brd); 
		}
		else
		{
			pgnLoad.IntFilter_FilteredRAMGamesIntoResultCache(&moves_vec,tsc->vecResults, black_white, brd); 
		}
	//pgnLoad.StringFilterRAMGamesIntoResultCache(str, tsc->vecResults, black_white);
	}
	else
	{
		//if( pgnLoad.AreFiltersClear() )
		{
			pgnLoad.IntFilterRAMGamesIntoResultCache_StartPos( tsc->vecResults, brd);
		}

	}

	tsc->SortResultCaches();
	tsc->Refresh();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	
	//m_mgr.Update();


	//wxMessageBox( wxString::Format( "%f white wins, %f black wins, %f draws, %d Games number", 
	//	pgnLoad.temporyResCache.GetWhiteWinsPercentage(), pgnLoad.temporyResCache.GetBlackWinsPercentage(), 
	//	pgnLoad.temporyResCache.GetDrawPercentage(), pgnLoad.temporyResCache.GetTotal()));
//	wxMessageBox(str);
}

void MyFrame::PositionSearch()
{
	StopAllAnimation();
	for( int i = 0; i < numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() && 
			m_pipePanel[i]->IsAnalysing() )
		{
			m_pipePanel[i]->KibitzStops();
		}
	}
	PositionSetupFrame *positionSetupFrame = new PositionSetupFrame(this, true);

	positionSetupFrame->Show(true);

	
}

void MyFrame::SetPositionSearchData(ChessBoard* brd, bool pawns_only)
{
	for( int i = 0; i < 8; i++ )
	{
		for( int j =0; j < 8; j++ )
		{
			pgnLoad.m_positionSearch.position[i][j] = 0;
			if( pawns_only )
			{
				if( (brd->board[i][j] == ChessBoard::wPawn) ||
					(brd->board[i][j] == ChessBoard::bPawn))
				{
					pgnLoad.m_positionSearch.position[i][j] = brd->board[i][j];
				}
			}
			else
			{
				pgnLoad.m_positionSearch.position[i][j] = brd->board[i][j];
			}
		}
	}
}

void MyFrame::DoPositionSearch(   bool pawns_only, bool find_similar)
{
	ChessBoard temp;
	if( pawns_only )
	{
		if( find_similar )
		{
			this->pgnLoad.PositionSearchPawnsSimilar(temp, -1 );
		}
		else
		{
			this->pgnLoad.PositionSearchPawns(temp, -1 );
		}
	}
	else
	{
		if( find_similar )
		{
			this->pgnLoad.PositionSearchSimilar(temp, -1 );
		}
		else
		{
			this->pgnLoad.PositionSearch(temp, -1 );
		}
	}

	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}


void MyFrame::RemoveEmptyGamesFromDB()
{
}

void MyFrame::SortByMoves()
{
	pgnLoad.SortByMoves();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByNumber()
{
	pgnLoad.SortByNumber();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByWhiteName()
{
	pgnLoad.SortByWhiteName();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByBlackName()
{
	pgnLoad.SortByBlackName();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByWhiteElo()
{
	pgnLoad.SortByWhiteElo();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByBlackElo()
{
	pgnLoad.SortByBlackElo();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByECO()
{
	pgnLoad.SortByECO();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByEvent()
{
	pgnLoad.SortByEvent();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortBySite()
{
	pgnLoad.SortBySite();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}
void MyFrame::SortByDate()
{
	pgnLoad.SortByDate();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}

void MyFrame::SortByResult()
{
	pgnLoad.SortByResult();
	RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}


void MyFrame::OnPlayThroughGameList(unsigned int num)
{
	if( num >= pgnLoad.games.size() )
	{
		SetCurrentGame(0);
		m_dataView->SelectItem(0);
		this->ResetPlayThroughData();
		this->playThroughGameList = false;
		if( this->m_gameTraverseTimer )
			this->m_gameTraverseTimer->Stop();
		board_canvas->bAnimating = false;

		return;
	}
	SetCurrentGame(num);

	m_dataView->SelectItem(num);
	
	
	if( !currentGame->HasGeneratedFens() )
	{
		wxMessageBox("Error parsing Game in module OnPlayList()","Internal Error");
		if( currentGame != pgnLoad.games[0] )
		{
			SetCurrentGame(0);
			m_dataView->SelectItem(0);
		}


		m_dataView->SelectItem(0);
		this->ResetPlayThroughData();
		this->playThroughGameList = false;
		if( this->m_gameTraverseTimer )
			this->m_gameTraverseTimer->Stop();
		board_canvas->bAnimating = false;
		return;
	}

	currentPlayGamePly = currentGame->CountMoves();

	my_canvas->move_count = 0;
	currentGame->ResetMoveList();
	//currentGame->EnumerateVariations();
	board_canvas->ResetBoard();

	my_canvas->Refresh(true);
	//board_canvas->Refresh(false);
	
	PlayThroughGame();
}
	

void MyFrame::PlayThroughGame()
{
	if( m_gameTraverseTimer == NULL )
	{
		m_gameTraverseTimer = new TraverseGameTimer(this);
		
	}
	m_gameTraverseTimer->SetTimeToPause(500);
	currentGame->PrepareForVisitor();
	m_gameTraverseTimer->start(); 
}

void MyFrame::SearchByName()
{
	// show message box (text entry)
	// enter the name you would like to search for ...
	// access list of names in pgnLoad
	// take all names with sharing the first 2 letters
	// ask the user to select 1 (or more) of the list elements
	// have other data, white or black
	
}

void MyFrame::OnCheckAll()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}
	this->m_dataView->CheckAllCheckBoxes();
}

void MyFrame::OnUnCheckAll()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}
	this->m_dataView->UnCheckAllCheckBoxes();
}

void MyFrame::CopyGame()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}
	PGN::Game *pGame = new PGN::Game();
	pGame->CopyGame(this->currentGame);//->CopyGame();
	pgnLoad.games.push_back(pGame);
	this->m_dataView->AppendGameList(&pgnLoad);
}

void MyFrame::OnMoveUp()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

	int selID = m_dataView->selectedRow;
	pgnLoad.MoveGameUpInList(selID);
	m_dataView->AppendGameList(&pgnLoad);
	if( selID > 0 )
	{
		m_dataView->SelectItem(selID-1);
	}
}
void MyFrame::OnMoveDown()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

	int selID = m_dataView->selectedRow;
	pgnLoad.MoveGameDownInList(selID);
	m_dataView->AppendGameList(&pgnLoad);
	if( selID < pgnLoad.games.size()-1 )
	{
		m_dataView->SelectItem(selID+1);
	}
}

void MyFrame::OnMergeSelected()
{
	StopAllAnimation();
	for( int j = 0; j < board_canvas->activeEngines.size(); j++ )
	{
		if( board_canvas->activeEngines[j]->IsActive() &&
			board_canvas->activeEngines[j]->IsAnalysing())
		{
			board_canvas->activeEngines[j]->KibitzStartsMove();
	//		activeEngines[j]->KibitzUserNewGame();
		}
	}

	PGN::Game* mergeTarget = 0;
	int mergeStart = 0;
	for( unsigned int i = 0; i < pgnLoad.games.size(); i++ )
	{
		if( this->m_dataView->IsSelected(i) )
		{
			mergeTarget = pgnLoad.games[i];
			mergeStart = i;
			break;
		}
	}

	if( mergeTarget == 0 )
		return;

	for( unsigned int i = mergeStart+1; i <pgnLoad.games.size() ; i++ )
	{
		if( !this->m_dataView->IsSelected(i) )
		{
			continue;
		}
		bool needsFens = false;
		wxStopWatch sw;
		long time_inc_01 = 0;
		if( !pgnLoad.games[i]->HasGeneratedFens() )
		{

			int ret = pgnLoad.games[i]->Parse();

			if( ret == 0 )
			{
				wxMessageBox(pgnLoad.games[i]->GetParseError(),"Error");
				return;
			}
			pgnLoad.games[i]->ParseHeader();

			needsFens = true;
		}

		if( needsFens )
		{
			PGN::Move * pMove = pgnLoad.games[i]->pRoot;
			while(pMove)
			{
				if( pMove->white != "" )
				{
					//time_01 = sw2.Time();
					board_canvas->GetBoard()->parseMove(pMove->white);
					//time_02 = sw2.Time();
					//total_time_in_variation += time_02 - time_01;
					
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
		}
		//time_inc_01 = sw.Time();
		//fileLoadLogger << "Timer To Parse File: " + time_inc_01;
		PGN::Move* move = pgnLoad.games[i]->pRoot->GetLastMove();
		if( move->black == "")
		{
			string inpt = "{ ";
			for( unsigned int j = 0; j < pgnLoad.games[i]->vec_header.size(); j++ )
			{
				if( pgnLoad.games[i]->vec_header[j].data.find("White ") != -1 )
				{
					inpt += GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Black ") != -1 )
				{
					inpt += " " + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("ECO ") != -1 )
				{
					inpt += " ECO:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Opening ") != -1 )
				{
					inpt += " Opening:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Variation ") != -1 )
				{
					inpt += " Variation:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
			}

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(move->annotation_white.text));
			move->annotation_white.text.push_back("}");
			vector<string>::iterator strit = move->annotation_white.text.begin();
			
		/*	while( strit < move->annotation_white.text.end())
			{
				move->annotation_white.text.insert(strit, " ");
				if( strit + 1 !=  move->annotation_white.text.end() )
					strit++;
				else
					break;
				if( strit + 1 !=  move->annotation_white.text.end() )
					strit++;
				else 
					break;
			}*/
		}
		else
		{
			string inpt = "{ ";
			for( unsigned int j = 0; j < pgnLoad.games[i]->vec_header.size(); j++ )
			{
				if( pgnLoad.games[i]->vec_header[j].data.find("White ") != -1 )
				{
					inpt += GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Black ") != -1 )
				{
					inpt += " " + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("ECO ") != -1 )
				{
					inpt += " ECO:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Opening ") != -1 )
				{
					inpt += " Opening:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Variation ") != -1 )
				{
					inpt += " Variation:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
			}

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(move->annotation_black.text));
			move->annotation_black.text.push_back("}");
			vector<string>::iterator strit = move->annotation_black.text.begin();
			


			/*while( strit < move->annotation_black.text.end())
			{
				move->annotation_black.text.insert(strit, " ");
				if( strit < move->annotation_black.text.end() )
					strit++;
				else
					break;
				if( strit+1 != move->annotation_black.text.end() )
					strit++;
				else
					break;
			}*/
		}


		//pgnLoad.games[i]->Parse();
		mergeTarget->MergeGame(pgnLoad.games[i]);


		//pgnLoad.RemoveLastGame();
	}

	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	//RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	//board_canvas->Refresh(true);
	my_canvas->move_count = 0;
	//this->my_canvas->Refresh(true);
	
	PGN::Move* pMove = mergeTarget->pRoot;





	currentGame = mergeTarget;
	currentGame->pCurrMove = mergeTarget->pRoot;
	currentGame->SetHasGeneratedFens();
	
	ostringstream of;
	mergeTarget->GameToBuffer(of);
	//this->m_dataView->Unselect(mergeStart);



	//delete mergeTarget;

	// MERGE_FIX
	//PGN::Game *newMergeTarget = new PGN::Game();

	//newMergeTarget->

	//pgnLoad.ClearAllAfterFirstGame();
	this->DeleteAllSelectedGames();

	currentGame = pgnLoad.LoadPgnFromString(of.str().c_str());

	

	if( !currentGame->Parse() )
	{
		wxMessageBox("Error Parsing Merged Game", "Merge Error");
	}
	currentGame->pCurrMove = currentGame->pRoot;

	if( !this->GenerateFensCurrentGame() )
	{
		wxMessageBox("Error Generating Current Game", "Game Generation Error");
	}

	m_dataView->AppendGameList(&pgnLoad);

	currentGame->ResetMoveList();
	//currentGame->EnumerateVariations();
	board_canvas->ResetBoard();
}

void MyFrame::OnMergeAll()
{
	string path = wxStandardPaths::Get().GetResourcesDir().c_str() + "/Session Data/FileLoadLog.log";
	ofstream fileLoadLogger;
	fileLoadLogger.open(path);
	//int totalParseTime = 0;
	
	for( unsigned int i = 1; i <pgnLoad.games.size() ; i++ )
	{
		bool needsFens = false;
		wxStopWatch sw;
		long time_inc_01 = 0;
		if( !pgnLoad.games[i]->HasGeneratedFens() )
		{
			wxStopWatch sw;
			int ret = pgnLoad.games[i]->Parse();

			if( ret == 0 )
			{
				wxMessageBox(pgnLoad.games[i]->GetParseError(),"Error");
				return;
			}
			pgnLoad.games[i]->ParseHeader();

			needsFens = true;
		}

		if( needsFens )
		{
			PGN::Move * pMove = pgnLoad.games[i]->pRoot;
			while(pMove)
			{
				if( pMove->white != "" )
				{
					//time_01 = sw2.Time();
					board_canvas->GetBoard()->parseMove(pMove->white);
					//time_02 = sw2.Time();
					//total_time_in_variation += time_02 - time_01;
					
					pMove->SetFenWhite(board_canvas->GetBoard()->Fen());
					
					
					pMove->SetWhiteMoveSquares(board_canvas->GetBoard()->currMove);
					board_canvas->GetBoard()->SetBoard(pMove->m_fenWhite);
				}
				else board_canvas->GetBoard()->mov = 1;

				//time_01 = sw2.Time();
				board_canvas->GetBoard()->parseMove(pMove->black);
				//time_02 = sw2.Time();
				//total_time_in_variation += time_02 - time_01;
				//time_01 = sw2.Time();
				pMove->SetFenBlack(board_canvas->GetBoard()->Fen());
				//time_02 = sw2.Time();
				//total_time_in_variation += time_02 - time_01;
				pMove->SetBlackMoveSquares(board_canvas->GetBoard()->currMove);
				board_canvas->GetBoard()->SetBoard(pMove->m_fenBlack);
				
				//currentGame->NextMove();
				pMove->PrepareAnnotations();
				CreateFenForVariation(pMove);
				board_canvas->GetBoard()->SetBoard( pMove->getFenBlack());
				pMove = pMove->pNext;
			}
		}
		//time_inc_01 = sw.Time();
		//fileLoadLogger << "Timer To Parse File: " + time_inc_01;
		PGN::Move* move = pgnLoad.games[i]->pRoot->GetLastMove();
		if( move->black == "")
		{
			string inpt = "{ ";
			for( unsigned int j = 0; j < pgnLoad.games[i]->vec_header.size(); j++ )
			{
				if( pgnLoad.games[i]->vec_header[j].data.find("White ") != -1 )
				{
					inpt += GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Black ") != -1 )
				{
					inpt += " " + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("ECO ") != -1 )
				{
					inpt += " ECO:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Opening ") != -1 )
				{
					inpt += " Opening:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Variation ") != -1 )
				{
					inpt += " Variation:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
			}

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(move->annotation_white.text));
			move->annotation_white.text.push_back("}");
			vector<string>::iterator strit = move->annotation_white.text.begin();
			
		/*	while( strit < move->annotation_white.text.end())
			{
				move->annotation_white.text.insert(strit, " ");
				if( strit + 1 !=  move->annotation_white.text.end() )
					strit++;
				else
					break;
				if( strit + 1 !=  move->annotation_white.text.end() )
					strit++;
				else 
					break;
			}*/
		}
		else
		{
			string inpt = "{ ";
			for( unsigned int j = 0; j < pgnLoad.games[i]->vec_header.size(); j++ )
			{
				if( pgnLoad.games[i]->vec_header[j].data.find("White ") != -1 )
				{
					inpt += GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Black ") != -1 )
				{
					inpt += " " + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("ECO ") != -1 )
				{
					inpt += " ECO:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Opening ") != -1 )
				{
					inpt += " Opening:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
				if( pgnLoad.games[i]->vec_header[j].data.find("Variation ") != -1 )
				{
					inpt += " Variation:" + GetSubstringInQuotes(pgnLoad.games[i]->vec_header[j].data);
				}
			}

			istringstream iss(inpt);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
					back_inserter<vector<string> >(move->annotation_black.text));
			move->annotation_black.text.push_back("}");
			vector<string>::iterator strit = move->annotation_black.text.begin();
			


			/*while( strit < move->annotation_black.text.end())
			{
				move->annotation_black.text.insert(strit, " ");
				if( strit < move->annotation_black.text.end() )
					strit++;
				else
					break;
				if( strit+1 != move->annotation_black.text.end() )
					strit++;
				else
					break;
			}*/
		}

		long time_inc_02 = sw.Time() - time_inc_01;

		fileLoadLogger << "  Time to Set Annotation: "<<time_inc_02;
		//pgnLoad.games[i]->Parse();
		pgnLoad.games[0]->MergeGame(pgnLoad.games[i]);

		long time_inc_03 = sw.Time() - time_inc_02;

		fileLoadLogger << "  Time to merge game: "<< i << " was "<<time_inc_03 << endl;
		//pgnLoad.RemoveLastGame();
	}

	//RecreateList(wxLC_REPORT | wxLC_SINGLE_SEL);
	//RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
	//board_canvas->Refresh(true);
	my_canvas->move_count = 0;
	//this->my_canvas->Refresh(true);
	wxStopWatch sw2;
	PGN::Move* pMove = pgnLoad.games[0]->pRoot;
	long time_01 = 0;
	long time_02 = 0;
//	long total_time_in_parse = 0;
	//long 
		total_time_in_variation = 0;
		profile_enabled = true;


	fileLoadLogger << "Time To Merge Games: "<< sw2.Time() << endl;
	fileLoadLogger << "Total Time In Parse: "<<total_time_in_variation<<endl;

	currentGame = pgnLoad.games[0];
	currentGame->pCurrMove = pgnLoad.games[0]->pRoot;
	currentGame->SetHasGeneratedFens();

	

	ostringstream of;
	currentGame->GameToBuffer(of);

	pgnLoad.Reset();

	currentGame = pgnLoad.LoadPgnFromString(of.str().c_str());

	if( !currentGame->Parse() )
	{
		wxMessageBox("Error Parsing Merged Game", "Merge Error");
	}

	currentGame->pCurrMove = currentGame->pRoot;

	if( !this->GenerateFensCurrentGame() )
	{
		wxMessageBox("Error Generating Current Game", "Game Generation Error");
	}
		

	pgnLoad.games.push_back(currentGame);

	m_dataView->AppendGameList(&pgnLoad);

	currentGame->ResetMoveList();
	//currentGame->EnumerateVariations();
	board_canvas->ResetBoard();
}

void MyFrame::SaveLinesInFullAnalysis()
{
	saveLinesInFullAnalysisMode = true;

}

void MyFrame::OnFullAnalysis()
{
	StopAllAnimation();

	/*if( this->m_gameTraverseTimer )
	{
		this->m_gameTraverseTimer->Stop();
	}*/

	for( int v = 0; v < numActivePipePanels; v++ )
	{
		if( m_pipePanel[v]->IsActive() &&
			m_pipePanel[v]->IsAnalysing() )
		{
			m_pipePanel[v]->KibitzStartsMove();
		}
	}
	fullAnalysisMode = true;
    MyWizard wizard(this);

    wizard.RunWizard(wizard.GetFirstPage());
	/*FullAnalysisDialog dialog(this,this, wxID_ANY,"Full Analysis Dialog");
	if( dialog.ShowModal() == wxID_OK )
	{
		wxMessageBox("Ok Clicked","Random Message");
		//this->m_fullAnalysisSettings.engine1 
	}*/

	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() )
		{
			if( m_pipePanel[i]->GetEngineName() == this->m_fullAnalysisSettings.engine1 )
			{
				this->m_fullAnalysisSettings.pEngine[0] = m_pipePanel[i];
			}
		}
	}
	if( m_fullAnalysisSettings.pEngine[0] && m_fullAnalysisSettings.pEngine[0]->IsActive() )
	{
		
		m_fullAnalysisSettings.pEngine[0]->KibitzUserNewGame();
		if( this->m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_TIME )
		{
			if( this->m_fullAnalysisSettings.guiControlsTime )
			{
				m_fullAnalysisSettings.pEngine[0]->SetFAGoMode(GO_INFINITE);
			}
			else
			{
				m_fullAnalysisSettings.pEngine[0]->SetFAGoMode(GO_FIXED_TIME);
				m_fullAnalysisSettings.pEngine[0]->fixedTimeMillis = m_fullAnalysisSettings.timePerPlySeconds * 1000;
				
			}
		}
		else if( this->m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_DEPTH )
		{
			m_fullAnalysisSettings.pEngine[0]->SetFAGoMode(GO_FIXED_DEPTH);
			m_fullAnalysisSettings.pEngine[0]->fixedDepth = m_fullAnalysisSettings.depthPerPly;
		}
		else if( this->m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_DEPTH_AND_TIME )
		{
			m_fullAnalysisSettings.pEngine[0]->SetFAGoMode(GO_FIXED_DEPTH_AND_TIME);
			m_fullAnalysisSettings.pEngine[0]->fixedDepth = m_fullAnalysisSettings.depthPerPly;
		}
		else if( this->m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_DEPTH_OR_TIME )
		{
			m_fullAnalysisSettings.pEngine[0]->SetFAGoMode(GO_FIXED_DEPTH_OR_TIME);
			m_fullAnalysisSettings.pEngine[0]->fixedDepth = m_fullAnalysisSettings.depthPerPly;
		}
	//	m_pipePanel[0]->GetNumKbtzLines
		m_fullAnalysisSettings.pEngine[0]->SendMPVModeOption(m_fullAnalysisSettings.numLines);

		m_fullAnalysisSettings.pEngine[0]->SetFullAnalyzeMode();
		m_fullAnalysisSettings.pEngine[0]->StartAnalyzing();
		
	}
	else
	{
		fullAnalysisMode = false;
		return;
	}
	if( m_gameTraverseTimer == NULL )
	{
		m_gameTraverseTimer = new TraverseGameTimer(this);
		
	}

	if( !currentGame->pCurrMove->IsThisMainLine() )
		this->m_fullAnalysisSettings.analyzeCurrentVariation = true;

	//m_fullAnalysisSettings.
	if ( m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_DEPTH )
	{
		m_gameTraverseTimer->SetTimeToPause(1000);
	}
	else if( m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_TIME )
	{
		if( m_fullAnalysisSettings.guiControlsTime )
		{
			m_gameTraverseTimer->SetTimeToPause(this->m_fullAnalysisSettings.timePerPlySeconds*1000);
		}
		else
		{
			m_gameTraverseTimer->SetTimeToPause(1000);
		}
	}
	
	currentGame->PrepareForVisitor();
	m_gameTraverseTimer->start(); 


}

void MyFrame::DoStepFullAnalysis()
{
}

void MyFrame::OnFinishFullAnalysisOfVariation()
{
	if( m_fullAnalysisSettings.writeToAnnotation )
	{
		currentGame->CopyStoredScoresToAnnotationForVariation();
	}

	if( m_fullAnalysisSettings.colorCodeMatches )
	{
		currentGame->ColorCodeFullAnalysisMatchesInVariation(this->m_moveColorPallete);
	}

	fullAnalysisMode = false;

	//my_canvas->white_black_move = BLACK;
	this->m_gameTraverseTimer->Stop();
	for( int v = 0; v < numActivePipePanels; v++ )
	{
		if( m_pipePanel[v]->IsActive() &&
			m_pipePanel[v]->IsAnalysing() )
		{
			m_pipePanel[v]->KibitzStartsMove();
			m_pipePanel[v]->SetAnalyzeMode();
			m_pipePanel[v]->SetGoMode(GO_INFINITE);
			m_pipePanel[v]->SetFAGoMode(GO_INFINITE);
		}
	}
}
	
void MyFrame::StopPlayThroughGame()
{
	if( m_gameTraverseTimer != NULL )
	{
		m_gameTraverseTimer->Stop();
		board_canvas->bAnimating = false;
	}

	int t1w = 0;
	int t2w = 0;
	int t3w = 0;
	int t4w = 0;

	int t1b = 0;
	int t2b = 0;
	int t3b = 0;
	int t4b = 0;

	int numW = 0;
	int numB = 0;
	pgnLoad.games[0]->CountFullAnalysisMatches(t1w, t2w, t3w, t4w, numW, t1b, t2b, t3b, t4b, numB);

	wxMessageBox( wxString::Format("white moves: %d, t1=%f, t2=%f, t3=%f, t4=%f \n black moves: %d, t1=%f, t2=%f, t3=%f, t4=%f"
		, numW, 100.0f* float(t1w)/float(numW),100.0f* float(t2w)/float(numW),100.0f* float(t3w)/float(numW),100.0f* float(t4w)/float(numW),numB,
		100.0f* float(t1b)/float(numB),100.0f* float(t2b)/float(numB),100.0f* float(t3b)/float(numB),100.0f* float(t4b)/float(numB)) );
	//if( pgnLoad.games[10]->moves.size() == 0 )
	//	pgnLoad.games[10]->Parse();
//	OnMergeAll();
/**/
	//OnToggleD3DMode();

	//TimeControlDialog dlg(this, currentGame, this, wxID_ANY,"Time Control Dialog");
	//dlg.ShowModal();

	//string str = "";
	//wxMessageBox(wxString::Format("Capacity: %d, Size: %d",str.capacity(), str.size()),"");
   
	/*m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().
                  Caption(wxT("Information")).
                  Float().FloatingPosition(GetStartPosition()).
                  //FloatingSize(300,200).
				  CloseButton(true).MaximizeButton(true));

   m_mgr.Update();*/
}

void MyFrame::LoadECOClassifier()
{
//	return;
	if( ecoClassify.LoadPgn("pgn/britchamps2011/ECO_Test.pgn") != 1 )
	{
		wxMessageBox("Error Loading ECO Classifier", "Program Problem");
	}

	ecoClassify.ParseGames();

	PGN::Move* pMove = ecoClassify.games[0]->pRoot;
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

	ecoClassify.games[0]->SetHasGeneratedFens();

	ecoClassify.games[0]->ResetMoveList();
	//currentGame->EnumerateVariations();
	board_canvas->ResetBoard();

	//wxMessageBox("ECO Classifier Loaded OK","");
}


void MyFrame::ClassifyECO()
{
	wxString newEco;
	if( currentGame->pCurrMove->IsThisMainLine() )
	{
		newEco = ecoClassify.games[0]->pRoot->PushMove2(currentGame->pRoot);

	
		if( newEco != "" )
		{
			this->currentECO = newEco;
		}
	}
	else
	{
		PGN::Move* pRootNew = currentGame->pCurrMove->GetMoveChainFromRootToVariation();

		newEco = ecoClassify.games[0]->pRoot->PushMove2(pRootNew->pNext);

		if( newEco != "" )
		{
			this->currentECO = newEco;
		}

		delete pRootNew;
	}
}

void MyFrame::IncMove()
{
	bool enterVariations = true;

	if( this->fullAnalysisMode )
	{
		if( (m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_TIME) &&
			(m_fullAnalysisSettings.guiControlsTime == false) &&
			 m_fullAnalysisSettings.pEngine[0] &&
			(m_fullAnalysisSettings.pEngine[0]->fa_goFixedTimeFinished == false))
		{
			return;
		}

		if((m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_DEPTH ) && 
			m_fullAnalysisSettings.pEngine[0] &&
			(m_fullAnalysisSettings.pEngine[0]->fa_goFixedDepthFinished == false))
		{
			return;
		}

		if( (m_fullAnalysisSettings.go_command == FullAnalysisSettings::GO_DEPTH_AND_TIME) &&
			m_fullAnalysisSettings.pEngine[0] &&
			(m_fullAnalysisSettings.pEngine[0]->fa_goFixedDepthFinished == false))
		{
			return;
		}
	}


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

								
				//StopEnginesBeforeMoveChange();

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
						/*!currentGame->pCurrMove->atEndBlack*/!currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK))
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
							else 
							{
								//if( !board_canvas->bAnimating )
								//	StartEnginesAfterMoveChange();
								
								return;
							}

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

						//	if( !board_canvas->bAnimating )
						//		StartEnginesAfterMoveChange();
						//m_owner->treeCanvas->Refresh(true);
							return;
						}
					}

					if( currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					
					{
						if( currentGame->AtEnd())
						{
								if( /*currentGame->pCurrMove->atEndWhite*/currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE) )
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
												
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
												
													return;
												}
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
												
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												
												
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
													return;
												}
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
											}
																					
										}
										//return;
									}
									//my_canvas->white_black_move = WHITE;
									my_canvas->move_count++;
									treeUsed = true;
								}
								else if( /*currentGame->pCurrMove->atEndBlack*/currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK) )
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
												
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
													return;
												}
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
												
													return;
												}
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
							//	if( !board_canvas->bAnimating )
							//		StartEnginesAfterMoveChange();
								
								return;
							}
						}

					}
					//else treeUsed = true;
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

						//if( !board_canvas->bAnimating )
						//	StartEnginesAfterMoveChange();
						
						return;
					}

					if(currentGame->GetCurrentMove()->GetVariationRoot()!=NULL)
					{
						if( currentGame->AtEnd())
						{
							if( /*currentGame->pCurrMove->atEndWhite*/currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE) )
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
												
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
													return;
												}
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
												
													return;
												}
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
												
											}
										}

										treeUsed = true;
										//return;
									}
								//	my_canvas->white_black_move = WHITE;
								}
								else if( /*currentGame->pCurrMove->atEndBlack*/ currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK) )
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
												
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
													return;
												}
												currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
											}
											else
											{
												my_canvas->white_black_move = WHITE;
												
												if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
												{
													OnFinishFullAnalysisOfVariation();
													currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
													return;
												}
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
							//	if( !board_canvas->bAnimating )
								//	StartEnginesAfterMoveChange();
								
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

						//if( !board_canvas->bAnimating )
						//	StartEnginesAfterMoveChange();
						return;
					}
				}

				

				if( currentGame->AtEnd()  )
				{
					board_canvas->pCurrMov=(MyChessMove*)0xffffff;

					if( /*currentGame->pCurrMove->atEndWhite*/ currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_WHITE) )
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
									if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
									{
										OnFinishFullAnalysisOfVariation();
										currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
									
										return;
									}
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
									
								}
								else
								{
									my_canvas->white_black_move = WHITE;
									if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
									{
										OnFinishFullAnalysisOfVariation();
										currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
									
										return;
									}
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
									
								}
							}
							else 
							{
								if( this->m_gameTraverseTimer )
								{
									if( playThroughGameList )
									{
										this->OnPlayThroughGameList(currentPlayGameNumber++ );
										
										//if( !board_canvas->bAnimating )
										//	StartEnginesAfterMoveChange();
										
										
									}
									else
									{
										if( this->fullAnalysisMode )
										{
											if( m_fullAnalysisSettings.writeToAnnotation )
											{
												currentGame->CopyStoredScoresToAnnotationForMainLine();
											}
											if( m_fullAnalysisSettings.colorCodeMatches )
											{
												currentGame->ColorCodeFullAnalysisMatches(this->m_moveColorPallete);
											}
										}
										my_canvas->white_black_move = WHITE;
										this->m_gameTraverseTimer->Stop();
										for( int v = 0; v < numActivePipePanels; v++ )
										{
											if( m_pipePanel[v]->IsActive() &&
												m_pipePanel[v]->IsAnalysing() )
											{
												m_pipePanel[v]->KibitzStartsMove();
											}
										}
										
									}

									return;

								}
							}
						}
						
						my_canvas->white_black_move = WHITE;
					}
					else if( /*currentGame->pCurrMove->atEndBlack*/ currentGame->pCurrMove->MoveFlagIsSet(MOVE_FLAG_AT_END_BLACK) )
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
									if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
									{
										OnFinishFullAnalysisOfVariation();
										currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
									
										return;
									}
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot()->pPrev;
									
								}
								else
								{
									my_canvas->white_black_move = WHITE;
									if( this->fullAnalysisMode && this->m_fullAnalysisSettings.analyzeCurrentVariation )
									{
										OnFinishFullAnalysisOfVariation();
										currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
									
										return;
									}
									currentGame->pCurrMove = currentGame->pCurrMove->GetVariationRoot();
									
								}
							}
							else
							{
								if( this->m_gameTraverseTimer )
								{
									if( playThroughGameList )
									{
										this->OnPlayThroughGameList(currentPlayGameNumber++ );
										
										//if( !board_canvas->bAnimating )
										//	StartEnginesAfterMoveChange();
										
										
									}
									else
									{
										if( this->fullAnalysisMode )
										{
											if( m_fullAnalysisSettings.writeToAnnotation )
											{
												currentGame->CopyStoredScoresToAnnotationForMainLine();
											}
											if( m_fullAnalysisSettings.colorCodeMatches )
											{
												currentGame->ColorCodeFullAnalysisMatches(this->m_moveColorPallete);
											}
										}
										my_canvas->white_black_move = BLACK;
										this->m_gameTraverseTimer->Stop();
										for( int v = 0; v < numActivePipePanels; v++ )
										{
											if( m_pipePanel[v]->IsActive() &&
												m_pipePanel[v]->IsAnalysing() )
											{
												m_pipePanel[v]->KibitzStartsMove();
											}
										}
									}

									return;
								}
							}
							//return;
						}
						
					}

					bStartAnimation = false;
					//break;
				}

				if( currentGame->pCurrMove->info == "" )
				{
					if( this->m_gameTraverseTimer )
					{
						if( playThroughGameList )
						{
							this->OnPlayThroughGameList(currentPlayGameNumber++ );
							
							//if( !board_canvas->bAnimating )
							//	StartEnginesAfterMoveChange();
							
							
						}
						else
						{
							if( this->fullAnalysisMode )
							{
								if( m_fullAnalysisSettings.writeToAnnotation )
								{
									currentGame->CopyStoredScoresToAnnotationForMainLine();
								}

								if( m_fullAnalysisSettings.colorCodeMatches )
								{
									currentGame->ColorCodeFullAnalysisMatches(this->m_moveColorPallete);
								}
							}

							this->m_gameTraverseTimer->Stop();
							for( int v = 0; v < numActivePipePanels; v++ )
							{
								if( m_pipePanel[v]->IsActive() &&
									m_pipePanel[v]->IsAnalysing() )
								{
									m_pipePanel[v]->KibitzStartsMove();
								}
							}
							
						}

						return;
					}
				}
		

				//MyChessMove mv = 
				if( my_canvas->white_black_move == WHITE )
				{
					
					//ctrl->chess_board.parseMove( currentGame->GetSelectedMove(this->my_canvas->selectedMove)->white );
					//m_owner->ctrl->chess_board.parseMove( m_owner->currentGame->GetNextMove()->white );
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetWhiteMove());
					
					board_canvas->GetBoard()->SetBoard( currentGame->CurrFenWhite() );
				
					my_canvas->white_black_move = BLACK;

					// updata thee progress data
					if( m_dataView->IsShown() )
					{
						m_dataView->SetSelectedItemProgress((int) 100*((float)my_canvas->move_count/(float)currentPlayGamePly));
						m_dataView->Refresh(true);
					}

					wnd10->Clear();
					if( currentGame->pCurrMove->annotation_white.text.size() > 0 )
					{
						wxString inpt ="";

						for( unsigned int aa = 0; aa < currentGame->pCurrMove->annotation_white.text.size(); aa++ )
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
					if( (optionsFrame->m_GameOptions.animate == true) && (this->gameMode!= 1))
						board_canvas->GetLastMoveInCaseOfCapture(currentGame->pCurrMove->GetBlackMove());
					
					board_canvas->GetBoard()->SetBoard( currentGame->CurrFenBlack() );
					my_canvas->white_black_move = WHITE;

					// updata thee progress data
					if( m_dataView->IsShown() )
					{
						m_dataView->SetSelectedItemProgress((int) 100*((float)my_canvas->move_count/(float)currentPlayGamePly));
						m_dataView->Refresh(true);
					}
					wnd10->Clear();
					if( currentGame->pCurrMove->annotation_black.text.size() > 0 )
					{
						wxString inpt = "";
						for( unsigned int aa = 0; aa < currentGame->pCurrMove->annotation_black.text.size(); aa++ )
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

				//if( !board_canvas->bAnimating )
				//	StartEnginesAfterMoveChange();
				if( fullAnalysisMode )
				{
					if( m_fullAnalysisSettings.pEngine[0] && m_fullAnalysisSettings.pEngine[0]->IsActive() &&
						m_fullAnalysisSettings.pEngine[0]->IsAnalysing() )
					{
						m_fullAnalysisSettings.pEngine[0]->KibitzStartsMove();
					}
				}

				if( (board_canvas->GetBoard()->GetGameStatus() != CHECKMATE) &&
					(board_canvas->GetBoard()->GetGameStatus() != STALEMATE))
				{
					if( fullAnalysisMode && m_fullAnalysisSettings.pEngine[0] )
					{
						if( !currentGame->AtEnd() )
						{
							if( m_fullAnalysisSettings.generateAlternativeLines )
							{
								m_fullAnalysisSettings.pEngine[0]->CopyAllToNotation();
							}
						}
						m_fullAnalysisSettings.pEngine[0]->KibitzUserEndsMove();
					}
					
				}
			/*	for( int v = 0; v < numActivePipePanels; v++ )
				{
					if( m_pipePanel[v]->IsActive() &&
						m_pipePanel[v]->IsAnalysing() )
					{
						m_pipePanel[v]->KibitzStartsMove();

						if( (board_canvas->GetBoard()->GetGameStatus() != CHECKMATE) &&
							(board_canvas->GetBoard()->GetGameStatus() != STALEMATE))
						{
							if( fullAnalysisMode )
							{
								if( !currentGame->AtEnd() )
								{
									if( m_fullAnalysisSettings.generateAlternativeLines )
									{
										m_pipePanel[v]->CopyAllToNotation();
									}
								}
							}
							m_pipePanel[v]->KibitzUserEndsMove();
						}
					}
				}*/
				
				//Refresh();
			//	ctrl->chess_board.

}



void MyFrame::EngineVsEngine::Setup()
{
	if( engine01->IsActive() && engine02->IsActive() )
	{
		engine01->SetEngineColor(WHITE);
		engine02->SetEngineColor(BLACK);
		engine01->SetOpponent(engine02);
		engine02->SetOpponent(engine01);
	}
	else
	{
		wxMessageBox("Error 2 engines must be loaded", "Error");
	}
}

void MyFrame::AjournGame()
{
	if( enginesMatch == false )
		return;

	for( int i = 0; i < numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() )
		{
			m_pipePanel[i]->KibitzStops();
		}
	}

	string result = "1/2-1/2";
	currentGame->AddResultNode(result);
	currentGame->WriteResultIntoHeader();

	string gameResultString = "Game Ajourned by Admin";
	currentGame->WriteTerminationIntoHeader(gameResultString);
	

	if( m_engineVsEngine.engine01 && m_engineVsEngine.engine01->IsActive())
		m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);

	if( m_engineVsEngine.engine02 && m_engineVsEngine.engine02->IsActive())
		m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);

	gameClock->Stop();
	enginesMatch = false;

	NextEngineVsEngineGame();
}

void MyFrame::StopMatch()
{
	if( enginesMatch == false )
		return;

	for( int i = 0; i < numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() )
		{
			m_pipePanel[i]->KibitzStops();
		}
	}

	string gameResult = "1/2-1/2";
	currentGame->AddResultNode(gameResult);
	currentGame->WriteResultIntoHeader();

	string gameResultString = "Game Stopped By Admin";
	currentGame->WriteTerminationIntoHeader(gameResultString);

	if( m_engineVsEngine.engine01 )
		m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);

	if( m_engineVsEngine.engine02 )
		m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);

	//this->playersOpponent->m_readTimer->SetTimeBetweenReads(100);

	gameClock->Stop();
	enginesMatch = false;

}


void MyFrame::NextEngineVsEngineGame()
{
	if( AutosaveScheduled  )
	{
		DoAutosave();
	}

	this->enginesMatch = true;

	// first locate the listed engine if available...
/*	for( int i = 0; i < subframe->numActivePipePanels; i++ )
	{
		if( subframe->m_pipePanel[i] && subframe->m_pipePanel[i]->IsActive() )
		{
			if( subframe->m_engineVsEngine.engine1Name == subframe->m_pipePanel[i]->GetEngineName())
				subframe->m_engineVsEngine.engine01 = subframe->m_pipePanel[i];

			if( subframe->m_engineVsEngine.engine2Name == subframe->m_pipePanel[i]->GetEngineName())
				subframe->m_engineVsEngine.engine02 = subframe->m_pipePanel[i];
		}
	}

	if( ((subframe->m_engineVsEngine.engine02 == NULL)|| (!subframe->m_engineVsEngine.engine02->IsActive()) ) ||
		((subframe->m_engineVsEngine.engine01 == NULL)|| (!subframe->m_engineVsEngine.engine01->IsActive()) ) )
	{
		wxMessageBox("Load 2 engines first");
		return;
	}*/
	//---------------------------------------------------------------

	//engine located, now proceed

	

	if( this->m_engineVsEngine.numGames <= this->m_engineVsEngine.numGamesPlayed )
	{
		return;
	}

	this->currentGame->WriteEventNameIntoHeader(this->m_engineVsEngine.eventName);
	this->ClassifyECO();
	string eco = this->currentECO.c_str().AsChar();
	this->currentGame->WriteECONameIntoHeader(eco);

	match_results->Refresh(true);

	if( this->m_engineVsEngine.writeAllToSameGame == false )
	{
		this->NewGame(false);
		this->m_engineVsEngine.list_engine_games.push_back(this->currentGame);
		this->currentGame->SetHasGeneratedFens();
		if( this->m_engineVsEngine.openings.size() > 0 )
		{
			this->currentGame->CopyGame(this->m_engineVsEngine.openings[0]);
			this->currentGame->pCurrMove = this->currentGame->TraverseToLastMove();
			if( currentGame->pCurrMove->black == "" )
			{
				if( currentGame->pCurrMove->white != "" )
				{
					board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
					//board_canvas->Refresh();
				}
				else
				{
					this->currentGame->pCurrMove = currentGame->pRoot;
				}
			}
			else
			{
				board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);
				//board_canvas->Refresh();
			}
		}
	}
	else
	{
		/*int count = 0;
		int wb = 0;
		PGN::Move* pMove = currentGame->pRoot;
		while( pMove )
		{
			count++;
			if( count == m_engineVsEngine.plyForBook )
			{
				wb = 1;
				break;
			}
			
			count++;
			if( count == m_engineVsEngine.plyForBook )
			{
				wb = 0;
				break;
			}
			pMove = pMove->pNext;
		}*/
		if( !(currentGame->pCurrMove->GetVariationRoot() == NULL) )
		{
			currentGame->pCurrMove = currentGame->pCurrMove->PromoteVariation(currentGame->pCurrMove);
		}

		pgnLoad.ClearHashTable();

		currentGame->pCurrMove = this->m_engineVsEngine.m_MCTS->GetStartNode()->pMove;
		if( this->m_engineVsEngine.m_MCTS->GetStartNode()->IsBlackMove() )
			board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenWhite);
		else
			board_canvas->GetBoard()->SetBoard(currentGame->pCurrMove->m_fenBlack);

		for( int j = 0; j < this->numActivePipePanels; j++ )
		{
			if( m_pipePanel[j]->IsActive())//&&
				//board_canvas->activeEngines[j]->IsAnalysing())
			{
				m_pipePanel[j]->KibitzStops();
				if( this->m_engineVsEngine.writeAllToSameGame == false )
				{
					m_pipePanel[j]->KibitzUserNewGame();
				}
				
			}
		}
	}

	if( this->m_engineVsEngine.swapSidesBetweenGames == true )
	{
		MyPipePanel *temp = this->m_engineVsEngine.engine01;
		this->m_engineVsEngine.engine01 = this->m_engineVsEngine.engine02;
		this->m_engineVsEngine.engine02 = temp;
	}


	//int tickMillis;

	if( this->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::HYPER_BULLET )
	{
		//this->gameClock->SetTimeControl(0,10,0, 1);

		this->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(1);
		this->m_engineVsEngine.engine01->SetTimeMode(HYPER_BULLET);
		this->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		this->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(1);
		this->m_engineVsEngine.engine02->SetTimeMode(HYPER_BULLET);
		this->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( this->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::BULLET )
	{
		//this->gameClock->SetTimeControl(1,0,0, 50);

		this->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(50);
		this->m_engineVsEngine.engine01->SetTimeMode(BULLET);
		this->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		this->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(50);
		this->m_engineVsEngine.engine02->SetTimeMode(BULLET);
		this->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
	}
	else if( this->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::BLITZ )
	{
		//this->gameClock->SetTimeControl(5,0,3000, 100);

		this->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine01->SetTimeMode(BLITZ);
		this->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		this->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine02->SetTimeMode(BLITZ);
		this->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( this->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::RAPID )
	{
		//this->gameClock->SetTimeControl(30,0,0, 100);

		this->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine01->SetTimeMode(RAPID);
		this->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		this->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine02->SetTimeMode(RAPID);
		this->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( this->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::STANDARD )
	{
		//this->gameClock->SetTimeControl(60,0,0, 100);

		this->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine01->SetTimeMode(STANDARD);
		this->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		this->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine02->SetTimeMode(STANDARD);
		this->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}
	else if( this->m_engineVsEngine.timeControlID == MyFrame::EngineVsEngine::LONG_TIME )
	{
		//this->gameClock->SetTimeControl(90,0,0, 100);

		this->m_engineVsEngine.engine01->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine01->SetTimeMode(LONG_TIME);
		this->m_engineVsEngine.engine01->SetGoMode(GO_TIME);
		this->m_engineVsEngine.engine02->m_readTimer->SetTimeBetweenReads(100);
		this->m_engineVsEngine.engine01->SetTimeMode(LONG_TIME);
		this->m_engineVsEngine.engine02->SetGoMode(GO_TIME);
	}

	this->gameClock->ResetTime();
		
	this->m_mgr.GetPane("Engine_Panel").Show();
	this->m_mgr.GetPane("Engine_Panel_2").Show();

	
	this->currentGame->whiteName = this->m_engineVsEngine.engine01->GetEngineName();
	this->currentGame->blackName = this->m_engineVsEngine.engine02->GetEngineName();
	
	for( unsigned int i = 0; i < this->currentGame->vec_header.size(); i++ )
	{
		if( this->currentGame->vec_header[i].data.find("White ")!=-1)
		{
			wxString str = this->currentGame->vec_header[i].data;
			SetSubstringInQuotes(str, this->m_engineVsEngine.engine01->GetEngineName());
			this->currentGame->vec_header[i].data = str;
		}
		if( this->currentGame->vec_header[i].data.find("Black ")!=-1) 
		{
			wxString str = this->currentGame->vec_header[i].data;
			SetSubstringInQuotes(str, this->m_engineVsEngine.engine02->GetEngineName());
			this->currentGame->vec_header[i].data = str;

		}
	}
	this->noteFrame->Refresh();

	wxString infoString = "Initializing, Please Wait";
	wxBusyInfo info(infoString, this);
	//wxBusyInfo info(loadingInfo, this->pParent);

    for ( int i = 0; i < 10; i++ )
    {
        wxMilliSleep(100);
        wxTheApp->Yield();
    }



	this->m_engineVsEngine.Setup();
	this->board_canvas->whoseMove = BaseGraphicsBoard::WAIT_COMPUTER;
	this->m_engineVsEngine.engine01->SetGameMode();
	this->m_engineVsEngine.engine02->SetGameMode();
	this->gameClock->StartGame();
	this->gameClock->UnPause();
	this->m_engineVsEngine.engine01->SetEngineColor(WHITE);
	this->m_engineVsEngine.engine02->SetEngineColor(BLACK);

	if( (board_canvas->GetBoard()->mov == 0) || 
		(board_canvas->GetBoard()->mov == 2) )
	{
		this->m_engineVsEngine.engine01->PlayerMoves();
	}
	else
	{
		this->m_engineVsEngine.engine02->PlayerMoves();
	}
	



}



void MyFrame::DoLookupFromBook()
{
	
	//if( my_canvas->move_count > 40 )
	//	return;
	//,moveData
	
	/*
	Owner Note: This section was unceremoniously segfaulted from
	Michael Van De Burgh's implementation on his website,
	The source was public domain.
	*/
	if( polyglotBook && 
		m_mgr.GetPane("moveData").IsShown())
	{
		
		uint64 curr_key;
		string startFen;


		if( board_canvas->GetBoard()->mov != 1 )
		{
			
			curr_key =  currentGame->pCurrMove->zhashKeyBlack;
			//if( res == -1 )
			//	return;
			//startFen = currentGame->pCurrMove->m_fenBlack;
			//temp_board.SetBoard(startFen);
		}
		else
		{
			
			curr_key =  currentGame->pCurrMove->zhashKeyWhite;
			//fen = currentGame->pCurrMove->m_fenBlack;
			//int res = find_key(m_frame->polyglotBook, m_frame->currentGame->pCurrMove->zhashKeyBlack,&entry );
			//if( res == -1 )
			//	return;
			//startFen = currentGame->pCurrMove->m_fenWhite;
			//temp_board.SetBoard(startFen);
		}

		if( curr_key == 0 )
			return;

		entry_t entry;
		int offset;
		//char *file_name;
		

		
		bookLookupCount=0;
		int ret;// i;
		
		//int total_weight;
		//rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq c3 1 2
		/*if(argc<=2){
			printf("Usage: pg_show <book> <hex key>\n");
			return 1;
		}*/
		


		//curr_key = hash_from_fen((char*)fen.c_str());
   
		offset=find_key(polyglotBook,curr_key,&entry);
		if(entry.key!=curr_key){

			return;
		}

		ChessBoard temp_board;
		temp_board.SetInitialPosition();
		if( board_canvas->GetBoard()->mov != 1 )
		{
			
			curr_key =  currentGame->pCurrMove->zhashKeyBlack;
			//if( res == -1 )
			//	return;
			startFen = currentGame->pCurrMove->m_fenBlack;
			temp_board.SetBoard(startFen);
		}
		else
		{
			
			curr_key =  currentGame->pCurrMove->zhashKeyWhite;
			//fen = currentGame->pCurrMove->m_fenBlack;
			//int res = find_key(m_frame->polyglotBook, m_frame->currentGame->pCurrMove->zhashKeyBlack,&entry );
			//if( res == -1 )
			//	return;
			startFen = currentGame->pCurrMove->m_fenWhite;
			temp_board.SetBoard(startFen);
		}
		//else
		{
			char move_s1[6];

			move_to_string(move_s1,entry.move);			
			string newmove1 = move_s1;
			newmove1[1]--;
			newmove1[3]--;
			bool promote1 = false;
			//int promoteType = 0;
			if( newmove1.length() == 5 )
			{
				// switch type
				promote1 = true;
			}
			entry.san = temp_board.EPN_Move_to_PGN_Move(newmove1, promote1,0);
			temp_board.SetBoard(startFen);
			entries[0]=entry;
		}
		//entries[0]=entry;
		bookLookupCount=1;
		fseek(polyglotBook,16*(offset+1),SEEK_SET);
		while(1){
			ret=entry_from_file(polyglotBook,&entry);
			if(ret){
				break;
			}
			if(entry.key!=curr_key){
				break;
			}
			if(bookLookupCount==100){
				//printf("Too many moves in this position (max=%d)\n",MAX_MOVES);
				return;
			}
			//switch()
			char move_s[6];

			move_to_string(move_s,entry.move);			
			string newmove = move_s;
			newmove[1]--;
			newmove[3]--;
			bool promote = false;
			//int promoteType = 0;
			if( newmove.length() == 5 )
			{
				// switch type
				promote = true;
			}
			entry.san = temp_board.EPN_Move_to_PGN_Move(newmove, promote,0);
			temp_board.SetBoard(startFen);
			entries[bookLookupCount++]=entry;
		}
	}

}


void MyFrame::ShowResults()
{
	if( this->enginesMatch == false )
		return;

	/*m_mgr.AddPane(CreateNotebook(), wxAuiPaneInfo().
                  Caption(wxT("Information")).
                  Float().FloatingPosition(GetStartPosition()).
                  //FloatingSize(300,200).
				  CloseButton(true).MaximizeButton(true));
*/
	if( m_mgr.GetPane("matchResults").IsShown()== false )
	{
		m_mgr.GetPane("matchResults").Show(true);
	}
	else
	{
		m_mgr.GetPane("matchResults").Show(false);
		m_mgr.GetPane("matchResults").Hide();
	}
   
	m_mgr.Update();

}

void MyFrame::PlayerResign()
{
	if( this->enginesMatch == true )
		return;

	engineVsHumanMatch = false;

	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( this->m_pipePanel[i]->IsActive() )
		{
			this->m_pipePanel[i]->KibitzStops();
		}
	}

	this->playersOpponent->m_readTimer->SetTimeBetweenReads(100);

	if( this->playersOpponent->GetEngineColor()== BLACK )
	{
		string blackWins = "0-1";
		currentGame->AddResultNode(blackWins);
	}
	else
	{
		string whiteWins = "1-0";
		currentGame->AddResultNode(whiteWins);
	}

	currentGame->WriteResultIntoHeader();
	this->ClassifyECO();
	string ecoName = this->currentECO.c_str().AsChar();
	currentGame->WriteECONameIntoHeader(ecoName);
	string eventName = "Man vs Machine";
	currentGame->WriteEventNameIntoHeader(eventName);
	this->gameClock->Stop();

	this->playersOpponent->ClearGameAnalysisBuffers();
	//subframe->enginesMatch = false;
}

void MyFrame::OnIdle(wxIdleEvent& event)
{
	if( AutosaveScheduled )
	{
		DoAutosave();
	}
}

bool MyFrame::IsAutoSaveActive()
{
	return (m_autoSaveTimer != NULL);
}

void MyFrame::ActivateAutosave()
{
	if( !m_autoSaveTimer )
	{
		m_autoSaveTimer = new AutosaveGameTimer(this);
	}
}

void MyFrame::DeactivateAutosave()
{
	if( m_autoSaveTimer )
	{
		delete m_autoSaveTimer;
		m_autoSaveTimer = NULL;
	}
}

void MyFrame::SetAutosaveFrequency(int timeInMinutes)
{
	if( m_autoSaveTimer )
	{
		m_autoSaveTimer->SetTimeToPause( timeInMinutes * 1000 );
	}
}

bool MyFrame::IsAppReadyForAutosave()
{
	for( int i = 0; i < this->numActivePipePanels; i++ )
	{
		if( m_pipePanel[i]->IsActive() && m_pipePanel[i]->IsAnalysing() )
		{
			return false;
		}
	}
	if( board_canvas->bAnimating )
	{
		return false;
	}

	if( m_gameTraverseTimer->IsRunning() )
	{
		return false;
	}

	if( this->enginesMatch )
	{
		return false;
	}

	return true;
}

void MyFrame::ScheduleAutosave()
{
	this->AutosaveScheduled = true;
}

void MyFrame::DoAutosave()
{
	this->AutosaveScheduled = false;
	if( !this->m_autoSaveTimer )
		return;

	ostringstream oss;
	for( unsigned int i = 0; i < pgnLoad.games.size(); i++ )
	{
		pgnLoad.games[i]->GameToBuffer(oss);
		oss << endl << endl;
	}

	ofstream ofs;
	ofs.open(this->m_autoSaveTimer->autosavePath);
	ofs << oss.str();
		
	//ofs.write
	ofs.close();
}


// Threads
void MyFrame::OnWorkerEvent(wxThreadEvent& event)
{
    int n = event.GetInt();
    if ( n == -1 )
    {
        m_dlgProgress->Destroy();
        m_dlgProgress = (wxProgressDialog *)NULL;

        // the dialog is aborted because the event came from another thread, so
        // we may need to wake up the main event loop for the dialog to be
        // really closed
        wxWakeUpIdle();
    }
    else
    {
        if ( !m_dlgProgress->Update(n) )
        {
            wxCriticalSectionLocker lock(m_csCancelled);

            m_cancelled = true;
        }
    }
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
    wxTimer::Start(timeToPause);
}


AutosaveGameTimer::AutosaveGameTimer(MyFrame *frame) : wxTimer()
{
    m_frame = frame;
	this->autosavePath = wxStandardPaths::Get().GetResourcesDir()+"/Session Data/Autosave/autosave.pgn";
	timeToPause = 5000;
}
 
void AutosaveGameTimer::Notify()
{
    //pane->Refresh(true);
	//if( pane->gameState == BoardCanvas::GameState::SELECTED_PIECE )
	//m_frame->IncMove();
	/*if( useCurrentDate )
	{
		wxDateTime dt;
		dt = dt.GetDateOnly();
		//string s = dt.GetStr
	}*/

	m_frame->ScheduleAutosave();


	//pane->Notified = true;
}
 
void AutosaveGameTimer::start()
{
    wxTimer::Start(timeToPause);
}


