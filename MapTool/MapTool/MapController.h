#pragma once


class cMapController : public common::cSingleton<cMapController>
{
public:
	cMapController(void);
	~cMapController(void);

	bool LoadHeightMap(const string &fileName);
	bool LoadHeightMapTexture(const string &fileName);

	graphic::cTerrain& GetTerrain();


private:
	graphic::cTerrain m_terrain;
};


inline graphic::cTerrain& cMapController::GetTerrain() { return m_terrain; }
