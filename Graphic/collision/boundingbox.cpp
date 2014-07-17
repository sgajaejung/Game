
#include "stdafx.h"
#include "boundingbox.h"

using namespace graphic;


void cBoundingBox::SetBoundingBox(const Vector3 &vMin, const Vector3 &vMax )
{
	m_min = vMin;
	m_max = vMax;
}


void cBoundingBox::SetTransform( const Matrix44 &tm )
{
	m_tm = tm;
}


bool cBoundingBox::Collision( cBoundingBox &box )
{

	return false;
}
