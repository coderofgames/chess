#include "MyListPanel.h"
#include "ListBox.h"
#include "mdi.h"
#include "MiscUtils.h"

//=============================================================================

// listbox stuff

// ----------------------------------------------------------------------------
// Constants and globals
// ----------------------------------------------------------------------------

const wxChar *SMALL_VIRTUAL_VIEW_ITEMS[][2] =
{
    { wxT("Cat"), wxT("meow") },
    { wxT("Cow"), wxT("moo") },
    { wxT("Crow"), wxT("caw") },
    { wxT("Dog"), wxT("woof") },
    { wxT("Duck"), wxT("quack") },
    { wxT("Mouse"), wxT("squeak") },
    { wxT("Owl"), wxT("hoo") },
    { wxT("Pig"), wxT("oink") },
    { wxT("Pigeon"), wxT("coo") },
    { wxT("Sheep"), wxT("baaah") },
};

// number of items in icon/small icon view
static const int NUM_ICONS = 9;


   #include "bitmaps/toolbrai.xpm"
    #include "bitmaps/toolchar.xpm"
    #include "bitmaps/tooldata.xpm"
    #include "bitmaps/toolnote.xpm"
    #include "bitmaps/tooltodo.xpm"
    #include "bitmaps/toolchec.xpm"
    #include "bitmaps/toolgame.xpm"
    #include "bitmaps/tooltime.xpm"
    #include "bitmaps/toolword.xpm"
    #include "bitmaps/small1.xpm"


BEGIN_EVENT_TABLE(MyListPanel, wxPanel)
	//EVT_PAINT  (NotationFrame::OnPaint)
    //EVT_SIZE(NotationFrame::OnSize)

END_EVENT_TABLE()


MyListPanel::MyListPanel(MyFrame *parent) : wxPanel(parent, wxID_ANY)
{


	m_parent = parent;
	m_listCtrl = NULL;

    m_logWindow = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
                                 wxDefaultPosition, wxDefaultSize,
                                 wxTE_READONLY | wxTE_MULTILINE | wxBORDER_THEME);

	m_logWindow->Hide();

    m_imageListNormal = new wxImageList(32, 32, true);
    m_imageListSmall = new wxImageList(16, 16, true);

#ifndef __WXMSW__
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon1"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon2"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon3"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon4"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon5"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon6"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon7"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon8"), wxBITMAP_TYPE_ICO_RESOURCE) );
    m_imageListNormal->Add( wxIcon(wxT("Bitmaps/icon9"), wxBITMAP_TYPE_ICO_RESOURCE) );

    m_imageListSmall->Add( wxIcon(wxT("Bitmaps/iconsmall"), wxBITMAP_TYPE_ICO_RESOURCE) );

#else
    m_imageListNormal->Add( wxIcon( toolbrai_xpm ) );
    m_imageListNormal->Add( wxIcon( toolchar_xpm ) );
    m_imageListNormal->Add( wxIcon( tooldata_xpm ) );
    m_imageListNormal->Add( wxIcon( toolnote_xpm ) );
    m_imageListNormal->Add( wxIcon( tooltodo_xpm ) );
    m_imageListNormal->Add( wxIcon( toolchec_xpm ) );
    m_imageListNormal->Add( wxIcon( toolgame_xpm ) );
    m_imageListNormal->Add( wxIcon( tooltime_xpm ) );
    m_imageListNormal->Add( wxIcon( toolword_xpm ) );

    m_imageListSmall->Add( wxIcon( small1_xpm) );
#endif

	m_sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(m_sizer);
	m_sizer->Fit(this);

	m_smallVirtual = false;
    //m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_logWindow));
}

MyListPanel::~MyListPanel()
{
}

// ----------------------------------------------------------------------------
// changing listctrl modes
// ----------------------------------------------------------------------------

void MyListPanel::RecreateList(long flags, bool withText)
{
    // we could avoid recreating it if we don't set/clear the wxLC_VIRTUAL
    // style, but it is more trouble to do it than not
#if 0
    if ( !m_listCtrl || ((flags & wxLC_VIRTUAL) !=
            (m_listCtrl->GetWindowStyleFlag() & wxLC_VIRTUAL)) )
#endif
    {
		if( m_listCtrl )
			delete m_listCtrl;


        m_listCtrl = new MyListCtrl(this, m_parent, LIST_CTRL,
                                    wxDefaultPosition, wxDefaultSize,
                                    flags |
                                    wxBORDER_THEME | wxLC_EDIT_LABELS);

		int w, h;
		GetClientSize(&w, &h);

	//m_panel->SetSize(w,120);
   // m_textWindow->SetSize(0, 0, 200, h);
		m_listCtrl->SetSize(0, 0, w, h);		
		m_sizer->Add(m_listCtrl,5,wxEXPAND);
		m_listCtrl->AlwaysShowScrollbars(true,true);

        switch ( flags & wxLC_MASK_TYPE )
        {

            case wxLC_REPORT:
             //   if ( flags & wxLC_VIRTUAL )
                    InitWithVirtualItems();
             //   else
             //       InitWithReportItems((m_parent->currentGame!=NULL));
                break;

            default:
                wxFAIL_MSG( wxT("unknown listctrl mode") );
        }

		//InitWithVirtualItems();
		
		m_listCtrl->SetSingleStyle(wxLC_HRULES | wxLC_VRULES, true);
		
    }

    DoSize();

    m_logWindow->Clear();
}

void MyListPanel::DoSize()
{
    //if ( !m_logWindow )
    //    return;

    //wxSize size = GetClientSize();
    //wxCoord y = (2*size.y)/3;
    //m_listCtrl->SetSize(0, 32, size.x, size.y - 32);
	//m_listCtrl->AlwaysShowScrollbars(true,true);
	m_sizer->Fit(this);
//    m_logWindow->SetSize(0, y + 1, size.x, size.y - y -1);
}


void MyListPanel::InitWithListItems()
{
    for ( int i = 0; i < m_numListItems; i++ )
    {
        m_listCtrl->InsertItem(i, wxString::Format(wxT("Item %d"), i));
    }
}

bool MyListPanel::CheckNonVirtual() const
{
    if ( !m_listCtrl->HasFlag(wxLC_VIRTUAL) )
        return true;

    // "this" == whatever
    wxLogWarning(wxT("Can't do this in virtual view, sorry."));

    return false;
}
/*
void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}*/




void MyListPanel::InitWithReportItems(bool pgnLoaded)
{
	if( pgnLoaded )
	{
		//m_listCtrl->SetImageList(m_imageListSmall, wxIMAGE_LIST_SMALL);
		int numGames = m_parent->pgnLoad.games.size();
		
		wxListItem itemCol;

		itemCol.SetText("Number");
		itemCol.SetAlign(wxLIST_FORMAT_LEFT);
		m_listCtrl->InsertColumn(0, itemCol );

		itemCol.SetText(wxT("White"));
		itemCol.SetAlign(wxLIST_FORMAT_LEFT);
		m_listCtrl->InsertColumn(1, itemCol);

		itemCol.SetText(wxT("Black"));
		itemCol.SetAlign(wxLIST_FORMAT_LEFT);
		m_listCtrl->InsertColumn(2, itemCol);

		itemCol.SetText(wxT("Result"));
		itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
		m_listCtrl->InsertColumn(3, itemCol);		

		itemCol.SetText(wxT("Termination"));
		itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
		m_listCtrl->InsertColumn(4, itemCol);	

		itemCol.SetText(wxT("Event"));
		itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
		m_listCtrl->InsertColumn(5, itemCol);	

		itemCol.SetText(wxT("Date"));
		itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
		m_listCtrl->InsertColumn(6, itemCol);


		
		m_listCtrl->Hide();

		wxStopWatch sw;
		
		for( int i = 0; i < numGames; i++ )
		{
			PGN::Game *pGame = m_parent->pgnLoad.games[i];
			if( pGame )
			{
				//if( pgnLoad.games[i]->vec_header.size() >= 5 )
				{
					wxString strEvent = " ";
					wxString strDate = " ";
					wxString strWhite = " ";
					wxString strBlack = " ";
					wxString strResult = " ";
					wxString strTermination = "";

					for( int j = 0; j < m_parent->pgnLoad.games[i]->vec_header.size(); j++ )
					{
						string currString = m_parent->pgnLoad.games[i]->vec_header[j].data;
						
						int findEvent = -1;
						findEvent = currString.find("Event");
						int findEventLc = -1;
						findEventLc = currString.find("event");
						if( findEvent!= -1 )
						{

							strEvent =  GetSubstringInQuotes(currString);
							continue;
						}
						if( findEventLc!= -1 )
						{
							strEvent =  GetSubstringInQuotes(currString);
							continue;
						}

						int findDate = -1;
						findDate = currString.find("Date");
						int findDateLc = -1;
						findDateLc = currString.find("date");
						if( findDate != -1 )
						{
							strDate =  GetSubstringInQuotes(currString);
							continue;
						}
						if( findDateLc != -1 )
						{
							strDate =  GetSubstringInQuotes(currString);
							continue;
						}

						int findWhite = -1; 
						findWhite = currString.find("White ");
						int findWhiteLc = -1; 
						findWhiteLc = currString.find("white ");
						int findWhiteElo = -1;
						findWhiteElo = currString.find("WhiteElo ");
						int findWhiteEloLc = -1;
						findWhiteEloLc = currString.find("whiteelo ");
						if( (findWhite != -1) && (findWhiteElo != findWhite))
						{
							 
							strWhite =  GetSubstringInQuotes(currString);
							continue;
						}
						if( (findWhiteLc != -1) && (findWhiteEloLc != findWhiteLc))
						{
							strWhite =  GetSubstringInQuotes(currString);
							continue;
						}

						int findBlack = -1; 
						findBlack = currString.find("Black ");
						int findBlackLc = -1;
						findBlackLc = currString.find("black ");
						int findBlackElo = -1;
						findBlackElo = currString.find("BlackElo ");
						int findBlackEloLc = -1;
						findBlackEloLc = currString.find("blackelo ");
						if( (findBlack != -1) && (findBlackElo != findBlack) )
						{
							strBlack =  GetSubstringInQuotes(currString);
							continue;
						}
						if( (findBlackLc != -1) && (findBlackEloLc != findBlackLc) )
						{
							strBlack =  GetSubstringInQuotes(currString);
							continue;
						}

						int findResult = -1; 
						findResult = currString.find("Result");
						int findResultLc = -1; 
						findResultLc = currString.find("result");
						if( findResult != -1 )
						{
							strResult =  GetSubstringInQuotes(currString);
							continue;
						}	
						if( findResultLc != -1 )
						{
							strResult =  GetSubstringInQuotes(currString);
							continue;
						}	

						int findTermination = -1;
						findTermination = currString.find("Termination");
						int findTerminationLc = -1;
						findTerminationLc = currString.find("termination");
						if( findTermination != -1 )
						{
							strTermination =  GetSubstringInQuotes(currString);
							continue;
						}	
						if( findTerminationLc != -1 )
						{
							strTermination =  GetSubstringInQuotes(currString);
							continue;
						}	
					}

					m_listCtrl->InsertItemInReportView(i, 
						strWhite, strBlack, strResult, strTermination, strEvent, strDate);
				}

				wxListItem item;
				item.m_itemId = i;
				//item.
				item.SetFont(wxFont(8, wxMODERN, wxNORMAL, wxBOLD, false));
				m_listCtrl->SetItem( item );
				//wxDeprecatedGUIConstants::wxSOLID
			}
		}

		m_listCtrl->Show();

		m_listCtrl->SetColumnWidth( 0, wxLIST_AUTOSIZE );
		m_listCtrl->SetColumnWidth( 1, wxLIST_AUTOSIZE );
		m_listCtrl->SetColumnWidth( 2, wxLIST_AUTOSIZE );
	}
	else
	{
/*		m_listCtrl->SetImageList(m_imageListSmall, wxIMAGE_LIST_SMALL);

		// note that under MSW for SetColumnWidth() to work we need to create the
		// items with images initially even if we specify dummy image id
		wxListItem itemCol;
		itemCol.SetText(wxT("Column 1"));
		itemCol.SetImage(-1);
		m_listCtrl->InsertColumn(0, itemCol);

		itemCol.SetText(wxT("Column 2"));
		itemCol.SetAlign(wxLIST_FORMAT_CENTRE);
		m_listCtrl->InsertColumn(1, itemCol);

		itemCol.SetText(wxT("Column 3"));
		itemCol.SetAlign(wxLIST_FORMAT_RIGHT);
		m_listCtrl->InsertColumn(2, itemCol);

		// to speed up inserting we hide the control temporarily
		m_listCtrl->Hide();

		wxStopWatch sw;

		for ( int i = 0; i < m_numListItems; i++ )
		{
			m_listCtrl->InsertItemInReportView(i);
		}

		m_logWindow->WriteText(wxString::Format(wxT("%d items inserted in %ldms\n"),
                                            m_numListItems, sw.Time()));
		m_listCtrl->Show();

		// we leave all mask fields to 0 and only change the colour
		wxListItem item;
		item.m_itemId = 0;

		m_listCtrl->SetItem( item );

		item.m_itemId = 2;
		//item.SetTextColour(*wxGREEN);
		m_listCtrl->SetItem( item );
		item.m_itemId = 4;
		//item.SetTextColour(*wxLIGHT_GREY);
		 item.SetFont(wxFont(8, wxMODERN, wxNORMAL, wxBOLD, false));
		// item.SetBackgroundColour(*wxRED);
		m_listCtrl->SetItem( item );

		// m_listCtrl->SetTextColour(*wxBLUE);

		m_listCtrl->SetColumnWidth( 0, wxLIST_AUTOSIZE );
		m_listCtrl->SetColumnWidth( 1, wxLIST_AUTOSIZE );
		m_listCtrl->SetColumnWidth( 2, wxLIST_AUTOSIZE );

		// Set images in columns
		m_listCtrl->SetItemColumnImage(1, 1, 0);

		wxListItem info;
		info.SetImage(0);
		info.SetId(3);
		info.SetColumn(2);
		m_listCtrl->SetItem(info);
			
		// test SetItemFont too
		// m_listCtrl->SetItemFont(0, *wxITALIC_FONT);
		//m_listCtrl->SetTransparent(wxByte(0xffff));*/
	}
}

void MyListPanel::InitWithIconItems(bool withText, bool sameIcon)
{
    m_listCtrl->SetImageList(m_imageListNormal, wxIMAGE_LIST_NORMAL);
    m_listCtrl->SetImageList(m_imageListSmall, wxIMAGE_LIST_SMALL);

    for ( int i = 0; i < NUM_ICONS; i++ )
    {
        int image = sameIcon ? 0 : i;

        if ( withText )
        {
            m_listCtrl->InsertItem(i, wxString::Format(wxT("Label %d"), i),
                                   image);
        }
        else
        {
            m_listCtrl->InsertItem(i, image);
        }
    }
}



void MyListPanel::InitWithVirtualItems()
{
    /*m_listCtrl->SetImageList(m_imageListSmall, wxIMAGE_LIST_SMALL);

    if ( m_smallVirtual )
    {
        m_listCtrl->InsertColumn(0, wxT("Animal"));
        m_listCtrl->InsertColumn(1, wxT("Sound"));
        m_listCtrl->SetItemCount(WXSIZEOF(SMALL_VIRTUAL_VIEW_ITEMS));
    }
    else*/
    {

	
		
		
       // m_listCtrl->InsertColumn(0, wxT("First Column"));
       // m_listCtrl->InsertColumn(1, wxT("Second Column"));

		// GET THE NUM-GAMES FROM THE VM STUFF
		if( (m_parent->pgnLoad.pgn_load_mode == 1) || (m_parent->pgnLoad.pgn_load_mode == 3))
		{
			if( m_parent->pgnLoad.sort_method == 1 )
			{
				m_listCtrl->InsertColumn(0, wxT("Number") );
				m_listCtrl->InsertColumn(1, wxT("White"));
				m_listCtrl->InsertColumn(2, wxT("Black"));
				m_listCtrl->InsertColumn(3, wxT("WhiteElo"));
				m_listCtrl->InsertColumn(4, wxT("BlackElo"));
				m_listCtrl->InsertColumn(5, wxT("ECO"));
				m_listCtrl->InsertColumn(6, wxT("Event"));
				m_listCtrl->InsertColumn(7, wxT("Site"));
				m_listCtrl->InsertColumn(8, wxT("Date"));
				m_listCtrl->InsertColumn(9, wxT("Result"));

				if( m_parent->pgnLoad.filter == true )
				{
					if( m_parent->pgnLoad.SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_VM )
					{
						m_listCtrl->SetItemCount(m_parent->pgnLoad.tree_filtered_indexes.size());
					}
					else
					{
						m_listCtrl->SetItemCount(m_parent->pgnLoad.m_filteredIndexes.size());
					}
				}
				else
				{
					m_listCtrl->SetItemCount(m_parent->pgnLoad.virtualGameCount);
				}
			}
			else
			{
				m_listCtrl->InsertColumn(0, wxT("Number") );
				m_listCtrl->InsertColumn(1, wxT("White"));
				m_listCtrl->InsertColumn(2, wxT("Black"));
				m_listCtrl->InsertColumn(3, wxT("Event"));
				m_listCtrl->InsertColumn(4, wxT("Date"));
				m_listCtrl->InsertColumn(5, wxT("Result"));
				m_listCtrl->InsertColumn(6, wxT("Termination"));
				m_listCtrl->SetColumnWidth(0, 150);
				m_listCtrl->SetColumnWidth(1, 150);
					// pgnLoadDataIsValid()
					if( m_parent->pgnLoad.MemFileDataIsOk() )
					{
						m_listCtrl->SetItemCount(m_parent->pgnLoad.virtualGameCount);
					}
			}


		}
		else if( m_parent->pgnLoad.pgn_load_mode == 2 )
		{

		m_listCtrl->InsertColumn(0, wxT("Number") );
		m_listCtrl->InsertColumn(1, wxT("White"));
		m_listCtrl->InsertColumn(2, wxT("Black"));
		m_listCtrl->InsertColumn(3, wxT("WhiteElo"));
		m_listCtrl->InsertColumn(4, wxT("BlackElo"));
		m_listCtrl->InsertColumn(5, wxT("Event"));
		m_listCtrl->InsertColumn(6, wxT("Site"));
		m_listCtrl->InsertColumn(7, wxT("Date"));
		m_listCtrl->InsertColumn(8, wxT("Result"));
		m_listCtrl->InsertColumn(9, wxT("ECO"));
		m_listCtrl->InsertColumn(10, wxT("Moves"));

			m_listCtrl->SetColumnWidth(0, 150);
			m_listCtrl->SetColumnWidth(1, 150);
			m_listCtrl->SetItemCount(m_parent->pgnLoad.rec_count);
				
		}
		else
		{
			m_listCtrl->SetItemCount(m_parent->pgnLoad.games.size());
		}
    }
}



void MyListPanel::OnFreeze(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage(wxT("Freezing the control"));

    m_listCtrl->Freeze();
}

void MyListPanel::OnThaw(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage(wxT("Thawing the control"));

    m_listCtrl->Thaw();
}

void MyListPanel::OnToggleLines(wxCommandEvent& event)
{
    m_listCtrl->SetSingleStyle(wxLC_HRULES | wxLC_VRULES, event.IsChecked());
}

void MyListPanel::OnToggleHeader(wxCommandEvent& event)
{
    wxLogMessage("%s the header", event.IsChecked() ? "Showing" : "Hiding");

    m_listCtrl->ToggleWindowStyle(wxLC_NO_HEADER);
}

void MyListPanel::OnGoTo(wxCommandEvent& WXUNUSED(event))
{
    long index = 3;
    m_listCtrl->SetItemState(index, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);

    long sel = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
    if ( sel != -1 )
        m_listCtrl->SetItemState(sel, 0, wxLIST_STATE_SELECTED);
    m_listCtrl->SetItemState(index, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void MyListPanel::OnFocusLast(wxCommandEvent& WXUNUSED(event))
{
    long index = m_listCtrl->GetItemCount() - 1;
    if ( index == -1 )
    {
        return;
    }

    m_listCtrl->SetItemState(index, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
    m_listCtrl->EnsureVisible(index);
}

void MyListPanel::OnToggleFirstSel(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->SetItemState(0, (~m_listCtrl->GetItemState(0, wxLIST_STATE_SELECTED) ) & wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void MyListPanel::OnDeselectAll(wxCommandEvent& WXUNUSED(event))
{
    if ( !CheckNonVirtual() )
        return;

    int n = m_listCtrl->GetItemCount();
    for (int i = 0; i < n; i++)
        m_listCtrl->SetItemState(i,0,wxLIST_STATE_SELECTED);
}

void MyListPanel::OnSelectAll(wxCommandEvent& WXUNUSED(event))
{
    if ( !CheckNonVirtual() )
        return;

    int n = m_listCtrl->GetItemCount();
    for (int i = 0; i < n; i++)
        m_listCtrl->SetItemState(i,wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
}

void MyListPanel::OnListView(wxCommandEvent& WXUNUSED(event))
{
    RecreateList(wxLC_LIST);
}

void MyListPanel::OnReportView(wxCommandEvent& WXUNUSED(event))
{
    RecreateList(wxLC_REPORT);
}


void MyListPanel::OnIconView(wxCommandEvent& WXUNUSED(event))
{
    RecreateList(wxLC_ICON, false);
}

void MyListPanel::OnIconTextView(wxCommandEvent& WXUNUSED(event))
{
    RecreateList(wxLC_ICON);
}

void MyListPanel::OnSmallIconView(wxCommandEvent& WXUNUSED(event))
{
    RecreateList(wxLC_SMALL_ICON, false);
}

void MyListPanel::OnSmallIconTextView(wxCommandEvent& WXUNUSED(event))
{
    RecreateList(wxLC_SMALL_ICON);
}

void MyListPanel::OnVirtualView(wxCommandEvent& WXUNUSED(event))
{
    m_smallVirtual = false;
    RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}

void MyListPanel::OnSmallVirtualView(wxCommandEvent& WXUNUSED(event))
{
    m_smallVirtual = true;
    RecreateList(wxLC_REPORT | wxLC_VIRTUAL);
}

void MyListPanel::OnSetItemsCount(wxCommandEvent& WXUNUSED(event))
{
    int numItems = wxGetNumberFromUser
                   (
                        "Enter the initial number of items for "
                        "the list and report views",
                        "Number of items:",
                        "wxWidgets wxListCtrl sample",
                        m_numListItems,
                        0,
                        10000,
                        this
                   );
    if ( numItems == -1 || numItems == m_numListItems )
        return;

    m_numListItems = numItems;

    if ( m_listCtrl->HasFlag(wxLC_REPORT) &&
            !m_listCtrl->HasFlag(wxLC_VIRTUAL) )
        RecreateList(wxLC_REPORT);
    else if ( m_listCtrl->HasFlag(wxLC_LIST) )
        RecreateList(wxLC_LIST);
}


int wxCALLBACK
MyCompareFunction(long item1, long item2, wxIntPtr WXUNUSED(sortData))
{
    // inverse the order
    if (item1 < item2)
        return -1;
    if (item1 > item2)
        return 1;

    return 0;
}

void MyListPanel::OnSort(wxCommandEvent& WXUNUSED(event))
{
    wxStopWatch sw;

    m_listCtrl->SortItems(MyCompareFunction, 0);

    m_logWindow->WriteText(wxString::Format(wxT("Sorting %d items took %ld ms\n"),
                                            m_listCtrl->GetItemCount(),
                                            sw.Time()));
}

void MyListPanel::OnFind(wxCommandEvent& WXUNUSED(event))
{
    wxStopWatch sw;

    const int itemCount = m_listCtrl->GetItemCount();
    for ( int i = 0; i < itemCount; i++ )
        m_listCtrl->FindItem(-1, i);

    wxLogMessage("Calling Find() for all %d items took %ld ms",
                 itemCount, sw.Time());
}

void MyListPanel::OnShowSelInfo(wxCommandEvent& WXUNUSED(event))
{
    int selCount = m_listCtrl->GetSelectedItemCount();
    wxLogMessage(wxT("%d items selected:"), selCount);

    // don't show too many items
    size_t shownCount = 0;

    long item = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
                                        wxLIST_STATE_SELECTED);
    while ( item != -1 )
    {
        wxLogMessage(wxT("\t%ld (%s)"),
                     item, m_listCtrl->GetItemText(item).c_str());

        if ( ++shownCount > 10 )
        {
            wxLogMessage(wxT("\t... more selected items snipped..."));
            break;
        }

        item = m_listCtrl->GetNextItem(item, wxLIST_NEXT_ALL,
                                       wxLIST_STATE_SELECTED);
    }
}

void MyListPanel::OnShowViewRect(wxCommandEvent& WXUNUSED(event))
{
    const wxRect r = m_listCtrl->GetViewRect();
    wxLogMessage("View rect: (%d, %d)-(%d, %d)",
                 r.GetLeft(), r.GetTop(), r.GetRight(), r.GetBottom());
}

// ----------------------------------------------------------------------------
// column order tests
// ----------------------------------------------------------------------------

#ifdef wxHAS_LISTCTRL_COLUMN_ORDER

static wxString DumpIntArray(const wxArrayInt& a)
{
    wxString s("{ ");
    const size_t count = a.size();
    for ( size_t n = 0; n < count; n++ )
    {
        if ( n )
            s += ", ";
        s += wxString::Format("%lu", (unsigned long)a[n]);
    }

    s += " }";

    return s;
}

void MyListPanel::OnSetColOrder(wxCommandEvent& WXUNUSED(event))
{
    wxArrayInt order(3);
    order[0] = 2;
    order[1] = 0;
    order[2] = 1;
    if ( m_listCtrl->SetColumnsOrder(order) )
    {
        wxLogMessage("Column order set to %s", DumpIntArray(order));
    }
}

void MyListPanel::OnGetColOrder(wxCommandEvent& WXUNUSED(event))
{
    // show what GetColumnsOrder() returns
    const wxArrayInt order = m_listCtrl->GetColumnsOrder();
    wxString msg = "Columns order: " +
                        DumpIntArray(m_listCtrl->GetColumnsOrder()) + "\n";

    int n;
    const int count = m_listCtrl->GetColumnCount();

    // show the results of GetColumnOrder() for each column
    msg += "GetColumnOrder() results:\n";
    for ( n = 0; n < count; n++ )
    {
        msg += wxString::Format("    %2d -> %2d\n",
                                n, m_listCtrl->GetColumnOrder(n));
    }

    // and the results of GetColumnIndexFromOrder() too
    msg += "GetColumnIndexFromOrder() results:\n";
    for ( n = 0; n < count; n++ )
    {
        msg += wxString::Format("    %2d -> %2d\n",
                                n, m_listCtrl->GetColumnIndexFromOrder(n));
    }

    wxLogMessage("%s", msg);
}

#endif // wxHAS_LISTCTRL_COLUMN_ORDER

void MyListPanel::OnShowColInfo(wxCommandEvent& WXUNUSED(event))
{
    int count = m_listCtrl->GetColumnCount();
    wxLogMessage(wxT("%d columns:"), count);
    for ( int c = 0; c < count; c++ )
    {
        wxLogMessage(wxT("\tcolumn %d has width %d"), c,
                     m_listCtrl->GetColumnWidth(c));
    }
}

void MyListPanel::OnUpdateUIEnableInReport(wxUpdateUIEvent& event)
{
    event.Enable( (m_listCtrl->GetWindowStyleFlag() & wxLC_REPORT) != 0 );
}

void MyListPanel::OnToggleMultiSel(wxCommandEvent& WXUNUSED(event))
{
    long flags = m_listCtrl->GetWindowStyleFlag();
    if ( flags & wxLC_SINGLE_SEL )
        flags &= ~wxLC_SINGLE_SEL;
    else
        flags |= wxLC_SINGLE_SEL;

    m_logWindow->WriteText(wxString::Format(wxT("Current selection mode: %sle\n"),
                           (flags & wxLC_SINGLE_SEL) ? wxT("sing") : wxT("multip")));

    RecreateList(flags);
}

void MyListPanel::OnUpdateToggleMultiSel(wxUpdateUIEvent& event)
{
     event.Check(!m_listCtrl->HasFlag(wxLC_SINGLE_SEL));
}

void MyListPanel::OnUpdateToggleHeader(wxUpdateUIEvent& event)
{
    event.Check(!m_listCtrl->HasFlag(wxLC_NO_HEADER));
}

void MyListPanel::OnSetFgColour(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->SetForegroundColour(wxGetColourFromUser(this));
    m_listCtrl->Refresh();
}

void MyListPanel::OnSetBgColour(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->SetBackgroundColour(wxGetColourFromUser(this));
    m_listCtrl->Refresh();
}

void MyListPanel::OnAdd(wxCommandEvent& WXUNUSED(event))
{
    m_listCtrl->InsertItem(m_listCtrl->GetItemCount(), wxT("Appended item"));
}

void MyListPanel::OnEdit(wxCommandEvent& WXUNUSED(event))
{
    // demonstrate cancelling editing: this currently is wxMSW-only
#ifdef __WXMSW__
    if ( m_listCtrl->GetEditControl() )
    {
        m_listCtrl->EndEditLabel(true);
    }
    else // start editing
#endif // __WXMSW__
    {
        long itemCur = m_listCtrl->GetNextItem(-1, wxLIST_NEXT_ALL,
                                               wxLIST_STATE_FOCUSED);

        if ( itemCur != -1 )
        {
            m_listCtrl->EditLabel(itemCur);
        }
        else
        {
            m_logWindow->WriteText(wxT("No item to edit"));
        }
    }
}

void MyListPanel::OnDelete(wxCommandEvent& WXUNUSED(event))
{
    if ( m_listCtrl->GetItemCount() )
    {
        m_listCtrl->DeleteItem(0);
    }
    else
    {
        m_logWindow->WriteText(wxT("Nothing to delete"));
    }
}

void MyListPanel::OnDeleteAll(wxCommandEvent& WXUNUSED(event))
{
    wxStopWatch sw;

    int itemCount = m_listCtrl->GetItemCount();

    m_listCtrl->DeleteAllItems();

   // m_logWindow->WriteText(wxString::Format(wxT("Deleting %d items took %ld ms\n"),
    //                                        itemCount,
     //                                       sw.Time()));
}