#include "D3D9Board.h"
#include "MyPipePanel.h"
#include "mdi.h"
#include "PromotionSelectionDialog.h"
#include "OptionsFrame.h"
#include "NotationCanvas.h"
#include "TreeCanvas.h"
#include "GameClock.h"
#include "MoveDataCanvas.h"
#include "NotationFrame.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------

struct TXVertex
{
public:
  float x, y, z;
  DWORD diffuse;
  float u, v;
};

TXVertex Vunit[] = {
  {-1.0,1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 0.0,0.0},
  {1.0,1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 1.0,0.0},
  {1.0,-1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 1.0,1.0},
  {-1.0,-1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 0.0,1.0}
};

TXVertex QuadTXVertexes[] =
{
	{0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0.0f},
	{1.0f, -1.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 1.0f},
	{0.0f, -1.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 1.0f}

};

TXVertex ArrowHeadVertexes[] = 
{

	{0.0f, 0.7f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0.0f},
	{0.3f, -0.5f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 1.0f},
	{0.0f, -1.7f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 1.0f}

};

#define D3DFVF_TXVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )




//vertex data for quad (NEW)

/*Vertex g_cubeVertices[] =
{
	{-1.0f, 1.0f,-1.0f, 0xffff0000, 0.0, 0.0},
	{ 1.0f, 1.0f,-1.0f, 0xffff0000, 0.0, 0.0},
	{-1.0f,-1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f,-1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	
	{-1.0f, 1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{-1.0f,-1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f, 1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f,-1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	
	{-1.0f, 1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f, 1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{-1.0f, 1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f, 1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	
	{-1.0f,-1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{-1.0f,-1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f,-1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f,-1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},

	{ 1.0f, 1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f, 1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f,-1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{ 1.0f,-1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	
	{-1.0f, 1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{-1.0f,-1.0f,-1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{-1.0f, 1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0},
	{-1.0f,-1.0f, 1.0f, 1.0,0.0,0.0, 0.0, 0.0}
};*/


Vertex QuadVertexes[] =
{
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
	{1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
	{0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f}

};



// ---------------------------------------------------------------------------
// TestD3D9Canvas
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(D3D9Canvas, D3D9BaseCanvas)
    EVT_SIZE(D3D9Canvas::OnSize)
    EVT_PAINT(D3D9Canvas::OnPaint)
    EVT_ERASE_BACKGROUND(D3D9Canvas::OnEraseBackground)
    EVT_IDLE(D3D9Canvas::OnIdle)
	EVT_LEFT_DOWN(D3D9Canvas::mouseLeftDown)
	EVT_MOTION (D3D9Canvas::mouseMove)
	EVT_LEFT_UP(D3D9Canvas::mouseReleased)
END_EVENT_TABLE()

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3D9Canvas::D3D9Canvas(wxWindow *parent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : D3D9BaseCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
	//BaseGraphicsBoard(
{

	this->m_owner = (MyFrame*)parent;
   g_fTime = 0;
   bSelectedPiece = false;
	chess_board = new ChessBoard();
	chess_board->LoadTextures();
	chess_board->ChangeTextures(string(wxStandardPaths::Get().GetResourcesDir().c_str().AsChar())+string("/images/PieceThemes/Fritz/100/dropshadow/"));


	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			
			g_pTexture[i][j] = NULL;
			
		}
	}

   gameState = GameState::STATE_ZERO;
   bFlip = true;
   pMov = NULL;
   this->m_render_timer = NULL;

   g_pVertexBuffer = NULL;

   this->m_backgroundColour = this->m_owner->optionsFrame->m_BoardOptions.background;
   this->colBorder = this->m_owner->optionsFrame->m_BoardOptions.border;
   this->colArrow = this->m_owner->optionsFrame->m_BoardOptions.arrow;
   this->colText = this->m_owner->optionsFrame->m_BoardOptions.colText;
}

D3D9Canvas::D3D9Canvas(wxWindow *parent, BaseGraphicsBoard* boardToCopy,
		wxWindowID id,
        const wxPoint& pos ,
        const wxSize& size , long style ,
        const wxString& name )
		: D3D9BaseCanvas(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
{
	if( boardToCopy )
	{
		this->PerformCopy(boardToCopy);
	}
	this->m_owner = (MyFrame*)parent;
   g_fTime = 0;
   bSelectedPiece = false;
   if( !boardToCopy )
   {
		chess_board = new ChessBoard();
		gameState = GameState::STATE_ZERO;
   }

	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			
			g_pTexture[i][j] = NULL;
			
		}
	}

   bFlip = true;
   pMov = NULL;
   this->m_render_timer = NULL;

   this->m_backgroundColour = this->m_owner->optionsFrame->m_BoardOptions.background;
   this->colBorder = this->m_owner->optionsFrame->m_BoardOptions.border;
   this->colArrow = this->m_owner->optionsFrame->m_BoardOptions.arrow;
   this->colText = this->m_owner->optionsFrame->m_BoardOptions.colText;

}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3D9Canvas::~D3D9Canvas()
{
	if( this->m_render_timer )
	{
		if( this->m_render_timer->IsRunning())
			this->m_render_timer->Stop();
		
		delete this->m_render_timer;
		this->m_render_timer = NULL;
	}

    if( g_pVertexBuffer != NULL )
        g_pVertexBuffer->Release(); 

	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			if( g_pTexture[i][j] )
			{
				g_pTexture[i][j]->Release();
			}
		}
	}

	for( int i = 0; i < 6; i++ )
	{
		if( g_pWhitePieces[i] )
			g_pWhitePieces[i]->Release();

		if( g_pBlackPieces[i] )
			g_pBlackPieces[i]->Release();
	}
	
	for( int i = 0; i < 8; i++ )
	{
		if( aToH[i] )
			aToH[i]->Release();

		if( _1to8[i] )
			_1to8[i]->Release();
	}

	if( border1 )
		border1->Release();

	if( blackTex )
		blackTex->Release();

	//LPD3DXLINE ppLine;
	
	if( m_font )
		m_font->Release();

	if( quadLineVB )
		quadLineVB->Release();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::OnIdle(wxIdleEvent &event)
{
  //  event.RequestMore(true);
    
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    wxPaintDC dc(this);



    // Initialize D3D
    if (!m_init)
    {
        InitD3D();
        ResetProjectionMode();
        m_init= true;
    }

	Render();
	//Render();
	
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::OnSize(wxSizeEvent& event)
{

	resizing = true;

	if( !m_init )
	{
		this->InitD3D();
	}
	else if( m_owner->optionsFrame->m_BoardOptions.d3dModeRecreateDevice == true )
	{
		static int i = 0;
		if( i == 0 )
		{
			i++;
		}
		else
		{
			RecreateDevice();

	
		}

	}

		g_pd3dDevice->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
			g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );
			g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, false );

			EnableAlphaBlending();

			/*
			g_pd3dDevice->SetSamplerState(0,
											D3DSAMP_MIPFILTER,
											D3DTEXF_ANISOTROPIC);
		 g_pd3dDevice->SetSamplerState(0,
											D3DSAMP_MAXANISOTROPY,
											0);
				*/

			//CreateLines();
			//ppLine->OnResetDevice();

			SetLinearFiltering();
			 g_pd3dDevice->SetSamplerState(0,
												D3DSAMP_MIPFILTER,
												D3DTEXF_ANISOTROPIC);
			 g_pd3dDevice->SetSamplerState(0,
												D3DSAMP_MAXANISOTROPY,
												0);


//if( g_pTexture[0][0] == NULL )
//	wxMessageBox("texture is null");

	wxColour cl = this->m_backgroundColour;
   g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         //D3DCOLOR_COLORVALUE(cl.Red(),cl.Red(),cl.Red(),1.0f), 1.0f, 0 );
   D3DCOLOR_ARGB(255,cl.Red(),cl.Green(),cl.Blue()), 1.0f, 0 );
    // Reset the D3D view aspect
    ResetProjectionMode();
	//Refresh(true);
	//Refresh(true);

	resizing = false;
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::CreateLines()
{
/*	if( D3DXCreateLine(g_pd3dDevice,&ppLine)!=D3D_OK)
	{
		wxMessageBox("Failed to Create Lines","");
	}*/

	
	
//	D3DXMatrixTranslation( &matTrans, i*1.0f-4.0f, j*1.0f-3.0f, 11.0f );

	int count = 0;
	for( int i = 0; i < 17; i+=2 )
	{
		lines[i] = D3DXVECTOR3(count*1.0f-7.0f,  -8.0f, 0.0f);
		lines[i+1] = D3DXVECTOR3(count*1.0f-7.0f, 0.0f, 0.0f);
		count++;
	}
	count = 0;
	for( int j = 0; j < 17; j+=2 )
	{
		lines[j+18] = D3DXVECTOR3(-7.0f, count*1.0f -8.0f, 0.0f);
		lines[j+18+1] = D3DXVECTOR3(0.0f, count*1.0f-8.0f, 0.0f);
		count++;
	}		
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::InitD3D()
{
	if( g_pd3dDevice == NULL )
		CreateDevice();


	CreateQuad();

	LoadTiles();

//	CreateLines();

	LoadManagedTextures();
	

	quadLineVB = this->CreateQuadTemplate<TXVertex>(D3DFVF_TXVERTEX,QuadTXVertexes);

	arrowHeadVB = this->CreateVertexBuffer<TXVertex>(sizeof(ArrowHeadVertexes),D3DFVF_TXVERTEX );

	TXVertex *pData = NULL;
	arrowHeadVB->Lock(0, sizeof(ArrowHeadVertexes), (void**)&pData, 0 );
	memcpy((void*)pData, (void*)&ArrowHeadVertexes, sizeof(ArrowHeadVertexes));
	arrowHeadVB->Unlock();
//g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
//g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );

	/*
	
g_pd3dDevice->SetSamplerState(0,
                                    D3DSAMP_MIPFILTER,
                                    D3DTEXF_ANISOTROPIC);
 g_pd3dDevice->SetSamplerState(0,
                                    D3DSAMP_MAXANISOTROPY,
                                    0);
		*/							

	SetLinearFiltering();
	 g_pd3dDevice->SetSamplerState(0,
										D3DSAMP_MIPFILTER,
										D3DTEXF_ANISOTROPIC);
										//D3DTEXF_POINT);
	 g_pd3dDevice->SetSamplerState(0,
										D3DSAMP_MAXANISOTROPY,
										0);


	 CreateLights();

	 m_font = CreateFont();

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, false );
// Enable alpha blending.
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 
							TRUE);

// Set the source blend state.
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, 
							D3DBLEND_SRCALPHA);

// Set the destination blend state.
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, 
							D3DBLEND_INVSRCALPHA);

	//alpha blending enabled
	/*g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,true);
	//source blend factor
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	//destination blend factor
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//alpha from texture
	g_pd3dDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	*/

	CreateMaterials();

	SetupBoard();

	D3DXMatrixIdentity(&matView);

	if( !m_render_timer )
	{
		m_render_timer = new D3DRenderTimer(this);
		m_render_timer->start();
	}
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::ResetProjectionMode()
{
   if(g_pd3dDevice == NULL)
	return;

    
    GetClientSize(&w, &h);

	static bool first_time = true;



	if( first_time )
	{
		first_time = false;
	}
	
	if( w > h )
    {
		D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 44.5f ), 
                                (float)w / float(h), 0.1f, 100.0f );
	}
	else
	{
		D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 44.5f ), 
                                (float)h / float(w), 0.1f, 100.0f );
		float temp = matProj._22;// = 
		matProj._22 = matProj._11;
		matProj._11 = temp;
	}
	

/*
xScale     0          0               0
0        yScale       0               0
0          0       zf/(zf-zn)         1
0          0       -zn*zf/(zf-zn)     0
where:
yScale = cot(fovY/2)

xScale = yScale / aspect ratio
*/
//D3DXMatrixPerspectiveLH(&matProj,  
 //                               (float)w, float(h), 1.0f, 100.0f );

/*
2*zn/w  0       0              0
0       2*zn/h  0              0
0       0       zf/(zf-zn)     1
0       0       zn*zf/(zn-zf)  0
*/
/*D3DXMatrixOrthoLH(
  &matProj,
  w, h, 0.1f, 100.0f);
  */
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::Render()
{
	//wxColour clearCol(100,50,255,1);
	Clear(D3DCLEAR_TARGET , this->m_backgroundColour);
//	g_pd3dDevice->Clear(0,0,
  // D3DCOLOR_COLORVALUE(100.0f,50.0f,255.0f,1.0f), 1.0f, 0 );


	if( newMaterials )
	{
		this->CreateMaterials();
	}
   static float fXrot = 0.0f;
   static float fYrot = 0.0f;
   static float fZrot = 0.0f;

   fXrot += 10.1f * g_fTime;
   fYrot += 10.2f * g_fTime;
   fZrot += 10.3f * g_fTime;
	
   D3DXMATRIX matWorld;
   D3DXMATRIX matTrans;
   D3DXMATRIX matRot;

   bool whiteMove = false;
   
   if( !bAnimating )
   {
   		if( chess_board->mov == BLACK )
		{
			//highlight_white = true;
			if( m_owner->currentGame != NULL )
				pMov = &m_owner->currentGame->pCurrMove->whiteMove;

			whiteMove = true;
		}
		else
		{
			if( m_owner->currentGame != NULL )
				pMov = &m_owner->currentGame->pCurrMove->blackMove;
		}
   }




   D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 15.0f );

   D3DXMatrixRotationYawPitchRoll( &matRot, 
		                            D3DXToRadian(fXrot), 
		                            D3DXToRadian(fYrot), 
		                            D3DXToRadian(fZrot) );

    matWorld = matRot * matTrans;
	D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 145.0f );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );

    BeginScene();


//	g_pd3dDevice->SetRenderState(D3D_TEXENABLE



    g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(Vertex) );
    g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );


	//border1
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixTranslation( &matTrans, -4.35f, 7.7 -3.35f, 11.0f );
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 8.7f, 8.7f, 1.0f);
	matTrans = matScale * matTrans;
	if( m_owner->optionsFrame->m_BoardOptions.useBorderBitmap == true )
	{
		g_pd3dDevice->SetTexture(0, border1 );
		g_pd3dDevice->SetMaterial(&mat1);
	}
	else
	{
		g_pd3dDevice->SetTexture(0, NULL );
		g_pd3dDevice->SetMaterial(&mat3);
	}
	
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );

	g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);

	//D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.25, 0.25, 1.0f);
//	
	if( this->m_owner->optionsFrame->m_BoardOptions.showCoordinates )
	{
		if( bFlip )
		{
			for( int i = 0; i < 8; i++ )
			{
				D3DXMatrixTranslation( &matTrans, float(i)-4.0f+0.5-0.125, -4.0f-0.04, 11.0f );
				matTrans = matScale * matTrans;
				g_pd3dDevice->SetTransform(D3DTS_WORLD,&matTrans);
				g_pd3dDevice->SetMaterial(&matTxt);
				g_pd3dDevice->SetTexture(0, aToH[i]);
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);

				D3DXMatrixIdentity(&matTrans);

				D3DXMatrixTranslation( &matTrans, -4.0f-0.3, float(i)-4.0f+0.5+0.125, 11.0f );
				matTrans = matScale * matTrans;
				g_pd3dDevice->SetTransform(D3DTS_WORLD,&matTrans);
				g_pd3dDevice->SetMaterial(&matTxt);
				g_pd3dDevice->SetTexture(0, _1to8[i]);
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
			}
		}
		else
		{
			for( int i = 0; i <8; i++ )
			{
				D3DXMatrixTranslation( &matTrans, float(7-i)-4.0f+0.5-0.125, -4.0f-0.04, 11.0f );
				matTrans = matScale * matTrans;
				g_pd3dDevice->SetTransform(D3DTS_WORLD,&matTrans);
				g_pd3dDevice->SetMaterial(&matTxt);
				g_pd3dDevice->SetTexture(0, aToH[i]);
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);

				D3DXMatrixIdentity(&matTrans);

				D3DXMatrixTranslation( &matTrans, -4.0f-0.3, float(7-i)-4.0f+0.5+0.125, 11.0f );
				matTrans = matScale * matTrans;
				g_pd3dDevice->SetTransform(D3DTS_WORLD,&matTrans);
				g_pd3dDevice->SetMaterial(&matTxt);
				g_pd3dDevice->SetTexture(0, _1to8[i]);
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
			}
		}
	}


	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{

			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation( &matTrans, i*1.0f-4.0f, j*1.0f-3.0f, 11.0f );
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );

			if( this->m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == true )
			{
				g_pd3dDevice->SetTexture(0, g_pTexture[i][j] );
			}
			else
			{
				g_pd3dDevice->SetTexture(0, NULL );
			}


			g_pd3dDevice->SetMaterial(&mat1);
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
			if( (i+j)%2 != 0 )
			{
				g_pd3dDevice->SetMaterial(&mat1);
			}
			else
			{
				
				g_pd3dDevice->SetMaterial(&mat2);
			}
			g_pd3dDevice->SetTexture(0, 0 );
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
		}
	}

	

	this->EnableAlpha(false);
	this->LightingEnabled(false);


	//g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 6);

	if( m_owner->optionsFrame->m_BoardOptions.showLines == true )
	{
		for( int i = 0; i < 9; i++ )
		{
			if( i != 0 && i != 8 ) continue;
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation( &matTrans, float(i)*1.0f-4.0f - 0.0125f, 8.0f-4.0f + 0.0125, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 0.025f, 8.025f,1.0f);
			matTrans = matScale * matTrans;
			g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation( &matTrans, -4.0f, 8.0-4.0f-float(i)*1.0f +0.008f, 11.0f );
			//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 8.0f, 0.025f,1.0f);
			matTrans = matScale * matTrans;
			g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		}
	}

	this->EnableAlpha(true);
	this->LightingEnabled(true);

	static int starti = 4;
	static int startj = 1;
	static int endi = 4;
	static int endj = 3;
	static int step = 0;



	

	static int indx = 0;

	static D3DXVECTOR4 moves[] = {D3DXVECTOR4(4,1,4,3), D3DXVECTOR4(4,6,4,4), 
								  D3DXVECTOR4(5,1,5,3), D3DXVECTOR4(4,6,5,3),
								  D3DXVECTOR4(6,0,5,2), D3DXVECTOR4(3,6,3,5),
								  D3DXVECTOR4(5,0,2,3), D3DXVECTOR4(6,6,6,4)};

	static int col = -1;
	static double currX = starti;
	static double currY = startj;

	step++;
	if( step > 20 )
	{
		step = 0;
		col -=col;
		indx ++;
	}

	bool drawArrows = true;

	D3DXVECTOR3 arrowStart;//(pMov->from.file - 4.0f+0.5, pMov->from.rank - 3.0f-0.5 , 11.0f);
	D3DXVECTOR3 arrowEnd;//(pMov->to.file - 4.0f+0.5, pMov->to.rank - 3.0f-0.5 , 11.0f);
	
	if( bFlip )
	{
		arrowStart.x = pMov->from.file - 4.0f+0.5;
		arrowStart.y = pMov->from.rank - 3.0f-0.5;
		arrowEnd.x =   pMov->to.file - 4.0f+0.5;
		arrowEnd.y =   pMov->to.rank - 3.0f-0.5;
	}
	else
	{
		arrowStart.x = (7-pMov->from.file) - 4.0f+0.5;
		arrowStart.y = (7-pMov->from.rank) - 3.0f-0.5;
		arrowEnd.x =   (7-pMov->to.file) - 4.0f+0.5;
		arrowEnd.y =   (7-pMov->to.rank) - 3.0f-0.5;
	}


	g_pd3dDevice->SetStreamSource(0, quadLineVB,0,sizeof(TXVertex));
	g_pd3dDevice->SetFVF( D3DFVF_TXVERTEX );
	g_pd3dDevice->SetMaterial(&matArrow);
	g_pd3dDevice->SetTexture( 0, /*yellowTex*/0 );
	
	if( m_owner->currentGame->pCurrMove != m_owner->currentGame->pBase )
	{
		//1
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation( &matTrans, arrowStart.x-0.5, arrowStart.y+0.5, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 0.05f, 1.0f,1.0f);
			matTrans = matScale * matTrans;
		//	g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		//2
			D3DXMatrixTranslation( &matTrans, arrowStart.x+0.5-0.05, arrowStart.y+0.5, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 0.05f, 1.0f,1.0f);
			matTrans = matScale * matTrans;
		//	g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		//3
			D3DXMatrixTranslation( &matTrans, arrowStart.x-0.5, arrowStart.y+0.5 , 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 1.0f, 0.05f ,1.0f);
			matTrans = matScale * matTrans;
		//	g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		//4
			D3DXMatrixTranslation( &matTrans, arrowStart.x-0.5, arrowStart.y-0.5+0.05, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 1.0f,0.05f,1.0f);
			matTrans = matScale * matTrans;
		//	g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		//1   (2nd arrow)
			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation( &matTrans, arrowEnd.x-0.5, arrowEnd.y+0.5, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 0.05f, 1.0f,1.0f);
			matTrans = matScale * matTrans;
		//	g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			D3DXMatrixTranslation( &matTrans, arrowEnd.x+0.5-0.05, arrowEnd.y+0.5, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 0.05f, 1.0f,1.0f);
			matTrans = matScale * matTrans;
		//	g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			D3DXMatrixTranslation( &matTrans, arrowEnd.x-0.5, arrowEnd.y+0.5, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 1.0f, 0.05f ,1.0f);
			matTrans = matScale * matTrans;
		//	g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

			D3DXMatrixTranslation( &matTrans, arrowEnd.x-0.5, arrowEnd.y-0.5+0.05, 11.0f );
		//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 1.0f,0.05f,1.0f);
			matTrans = matScale * matTrans;
	//		g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		/*	D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation( &matTrans, -4.0f, 8.0-4.0f-float(i)*1.0f +0.008f, 11.0f );
			//D3DXMATRIX matScale;
			D3DXMatrixScaling(&matScale, 8.0f, 0.025f,1.0f);
			matTrans = matScale * matTrans;
			g_pd3dDevice->SetTexture(0, blackTex);
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
			g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);*/
	}
	else
	{
		drawArrows = false;
	}

	arrowStart.z = arrowEnd.z = 11.0f;

	g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(Vertex) );
    g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_pd3dDevice->SetMaterial(&whiteMat);
	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			double step_size = double(moves[indx].w - moves[indx].y) / 20.0f;

			int indx1, indx2;
			if( bFlip )
			{
				indx1 = i;
				indx2 = j;
			}
			else
			{
				indx1 = 7-i;
				indx2 = 7-j;
			}

			if( ((i == selX) && (j == selY) && 
				(gameState == GameState::SELECTED_PIECE))|| 
				 (bAnimating && (indx1 == selX) &&(indx2 == selY)) )
			{

			//	if( bdraw )
			//		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
			}
			else
			{
				D3DXMatrixIdentity(&matTrans);
				D3DXMatrixTranslation( &matTrans, i*1.0f-4.0f, j*1.0f-3.0f, 11.0f );
				g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
				 
				bool bdraw = true;

				switch( chess_board->board[indx1][indx2] )
				{
				case 0: bdraw  = false; break;
				case ChessBoard::wPawn:g_pd3dDevice->SetTexture(0, g_pWhitePieces[0] );break;
				case ChessBoard::wKnight:g_pd3dDevice->SetTexture(0, g_pWhitePieces[2] );break;
				case ChessBoard::wBishop:g_pd3dDevice->SetTexture(0, g_pWhitePieces[1] );break;
				case ChessBoard::wRook:g_pd3dDevice->SetTexture(0, g_pWhitePieces[3] );break;
				case ChessBoard::wQueen: g_pd3dDevice->SetTexture(0, g_pWhitePieces[4] );break;
				case ChessBoard::wKing: g_pd3dDevice->SetTexture(0, g_pWhitePieces[5] );break;

				case ChessBoard::bPawn:g_pd3dDevice->SetTexture(0, g_pBlackPieces[0] );break;
				case ChessBoard::bKnight:g_pd3dDevice->SetTexture(0, g_pBlackPieces[2] );break;
				case ChessBoard::bBishop:g_pd3dDevice->SetTexture(0, g_pBlackPieces[1] );break;
				case ChessBoard::bRook:g_pd3dDevice->SetTexture(0, g_pBlackPieces[3] );break;
				case ChessBoard::bQueen:g_pd3dDevice->SetTexture(0, g_pBlackPieces[4] );break;
				case ChessBoard::bKing:g_pd3dDevice->SetTexture(0, g_pBlackPieces[5] );break;
				};

				if( bdraw)
					g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
				
			}
		}
	}



	if( gameState == GameState::SELECTED_PIECE )
	{
		drawArrows = false;
		for( int i = 0; i < 8; i++ )
		{
			for( int j = 0; j < 8; j++ )
			{


				if( i == selX && 7-j == selY )
				{
					D3DXMatrixIdentity(&matTrans);
					D3DXMatrixTranslation( &matTrans, (float)mouseX-0.5f, (float)mouseY+0.5f, 11.0f );
					D3DXMatrixScaling(&matScale, 1.2f, 1.2f,1.0f);
					matTrans = matScale*matTrans;
					g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
					bool bdraw = true;
					/*int drawID;
					if( this->chess_board->SquareHasPiece(j,i,&drawID) )
					{
					}*/
					int indx1, indx2;
					if( !bFlip )
					{
						indx1 = i;
						indx2 = j;
					}
					else
					{
						indx1 = 7-i;
						indx2 = 7-j;
					}
					switch( selPieceID )
					{
					case 0: bdraw = false; break;
					case ChessBoard::wPawn:g_pd3dDevice->SetTexture(0, g_pWhitePieces[0] );break;
					case ChessBoard::wKnight:g_pd3dDevice->SetTexture(0, g_pWhitePieces[2] );break;
					case ChessBoard::wBishop:g_pd3dDevice->SetTexture(0, g_pWhitePieces[1] );break;
					case ChessBoard::wRook:g_pd3dDevice->SetTexture(0, g_pWhitePieces[3] );break;
					case ChessBoard::wQueen: g_pd3dDevice->SetTexture(0, g_pWhitePieces[4] );break;
					case ChessBoard::wKing: g_pd3dDevice->SetTexture(0, g_pWhitePieces[5] );break;

					case ChessBoard::bPawn:g_pd3dDevice->SetTexture(0, g_pBlackPieces[0] );break;
					case ChessBoard::bKnight:g_pd3dDevice->SetTexture(0, g_pBlackPieces[2] );break;
					case ChessBoard::bBishop:g_pd3dDevice->SetTexture(0, g_pBlackPieces[1] );break;
					case ChessBoard::bRook:g_pd3dDevice->SetTexture(0, g_pBlackPieces[3] );break;
					case ChessBoard::bQueen:g_pd3dDevice->SetTexture(0, g_pBlackPieces[4] );break;
					case ChessBoard::bKing:g_pd3dDevice->SetTexture(0, g_pBlackPieces[5] );break;
					};
					if( bdraw )
						g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
				}
			}
		}
	}

	if( bAnimating && pMov)
	{
		// = 
		float fromX, fromY, toX, toY;
		D3DXVECTOR2 from;
		D3DXVECTOR2 to;
		if( bFowardsAnim )
		{
			if( bFlip )
			{
				from.x = pMov->from.file - 4.0f;
				from.y = pMov->from.rank - 3.0f;
				to.x =   pMov->to.file - 4.0f;
				to.y =   pMov->to.rank - 3.0f;
			}
			else
			{
				from.x = (7-pMov->from.file) - 4.0f;
				from.y = (7-pMov->from.rank) - 3.0f;
				to.x =   (7-pMov->to.file) - 4.0f;
				to.y =   (7-pMov->to.rank) - 3.0f;
			}

			if( this->capturePiece != 0 )
			{
				switch( this->capturePiece )
				{
				case ChessBoard::wPawn:g_pd3dDevice->SetTexture(0, g_pWhitePieces[0] );break;
				case ChessBoard::wKnight:g_pd3dDevice->SetTexture(0, g_pWhitePieces[2] );break;
				case ChessBoard::wBishop:g_pd3dDevice->SetTexture(0, g_pWhitePieces[1] );break;
				case ChessBoard::wRook:g_pd3dDevice->SetTexture(0, g_pWhitePieces[3] );break;
				case ChessBoard::wQueen:g_pd3dDevice->SetTexture(0, g_pWhitePieces[4] );break;
				case ChessBoard::wKing:g_pd3dDevice->SetTexture(0, g_pWhitePieces[5] );break;
				case ChessBoard::bPawn:g_pd3dDevice->SetTexture(0, g_pBlackPieces[0] );break;
				case ChessBoard::bKnight:g_pd3dDevice->SetTexture(0, g_pBlackPieces[2] );break;
				case ChessBoard::bBishop:g_pd3dDevice->SetTexture(0, g_pBlackPieces[1] );break;
				case ChessBoard::bRook:g_pd3dDevice->SetTexture(0, g_pBlackPieces[3] );break;
				case ChessBoard::bQueen:g_pd3dDevice->SetTexture(0, g_pBlackPieces[4] );break;
				case ChessBoard::bKing:g_pd3dDevice->SetTexture(0, g_pBlackPieces[5] );break;
				};
				
				D3DXMatrixTranslation(&matTrans, to.x, to.y, 11.0f);
				g_pd3dDevice->SetTransform(D3DTS_WORLD, &matTrans);
				g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
			}
		}
		else
		{
			if( bFlip )
			{
				from.x = pMov->to.file - 4.0f;
				from.y = pMov->to.rank - 3.0f;
				to.x =   pMov->from.file - 4.0f;
				to.y =   pMov->from.rank - 3.0f;
			}
			else
			{
				from.x = (7-pMov->to.file) - 4.0f;
				from.y = (7-pMov->to.rank) - 3.0f;
				to.x =   (7-pMov->from.file) - 4.0f;
				to.y =   (7-pMov->from.rank) - 3.0f;
			}
		}
		static int numTicksSinceLast = 0;

		D3DXVECTOR2 dir = to-from;
		float length = D3DXVec2Length(&dir);
		int numTicks = animationSpeed;
		float tickDist = length / (float) numTicks;

		numTicksSinceLast++;

		D3DXVec2Normalize(&dir, &dir);

		D3DXVECTOR2 curr(this->currAnimX, this->currAnimY);

		curr += dir*tickDist;

		this->currAnimX = curr.x;
		this->currAnimY = curr.y;

		arrowEnd.x = currAnimX+0.5;
		arrowEnd.y = currAnimY-0.5;

		D3DXVECTOR2 newDist = to - curr;
		float newLen = D3DXVec2Length(&newDist);

		switch( selPieceID )
		{
			case ChessBoard::wPawn:g_pd3dDevice->SetTexture(0, g_pWhitePieces[0] );break;
			case ChessBoard::wKnight:g_pd3dDevice->SetTexture(0, g_pWhitePieces[2] );break;
			case ChessBoard::wBishop:g_pd3dDevice->SetTexture(0, g_pWhitePieces[1] );break;
			case ChessBoard::wRook:g_pd3dDevice->SetTexture(0, g_pWhitePieces[3] );break;
			case ChessBoard::wQueen:g_pd3dDevice->SetTexture(0, g_pWhitePieces[4] );break;
			case ChessBoard::wKing:g_pd3dDevice->SetTexture(0, g_pWhitePieces[5] );break;
			case ChessBoard::bPawn:g_pd3dDevice->SetTexture(0, g_pBlackPieces[0] );break;
			case ChessBoard::bKnight:g_pd3dDevice->SetTexture(0, g_pBlackPieces[2] );break;
			case ChessBoard::bBishop:g_pd3dDevice->SetTexture(0, g_pBlackPieces[1] );break;
			case ChessBoard::bRook:g_pd3dDevice->SetTexture(0, g_pBlackPieces[3] );break;
			case ChessBoard::bQueen:g_pd3dDevice->SetTexture(0, g_pBlackPieces[4] );break;
			case ChessBoard::bKing:g_pd3dDevice->SetTexture(0, g_pBlackPieces[5] );break;
		};

		D3DXMatrixTranslation(&matTrans, curr.x, curr.y, 11.0f);
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matTrans);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);

		if( (newLen < tickDist) || (numTicksSinceLast > numTicks+1))
		{
			numTicksSinceLast = 0;
			bAnimating = false;
				selX =selY=-1;
				selPieceID=-1;
				capturePiece = 0;
				
				if( !(m_owner->m_gameTraverseTimer) || 
					(m_owner->m_gameTraverseTimer && !m_owner->m_gameTraverseTimer->IsRunning()) )
				{
					if( startEnginesOnKeyUp )
					{
						m_owner->StartEnginesAfterMoveChange();
						startEnginesOnKeyUp = false;
					}
				}
				else
				{
					//Refresh(false);
				}
				//animationFinishedLastFrame = true;
		}
		else
		{
			if( !(m_owner->m_gameTraverseTimer) || 
				(m_owner->m_gameTraverseTimer && !m_owner->m_gameTraverseTimer->IsRunning()) )
			{
				//Refresh(false);
			}
		}
	}

	if( drawArrows && (m_owner->optionsFrame->m_BoardOptions.showArrows == true) )
	{

		this->EnableAlphaBlending();
		matArrow.Ambient.a = matArrow.Diffuse.a = 0.5f;
		D3DXVECTOR3 arrow = arrowEnd - arrowStart;

		float arrowLength = D3DXVec3Length(&arrow);

		D3DXVECTOR3 arrowNorm = arrow;

		D3DXVec3Normalize(&arrowNorm, &arrowNorm);

		float theta = 0.0f;

		theta = atan2(arrow.y , arrow.x );
		

		D3DXMATRIX matArrowRotate;


		D3DXMatrixRotationZ( &matArrowRotate, theta );

		D3DXMatrixTranslation( &matTrans, arrowStart.x - arrowNorm.y *0.05, arrowStart.y + arrowNorm.x *0.05, arrowStart.z );

		D3DXMatrixScaling(&matScale, arrowLength-0.3, 0.1, 1.0f);
		matTrans = matArrowRotate * matTrans;
		matTrans = matScale * matTrans;

		D3DXMATRIX matTrans2; 


		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matTrans);


		g_pd3dDevice->SetStreamSource(0, quadLineVB,0,sizeof(TXVertex));
		g_pd3dDevice->SetFVF( D3DFVF_TXVERTEX );

		g_pd3dDevice->SetMaterial(&matArrow);
		g_pd3dDevice->SetTexture( 0, yellowTex );

	
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);


		float scaleFactor = arrowLength -0.3;
		scaleFactor /= arrowLength ;
		D3DXMatrixTranslation( &matTrans, arrowStart.x + arrow.x*scaleFactor - arrowNorm.y *0.05, arrowStart.y+arrow.y*scaleFactor + arrowNorm.x *0.05, arrowStart.z );

		D3DXMatrixScaling(&matScale, 1.0f, 0.1, 1.0f);
		matTrans = matArrowRotate * matTrans;
		matTrans = matScale * matTrans;


		g_pd3dDevice->SetTransform(D3DTS_WORLD, &matTrans);

		g_pd3dDevice->SetStreamSource(0, arrowHeadVB,0,sizeof(TXVertex));
		g_pd3dDevice->SetTexture( 0, yellowTex );
		g_pd3dDevice->SetMaterial(&matArrow);
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 1);

		g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

		//g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );

		//matArrow.Diffuse.a = diffMatArrow;

		matArrow.Ambient.a = matArrow.Diffuse.a = 1.0f;
		//this->EnableAlpha(false);
	}

	   g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(Vertex) );
    g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );


	D3DXMatrixIdentity(&matTrans);
	
	
	


	EndScene();

    g_fTime += 0.002f;
}



void D3D9Canvas::ChangeTextures(wxString &name)
{
	for( int i =0; i < 6; i++ )
	{
		g_pWhitePieces[i]->Release();
		g_pWhitePieces[i] = NULL;

		g_pBlackPieces[i]->Release();
		g_pBlackPieces[i] = NULL;
	}

	wxString path = "/images/PieceThemes/";

	path += name;



	g_pWhitePieces[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+ path + "/wp.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path + "/wb.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/wn.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/wr.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/wq.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/wk.png", D3DX_DEFAULT, D3DX_DEFAULT);
	
	g_pBlackPieces[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/bp.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/bb.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/bn.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/br.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/bq.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+path+"/bk.png", D3DX_DEFAULT, D3DX_DEFAULT);


}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::LoadManagedTextures()
{

	g_pWhitePieces[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/wp.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/wb.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/wn.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/wr.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/wq.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/wk.png", D3DX_DEFAULT, D3DX_DEFAULT);
	
	g_pBlackPieces[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/bp.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/bb.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/bn.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/br.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/bq.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Merida/bk.png", D3DX_DEFAULT, D3DX_DEFAULT);

	border1 = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/border_frame.png", D3DX_DEFAULT, D3DX_DEFAULT);

	blackTex = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/black.png",D3DX_DEFAULT, D3DX_DEFAULT); 

	yellowTex = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/yellow.png",D3DX_DEFAULT, D3DX_DEFAULT); 

	aToH[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/A.png");
	aToH[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/B.png");
	aToH[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/C.png");
	aToH[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/D.png");
	aToH[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/E.png");
	aToH[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/F.png");
	aToH[6] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/G.png");
	aToH[7] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/H.png");

	_1to8[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/1.png");
	_1to8[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/2.png");
	_1to8[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/3.png");
	_1to8[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/4.png");
	_1to8[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/5.png");
	_1to8[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/6.png");
	_1to8[6] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/7.png");
	_1to8[7] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/Letters/8.png");
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::CreateQuad()
{
	g_pVertexBuffer = CreateVertexBuffer<Vertex>(6, D3DFVF_CUSTOMVERTEX);

	void *pVertices = NULL;

    g_pVertexBuffer->Lock( 0, sizeof(QuadVertexes), (void**)&pVertices, 0 );
    memcpy( pVertices, QuadVertexes, sizeof(QuadVertexes) );
    g_pVertexBuffer->Unlock();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::ChangeTiles(wxString &dir, wxString &dir2)
{
	bool loaded = true;
	int loaded_count = 0;
	for( char a = 'a'; a<'i'; a++ )
	{
		for( char n = '1'; n < '9'; n++ )
		{
			int indx1 = a-'a';
			int indx2 = n-'1';

			wxString path = wxStandardPaths::Get().GetResourcesDir();
			if( (indx1+indx2) %2 != 1 )
			{
				path += "/images/squares/indi/" + dir;
				path += a;
				path += n;
				path += ".png";
			}
			else
			{
				path += "/images/squares/indi/" +dir2;
				path += a;
				path += n;
				path += ".png";
			}
	//		D3DXCreateTextureFromFile(g_pd3dDevice,
    //                      path.c_str(),
    //                      &g_pTexture[indx1][indx2]);

			if( g_pTexture[indx1][indx2] )
			{
				g_pTexture[indx1][indx2]->Release();
				g_pTexture[indx1][indx2] = NULL;
			}
			
			if( D3DXCreateTextureFromFileEx(
			  g_pd3dDevice,
			  path.c_str(),115,115, 0,0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
			  D3DX_FILTER_LINEAR,D3DX_DEFAULT, 0, 0, 0, &g_pTexture[indx1][indx2]) != D3D_OK )
			//D3DX_FILTER_NONE,D3DX_DEFAULT, 0, 0, 0, &g_pTexture[indx1][indx2]);
			//D3DX_FILTER_POINT
			{
				loaded = false;
				loaded_count++;
			}

		}
	}

	if( loaded == false )
	{
		wxMessageBox(wxString::Format("Failed to load %d board textures", loaded_count));
	}

}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::LoadTiles()
{
	//D3DX9LoadTexture(
	bool loaded = true;
	int loaded_count = 0;
	for( char a = 'a'; a<'i'; a++ )
	{
		for( char n = '1'; n < '9'; n++ )
		{
			int indx1 = a-'a';
			int indx2 = n-'1';

			wxString path = wxStandardPaths::Get().GetResourcesDir();
			if( (indx1+indx2) %2 != 1 )
			{
				path += "/images/squares/indi/Fiddleback Makore/";
				path += a;
				path += n;
				path += ".png";
			}
			else
			{
				path += "/images/squares/indi/Burl Walnut/";
				path += a;
				path += n;
				path += ".png";
			}
	//		D3DXCreateTextureFromFile(g_pd3dDevice,
    //                      path.c_str(),
    //                      &g_pTexture[indx1][indx2]);

			if( D3DXCreateTextureFromFileEx(
			  g_pd3dDevice,
			  path.c_str(),115,115, 0,0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
			  D3DX_FILTER_LINEAR,D3DX_DEFAULT, 0, 0, 0, &g_pTexture[indx1][indx2]) != D3D_OK)
			{
				loaded = false;
				loaded_count++;
			}
			//D3DX_FILTER_NONE,D3DX_DEFAULT, 0, 0, 0, &g_pTexture[indx1][indx2]);
			//D3DX_FILTER_POINT
		}
	}

	if( loaded == false )
	{
		wxMessageBox(wxString::Format("Failed to load %d board textures", loaded_count));
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::CreateManagedResources()
{
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::CreateLights()
{
	D3DLIGHT9 Light;
	ZeroMemory(&Light,sizeof(Light));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Diffuse.r = 1.0f;
	Light.Diffuse.g = 1.0f;
	Light.Diffuse.b = 1.0f;
	Light.Direction.x = 0.0f;
	Light.Direction.y = 0.0f;
	Light.Direction.z = 1.0f;
	Light.Range = 1000.0f;

	g_pd3dDevice->SetLight(0,&Light);  //set the light (NEW)
	g_pd3dDevice->LightEnable(0,false); //enables the light (NEW)

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );

	D3DLIGHT9 light2;
	ZeroMemory( &light2, sizeof(D3DLIGHT9) );
	light2.Type       = D3DLIGHT_POINT;
	light2.Diffuse.r  = 1.0f;
	light2.Diffuse.g  = 1.0f;
	light2.Diffuse.b  = 1.0f;
    
	// Point lights have no direction but do have a position
	light2.Position = D3DXVECTOR3(0.0f,0.0f,-5.0f);
    
	// Tell the device about the light and turn it on
	light2.Attenuation0 = 0.1f;
	light2.Range=28.0f;
	g_pd3dDevice->SetLight( 1, &light2 );
	g_pd3dDevice->LightEnable( 1, FALSE );
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::CreateMaterials()
{
   ZeroMemory(&mat1, sizeof(D3DMATERIAL9));
   mat1.Ambient.r = 0.95f;
   mat1.Ambient.g = 0.95f;
   mat1.Ambient.b = 0.95f;
   mat1.Ambient.a = 0.95f;
   mat1.Diffuse.r = 0.95f;
   mat1.Diffuse.g = 0.95f;
   mat1.Diffuse.b = 0.95f;
   mat1.Diffuse.a = 0.6f;

   if( m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == false )
   {
	mat1.Ambient.r=   mat1.Diffuse.r = (float)(this->colWhite.Red()) / 255.0f;
   mat1.Ambient.g=mat1.Diffuse.g = (float)(this->colWhite.Green()) / 255.0f;
   mat1.Ambient.b=mat1.Diffuse.b = (float)(this->colWhite.Blue()) / 255.0f;
   mat1.Ambient.a = mat1.Diffuse.a = 1.0f;
   }

      
	ZeroMemory(&mat2, sizeof(D3DMATERIAL9));
   mat2.Ambient.r = 0.3f;
   mat2.Ambient.g = 0.3f;
   mat2.Ambient.b = 0.3f;
   mat2.Diffuse.a = 0.3f;
   mat2.Diffuse.r = 0.0f;
   mat2.Diffuse.g = 0.0f;
   mat2.Diffuse.b = 0.0f;
   mat2.Diffuse.a = 0.3f;


	ZeroMemory(&mat3, sizeof(D3DMATERIAL9));
   mat3.Ambient.r = 0.3f;
   mat3.Ambient.g = 0.3f;
   mat3.Ambient.b = 0.3f;
   mat3.Diffuse.a = 1.0f;
   mat3.Diffuse.r = 0.0f;
   mat3.Diffuse.g = 0.0f;
   mat3.Diffuse.b = 0.0f;
   mat3.Diffuse.a = 1.0f;

   

   if( m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == false )
   {

	mat2.Ambient.r=   mat2.Diffuse.r = (float)(this->colBlack.Red()) / 255.0f;
   mat2.Ambient.g =mat2.Diffuse.g = (float)(this->colBlack.Green()) / 255.0f;
   mat2.Ambient.b =mat2.Diffuse.b = (float)(this->colBlack.Blue()) / 255.0f;
   mat2.Ambient.a = mat2.Diffuse.a = 1.0f;
   }

   if( m_owner->optionsFrame->m_BoardOptions.useBorderBitmap == false )
   {

	mat3.Ambient.r=   mat3.Diffuse.r = (float)(this->colBorder.Red()) / 255.0f;
   mat3.Ambient.g =mat3.Diffuse.g = (float)(this->colBorder.Green()) / 255.0f;
   mat3.Ambient.b =mat3.Diffuse.b = (float)(this->colBorder.Blue()) / 255.0f;
   mat3.Ambient.a = mat3.Diffuse.a = 1.0f;
   }

   


   ZeroMemory(&matArrow, sizeof(D3DMATERIAL9));
   matArrow.Ambient.r = 0.0f;
   matArrow.Ambient.g = 0.0f;
   matArrow.Ambient.b = 0.0f;
   matArrow.Diffuse.a = 0.0f;
   matArrow.Diffuse.r = 0.0f;
   matArrow.Diffuse.g = 0.0f;
   matArrow.Diffuse.b = 0.0f;
   matArrow.Diffuse.a = 0.0f;

//   if( (m_owner->optionsFrame->m_BoardOptions.showArrows == true) )
   {

	   matArrow.Ambient.r=   matArrow.Diffuse.r = (float)(this->colArrow.Red()) / 255.0f;
   matArrow.Ambient.g =matArrow.Diffuse.g = (float)(this->colArrow.Green()) / 255.0f;
   matArrow.Ambient.b =matArrow.Diffuse.b = (float)(this->colArrow.Blue()) / 255.0f;
   matArrow.Ambient.a = matArrow.Diffuse.a = 1.0f;
   }


   ZeroMemory(&matTxt, sizeof(D3DMATERIAL9));
   matTxt.Ambient.r = 0.0f;
   matTxt.Ambient.g = 0.0f;
   matTxt.Ambient.b = 0.0f;
   matTxt.Diffuse.a = 0.0f;
   matTxt.Diffuse.r = 0.0f;
   matTxt.Diffuse.g = 0.0f;
   matTxt.Diffuse.b = 0.0f;
   matTxt.Diffuse.a = 0.0f;

	//if( m_owner->optionsFrame->m_BoardOptions.useBorderBitmap == false )
	{
	   matTxt.Ambient.r=   matTxt.Diffuse.r = (float)(this->colText.Red()) / 255.0f;
   matTxt.Ambient.g =matTxt.Diffuse.g = (float)(this->colText.Green()) / 255.0f;
   matTxt.Ambient.b =matTxt.Diffuse.b = (float)(this->colText.Blue()) / 255.0f;
   matTxt.Ambient.a = matTxt.Diffuse.a = 1.0f;
	}

   this->newMaterials = false;
   ZeroMemory(&blackMat, sizeof(D3DMATERIAL9));
   blackMat.Ambient.r = 0.0f;
   blackMat.Ambient.g = 0.0f;
   blackMat.Ambient.b = 0.0f;
   blackMat.Diffuse.a = 0.0f;
   blackMat.Diffuse.r = 0.0f;
   blackMat.Diffuse.g = 0.0f;
   blackMat.Diffuse.b = 0.0f;
   blackMat.Diffuse.a = 0.0f;


   

   ZeroMemory(&whiteMat, sizeof(D3DMATERIAL9));
   whiteMat.Ambient.r = 1.0f;
   whiteMat.Ambient.g = 1.0f;
   whiteMat.Ambient.b = 1.0f;
   whiteMat.Ambient.a = 1.0f;
   whiteMat.Diffuse.r = 1.0f;
   whiteMat.Diffuse.g = 1.0f;
   whiteMat.Diffuse.b = 1.0f;
   whiteMat.Diffuse.a = 0.6f;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::SetupBoard()
{
	for( int i = 0; i < 8; i++)
	{
		for( int j = 0; j < 8; j++ )
		{
			board[i][j] = 0;
			if( i == 0 )
			{
				switch(j)
				{
				case 0: board[i][j] = wRook;break;
				case 1:board[i][j] = wKnight;break;
				case 2:board[i][j] = wBishop;break;
				case 3:board[i][j] = wQueen;break;
				case 4:board[i][j] = wKing;break;
				case 5:board[i][j] = wBishop;break;
				case 6:board[i][j] = wKnight;break;
				case 7: board[i][j] = wRook;break;
				//case 8:

				};
			}
			if( i == 1 )
				board[i][j] = wPawn;
			if( i == 6) 
				board[i][j] = bPawn;
			if( i == 7 )
			{
				switch(j)
				{
				case 0: board[i][j] = bRook;break;
				case 1:board[i][j] = bKnight;break;
				case 2:board[i][j] = bBishop;break;
				case 3:board[i][j] = bQueen;break;
				case 4:board[i][j] = bKing;break;
				case 5:board[i][j] = bBishop;break;
				case 6:board[i][j] = bKnight;break;
				case 7: board[i][j] = bRook;break;
				//case 8:

				};
			}
		}
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::mouseReleased( wxMouseEvent& event )
{
	wxClientDC dc(this);
    PrepareDC(dc);
  
	wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );

	//mouseX = x;
	//mouseY = y;

	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * x ) / w  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * y ) / h ) - 1 ) / matProj._22;
	v.z =  1.0f;



	D3DXMATRIX m;
	D3DXVECTOR3 rayOrigin,rayDir;

	D3DXMatrixInverse( &m, NULL, &matView );

	// Transform the screen space pick ray into 3D space
	rayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

	rayOrigin += rayDir*11.0;
			mouseX = rayOrigin.x;
			mouseY = rayOrigin.y;
	
	
	if( rayOrigin.x > (-4.0f) && rayOrigin.y > (-4.0f) &&
		rayOrigin.x < (-4.0f+8.0f) && rayOrigin.y < (8.0f-4.0f))
	//	wxMessageBox(wxString::Format("Hit x: %d, Hit y: %d",(int)(rayOrigin.x+4.0f),(int)(rayOrigin.y+4.0f)),"");
	{
		if( gameState == GameState::SELECTED_PIECE )
		{
			int indx1, indx2;
			if( bFlip )
			{
				theMove.from.file = selX;
				theMove.from.rank = selY;
				theMove.to.file=(int)(rayOrigin.x+4.0f);
				theMove.to.rank=(int)(rayOrigin.y+4.0f);
			}
			else
			{
				theMove.from.file = 7-selX;
				theMove.from.rank = 7-selY;
				theMove.to.file=7-(int)(rayOrigin.x+4.0f);
				theMove.to.rank= 7-(int)(rayOrigin.y+4.0f);
			}

			//if( chess_board->board[theMove.from.file][theMove.from.rank] == wPawn

			bool promote = false;
			bool pinned = false;
			bool moveFound = false;

			bool moveIsLegal = false;
			chess_board->GenLegalMoves();
			for( int i = 0; i < chess_board->numLegalMoves; i++ )
			{
				if( theMove == chess_board->legal_moves[i].mv )
				{
					moveIsLegal = true;
					break;
				}
			}



			if( moveIsLegal )//|| chess_board->IsMoveValid(&theMove, &promote, &pinned) )
			{
				promote = chess_board->IsMovePromotingPawn(&theMove);
				//promote = (theMove.promo!=-1?true:false);
				//m_owner->currentGame->RemoveResultNode();
				DoMove(promote);
				m_owner->DoLookupFromBook();
				m_owner->treeCanvas->MyRefresh();
			}
			else
			{
			//	wxMessageBox("Illegal Move", "");
				for( int j = 0; j < activeEngines.size(); j++ )
				{
					if( activeEngines[j]->IsActive() &&
						activeEngines[j]->IsAnalysing() && mode == 0)
					{
				//		activeEngines[j]->KibitzStartsMove();
						activeEngines[j]->KibitzUserEndsMove();
					}
				}
			}


			gameState = GameState::STATE_ZERO;
		}
		
	}
	else
	{
		for( int j = 0; j < activeEngines.size(); j++ )
		{
			if( activeEngines[j]->IsActive() &&
				activeEngines[j]->IsAnalysing() && mode == 0)
			{
			//	activeEngines[j]->KibitzStartsMove();
				activeEngines[j]->KibitzUserEndsMove();
			}
		}
	}


}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::mouseLeftDown( wxMouseEvent& event )
{
	wxClientDC dc(this);
    PrepareDC(dc);
  
	wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );

	//mouseX = x;
	//mouseY = y;

	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * x ) / w  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * y ) / h ) - 1 ) / matProj._22;
	v.z =  1.0f;



	D3DXMATRIX m;
	D3DXVECTOR3 rayOrigin,rayDir;

	D3DXMatrixInverse( &m, NULL, &matView );

	// Transform the screen space pick ray into 3D space
	rayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

	rayOrigin += rayDir*11.0;
			mouseX = rayOrigin.x;
			mouseY = rayOrigin.y;
	
	
	if( rayOrigin.x > (-4.0f) && rayOrigin.y > (-4.0f) &&
		rayOrigin.x < (-4.0f+8.0f) && rayOrigin.y < (8.0f-4.0f))
	//	wxMessageBox(wxString::Format("Hit x: %d, Hit y: %d",(int)(rayOrigin.x+4.0f),(int)(rayOrigin.y+4.0f)),"");
	{
		if( gameState == GameState::STATE_ZERO )
		{
			int indx1, indx2;
			if( bFlip )
			{
				indx1 = (int)(rayOrigin.x+4.0f);
				indx2 = (int)(rayOrigin.y+4.0f);
			}
			else
			{
				indx1 = 7-(int)(rayOrigin.x+4.0f);
				indx2 = 7-(int)(rayOrigin.y+4.0f);
			}
			if( chess_board->board[indx1][ indx2 ] != 0 )
			{

				selX = (int)(rayOrigin.x+4.0f);
				selY = (int)(rayOrigin.y+4.0f);
				bSelectedPiece = true;
				this->selPieceID = chess_board->board[indx1][ indx2 ];
				gameState = GameState::SELECTED_PIECE;

				for( int j = 0; j < activeEngines.size(); j++ )
				{
					if( activeEngines[j]->IsActive() &&
						activeEngines[j]->IsAnalysing() && mode == 0)
					{
						activeEngines[j]->KibitzStartsMove();
				//		activeEngines[j]->KibitzUserEndsMove();
					}
				}
			}
		}
		else if( gameState == GameState::SELECTED_PIECE )
		{
			int indx1, indx2;
			if( !bFlip )
			{
				indx1 = 7-(int)(rayOrigin.x+4.0f);
				indx2 = 7-(int)(rayOrigin.y+4.0f);
			}
			else
			{
				indx1 = (int)(rayOrigin.x+4.0f);
				indx2 = (int)(rayOrigin.y+4.0f);
			}
			theMove.to.file=indx1;
			theMove.to.rank=indx2;
			bool promote = false;
			bool pinned = false;
			bool moveFound = false;


			if( chess_board->IsMoveValid(&theMove, &promote, &pinned) )
			{
				//m_owner->currentGame->RemoveResultNode();
				DoMove(promote);
				m_owner->DoLookupFromBook();
				gameState = GameState::STATE_ZERO;
			}

		}
		
	}
	/*for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			D3DXVECTOR3 pMin( float(i)-4.0f, float(j)-3.0f, 11.0f);
			D3DXVECTOR3 pMax( float(i)-4.0f, float(j)-3.0f, 11.05f);
			bool bHit = D3DXBoxBoundProbe(&pMin, &pMax, &rayOrigin, &rayDir );
			if( bHit )
			{
				wxMessageBox(wxString::Format("Hit x: %d, Hit y: %d",i,j),"");
			}
		}
	}*/

}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::mouseMove( wxMouseEvent& event )
{
	wxClientDC dc(this);
    PrepareDC(dc);
  
	wxPoint pos = event.GetPosition();
    long x = dc.DeviceToLogicalX( pos.x );
    long y = dc.DeviceToLogicalY( pos.y );

	D3DXVECTOR3 v;
	v.x =  ( ( ( 2.0f * x ) / w  ) - 1 ) / matProj._11;
	v.y = -( ( ( 2.0f * y ) / h ) - 1 ) / matProj._22;
	v.z =  1.0f;



	D3DXMATRIX m;
	D3DXVECTOR3 rayOrigin,rayDir;

	D3DXMatrixInverse( &m, NULL, &matView );

	// Transform the screen space pick ray into 3D space
	rayDir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrigin.x = m._41;
	rayOrigin.y = m._42;
	rayOrigin.z = m._43;

	rayOrigin += rayDir*11.0;
			mouseX = rayOrigin.x;
			mouseY = rayOrigin.y;

}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3DXVECTOR2 D3D9Canvas::WorldToScreen(D3DXVECTOR3 worldPoint)
{
	D3DXVECTOR3 vecDir(0.0f,0.0f,-1.0f);
	worldPoint -= vecDir*11.0f;
	
	D3DXVECTOR2 v;
	v.x = ((worldPoint.x *matProj._11 +1 )*w) / 2.0f;
	v.y =-((worldPoint.y *matProj._22 +1)*h) / 2.0f;

	return v;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
LPD3DXFONT D3D9Canvas::CreateFont()
{
	LPD3DXFONT pFont;
	D3DXCreateFont( g_pd3dDevice, 20, 0, FW_BOLD, 0, 
		FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		TEXT("Arial"), &pFont );

	return pFont;
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::DoMove(bool &promote)
{
			//moveFound = true;


			string epn="    ";
			char buff[10];
			
			switch( theMove.from.file )
			{
			case 0: epn[0] = 'a'; break;
			case 1:epn[0] = 'b'; break;
			case 2: epn[0] = 'c'; break;
			case 3: epn[0] = 'd'; break;
			case 4: epn[0] = 'e'; break;
			case 5: epn[0] = 'f'; break;
			case 6: epn[0] = 'g'; break;
			case 7: epn[0] = 'h'; break;
			
			}
			itoa(theMove.from.rank, buff, 10);
			epn[1] = buff[0];
			switch( theMove.to.file )
			{
			case 0: epn[2] = 'a'; break;
			case 1:epn[2] = 'b'; break;
			case 2: epn[2] = 'c'; break;
			case 3: epn[2] = 'd'; break;
			case 4: epn[2] = 'e'; break;
			case 5: epn[2] = 'f'; break;
			case 6: epn[2] = 'g'; break;
			case 7: epn[2] = 'h'; break;
			
			}
			itoa(theMove.to.rank, buff, 10);
			epn[3] = buff[0];
			int white_black = 0;
			if( chess_board->mov == 0 || chess_board->mov == 2 )
				white_black = 0;
			else white_black = 1;

			int promotePieceID = 0;
			if( promote && (m_owner->optionsFrame->m_GameOptions.alwaysPromoteToQueen == false))
			{
				PromotionSelectionDialaog dialog(this->m_parent, chess_board,wxID_ANY,
					wxT("Select Piece to Promote to"));

				dialog.SetPromoteID( &promotePieceID );
				dialog.ShowModal();

				//switch( promotePieceID )
				//epn+=
			}

			
			
			string pgn = chess_board->EPN_Move_to_PGN_Move(epn, promote, promotePieceID);
			
			string fen = chess_board->Fen();
			PGN::MoveInserter mv(pgn,white_black, fen, chess_board->GetLastMove());

			if( white_black == WHITE )
			{
				m_owner->gameClock->SetWhiteBlack(BLACK);
			}
			else
			{
				m_owner->gameClock->SetWhiteBlack(WHITE);
			}

			int repetitions = 0;
			if( this->m_owner->currentGame->pCurrMove == this->m_owner->currentGame->pBase )
				this->m_owner->currentGame->pCurrMove = this->m_owner->currentGame->pRoot->InsertMove( &mv, &repetitions);
			else
				this->m_owner->currentGame->pCurrMove = this->m_owner->currentGame->GetCurrentMove()->InsertMove( &mv, &repetitions);
			


			int gameStatus = chess_board->GetGameStatus();

			switch( gameStatus )
			{
			case PLAYING:
			case KINGSAFE:
				break;
			case CHECKMATE:
				{
					m_gameState = CHECKMATE;

					if( chess_board->mov == 0 || chess_board->mov == 2 )
					{
						m_displayGameResult = true;
						gameResultString = "Checkmate";
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "Black "+gameResultString+"s!","Game Over");
						}
						m_owner->currentGame->result = "0-1";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					else
					{
						gameResultString = "Checkmate";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "White "+gameResultString+"s!","Game Over");
						}
						m_owner->currentGame->result = "1-0";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					m_owner->currentGame->WriteResultIntoHeader();
					m_owner->gameClock->Stop();
					m_owner->RecreateGameList();
				}
				break;
			case STALEMATE:
				{
					m_gameState = STALEMATE;

					if( chess_board->mov == 0 || chess_board->mov == 2 )
					{
						gameResultString = "Stalemate";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "White "+gameResultString+"d!","Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					else
					{
						gameResultString = "Stalemate";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( "Black" + gameResultString +"d!","Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					m_owner->currentGame->WriteResultIntoHeader();
					m_owner->RecreateGameList();
					m_owner->gameClock->Stop();
					
				}
				break;
			case DRAW_BY_50_MOVE:
				{
					m_gameState = DRAW_BY_50_MOVE;
					if( chess_board->mov == 0 || chess_board->mov == 2 )
					{
						gameResultString = "Drawn, 50 moves without a pawn move or capture!";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( gameResultString,"Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteResultIntoHeader();
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					else
					{
						gameResultString = "Drawn, 50 moves without a pawn move or capture!";
						m_displayGameResult = true;
						if( m_owner->optionsFrame->m_GameOptions.showMessageBoxAtGameEnd == true )
						{
							wxMessageBox( gameResultString,"Game Over");
						}
						m_owner->currentGame->result = "1/2-1/2";
						m_owner->currentGame->AddResultNode(m_owner->currentGame->result);
						m_owner->currentGame->WriteResultIntoHeader();
						m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
					}
					m_owner->gameClock->Stop();
					m_owner->RecreateGameList();
				}
				break;
			case CHECK:
				{
					m_gameState = CHECK;

				}
				break;
			}	

			if( repetitions >= 3 )
			{
				gameResultString = "Draw By Repetition";
				wxMessageBox("Draw By Repetition","");
				m_gameState = DRAW_BY_REPETITION;
				m_owner->currentGame->WriteTerminationIntoHeader(gameResultString);
				m_owner->gameClock->Stop();
			}

			chess_board->SetBoard(fen);
			//m_owner->currentGame->EnumerateVariations();
			this->m_owner->my_canvas->move_count++;
			if( m_owner->my_canvas->white_black_move == WHITE )
				m_owner->my_canvas->white_black_move = BLACK;
			else m_owner->my_canvas->white_black_move = WHITE;
			
			if( this->m_owner->my_canvas->move_count < 10 )
				this->m_owner->my_canvas->Scroll(0,0);
			this->m_owner->my_canvas->ScrollToCursor();
			this->m_owner->my_canvas->Refresh(true);
			this->m_owner->my_canvas->needsRefresh = true;
			this->m_owner->moveDataCanvas->Refresh(true);
			this->m_owner->noteFrame->Refresh(true);
			//Refresh(false);
			needsRefresh = true;
			droppedPieceLastMove = true;
			
			

			if( (mode == 1) && (m_owner->engineVsHumanMatch) )
			{
				whoseMove = WAIT_COMPUTER;
				if( (m_owner->playersOpponent) && (m_owner->playersOpponent->IsActive()) )
				{

					m_owner->playersOpponent->PlayerMoves();
					
				}

				if( m_owner->gameClock->IsPaused() )
				{
					m_owner->gameClock->StartGame();
					m_owner->gameClock->UnPause();
				}
			}
			else
			{

				for( int j = 0; j < activeEngines.size(); j++ )
				{
					if( activeEngines[j]->IsActive() && 
						activeEngines[j]->IsAnalysing() && mode == 0)
					{
				//		activeEngines[j]->KibitzStartsMove();
						activeEngines[j]->KibitzUserEndsMove();
					}
				}
			}
#ifdef USE_CANVAS_SET_FOCUS
		//	this->m_owner->my_canvas->SetFocus();
#endif
}




//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::BackAnimation()
{
	if( this->m_owner->optionsFrame->m_GameOptions.animate == false || this->mode == 1)
	{
		this->bAnimating = false;
		return;
	}
	needsRefresh = true;
	bAnimating = true;
	bFowardsAnim = false;
	//animationStart = wxGetLocalTimeMillis ();
	//MyChessMove* pMov = NULL;
	if( chess_board->mov == BLACK )
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->blackMove;

	}
	else
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->pNext->whiteMove;
	}

	if( bFlip )
	{
		currAnimX = (pMov->to.file)-4.0f;
		currAnimY = (pMov->to.rank)-3.0f;
	}
	else
	{
		currAnimX = (7-pMov->to.file)- 4.0f;
		currAnimY = (7-pMov->to.rank) - 3.0f;
	}

	selX = pMov->from.file;
	selY = pMov->from.rank;

	chess_board->SquareHasPiece(pMov->from.file, pMov->from.rank, &selPieceID );
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9Canvas::StartAnimation()
{
	if( this->m_owner->optionsFrame->m_GameOptions.animate == false || this->mode == 1)
	{
		bAnimating = false;
		return;
	}
	bAnimating = true;
	bFowardsAnim = true;
	needsRefresh = true;
	//animationStart = wxGetLocalTimeMillis ();
	//MyChessMove* pMov = NULL;
	if( chess_board->mov == BLACK )
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->whiteMove;
	}
	else
	{
		if( m_owner->currentGame != NULL )
			pMov = &m_owner->currentGame->pCurrMove->blackMove;
	}

	if( bFlip )
	{
		currAnimX = pMov->from.file - 4.0f;
		currAnimY = pMov->from.rank - 3.0f;
	}
	else
	{
		currAnimX = (7-pMov->from.file) - 4.0f;
		currAnimY = (7-pMov->from.rank) - 3.0f;
	}

	selX = pMov->to.file;
	selY = pMov->to.rank;

	chess_board->SquareHasPiece(pMov->to.file, pMov->to.rank, &selPieceID );
}