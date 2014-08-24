
#include "stdafx.h"
#include "character.h"
#include "characteranalyzer.h"

using namespace graphic;

cCharacterAnalyzer::cCharacterAnalyzer(const int id) :
	cCharacter(id)
{
}

cCharacterAnalyzer::~cCharacterAnalyzer()
{
}


bool cCharacterAnalyzer::Move(const float elapseTime)
{

	return true;
}


void cCharacterAnalyzer::Render(const Matrix44 &tm)
{

}
