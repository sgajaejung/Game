#pragma once


class cController : public common::cSingleton<cController>
{
public:
	cController();
	virtual ~cController();

	bool LoadFile( const string &fileName );
	graphic::cModel* GetModel();

protected:


private:
	graphic::cModel *m_model;

};


inline graphic::cModel* cController::GetModel() { return m_model; }
