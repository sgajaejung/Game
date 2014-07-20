
#include "stdafx.h"
#include "boundingbox.h"

using namespace graphic;


cBoundingBox::cBoundingBox()
{
}

cBoundingBox::cBoundingBox(const cCube &cube)
{
	operator=(cube);
}


void cBoundingBox::SetBoundingBox(const Vector3 &vMin, const Vector3 &vMax )
{
	m_min = vMin;
	m_max = vMax;
}


void cBoundingBox::SetTransform( const Matrix44 &tm )
{
	m_tm = tm;
}

void SATtest( const Vector3& axis, const Vector3 ptSet[8], float& minAlong, float& maxAlong )
{
	minAlong=HUGE, maxAlong=-HUGE;
	for( u_int i = 0 ; i < 8; i++ )
	{
		// just dot it to get the min/max along this axis.
		float dotVal = ptSet[ i].DotProduct( axis ) ;
		if( dotVal < minAlong )  minAlong=dotVal;
		if( dotVal > maxAlong )  maxAlong=dotVal;
	}
}


inline bool isBetweenOrdered( float val, float lowerBound, float upperBound ) {
	return lowerBound <= val && val <= upperBound ;
}

bool overlaps( float min1, float max1, float min2, float max2 )
{
	return isBetweenOrdered( min2, min1, max1 ) || isBetweenOrdered( min1, min2, max2 ) ;
}


// OBB vs OBB 충돌처리.
bool cBoundingBox::Collision( cBoundingBox &box )
{
	vector<Vector3> normals(6);
	normals[ 0] = Vector3(0,1,0).MultiplyNormal(m_tm);
	normals[ 1] = Vector3(0,0,1).MultiplyNormal(m_tm);
	normals[ 2] = Vector3(1,0,0).MultiplyNormal(m_tm);
	normals[ 3] = Vector3(0,1,0).MultiplyNormal(box.m_tm);
	normals[ 4] = Vector3(0,0,1).MultiplyNormal(box.m_tm);
	normals[ 5] = Vector3(1,0,0).MultiplyNormal(box.m_tm);

	Vector3 vertices1[8] = {
		Vector3(m_min.x, m_max.y, m_min.z), 
		Vector3(m_max.x, m_max.y, m_min.z), 
		Vector3(m_min.x, m_min.y, m_min.z), 
		Vector3(m_max.x, m_min.y, m_min.z),
		Vector3(m_min.x, m_max.y, m_max.z), 
		Vector3(m_max.x, m_max.y, m_max.z),
		Vector3(m_min.x, m_min.y, m_max.z), 
		Vector3(m_max.x, m_min.y, m_max.z),
	};
	for (u_int i=0; i < 8; ++i)
		vertices1[ i] *= m_tm;


	Vector3 vertices2[8] = {
		Vector3(box.m_min.x, box.m_max.y, box.m_min.z), 
		Vector3(box.m_max.x, box.m_max.y, box.m_min.z), 
		Vector3(box.m_min.x, box.m_min.y, box.m_min.z), 
		Vector3(box.m_max.x, box.m_min.y, box.m_min.z),
		Vector3(box.m_min.x, box.m_max.y, box.m_max.z), 
		Vector3(box.m_max.x, box.m_max.y, box.m_max.z),
		Vector3(box.m_min.x, box.m_min.y, box.m_max.z), 
		Vector3(box.m_max.x, box.m_min.y, box.m_max.z),
	};
	for (u_int i=0; i < 8; ++i)
		vertices2[ i] *= box.m_tm;


	for( u_int i = 0 ; i < normals.size() ; i++ )
	{
		float shape1Min, shape1Max, shape2Min, shape2Max ;
		SATtest( normals[ i], vertices1, shape1Min, shape1Max ) ;
		SATtest( normals[i], vertices2, shape2Min, shape2Max ) ;
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false; // NO INTERSECTION
		}
	}

	return true;
}


// 할당 연산자.
cBoundingBox& cBoundingBox::operator=(const cCube &cube)
{
	SetBoundingBox(cube.GetMin(), cube.GetMax());
	SetTransform(cube.GetTransform());
	return *this;
}
