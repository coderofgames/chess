/////////////////////////////////////////////////////////////////////////////
// Name:        mdi.cpp
// Purpose:     MDI sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: mdi.h 64940 2010-07-13 13:29:13Z VZ $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////
#ifndef MDI_H
#define MDI_H

#include "wx/toolbar.h"

#include "wx/ribbon/bar.h"
#include "wx/ribbon/buttonbar.h"
#include "wx/ribbon/gallery.h"
#include "wx/ribbon/toolbar.h"


#include "headers.h"
#include "wx/mdi.h"
#include "wx/dcmemory.h"

#include "GameDataViewPanel.h"
#include "BaseGraphicsBoard.h"
#include "PGKey.h"
#include "PGLookup.h"
#include "MCTS_Analysis.h"


class wxSizeReportCtrl;
class wxGLSizeReportCtrl;
class MyCanvas;
class BoardCanvas;
class MyListCtrl;
class MyPipePanel;
class MyThread;
class VariationSelectionFrame;
class NotationFrame;
class CLCDSampleFrame;
class ClockTimer;
class wxLCDWindow;
class MyListPanel;
class TreeCanvas;
class OptionsFrame;
class GameClockPanel;
class GameAnalyser;
class EngineManager;
class MoveDataCanvas;
class ScoreGraph;
class TreeStatsCanvas;
class ScorePanel;
class VariationBoardFrame;
class MatchResultsPanel;


#include "ColorPallete.h"

// Define a new application
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

#define MAX_NUM_PIPE_PANELS 30


class MyFrame;

// Define a new frame
class MDIParentFrame : public wxMDIParentFrame
{
public:
    MDIParentFrame();
    virtual ~MDIParentFrame();

    enum
    {
        ID_CIRCLE = wxID_HIGHEST + 1,
		ID_OPEN,
		ID_SAVE,
		ID_LOAD_PGN,
		ID_OPEN_BTN,
		ID_SAVE_BTN,
		ID_LOAD_PGN_BTN,
		ID_ADD_PGN,
		ID_PASTE_PGN,
		ID_MERGE_PGN_FROM_CLIPBOARD,
		ID_MERGE_PGN_FROM_DISK,
		ID_COPY_PGN_TO_CLIPBOARD,
		ID_COPY_FEN_TO_CLIPBOARD,
		ID_COPY_VARIATION_TO_CLIPBOARD,
		ID_PASTE_FEN,
		ID_SAVE_PGN,
		ID_SAVE_GAME_LIST_AS_DATABASE,
		ID_NEW_GAME,
		ID_CLIPBOARD_BTN,
		ID_SETUP_POSITION,
        ID_CROSS,
        ID_TRIANGLE,
        ID_SQUARE,
		ID_NEW_GAME_BTN,
		ID_SQUARE_BTN,
        ID_POLYGON,
        ID_SELECTION_EXPAND_H,
        ID_SELECTION_EXPAND_V,
        ID_SELECTION_CONTRACT,
		ID_FLIP_BOARD,
		ID_FLIP_BOARD_BTN,
		ID_INFO,
		ID_EDIT_PGN_HEADER,
		ID_EDIT_PGN_HEADER_BTN,
		ID_EDIT_ANNOTATION,
		ID_DELETE_REMAINING_MOVES,
		ID_DELETE_VARIATION,
		ID_PROMOTE_VARIATION,
        ID_PRIMARY_COLOUR,
        ID_SECONDARY_COLOUR,
        ID_DEFAULT_PROVIDER,
        ID_AUI_PROVIDER,
        ID_MSW_PROVIDER,
        ID_MAIN_TOOLBAR,
		ID_MAIN_TOOLBAR2,
        ID_POSITION_TOP,
        ID_POSITION_TOP_ICONS,
        ID_POSITION_TOP_BOTH,
        ID_POSITION_LEFT,
        ID_POSITION_LEFT_LABELS,
        ID_POSITION_LEFT_BOTH,
		ID_RIBBON_BAR_TAB_LEFT_CLICKED,
		ID_TOGGLE_MAXIMIZE,
		ID_LOAD_MEDIA_FROM_DISK,
		ID_LOAD_MEDIA_FROM_URL,
		ID_MEDIA_OPTIONS,
		ID_SWAP_SIDES,
		ID_CLOCK,
		ID_CLOCK_SET_TIME_CONTROLS,
		ID_CLOCK_RESET,
		ID_CLOCK_SET_TIME,
		ID_PLAY_THROUGH_GAME,
		ID_STOP_PLAY_THROUGH_GAME,
		ID_MERGE_ALL_GAMES,
		ID_MERGE_SELECTED_GAMES,
		ID_PLAY_THROUGH_GAME_LIST,
		ID_REMOVE_GAME_FROM_DB,
		ID_ADD_GAME_TO_DB,
		ID_LOAD_DB,
		ID_SAVE_DB,
		ID_EXPORT_DB,
		ID_COMPRESS_DB,
		ID_EDIT_MODE,
		ID_ENGINES_MATCH,
		ID_SHOW_GRAPH,
		ID_STOP_ENGINES_PLAYING,
		ID_RESIGN,
		ID_TOURNAMENT,
		ID_FULL_ANALYSIS,
		ID_SHOW_RESULTS,
		ID_LOAD_BOOK,
		ID_SHOW_BOOK,
		ID_SHOW_CLOCK_WINDOW,
		ID_SEARCH_FILTER,
		ID_CHECK_ALL,
		ID_UNCHECK_ALL,
		ID_DATAVIEW_DELETE_ALL_SELECTED_GAMES,
		ID_COPY_GAME_LIST_GAME,
		ID_LOAD_PGN_VM,
		ID_ADD_PGN_VM,
		ID_LOAD_DB_VM,
		ID_CLEAR_DB_FILTER,
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
		ID_LOAD_SINGLE_GAME,
		ID_LOAD_GAME_LIST_TO_ACTIVE,
		ID_MOVE_UP,
		ID_MOVE_DOWN,
		ID_AJOURN_GAME,
		ID_OPEN_DB_NEW,
		ID_SHOW_VARIATION_BOARD,
		ID_FIND_NAME,
		ID_APPLY_FILTER_ON_SUBSET,
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

		ID_FIND_DROPDOWN,
		ID_FILTER_DROPDOWN,

		ID_POSITION_SEARCH,
	

		//ID_LOAD_DB,
		//ID_PLAY_AGAINST_ENGINE,
    };

    static wxMenuBar *CreateMainMenubar();

private:
    void InitToolBar(wxToolBar* toolBar);
	
    void OnSize(wxSizeEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnNewWindow(wxCommandEvent& event);
    void OnFullScreen(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnCloseAll(wxCommandEvent& event);

    void OnClose(wxCloseEvent& event);

    wxTextCtrl *m_textWindow;
public:
	
  
    


 //   void OnSquareButton(wxRibbonButtonBarEvent& evt);
	void OnSquareButton(wxRibbonToolBarEvent& evt);
	void OnNewGameButton(wxRibbonButtonBarEvent& evt);

	

	// no need for these
    void OnPolygonDropdown(wxRibbonButtonBarEvent& evt);
    void OnSelectionExpandHButton(wxRibbonButtonBarEvent& evt);
    void OnTriangleButton(wxRibbonButtonBarEvent& evt);
    void OnTriangleDropdown(wxRibbonButtonBarEvent& evt);


	// Tab 1 first toolbar
    void OnNew(wxRibbonToolBarEvent& evt);
	void OnNewDropdown(wxRibbonToolBarEvent& evt);
	void OnOpen(wxRibbonToolBarEvent& evt);
	void OnOpenDropdown(wxRibbonToolBarEvent& evt);
	void OnSave(wxRibbonToolBarEvent& evt);
	void OnSaveDropdown(wxRibbonToolBarEvent& evt);

	// Tab 1 new Ribbon Button Bar
    void OnClipboardBtn(wxRibbonButtonBarEvent& evt);
	void OnClipboardDropdownBtn(wxRibbonButtonBarEvent& evt);
	void OnOpenBtn(wxRibbonButtonBarEvent& evt);
	void OnOpenDropdownBtn(wxRibbonButtonBarEvent& evt);
	void OnSaveBtn(wxRibbonButtonBarEvent& evt);
	void OnSaveDropdownBtn(wxRibbonButtonBarEvent& evt);


	// Tab 1 main button bar/ engine/options/book/wizard
	void OnEngineLoadBtn(wxRibbonButtonBarEvent& evt);
	void OnOptions(wxRibbonButtonBarEvent& evt);
    


	// PGN editing button
	void OnInserAnnotationBtn(wxRibbonButtonBarEvent& evt);
	void OnEditAnnotation(wxRibbonButtonBarEvent& evt);
    void OnDeleteRemainingMoves(wxRibbonButtonBarEvent& evt);
	void OnDeleteVariation(wxRibbonButtonBarEvent& evt);
	void OnPromoteVariation(wxRibbonButtonBarEvent& evt);

	void RibbonTabDoubleClick(wxRibbonBarEvent& event);
	
	// Clock
	void OnClockDropdown(wxRibbonButtonBarEvent& evt);




	// play through game buttons
	void OnPlayThroughGame(wxRibbonButtonBarEvent& evt);
	void OnStopPlayThroughGame(wxRibbonButtonBarEvent& evt);
	void OnPlayThroughGameList(wxRibbonButtonBarEvent& evt);

	void OnFullAnalysis(wxRibbonButtonBarEvent& evt);


	void OnVariationBoard(wxRibbonButtonBarEvent& evt);

	// media tab
	void OnLoadMediaFromDisk(wxRibbonButtonBarEvent& evt);
	void OnLoadMediaFromURL(wxRibbonButtonBarEvent& evt);
	//void OnMediaOptions(wxRibbonButtonBarEvent& evt);

	// maximize Key event
	void ToggleMaximize(wxCommandEvent& evt);

	// info tab with fullscreen (add exit app)
	void OnInfo(wxRibbonButtonBarEvent& evt);
	void OnToggleFullScreenButton(wxRibbonButtonBarEvent& evt);

  //  
	// Game Tab Toolbar
	void OnEditPGNHeader(wxRibbonToolBarEvent& evt);
	void OnFlipBoard(wxRibbonToolBarEvent& evt);
	void OnSwapSides(wxRibbonButtonBarEvent& evt);

	void OnShowGraph(wxRibbonButtonBarEvent& evt);
	void OnShowResults(wxRibbonButtonBarEvent& evt);

	// Game Tab Button bar
	void OnEditPGNHeaderBtn(wxRibbonButtonBarEvent& evt);
	void OnFlipBoardBtn(wxRibbonButtonBarEvent& evt);
	void OnEngineMatchBtn(wxRibbonButtonBarEvent& evt);
	void OnStopEnginesPlayingBtn(wxRibbonButtonBarEvent& evt);
	void OnPlayerResign(wxRibbonButtonBarEvent& evt);
	void AjournGame(wxRibbonButtonBarEvent& evt);

	// merge all games (Database)
	void OnMergeAll(wxRibbonButtonBarEvent& evt);
	void OnMergeSelected(wxRibbonButtonBarEvent& evt);
	void OnMoveUp(wxRibbonButtonBarEvent& evt);
	void OnMoveDown(wxRibbonButtonBarEvent& evt);
	void OnCheckAll(wxRibbonButtonBarEvent& evt);
	void OnUnCheckAll(wxRibbonButtonBarEvent& evt);
	void OnDeleteAllSelectedGames(wxRibbonButtonBarEvent& evt);
	void OnCopyGame(wxRibbonButtonBarEvent& evt);

    void OnRemoveGameFromDB(wxRibbonButtonBarEvent& evt);
    void OnAddGameToDB(wxRibbonButtonBarEvent& evt);
	void OnSaveDB(wxRibbonButtonBarEvent& evt);
	void OnExportDB(wxRibbonButtonBarEvent& evt);
	void OnCompressDB(wxRibbonButtonBarEvent& evt);
	void OnEditMode(wxRibbonButtonBarEvent& evt);

	void OnOpenDB_New(wxRibbonButtonBarEvent& evt);

	void OnApplySearchFilter(wxRibbonButtonBarEvent& evt);
	void OnClearSearchFilter(wxRibbonButtonBarEvent& evt);
	void ApplyFilterOnSubset(wxRibbonButtonBarEvent& evt);
	void OnDatabaseShowFilters(wxRibbonButtonBarEvent& evt);
	void OnDatabaseFindDropdown(wxRibbonButtonBarEvent& evt);
	void OnDatabaseFilterDropdown(wxRibbonButtonBarEvent& evt);
	void OnDatabasePositionSearch(wxRibbonButtonBarEvent& evt);

    void OnHoveredColourChange(wxRibbonGalleryEvent& evt);
    void OnPrimaryColourSelect(wxRibbonGalleryEvent& evt);
    void OnSecondaryColourSelect(wxRibbonGalleryEvent& evt);
    void OnColourGalleryButton(wxCommandEvent& evt);
    void OnDefaultProvider(wxRibbonButtonBarEvent& evt);
    void OnAUIProvider(wxRibbonButtonBarEvent& evt);
    void OnMSWProvider(wxRibbonButtonBarEvent& evt);

   
    void OnPrint(wxRibbonToolBarEvent& evt);
    void OnPrintDropdown(wxRibbonToolBarEvent& evt);
    void OnRedoDropdown(wxRibbonToolBarEvent& evt);
    void OnUndoDropdown(wxRibbonToolBarEvent& evt);
    void OnPositionTop(wxRibbonToolBarEvent& evt);
    void OnPositionTopLabels(wxCommandEvent& evt);
    void OnPositionTopIcons(wxCommandEvent& evt);
    void OnPositionTopBoth(wxCommandEvent& evt);
    void OnPositionTopDropdown(wxRibbonToolBarEvent& evt);
    void OnPositionLeft(wxRibbonToolBarEvent& evt);
    void OnPositionLeftLabels(wxCommandEvent& evt);
    void OnPositionLeftIcons(wxCommandEvent& evt);
    void OnPositionLeftBoth(wxCommandEvent& evt);
    void OnPositionLeftDropdown(wxRibbonToolBarEvent& evt);
	

	//void OnGameDropdown(wxRibbonButtonBarEvent& evt);
	void OnNewGameDropDown(wxRibbonButtonBarEvent& evt);
	
	void OnGameDropdown(wxRibbonToolBarEvent& evt);
	void OnNewGame(wxCommandEvent& evt);
	void OnSetupPosition(wxCommandEvent& evt);
	void OnShowClock(wxCommandEvent& evt);

	void OnIdle(wxIdleEvent &event);

	// menu 1
	void OnLoadPGN(wxCommandEvent& evt);
	void OnLoadIndividualGame(wxCommandEvent &evt);
	void OnLoadGameListToActive(wxCommandEvent &evt);

	void OnAddPGN(wxCommandEvent& evt);
	void OnPastePGN(wxCommandEvent& evt);
	void OnLoadDB(wxCommandEvent& evt);
	void OnSaveDBInfo(wxCommandEvent& evt);
	// VM
	void OnLoadPGN_VM(wxCommandEvent& evt);
	void OnAddPGN_VM(wxCommandEvent& evt);
	void OnLoadDB_VM(wxCommandEvent& evt);

	void OnOpenDB_New(wxCommandEvent& evt);
	// VM
	void OnMergePGNFromClipboard(wxCommandEvent& evt);
	void OnMergePGNFromDisk(wxCommandEvent& evt);
	void OnCopyPGNToClipBoard(wxCommandEvent& evt);
	void OnCopyFENToClipBoard(wxCommandEvent& evt);
	void OnCopyVariationToClipBoard(wxCommandEvent& evt);
	void OnSavePGN(wxCommandEvent& evt);
	void OnSaveGameListAsDatabase(wxCommandEvent& evt);
	void OnPasteFEN(wxCommandEvent& evt);

	void OnLoadBook(wxCommandEvent& evt);
	void OnShowBookWindow(wxCommandEvent& evt);

	// menu clock
	void OnClockSetTimeControls(wxCommandEvent& evt);
	void OnClockReset(wxCommandEvent& evt);
	void OnClockSetTime(wxCommandEvent& evt);

	// Database Sort
	void OnSort(wxRibbonButtonBarEvent& evt);
	void RemoveEmptyGamesFromDB(wxCommandEvent& evt);
	void SortByMoves(wxCommandEvent& evt);
	void SortByNumber(wxCommandEvent& evt);
	void SortByWhiteName(wxCommandEvent& evt);
	void SortByBlackName(wxCommandEvent& evt);
	void SortByWhiteElo(wxCommandEvent& evt);
	void SortByBlackElo(wxCommandEvent& evt);
	void SortByECO(wxCommandEvent& evt);
	void SortByEvent(wxCommandEvent& evt);
	void SortBySite(wxCommandEvent& evt);
	void SortByDate(wxCommandEvent& evt);
	void SortByResult(wxCommandEvent& evt);

	void OnDatabaseFindName(wxCommandEvent& evt);

	void OnDatabaseFilterDate(wxCommandEvent& evt);
	void OnDatabaseFilterEvent(wxCommandEvent& evt);
	void OnDatabaseFilterSite(wxCommandEvent& evt);
	void OnDatabaseFilterElo(wxCommandEvent& evt);
	void OnDatabaseFilterEloWhite(wxCommandEvent& evt);
	void OnDatabaseFilterEloBlack(wxCommandEvent& evt);
	void OnDatabaseFilterEco(wxCommandEvent& evt);
	void OnDatabaseFilterNameWhite(wxCommandEvent& evt);
	void OnDatabaseFilterNameBlack(wxCommandEvent& evt);
	

protected:
    wxRibbonGallery* PopulateColoursPanel(wxWindow* panel, wxColour def,
        int gallery_id);
    void AddText(wxString msg);
    wxRibbonGalleryItem* AddColourToGallery(wxRibbonGallery *gallery,
        wxString name, wxMemoryDC& dc, wxColour* value = NULL);
    wxColour GetGalleryColour(wxRibbonGallery *gallery,
        wxRibbonGalleryItem* item, wxString* name);
    void ResetGalleryArtProviders();
    void SetArtProvider(wxRibbonArtProvider* prov);
    void SetBarStyle(long style);

    wxRibbonBar* m_ribbon;
    wxRibbonGallery* m_primary_gallery;
    wxRibbonGallery* m_secondary_gallery;
    wxTextCtrl* m_logwindow;
    wxColourData m_colour_data;
    wxColour m_default_primary;
    wxColour m_default_secondary;
    wxColour m_default_tertiary;
    wxMemoryDC m_bitmap_creation_dc;
		wxLCDWindow *mClock;
	ClockTimer *m_clockTimer;
	bool bFullScreen;

	MyFrame *subframe;
	GameClockPanel *gcp;

	

    DECLARE_EVENT_TABLE()
};



class TraverseGameTimer : public wxTimer
{
public:
    TraverseGameTimer(MyFrame* frame);
    void Notify();
    void start();

	void SetTimeToPause(int t)
	{
		timeToPause = t;
	}
	int timeToPause;

	MyFrame* m_frame;

};


class AutosaveGameTimer : public wxTimer
{
public:
    AutosaveGameTimer(MyFrame* frame);
    void Notify();
    void start();

	void SetTimeToPause(int t)
	{
		timeToPause = t;
	}

	void SetAutosavePath(string &s)
	{
		autosavePath = s;
	}

	void UseCurrentDate(bool b)
	{
		useCurrentDate = b;
	}

	int timeToPause;

	MyFrame* m_frame;
	string autosavePath;
	bool useCurrentDate;

};


class MyFrame : public wxMDIChildFrame
{
public:
    MyFrame(MDIParentFrame *parent);
    virtual ~MyFrame();

    static unsigned GetChildrenCount() { return ms_numChildren; }


private:
    void OnActivate(wxActivateEvent& event);

    void OnRefresh(wxCommandEvent& event);
    void OnUpdateRefresh(wxUpdateUIEvent& event);
    void OnChangeTitle(wxCommandEvent& event);
    void OnChangePosition(wxCommandEvent& event);
    void OnChangeSize(wxCommandEvent& event);
    void OnClose(wxCommandEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnMove(wxMoveEvent& event);
    void OnCloseWindow(wxCloseEvent& event);
	void OnIdle(wxIdleEvent& event);

#if wxUSE_CLIPBOARD
    void OnPaste(wxCommandEvent& event);
    void OnUpdatePaste(wxUpdateUIEvent& event);
#endif // wxUSE_CLIPBOARD

    static unsigned ms_numChildren;

    MyCanvas *m_canvas;

public:
	   enum
    {
        ID_CreateTree = wxID_HIGHEST+900,
        ID_CreateGrid,
        ID_CreateText,
        ID_CreateHTML,
		ID_LoadPGN,
		ID_ADD_PGN,
		ID_LoadPGNFromClipBoard,
		ID_MergePGNFromClipBoard,
		ID_MergePGNFromDisk,
		ID_CopyPGNToClipBoard,
		ID_CopyFENToClipBoard,
		ID_SavePGN,
		ID_SAVE_GAME_LIST_AS_DATABASE,
        ID_CreateNotebook,
        ID_CreateSizeReport,
        ID_GridContent,
        ID_TextContent,
        ID_TreeContent,
        ID_HTMLContent,
        ID_NotebookContent,
        ID_SizeReportContent,
        ID_CreatePerspective,
        ID_CopyPerspectiveCode,
        ID_AllowFloating,
        ID_AllowActivePane,
        ID_TransparentHint,
        ID_VenetianBlindsHint,
        ID_RectangleHint,
        ID_NoHint,
        ID_HintFade,
        ID_NoVenetianFade,
        ID_TransparentDrag,
        ID_NoGradient,
        ID_VerticalGradient,
        ID_HorizontalGradient,
        ID_LiveUpdate,
        ID_Settings,
        ID_CustomizeToolbar,
        ID_DropDownToolbarItem,
        ID_NotebookNoCloseButton,
        ID_NotebookCloseButton,
        ID_NotebookCloseButtonAll,
        ID_NotebookCloseButtonActive,
        ID_NotebookAllowTabMove,
        ID_NotebookAllowTabExternalMove,
        ID_NotebookAllowTabSplit,
        ID_NotebookWindowList,
        ID_NotebookScrollButtons,
        ID_NotebookTabFixedWidth,
        ID_NotebookArtGloss,
        ID_NotebookArtSimple,
        ID_NotebookAlignTop,
        ID_NotebookAlignBottom,
		
		WORKER_EVENT,
        ID_SampleItem,

        ID_FirstPerspective = ID_CreatePerspective+1000
    };



	wxProgressDialog *m_dlgProgress;

	bool listHasBeenSorted;
	int selectedDBItem;
	wxString perspective_all;

    wxAuiDockArt* GetDockArt();
    void DoUpdate();

	void DrawVariation(int &indent, PGN::Move* pMove);
	
//	Book* GetBook(){return objs.book;}


	// input stuff
	void OnKeyDown(wxKeyEvent& event) { LogEvent("KeyDown", event); }
    void OnKeyUp(wxKeyEvent& event) { /*LogEvent("KeyUp", event);*/ }
    void OnChar(wxKeyEvent& event) { LogEvent("Char", event); }
   // void OnPaintInputWin(wxPaintEvent& event);
	void OnToolLeftClick(wxCommandEvent& event);
	void mouseMoved(wxMouseEvent& event){}
	void mouseDown(wxMouseEvent& event){}
	void mouseWheelMoved(wxMouseEvent& event){}
	void mouseReleased(wxMouseEvent& event){}
	void rightClick(wxMouseEvent& event){}
	

    void LogEvent(const wxString& name, wxKeyEvent& event);
	void WriteClipboardData()
	{
		if (wxTheClipboard->Open())
		{
			if (wxTheClipboard->IsSupported( wxDF_UNICODETEXT ))
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
	}

private:
    wxTextCtrl* CreateTextCtrl(const wxString& text = wxEmptyString);
    wxGrid* CreateGrid();
    wxTreeCtrl* CreateTreeCtrl();
	BoardCanvas* CreateBoardCanvas();
    wxSizeReportCtrl* CreateSizeReportCtrl(int width = 80, int height = 80);
    wxPoint GetStartPosition();
    wxHtmlWindow* CreateHTMLCtrl(wxWindow* parent = NULL);
    wxAuiNotebook* CreateNotebook();

    wxString GetIntroText();



   
public:
	// pgn loading/saving
	void OpenDB(string filename);
	void OpenPGN(string filename,bool refreshList);
	void LoadPGN(bool refreshList);
	void OnLoadIndividualGame();
	void OnLoadGameListToActive();

	void OnLoadPGN_VM();
	void OnAddPGN_VM();
	void OnLoadDB_VM();

	void OpenDB_VM(string filename, bool refreshList);
	void OpenPGN_VM(string filename,bool refreshList);

	void OnOpenDB_New();

	void OnLoadPGN(wxCommandEvent& evt);
	void OnAddPGN(wxCommandEvent& evt);
	void OnSavePGN(wxCommandEvent& evt);
	void SaveGameToOutputStream(PGN::Game *gameToSave);
	void OnSaveGameListAsDatabase(wxCommandEvent& evt);
	void OnLoadPgnFromClipBoard(wxCommandEvent& evt);
	void OnMergePGNFromClipBoard(wxCommandEvent& evt);
	void OnMergePGNFromDisk(wxCommandEvent& evt);
	void OnCopyPGNToClipBoard(wxCommandEvent& evt);
	void CopyFenToClipBoard(wxCommandEvent& evt);
	void OnPasteFEN(wxCommandEvent& evt);
	void OnCopyVariationToClipBoard(wxCommandEvent& evt);
	void OnLoadDB(wxCommandEvent& evt);

	void OnInsertAnnotation(wxCommandEvent &evt);

	void NewGame(bool removeList);
	
	void OnNewGame(wxCommandEvent& evt);
	void OnCopyPerspectiveCode(wxCommandEvent& evt);

	void OnEraseElementFromDB();
	void StopAllAnimation();

	void OnMergeAll();
	void OnMergeSelected();
	void PlayThroughGame();
	void IncMove();
	void StopPlayThroughGame();
	void OnPlayThroughGameList(unsigned int currNum);

	bool GenerateFensCurrentGame();
    void OnRemoveGameFromDB();
    void OnAddGameToDB();
	void OnSaveDB();
	void OnExportDB();
	void OnCompressDB();
	void OnEditMode();

	void PlayerResign();
	void ShowResults();
	void AjournGame();

	void StartEnginesAfterMoveChange();
	void StopEnginesBeforeMoveChange();

	void KickStartEnginesAfterMoveChange();

	void OnLoadMedia(wxURI &uripath);

	void DoRefresh();

	void ApplySearchFilter();
	void ClearSearchFilter();
	void FindName();
	void ApplyFilterOnSubset();
	void PositionSearch();

	void SetPositionSearchData(ChessBoard* brd,bool pawns_only);
	void DoPositionSearch(  bool pawns_only, bool find_similar);
	

	void RemoveEmptyGamesFromDB();
	void SortByMoves();
	void SortByNumber();
	void SortByWhiteName();
	void SortByBlackName();
	void SortByWhiteElo();
	void SortByBlackElo();
	void SortByECO();
	void SortByEvent();
	void SortBySite();
	void SortByDate();
	void SortByResult();

	void OnDatabaseFilterDate();
	void OnDatabaseFilterEvent();
	void OnDatabaseFilterSite();
	void OnDatabaseFilterElo();
	void OnDatabaseFilterEloWhite();
	void OnDatabaseFilterEloBlack();
	void OnDatabaseFilterEco();
	void OnDatabaseFilterNameWhite();
	void OnDatabaseFilterNameBlack();
	void OnDatabaseShowFilters();


	void CopyGame();
	void OnCheckAll(); // game list
	void OnUnCheckAll();

	void DeleteGameAtIndex(int i);
	void DeleteAllSelectedGames();

	void OnMoveUp();
	void OnMoveDown();

	void SearchByName();


	bool IsAutoSaveActive();
	void ActivateAutosave();
	void DeactivateAutosave();

	void SetAutosaveFrequency(int timeInMinutes);

	bool IsAppReadyForAutosave();

	void ScheduleAutosave();

	void DoAutosave();


	// Threads
	void OnWorkerEvent(wxThreadEvent& event);

    //void OnSize(wxSizeEvent& evt);
private:
	 void OnEraseBackground(wxEraseEvent& evt);
    void OnCreateTree(wxCommandEvent& evt);
	//void OnCreatePipePanel(wxCommandEvent& evt);
    void OnCreateGrid(wxCommandEvent& evt);
    void OnCreateHTML(wxCommandEvent& evt);
    void OnCreateNotebook(wxCommandEvent& evt);
    void OnCreateText(wxCommandEvent& evt);
    void OnCreateSizeReport(wxCommandEvent& evt);
    void OnChangeContentPane(wxCommandEvent& evt);
    void OnDropDownToolbarItem(wxAuiToolBarEvent& evt);
    void OnCreatePerspective(wxCommandEvent& evt);
    
    void OnRestorePerspective(wxCommandEvent& evt);
    void OnSettings(wxCommandEvent& evt);
    void OnCustomizeToolbar(wxCommandEvent& evt);
    void OnAllowNotebookDnD(wxAuiNotebookEvent& evt);
    void OnNotebookPageClose(wxAuiNotebookEvent& evt);
    void OnNotebookPageClosed(wxAuiNotebookEvent& evt);
    void OnExit(wxCommandEvent& evt);
    void OnAbout(wxCommandEvent& evt);
    void OnTabAlignment(wxCommandEvent &evt);

    void OnGradient(wxCommandEvent& evt);
    void OnManagerFlag(wxCommandEvent& evt);
    void OnNotebookFlag(wxCommandEvent& evt);
    void OnUpdateUI(wxUpdateUIEvent& evt);

	

void OnPOpen(wxCommandEvent& WXUNUSED(event));

    void OnPaneClose(wxAuiManagerEvent& evt);

	void OnSwapSides();
	

	void OnToggleD3DMode();


	// listbox stuff

	// Create pipe panels
	
	//void InitWithIconItems(bool withText, bool sameIcon);
/*void onIdle(wxIdleEvent& evt)
{
    //if(render_loop_on)
    {
		 //wxClientDC dc(my_canvas);
    //render(dc);
		//wxPaintEvent event;
		//my_canvas->OnPaint(event);
       // evt.RequestMore(); // render continuously, not only once on idle
		my_canvas->Refresh(true);
    }
}
*/
public:
	// for the find dialog.


	vector<FindNameInfo> find_possibilities;

	bool AutosaveScheduled;

	ColorPallete m_moveColorPallete;
	long total_time_in_variation;
	bool profile_enabled;
	// internal class
	class FullAnalysisSettings
	{
	public:

		MyFrame *m_frame;
		enum
		{
			GO_DEPTH,
			GO_TIME,
			GO_DEPTH_AND_TIME,
			GO_DEPTH_OR_TIME,
		};

		int go_command;

		int timePerPlySeconds;

		int depthPerPly;


		bool saveDepthScores;

		bool createGraphs;

		wxString engine1;
		wxString engine2;
		wxString engine3;
		wxString engine4;

		MyPipePanel *pEngine[4];

		int numEnginesUsed;

		bool generateAlternativeLines;

		int numLines; // override with num Engines / give engine score for each

		bool findBlunders;

		bool findKillers;

		bool markForcedMoves;

		bool findZugzwangs;

		bool markBookMoves;

		bool classifyECO;

		wxString bookName;

		bool storeAll;

		bool insertTextAnalysis;

		bool writeToAnnotation;

		bool writeScores;

		bool writeDepth_and_time;

		bool writePV; // if not insert PV

		bool writeToLog;

		wxString LogFile;

		bool performQuickScan;
	    bool goDeeperIntoPromisingMoves;
		bool reAnalyzeUnclearMoves;
		bool clearHashInBetweenPhases;

		bool analyzeBookLines;
		int plyForBookLines;

		bool findForcedMoves;
		bool findUnclearMoves;
		bool findInnacuracies;

		bool colorCodeMatches;
		bool createTables;

		bool writeDepthToAnnotation;
		bool writeTimeToAnnotation;
		bool writePureLog;

		bool analyzeCurrentVariation;
		bool enterVariations;

		bool guiControlsTime;

		FullAnalysisSettings(MyFrame *frame)
		{
			m_frame = frame;

			Reset();
		}

		FullAnalysisSettings()
		{
			go_command = GO_TIME;
			timePerPlySeconds = 30;
			depthPerPly = 20;
			storeAll = false;
			saveDepthScores = false;
			createGraphs = false;

			engine1 = "";
			engine2 = "";
			engine3 = "";
			engine4 = "";

			pEngine[0] = NULL;
			pEngine[1] = NULL;
			pEngine[2] = NULL;
			pEngine[3] = NULL;
			numEnginesUsed = 1;
			generateAlternativeLines = false;
			numLines = 1; // override with num Engines / give engine score for each
			findBlunders = false;
			findKillers = false;
			markForcedMoves = false;
			findZugzwangs = false;
			markBookMoves = false;
			classifyECO = false;
			bookName = "";
			storeAll = false;
			insertTextAnalysis = false;
			writeToAnnotation = false;
			writeScores = false;
			writeDepth_and_time = false;
			writePV = false; // if not insert PV
			writeToLog = false;
			LogFile = "";
			performQuickScan = false;
			goDeeperIntoPromisingMoves = false;
			reAnalyzeUnclearMoves = false;
			clearHashInBetweenPhases = false;
			analyzeBookLines = false;
			plyForBookLines = 0;
			findForcedMoves = false;
			findInnacuracies = false;
			findUnclearMoves = false;
			colorCodeMatches = false;
			createTables = false;
			writeDepthToAnnotation = false;
			writeTimeToAnnotation = false;
			//insertTextAnalysis = false;
			writePureLog = false;
			analyzeCurrentVariation = false;
			enterVariations = false;
			guiControlsTime = false;
		}

		~FullAnalysisSettings(){}


		void Reset()
		{
			go_command = GO_TIME;
			timePerPlySeconds = 30;
			depthPerPly = 20;
			storeAll = false;

			saveDepthScores = false;
			createGraphs = false;

			engine1 = "";
			engine2 = "";
			engine3 = "";
			engine4 = "";

			pEngine[0] = NULL;
			pEngine[1] = NULL;
			pEngine[2] = NULL;
			pEngine[3] = NULL;
			numEnginesUsed = 1;
			generateAlternativeLines = false;
			numLines = 1; // override with num Engines / give engine score for each
			findBlunders = false;
			findKillers = false;
			markForcedMoves = false;
			findZugzwangs = false;
			markBookMoves = false;
			classifyECO = false;
			bookName = "";
			storeAll = false;
			insertTextAnalysis = false;
			writeToAnnotation = false;
			writeScores = false;
			writeDepth_and_time = false;
			writePV = false; // if not insert PV
			writeToLog = false;
			LogFile = "";
			performQuickScan = false;
			goDeeperIntoPromisingMoves = false;
			reAnalyzeUnclearMoves = false;
			clearHashInBetweenPhases = false;
			analyzeBookLines = false;
			plyForBookLines = 0;
			findForcedMoves = false;
			findInnacuracies = false;
			findUnclearMoves = false;
			colorCodeMatches = false;
			createTables = false;
			writeDepthToAnnotation = false;
			writeTimeToAnnotation = false;
			//insertTextAnalysis = false;
			writePureLog = false;
			analyzeCurrentVariation = false;
			enterVariations = false;
			guiControlsTime = false;
		}
	};

	FullAnalysisSettings m_fullAnalysisSettings;

	void OnFinishFullAnalysisOfVariation();

	void OnCreatePipePanel(wxCommandEvent& evt);



	int GetNumActivePipePanels()
	{
		return numActivePipePanels;
	}


	void DoSize();

	void RecreateGameList();
	void RecreateList(long flags, bool withText=true);

	void CreateFenForVariation(PGN::Move* pMove);
	void CreateFenForVariationWhite(PGN::Move* pMove, string fen_last);
	void CreateFenForVariationBlack(PGN::Move* pMove, string fen_last);

	void CreateVariationSelectionBox(int white_or_black);
	
	void UpdateAnnotationWindow();

	// accessor stuff
	void SetCurrentGame(int i);

	void SelectMove(int sel);
	void SelectMoveFromTree(int sel);

	void SelectMoveFromTreeAutoTraversal(int sel);


	void OnFullAnalysis();
	void DoStepFullAnalysis();
	/*
			ID_EDIT_PGN_HEADER,
		ID_EDIT_ANNOTATION,
		ID_DELETE_REMAINING_MOVES,
		ID_DELETE_VARIATION,
		ID_PROMOTE_VARIATION,
    void OnEditPGNHeader(wxRibbonButtonBarEvent& evt);
    void OnEditAnnotation(wxRibbonButtonBarEvent& evt);
    void OnDeleteRemainingMoves(wxRibbonButtonBarEvent& evt);
	void OnDeleteVariation(wxRibbonButtonBarEvent& evt);
	void OnPromoteVariation(wxRibbonButtonBarEvent& evt);
	*/

	void EditPGNHeader();
	void EditAnnotation();
	void InsertAnnotationData(string &str);
	void DeleteRemainingMoves();
	void DeleteVariation();
	void PromoteVariation();

	void ClassifyECO();

   void OnSetFocus(wxFocusEvent &evt);


   void LoadECOClassifier();

   void ResetPlayThroughData()
   {
	   playThroughGameList = true;
	   currentPlayGameNumber = 0;
	   currentPlayGamePly = 0;
   }

   void SaveLinesInFullAnalysis();

   bool saveLinesInFullAnalysisMode;

   bool fullAnalysisMode;

   bool playThroughGameList;
   unsigned int currentPlayGameNumber;
   int currentPlayGamePly;

   MDIParentFrame *m_parent;

	wxLCDWindow *mClock;
	ClockTimer *m_clockTimer;

    wxPanel *m_panel;
	wxPanel *m_panelFolderList;
	wxTextCtrl *m_logWindow;

	MyListPanel *m_listPanel;
	
	MyListPanel *m_folderList;
   
	wxLog *m_logOld;
	wxTextCtrl* wnd10; // for testing
	wxTextDataObject data; //clipboard
	MyPipePanel *m_pipePanel[MAX_NUM_PIPE_PANELS];
	MyPipePanel *playersOpponent;
	int numActivePipePanels;

	TreeCanvas *treeCanvas;
	OptionsFrame *optionsFrame;
	GameClockPanel *gameClock;


	VariationBoardFrame *m_variationBoardFrame;
	//MyPipePanel *m_pVariationSelection;

	bool engineVsHumanMatch;

	class EngineVsEngine
	{
	public:
		enum TIME_CONTROL
		{
			TIMED,
			GO_DEPTH,
			GO_TIME,
		};

		enum TIME_CONTROL_TIMED_ID
		{
			HYPER_BULLET,
			BULLET,
			BLITZ,
			RAPID,
			STANDARD,
			LONG_TIME,
		};

		EngineVsEngine()
		{
			eventName = "Engine Match";
			timeControlType = TIME_CONTROL::TIMED;
			timeControlID = TIME_CONTROL_TIMED_ID::BULLET;
			engine1Name = "";
			engine2Name = "";
			numGames = 1;
			numGamesPlayed = 0;
			swapSidesBetweenGames = false;
			playSamePositionForEachColor = false;
			logEngineOutput = false;
			timeInMilliSecs = 0;
			timeInSeconds = 0;
			timeInMinutes = 0;
			incrementMillis = 0;
			incrementSeconds = 0;
			incrementMinutes = 0;
			plyForBook = 10;
			fixedDepth = 12;
			writeAllToSameGame = true;
			currOpeningNumber = 0;	
		}

		~EngineVsEngine()
		{
			delete m_MCTS;
		}
		MyPipePanel* engine01;
		MyPipePanel* engine02;

		bool writeAllToSameGame;
		int plyForBook;
		int timeControlType;
		int timeControlID;
		wxString engine1Name;
		wxString engine2Name;
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

		int fixedDepth;
		string eventName;
	
		int numGamesPlayed;
		vector<PGN::Game*> list_engine_games;

		vector<PGN::Game* > openings;
		int currOpeningNumber;
		void Setup();
		
		MCTS_Analysis *m_MCTS;
	};

	void StopMatch();
	void NextEngineVsEngineGame();

	EngineVsEngine m_engineVsEngine;

	bool enginesMatch;

	int gameMode;

    // number of items to initialize list/report view with

	class EngineTournament
	{
	public:

		struct Pairing
		{
			int A;
			int B;
		};

		vector <Pairing> pairings;
		//vector <MyPipePanel*> engines;
		vector<string> engine_names;
		vector<string> loadedEngines;

		/*
		void Tournament()
		{
			// construct initial pairings
			8 initial engines, pair off sequence
			// construct diagram for the tournament
			// show message, shutting down current engines
			for each pairing
			index the vector and load the engines
			run the game
			on termination, shutdown the engines, load next pairing and repeat
		}
		*/
	};

	enum PGN_VIEW_MODE
	{
		FILE_IN_RAM,
		FILE_IN_VIRTUAL_MEMORY,
		DATABASE,
	};

	int pgnViewMode;

	enum GRAPHICS_MODE
	{
		DC_GRAPHICS,
		GC_GRAPHICS,
		D3D_GRAPHICS,
		OPENGL_GRAPHICS
	};

	int m_graphicsModeSetting;

    wxAuiManager m_mgr;
    wxArrayString m_perspectives;
    wxMenu* m_perspectives_menu;
    long m_notebook_style;
    long m_notebook_theme;


	

	//program specific
	wxHtmlWindow* html_ctrl;
	PGN::PgnLoad pgnLoad;
	PGN::PgnLoad ecoClassify;
	wxString currentECO;
	PGN::Game *currentGame;
	wxRichTextCtrl *m_richTextCtrl;// = new wxRichTextCtrl(splitter, ID_RICHTEXT_CTRL, wxEmptyString, wxDefaultPosition, wxSize(200, 200), wxVSCROLL|wxHSCROLL|wxNO_BORDER|wxWANTS_CHARS);
	MyCanvas *my_canvas;
	BaseGraphicsBoard* board_canvas;
	//RenderTimer *renderTimer;
	wxGLSizeReportCtrl* ctrl;
	wxBitmap bmap; 
	VariationSelectionFrame *variationSelectionBox;
	NotationFrame *noteFrame;
	MoveDataCanvas *moveDataCanvas;

	TreeStatsCanvas *tsc;
	 
	MatchResultsPanel *match_results;

	GameDataViewPanel *m_dataView;
	EngineManager *m_engineManager;

	GameAnalyser* m_analyser;

	wxMediaCtrl* m_mediactrl;   //Our media control

	ScoreGraph *m_scoreGraph;
	ScorePanel *m_scoreGraphPanel;

	FILE* polyglotBook;
	string polyGlotBookName;

	entry_t entries[100];
	wxString entryText[100];
	int bookLookupCount;

	void DoLookupFromBook();

	TraverseGameTimer* m_gameTraverseTimer;
	AutosaveGameTimer* m_autoSaveTimer;
private:



public:

	bool Cancelled()
	{
		wxCriticalSectionLocker lock(m_csCancelled);

		return m_cancelled;
	}

    // simple test event handler class
    class EventHandler : public wxEvtHandler
    {
    public:
        EventHandler(unsigned numChild) : m_numChild(numChild) { }

    private:
        void OnRefresh(wxCommandEvent& event)
        {
            wxLogMessage("Child #%u refreshed.", m_numChild);
            event.Skip();
        }

        const unsigned m_numChild;

        DECLARE_EVENT_TABLE()

        wxDECLARE_NO_COPY_CLASS(EventHandler);
    };


    // was the worker thread cancelled by user?
    bool m_cancelled;
    wxCriticalSection m_csCancelled;        // protects m_cancelled

    DECLARE_EVENT_TABLE()
};



// -- wxSizeReportCtrl --
// (a utility control that always reports it's client size)

class wxSizeReportCtrl : public wxControl
{
public:

    wxSizeReportCtrl(wxWindow* parent, wxWindowID id = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     wxAuiManager* mgr = NULL)
                     : wxControl(parent, id, pos, size, wxNO_BORDER)
    {
        m_mgr = mgr;
    }

private:

    void OnPaint(wxPaintEvent& WXUNUSED(evt))
    {
        wxPaintDC dc(this);
        wxSize size = GetClientSize();
        wxString s;
        int h, w, height;

        s.Printf(wxT("Size: %d x %d"), size.x, size.y);

        dc.SetFont(*wxNORMAL_FONT);
        dc.GetTextExtent(s, &w, &height);
        height += 3;
        dc.SetBrush(*wxWHITE_BRUSH);
        dc.SetPen(*wxWHITE_PEN);
        dc.DrawRectangle(0, 0, size.x, size.y);
        dc.SetPen(*wxLIGHT_GREY_PEN);
        dc.DrawLine(0, 0, size.x, size.y);
        dc.DrawLine(0, size.y, size.x, 0);
      //  dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2));

        if (m_mgr)
        {
            wxAuiPaneInfo pi = m_mgr->GetPane(this);

            s.Printf(wxT("Layer: %d"), pi.dock_layer);
            dc.GetTextExtent(s, &w, &h);
        //    dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*1));

            s.Printf(wxT("Dock: %d Row: %d"), pi.dock_direction, pi.dock_row);
            dc.GetTextExtent(s, &w, &h);
        //    dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*2));

            s.Printf(wxT("Position: %d"), pi.dock_pos);
            dc.GetTextExtent(s, &w, &h);
        //    dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*3));

            s.Printf(wxT("Proportion: %d"), pi.dock_proportion);
            dc.GetTextExtent(s, &w, &h);
        //    dc.DrawText(s, (size.x-w)/2, ((size.y-(height*5))/2)+(height*4));
        }
    }

    void OnEraseBackground(wxEraseEvent& WXUNUSED(evt))
    {
        // intentionally empty
    }

    void OnSize(wxSizeEvent& WXUNUSED(evt))
    {
        Refresh();
    }
private:

    wxAuiManager* m_mgr;

    DECLARE_EVENT_TABLE()
};




class SettingsPanel : public wxPanel
{
    enum
    {
        ID_PaneBorderSize = wxID_HIGHEST+1900,
        ID_SashSize,
        ID_CaptionSize,
        ID_BackgroundColor,
        ID_SashColor,
        ID_InactiveCaptionColor,
        ID_InactiveCaptionGradientColor,
        ID_InactiveCaptionTextColor,
        ID_ActiveCaptionColor,
        ID_ActiveCaptionGradientColor,
        ID_ActiveCaptionTextColor,
        ID_BorderColor,
        ID_GripperColor
    };

public:

    SettingsPanel(wxWindow* parent, MyFrame* frame)
            : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize),
              m_frame(frame)
    {
        //wxBoxSizer* vert = new wxBoxSizer(wxVERTICAL);

        //vert->Add(1, 1, 1, wxEXPAND);

        wxBoxSizer* s1 = new wxBoxSizer(wxHORIZONTAL);
        m_border_size = new wxSpinCtrl(this, ID_PaneBorderSize, wxString::Format(wxT("%d"), frame->GetDockArt()->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE)), wxDefaultPosition, wxSize(50,20), wxSP_ARROW_KEYS, 0, 100, frame->GetDockArt()->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE));
        s1->Add(1, 1, 1, wxEXPAND);
        s1->Add(new wxStaticText(this, wxID_ANY, wxT("Pane Border Size:")));
        s1->Add(m_border_size);
        s1->Add(1, 1, 1, wxEXPAND);
        s1->SetItemMinSize((size_t)1, 180, 20);
        //vert->Add(s1, 0, wxEXPAND | wxLEFT | wxBOTTOM, 5);

        wxBoxSizer* s2 = new wxBoxSizer(wxHORIZONTAL);
        m_sash_size = new wxSpinCtrl(this, ID_SashSize, wxString::Format(wxT("%d"), frame->GetDockArt()->GetMetric(wxAUI_DOCKART_SASH_SIZE)), wxDefaultPosition, wxSize(50,20), wxSP_ARROW_KEYS, 0, 100, frame->GetDockArt()->GetMetric(wxAUI_DOCKART_SASH_SIZE));
        s2->Add(1, 1, 1, wxEXPAND);
        s2->Add(new wxStaticText(this, wxID_ANY, wxT("Sash Size:")));
        s2->Add(m_sash_size);
        s2->Add(1, 1, 1, wxEXPAND);
        s2->SetItemMinSize((size_t)1, 180, 20);
        //vert->Add(s2, 0, wxEXPAND | wxLEFT | wxBOTTOM, 5);

        wxBoxSizer* s3 = new wxBoxSizer(wxHORIZONTAL);
        m_caption_size = new wxSpinCtrl(this, ID_CaptionSize, wxString::Format(wxT("%d"), frame->GetDockArt()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE)), wxDefaultPosition, wxSize(50,20), wxSP_ARROW_KEYS, 0, 100, frame->GetDockArt()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE));
        s3->Add(1, 1, 1, wxEXPAND);
        s3->Add(new wxStaticText(this, wxID_ANY, wxT("Caption Size:")));
        s3->Add(m_caption_size);
        s3->Add(1, 1, 1, wxEXPAND);
        s3->SetItemMinSize((size_t)1, 180, 20);
        //vert->Add(s3, 0, wxEXPAND | wxLEFT | wxBOTTOM, 5);

        //vert->Add(1, 1, 1, wxEXPAND);


        wxBitmap b = CreateColorBitmap(*wxBLACK);

        wxBoxSizer* s4 = new wxBoxSizer(wxHORIZONTAL);
        m_background_color = new wxBitmapButton(this, ID_BackgroundColor, b, wxDefaultPosition, wxSize(50,25));
        s4->Add(1, 1, 1, wxEXPAND);
        s4->Add(new wxStaticText(this, wxID_ANY, wxT("Background Color:")));
        s4->Add(m_background_color);
        s4->Add(1, 1, 1, wxEXPAND);
        s4->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s5 = new wxBoxSizer(wxHORIZONTAL);
        m_sash_color = new wxBitmapButton(this, ID_SashColor, b, wxDefaultPosition, wxSize(50,25));
        s5->Add(1, 1, 1, wxEXPAND);
        s5->Add(new wxStaticText(this, wxID_ANY, wxT("Sash Color:")));
        s5->Add(m_sash_color);
        s5->Add(1, 1, 1, wxEXPAND);
        s5->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s6 = new wxBoxSizer(wxHORIZONTAL);
        m_inactive_caption_color = new wxBitmapButton(this, ID_InactiveCaptionColor, b, wxDefaultPosition, wxSize(50,25));
        s6->Add(1, 1, 1, wxEXPAND);
        s6->Add(new wxStaticText(this, wxID_ANY, wxT("Normal Caption:")));
        s6->Add(m_inactive_caption_color);
        s6->Add(1, 1, 1, wxEXPAND);
        s6->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s7 = new wxBoxSizer(wxHORIZONTAL);
        m_inactive_caption_gradient_color = new wxBitmapButton(this, ID_InactiveCaptionGradientColor, b, wxDefaultPosition, wxSize(50,25));
        s7->Add(1, 1, 1, wxEXPAND);
        s7->Add(new wxStaticText(this, wxID_ANY, wxT("Normal Caption Gradient:")));
        s7->Add(m_inactive_caption_gradient_color);
        s7->Add(1, 1, 1, wxEXPAND);
        s7->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s8 = new wxBoxSizer(wxHORIZONTAL);
        m_inactive_caption_text_color = new wxBitmapButton(this, ID_InactiveCaptionTextColor, b, wxDefaultPosition, wxSize(50,25));
        s8->Add(1, 1, 1, wxEXPAND);
        s8->Add(new wxStaticText(this, wxID_ANY, wxT("Normal Caption Text:")));
        s8->Add(m_inactive_caption_text_color);
        s8->Add(1, 1, 1, wxEXPAND);
        s8->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s9 = new wxBoxSizer(wxHORIZONTAL);
        m_active_caption_color = new wxBitmapButton(this, ID_ActiveCaptionColor, b, wxDefaultPosition, wxSize(50,25));
        s9->Add(1, 1, 1, wxEXPAND);
        s9->Add(new wxStaticText(this, wxID_ANY, wxT("Active Caption:")));
        s9->Add(m_active_caption_color);
        s9->Add(1, 1, 1, wxEXPAND);
        s9->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s10 = new wxBoxSizer(wxHORIZONTAL);
        m_active_caption_gradient_color = new wxBitmapButton(this, ID_ActiveCaptionGradientColor, b, wxDefaultPosition, wxSize(50,25));
        s10->Add(1, 1, 1, wxEXPAND);
        s10->Add(new wxStaticText(this, wxID_ANY, wxT("Active Caption Gradient:")));
        s10->Add(m_active_caption_gradient_color);
        s10->Add(1, 1, 1, wxEXPAND);
        s10->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s11 = new wxBoxSizer(wxHORIZONTAL);
        m_active_caption_text_color = new wxBitmapButton(this, ID_ActiveCaptionTextColor, b, wxDefaultPosition, wxSize(50,25));
        s11->Add(1, 1, 1, wxEXPAND);
        s11->Add(new wxStaticText(this, wxID_ANY, wxT("Active Caption Text:")));
        s11->Add(m_active_caption_text_color);
        s11->Add(1, 1, 1, wxEXPAND);
        s11->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s12 = new wxBoxSizer(wxHORIZONTAL);
        m_border_color = new wxBitmapButton(this, ID_BorderColor, b, wxDefaultPosition, wxSize(50,25));
        s12->Add(1, 1, 1, wxEXPAND);
        s12->Add(new wxStaticText(this, wxID_ANY, wxT("Border Color:")));
        s12->Add(m_border_color);
        s12->Add(1, 1, 1, wxEXPAND);
        s12->SetItemMinSize((size_t)1, 180, 20);

        wxBoxSizer* s13 = new wxBoxSizer(wxHORIZONTAL);
        m_gripper_color = new wxBitmapButton(this, ID_GripperColor, b, wxDefaultPosition, wxSize(50,25));
        s13->Add(1, 1, 1, wxEXPAND);
        s13->Add(new wxStaticText(this, wxID_ANY, wxT("Gripper Color:")));
        s13->Add(m_gripper_color);
        s13->Add(1, 1, 1, wxEXPAND);
        s13->SetItemMinSize((size_t)1, 180, 20);

        wxGridSizer* grid_sizer = new wxGridSizer(2);
        grid_sizer->SetHGap(5);
        grid_sizer->Add(s1);  grid_sizer->Add(s4);
        grid_sizer->Add(s2);  grid_sizer->Add(s5);
        grid_sizer->Add(s3);  grid_sizer->Add(s13);
        grid_sizer->Add(1,1); grid_sizer->Add(s12);
        grid_sizer->Add(s6);  grid_sizer->Add(s9);
        grid_sizer->Add(s7);  grid_sizer->Add(s10);
        grid_sizer->Add(s8);  grid_sizer->Add(s11);

        wxBoxSizer* cont_sizer = new wxBoxSizer(wxVERTICAL);
        cont_sizer->Add(grid_sizer, 1, wxEXPAND | wxALL, 5);
        SetSizer(cont_sizer);
        GetSizer()->SetSizeHints(this);

        m_border_size->SetValue(frame->GetDockArt()->GetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE));
        m_sash_size->SetValue(frame->GetDockArt()->GetMetric(wxAUI_DOCKART_SASH_SIZE));
        m_caption_size->SetValue(frame->GetDockArt()->GetMetric(wxAUI_DOCKART_CAPTION_SIZE));

        UpdateColors();
    }

private:

    wxBitmap CreateColorBitmap(const wxColour& c)
    {
        wxImage image;
        image.Create(25,14);
        for (int x = 0; x < 25; ++x)
            for (int y = 0; y < 14; ++y)
            {
                wxColour pixcol = c;
                if (x == 0 || x == 24 || y == 0 || y == 13)
                    pixcol = *wxBLACK;
                image.SetRGB(x, y, pixcol.Red(), pixcol.Green(), pixcol.Blue());
            }
        return wxBitmap(image);
    }

    void UpdateColors()
    {
        wxColour bk = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_BACKGROUND_COLOUR);
        m_background_color->SetBitmapLabel(CreateColorBitmap(bk));

        wxColour cap = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_INACTIVE_CAPTION_COLOUR);
        m_inactive_caption_color->SetBitmapLabel(CreateColorBitmap(cap));

        wxColour capgrad = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_INACTIVE_CAPTION_GRADIENT_COLOUR);
        m_inactive_caption_gradient_color->SetBitmapLabel(CreateColorBitmap(capgrad));

        wxColour captxt = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_INACTIVE_CAPTION_TEXT_COLOUR);
        m_inactive_caption_text_color->SetBitmapLabel(CreateColorBitmap(captxt));

        wxColour acap = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_ACTIVE_CAPTION_COLOUR);
        m_active_caption_color->SetBitmapLabel(CreateColorBitmap(acap));

        wxColour acapgrad = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_ACTIVE_CAPTION_GRADIENT_COLOUR);
        m_active_caption_gradient_color->SetBitmapLabel(CreateColorBitmap(acapgrad));

        wxColour acaptxt = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_ACTIVE_CAPTION_TEXT_COLOUR);
        m_active_caption_text_color->SetBitmapLabel(CreateColorBitmap(acaptxt));

        wxColour sash = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_SASH_COLOUR);
        m_sash_color->SetBitmapLabel(CreateColorBitmap(sash));

        wxColour border = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_BORDER_COLOUR);
        m_border_color->SetBitmapLabel(CreateColorBitmap(border));

        wxColour gripper = m_frame->GetDockArt()->GetColor(wxAUI_DOCKART_GRIPPER_COLOUR);
        m_gripper_color->SetBitmapLabel(CreateColorBitmap(gripper));
    }

    void OnPaneBorderSize(wxSpinEvent& event)
    {
        m_frame->GetDockArt()->SetMetric(wxAUI_DOCKART_PANE_BORDER_SIZE,
                                         event.GetPosition());
        m_frame->DoUpdate();
    }

    void OnSashSize(wxSpinEvent& event)
    {
        m_frame->GetDockArt()->SetMetric(wxAUI_DOCKART_SASH_SIZE,
                                         event.GetPosition());
        m_frame->DoUpdate();
    }

    void OnCaptionSize(wxSpinEvent& event)
    {
        m_frame->GetDockArt()->SetMetric(wxAUI_DOCKART_CAPTION_SIZE,
                                         event.GetPosition());
        m_frame->DoUpdate();
    }

    void OnSetColor(wxCommandEvent& event)
    {
        wxColourDialog dlg(m_frame);
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
        UpdateColors();
    }

private:

    MyFrame* m_frame;
    wxSpinCtrl* m_border_size;
    wxSpinCtrl* m_sash_size;
    wxSpinCtrl* m_caption_size;
    wxBitmapButton* m_inactive_caption_text_color;
    wxBitmapButton* m_inactive_caption_gradient_color;
    wxBitmapButton* m_inactive_caption_color;
    wxBitmapButton* m_active_caption_text_color;
    wxBitmapButton* m_active_caption_gradient_color;
    wxBitmapButton* m_active_caption_color;
    wxBitmapButton* m_sash_color;
    wxBitmapButton* m_background_color;
    wxBitmapButton* m_border_color;
    wxBitmapButton* m_gripper_color;

    DECLARE_EVENT_TABLE()
};




// menu items ids
enum
{
    MDI_FULLSCREEN = 100,
    MDI_REFRESH,
    MDI_CHANGE_TITLE,
    MDI_CHANGE_POSITION,
    MDI_CHANGE_SIZE
};


#endif