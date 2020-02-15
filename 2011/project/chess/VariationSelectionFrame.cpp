#include "VariationSelectionFrame.h"
#include "mdi.h"
#include "BoardCanvas.h"
#include "wx/listctrl.h"
#include "wx/listbox.h"

enum
{
    // menu items
    Exec_Quit = wxID_HIGHEST +5000,


    // control ids
    Exec_Btn_Send,// = 1000,
    Exec_Btn_SendFile,
    Exec_Btn_Get,
	KEYDOWN,
	LIST_KEY_DOWN,
    Exec_Btn_Close
};

BEGIN_EVENT_TABLE(VariationSelectionFrame, wxFrame)
    EVT_BUTTON(Exec_Btn_Send, VariationSelectionFrame::OnBtnSend)
  //  EVT_BUTTON(Exec_Btn_SendFile, VariationSelectionFrame::OnBtnSendFile)
	EVT_CHAR_HOOK(VariationSelectionFrame::OnKeyDown)
	//EVT_LIST(LIST_KEY_DOWN, VariationSelectionFrame::OnKeyDown)
	EVT_CLOSE(VariationSelectionFrame::OnClose)
	
END_EVENT_TABLE()

VariationSelectionFrame::VariationSelectionFrame(MyFrame *parent)
	:wxFrame((wxWindow*)parent, wxID_ANY, "Choose a Variation", wxPoint(150,150), wxDefaultSize, wxWANTS_CHARS|wxWS_EX_BLOCK_EVENTS|/*wxSTAY_ON_TOP*/wxFRAME_FLOAT_ON_PARENT)
{
	m_bListsWhiteMoves = false;
	this->pParent = parent;
	
	SetSize(200,400);
	m_lbox = new wxListBox(this, wxID_ANY);
    wxSizer *sizerTop = new wxBoxSizer(wxVERTICAL);
    sizerTop->Add(m_lbox, 200, wxGROW | wxALL, 5);

    wxSizer *sizerBtns = new wxBoxSizer(wxHORIZONTAL);
    sizerBtns->
        Add(new wxButton(this, Exec_Btn_Send, wxT("&Send")), 0, wxALL, 5);
    sizerBtns->
        Add(new wxButton(this, Exec_Btn_SendFile, wxT("&File...")), 0, wxALL, 5);
    sizerBtns->
        Add(new wxButton(this, Exec_Btn_Get, wxT("&Get")), 0, wxALL, 5);
    sizerBtns->
        Add(new wxButton(this, Exec_Btn_Close, wxT("&Close")), 0, wxALL, 5);

    sizerTop->Add(sizerBtns, 0, wxCENTRE | wxALL, 5);
 //   sizerTop->Add(m_textIn, 1, wxGROW | wxALL, 5);
  //  sizerTop->Add(m_textErr, 1, wxGROW | wxALL, 5);

    this->SetSizer(sizerTop);
    sizerTop->Fit(this);

  //  Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(VariationSelectionFrame::OnKeyDown),
   //                     NULL, this);
   // Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(VariationSelectionFrame::OnKeyDown),
    //                    NULL, m_lbox);
}

VariationSelectionFrame::~VariationSelectionFrame()
{

	if( m_lbox )
		delete m_lbox;
}

void VariationSelectionFrame::OnBtnSend(wxCommandEvent& WXUNUSED(event))
{
	pParent->SelectMove(m_lbox->GetSelection());
	this->Show(false);
}

void VariationSelectionFrame::OnBtnSendFile(wxCommandEvent& WXUNUSED(event))
{
}

void VariationSelectionFrame::OnBtnGet(wxCommandEvent& WXUNUSED(event))
{
}

void VariationSelectionFrame::OnBtnClose(wxCommandEvent& WXUNUSED(event))
{
}

void VariationSelectionFrame::OnClose(wxCloseEvent& event)
{
	pParent->SelectMove(0);
	this->Show(false);
	//event.Skip();
}


void VariationSelectionFrame::DoClose()
{
}

void VariationSelectionFrame::OnKeyDown(wxKeyEvent& event) 
{ 
	LogEvent("KeyDown", event); 
}

void VariationSelectionFrame::LogEvent(const wxString& name, wxKeyEvent& event)
{
	int keycode = event.GetKeyCode();
	
	switch ( keycode )
	{
		case WXK_BACK:break;
        case WXK_TAB:break;
        case WXK_RETURN:
			{
				pParent->SelectMove(m_lbox->GetSelection());
				this->Show(false);
			}
			break;
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

			}break;
        case WXK_UP:
			{
				int sel = m_lbox->GetSelection();
				if( sel-1 > -1 )// < m_lbox->GetCount() )
					m_lbox->Select(sel-1);
			}
			break;
        case WXK_RIGHT:
			{
				pParent->SelectMove(m_lbox->GetSelection());
				this->Show(false);
			}break;
        case WXK_DOWN:
			{
				int sel = m_lbox->GetSelection();
				if( sel+1 < m_lbox->GetCount() )
					m_lbox->Select(sel+1);
			}
			break;
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