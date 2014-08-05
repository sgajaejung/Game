#pragma once


class cMapController : public common::cSingleton<cMapController>
{
public:
	cMapController(void);
	~cMapController(void);

	bool LoadHeightMap(const string &fileName);
	bool LoadHeightMapTexture(const string &fileName);

	graphic::cTerrain& GetTerrain();
	const string& GetHeightMapFileName();
	const string& GetTextureFileName();


private:
	graphic::cTerrain m_terrain;
	string m_heightMapFileName;
	string m_textFileName;
};


inline graphic::cTerrain& cMapController::GetTerrain() { return m_terrain; }
inline const string& cMapController::GetHeightMapFileName() { return m_heightMapFileName; }
inline const string& cMapController::GetTextureFileName() { return m_textFileName; }
