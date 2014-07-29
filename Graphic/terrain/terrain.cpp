
#include "stdafx.h"
#include "terrain.h"
#include <objidl.h>
#include <gdiplus.h> 
#pragma comment( lib, "gdiplus.lib" ) 
using namespace std;
using namespace Gdiplus;
using namespace graphic;


ULONG_PTR g_gdiplusToken;
GdiplusStartupInput g_gdiplusStartupInput; 

cTerrain::cTerrain()
{
	static bool onlyOne = false;
	if (!onlyOne)
	{	
		Gdiplus::GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL); 
		onlyOne = true;
	}
}

cTerrain::~cTerrain()
{
	static bool onlyOne = false;
	if (!onlyOne)
	{
		Gdiplus::GdiplusShutdown(g_gdiplusToken);
		onlyOne = true;
	}
}


bool cTerrain::CreateFromHeightMap( const string &heightMapFileName, 
	const string &textureFileName, const float heightFactor ) // heightFactor=3.f
{
	wstring wfileName = common::str2wstr(heightMapFileName);
	Bitmap bmp(wfileName.c_str());
	
	m_grid.Create(64, 64, 50, 1.f);
	// 3200 x 3200

	const int width = bmp.GetWidth();
	const int height = bmp.GetHeight();

	const int incX = width / 64;
	const int incY = height / 64;

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();

	for (int i=0; i < 65; ++i)
	{
		for (int k=0; k < 65; ++k)
		{
			sVertexNormTex *vtx = pv + (k*65) + i;

			Color color;
			bmp.GetPixel(i*incX, k*incY, &color);
			const float h = ((color.GetR() + color.GetG() + color.GetB()) / 3.f) 
				* heightFactor;
			vtx->p.y = h;
		}
	}

	m_grid.GetVertexBuffer().Unlock();

	m_grid.CalculateNormals();
	m_grid.GetTexture().Create( textureFileName );
	return true;
}


void cTerrain::Render()
{
	m_grid.Render();
}
