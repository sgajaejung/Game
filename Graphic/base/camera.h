#pragma once


namespace graphic
{


	class cCamera
	{
	public:
		cCamera();
		cCamera(const Vector3 &eyePos, const Vector3 &lookAt, const Vector3 &up);
		virtual ~cCamera();

		void Update();

		void SetCamera(const Vector3 &eyePos, const Vector3 &lookAt, const Vector3 &up);
		void SetEyePos(const Vector3 &eye);
		void SetLookAt(const Vector3 &lookAt);
		void SetUpVector(const Vector3 &up);
		void SetViewMatrix(const Matrix44 &view);
		const Vector3& GetEyePos();
		const Vector3& GetLookAt();
		const Vector3& GetUpVector();
		const Matrix44& GetViewMatrix();
		Vector3 GetDirection();
		Vector3 GetRight();
		float GetDistance(); // lookAt - eyePos 사이 거리.

		// LookAt 이 이동한다.
		void Pitch( const float radian );
		void Yaw( const float radian );
		void Roll( const float radian );

		// EyePos 가 이동한다.
		void Pitch2( const float radian );
		void Yaw2( const float radian );
		void Roll2( const float radian );

		void MoveFront( const float len );
		void MoveUp( const float len );
		void MoveRight( const float len );
		void Zoom( const float len );


	protected:
		void UpdateViewMatrix();


	private:
		Vector3 m_eyePos;
		Vector3 m_lookAt;
		Vector3 m_up;
		Matrix44 m_view; // 카메라 행렬.
	};


	inline void cCamera::SetEyePos(const Vector3 &eye) { m_eyePos = eye; UpdateViewMatrix(); }
	inline void cCamera::SetLookAt(const Vector3 &lookAt) { m_lookAt = lookAt; UpdateViewMatrix(); }
	inline void cCamera::SetUpVector(const Vector3 &up) { m_up = up; UpdateViewMatrix(); }
	inline void cCamera::SetViewMatrix(const Matrix44 &view) { m_view = view; }
	inline const Vector3& cCamera::GetEyePos() { return m_eyePos; }
	inline const Vector3& cCamera::GetLookAt() { return m_lookAt; }
	inline const Vector3& cCamera::GetUpVector() { return m_up; }
	inline const Matrix44& cCamera::GetViewMatrix() { return m_view; }
}
