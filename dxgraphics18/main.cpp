// file name:	main.cpp
// author:		Rick Wong
// e-mail:		rickinhethuis@gmail.com
// web site:	http://www.two-kings.de

#include "main.h"

#include "DXUT.h"
#include "DXUTcamera.h"
#include "DXUTsettingsdlg.h"
#include "SDKmisc.h"
#include "SDKmesh.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
//#include "resource.h"

ID3DXEffect*            g_pEffect = NULL;        // D3DX effect interface
D3DXHANDLE              g_hTRenderScene;         // Handle to RenderScene technique
CDXUTXFileMesh          g_SceneMesh;          // Mesh objects in the scene
int                     g_nPasses = 1;           // Number of passes required to render scene
D3DXMATRIXA16           g_mMeshWorld;            // World matrix (xlate and scale) for the mesh

Application	app;
//bool Left = true;

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "logging.h"

/*
va_list va_alist;
ofstream ofile;

void __cdecl open_log (void)
{
	static char dir[512] = {0};
	static char path[ MAX_PATH ]; 

	// get a windows module handle for the plugin 
	HINSTANCE module = GetModuleHandle( NULL ); 
	if (module != NULL) 
	{ 
		// get the full path to the plugin
		if (GetModuleFileName( module, path, MAX_PATH-1 ) != 0) 
		{ 
			// find the dot in the pathname  
			char* dot = strrchr( path, '.' ); 
			//if (dot != NULL) 
			//{ 
			//	strcpy( dot, ".ini" ); // change the name to anc.ini 
			//} 
		} 
	} 

	if(dir[0] == 0) { strcpy(dir, path); strcat(dir, "_log.txt"); }
	
	ofile.open(dir, ios::app);
}

void __cdecl close_log (void)
{
	if(ofile)
		ofile.close();
}

void __cdecl ADD_LOG (const char *fmt, ...)
{
	char logbuf[50000] = {0};

	if(!fmt) { return; }

	va_start (va_alist, fmt);
	_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	if(ofile)
	{
		ofile << logbuf << endl;
	}
}
*/

//--------------------------------------------------------------------------------------
// Compute the translate and scale transform for the current mesh.
HRESULT ComputeMeshWorldMatrix( LPD3DXMESH pMesh )
{
    LPDIRECT3DVERTEXBUFFER9 pVB;
    if( FAILED( pMesh->GetVertexBuffer( &pVB ) ) )
        return E_FAIL;

    LPVOID pVBData;
    if( SUCCEEDED( pVB->Lock( 0, 0, &pVBData, D3DLOCK_READONLY ) ) )
    {
        D3DXVECTOR3 vCtr;
        float fRadius;
        D3DXComputeBoundingSphere( (D3DXVECTOR3*)pVBData, pMesh->GetNumVertices(),
                                   D3DXGetFVFVertexSize( pMesh->GetFVF() ),
                                   &vCtr, &fRadius );

        D3DXMatrixTranslation( &g_mMeshWorld, -vCtr.x, -vCtr.y, -vCtr.z );
        D3DXMATRIXA16 m;
        D3DXMatrixScaling( &m, 1/fRadius, 1/fRadius, 1/fRadius );
        D3DXMatrixMultiply( &g_mMeshWorld, &g_mMeshWorld, &m );

        pVB->Unlock();
    }

    SAFE_RELEASE( pVB );

    return S_OK;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR cmdline, int cmdShow)
{
	

	MSG message;
	HRESULT result;

    // variables for this tutorial
    LPDIRECT3DVERTEXSHADER9         vertexShader   = NULL; // VS                   (NEW)
    LPDIRECT3DPIXELSHADER9          pixelShader    = NULL; // PS                   (NEW)
	LPD3DXCONSTANTTABLE             constantTable  = NULL; // ConstantTable        (NEW)
//    LPD3DXBUFFER                    code           = NULL; // Temporary buffer     (NEW)

	Sleep(100);

	open_log();

	ADD_LOG("-20");

    // set up vertex buffer
/*    D3DVERTEX quad[4] = {{D3DXVECTOR3(-3.0f, -3.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)},
						{ D3DXVECTOR3(-3.0f,  3.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f)},
						{ D3DXVECTOR3( 3.0f, -3.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f)},
						{ D3DXVECTOR3( 3.0f,  3.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)}};
    D3DVERTEX *ptr = NULL;
    app.getDevice()->CreateVertexBuffer(	sizeof(quad),
					                        D3DUSAGE_WRITEONLY,
					                        0,
					                        D3DPOOL_MANAGED,
					                        &quadVB,
					                        NULL);
    quadVB->Lock(0, 0, (void**)&ptr, 0);
    memcpy((void*)ptr, (void*)quad, sizeof(quad));
    quadVB->Unlock();
*/
    // set up texture
//    D3DXCreateTextureFromFile(app.getDevice(), L"texture.png", &quadTexture);

	// Create the environment mapping texture
/*    WCHAR str[MAX_PATH];
	HRESULT hr;
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"pipetex.dds" ) );
    if( FAILED( D3DXCreateCubeTextureFromFile( app.getDevice(), str, &quadTexture ) ) )
        return DXUTERR_MEDIANOTFOUND;
	*/


    // set up Vertex Shader (NEW)
 /*   D3DVERTEXELEMENT9 decl[] = {{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
								{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
								D3DDECL_END()};
    app.getDevice()->CreateVertexDeclaration(decl, &vertexDecl);
*/
 /*   result = D3DXCompileShaderFromFile( L"vertex.vsh",     //filepath
                                         NULL,            //macro's
                                         NULL,            //includes
                                         "vs_main",       //main function
                                         "vs_3_0",        //shader profile
                                         0,               //flags
                                         &code,           //compiled operations
                                         NULL,            //errors
                                         &constantTable); //constants
    if(FAILED(result))
        MessageBox(app.getHandle(), L"Invalid vs code", L"Error", MB_OK);

//	const char* g_strVertexShader= 
/*	"vs_3_0\n"
    "def c17, 1, 0, 0, 0\n"
    "dcl_position v0\n"
//    "dcl_normal v1\n"
    "dcl_texcoord v2\n"
    "dcl_position o0\n"
//    "dcl_color o1\n"
    "dcl_texcoord o2.xy\n"
    "dcl_texcoord1 o3.xyz\n"
    "dcl_texcoord2 o4.xyz\n"
    "dp4 r0.x, v0, c12\n"
    "dp4 r0.y, v0, c13\n"
    "dp4 r0.z, v0, c14\n"
    "mov o4.x, r0.x\n"
    "mov o4.y, r0.y\n"
    "mov o4.z, r0.z\n"
    "mov r0.w, c17.x\n"
    "dp4 r1.x, r0, c0\n"
    "dp4 r1.y, r0, c1\n"
    "dp4 r1.z, r0, c2\n"
    "dp4 r1.w, r0, c3\n"
    "dp4 r0.x, r1, c4\n"
    "dp4 r2.y, r1, c5\n"
    "dp4 r2.z, r1, c6\n"
    "dp4 r2.w, r1, c7\n"
    "add r2.x, r0.x, c15.x\n"
    "dp4 r0.x, r2, c8\n"
	"dp4 r0.y, r2, c9\n"
    "dp4 r0.z, r2, c10\n"
    "dp4 r0.w, r2, c11\n"
    "mov o0, r0\n"
    "dp3 r0.x, v1, c12\n"
    "dp3 r0.y, v1, c13\n"
    "dp3 r0.z, v1, c14\n"
    "dp3 o1, c16, r0\n"
    "mov o2.xy, v2\n"
    "mov o3.xyz, r0\n"
	"mov o0, v0\n" // line added
	"mov o2.y, v2\n" // line added
	"";*/

	/*"vs_2_0\n"
    "dcl_position v0\n"
    "dcl_texcoord v1\n"
    "dp4 oPos.x, v0, c0\n"
    "dp4 oPos.y, v0, c1\n"
    "dp4 oPos.z, v0, c2\n"
    "dp4 oPos.w, v0, c3\n"
    "mov oT0.xy, v1\n"
	"";*/

/*	"vs_3_0\n"
	"dcl_position v0\n"
	"dcl_texcoord v1\n"
	"dcl_position o0\n"
	"dcl_texcoord o1.xy\n"
	"dp4 o0.x, v0, c0\n"
	"dp4 o0.y, v0, c1\n"
	"dp4 o0.z, v0, c2\n"
	"dp4 o0.w, v0, c3\n"
	"mov o1.xy, v1\n"
	"";*/

/*	"vs_3_0\n"
    "dcl_position v0\n"
    "dcl_texcoord v1\n"
    "dcl_position o0\n"
    "dcl_texcoord o1.xy\n"
    "dp4 r0.x, v0, c0\n"
    "dp4 r0.y, v0, c1\n"
    "dp4 r0.z, v0, c2\n"
	"dp4 r0.w, v0, c3\n"
	"mov o0, r0\n"
    "mov o1.xy, v1\n"
	"";*/

/*	"vs_3_0\n"
    "def c200, 1.33333333, 0, 0, 0\n"
    "def c201, 0, 1, 0, 0\n"
    "def c202, 0, 0, 0, 1\n"
    "def c203, 0, 0, -9.999875, 10\n"
    "def c204, 1, 0, 0, 0\n"
    "def c205, 0, 0, 1, 0\n"
    "def c206, 0, 0, 0, 1\n"
//    "def c207, 1, 0, 0, 5\n"
    "def c208, 0, 0, 1, 0\n"
    "def c209, 0, 0, 0, 1\n"
    "def c210, 0.75, 0, 0, 0\n"
    "def c211, 0, 0, 1.00005000, -0.10000500\n"
    "def c212, 0, 0, 1, 0\n"
    "dcl_position v0\n"
    "dcl_texcoord v1\n"
    "dcl_position o0\n"
    "dcl_texcoord o1.xy\n"
    "dp4 r0.x, v0, c0\n"
    "dp4 r0.y, v0, c1\n"
    "dp4 r0.z, v0, c2\n"
    "dp4 r0.w, v0, c3\n"
    "dp4 r30.x, r0, c200\n"
    "dp4 r31.y, r0, c201\n"
    "dp4 r31.z, r0, c202\n"
    "dp4 r31.w, r0, c203\n"
    "add r31.x, r30.x, c207.w\n"
    "dp4 r0.x, r31, c210\n"
    "dp4 r0.y, r31, c201\n"
    "dp4 r0.z, r31, c211\n"
    "dp4 r0.w, r31, c212\n"
    "mov o0, r0\n"
    "mov o1.xy, v1\n"
	"";*/


/*
	result = D3DXAssembleShader( g_strVertexShader, 
		                     strlen( g_strVertexShader ), 
							 NULL, 
							 NULL, 
							 0, 
							 &code, 
							 NULL);





	if(FAILED(result))
        MessageBox(app.getHandle(), "Invalid vs code", "Error", MB_OK);

*/
	// En utilisant effect framework pour tester avec vu que je n'arrive pas à faire marcher avec les samples du dx sdk
	 
	// Read the D3DX effect file
    WCHAR str[MAX_PATH];
	HRESULT hr;
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Scene.fx" ) );

	ADD_LOG("-19");

	DWORD dwShaderFlags = 0;
	 // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V_RETURN( D3DXCreateEffectFromFile( app.getDevice(), str, NULL, NULL, dwShaderFlags, 
                                        NULL, &g_pEffect, NULL ) );
	
	ADD_LOG("-18");
	 // Obtain the technique handles
    switch( g_nPasses )
    {
        case 1:
            g_hTRenderScene = g_pEffect->GetTechniqueByName( "RenderScene" );
            break;
        case 2:
            g_hTRenderScene = g_pEffect->GetTechniqueByName( "RenderSceneTwoPasses" );
            break;
        case 3:
            g_hTRenderScene = g_pEffect->GetTechniqueByName( "RenderSceneThreePasses" );
            break;
    }

	
//	open_log();

//	LPD3DXBUFFER pBufIn; // Buffer qui contient le code assembleur du shader en ASCII (size manuelle)
//	D3DXCreateBuffer(50000, &pBufIn); // oublié de deleter le buffer
//	D3DXDisassembleShader((DWORD*)code->GetBufferPointer(), false, NULL, &pBufIn); // size manuelle
//	ADD_LOG("Trying to add input log now before D3DXAssembleShader");
//	ADD_LOG((char*)pBufIn->GetBufferPointer());
//	ADD_LOG("pass input!");
//	ADD_LOG("\n");


/*
	app.getDevice()->CreateVertexShader((DWORD*)code->GetBufferPointer(), &vertexShader);

	//char pShaderData[50000];
	//UINT size = vertexShader->GetFunction((void*)&pShaderData[0], NULL);
	//vertexShader->GetFunction((void*)pShaderData, &size);

//	D3DXSEMANTIC semantics[MAXD3DDECLLENGTH];
//	UINT semanticsCount;
//	UINT shaderSize;
//	vertexShader->GetFunction( NULL, &shaderSize );
//	BYTE* shaderFunc = new BYTE[shaderSize];
//	vertexShader->GetFunction( shaderFunc, &shaderSize );
//	D3DXGetShaderInputSemantics( (DWORD*)shaderFunc, semantics, &semanticsCount );
	


	//LPD3DXBUFFER pBufIn2; // Buffer qui contient le code assembleur du shader en ASCII (size manuelle)
	//D3DXCreateBuffer(50000, &pBufIn2); // oublié de deleter le buffer
	//D3DXDisassembleShader((DWORD*)code->GetBufferPointer(), false, NULL, &pBufIn2); // size manuelle
	//ADD_LOG("Trying to add input log now ---=== after ===--- D3DXAssembleShader");
	//ADD_LOG((char*)pBufIn2->GetBufferPointer());
	//ADD_LOG((char *)shaderFunc);
	//int byteSize = D3DXGetShaderSize(
	//ADD_LOG("shaderSize is : %d bytes", shaderSize);


	//ADD_LOG("pass input!");
	//ADD_LOG("\n");

	//close_log();

	//ofstream os( "test.txt" );
    //os.write( (char *) shaderFunc, shaderSize );

//	delete [] shaderFunc;
//   code->Release();

    // set up Pixel Shader (NEW)
    result = D3DXCompileShaderFromFile( L"pixel.psh",   //filepath
                                        NULL,          //macro's            
                                        NULL,          //includes           
                                        "ps_main",     //main function      
                                        "ps_3_0",      //shader profile     
                                        0,             //flags              
                                        &code,         //compiled operations
                                        NULL,          //errors
                                        NULL);         //constants
    if(FAILED(result))
        MessageBox(app.getHandle(), L"Invalid ps code", L"Error", MB_OK);



	// Pixel shader asm fonctionne bien
/*	const char* g_strPixelShader = 
	"ps_2_0\n"
	"def c0, 0.899999976, 0.800000012, 0.400000006, 1\n"
	"dcl t0.xy\n"
	"dcl_2d s0\n"
	"texld r0, t0, s0\n"
	"mul r0, r0, c0\n"
	"mov oC0, r0\n"
	"";

/*	"ps_3_0\n"
    "def c0, 0.899999976, 0.800000012, 0.400000006, 1\n"
    "dcl_texcoord v0.xy\n"
    "dcl_2d s0\n"
    "texld r0, v0, s0\n"
    "mul oC0, r0, c0\n"
	"";*/

	//HRESULT hr;
/*	result = D3DXAssembleShader( g_strPixelShader, 
		                     strlen( g_strPixelShader ), 
							 NULL, 
							 NULL, 
							 0, 
							 &code, 
							 NULL);
	if(FAILED(result))
        MessageBox(app.getHandle(), "Invalid ps code", "Error", MB_OK);
*/


//	app.getDevice()->CreatePixelShader((DWORD*)code->GetBufferPointer(), &pixelShader);
 //	code->Release();

	ADD_LOG("-11");
	ADD_LOG("-11");
	// Load the meshes
/*	if( FAILED( g_SceneMesh.Create( app.getDevice(), L"J:\\dx prog\\dxgraphics18\\dwarf\\dwarf.x" ) ) )
	//if( FAILED( g_SceneMesh.Create( app.getDevice(), L"test.x" ) ) )
	//if( FAILED( g_SceneMesh.Create( app.getDevice(), L"test_result.x" ) ) )
	{
		if( FAILED(hr) ) MessageBox(NULL,DXGetErrorDescription(hr),"Load Error",MB_OK);
		ADD_LOG("-11 failed");
        return DXUTERR_MEDIANOTFOUND;
	}

	*/

	
	hr = g_SceneMesh.Create( app.getDevice(), L"J:\\dx prog\\dxgraphics18\\dwarf\\dwarf.x" );
	if( FAILED(hr) ) 
	{
	//	MessageBox(NULL,DXGetErrorDescription(hr),L"Load Error",MB_OK);
		return DXUTERR_MEDIANOTFOUND;
	}









	ADD_LOG("-10.9");

	 // Initialize the mesh's world matrix
    ComputeMeshWorldMatrix( g_SceneMesh.GetMesh() );

//	std::string DataFilename = "datalog.txt";
//	std::ofstream myData(DataFilename.c_str(), std::ios_base::trunc);
//	std::basic_ostream<char>* pOstream = static_cast<std::basic_ostream<char>*>(&myData);

	int last_sl = 0;
	BOOL last_blank = false;
	int count = 0;

	DWORD dwCurrent;
	DWORD dwLast;
	DWORD dwTickElapsed;

	timeBeginPeriod(1); // added

	dwCurrent = timeGetTime();
	dwLast = dwCurrent;
	Sleep(15);

	BOOL presentNow = FALSE;

	//SetPriorityClass(app.getHandle(), REALTIME_PRIORITY_CLASS);
	//SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);

	// IDLE_PRIORITY_CLASS
	// BELOW_NORMAL_PRIORITY_CLASS
	// NORMAL_PRIORITY_CLASS
	// ABOVE_NORMAL_PRIORITY_CLASS
	// HIGH_PRIORITY_CLASS
	// REALTIME_PRIORITY_CLASS

	bool green = true;

	bool Left = true;

	IDirect3DSurface9 *		pRenderSurfBackBuffer;
//	IDirect3DSurface9 *		pRenderSurfBackBufferDepth;

//	app.getDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurfBackBuffer); // Calling this method will increase the internal reference count on the IDirect3DSurface9 interface. Failure to call IUnknown::Release when finished using this IDirect3DSurface9 interface results in a memory leak.
//	D3DSURFACE_DESC desc;
//	hr = pRenderSurfBackBuffer->GetDesc(&desc);
//	if (SUCCEEDED(hr))
//	{   //should verify return values of the next two functions
//		app.getDevice()->CreateDepthStencilSurface(1024, 768, D3DFMT_D24S8, desc.MultiSampleType, desc.MultiSampleQuality, FALSE, &pRenderSurfBackBufferDepth, NULL);
//		//f_pD3DDevice->GetDepthStencilSurface(m_pRenderSurfBackBufferDepthFirst); // Not sure if needed
//	}

	while(app.getRunning())
	{
		// Use PeekMessage() so we can use idle time to render the scene. 
		if(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else // Render a frame during idle time (no messages are waiting)
		{
			ADD_LOG("-10");
			Sleep(0);
			ADD_LOG("-9");
			if(app.checkDevice())
			{
				ADD_LOG("-8");
	/*			if (green)
				{
					green = false;
					app.getDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200,0,0), 1.0f, 0);
				}
				else
				{
					green = true;
					app.getDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,200,0), 1.0f, 0);
				}*/
				
				IDirect3DDevice9 *myDevice;
				myDevice = app.getDevice();
				ADD_LOG("-7.9");
				app.getDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurfBackBuffer); 
				ADD_LOG("-7.8");


				//myDevice->SetRenderTarget(0, pRenderSurfBackBuffer); // enlever pour tester modif progr qui marche sans ceci
	//			app.getDevice()->SetDepthStencilSurface(pRenderSurfBackBufferDepth);
				app.getDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100,100,100), 1.0f, 0);

				ADD_LOG("-7");
				UINT cPass, p;
				LPD3DXMESH pMeshObj;

				ADD_LOG("-6");
				// Render the scene
				if( SUCCEEDED( app.getDevice()->BeginScene() ) )
				{
					ADD_LOG("-5");
					// Render the mesh
					D3DXMATRIXA16 matWorld, matView, matProj;
					app.getDevice()->GetTransform(D3DTS_WORLD, &matWorld);
					app.getDevice()->GetTransform(D3DTS_VIEW, &matView);
					app.getDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

					D3DXMATRIX matTrans;

					ADD_LOG("1");
	/*				if (Left)
					{
						Left = false;
						D3DXMatrixTranslation( &matTrans, -0.1f, 0.0f, 2.5f );
					}
					else
					{
						Left = true;
						D3DXMatrixTranslation( &matTrans, 0.1f, 0.0f, 2.5f );
					}
	*/
					D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 7.0f );

					ADD_LOG("2");

					D3DXMATRIX matRot;
					D3DXMatrixRotationYawPitchRoll( &matRot,
										D3DXToRadian(0),
										D3DXToRadian(0),
										0.0f );


					matWorld = matWorld * matRot * matTrans;

					ADD_LOG("3");
					// Render the mesh
					D3DXMATRIXA16 mWorldView;
					D3DXMatrixMultiply( &mWorldView, &mWorldView, &matView );
					D3DXMatrixMultiply( &mWorldView, &g_mMeshWorld, &matWorld );
					D3DXMatrixMultiply( &mWorldView, &mWorldView, &matView );
					HRESULT hr;
	        
					ADD_LOG("4");

					V( g_pEffect->SetMatrix( "g_mWorldView", &mWorldView ) );

					ADD_LOG("5");

					V( g_pEffect->SetMatrix( "g_mProj", &matProj ) );

					ADD_LOG("6");

					V( g_pEffect->SetTechnique( g_hTRenderScene ) );

					ADD_LOG("7");

					pMeshObj = g_SceneMesh.GetMesh();

					ADD_LOG("8");

					V( g_pEffect->Begin( &cPass, 0 ) );

					ADD_LOG("9");

					for( p = 0; p < cPass; ++p )
					{
						V( g_pEffect->BeginPass( p ) );

						ADD_LOG("10");

						// Iterate through each subset and render with its texture
						for( DWORD m = 0; m < g_SceneMesh.m_dwNumMaterials; ++m )
						{
							V( g_pEffect->SetTexture( "g_txScene", g_SceneMesh.m_pTextures[m] ) );
							V( g_pEffect->CommitChanges() );
							V( pMeshObj->DrawSubset( m ) );
						}

						ADD_LOG("11");

						V( g_pEffect->EndPass() );

						ADD_LOG("12");
					}

					ADD_LOG("13");

					V( g_pEffect->End() ); // plante???

					ADD_LOG("14");

					V( app.getDevice()->EndScene() );

					ADD_LOG("15");
				}

				ADD_LOG("16");

				app.getDevice()->Present(0, 0, 0, 0);

				ADD_LOG("17");
				

			}
		}
	}

//	g_SceneMesh.Destroy();	
	g_pEffect->Release();


//	pRenderSurfBackBuffer->Release();

	timeEndPeriod(1); // added

    // clean up shaders (NEW)
    if(constantTable)
    {
        constantTable->Release();
        constantTable = NULL;
    }

    if(pixelShader)
    {
        pixelShader->Release();
        pixelShader = NULL;
    }

    if(vertexShader)
    {
        vertexShader->Release();
        vertexShader = NULL;
    }

	return 0;
}
