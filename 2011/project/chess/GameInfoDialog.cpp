#include "GameInfoDialog.h"
#include "mdi.h"
#include "PgnLoad.h"
#include "MiscUtils.h"
#include "NotationFrame.h"
#include "SetDateDialog.h"

BEGIN_EVENT_TABLE(GameInfoModalDialog, wxDialog)
	EVT_BUTTON(wxID_OK, GameInfoModalDialog::OnOk)
	//EVT_BUTTON(wxID_CANCEL, GameInfoModalDialog::OnCancel)
	EVT_BUTTON(EDIT_LIST, GameInfoModalDialog::OnEditElem)
	EVT_BUTTON(ADD_TO_LIST, GameInfoModalDialog::OnEditElem)
	EVT_BUTTON(DELETE_ELEM, GameInfoModalDialog::OnEditElem)

END_EVENT_TABLE()


GameInfoModalDialog::GameInfoModalDialog(MyFrame* pFrame, PGN::Game* game, wxWindow * parent, wxWindowID id, const wxString & title,
	              const wxPoint & pos , const wxSize & size , long style ) 
	: wxDialog(parent, id, title, pos, size, style)
{


	m_pFrame = pFrame;
	pGame = game;



	wxButton *WxButton5 = new wxButton(this, wxID_CANCEL, wxT("Cancel"), wxPoint(120, 238), wxSize(85, 32), 0, wxDefaultValidator, wxT("WxButton5"));

	wxButton *WxButton4 = new wxButton(this, wxID_OK, wxT("Ok"), wxPoint(19, 238), wxSize(85, 32), 0, wxDefaultValidator, wxT("WxButton4"));

	wxButton *WxButton3 = new wxButton(this, DELETE_ELEM, wxT("Delete"), wxPoint(187, 177), wxSize(64, 28), 0, wxDefaultValidator, wxT("WxButton3"));

	wxButton *WxButton2 = new wxButton(this, EDIT_LIST, wxT("Edit"), wxPoint(104, 177), wxSize(64, 28), 0, wxDefaultValidator, wxT("WxButton2"));

	wxButton *WxButton1 = new wxButton(this, ADD_TO_LIST, wxT("Add"), wxPoint(17, 177), wxSize(64, 28), 0, wxDefaultValidator, wxT("WxButton1"));

	wxArrayString arrayStringFor_WxListBox1;
	m_list = new wxListBox(this, HEADER_LIST, wxPoint(16, 17), wxSize(236, 142), arrayStringFor_WxListBox1, wxLB_SINGLE);
	wxFont fontRich(8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD);



	m_list->SetFont(fontRich);

	//PGN::Game* pGame = m_pFrame->currentGame;
	for( int i = 0; i < pGame->vec_header.size(); i++ )
	{
		m_list->Append(pGame->vec_header[i].data.c_str());
	}

	SetTitle(wxT("Edit PGN Header"));
	SetIcon(wxNullIcon);
	SetSize(8,8,289,320);
	Center();

	edited= false;
	
	//p.x += 110;
	//wxButton * c = new wxButton( this, wxID_CANCEL, _("Cancel"), p, wxDefaultSize );
}

GameInfoModalDialog::~GameInfoModalDialog()
{
}

void GameInfoModalDialog::OnOk( wxCommandEvent & event )
{
	//PGN::Game* pGame = m_pFrame->currentGame;
	pGame->vec_header.clear();

	for( int i = 0; i < m_list->GetCount(); i++ )
	{
		PGN::HeaderElem h;
		h.data = m_list->GetString(i);
		pGame->vec_header.push_back(h);
	}
	pGame->ParseHeader();
	m_pFrame->noteFrame->Refresh(true);
	Close(true);
}



void GameInfoModalDialog::OnEditElem( wxCommandEvent & event )
{
	wxString s = m_list->GetStringSelection();

	wxString toSet = GetSubstringInQuotes(s);
	int npos = s.Find(" ");
	wxString type = s.substr(1,npos+1); 
	wxString message = wxT("Enter Item for: ");
	message += type;

	if( s.find( "Date" )!= -1 )
	{
		// handle the date
		SetDateDialog dialog(this, wxID_ANY, "Set The Date", "????.??.??");
		if( dialog.ShowModal() == wxID_OK)
		{
			wxString s2 = dialog.dateCtrl->GetValue().FormatDate();//GetString();
			s = SetSubstringInQuotes(s, s2);
			m_list->SetString(m_list->GetSelection(),s);

		}
	}
	else if( s.find( "Result" )!= -1 )
	{
		// handle result
		wxString choices[4] = {"1-0", "0-1", "1/2-1/2", "*"};
		wxSingleChoiceDialog dialog(this, "Choose", "Select a Result", 4, choices);
		if (dialog.ShowModal() == wxID_OK)
		{
		//	wxMessageBox(choices[dialog.GetSelection()], wxT("Got string"), wxOK | wxICON_INFORMATION, this);
			s = SetSubstringInQuotes(s, choices[dialog.GetSelection()]);
			m_list->SetString(m_list->GetSelection(),s);
			
		}
				
	}
	//if( s.Find( "
	else
	{
		wxTextEntryDialog dialog(this,
			                     message,
			                     wxT("Please enter a string"),
			                     toSet,
			                     wxOK | wxCANCEL);

		if (dialog.ShowModal() == wxID_OK)
		{
		//    wxMessageBox(dialog.GetValue(), wxT("Got string"), wxOK | wxICON_INFORMATION, this);
			s = SetSubstringInQuotes(s, dialog.GetValue());
			m_list->SetString(m_list->GetSelection(),s);
			
		}

	}

	edited = true;
}

void GameInfoModalDialog::OnCancel( wxCommandEvent & event )
{
	Close(true);
}