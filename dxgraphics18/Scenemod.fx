//-----------------------------------------------------------------------------
// File: Scene.fx
//
// Desc: Effect file for image post-processing sample.  This effect contains
//       a technique that renders a scene with vertex and pixel shaders.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


matrix  matWorldViewProj;
float3  g_vLightDirView = float3( 0.0f, 0.0f, -1.0f );
texture g_txScene;

sampler2D g_samScene =
sampler_state
{
    Texture = <g_txScene>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Point;
    MagFilter = Linear;
    MipFilter = Linear;
};

void RenderSceneVS( float4 Pos : POSITION,
                    float2 Tex : TEXCOORD0,
                    out float4 oPos : POSITION,
                    out float4 Diffuse : COLOR,
                    out float2 oTex : TEXCOORD0 )
{
	//float4 temp1;
	
    // Output screen space position
    //oViewPos = mul( Pos, g_mWorldView );
    
    //oPos = mul( float4( oViewPos, 1.0f ), g_mProj );
    //temp1  = mul( float4( oViewPos, 1.0f ), g_mProj );
    
    // Added to test
    //temp1 =  mul( temp1, g_myinvProj );
    //temp1.x = temp1.x + g_fTest;
    //temp1 = mul( temp1, g_myProj );
    
    //oPos = temp1;
    
    // Transform normal to view space
    //oNormal = mul( Normal, (float3x3)g_mWorldView );


	oPos = mul( Pos, matWorldViewProj );

    // Compute diffuse
    //Diffuse = dot( g_vLightDirView, oNormal );
    Diffuse = float4(g_vLightDirView, 1.0f);
    
    // Propagate tex coord
    oTex = Tex;
	
}


void RenderScenePS( float4 Diffuse : COLOR,
                    float2 Tex : TEXCOORD0,
                    float3 Pos : TEXCOORD2,
                    out float4 oCol : COLOR0,
                    out float4 oPos : COLOR1 )
{
    //
    // Output pixel color
    //
    oCol = tex2D( g_samScene, Tex ) * Diffuse;
    
    //
    // Output normal
    //
    //oNormal = float4( normalize( Normal ), 1.0f );

    //
    // Output view position
    //
    oPos = float4( Pos, 1.0f );
}


void RenderScenePS_ColorNormal( float4 Diffuse : COLOR,
                                float2 Tex : TEXCOORD0,
                                float3 Pos : TEXCOORD2,
                                out float4 oCol : COLOR0,
                                out float4 oNormal : COLOR1 )
{
    //
    // Output pixel color
    //
    oCol = tex2D( g_samScene, Tex ) * Diffuse;
    
    //
    // Output normal
    //
    //oNormal = float4( normalize( Normal ), 1.0f );
}

void RenderScenePS_Position( float4 Diffuse : COLOR,
                             float2 Tex : TEXCOORD0,
                             float3 Normal : TEXCOORD1,
                             float3 Pos : TEXCOORD2,
                             out float4 oPos : COLOR0 )
{
    //
    // Output view position
    //
    oPos = float4( Pos, 1.0f );
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Technique: RenderScene
// Desc: Renders the scene with a vertex and pixel shader.
//-----------------------------------------------------------------------------
technique RenderScene
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderSceneVS();	
        PixelShader = compile ps_3_0 RenderScenePS();
        ZEnable = true;
    }
}


technique RenderSceneTwoPasses
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader = compile ps_3_0 RenderScenePS_ColorNormal();
        ZEnable = true;
    }
    pass p1
    {
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader = compile ps_3_0 RenderScenePS_Position();
    }
}

