// ----------------------------------------------------------------------------
// MyListCtrl
// ----------------------------------------------------------------------------

#include "ListBox.h"
#include "mdi.h"
#include "MiscUtils.h"
#include "ExpandableArray.h"


extern vector<string> output;

// IDs for the menu commands




BEGIN_EVENT_TABLE(MyListCtrl, wxListCtrl)
    EVT_LIST_BEGIN_DRAG(LIST_CTRL, MyListCtrl::OnBeginDrag)
    EVT_LIST_BEGIN_RDRAG(LIST_CTRL, MyListCtrl::OnBeginRDrag)
   // EVT_LIST_BEGIN_LABEL_EDIT(LIST_CTRL, MyListCtrl::OnBeginLabelEdit)
    //EVT_LIST_END_LABEL_EDIT(LIST_CTRL, MyListCtrl::OnEndLabelEdit)
    EVT_LIST_DELETE_ITEM(LIST_CTRL, MyListCtrl::OnDeleteItem)
    EVT_LIST_DELETE_ALL_ITEMS(LIST_CTRL, MyListCtrl::OnDeleteAllItems)
    EVT_LIST_ITEM_SELECTED(LIST_CTRL, MyListCtrl::OnSelected)
    //EVT_LIST_ITEM_DESELECTED(LIST_CTRL, MyListCtrl::OnDeselected)
    EVT_LIST_KEY_DOWN(LIST_CTRL, MyListCtrl::OnListKeyDown)
    //EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, MyListCtrl::OnActivated)
    //EVT_LIST_ITEM_FOCUSED(LIST_CTRL, MyListCtrl::OnFocused)

    EVT_LIST_COL_CLICK(LIST_CTRL, MyListCtrl::OnColClick)
    EVT_LIST_COL_RIGHT_CLICK(LIST_CTRL, MyListCtrl::OnColRightClick)
    //EVT_LIST_COL_BEGIN_DRAG(LIST_CTRL, MyListCtrl::OnColBeginDrag)
    //EVT_LIST_COL_DRAGGING(LIST_CTRL, MyListCtrl::OnColDragging)
   // EVT_LIST_COL_END_DRAG(LIST_CTRL, MyListCtrl::OnColEndDrag)

    EVT_LIST_CACHE_HINT(LIST_CTRL, MyListCtrl::OnCacheHint)

#if USE_CONTEXT_MENU
    EVT_CONTEXT_MENU(MyListCtrl::OnContextMenu)
#endif
    EVT_CHAR(MyListCtrl::OnChar)

    EVT_RIGHT_DOWN(MyListCtrl::OnRightClick)
END_EVENT_TABLE()

void MyListCtrl::OnCacheHint(wxListEvent& event)
{
    wxLogMessage( wxT("OnCacheHint: cache items %ld..%ld"),
                  event.GetCacheFrom(), event.GetCacheTo() );
}

void MyListCtrl::SetColumnImage(int col, int image)
{
    wxListItem item;
    item.SetMask(wxLIST_MASK_IMAGE);
    item.SetImage(image);
    SetColumn(col, item);
}

MyListCtrl* caller;
int wxCALLBACK
MyCompareFunction1(long item1, long item2, wxIntPtr WXUNUSED(sortData))
{
	int colToSort;

	if( caller->sortMode == 1 )
	{
		colToSort = 0;
	}
	else
	{
		colToSort = caller->ColToSort;
	}

    // inverse the order
    if (item1 < item2)
		return caller->colSortToggle[colToSort] ? 1 : -1;
    if (item1 > item2)
        return caller->colSortToggle[colToSort] ? -1 : 1;

    return 0;
}



int wxCALLBACK
MyCompareFunction2(long item1, long item2, wxIntPtr WXUNUSED(sortData))
{
    wxListItem info;
    info.m_itemId = item1;
	info.m_col = caller->ColToSort;
	caller->GetItem(info);

	wxString itemText1 = caller->GetItemText(info, caller->ColToSort);

    wxListItem info2;
    info2.m_itemId = item2;
	info2.m_col = caller->ColToSort;
	caller->GetItem(info2);
	//info2.
	wxString itemText2 = caller->GetItemText(info2, caller->ColToSort);
   

	if( itemText1.Length() == 0 )
	{

		return caller->colSortToggle[caller->ColToSort] ? 1 : -1;
	}
	if( itemText2.Length() == 0 )
	{

		return caller->colSortToggle[caller->ColToSort] ? -1 : 1;
	}

	if( itemText1[0] < itemText2[0] )
	{

		return caller->colSortToggle[caller->ColToSort] ? 1 : -1;
	}
	if( itemText1[0] > itemText2[0] )
	{

		return caller->colSortToggle[caller->ColToSort] ? -1 : 1;
	}

	if( itemText1[0] == itemText2[0] )
	{
		for( int i = 0; i < itemText1.Length(); i++ )
		{
			if( i < itemText2.Length() )
			{
				if( itemText1[i] < itemText2[i] )
				{

					return caller->colSortToggle[caller->ColToSort] ? 1 : -1;
				}
				if( itemText1[i] > itemText2[i] )
				{
					
					return caller->colSortToggle[caller->ColToSort] ? -1 : 1;
				}
			}
			else
			{
 
				return caller->colSortToggle[caller->ColToSort] ? -1 : 1;
			}
		}
	}


    return 0;
}

int 
MyCompareFunctionStrings(wxString item1, wxString item2)
{
	if( item1.Length() == 0 )
		return -1;
	if( item2.Length() == 0 )
		return 1;

	if( item1[0] < item2[0] )
		return -1;
	if( item1[0] > item2[0] )
		return 1;

	if( item1[0] == item2[0] )
	{
		for( int i = 0; i < item1.Length(); i++ )
		{
			if( i < item2.Length() )
			{
				if( item1[i] < item2[i] )
					return -1;
				if( item1[i] > item2[i] )
					return 1;
			}
			else
			{
				return 1;
			}
		}
	}


    return 0;
}



void MyListCtrl::OnColClick(wxListEvent& event)
{
	
    int col = event.GetColumn();
	
	ColToSort = col;
	colSortToggle[ColToSort] = !colSortToggle[ColToSort];

	caller = this;

	wxListItem list_item;
	this->GetColumn(col, list_item );
	wxArrayInt colOrd = this->GetColumnsOrder();

	col = colOrd.Index(col); 
	//wxListItemAttr * attr = list_item.GetAttributes();
	
	
	wxStopWatch sw;

	if( col == 0 )
	{
		sortMode = 0 ;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
		//	SortItems(MyCompareFunction1, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{
				/*if( colSortToggle[ColToSort] )
				{
					m_frame->pgnLoad.SortAscendingRAM(col);
				}
				else
				{
					m_frame->pgnLoad.SortDescendingRAM(col);
				}*/


					m_frame->pgnLoad.SortByNumber();

			}
		}
		
	}
	else if( col == 1 )
	{
		sortMode = 1;
		/*if( colSortToggle[ColToSort] == false )
		{
			colSortToggle[0] = true;
		}
		else
		{
			colSortToggle[0] = false;
		}*/
		//colSortToggle[0] = !colSortToggle[0];
		
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
//			SortItems(MyCompareFunction1, 0);
//			SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "White" )
				{
					m_frame->pgnLoad.SortByWhiteName();
				}
	
			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

			//	else if(  list_item.GetText() == "White" )
				{
					m_frame->SortByWhiteName();
				}
			
			}

		}
	}
	else if( col == 2 )
	{
		/*if( colSortToggle[ColToSort] == false )
		{
			colSortToggle[0] = true;
		}
		else
		{
			colSortToggle[0] = false;
		}*/
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
//			SortItems(MyCompareFunction1, 0);
//			SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "Black" )
				{
					m_frame->SortByBlackName();
				}
	
			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{
	
				//else if(  list_item.GetText() == "Black" )
				{
					m_frame->SortByBlackName();
				}
	
			}

		}
	}
	else if( col == 3 )
	{
		/*if( colSortToggle[ColToSort] == false )
		{
		}
		else
		{
		}*/
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
//			SortItems(MyCompareFunction1, 0);
//			SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "WhiteElo" )
				{
					m_frame->SortByWhiteElo();
				}
	
			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "WhiteElo" )
				{
					m_frame->SortByWhiteElo();
				}

			}

		}
	}
	else if( col == 4 )
	{
		/*if( colSortToggle[ColToSort] == false )
		{
		}
		else
		{
		}*/
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
		//SortItems(MyCompareFunction1, 0);
		//	SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "BlackElo" )
				{
					m_frame->SortByBlackElo();
				}
	
			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "BlackElo" )
				{
					m_frame->SortByBlackElo();
				}
	
			}

		}
	}
	else if( col == 5 )
	{
		/*if( colSortToggle[ColToSort] == false )
		{
		}
		else
		{
		}*/
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
		//	SortItems(MyCompareFunction1, 0);
		//	SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "ECO" )
				{
					m_frame->SortByECO();
				}
	
			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "ECO" )
				{
					m_frame->SortByECO();
				}
	
			}

		}

	}
	else if (col == 6)
	{
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
			//SortItems(MyCompareFunction1, 0);
			//SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "Event" )
				{
					m_frame->SortByEvent();
				}

			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{
	
				//else if(  list_item.GetText() == "Event" )
				{
					m_frame->SortByEvent();
				}
	
			}

		}
	}
	else if (col == 7)
	{
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
			//SortItems(MyCompareFunction1, 0);
			//SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "Site" )
				{
					m_frame->SortBySite();
				}
	
			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "Site" )
				{
					m_frame->SortBySite();
				}

			}
		}
	}
	else if (col == 8)
	{
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
			//SortItems(MyCompareFunction1, 0);
			//SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "Date" )
				{
					m_frame->SortByDate();
				}

			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{
	
				//else if(  list_item.GetText() == "Date" )
				{
					m_frame->SortByDate();
				}

			}
		}
	}	
	else if (col == 9)
	{
		sortMode = 1;
		colSortToggle[0] = false;
		if( m_frame->pgnLoad.pgn_load_mode == 0 )
		{
			//SortItems(MyCompareFunction1, 0);
			//SortItems(MyCompareFunction2, 0);
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 1 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{

				//else if(  list_item.GetText() == "Date" )
				{
					m_frame->SortByResult();
				}

			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 3 )
		{
			m_frame->pgnLoad.sortType = col;
			if( m_frame->pgnLoad.sort_method == 1 )
			{
	
				//else if(  list_item.GetText() == "Date" )
				{
					m_frame->SortByResult();
				}

			}
		}
	}	

	//m_frame->pgnLoad.sortType = 0;
	
    // set or unset image
   // static bool x = false;
   // x = !x;
   // SetColumnImage(col, x ? 0 : -1);
   // m_logWindow->WriteText(wxString::Format(wxT("Sorting %d items took %ld ms\n"),
   //                                         GetItemCount(),
    //                                        sw.Time()));
    wxLogMessage( wxT("OnColumnClick at %d."), col );
}

void MyListCtrl::OnColRightClick(wxListEvent& event)
{
    int col = event.GetColumn();
    if ( col != -1 )
    {
        SetColumnImage(col, -1);
    }

    // Show popupmenu at position
    wxMenu menu(wxT("Test"));
    menu.Append(LIST_ABOUT, wxT("&About"));
    PopupMenu(&menu, event.GetPoint());

    wxLogMessage( wxT("OnColumnRightClick at %d."), event.GetColumn() );
}

void MyListCtrl::LogColEvent(const wxListEvent& event, const wxChar *name)
{
    const int col = event.GetColumn();

    wxLogMessage(wxT("%s: column %d (width = %d or %d)."),
                 name,
                 col,
                 event.GetItem().GetWidth(),
                 GetColumnWidth(col));
}

void MyListCtrl::OnColBeginDrag(wxListEvent& event)
{
    LogColEvent( event, wxT("OnColBeginDrag") );

    if ( event.GetColumn() == 0 )
    {
        wxLogMessage(wxT("Resizing this column shouldn't work."));

        event.Veto();
    }
}

void MyListCtrl::OnColDragging(wxListEvent& event)
{
   // LogColEvent( event, wxT("OnColDragging") );
}

void MyListCtrl::OnColEndDrag(wxListEvent& event)
{
   // LogColEvent( event, wxT("OnColEndDrag") );
}

void MyListCtrl::OnBeginDrag(wxListEvent& event)
{
    //const wxPoint& pt = event.m_pointDrag;

   // int flags;
   // wxLogMessage( wxT("OnBeginDrag at (%d, %d), item %ld."),
   //               pt.x, pt.y, HitTest(pt, flags) );
}

void MyListCtrl::OnBeginRDrag(wxListEvent& event)
{
 //   wxLogMessage( wxT("OnBeginRDrag at %d,%d."),
  //                event.m_pointDrag.x, event.m_pointDrag.y );
}

void MyListCtrl::OnBeginLabelEdit(wxListEvent& event)
{
    //wxLogMessage( wxT("OnBeginLabelEdit: %s"), event.m_item.m_text.c_str());

   /* wxTextCtrl * const text = GetEditControl();
    if ( !text )
    {
        wxLogMessage("BUG: started to edit but no edit control");
    }
    else
    {
        wxLogMessage("Edit control value: \"%s\"", text->GetValue());
    }*/
}

void MyListCtrl::OnEndLabelEdit(wxListEvent& event)
{
    wxLogMessage( wxT("OnEndLabelEdit: %s"),
        (
            event.IsEditCancelled() ?
            wxString("[cancelled]") :
            event.m_item.m_text
        ).c_str()
    );
}

void MyListCtrl::OnDeleteItem(wxListEvent& event)
{
    LogEvent(event, wxT("OnDeleteItem"));
    wxLogMessage( wxT("Number of items when delete event is sent: %d"), GetItemCount() );
}

void MyListCtrl::OnDeleteAllItems(wxListEvent& event)
{
    LogEvent(event, wxT("OnDeleteAllItems"));
}

void MyListCtrl::OnSelected(wxListEvent& event)
{
    LogEvent(event, wxT("OnSelected"));

    if ( GetWindowStyle() & wxLC_REPORT )
    {
        wxListItem info;
        info.m_itemId = event.m_itemIndex;
		    

		wxString idString = GetItemText(info, 0);
		int indexToSet = 0;
		indexToSet = info.m_itemId;
		/*if( !m_frame->pgnLoad.filter )
		{
			indexToSet = info.m_itemId;
		}
		else
		{
			indexToSet = atoi(idString.c_str().AsChar());
		}*/

		if( (m_frame->pgnLoad.pgn_load_mode == 0))
		{
			m_frame->SetCurrentGame(indexToSet-1);
			m_frame->selectedDBItem = indexToSet-1;
		}
		/*****
		Owner Note: 
		This code takes the integer, Subtracts 1,
		The same is true for load mode=0, this can be ||
		The list was created with index+1
		The function that loads from VM was called with index++
		
		All this could affect the Heap Sort, because Heap Sort
		needs Array entry 0 to be unused ... Therefore the important
		thing to realize is how the indexes are accessed.
		*/
		else if(m_frame->pgnLoad.pgn_load_mode == 1)
		{
			if( m_frame->pgnLoad.SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
			{
				m_frame->SetCurrentGame(indexToSet-1);
				m_frame->selectedDBItem = indexToSet-1;
			}
			else
			{
				m_frame->SetCurrentGame(indexToSet);
				m_frame->selectedDBItem = indexToSet;
			}
		}
		else if( m_frame->pgnLoad.pgn_load_mode == 2 )
		{
			m_frame->SetCurrentGame(info.m_itemId+1);
			m_frame->selectedDBItem = info.m_itemId+1;
		}
		else if (m_frame->pgnLoad.pgn_load_mode == 3)
		{
		/*	m_frame->SetCurrentGame(info.m_itemId);
			m_frame->selectedDBItem = info.m_itemId;
			*/
			if( m_frame->pgnLoad.SORTABLE_INDEX_MODE == SORTABLE_GAME_INDEX_MODE_RAM )
			{
				m_frame->SetCurrentGame(indexToSet-1);
				m_frame->selectedDBItem = indexToSet-1;
			}
			else
			{
				m_frame->SetCurrentGame(indexToSet);
				m_frame->selectedDBItem = indexToSet;
			}
		}

        info.m_col = 1;
        info.m_mask = wxLIST_MASK_TEXT;
		info.SetBackgroundColour(*wxLIGHT_GREY);
        if ( GetItem(info) )
        {
            wxLogMessage(wxT("Value of the 2nd field of the selected item: %s"),
                         info.m_text.c_str());
        }
        else
        {
            wxFAIL_MSG(wxT("wxListCtrl::GetItem() failed"));
        }
    }
}

void MyListCtrl::OnDeselected(wxListEvent& event)
{
    LogEvent(event, wxT("OnDeselected"));
}

void MyListCtrl::OnActivated(wxListEvent& event)
{
    LogEvent(event, wxT("OnActivated"));
}

void MyListCtrl::OnFocused(wxListEvent& event)
{
    LogEvent(event, wxT("OnFocused"));

    event.Skip();
}

void MyListCtrl::OnListKeyDown(wxListEvent& event)
{
    long item;

    switch ( event.GetKeyCode() )
    {
        case 'C': // colorize
            {
                wxListItem info;
                info.m_itemId = event.GetIndex();
                if ( info.m_itemId == -1 )
                {
                    // no item
                    break;
                }

                GetItem(info);

                wxListItemAttr *attr = info.GetAttributes();
                if ( !attr || !attr->HasTextColour() )
                {
                    info.SetTextColour(*wxCYAN);

                    SetItem(info);

                    RefreshItem(info.m_itemId);
                }
            }
            break;

        case 'N': // next
            item = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_FOCUSED);
            if ( item++ == GetItemCount() - 1 )
            {
                item = 0;
            }

            wxLogMessage(wxT("Focusing item %ld"), item);

            SetItemState(item, wxLIST_STATE_FOCUSED, wxLIST_STATE_FOCUSED);
            EnsureVisible(item);
            break;

        case 'R': // show bounding rectangle
            {
                item = event.GetIndex();
                wxRect r;
                if ( !GetItemRect(item, r) )
                {
                    wxLogError(wxT("Failed to retrieve rect of item %ld"), item);
                    break;
                }

                wxLogMessage(wxT("Bounding rect of item %ld is (%d, %d)-(%d, %d)"),
                             item, r.x, r.y, r.x + r.width, r.y + r.height);
            }
            break;

        case '1': // show sub item bounding rectangle for the given column
        case '2': // (and icon/label rectangle if Shift/Ctrl is pressed)
        case '3':
        case '4': // this column is invalid but we want to test it too
            if ( InReportView() )
            {
                int subItem = event.GetKeyCode() - '1';
                item = event.GetIndex();
                wxRect r;

                int code = wxLIST_RECT_BOUNDS;
                if ( wxGetKeyState(WXK_SHIFT) )
                    code = wxLIST_RECT_ICON;
                else if ( wxGetKeyState(WXK_CONTROL) )
                    code = wxLIST_RECT_LABEL;

				
                if ( !GetSubItemRect(item, subItem, r, code) )
                {
                    wxLogError(wxT("Failed to retrieve rect of item %ld column %d"), item, subItem + 1);
                    break;
                }

                wxLogMessage(wxT("Bounding rect of item %ld column %d is (%d, %d)-(%d, %d)"),
                             item, subItem + 1,
                             r.x, r.y, r.x + r.width, r.y + r.height);
            }
            break;

        case 'U': // update
            if ( !IsVirtual() )
                break;

            if ( m_updated != -1 )
                RefreshItem(m_updated);

            m_updated = event.GetIndex();
            if ( m_updated != -1 )
            {
                // we won't see changes to this item as it's selected, update
                // the next one (or the first one if we're on the last item)
                if ( ++m_updated == GetItemCount() )
                    m_updated = 0;

                wxLogMessage("Updating colour of the item %ld", m_updated);
                RefreshItem(m_updated);
            }
            break;

        case 'D': // delete
            item = GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            while ( item != -1 )
            {
                DeleteItem(item);

                wxLogMessage(wxT("Item %ld deleted"), item);

                // -1 because the indices were shifted by DeleteItem()
                item = GetNextItem(item - 1,
                                   wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
            }
            break;

        case 'I': // insert
            if ( GetWindowStyle() & wxLC_REPORT )
            {
                if ( GetWindowStyle() & wxLC_VIRTUAL )
                {
                    SetItemCount(GetItemCount() + 1);
                }
                else // !virtual
                {
                    InsertItemInReportView(event.GetIndex());
                }
            }
            //else: fall through

        default:
            LogEvent(event, wxT("OnListKeyDown"));

            event.Skip();
    }
}

void MyListCtrl::OnChar(wxKeyEvent& event)
{
    wxLogMessage(wxT("Got char event."));

    switch ( event.GetKeyCode() )
    {
        case 'n':
        case 'N':
        case 'c':
        case 'C':
        case 'r':
        case 'R':
        case 'u':
        case 'U':
        case 'd':
        case 'D':
        case 'i':
        case 'I':
            // these are the keys we process ourselves
            break;

        default:
            event.Skip();
    }
}

void MyListCtrl::OnRightClick(wxMouseEvent& event)
{
    if ( !event.ControlDown() )
    {
        event.Skip();
        return;
    }

    int flags;
    long subitem;
    long item = HitTest(event.GetPosition(), flags, &subitem);

    wxString where;
    switch ( flags )
    {
        case wxLIST_HITTEST_ABOVE: where = wxT("above"); break;
        case wxLIST_HITTEST_BELOW: where = wxT("below"); break;
        case wxLIST_HITTEST_NOWHERE: where = wxT("nowhere near"); break;
        case wxLIST_HITTEST_ONITEMICON: where = wxT("on icon of"); break;
        case wxLIST_HITTEST_ONITEMLABEL: where = wxT("on label of"); break;
        case wxLIST_HITTEST_ONITEMRIGHT: where = wxT("right on"); break;
        case wxLIST_HITTEST_TOLEFT: where = wxT("to the left of"); break;
        case wxLIST_HITTEST_TORIGHT: where = wxT("to the right of"); break;
        default: where = wxT("not clear exactly where on"); break;
    }

    wxLogMessage(wxT("Right double click %s item %ld, subitem %ld"),
                 where.c_str(), item, subitem);
}

void MyListCtrl::LogEvent(const wxListEvent& event, const wxChar *eventName)
{
    wxLogMessage(wxT("Item %ld: %s (item text = %s, data = %ld)"),
                 event.GetIndex(), eventName,
                 event.GetText().c_str(), event.GetData());
}

wxString MyListCtrl::OnGetItemText(long item, long column) const
{
 /*   if ( GetItemCount() == WXSIZEOF(SMALL_VIRTUAL_VIEW_ITEMS) )
    {
        return SMALL_VIRTUAL_VIEW_ITEMS[item][column];
    }
    else // "big" virtual control
   */ 
	//	m_listCtrl->InsertColumn(0, wxT("Number") );
	/*	m_listCtrl->InsertColumn(1, wxT("White"));
		m_listCtrl->InsertColumn(2, wxT("Black"));
		m_listCtrl->InsertColumn(3, wxT("Event"));
		m_listCtrl->InsertColumn(4, wxT("Date"));
		m_listCtrl->InsertColumn(5, wxT("Result"));
		m_listCtrl->InsertColumn(6, wxT("Termination"));	
	*/

	return m_frame->pgnLoad.GetRowColumnData( item, column );

}

int MyListCtrl::OnGetItemColumnImage(long item, long column) const
{
    if (!column)
        return 0;

    if (!(item % 3) && column == 1)
        return 0;

    return -1;
}

wxListItemAttr *MyListCtrl::OnGetItemAttr(long item) const
{
    // test to check that RefreshItem() works correctly: when m_updated is
    // set to some item and it is refreshed, we highlight the item
    if ( item == m_updated )
    {
        static wxListItemAttr s_attrHighlight(*wxRED, wxNullColour, wxNullFont);
        return &s_attrHighlight;
    }

    return item % 2 ? NULL : (wxListItemAttr *)&m_attr;
}

void MyListCtrl::InsertItemInReportView(int i)
{
    wxString buf;
    buf.Printf(wxT("This is item %d"), i);
    long tmp = InsertItem(i, buf, 0);
    SetItemData(tmp, i);

    buf.Printf(wxT("Col 1, item %d"), i);
    SetItem(tmp, 1, buf);

    buf.Printf(wxT("Item %d in column 2"), i);
    SetItem(tmp, 2, buf);
}

void MyListCtrl::InsertItemInReportView(int i, wxString str1, wxString str2, wxString str3, wxString str4,wxString str5, wxString str6)
{
	char buff[200];
	_itoa(i+1, buff, 10);
	long tmp = InsertItem(i,buff,0);
	SetItemData(tmp, i);
	//tmp = InsertItem(i, str1, 0);
	SetItem(tmp, 1, str1);
	SetItem(tmp, 2, str2);
	SetItem(tmp, 3, str3);
	SetItem(tmp, 4, str4);
	SetItem(tmp, 5, str5);
	SetItem(tmp, 6, str6);
}

#if USE_CONTEXT_MENU
void MyListCtrl::OnContextMenu(wxContextMenuEvent& event)
{
    if (GetEditControl() == NULL)
    {
        wxPoint point = event.GetPosition();
        // If from keyboard
        if ( (point.x == -1) && (point.y == -1) )
        {
            wxSize size = GetSize();
            point.x = size.x / 2;
            point.y = size.y / 2;
        }
        else
        {
            point = ScreenToClient(point);
        }
        ShowContextMenu(point);
    }
    else
    {
        // the user is editing:
        // allow the text control to display its context menu
        // if it has one (it has on Windows) rather than display our one
        event.Skip();
    }
}
#endif

void MyListCtrl::ShowContextMenu(const wxPoint& pos)
{
    wxMenu menu;

    menu.Append(wxID_ABOUT, wxT("&About"));
    menu.AppendSeparator();
    menu.Append(wxID_EXIT, wxT("E&xit"));

    PopupMenu(&menu, pos.x, pos.y);
}
