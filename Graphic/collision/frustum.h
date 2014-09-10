// 절두체 클래스.
#pragma once


namespace graphic
{


	class cFrustum
	{
	public:
		cFrustum();
		virtual ~cFrustum();

		bool Create( const Matrix44 &matViewProj );
		bool IsIn( const Vector3 &point );
		bool IsInSphere( const Vector3 &point, float radius );
		void Render();
		const Vector3& GetPos() const;


	private:
		Vector3 m_Vtx[ 8]; // frustum 정점 좌표
		Plane m_Plane[ 6];	// frustum의 6개 평면
		Vector3 m_vPos; // 절두체 위치
	};


	const Vector3& cFrustum::GetPos() const { return m_vPos; }
}
