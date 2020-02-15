///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __noname__
#define __noname__

#include <wx/string.h>
#include <wx/combobox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>
#include <string>

using std::string;

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ECO_FilterDialog
///////////////////////////////////////////////////////////////////////////////
class ECO_FilterDialog : public wxDialog 
{
	private:
	
	protected:
		wxComboBox* m_comboBox1;
		wxSpinCtrl* m_spinCtrl2;
		wxComboBox* m_comboBox3;
		wxSpinCtrl* m_spinCtrl3;
		wxButton* m_button5;
		wxButton* m_button6;
	
	public:
		
		ECO_FilterDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 329,251 ), long style = wxDEFAULT_DIALOG_STYLE );
		~ECO_FilterDialog();

		void OnOk( wxCommandEvent & event );

		string eco1;
		string eco2;
		bool Ok;
	
	DECLARE_EVENT_TABLE()
};

#endif //__noname__