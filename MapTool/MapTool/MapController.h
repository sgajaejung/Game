#pragma once


class cMapController  : public common::cObservable
									,public common::cSingleton<cMapController>
{
public:
	cMapController(void);
	~cMapController(void);

	bool LoadHeightMap(const string &fileName);
	bool LoadHeightMapTexture(const string &fileName);
	bool CreateDefaultTerrain();

	graphic::cTerrain& GetTerrain();
	const string& GetHeightMapFileName();
	const string& GetTextureFileName();

	void ChangeEditMode(EDIT_MODE::TYPE mode);
	EDIT_MODE::TYPE GetEditMode() const;


private:
	graphic::cTerrain m_terrain;
	string m_heightMapFileName;
	string m_textFileName;
	EDIT_MODE::TYPE m_editMode;
};


inline graphic::cTerrain& cMapController::GetTerrain() { return m_terrain; }
inline const string& cMapController::GetHeightMapFileName() { return m_heightMapFileName; }
inline const string& cMapController::GetTextureFileName() { return m_textFileName; }
inline EDIT_MODE::TYPE cMapController::GetEditMode() const { return m_editMode; }
