#ifndef NOTATION_CANVAS_H
#define NOTATION_CANVAS_H

#include "headers.h"
#include "pgnload.h"

// what do we show on screen (there are too many shapes to put them all on
// screen simultaneously)
enum ScreenToShow
{
    Show_Default,
    Show_Text,
    Show_Lines,
    Show_Brushes,
    Show_Polygons,
    Show_Mask,
    Show_Ops,
    Show_Regions,
    Show_Circles,
    Show_Splines,
#if wxUSE_GRAPHICS_CONTEXT
    Show_Alpha,
#endif
    Show_Gradient,
    Show_Max
};


class MyFrame;
class MyPipePanel;

class Line;


enum DISPLAY_NODE_TYPE
{
	DISP_BASE,
	TEXT_DISP_NODE,
	CODE_FOLD_DISP_NODE,
	DIAGRAM_DISP_NODE,
	MARKER_DISP_NODE,
	BRACKET_DISP_NODE,
	ANNOTATION_DISP_NODE,
	NUMBER_ONLY_DISP_NODE,
};

enum TEXT_DISP_NODE_STYLE
{
	TEXT_DISP_NORMAL,
	TEXT_DISP_BOLD,
	TEXT_DISP_ITALIC,
};

class DisplayNode
{
public:

	DisplayNode()
	{
		pMove = NULL;
		bWhiteBlack = 0;
	}

	~DisplayNode()
	{
	}


	virtual wxString GetText()
	{
		return wxString(text.c_str());
	}

	void SetWhiteBlack(int num )
	{
//		if( num == 1 )
			bWhiteBlack = num;

	}

	void SetRect( int x, int y, int w, int h)
	{
		m_rect.width = w;
		m_rect.height = h;
		m_rect.x = x;
		m_rect.y = y;
	}

	void SetMovePtr( PGN::Move* ptr)
	{
		pMove = ptr;
	}

	void SetLine(Line* pLine)
	{
		m_pLine = pLine;
	}

	void SetText(const char* buff)
	{
		text = buff;
	}

	virtual int GetType()
	{
		return DISP_BASE;
	}

	virtual void SetStyle( int style )
	{
	}

	virtual int GetStyle()
	{
		return 0;
	}
	virtual void SetState( int st )
	{
		//m_state = st;
	}

	virtual int GetState()
	{
		return 0;
	}

	void SetImageID(int id)
	{
	}

	virtual int GetImageID()
	{
		return 0;
	}

	void SetType( int type )
	{
		m_type = type;
	}

	void SetColour(wxColour &col)
	{
		m_color = col;
	}

	void SetColour(int r, int g, int b)
	{
		m_color.Set(r,g,b,255);
	}

	wxColour& GetColor()
	{
		return m_color;
	}


	wxColour m_color;
	string text;
	int bWhiteBlack;
	Line* m_pLine;
	wxRect m_rect;
	bool bSelected;
	PGN::Move* pMove;
	int m_type;
};

class TextDisplayNode : public DisplayNode
{
public:
	TextDisplayNode()
	{
		m_style = TEXT_DISP_NORMAL;
		m_type = TEXT_DISP_NODE;
	}
	~TextDisplayNode(){}



	wxString GetText()
	{
		return wxString(text.c_str());
	}

	int GetType()
	{
		return m_type;
	}

	void SetStyle( int style )
	{
		m_style = style;
	}

	int GetStyle()
	{
		return m_style;
	}

	

	int m_style;
};


class AnnotationDislayNode : public TextDisplayNode
{
public:
	AnnotationDislayNode():TextDisplayNode()
	{
		m_style = TEXT_DISP_NORMAL;
		m_type = ANNOTATION_DISP_NODE;
	}
	~AnnotationDislayNode(){}
};



class ImageDisplayNode : public DisplayNode
{
public:

	ImageDisplayNode(){}
	~ImageDisplayNode(){}

	void SetImageID(int id)
	{
		imgID = id;
	}
	int GetImageID()
	{
		return imgID;
	}
	int imgID;
};

class CodeFoldNode : public ImageDisplayNode
{
public:

	CodeFoldNode(){}
	~CodeFoldNode(){}

	void SetImageID2(int id)
	{
		imgID2 = id;
	}
	int GetImageID2()
	{
		return imgID2;
	}

	void SetState(int st)
	{
		state = st;
	}

	int GetState()
	{
		return state;
	}
	int state;
	int imgID2;
};

class CodeFold : public DisplayNode
{
public:

	CodeFold(){
		m_state = 0;
		m_type = CODE_FOLD_DISP_NODE;
	}
	~CodeFold(){}

	void SetState( int st )
	{
		m_state = st;
	}

	int GetState()
	{
		return m_state;
	}

	int GetType()
	{
		return m_type;
	}
	int m_state;
};



class Line
{
public:

	Line()
	{
		text = "";
	}

	~Line()
	{
		for( int i = 0; i < nodes.size(); i++ )
		{
			if( nodes[i] )
			{
				delete nodes[i];
				nodes[i] = NULL;
			}
		}
		nodes.clear();
	}
	void AddNode( DisplayNode * newNode )
	{
		nodes.push_back(newNode);
	}

	void AddNode( char* text, int whiteBlack, wxRect &rct )
	{
		TextDisplayNode *t = new TextDisplayNode();
		t->SetText( text );
		t->SetWhiteBlack(whiteBlack);
		t->SetRect(rct.x, rct.y, rct.width, rct.height );
		nodes.push_back(t);
	}

	void BuildText()
	{
		for( int i = 0; i < nodes.size(); i++ )
		{
			text+= nodes[i]->text;
		}
	}
	string text;
	vector<DisplayNode*> nodes;

};

class Page
{
public:

	Page()
	{
	}

	~Page()
	{
		Clear();
	}
	void Clear()
	{
		for( int i = 0; i < lines.size(); i++ )
		{
			if( lines[i] )
			{
				delete lines[i];
				lines[i] = NULL;
			}

		}
		lines.clear();
	}

	void MergeEqualNodesInLines()
	{
		for( int i = 0; i < lines.size(); i++ )
		{
			for( int j = 0; j < lines[i]->nodes.size(); j++ )
			{
				for( int k = 0; k < lines[i]->nodes.size(); k++ )
				{
					if( j != k )
					{
						if( lines[i]->nodes[j]->m_rect == lines[i]->nodes[k]->m_rect )
						{
							lines[i]->nodes.erase( lines[i]->nodes.begin()+k,lines[i]->nodes.begin()+k+1);
						}
					}
				}
			}
		}
	}

	void AddLine( Line* newLine )
	{
		lines.push_back(newLine);
	}
	vector<Line*> lines;
};


// define a scrollable canvas for drawing onto
class MyCanvas: public wxScrolledWindow
{
public:
	MyCanvas( MyFrame* parent, wxWindow *owner);
    MyCanvas( MyFrame *parent );


	void MyCanvas::LogEvent(const wxString& name, wxKeyEvent& event);
	void ResetBoard();

    void OnPaint(wxPaintEvent &event);
	void OnIdle(wxIdleEvent &event);
	void OnEraseBackground(wxEraseEvent& event){}
    void OnMouseMove(wxMouseEvent &event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	//void OnScroll(wxScollEvent &event);

    void ToShow(ScreenToShow show) { m_show = show; Refresh(); }

	void AddActiveEngine(MyPipePanel *pPanel)
	{
		activeEngines.push_back(pPanel);
	}

    // set or remove the clipping region
    void Clip(bool clip) { m_clip = clip; Refresh(); }
#if wxUSE_GRAPHICS_CONTEXT
    void UseGraphicContext(bool use) { m_useContext = use; Refresh(); }

#endif

	void ScrollToCursor()
	{
		MoveToCursor = true;
	}

	void HandleRightKey();
	void HandleLeftKey();


protected:
	int DrawVariation(int &currPos, int *depth, int height, 
						PGN::Move *pMove, wxDC& dc, wxSize *sz);

 int DrawVariationBlack(int &currPos, int *depth, int height,
	 PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset, char start = '[', char end = ']');
 int DrawVariationWhite(int &currPos, int *depth, int height,
							PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset, char start = '[', char end = ']');
 int DrawAnnotationWhite(int &currPos, int *depth, int height,
	 PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset);    
 int DrawAnnotationBlack(int &currPos, int *depth, int height,
	 PGN::Move *pMove, wxDC& dc, wxSize *sz, int offset);  

	void ScrollTest(int depth, wxSize *sz);
 
	void DrawTestLines( int x, int y, int width, wxDC &dc );
    void DrawTestPoly(wxDC& dc);
    void DrawTestBrushes(wxDC& dc);
    void DrawText(wxDC& dc);
    void DrawImages(wxDC& dc);
    //void DrawWithLogicalOps(wxDC& dc);
#if wxUSE_GRAPHICS_CONTEXT
    void DrawAlpha(wxDC& dc);
	void DrawGraphics(wxGraphicsContext* gc);
#endif
    void DrawRegions(wxDC& dc);
    void DrawCircles(wxDC& dc);
    void DrawSplines(wxDC& dc);
    void DrawDefault(wxDC& dc);
    void DrawGradients(wxDC& dc);

    void DrawRegionsHelper(wxDC& dc, wxCoord x, bool firstTime);

	
	void SetMoveColor(wxCommandEvent& evt);
	void SetVariationColor(wxCommandEvent& evt);
	void PromoteVariation(wxCommandEvent& evt);
	void DeleteVariation(wxCommandEvent& evt);
	void DeleteRemainingMoves(wxCommandEvent& evt);
	void AddAnnotation(wxCommandEvent& evt);
	void OnAddGlyph(wxCommandEvent& evt);
	void OnToggleAllVariationFolds(wxCommandEvent& evt);
	//void OnSetGlyph(wxCommandEvent& evt);

	void OnAddGlyphExclamation(wxCommandEvent& evt);
	void OnSetGlyphDoubleExclamation(wxCommandEvent& evt);
	void OnAddGlyphExclamationQuestion(wxCommandEvent& evt);
	void OnSetGlyphQuestion(wxCommandEvent& evt);
	void OnAddGlyphDoubleQuestion(wxCommandEvent& evt);
	void OnSetGlyphQuestionExclamation(wxCommandEvent& evt);
	
	void OnKeyDown(wxKeyEvent& event) { LogEvent("KeyDown", event); }
    void OnKeyUp(wxKeyEvent& event) { LogEvent("KeyUp", event); }
	void OnChar(wxKeyEvent& event) { LogEvent("Char", event); }

	void OnSize(wxSizeEvent& WXUNUSED(evt))
    {
		needsRefresh = true;
        Refresh();
    }





private:
public:
	void SetAnnotateTextColor(wxColour &AnnotateTextColor)
	{
		annotateTextColor = AnnotateTextColor;
	}

	void SetFonts(wxFont norm, wxFont bold, wxFont italic);
	void SetTextViewportBackground(wxColor col);

    vector<MyPipePanel*> activeEngines;
	Page m_dataPage;
	bool m_buildDataPage;
	Line *currLine;
	TextDisplayNode *currDataNode;
	bool needsRefresh;

	bool rememberedScrollPos;
	int remXX;
	int remYY;

	//wxBitmap svgTextures[10];

		//svgTextures[0]->LoadFile("images/PieceThemes/Internet/bp.png", wxBITMAP_TYPE_PNG);
	
	wxFont fontBold;
	wxFont fontItalic;
	wxFont fontNormal;
	int fontSize;

	wxColour annotateTextColor;

	vector<TextDisplayNode*> page; 
	int line_height;
	MyFrame *m_owner;
	int move_count;
	int selectedMove;
	int white_black_move;
private:

	int scrollX;
	int scrollY;
	bool bCheckVirtualSize;
	int XX, YY;
	int w, h;
    ScreenToShow m_show;
    wxBitmap     m_smile_bmp;
    wxIcon       m_std_icon;
    bool         m_clip;
#if wxUSE_GRAPHICS_CONTEXT
    bool         m_useContext ;

	bool MoveToCursor;
	
	//wxPanel *m_panel;

#endif
	

	wxBitmap *gs_bmp4;

    DECLARE_EVENT_TABLE()
};

#endif