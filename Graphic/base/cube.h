#pragma once


namespace graphic
{

	class cCube
	{
	public:
		cCube();
		cCube(const Vector3 &vMin, const Vector3 &vMax );

		void SetCube(const Vector3 &vMin, const Vector3 &vMax );
		void SetTransform( const Matrix44 &tm );
		void Render(const Matrix44 &tm);


	protected:
		void InitCube();


	private:
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
		Vector3 m_min;
		Vector3 m_max;
		Matrix44 m_tm;
	};	


	inline void cCube::SetTransform( const Matrix44 &tm ) { m_tm = tm; }
}
