// 투영 그림자 클래스
// 모델 하나의 그림자 텍스쳐를 생성한다.
// 하나 이상의 모델의 그림자를 표현 할 때는 이 클래스를 사용하지 않는게 좋다.
#pragma once


namespace graphic
{

	class cShadow1
	{
	public:
		cShadow1();
		virtual ~cShadow1();

		bool Create(const int textureWidth, const int textureHeight);
		void UpdateShadow( cNode &node );
		void RenderShadowMap();
		IDirect3DTexture9* GetTexture();


	private:
		cSurface m_surface;
	};


	inline IDirect3DTexture9* cShadow1::GetTexture() { return m_surface.GetTexture(); }
}
