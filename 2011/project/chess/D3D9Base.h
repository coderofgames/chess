#ifndef D3D9_BASE_H
#define D3D9_BASE_H

#include "headers.h"
#include <d3d9.h>
#include <d3dx9.h>


class D3D9BaseCanvas;


class D3DRenderTimer : public wxTimer
{
    D3D9BaseCanvas* pane;
public:
    D3DRenderTimer(D3D9BaseCanvas* pane);
    void Notify();
    void start();
};

class D3D9BaseCanvas : public wxWindow
{
public:
    D3D9BaseCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("TestGLCanvas"));

    ~D3D9BaseCanvas();


	bool resizing;

	void CreateDevice();
	void RecreateDevice();

	void SetLinearFiltering()
	{
		g_pd3dDevice->SetSamplerState(0,
										D3DSAMP_MINFILTER,
										//D3DTEXF_POINT);
										D3DTEXF_LINEAR);
		g_pd3dDevice->SetSamplerState(0,
										D3DSAMP_MAGFILTER,
										//D3DTEXF_POINT);
										D3DTEXF_LINEAR);
	}

	void StopTimer()
	{
		if( m_render_timer && m_render_timer->IsRunning() )
			m_render_timer->Stop();
	}

	void EnableAlpha(bool val)
	{
		g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, val );
	}
	void EnableAlphaBlending( wxUint32 srcBlend = D3DBLEND_SRCALPHA, wxUint32 dstBlend = D3DBLEND_INVSRCALPHA );
	void ZBufferEnabled(bool value)
	{
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, value);
	}

	void EnableCull(wxUint32 cullFlag)
	{
		g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, cullFlag);
	}
	void LightingEnabled(bool value)
	{
		g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, value);
	}

	LPDIRECT3DTEXTURE9 CreateTexture(wxString textureName,wxUint32 filterType= D3DX_DEFAULT, wxUint32 mipFilter= D3DX_DEFAULT);

	template< class T >
	LPDIRECT3DVERTEXBUFFER9 CreateVertexBuffer( wxInt32 size, wxInt32 fvf); 

	template< class T >
	LPDIRECT3DVERTEXBUFFER9 CreateQuadTemplate(wxInt32 fvf, T* verts )
	{
		LPDIRECT3DVERTEXBUFFER9 pQuadVertsBuffer;
		pQuadVertsBuffer = CreateVertexBuffer<T>(6, fvf);

		void *pVertices = NULL;

		pQuadVertsBuffer->Lock( 0, sizeof(T)*6, (void**)&pVertices, 0 );
		memcpy( pVertices, verts, sizeof(T)*6 );
		pQuadVertsBuffer->Unlock();
		return pQuadVertsBuffer;
	}

	void Clear(wxUint32 flags, wxColour &col);
	void BeginScene();
	void EndScene();

protected:
	//void OnSize(wxSizeEvent& event);
	void OnSize(wxSizeEvent& event){}
	void OnEraseBackground(wxEraseEvent& event){}
	void OnIdle( wxIdleEvent& event ){}

	bool m_init;

	int w, h;

	HWND                    g_hWnd;//          = NULL;
	LPDIRECT3D9             g_pD3D ;//         = NULL;
	LPDIRECT3DDEVICE9       g_pd3dDevice;//    = NULL;
	D3DDISPLAYMODE d3ddm;
	D3DRenderTimer *m_render_timer;

	D3DXMATRIX matProj;
	D3DXMATRIX matView;
	
	double mouseX;
	double mouseY;
	DECLARE_EVENT_TABLE()
};

template< class T >
LPDIRECT3DVERTEXBUFFER9 D3D9BaseCanvas::CreateVertexBuffer( wxInt32 size, wxInt32 fvf)
{
	LPDIRECT3DVERTEXBUFFER9 pVB;
	g_pd3dDevice->CreateVertexBuffer( size*sizeof(T),0, fvf,
                                      D3DPOOL_MANAGED, &pVB, NULL );

	return pVB;
}


struct Vertex
{
	/*Vertex( float X, float Y, float Z, float NX, float NX, float NZ, float TU, float TV )
	{
		x=X;
		y=Y;
		z=Z;
		nx=NX;
		ny=NY;
		nz=NZ;
		u=TU;
		v=TV;
	}*/
    float x, y, z;
	float nx, ny, nz;
	//DWORD dwColor;
	float u,v;
};


#endif