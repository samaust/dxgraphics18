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

	Sleep(100);

	open_log();
	 
	// Read the D3DX effect file
    WCHAR str[MAX_PATH];
	HRESULT hr;
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Scene.fx" ) );

	DWORD dwShaderFlags = 0;
	 // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    V_RETURN( D3DXCreateEffectFromFile( app.getDevice(), str, NULL, NULL, dwShaderFlags, 
                                        NULL, &g_pEffect, NULL ) );
	
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

	// Load the meshes	
	V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, L"Dwarf.x"));
	hr = g_SceneMesh.Create( app.getDevice(), str);
	if( FAILED(hr) ) 
	{
	//	MessageBox(NULL,DXGetErrorDescription(hr),L"Load Error",MB_OK);
		return DXUTERR_MEDIANOTFOUND;
	}

	 // Initialize the mesh's world matrix
    ComputeMeshWorldMatrix( g_SceneMesh.GetMesh() );

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

	bool green = true;

	bool Left = true;

	IDirect3DSurface9 *		pRenderSurfBackBuffer;

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
			Sleep(0);
			if(app.checkDevice())
			{	
				IDirect3DDevice9 *myDevice;
				myDevice = app.getDevice();
				app.getDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pRenderSurfBackBuffer); 
				app.getDevice()->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(100,100,100), 1.0f, 0);

				UINT cPass, p;
				LPD3DXMESH pMeshObj;

				// Render the scene
				if( SUCCEEDED( app.getDevice()->BeginScene() ) )
				{
					// Render the mesh
					D3DXMATRIXA16 matWorld, matView, matProj;
					app.getDevice()->GetTransform(D3DTS_WORLD, &matWorld);
					app.getDevice()->GetTransform(D3DTS_VIEW, &matView);
					app.getDevice()->GetTransform(D3DTS_PROJECTION, &matProj);

					D3DXMATRIX matTrans;
					D3DXMatrixTranslation( &matTrans, 0.0f, 0.0f, 7.0f );

					D3DXMATRIX matRot;
					D3DXMatrixRotationYawPitchRoll( &matRot,
										D3DXToRadian(0),
										D3DXToRadian(0),
										0.0f );

					matWorld = matWorld * matRot * matTrans;

					// Render the mesh
					D3DXMATRIXA16 mWorldView;
					D3DXMatrixMultiply( &mWorldView, &mWorldView, &matView );
					D3DXMatrixMultiply( &mWorldView, &g_mMeshWorld, &matWorld );
					D3DXMatrixMultiply( &mWorldView, &mWorldView, &matView );
					HRESULT hr;
	        
					V( g_pEffect->SetMatrix( "g_mWorldView", &mWorldView ) );
					V( g_pEffect->SetMatrix( "g_mProj", &matProj ) );
					V( g_pEffect->SetTechnique( g_hTRenderScene ) );
					pMeshObj = g_SceneMesh.GetMesh();

					V( g_pEffect->Begin( &cPass, 0 ) );
					for( p = 0; p < cPass; ++p )
					{
						V( g_pEffect->BeginPass( p ) );

						// Iterate through each subset and render with its texture
						for( DWORD m = 0; m < g_SceneMesh.m_dwNumMaterials; ++m )
						{
							V( g_pEffect->SetTexture( "g_txScene", g_SceneMesh.m_pTextures[m] ) );
							V( g_pEffect->CommitChanges() );
							V( pMeshObj->DrawSubset( m ) );
						}

						V( g_pEffect->EndPass() );
					}

					V( g_pEffect->End() );
					V( app.getDevice()->EndScene() );
				}
				
				app.getDevice()->Present(0, 0, 0, 0);
			}
		}
	}

	g_SceneMesh.Destroy();	
	g_pEffect->Release();

	timeEndPeriod(1);

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
