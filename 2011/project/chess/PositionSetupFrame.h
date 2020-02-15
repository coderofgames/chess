#ifndef POSITION_SETUP_FRAME_H
#define POSITION_SETUP_FRAME_H

#include "headers.h"

#include "MiniBoardCanvas.h"

enum
{
	ID_CLEAR_BOARD = wxID_HIGHEST + 300,
	ID_START_POS,
	ID_PASTE_FEN,
	ID_COPY_FEN,
	ID_RADIO_WHITE,
	ID_RADIO_BLACK,
	ID_WHITE_QCASTLE,
	ID_WHITE_KCASTLE,
	ID_BLACK_QCASTLE,
	ID_BLACK_KCASTLE,
	ID_FLIP_BOARD,
	ID_FIND_SIMILAR,
	ID_FIND_PAWN_STRUCTURE,
};
// ----------------------------------------------------------------------------
// MyPipeFrame: allows the user to communicate with the child process
// ----------------------------------------------------------------------------

class PositionSetupFrame : public wxFrame
{
public:
    PositionSetupFrame(MyFrame *parent, bool search_mode);
	~PositionSetupFrame();

protected:

	void OnClearBoard(wxCommandEvent& WXUNUSED(event));
	void OnStartPosition(wxCommandEvent& WXUNUSED(event));
	void OnPasteFEN(wxCommandEvent& WXUNUSED(event));
	void OnCopyFEN(wxCommandEvent& WXUNUSED(event));
	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& WXUNUSED(event));
	void OnFlipBoard(wxCommandEvent& WXUNUSED(event));
	void OnCastleCheck(wxCommandEvent& event);

	void OnFindPawnStructure(wxCommandEvent& event);
	void OnFindSimilar(wxCommandEvent& event);

	void OnSize(wxSizeEvent &event);
	
	void OnRadio( wxCommandEvent &event );
    //void OnTextEnter(wxCommandEvent& WXUNUSED(event)) { DoSend(); }
   /* void OnBtnSend(wxCommandEvent& WXUNUSED(event));
    void OnBtnSendFile(wxCommandEvent& WXUNUSED(event));
    void OnBtnGet(wxCommandEvent& WXUNUSED(event));
    void OnBtnClose(wxCommandEvent& WXUNUSED(event));

    void OnClose(wxCloseEvent& event);


    void DoClose();


	void OnKeyDown(wxKeyEvent& event);
	void LogEvent(const wxString& name, wxKeyEvent& event);
*/

	MyFrame *pParent;
	MiniBoardCanvas* m_miniBoard;

	int whoseMove;
	bool whiteCanCastleQueenSide;
	bool whiteCanCastleKingSide;
	bool blackCanCastleQueenSide;
	bool blackCanCastleKingSide;

	
	bool m_search_mode;
	bool pawns_only;
	bool find_similar;
  /*  wxTextCtrl *m_textOut,
               *m_textIn,
               *m_textErr;
			   */
public:
	 //wxListBox *m_lbox;
	 //bool m_bListsWhiteMoves;
private:
    DECLARE_EVENT_TABLE()
};

#endif