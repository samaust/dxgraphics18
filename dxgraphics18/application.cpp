// file name:	capplication.cpp
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#define D3D_DEBUG_INFO

#include "main.h"
#include "logging.h"

#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"

typedef IDirect3D9* (WINAPI * LPDIRECT3DCREATE9) (UINT);
typedef INT         (WINAPI * LPD3DPERF_BEGINEVENT)(D3DCOLOR, LPCWSTR);
typedef INT         (WINAPI * LPD3DPERF_ENDEVENT)(void);
typedef VOID        (WINAPI * LPD3DPERF_SETMARKER)(D3DCOLOR, LPCWSTR);
typedef VOID        (WINAPI * LPD3DPERF_SETREGION)(D3DCOLOR, LPCWSTR);
typedef BOOL        (WINAPI * LPD3DPERF_QUERYREPEATFRAME)(void);
typedef VOID        (WINAPI * LPD3DPERF_SETOPTIONS)( DWORD dwOptions );
typedef DWORD       (WINAPI * LPD3DPERF_GETSTATUS)( void );

static HMODULE s_hModD3D9 = NULL;
static LPDIRECT3DCREATE9 s_DynamicDirect3DCreate9 = NULL;
static LPD3DPERF_BEGINEVENT s_DynamicD3DPERF_BeginEvent = NULL;
static LPD3DPERF_ENDEVENT s_DynamicD3DPERF_EndEvent = NULL;
static LPD3DPERF_SETMARKER s_DynamicD3DPERF_SetMarker = NULL;
static LPD3DPERF_SETREGION s_DynamicD3DPERF_SetRegion = NULL;
static LPD3DPERF_QUERYREPEATFRAME s_DynamicD3DPERF_QueryRepeatFrame = NULL;
static LPD3DPERF_SETOPTIONS s_DynamicD3DPERF_SetOptions = NULL;
static LPD3DPERF_GETSTATUS s_DynamicD3DPERF_GetStatus = NULL;


extern ID3DXEffect*            g_pEffect;        // D3DX effect interface
extern D3DXHANDLE              g_hTRenderScene;
/*!
 *	Set default values and invoke window and D3D creation.
 */
Application::Application(void)
{

	_object = NULL;
	_device = NULL;

	_width = 1920;// 3840; // 1920; //3840; // 640
	_height = 1080;// 2160; // 1080; //2160; // 480
	_colorFormat = D3DFMT_X8R8G8B8;// D3DFMT_R5G6B5;
	
	_multisampling = D3DMULTISAMPLE_NONE;

	_depthStencilFormat = D3DFMT_D24X8; //D3DFMT_D16;
	
	_vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;; // D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	_anisotropy = 2; // 0 to 2

	_fieldOfView = D3DXToRadian(75); // ;D3DX_PI / 4.0f;
	_nearPlane = 0.05f;
	_farPlane = 1000.0f;
	_aspectRatio = (float)_width / (float)_height;

	_running = true;

	initWindow();
	initDirect3D();
}

/*!
 *	Clean up D3D and window.
 */
Application::~Application(void)
{
	killDirect3D();
	killWindow();
}

/*!
 *	Register and create a window, hide mouse cursor.
 */
void Application::initWindow(void)
{
	WNDCLASS WindowClass;

	WindowClass.style = CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowProcedure;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = L"ClassName";

	RegisterClass(&WindowClass);

	_handle = CreateWindow(L"ClassName", L"D3D Tut 18: Simple Vertex and Pixel Shaders", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, _width, _height, NULL, NULL, GetModuleHandle(NULL), NULL); // ajouté WS_VISIBLE pour que ça fonctionne en windowed et changé taille

//	ShowCursor(false);
}

/*!
 *	Create D3D object and device. Invoke further D3D initialization.
 */
void Application::initDirect3D(void)
{
	Sleep(50);

	s_hModD3D9 = LoadLibrary( L"d3d9.dll" );
    if( s_hModD3D9 != NULL )
    {
        s_DynamicDirect3DCreate9 = (LPDIRECT3DCREATE9)GetProcAddress( s_hModD3D9, "Direct3DCreate9" );
        s_DynamicD3DPERF_BeginEvent = (LPD3DPERF_BEGINEVENT)GetProcAddress( s_hModD3D9, "D3DPERF_BeginEvent" );
        s_DynamicD3DPERF_EndEvent = (LPD3DPERF_ENDEVENT)GetProcAddress( s_hModD3D9, "D3DPERF_EndEvent" );
        s_DynamicD3DPERF_SetMarker = (LPD3DPERF_SETMARKER)GetProcAddress( s_hModD3D9, "D3DPERF_SetMarker" );
        s_DynamicD3DPERF_SetRegion = (LPD3DPERF_SETREGION)GetProcAddress( s_hModD3D9, "D3DPERF_SetRegion" );
        s_DynamicD3DPERF_QueryRepeatFrame = (LPD3DPERF_QUERYREPEATFRAME)GetProcAddress( s_hModD3D9, "D3DPERF_QueryRepeatFrame" );
        s_DynamicD3DPERF_SetOptions = (LPD3DPERF_SETOPTIONS)GetProcAddress( s_hModD3D9, "D3DPERF_SetOptions" );
        s_DynamicD3DPERF_GetStatus = (LPD3DPERF_GETSTATUS)GetProcAddress( s_hModD3D9, "D3DPERF_GetStatus" );
    }


	if( s_DynamicDirect3DCreate9 != NULL )
	{
		if( (_object = s_DynamicDirect3DCreate9( D3D_SDK_VERSION )) == NULL)
		{
			MessageBox(_handle, L"Direct3DCreate9() failed!", L"InitD3D()", MB_OK);
			_running = false;
		}
	}

//	if((_object = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
//	{
//		MessageBox(_handle, "Direct3DCreate9() failed!", "InitD3D()", MB_OK);
//		_running = false;
//	}

	ZeroMemory(&_presentParameters, sizeof(_presentParameters));
	
	_presentParameters.BackBufferWidth = _width;
	_presentParameters.BackBufferHeight = _height;
	_presentParameters.BackBufferFormat = _colorFormat;
	_presentParameters.BackBufferCount = 1;

	_presentParameters.MultiSampleType = _multisampling;
	_presentParameters.MultiSampleQuality = 0;

	_presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	_presentParameters.hDeviceWindow = _handle;
	_presentParameters.Windowed = true; //false; // true;
	_presentParameters.EnableAutoDepthStencil = true;//true;
	_presentParameters.AutoDepthStencilFormat = _depthStencilFormat;
	_presentParameters.Flags = 0;
	_presentParameters.FullScreen_RefreshRateInHz = 0;// 60;//60; // 0;
	_presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

//	D3DDISPLAYMODEEX pFullscreenDisplayMode;
//	ZeroMemory(&pFullscreenDisplayMode, sizeof(pFullscreenDisplayMode));
//	pFullscreenDisplayMode.Size = sizeof(D3DDISPLAYMODEEX);
//	pFullscreenDisplayMode.Width = _presentParameters.BackBufferWidth;
//	pFullscreenDisplayMode.Height = _presentParameters.BackBufferHeight;
//	pFullscreenDisplayMode.RefreshRate = 120;
//	pFullscreenDisplayMode.Format = _presentParameters.BackBufferFormat;
//	pFullscreenDisplayMode.ScanLineOrdering = D3DSCANLINEORDERING_PROGRESSIVE;

//	if(FAILED(_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, _handle, _vertexProcessing, &_presentParameters, &_device)))
//	{

	if(FAILED(_object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _handle, _vertexProcessing, &_presentParameters, &_device)))
	{
		MessageBox(_handle, L"CreateDevice() failed!", L"initDirect3D()", MB_OK);
		_running = false;
	}
//	}

	initScene();
	checkDeviceCaps();
}

/*!
 *	Set perspective and projection matrix. Initialize texture stages.
 */
void Application::initScene(void)
{
	D3DXMatrixPerspectiveFovLH(&_projection, _fieldOfView, _aspectRatio, _nearPlane, _farPlane);
	//D3DXMatrixOrthoLH(&_projection, 0.2f*4.0f, 0.2f*3.0f, _nearPlane, _farPlane); // pour tester projection orthographique
	_device->SetTransform(D3DTS_PROJECTION, &_projection);

	for(unsigned i = 0;i < 8;++i)
	{
		_device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC); // D3DTEXF_LINEAR to D3DTEXF_ANISOTROPIC
		_device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		_device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); // D3DTEXF_ANISOTROPIC to D3DTEXF_LINEAR
		_device->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, _anisotropy);
	}

	if (g_pEffect)
	{
		g_pEffect->OnResetDevice();

		WCHAR str[MAX_PATH];
		DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Scene.fx" );
		D3DXCreateEffectFromFile( app.getDevice(), str, NULL, NULL, 0, 
                                        NULL, &g_pEffect, NULL );
	
		// Obtain the technique handles
		g_hTRenderScene = g_pEffect->GetTechniqueByName( "RenderScene" );
	}
}

/*!
 *	So far empty.
 */
void Application::checkDeviceCaps(void)
{
}

/*!
 *	Check for lost device and resets it if need be.
 */
bool Application::checkDevice(void)
{
	switch(_device->TestCooperativeLevel())
	{
		case D3DERR_DEVICELOST: 
			if (g_pEffect)
			{
				g_pEffect->OnLostDevice();
			}
			return false;

		case D3DERR_DEVICENOTRESET:
		{
			if(FAILED(_device->Reset(&_presentParameters)))
			{
				//MessageBox(_handle, L"Reset() failed!", L"checkDevice()", MB_OK);
				return false;
			}
			initScene();
			return true;
		}
		default: return true;
	}
}

/*!
 *	Unregister window class.
 */
void Application::killWindow(void)
{
	UnregisterClass(L"ClassName", GetModuleHandle(NULL));
}

/*!
 *	Release D3D device and object.
 */
void Application::killDirect3D(void)
{
	if(_device != NULL)
	{
		_device->Release();
		_device = NULL;
	}

	if(_object != NULL)
	{
		_object->Release();
		_object = NULL;
	}
}