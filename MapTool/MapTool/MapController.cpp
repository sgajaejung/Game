#include "StdAfx.h"
#include "MapController.h"

using namespace graphic;


cMapController::cMapController(void) :
	m_editMode(EDIT_MODE::MODE_HEIGHTMAP)
{
	m_camera.SetCamera(Vector3(100,100,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection( D3DX_PI / 4.f, (float)VIEW_WIDTH / (float) VIEW_HEIGHT, 1.f, 10000.0f);

}


cMapController::~cMapController(void)
{
}


// 지형 파일 열기.
bool cMapController::LoadTerrainFile(const string &fileName)
{
	sRawTerrain rawTerrain;
	if (importer::ReadRawTerrainFile(fileName, rawTerrain))
	{
		m_terrain.LoadTerrain(rawTerrain);
	}
	else
	{
		return false;
	}

	NotifyObserver();
	return true;
}


// 높이맵 파일을 읽어서 지형에 적용한다.
bool cMapController::LoadHeightMap(const string &fileName)
{
	m_heightMapFileName = fileName;
	const bool result = m_terrain.CreateFromHeightMap(fileName, "empty" );

	NotifyObserver();
	return result;
}


// 지형 전체를 덮어 씌울 텍스쳐를 로딩한다.
bool cMapController::LoadHeightMapTexture(const string &fileName)
{
	m_textFileName = fileName;
	const bool result = m_terrain.CreateTerrainTexture(fileName);

	NotifyObserver();
	return result;
}


// 지형 정보를 파일에 저장한다.
bool cMapController::SaveTerrainFile(const string &fileName)
{
	sRawTerrain rawTerrain;
	m_terrain.GenerateRawTerrain(rawTerrain);

	// 알파 텍스쳐 파일 이름 설정.
	string name = common::GetFileNameExceptExt(fileName);
	name += "_alpha.png";
	string path = common::GetFilePathExceptFileName(fileName);
	if (!path.empty())
		path += "\\";

	const string alphaTextureName = path + name;
	rawTerrain.alphaTexture = graphic::cResourceManager::Get()->GetRelativePathToMedia(alphaTextureName);
	m_terrain.GetAlphaTexture().WriteFile(alphaTextureName);

	const bool result = exporter::WriteRawTerrainFile(fileName, rawTerrain);

	if (result)
	{
		AfxMessageBox( L"저장 성공!\n\n모든 파일 경로는 media 폴더의 상대주소로 저장됩니다. \nmedia폴더 안에 있지 않은 파일은 문제가 될 수 있으니,\n다시 한번 확인해주세요." );
	}
	else
	{
		AfxMessageBox( L"파일 저장 실패" );
	}

	return result;
}


// 지형 생성.
bool cMapController::CreateDefaultTerrain()
{
	m_terrain.CreateTerrain(64, 64, 50.f, 8.f);
	m_terrain.CreateTerrainTexture( "../../media/terrain/모래1.jpg" );

	NotifyObserver();
	return true;
}


// 툴 편집 모드 설정.
void cMapController::ChangeEditMode(EDIT_MODE::TYPE mode)
{
	m_editMode = mode;
}


// 스플래팅 텍스쳐 모드에서, 마우스로 브러슁을 할 때 호출한다.
void cMapController::Brush(CPoint point)
{
	const Ray ray(point.x, point.y, VIEW_WIDTH, VIEW_HEIGHT, 
		m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix() );

	Vector3 pickPos;
	m_terrain.Pick( ray.orig, ray.dir, pickPos );
	m_cursor.UpdateCursor( m_terrain, pickPos );

	const int oldLayerCount = m_terrain.GetSplatLayerCount();
	m_terrain.Brush( m_cursor );

	if (m_terrain.GetSplatLayerCount() != oldLayerCount)
	{
		NotifyObserver(NOTIFY_TYPE::NOTIFY_ADD_LAYER);
	}
}


// 브러쉬 업데이트.
void cMapController::UpdateBrush()
{
	CPoint pos(VIEW_WIDTH/2, VIEW_HEIGHT/2);
	Ray ray(pos.x, pos.y, VIEW_WIDTH, VIEW_HEIGHT, 
		m_camera.GetProjectionMatrix(), m_camera.GetViewMatrix() );

	Vector3 pickPos;
	m_terrain.Pick( ray.orig, ray.dir, pickPos);
	m_cursor.UpdateCursor(m_terrain, pickPos );

	NotifyObserver( NOTIFY_TYPE::NOTIFY_CHANGE_CURSOR );
}


// HeightFactor 가 업데이트 될 때 호출 한다.
void cMapController::UpdateHeightFactor(const float heightFactor)
{
	m_terrain.SetHeightFactor(heightFactor);
}


// 지형위에 모델이 추가되거나 제거 될 때 호출한다.
void cMapController::UpdatePlaceModel()
{
	NotifyObserver( NOTIFY_TYPE::NOTIFY_ADD_PLACE_MODEL );
}
