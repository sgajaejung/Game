#include "StdAfx.h"
#include "MapController.h"


cMapController::cMapController(void)
{
}


cMapController::~cMapController(void)
{
}


// 높이맵 파일을 읽어서 지형에 적용한다.
bool cMapController::LoadHeightMap(const string &fileName)
{

	m_terrain.CreateFromHeightMap(fileName, "empty" );

	return true;
}


// 지형 전체를 덮어 씌울 텍스쳐를 로딩한다.
bool cMapController::LoadHeightMapTexture(const string &fileName)
{

	return true;
}
