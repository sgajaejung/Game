#pragma once


class cController : public common::cObservable
							, public common::cSingleton<cController>							
{
public:
	cController();
	virtual ~cController();

	bool LoadFile( const string &fileName );
	graphic::cModel* GetModel();
	const string& GetCurrentMeshFileName();
	const string& GetCurrentAnimationFileName();


protected:


private:
	graphic::cModel *m_model;
	string m_currentMeshFileName;
	string m_currentAnimationFileName;

};


inline graphic::cModel* cController::GetModel() { return m_model; }
inline const string& cController::GetCurrentMeshFileName() { return m_currentMeshFileName; }
inline const string& cController::GetCurrentAnimationFileName() { return m_currentAnimationFileName; }
