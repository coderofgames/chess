#include "D3D9VariationBoard.h"


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

struct TXVertex2
{
public:
  float x, y, z;
  DWORD diffuse;
  float u, v;
};

TXVertex2 Vunit2[] = {
  {-1.0,1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 0.0,0.0},
  {1.0,1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 1.0,0.0},
  {1.0,-1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 1.0,1.0},
  {-1.0,-1.0,1.0, D3DCOLOR_ARGB(255,255,255,255), 0.0,1.0}
};

TXVertex2 QuadTXVertex2es2[] =
{
	{0.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0.0f},
	{1.0f, -1.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 1.0f},
	{0.0f, -1.0f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 1.0f}

};

TXVertex2 ArrowHeadVertexes2[] = 
{

	{0.0f, 0.7f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 0.0f},
	{0.3f, -0.5f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 1.0f, 1.0f},
	{0.0f, -1.7f, 0.0f, D3DCOLOR_ARGB(255,255,255,255), 0.0f, 1.0f}

};

#define D3DFVF_TXVertex2 ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

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


Vertex QuadVertexes2[] =
{
	{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
	{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
	{0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
	{1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
	{0.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f}

};



// ---------------------------------------------------------------------------
// TestD3D9VariationBoard
// ---------------------------------------------------------------------------

BEGIN_EVENT_TABLE(D3D9VariationBoard, D3D9BaseCanvas)
    EVT_SIZE(D3D9VariationBoard::OnSize)
    EVT_PAINT(D3D9VariationBoard::OnPaint)
    EVT_ERASE_BACKGROUND(D3D9VariationBoard::OnEraseBackground)
    EVT_IDLE(D3D9VariationBoard::OnIdle)
	EVT_LEFT_DOWN(D3D9VariationBoard::mouseLeftDown)
	EVT_MOTION (D3D9VariationBoard::mouseMove)
	EVT_LEFT_UP(D3D9VariationBoard::mouseReleased)
END_EVENT_TABLE()

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3D9VariationBoard::D3D9VariationBoard(MyFrame *parent, wxWindow *winParent, wxWindowID id,
    const wxPoint& pos, const wxSize& size, long style, const wxString& name)
    : D3D9BaseCanvas(winParent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE, name)
	//BaseGraphicsBoard(
{

	this->m_owner = parent;
   g_fTime = 0;
   bSelectedPiece = false;

  
   bFlip = true;
   pMov = NULL;
   this->m_render_timer = NULL;

   g_pVertexBuffer = NULL;

 
}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3D9VariationBoard::~D3D9VariationBoard()
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
void D3D9VariationBoard::OnIdle(wxIdleEvent &event)
{
  //  event.RequestMore(true);
    
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9VariationBoard::OnPaint( wxPaintEvent& WXUNUSED(event) )
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
void D3D9VariationBoard::OnSize(wxSizeEvent& event)
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
void D3D9VariationBoard::CreateLines()
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
void D3D9VariationBoard::OnEraseBackground(wxEraseEvent& WXUNUSED(event))
{
    // Do nothing, to avoid flashing on MSW
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9VariationBoard::InitD3D()
{
	if( g_pd3dDevice == NULL )
		CreateDevice();


	CreateQuad();

	LoadTiles();

//	CreateLines();

	LoadManagedTextures();
	

	quadLineVB = this->CreateQuadTemplate<TXVertex2>(D3DFVF_TXVertex2,QuadTXVertex2es2);



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
void D3D9VariationBoard::ResetProjectionMode()
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
void D3D9VariationBoard::Render()
{
	//wxColour clearCol(100,50,255,1);
	Clear(D3DCLEAR_TARGET , this->m_backgroundColour);
//	g_pd3dDevice->Clear(0,0,
  // D3DCOLOR_COLORVALUE(100.0f,50.0f,255.0f,1.0f), 1.0f, 0 );



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
//	if( m_owner->optionsFrame->m_BoardOptions.useBorderBitmap == true )
	{
		g_pd3dDevice->SetTexture(0, border1 );
		g_pd3dDevice->SetMaterial(&mat1);
	}
/*	else
	{
		g_pd3dDevice->SetTexture(0, NULL );
		g_pd3dDevice->SetMaterial(&mat3);
	}*/
	
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );

	g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);

	//D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 0.25, 0.25, 1.0f);
//	
/*	if( this->m_owner->optionsFrame->m_BoardOptions.showCoordinates )
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
	}*/


	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{

			D3DXMatrixIdentity(&matTrans);
			D3DXMatrixTranslation( &matTrans, i*1.0f-4.0f, j*1.0f-3.0f, 11.0f );
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );

/*			if( this->m_owner->optionsFrame->m_BoardOptions.useBoardBitmaps == true )
			{
				g_pd3dDevice->SetTexture(0, g_pTexture[i][j] );
			}
			else*/
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

/*	if( m_owner->optionsFrame->m_BoardOptions.showLines == true )
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
	}*/

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



	bool drawArrows = true;

	D3DXVECTOR3 arrowStart;//(pMov->from.file - 4.0f+0.5, pMov->from.rank - 3.0f-0.5 , 11.0f);
	D3DXVECTOR3 arrowEnd;//(pMov->to.file - 4.0f+0.5, pMov->to.rank - 3.0f-0.5 , 11.0f);
	
//	if( bFlip )
	{
		arrowStart.x = pMov->from.file - 4.0f+0.5;
		arrowStart.y = pMov->from.rank - 3.0f-0.5;
		arrowEnd.x =   pMov->to.file - 4.0f+0.5;
		arrowEnd.y =   pMov->to.rank - 3.0f-0.5;
	}
	/*else
	{
		arrowStart.x = (7-pMov->from.file) - 4.0f+0.5;
		arrowStart.y = (7-pMov->from.rank) - 3.0f-0.5;
		arrowEnd.x =   (7-pMov->to.file) - 4.0f+0.5;
		arrowEnd.y =   (7-pMov->to.rank) - 3.0f-0.5;
	}*/


	g_pd3dDevice->SetStreamSource(0, quadLineVB,0,sizeof(TXVertex2));
	g_pd3dDevice->SetFVF( D3DFVF_TXVertex2 );
	g_pd3dDevice->SetMaterial(&matArrow);
	g_pd3dDevice->SetTexture( 0, /*yellowTex*/0 );
	




	g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(Vertex) );
    g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	g_pd3dDevice->SetMaterial(&whiteMat);
	for( int i = 0; i < 8; i++ )
	{
		for( int j = 0; j < 8; j++ )
		{
			double step_size = double(moves[indx].w - moves[indx].y) / 20.0f;

			int indx1, indx2;
		//	if( bFlip )
			{
				indx1 = i;
				indx2 = j;
			}
		/*	else
			{
				indx1 = 7-i;
				indx2 = 7-j;
			}*/


			{
				D3DXMatrixIdentity(&matTrans);
				D3DXMatrixTranslation( &matTrans, i*1.0f-4.0f, j*1.0f-3.0f, 11.0f );
				g_pd3dDevice->SetTransform( D3DTS_WORLD, &matTrans );
				 
				bool bdraw = true;

				switch( internalBoard.board[indx1][indx2] )
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









	   g_pd3dDevice->SetStreamSource( 0, g_pVertexBuffer, 0, sizeof(Vertex) );
    g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );

	//D3DXMATRIX matIdent;
	D3DXMatrixIdentity(&matTrans);
	
	
	

	//g_pd3dDevice->SetTransform(D3DTS_WORLD,&matTrans);

	/*		D3DXMatrixIdentity(&matTrans);
			g_pd3dDevice->SetTransform(D3DTS_WORLD,&matTrans);
			D3DXMatrixTranslation( &matTrans, -4.0f, -3.0f, 12.0f );
			g_pd3dDevice->SetTexture(0, NULL);
			g_pd3dDevice->SetMaterial(&blackMat);
			ppLine->SetWidth(10.0f);
			ppLine->SetGLLines(true);
	this->ppLine->Begin();
	this->ppLine->DrawTransform(lines, 36, &matTrans, 0xFFFFFFFF);
	this->ppLine->End();
	*/
/*const DWORD D3DFVF_TL = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
//g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
this->EnableAlpha(false);
this->LightingEnabled(false);
//g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
g_pd3dDevice->SetFVF( D3DFVF_XYZ );
g_pd3dDevice->SetTexture( 0, blackTex );
//g_pd3dDevice->SetMaterial(&blac
for( int i = 0; i < 36; i+=2 )
	g_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, lines[i], sizeof( D3DXVECTOR3 ) );
	//g_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, lines[16], sizeof( D3DXVECTOR3 ) );
	//g_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 1, lines[18], sizeof( D3DXVECTOR3 ) );
	//g_pd3dDevice->DrawPrimitiveUP( D3DPT_LINELIST, 2, lines[34], sizeof( D3DXVECTOR3 ) );
	//g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST,0, 2);
	
*/

	EndScene();

    g_fTime += 0.002f;
}



void D3D9VariationBoard::ChangeTextures(wxString &name)
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
void D3D9VariationBoard::LoadManagedTextures()
{

	g_pWhitePieces[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/wp.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/wb.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/wn.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/wr.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/wq.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pWhitePieces[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/wk.png", D3DX_DEFAULT, D3DX_DEFAULT);
	
	g_pBlackPieces[0] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/bp.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[1] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/bb.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[2] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/bn.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[3] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/br.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[4] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/bq.png", D3DX_DEFAULT, D3DX_DEFAULT);
	g_pBlackPieces[5] = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/PieceThemes/Book/bk.png", D3DX_DEFAULT, D3DX_DEFAULT);

	border1 = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/border_frame.png", D3DX_DEFAULT, D3DX_DEFAULT);

	blackTex = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/black.png",D3DX_DEFAULT, D3DX_DEFAULT); 

	yellowTex = CreateTexture(wxStandardPaths::Get().GetResourcesDir()+"/images/yellow.png",D3DX_DEFAULT, D3DX_DEFAULT); 


}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9VariationBoard::CreateQuad()
{
	g_pVertexBuffer = CreateVertexBuffer<Vertex>(6, D3DFVF_CUSTOMVERTEX);

	void *pVertices = NULL;

    g_pVertexBuffer->Lock( 0, sizeof(QuadVertexes2), (void**)&pVertices, 0 );
    memcpy( pVertices, QuadVertexes2, sizeof(QuadVertexes2) );
    g_pVertexBuffer->Unlock();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9VariationBoard::LoadTiles()
{
	//D3DX9LoadTexture(
	for( char a = 'a'; a<'i'; a++ )
	{
		for( char n = '1'; n < '9'; n++ )
		{
			int indx1 = a-'a';
			int indx2 = n-'1';

			wxString path = "";
			if( (indx1+indx2) %2 != 1 )
			{
				path += "images/squares/indi/Fiddleback Makore/";
				path += a;
				path += n;
				path += ".png";
			}
			else
			{
				path += "images/squares/indi/Burl Walnut/";
				path += a;
				path += n;
				path += ".png";
			}
	//		D3DXCreateTextureFromFile(g_pd3dDevice,
    //                      path.c_str(),
    //                      &g_pTexture[indx1][indx2]);

			D3DXCreateTextureFromFileEx(
			  g_pd3dDevice,
			  path.c_str(),115,115, 0,0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
			  D3DX_FILTER_POINT,D3DX_DEFAULT, 0, 0, 0, &g_pTexture[indx1][indx2]);


		}
	}
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9VariationBoard::CreateManagedResources()
{
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9VariationBoard::CreateLights()
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
void D3D9VariationBoard::CreateMaterials()
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

   

 

   if( m_owner->optionsFrame->m_BoardOptions.useBorderBitmap == false )
   {

	/*mat3.Ambient.r=   mat3.Diffuse.r = (float)(this->colBorder.Red()) / 255.0f;
   mat3.Ambient.g =mat3.Diffuse.g = (float)(this->colBorder.Green()) / 255.0f;
   mat3.Ambient.b =mat3.Diffuse.b = (float)(this->colBorder.Blue()) / 255.0f;
   mat3.Ambient.a = mat3.Diffuse.a = 1.0f;*/
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

/*	   matArrow.Ambient.r=   matArrow.Diffuse.r = (float)(this->colArrow.Red()) / 255.0f;
   matArrow.Ambient.g =matArrow.Diffuse.g = (float)(this->colArrow.Green()) / 255.0f;
   matArrow.Ambient.b =matArrow.Diffuse.b = (float)(this->colArrow.Blue()) / 255.0f;
   matArrow.Ambient.a = matArrow.Diffuse.a = 1.0f;*/
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
void D3D9VariationBoard::SetupBoard()
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
void D3D9VariationBoard::mouseReleased( wxMouseEvent& event )
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
/*		if( gameState == GameState::SELECTED_PIECE )
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
			internalBoard.GenLegalMoves();
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
			}*/


		//	gameState = GameState::STATE_ZERO;
		//}
		
	}
	else
	{
	/*	for( int j = 0; j < activeEngines.size(); j++ )
		{
			if( activeEngines[j]->IsActive() &&
				activeEngines[j]->IsAnalysing() && mode == 0)
			{
			//	activeEngines[j]->KibitzStartsMove();
				activeEngines[j]->KibitzUserEndsMove();
			}
		}*/
	}


}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9VariationBoard::mouseLeftDown( wxMouseEvent& event )
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
//		if( gameState == GameState::STATE_ZERO )
		{
			int indx1, indx2;
		//	if( bFlip )
			{
				indx1 = (int)(rayOrigin.x+4.0f);
				indx2 = (int)(rayOrigin.y+4.0f);
			}
	/*		else
			{
				indx1 = 7-(int)(rayOrigin.x+4.0f);
				indx2 = 7-(int)(rayOrigin.y+4.0f);
			}*/
			if( internalBoard.board[indx1][ indx2 ] != 0 )
			{

				selX = (int)(rayOrigin.x+4.0f);
				selY = (int)(rayOrigin.y+4.0f);
				bSelectedPiece = true;
				//this->selPieceID = chess_board->board[indx1][ indx2 ];
				//gameState = GameState::SELECTED_PIECE;

	/*			for( int j = 0; j < activeEngines.size(); j++ )
				{
					if( activeEngines[j]->IsActive() &&
						activeEngines[j]->IsAnalysing() && mode == 0)
					{
						activeEngines[j]->KibitzStartsMove();
				//		activeEngines[j]->KibitzUserEndsMove();
					}
				}*/
			}
		}
		//else if( gameState == GameState::SELECTED_PIECE )
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
			/*theMove.to.file=indx1;
			theMove.to.rank=indx2;*/
			bool promote = false;
			bool pinned = false;
			bool moveFound = false;


	/*		if( chess_board->IsMoveValid(&theMove, &promote, &pinned) )
			{
				DoMove(promote);
				m_owner->DoLookupFromBook();
				gameState = GameState::STATE_ZERO;
			}*/

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
void D3D9VariationBoard::mouseMove( wxMouseEvent& event )
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
D3DXVECTOR2 D3D9VariationBoard::WorldToScreen(D3DXVECTOR3 worldPoint)
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
LPD3DXFONT D3D9VariationBoard::CreateFont()
{
	LPD3DXFONT pFont;
	D3DXCreateFont( g_pd3dDevice, 20, 0, FW_BOLD, 0, 
		FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
		TEXT("Arial"), &pFont );

	return pFont;
}






