#ifndef D3D9VARIATION_BOARD_H
#define D3D9VARIATION_BOARD_H

#include "D3D9Base.h"

#include "BaseVariationBoard.h"


class D3D9VariationBoard : public D3D9BaseCanvas, public BaseVariationBoard
{
public:
    D3D9VariationBoard(MyFrame *parent, wxWindow *winParent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("TestGLCanvas"));



    ~D3D9VariationBoard();

	void ResetBoard()
	{
		this->internalBoard.SetInitialPosition();
	}






	


	
protected:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnIdle( wxIdleEvent& event );
	void mouseLeftDown( wxMouseEvent& event );
	void mouseMove( wxMouseEvent& event );
	void mouseReleased( wxMouseEvent& event );

private:
    void InitD3D();
	void CreateManagedResources();
	void CreateQuad();
	void LoadTiles();
	void LoadManagedTextures();
	void CreateLights();
	void CreateMaterials();
    void ResetProjectionMode();
	void ReleaseBoardSquareTextures(){}


	void ChangeTextures(wxString &name);

    void Render();

	void DoMove(bool &promote);

	void CreateLines();


	bool bSelectedPiece;
	int selX, selY;

	MyChessMove move;

	bool bFlip;


	int board[8][8];

	enum{ wPawn=1, wBishop, wKnight, wRook, wQueen, wKing, 
		bPawn, bBishop, bKnight, bRook, bQueen, bKing };

	void SetupBoard();

	LPD3DXFONT CreateFont();

	D3DXVECTOR2 WorldToScreen(D3DXVECTOR3 worldPoint);
							
    MyChessMove *pMov;

	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer;// = NULL;
	LPDIRECT3DTEXTURE9 g_pTexture[8][8];
	LPDIRECT3DTEXTURE9 g_pWhitePieces[6]; //p b n r q k
	LPDIRECT3DTEXTURE9 g_pBlackPieces[6];


	LPDIRECT3DTEXTURE9 border1;
	LPDIRECT3DTEXTURE9 blackTex;
	LPDIRECT3DTEXTURE9 yellowTex;
	LPD3DXLINE ppLine;
	LPD3DXFONT m_font;

	LPDIRECT3DVERTEXBUFFER9 quadLineVB;

	LPDIRECT3DVERTEXBUFFER9 arrowHeadVB;

	D3DXVECTOR3 lines[36];
	D3DXVECTOR3 lineBorder[4];

	//vector< LPDIRECT3DTEXTURE9 > textures;

	LPD3DXSPRITE g_pSprite;

	float  g_fTime;

	

	MyFrame *m_owner;

	
	D3DMATERIAL9 mat1;
	D3DMATERIAL9 mat2;
	D3DMATERIAL9 mat3;
	D3DMATERIAL9 blackMat;
	D3DMATERIAL9 matArrow;
	D3DMATERIAL9 matTxt;
	D3DMATERIAL9 whiteMat;

    DECLARE_EVENT_TABLE()
};

#endif