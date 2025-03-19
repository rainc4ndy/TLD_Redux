//
// Created by Administrator on 2024/10/8.
//

#include "matrix.h"

#include <d3dx9math.h>

void CalcScreenCoors ( D3DXVECTOR3 *vecWorld, D3DXVECTOR3 *vecScreen )
{
	/** C++-ifyed function 0x71DA00, formerly called by CHudSA::CalcScreenCoors **/
	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX	m ( (float *)(0xB6FA2C) );

	// Get the static virtual screen (x,y)-sizes
	DWORD		*dwLenX = ( DWORD * ) ( 0xC17044 );
	DWORD		*dwLenY = ( DWORD * ) ( 0xC17048 );

	//DWORD *dwLenZ = (DWORD*)(0xC1704C);
	//double aspectRatio = (*dwLenX) / (*dwLenY);
	// Do a transformation
	vecScreen->x = ( vecWorld->z * m._31 ) + ( vecWorld->y * m._21 ) + ( vecWorld->x * m._11 ) + m._41;
	vecScreen->y = ( vecWorld->z * m._32 ) + ( vecWorld->y * m._22 ) + ( vecWorld->x * m._12 ) + m._42;
	vecScreen->z = ( vecWorld->z * m._33 ) + ( vecWorld->y * m._23 ) + ( vecWorld->x * m._13 ) + m._43;

	// Get the correct screen coordinates
	double	fRecip = (double)1.0 / vecScreen->z;	//(vecScreen->z - (*dwLenZ));
	vecScreen->x *= (float)( fRecip * (*dwLenX) );
	vecScreen->y *= (float)( fRecip * (*dwLenY) );
}

void CalcWorldCoors ( D3DXVECTOR3 *vecScreen, D3DXVECTOR3 *vecWorld )
{
	// Get the static view matrix as D3DXMATRIX
	D3DXMATRIX	m ( (float *)(0xB6FA2C) );

	// Invert the view matrix
	D3DXMATRIX minv;
	memset ( &minv, 0, sizeof ( D3DXMATRIX ) );
	m._44 = 1.0f;
	D3DXMatrixInverse ( &minv, NULL, &m );

	DWORD		*dwLenX = ( DWORD * ) ( 0xC17044 );
	DWORD		*dwLenY = ( DWORD * ) ( 0xC17048 );

	// Reverse screen coordinates
	double fRecip = (double)1.0 / vecScreen->z;
	vecScreen->x /= (float)(fRecip * (*dwLenX) );
	vecScreen->y /= (float)(fRecip * (*dwLenY) );

	// Do an (inverse) transformation
	vecWorld->x = ( vecScreen->z * minv._31 ) + ( vecScreen->y * minv._21 ) + ( vecScreen->x * minv._11 ) + minv._41;
	vecWorld->y = ( vecScreen->z * minv._32 ) + ( vecScreen->y * minv._22 ) + ( vecScreen->x * minv._12 ) + minv._42;
	vecWorld->z = ( vecScreen->z * minv._33 ) + ( vecScreen->y * minv._23 ) + ( vecScreen->x * minv._13 ) + minv._43;
}
