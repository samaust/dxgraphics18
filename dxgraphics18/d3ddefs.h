// file name:	d3ddefs.h
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#ifndef d3ddefs_h
#define d3ddefs_h

//includes
#include <d3d9.h>
#include <d3dx9.h>

//structures
struct D3DVERTEX
{
    D3DXVECTOR3 position;   // 3D Position
    D3DXVECTOR2 texcoord;   // Texture Coordinates
};

#endif