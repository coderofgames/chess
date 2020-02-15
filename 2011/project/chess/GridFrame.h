#ifndef GRID_FRAME_H
#define GRID_FRAME_H

#include "headers.h"

class MyFrame;

class GridFrame : public wxScrolledWindow
{
public:

	MyFrame *m_frame;

	GridFrame(wxFrame *mParent);
	~GridFrame();


	
	wxGrid *m_Grid;

    enum
    {
        ID_TOGGLEROWLABELS = 100,
        ID_TOGGLECOLLABELS,
        ID_TOGGLEEDIT,
        ID_TOGGLEROWSIZING,
        ID_TOGGLECOLSIZING,
        ID_TOGGLECOLMOVING,
        ID_TOGGLEGRIDSIZING,
        ID_TOGGLEGRIDDRAGCELL,
        ID_TOGGLEGRIDLINES,
        ID_AUTOSIZECOLS,
        ID_CELLOVERFLOW,
        ID_RESIZECELL,
        ID_SETLABELCOLOUR,
        ID_SETLABELTEXTCOLOUR,
        ID_SETLABEL_FONT,
        ID_ROWLABELALIGN,
        ID_ROWLABELHORIZALIGN,
        ID_ROWLABELVERTALIGN,
        ID_COLLABELALIGN,
        ID_COLLABELHORIZALIGN,
        ID_COLLABELVERTALIGN,
        ID_COLDEFAULTHEADER,
        ID_COLNATIVEHEADER,
        ID_COLCUSTOMHEADER,
        ID_GRIDLINECOLOUR,
        ID_INSERTROW,
        ID_INSERTCOL,
        ID_DELETEROW,
        ID_DELETECOL,
        ID_CLEARGRID,
        ID_CHANGESEL,
        ID_SELCELLS,
        ID_SELROWS,
        ID_SELCOLS,
        ID_SELROWSORCOLS,
        ID_SET_CELL_FG_COLOUR,
        ID_SET_CELL_BG_COLOUR,
        ID_VTABLE,
        ID_BUGS_TABLE,
        ID_TABULAR_TABLE,
        ID_SELECT_UNSELECT,
        ID_SHOW_SELECTION,
        ID_SELECT_ALL,
        ID_SELECT_ROW,
        ID_SELECT_COL,
        ID_SELECT_CELL,
        ID_DESELECT_ALL,
        ID_DESELECT_ROW,
        ID_DESELECT_COL,
        ID_DESELECT_CELL,
        ID_SIZE_ROW,
        ID_SIZE_COL,
        ID_SIZE_ROW_LABEL,
        ID_SIZE_COL_LABEL,
        ID_SIZE_LABELS_COL,
        ID_SIZE_LABELS_ROW,
        ID_SIZE_GRID,

        ID_SET_HIGHLIGHT_WIDTH,
        ID_SET_RO_HIGHLIGHT_WIDTH,

        ID_TESTFUNC
    };

	DECLARE_EVENT_TABLE()
};

class MyGridCellRenderer : public wxGridCellStringRenderer
{
public:
    virtual void Draw(wxGrid& grid,
                      wxGridCellAttr& attr,
                      wxDC& dc,
                      const wxRect& rect,
                      int row, int col,
                      bool isSelected);
};

// ----------------------------------------------------------------------------
// an example of custom attr provider: this one makes all odd rows appear grey
// ----------------------------------------------------------------------------

class MyGridCellAttrProvider : public wxGridCellAttrProvider
{
public:
    MyGridCellAttrProvider();
    virtual ~MyGridCellAttrProvider();

    virtual wxGridCellAttr *GetAttr(int row, int col,
                                    wxGridCellAttr::wxAttrKind  kind) const;

private:
    wxGridCellAttr *m_attrForOddRows;
};

#endif