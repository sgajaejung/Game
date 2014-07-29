
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
	const int CELL_COL_COUNT = 128;
	const int CELL_ROW_COUNT = 128;
	const int VERTEX_COL_COUNT = CELL_COL_COUNT+1;
	const int VERTEX_ROW_COUNT = CELL_ROW_COUNT+1;
	const float CELL_SIZE = 50.f;

	const wstring wfileName = common::str2wstr(heightMapFileName);
	Bitmap bmp(wfileName.c_str());
	
	m_grid.Create(CELL_ROW_COUNT, CELL_COL_COUNT, CELL_SIZE, 1.f);

	const float incX = (float)(bmp.GetWidth()-1) / (float)CELL_COL_COUNT;
	const float incY = (float)(bmp.GetHeight()-1) /(float) CELL_ROW_COUNT;

	sVertexNormTex *pv = (sVertexNormTex*)m_grid.GetVertexBuffer().Lock();

	for (int i=0; i < VERTEX_COL_COUNT; ++i)
	{
		for (int k=0; k < VERTEX_ROW_COUNT; ++k)
		{
			sVertexNormTex *vtx = pv + (k*VERTEX_COL_COUNT) + i;

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
