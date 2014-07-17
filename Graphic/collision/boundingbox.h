#pragma once


namespace graphic
{

	class cBoundingBox
	{
	public:
		void SetBoundingBox(const Vector3 &vMin, const Vector3 &vMax );
		void SetTransform( const Matrix44 &tm );
		bool Collision( cBoundingBox &box );


		Vector3 m_min;
		Vector3 m_max;
		Matrix44 m_tm;
	};

}
