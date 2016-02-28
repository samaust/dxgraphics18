//-----------------------------------------------------------------------------
// File: Scene.fx
//
// Desc: Effect file for image post-processing sample.  This effect contains
//       a technique that renders a scene with vertex and pixel shaders.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------




matrix  g_mRevView;
matrix  g_mWorldView;// = matrix(0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.00049185992, -0.84498215, 2.4975133, 1.0000000);
matrix  g_mProj;// = matrix(1.8106600, 0.0, 0.0, 0.0, 0.0, 2.4142134, 0.0, 0.0, 0.0, 0.0, 1.0010010, 1.0, 0.0, 0.0, -1.0010010, 0.0);
float   g_fTest;
texture g_txScene;
texture g_txEnvMap;
float3  g_vLightDirView = float3( 0.0f, 0.0f, -1.0f );

		
//matrix  g_myinvProj = matrix(1.33333333f, 0.0f, 0.0f, 0.0f,
//                          0.0f, 1.0f, 0.0f, 0.0f,
//                          0.0f, 0.0f, 0.0f, 1.0f,
//                          0.0f, 0.0f, -9.999875f, 10.0f);
//matrix  g_myProj = matrix(0.75f, 0.0f, 0.0f, 0.0f,
//                          0.0f, 1.0f, 0.0f, 0.0f,
//                          0.0f, 0.0f, 1.00005000f, -0.10000500f,
//                          0.0f, 0.0f, 1.0f, 0.0f);

matrix  g_myinvProj = matrix(1.33333333f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 0.0f, -9.999875f,
                          0.0f, 0.0f, 1.0f, 10.0f);
matrix  g_myProj = matrix(0.75f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.00005000f, 1.0f,
                          0.0f, 0.0f, -0.10000500f, 0.0f);


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


samplerCUBE g_samEnvMap =
sampler_state
{
    Texture = <g_txEnvMap>;
    AddressU = Wrap;
    AddressV = Wrap;
    MinFilter = Point;
    MagFilter = Linear;
    MipFilter = Linear;
};


samplerCUBE g_samSkyBox =
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
                    float3 Normal : NORMAL,
                    float2 Tex : TEXCOORD0,
                    out float4 oPos : POSITION,
                    out float4 Diffuse : COLOR,
                    out float2 oTex : TEXCOORD0,
                    out float3 oNormal : TEXCOORD1,
                    out float3 oViewPos : TEXCOORD2 )
{
	float4 temp1;
//	matrix  g_mWorldView = matrix(0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.00049185992, -0.84498215, 2.4975133, 1.0000000);
//	matrix  g_mProj = matrix(1.8106600, 0.0, 0.0, 0.0, 0.0, 2.4142134, 0.0, 0.0, 0.0, 0.0, 1.0010010, 1.0, 0.0, 0.0, -1.0010010, 0.0);
	
    // Output screen space position
    oViewPos = mul( Pos, g_mWorldView );
    
    //oPos = mul( float4( oViewPos, 1.0f ), g_mProj );
    temp1  = mul( float4( oViewPos, 1.0f ), g_mProj );
    
    // Added to test
    //temp1 =  mul( temp1, g_myinvProj );
    //temp1.x = temp1.x + g_fTest;
    //temp1 = mul( temp1, g_myProj );
    
    oPos = temp1;   

    // Transform normal to view space
    oNormal = mul( Normal, (float3x3)g_mWorldView );

    // Compute diffuse
    Diffuse = dot( g_vLightDirView, oNormal );

    // Propagate tex coord
    oTex = Tex;

	
}


void RenderScenePS( float4 Diffuse : COLOR,
                    float2 Tex : TEXCOORD0,
                    float3 Normal : TEXCOORD1,
                    float3 Pos : TEXCOORD2,
                    out float4 oCol : COLOR0,
                    out float4 oNormal : COLOR1,
                    out float4 oPos : COLOR2 )
{
    //
    // Output pixel color
    //
    oCol = tex2D( g_samScene, Tex ) * Diffuse;
    
    //
    // Output normal
    //
    oNormal = float4( normalize( Normal ), 1.0f );

    //
    // Output view position
    //
    oPos = float4( Pos, 1.0f );
}


void RenderScenePS_ColorNormal( float4 Diffuse : COLOR,
                                float2 Tex : TEXCOORD0,
                                float3 Normal : TEXCOORD1,
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
    oNormal = float4( normalize( Normal ), 1.0f );
}


void RenderScenePS_Color( float4 Diffuse : COLOR,
                          float2 Tex : TEXCOORD0,
                          float3 Normal : TEXCOORD1,
                          float3 Pos : TEXCOORD2,
                          out float4 oCol : COLOR0 )
{
    //
    // Output pixel color
    //
    oCol = tex2D( g_samScene, Tex ) * Diffuse;
}


void RenderScenePS_Normal( float4 Diffuse : COLOR,
                           float2 Tex : TEXCOORD0,
                           float3 Normal : TEXCOORD1,
                           float3 Pos : TEXCOORD2,
                           out float4 oNormal : COLOR0 )
{
    //
    // Output normal
    //
    oNormal = float4( normalize( Normal ), 1.0f );
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


void RenderEnvMapSceneVS( float4 Pos : POSITION,
                          float3 Normal : NORMAL,
                          float2 Tex : TEXCOORD0,
                          out float4 oPos : POSITION,
                          out float4 Diffuse : COLOR,
                          out float3 oTex : TEXCOORD0,
                          out float3 oNormal : TEXCOORD1,
                          out float3 oViewPos : TEXCOORD2 )
{
//	matrix  g_mWorldView = matrix(0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.00049185992, -0.84498215, 2.4975133, 1.0000000);
//	matrix  g_mProj = matrix(1.8106600, 0.0, 0.0, 0.0, 0.0, 2.4142134, 0.0, 0.0, 0.0, 0.0, 1.0010010, 1.0, 0.0, 0.0, -1.0010010, 0.0);
	
    // Output screen space position
    oViewPos = mul( Pos, g_mWorldView );
    oPos = mul( float4( oViewPos, 1.0f ), g_mProj );

    // Transform normal to view space
    oNormal = normalize( mul( Normal, (float3x3)g_mWorldView ) );

    // Compute diffuse
    Diffuse = dot( g_vLightDirView, oNormal );

    // Compute reflective vector for tex coord
    oTex = 2 * dot( -oViewPos, oNormal ) * oNormal + oViewPos;
    oTex = mul( oTex, (float3x3)g_mRevView );
}


void RenderEnvMapScenePS( float4 Diffuse : COLOR,
                          float3 Tex : TEXCOORD0,
                          float3 Normal : TEXCOORD1,
                          float3 Pos : TEXCOORD2,
                          out float4 oCol : COLOR0,
                          out float4 oNormal : COLOR1,
                          out float4 oPos : COLOR2 )
{
    //
    // Output pixel color
    //
    oCol = texCUBE( g_samEnvMap, Tex );// * Diffuse;

    //
    // Output normal
    //
    oNormal = float4( normalize( Normal ), 1.0f );

    //
    // Output view position
    //
    oPos = float4( Pos, 1.0f );
}


void RenderEnvMapScenePS_ColorNormal( float4 Diffuse : COLOR,
                                      float3 Tex : TEXCOORD0,
                                      float3 Normal : TEXCOORD1,
                                      float3 Pos : TEXCOORD2,
                                      out float4 oCol : COLOR0,
                                      out float4 oNormal : COLOR1 )
{
    //
    // Output pixel color
    //
    oCol = texCUBE( g_samEnvMap, Tex );// * Diffuse;

    //
    // Output normal
    //
    oNormal = float4( normalize( Normal ), 1.0f );
}


void RenderEnvMapScenePS_Color( float4 Diffuse : COLOR,
                                float3 Tex : TEXCOORD0,
                                float3 Normal : TEXCOORD1,
                                float3 Pos : TEXCOORD2,
                                out float4 oCol : COLOR0 )
{
    //
    // Output pixel color
    //
    oCol = texCUBE( g_samEnvMap, Tex );// * Diffuse;
}


void RenderEnvMapScenePS_Normal( float4 Diffuse : COLOR,
                                 float3 Tex : TEXCOORD0,
                                 float3 Normal : TEXCOORD1,
                                 float3 Pos : TEXCOORD2,
                                 out float4 oNormal : COLOR0 )
{
    //
    // Output normal
    //
    oNormal = float4( normalize( Normal ), 1.0f );
}


void RenderEnvMapScenePS_Position( float4 Diffuse : COLOR,
                                   float3 Tex : TEXCOORD0,
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
// Outputs texture value withtout lighting calculation.  Used for fullscreen
// quad copy.
float4 PixNoLight( float2 Tex : TEXCOORD0 ) : COLOR
{
    return tex2D( g_samScene, Tex );
}


//-----------------------------------------------------------------------------
// Renders skybox.  Used only for geometry with a 3D texcoord.
void RenderSkyBoxVS( float4 Pos : POSITION,
                     float3 Tex : TEXCOORD0,
                     out float4 oPos : POSITION,
                     out float3 oTex : TEXCOORD0,
                     out float3 oViewPos : TEXCOORD1 )
{
//	matrix  g_mWorldView = matrix(0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.0, 0.0, 0.0, 0.91782516, 0.0, 0.00049185992, -0.84498215, 2.4975133, 1.0000000);
//	matrix  g_mProj = matrix(1.8106600, 0.0, 0.0, 0.0, 0.0, 2.4142134, 0.0, 0.0, 0.0, 0.0, 1.0010010, 1.0, 0.0, 0.0, -1.0010010, 0.0);
	
    oViewPos = mul( Pos, g_mWorldView );
    
    // line added
    //oViewPos.x = mul(oViewPos.x, g_fTest); 
    
    oPos = mul( float4( oViewPos, 1.0f ), g_mProj );

    oTex = Tex;
}


void RenderSkyBoxPS( float3 Tex : TEXCOORD0,
                     float3 ViewPos : TEXCOORD1,
                     out float4 oColor : COLOR0,
                     out float4 oNormal : COLOR1,
                     out float4 oPos : COLOR2 )
{
    //
    // Output pixel color
    //
    oColor = texCUBE( g_samSkyBox, Tex );

    //
    // Output normal has (0, 0, -1) always
    //
    oNormal = float4( 0.0f, 0.0f, -1.0f, 1.0f );

    //
    // Output view position
    //
    oPos = float4( ViewPos, 1.0f );
}


void RenderSkyBoxPS_ColorNormal( float3 Tex : TEXCOORD0,
                                 float3 ViewPos : TEXCOORD1,
                                 out float4 oColor : COLOR0,
                                 out float4 oNormal : COLOR1 )
{
    //
    // Output pixel color
    //
    oColor = texCUBE( g_samSkyBox, Tex );

    //
    // Output normal has (0, 0, -1) always
    //
    oNormal = float4( 0.0f, 0.0f, -1.0f, 1.0f );
}


void RenderSkyBoxPS_Color( float3 Tex : TEXCOORD0,
                           float3 ViewPos : TEXCOORD1,
                           out float4 oColor : COLOR0 )
{
    //
    // Output pixel color
    //
    oColor = texCUBE( g_samSkyBox, Tex );
}


void RenderSkyBoxPS_Normal( float3 Tex : TEXCOORD0,
                            float3 ViewPos : TEXCOORD1,
                            out float4 oNormal : COLOR0 )
{
    //
    // Output normal has (0, 0, -1) always
    //
    oNormal = float4( 0.0f, 0.0f, -1.0f, 1.0f );
}


void RenderSkyBoxPS_Position( float3 Tex : TEXCOORD0,
                              float3 ViewPos : TEXCOORD1,
                              out float4 oPos : COLOR0 )
{
    //
    // Output view position
    //
    oPos = float4( ViewPos, 1.0f );
}


//-----------------------------------------------------------------------------
// Technique: RenderScene
// Desc: Renders the scene with a vertex and pixel shader.
//-----------------------------------------------------------------------------
technique RenderScene
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderSceneVS();
        
/*        VertexShaderConstant[0] = <g_mProj>; 		// c0-c3 is ModelView matrix
        VertexShaderConstant[4] = <g_myinvProj>;			// c4-c7 is ModelViewProjection matrix
		VertexShaderConstant[8] = <g_myProj>;
		//VertexShaderConstant[11] = {1.0,1.0,1.0,1.0};
		VertexShaderConstant[12] = <g_mWorldView>;
		VertexShaderConstant[15] = <g_fTest>;
		VertexShaderConstant[16] = <g_vLightDirView>;
//		VertexShaderConstant[12] = <g_mWorldView>;
//		VertexShaderConstant[13] = {1.0,0.0,0.0,1.0};
        VertexShader = asm
		{
		    vs_3_0
			def c17, 1, 0, 0, 0
			dcl_position v0
			dcl_normal v1
			dcl_texcoord v2
			dcl_position o0
			dcl_color o1
			dcl_texcoord o2.xy
			dcl_texcoord1 o3.xyz
			dcl_texcoord2 o4.xyz
			dp4 r0.x, v0, c12
			dp4 r0.y, v0, c13
			dp4 r0.z, v0, c14
			mov o4.x, r0.x
			mov o4.y, r0.y
			mov o4.z, r0.z
			mov r0.w, c17.x
			dp4 r1.x, r0, c0
			dp4 r1.y, r0, c1
			dp4 r1.z, r0, c2
			dp4 r1.w, r0, c3

			dp4 r0.x, r1, c4
			dp4 r2.y, r1, c5
			dp4 r2.z, r1, c6
			dp4 r2.w, r1, c7

			add r2.x, r0.x, c15.x

			dp4 r0.x, r2, c8
			dp4 r0.y, r2, c9
			dp4 r0.z, r2, c10
			dp4 r0.w, r2, c11

			mov o0, r0

			dp3 r0.x, v1, c12
			dp3 r0.y, v1, c13
			dp3 r0.z, v1, c14
			dp3 o1, c16, r0
			mov o2.xy, v2
			mov o3.xyz, r0
		};*/
	
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


technique RenderSceneThreePasses
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader = compile ps_3_0 RenderScenePS_Color();
        ZEnable = true;
    }
    pass p1
    {
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader = compile ps_3_0 RenderScenePS_Normal();
    }
    pass p2
    {
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader = compile ps_3_0 RenderScenePS_Position();
    }
}


//-----------------------------------------------------------------------------
// Technique: RenderEnvMapScene
// Desc: Renders the scene with with environment mapping.
//-----------------------------------------------------------------------------
technique RenderEnvMapScene
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderEnvMapSceneVS();
        PixelShader = compile ps_3_0 RenderEnvMapScenePS();
        ZEnable = true;
    }
}


technique RenderEnvMapSceneTwoPasses
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderEnvMapSceneVS();
        PixelShader = compile ps_3_0 RenderEnvMapScenePS_ColorNormal();
        ZEnable = true;
    }
    pass p1
    {
        VertexShader = compile vs_3_0 RenderEnvMapSceneVS();
        PixelShader = compile ps_3_0 RenderEnvMapScenePS_Position();
    }
}


technique RenderEnvMapSceneThreePasses
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderEnvMapSceneVS();
        PixelShader = compile ps_3_0 RenderEnvMapScenePS_Color();
        ZEnable = true;
    }
    pass p1
    {
        VertexShader = compile vs_3_0 RenderEnvMapSceneVS();
        PixelShader = compile ps_3_0 RenderEnvMapScenePS_Normal();
    }
    pass p2
    {
        VertexShader = compile vs_3_0 RenderEnvMapSceneVS();
        PixelShader = compile ps_3_0 RenderEnvMapScenePS_Position();
    }
}


technique RenderNoLight
{
    pass p0
    {
        VertexShader = null;
        PixelShader = compile ps_3_0 PixNoLight();
        ZEnable = false;
    }
}


//-----------------------------------------------------------------------------
// Technique: RenderSkyBox
// Desc: Renders using 3D texture coordinates to sample from a cube texture.
//-----------------------------------------------------------------------------
technique RenderSkyBox
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderSkyBoxVS();
        PixelShader = compile ps_3_0 RenderSkyBoxPS();
        ZEnable = true;
    }
}


technique RenderSkyBoxTwoPasses
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderSkyBoxVS();
        PixelShader = compile ps_3_0 RenderSkyBoxPS_ColorNormal();
        ZEnable = true;
    }
    pass p1
    {
        VertexShader = compile vs_3_0 RenderSkyBoxVS();
        PixelShader = compile ps_3_0 RenderSkyBoxPS_Position();
    }
}


technique RenderSkyBoxThreePasses
{
    pass p0
    {
        VertexShader = compile vs_3_0 RenderSkyBoxVS();
        PixelShader = compile ps_3_0 RenderSkyBoxPS_Color();
        ZEnable = true;
    }
    pass p1
    {
        VertexShader = compile vs_3_0 RenderSkyBoxVS();
        PixelShader = compile ps_3_0 RenderSkyBoxPS_Normal();
    }
    pass p2
    {
        VertexShader = compile vs_3_0 RenderSkyBoxVS();
        PixelShader = compile ps_3_0 RenderSkyBoxPS_Position();
    }
}
