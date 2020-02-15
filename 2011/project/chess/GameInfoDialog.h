#ifndef GAME_INFO_DIALOG
#define GAME_INFO_DIALOG

#include "headers.h"

enum
{
	HEADER_LIST = wxID_HIGHEST +210,
	EDIT_LIST,
	ADD_TO_LIST,
	DELETE_ELEM,

};

class GameInfoModalDialog : public wxDialog
{
public:
	GameInfoModalDialog(MyFrame* pFrame, PGN::Game* game, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos = wxDefaultPosition,
	              const wxSize & size = wxSize(300,350),
	              long style = wxDEFAULT_DIALOG_STYLE );
	~GameInfoModalDialog();


	void OnOk( wxCommandEvent & event );
	void OnEditElem( wxCommandEvent & event );
	void OnAddElem( wxCommandEvent & event );
	void OnDeleteElem( wxCommandEvent &event );
	void OnCancel( wxCommandEvent & event );


	MyFrame* m_pFrame;
	PGN::Game* pGame;
	wxListBox *m_list;

	bool edited;

	DECLARE_EVENT_TABLE()
};

#endif