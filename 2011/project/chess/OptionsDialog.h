#ifndef OPTIONS_DIALOG_H
#define OPTIONS_DIALOG_H

#include "headers.h"

// ----------------------------------------------------------------------------
// MyPipeFrame: allows the user to communicate with the child process
// ----------------------------------------------------------------------------

class OptionsDialog : public wxFrame
{
public:
    OptionsDialog(MyFrame *parent);
	~OptionsDialog();

protected:
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