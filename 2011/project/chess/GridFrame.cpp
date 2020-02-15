#include "GridFrame.h"
#include "mdi.h"
#include "PgnLoad.h"


// Custom renderer that renders column header cells without borders and in
// italic
class CustomColumnHeaderRenderer : public wxGridColumnHeaderRenderer
{
public:
    CustomColumnHeaderRenderer(const wxColour& colFg, const wxColour& colBg)
        : m_colFg(colFg),
          m_colBg(colBg)
    {
    }

    virtual void DrawLabel(const wxGrid& WXUNUSED(grid),
                           wxDC& dc,
                           const wxString& value,
                           const wxRect& rect,
                           int horizAlign,
                           int vertAlign,
                           int WXUNUSED(textOrientation)) const
    {
        dc.SetTextForeground(m_colFg);
        dc.SetFont(wxITALIC_FONT->Bold());
        dc.DrawLabel(value, rect, horizAlign | vertAlign);
    }

    virtual void DrawBorder(const wxGrid& WXUNUSED(grid),
                            wxDC& dc,
                            wxRect& rect) const
    {
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.SetBrush(wxBrush(m_colBg));
        dc.DrawRectangle(rect);
    }

private:
    const wxColour m_colFg, m_colBg;

    wxDECLARE_NO_COPY_CLASS(CustomColumnHeaderRenderer);
};

// And a custom attributes provider which uses custom column header renderer
// defined above
class CustomColumnHeadersProvider : public wxGridCellAttrProvider
{
public:
    // by default custom column renderer is not used, call
    // UseCustomColHeaders() to enable it
    CustomColumnHeadersProvider()
        : m_customOddRenderer(*wxYELLOW, *wxBLUE),
          m_customEvenRenderer(*wxWHITE, *wxBLACK),
          m_useCustom(false)
    {
    }

    // enable or disable the use of custom renderer for column headers
    void UseCustomColHeaders(bool use = true) { m_useCustom = use; }

protected:
    virtual const wxGridColumnHeaderRenderer& GetColumnHeaderRenderer(int col)
    {
        // if enabled, use custom renderers
        if ( m_useCustom )
        {
            // and use different ones for odd and even columns -- just to show
            // that we can
            return col % 2 ? m_customOddRenderer : m_customEvenRenderer;
        }

        return wxGridCellAttrProvider::GetColumnHeaderRenderer(col);
    }

private:
    CustomColumnHeaderRenderer m_customOddRenderer,
                               m_customEvenRenderer;

    bool m_useCustom;

    wxDECLARE_NO_COPY_CLASS(CustomColumnHeadersProvider);
};

// ----------------------------------------------------------------------------
// MyGridCellRenderer
// ----------------------------------------------------------------------------

// do something that the default renderer doesn't here just to show that it is
// possible to alter the appearance of the cell beyond what the attributes
// allow
void MyGridCellRenderer::Draw(wxGrid& grid,
                              wxGridCellAttr& attr,
                              wxDC& dc,
                              const wxRect& rect,
                              int row, int col,
                              bool isSelected)
{
    wxGridCellStringRenderer::Draw(grid, attr, dc, rect, row, col, isSelected);

    dc.SetPen(*wxGREEN_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.DrawEllipse(rect);
}

// ----------------------------------------------------------------------------
//m_GridFrame
// ----------------------------------------------------------------------------

BEGIN_EVENT_TABLE(GridFrame, wxScrolledWindow )

/*
    EVT_GRID_LABEL_LEFT_CLICK(m_GridFrame::OnLabelLeftClick )
    EVT_GRID_CELL_LEFT_CLICK(m_GridFrame::OnCellLeftClick )
    EVT_GRID_ROW_SIZE(m_GridFrame::OnRowSize )
    EVT_GRID_COL_SIZE(m_GridFrame::OnColSize )
    EVT_GRID_SELECT_CELL(m_GridFrame::OnSelectCell )
    EVT_GRID_RANGE_SELECT(m_GridFrame::OnRangeSelected )
    EVT_GRID_CELL_CHANGING(m_GridFrame::OnCellValueChanging )
    EVT_GRID_CELL_CHANGED(m_GridFrame::OnCellValueChanged )
    EVT_GRID_CELL_BEGIN_DRAG(m_GridFrame::OnCellBeginDrag )

    EVT_GRID_EDITOR_SHOWN(m_GridFrame::OnEditorShown )
    EVT_GRID_EDITOR_HIDDEN(m_GridFrame::OnEditorHidden )*/
END_EVENT_TABLE()



GridFrame::GridFrame(wxFrame* mParent)
        : wxScrolledWindow( (wxFrame *)mParent, wxID_ANY,
                   wxDefaultPosition,
                   wxDefaultSize )
{

	m_frame = (MyFrame*)mParent;
   // SetIcon(wxICON(sample));

   /* wxMenu *fileMenu = new wxMenu;
    fileMenu->Append( ID_VTABLE, wxT("&Virtual table test\tCtrl-V"));
    fileMenu->Append( ID_BUGS_TABLE, wxT("&Bugs table test\tCtrl-B"));
    fileMenu->Append( ID_TABULAR_TABLE, wxT("&Tabular table test\tCtrl-T"));
    fileMenu->AppendSeparator();
    fileMenu->Append( wxID_EXIT, wxT("E&xit\tAlt-X") );

    wxMenu *viewMenu = new wxMenu;
    viewMenu->AppendCheckItem(ID_TOGGLEROWLABELS, "&Row labels");
    viewMenu->AppendCheckItem(ID_TOGGLECOLLABELS, "&Col labels");
    viewMenu->AppendCheckItem(ID_TOGGLEEDIT,"&Editable");
    viewMenu->AppendCheckItem(ID_TOGGLEROWSIZING, "Ro&w drag-resize");
    viewMenu->AppendCheckItem(ID_TOGGLECOLSIZING, "C&ol drag-resize");
    viewMenu->AppendCheckItem(ID_TOGGLECOLMOVING, "Col drag-&move");
    viewMenu->AppendCheckItem(ID_TOGGLEGRIDSIZING, "&Grid drag-resize");
    viewMenu->AppendCheckItem(ID_TOGGLEGRIDDRAGCELL, "&Grid drag-cell");
    viewMenu->AppendCheckItem(ID_TOGGLEGRIDLINES, "&Grid Lines");
    viewMenu->AppendCheckItem(ID_SET_HIGHLIGHT_WIDTH, "&Set Cell Highlight Width...");
    viewMenu->AppendCheckItem(ID_SET_RO_HIGHLIGHT_WIDTH, "&Set Cell RO Highlight Width...");
    viewMenu->AppendCheckItem(ID_AUTOSIZECOLS, "&Auto-size cols");
    viewMenu->AppendCheckItem(ID_CELLOVERFLOW, "&Overflow cells");
    viewMenu->AppendCheckItem(ID_RESIZECELL, "&Resize cell (7,1)");

    wxMenu *rowLabelMenu = new wxMenu;

    viewMenu->Append( ID_ROWLABELALIGN, wxT("R&ow label alignment"),
                      rowLabelMenu,
                      wxT("Change alignment of row labels") );

    rowLabelMenu->AppendRadioItem( ID_ROWLABELHORIZALIGN, wxT("&Horizontal") );
    rowLabelMenu->AppendRadioItem( ID_ROWLABELVERTALIGN, wxT("&Vertical") );

    wxMenu *colLabelMenu = new wxMenu;

    viewMenu->Append( ID_COLLABELALIGN, wxT("Col l&abel alignment"),
                      colLabelMenu,
                      wxT("Change alignment of col labels") );

    colLabelMenu->AppendRadioItem( ID_COLLABELHORIZALIGN, wxT("&Horizontal") );
    colLabelMenu->AppendRadioItem( ID_COLLABELVERTALIGN, wxT("&Vertical") );

    wxMenu *colHeaderMenu = new wxMenu;

    viewMenu->Append( ID_ROWLABELALIGN, wxT("Col header style"),
                      colHeaderMenu,
                      wxT("Change style of col header") );

    colHeaderMenu->AppendRadioItem( ID_COLDEFAULTHEADER, wxT("&Default") );
    colHeaderMenu->AppendRadioItem( ID_COLNATIVEHEADER, wxT("&Native") );
    colHeaderMenu->AppendRadioItem( ID_COLCUSTOMHEADER, wxT("&Custom") );


    wxMenu *colMenu = new wxMenu;
    colMenu->Append( ID_SETLABELCOLOUR, wxT("Set &label colour...") );
    colMenu->Append( ID_SETLABELTEXTCOLOUR, wxT("Set label &text colour...") );
    colMenu->Append( ID_SETLABEL_FONT, wxT("Set label fo&nt...") );
    colMenu->Append( ID_GRIDLINECOLOUR, wxT("&Grid line colour...") );
    colMenu->Append( ID_SET_CELL_FG_COLOUR, wxT("Set cell &foreground colour...") );
    colMenu->Append( ID_SET_CELL_BG_COLOUR, wxT("Set cell &background colour...") );

    wxMenu *editMenu = new wxMenu;
    editMenu->Append( ID_INSERTROW, wxT("Insert &row") );
    editMenu->Append( ID_INSERTCOL, wxT("Insert &column") );
    editMenu->Append( ID_DELETEROW, wxT("Delete selected ro&ws") );
    editMenu->Append( ID_DELETECOL, wxT("Delete selected co&ls") );
    editMenu->Append( ID_CLEARGRID, wxT("Cl&earm_Grid cell contents") );

    wxMenu *selectMenu = new wxMenu;
    selectMenu->Append( ID_SELECT_UNSELECT, wxT("Add new cells to the selection"),
                        wxT("When off, old selection is deselected before ")
                        wxT("selecting the new cells"), wxITEM_CHECK );
    selectMenu->Append( ID_SHOW_SELECTION,
                        wxT("&Show current selection\tCtrl-Alt-S"));
    selectMenu->AppendSeparator();
    selectMenu->Append( ID_SELECT_ALL, wxT("Select all"));
    selectMenu->Append( ID_SELECT_ROW, wxT("Select row 2"));
    selectMenu->Append( ID_SELECT_COL, wxT("Select col 2"));
    selectMenu->Append( ID_SELECT_CELL, wxT("Select cell (3, 1)"));
    selectMenu->AppendSeparator();
    selectMenu->Append( ID_DESELECT_ALL, wxT("Deselect all"));
    selectMenu->Append( ID_DESELECT_ROW, wxT("Deselect row 2"));
    selectMenu->Append( ID_DESELECT_COL, wxT("Deselect col 2"));
    selectMenu->Append( ID_DESELECT_CELL, wxT("Deselect cell (3, 1)"));
    wxMenu *selectionMenu = new wxMenu;
    selectMenu->Append( ID_CHANGESEL, wxT("Change &selection mode"),
                      selectionMenu,
                      wxT("Change selection mode") );

    selectionMenu->Append( ID_SELCELLS, wxT("Select &cells") );
    selectionMenu->Append( ID_SELROWS, wxT("Select &rows") );
    selectionMenu->Append( ID_SELCOLS, wxT("Select col&umns") );
    selectionMenu->Append( ID_SELROWSORCOLS, wxT("Select rows &or columns") );

    wxMenu *autosizeMenu = new wxMenu;
    autosizeMenu->Append( ID_SIZE_ROW, wxT("Selected &row data") );
    autosizeMenu->Append( ID_SIZE_COL, wxT("Selected &column data") );
    autosizeMenu->Append( ID_SIZE_ROW_LABEL, wxT("Selected row la&bel") );
    autosizeMenu->Append( ID_SIZE_COL_LABEL, wxT("Selected column &label") );
    autosizeMenu->Append( ID_SIZE_LABELS_COL, wxT("Column la&bels") );
    autosizeMenu->Append( ID_SIZE_LABELS_ROW, wxT("Row label&s") );
    autosizeMenu->Append( ID_SIZE_GRID, wxT("Entire &grid") );

    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append( wxID_ABOUT, wxT("&About wxGrid demo") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( fileMenu, wxT("&File") );
    menuBar->Append( viewMenu, wxT("&Grid") );
    menuBar->Append( colMenu,  wxT("&Colours") );
    menuBar->Append( editMenu, wxT("&Edit") );
    menuBar->Append( selectMenu, wxT("&Select") );
    menuBar->Append( autosizeMenu, wxT("&Autosize") );
    menuBar->Append( helpMenu, wxT("&Help") );

    SetMenuBar( menuBar );*/

   // m_addToSel = false;

   m_Grid = new wxGrid( this,
                       wxID_ANY,
                       wxPoint( 0, 0 ),
                       wxSize( 400, 300 ) );




    // this will create am_Grid and, by default, an associatedm_Grid
    // table for strings
   m_Grid->CreateGrid( 0, 0 );

   m_Grid->GetTable()->SetAttrProvider(new CustomColumnHeadersProvider());

   m_Grid->AppendCols(m_frame->pgnLoad.m_filterStage.m_filters.size());

   m_Grid->AppendRows(2);

   for( int i = 0; i < m_frame->pgnLoad.m_filterStage.m_filters.size(); i++ )
   {
	   int type = m_frame->pgnLoad.m_filterStage.m_filters[i]->GetType();
/*#define FILTER_NAME 0
#define FILTER_ELO 1
#define FILTER_ECO 2
#define FILTER_EVENT 3
#define FILTER_SITE 4
#define FILTER_DATE 5
#define FILTER_RESULT 6*/

	   switch( type )
	   {
	   case FILTER_NAME:
		   {
			   m_Grid->SetCellBackgroundColour(0, i, *wxBLUE);
			   m_Grid->SetCellValue( 0, i, wxT("NAME"));
		   }
		   break;
	   case FILTER_ELO:
		   {
			   m_Grid->SetCellBackgroundColour(0, i, *wxGREEN);
			   m_Grid->SetCellValue( 0, i, wxT("Elo"));
		   }
		   break;
	   case FILTER_ECO:
		   {
			   m_Grid->SetCellBackgroundColour(0, i, *wxRED);
			    m_Grid->SetCellValue( 0, i, wxT("Eco"));
		   }
		   break;
	   case FILTER_EVENT:
		   {
			   m_Grid->SetCellBackgroundColour(0, i, *wxYELLOW);
			    m_Grid->SetCellValue( 0, i, wxT("Event"));
		   }
		   break;
	   case FILTER_SITE:
		   {
			   m_Grid->SetCellBackgroundColour(0, i, *wxWHITE);
			    m_Grid->SetCellValue( 0, i, wxT("Site"));
		   }
		   break;
	   case FILTER_DATE:
		   {
			   m_Grid->SetCellBackgroundColour(0, i, *wxLIGHT_GREY);
			    m_Grid->SetCellValue( 0, i, wxT("Date"));
		   }
		   break;
	   case FILTER_RESULT:
		   {
			   m_Grid->SetCellBackgroundColour(0, i, *wxBLUE);
			    m_Grid->SetCellValue( 0, i, wxT("Result"));
		   }
		   break;
	   }
   }
 //  m_Grid->AppendCols(100);

/*    int ir =m_Grid->GetNumberRows();
   m_Grid->DeleteRows(0, ir);
   m_Grid->AppendRows(ir);

   m_Grid->SetRowSize( 0, 60 );
   m_Grid->SetCellValue( 0, 0, wxT("Ctrl+Home\nwill go to\nthis cell") );

   m_Grid->SetCellValue( 0, 1, wxT("A long piece of text to demonstrate wrapping.") );
   m_Grid->SetCellRenderer(0 , 1, new wxGridCellAutoWrapStringRenderer);
   m_Grid->SetCellEditor( 0,  1 , new wxGridCellAutoWrapStringEditor);

   m_Grid->SetCellValue( 0, 2, wxT("Blah") );
   m_Grid->SetCellValue( 0, 3, wxT("Read only") );
   m_Grid->SetReadOnly( 0, 3 );

   m_Grid->SetCellValue( 0, 4, wxT("Can veto edit this cell") );

   m_Grid->SetCellValue( 0, 5, wxT("Press\nCtrl+arrow\nto skip over\ncells") );

   m_Grid->SetRowSize( 99, 60 );
   m_Grid->SetCellValue( 99, 99, wxT("Ctrl+End\nwill go to\nthis cell") );
   m_Grid->SetCellValue( 1, 0, wxT("This default cell will overflow into neighboring cells, but not if you turn overflow off."));

   m_Grid->SetCellTextColour(1, 2, *wxRED);
   m_Grid->SetCellBackgroundColour(1, 2, *wxGREEN);

   m_Grid->SetCellValue( 1, 4, wxT("I'm in the middle"));

   m_Grid->SetCellValue(2, 2, wxT("red"));

   m_Grid->SetCellTextColour(2, 2, *wxRED);
   m_Grid->SetCellValue(3, 3, wxT("green on grey"));
   m_Grid->SetCellTextColour(3, 3, *wxGREEN);
   m_Grid->SetCellBackgroundColour(3, 3, *wxLIGHT_GREY);

   m_Grid->SetCellValue(4, 4, wxT("a weird looking cell"));
   m_Grid->SetCellAlignment(4, 4, wxALIGN_CENTRE, wxALIGN_CENTRE);
   m_Grid->SetCellRenderer(4, 4, new MyGridCellRenderer);

   m_Grid->SetCellRenderer(3, 0, new wxGridCellBoolRenderer);
   m_Grid->SetCellEditor(3, 0, new wxGridCellBoolEditor);

    wxGridCellAttr *attr;
    attr = new wxGridCellAttr;
    attr->SetTextColour(*wxBLUE);
   m_Grid->SetColAttr(5, attr);
    attr = new wxGridCellAttr;
    attr->SetBackgroundColour(*wxRED);
   m_Grid->SetRowAttr(5, attr);

   m_Grid->SetCellValue(2, 4, wxT("a wider column"));
   m_Grid->SetColSize(4, 120);
   m_Grid->SetColMinimalWidth(4, 120);

   m_Grid->SetCellTextColour(5, 8, *wxGREEN);
   m_Grid->SetCellValue(5, 8, wxT("Bg from row attr\nText col from cell attr"));
   m_Grid->SetCellValue(5, 5, wxT("Bg from row attr Text col from col attr and this text is so long that it covers over many many empty cells but is broken by one that isn't"));

   m_Grid->SetColFormatFloat(6);
   m_Grid->SetCellValue(0, 6, wxString::Format(wxT("%g"), 3.1415));
   m_Grid->SetCellValue(1, 6, wxString::Format(wxT("%g"), 1415.0));
   m_Grid->SetCellValue(2, 6, wxString::Format(wxT("%g"), 12345.67890));

   m_Grid->SetColFormatFloat(7, 6, 2);
   m_Grid->SetCellValue(0, 7, wxString::Format(wxT("%g"), 3.1415));
   m_Grid->SetCellValue(1, 7, wxString::Format(wxT("%g"), 1415.0));
   m_Grid->SetCellValue(2, 7, wxString::Format(wxT("%g"), 12345.67890));

   m_Grid->SetColFormatNumber(8);
   m_Grid->SetCellValue(0, 8, "17");
   m_Grid->SetCellValue(1, 8, "0");
   m_Grid->SetCellValue(2, 8, "-666");
   m_Grid->SetCellAlignment(2, 8, wxALIGN_CENTRE, wxALIGN_TOP);
   m_Grid->SetCellValue(2, 9, "<- This numeric cell should be centred");

    const wxString choices[] =
    {
        wxT("Please select a choice"),
        wxT("This takes two cells"),
        wxT("Another choice"),
    };
   m_Grid->SetCellEditor(4, 0, new wxGridCellChoiceEditor(WXSIZEOF(choices), choices));
   m_Grid->SetCellSize(4, 0, 1, 2);
   m_Grid->SetCellValue(4, 0, choices[0]);
   m_Grid->SetCellOverflow(4, 0, false);

   m_Grid->SetCellSize(7, 1, 3, 4);
   m_Grid->SetCellAlignment(7, 1, wxALIGN_CENTRE, wxALIGN_CENTRE);
   m_Grid->SetCellValue(7, 1, wxT("Big box!"));

    // create a separator-like row: it's grey and it's non-resizeable
   m_Grid->DisableRowResize(10);
   m_Grid->SetRowSize(10, 30);
    attr = new wxGridCellAttr;
    attr->SetBackgroundColour(*wxLIGHT_GREY);
   m_Grid->SetRowAttr(10, attr);
   m_Grid->SetCellValue(10, 0, "You can't resize this row interactively -- try it");

    // this does exactly nothing except testing that SetAttr() handles NULL
    // attributes and does reference counting correctly
   m_Grid->SetAttr(11, 11, NULL);
   m_Grid->SetAttr(11, 11, new wxGridCellAttr);
   m_Grid->SetAttr(11, 11, NULL);
   */
    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    topSizer->Add(m_Grid,
                   1,
                   wxEXPAND );



    SetSizerAndFit( topSizer );
	


}


GridFrame::~GridFrame()
{
	delete m_Grid;
}