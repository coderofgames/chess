
#include "D3D9Base.h"


BEGIN_EVENT_TABLE(D3D9BaseCanvas, wxWindow)
	EVT_SIZE(D3D9BaseCanvas::OnSize)
END_EVENT_TABLE()


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3D9BaseCanvas::D3D9BaseCanvas(wxWindow *parent, wxWindowID id, const wxPoint& pos,
        const wxSize& size, long style, const wxString& name )
		: wxWindow(parent, id, pos, size, style|wxFULL_REPAINT_ON_RESIZE|wxWS_EX_PROCESS_UI_UPDATES, name)
{
    m_init = false;
   g_hWnd = (HWND)GetHWND();
   resizing = false;
   g_pd3dDevice =NULL;
   //CreateDevice();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3D9BaseCanvas::~D3D9BaseCanvas()
{
	if( this->m_render_timer )
	{
		this->m_render_timer->Stop();
		delete this->m_render_timer;
		this->m_render_timer = NULL;
	}

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 D3D9BaseCanvas::CreateTexture(wxString textureName, wxUint32 filterType, wxUint32 mipFilter)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;
	HRESULT res = D3DXCreateTextureFromFileEx(
	g_pd3dDevice,
	textureName,0,0, 0,0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED,
	D3DX_DEFAULT,D3DX_DEFAULT, 0, 0, 0, &pTexture);

	if( res != D3D_OK )
	{
		wxMessageBox("Error Creating Texture: " + textureName,"Texture Error");
	}
	return pTexture;
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9BaseCanvas::EnableAlphaBlending( wxUint32 blendOne, wxUint32 blendTwo )
{
	// Enable alpha blending.
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 
							TRUE);

	// Set the source blend state.
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, 
							D3DBLEND_SRCALPHA);

	// Set the destination blend state.
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, 
							D3DBLEND_INVSRCALPHA);
}


//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9BaseCanvas::CreateDevice()
{
	GetClientSize(&w, &h);
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

    
    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

	HRESULT hResult;

	DWORD total;

	hResult = g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
													D3DDEVTYPE_HAL,
													d3ddm.Format,
													true,
													D3DMULTISAMPLE_4_SAMPLES,
													&total);


	if(SUCCEEDED(hResult))
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		d3dpp.MultiSampleQuality = total - 1;
	}


    d3dpp.Windowed               = TRUE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
	d3dpp.BackBufferWidth = w;
	d3dpp.BackBufferHeight = h;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	


    g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                          /*D3DCREATE_SOFTWARE_VERTEXPROCESSING*/
						  D3DCREATE_HARDWARE_VERTEXPROCESSING,
                          &d3dpp, &g_pd3dDevice );
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9BaseCanvas::RecreateDevice()
{
	GetClientSize(&w, &h);
    g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );

	HRESULT hResult;

	DWORD total;

	hResult = g_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
													D3DDEVTYPE_HAL,
													d3ddm.Format,
													true,
													D3DMULTISAMPLE_4_SAMPLES,
													&total);


	if(SUCCEEDED(hResult))
	{
		d3dpp.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		d3dpp.MultiSampleQuality = total - 1;
	}


    d3dpp.Windowed               = TRUE;
    d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat       = d3ddm.Format;
	d3dpp.BackBufferWidth = w;
	d3dpp.BackBufferHeight = h;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
	


    g_pd3dDevice->Reset(&d3dpp);
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9BaseCanvas::Clear(wxUint32 flags, wxColour &col)
{
	g_pd3dDevice->Clear( 0, NULL, flags,/*D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,*/
                         D3DCOLOR_ARGB(255,col.Red(),col.Green(),col.Blue()), 1.0f, 0 );
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9BaseCanvas::BeginScene()
{
	g_pd3dDevice->BeginScene();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3D9BaseCanvas::EndScene()
{
    g_pd3dDevice->EndScene();
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}



//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
D3DRenderTimer::D3DRenderTimer(D3D9BaseCanvas* pane) : wxTimer()
{
    D3DRenderTimer::pane = pane;
}
 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3DRenderTimer::Notify()
{
    //pane->Refresh(true);
	//if( pane->gameState == BoardCanvas::GameState::SELECTED_PIECE )
	//if( pane->MouseOverWin )
	//if( !pane->resizing )
	if( pane )
	{

		pane->Refresh();
	}
	//pane->Notified = true;
}
 
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void D3DRenderTimer::start()
{
    wxTimer::Start(15);
}
