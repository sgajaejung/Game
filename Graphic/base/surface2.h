// RenderTarget 전용 서피스.
// 그림자를 만들 때 사용한다.
// cSurface 클래스를 다듬은 클래스다.
#pragma once


namespace graphic
{

	class cSurface2
	{
	public:
		cSurface2();
		virtual ~cSurface2();

		bool Create(int width, int height, int mipLevels,
			D3DFORMAT texFormat, bool useDepthBuffer,
			D3DFORMAT depthFormat, D3DVIEWPORT9& viewport, bool autoGenMips);

		void Begin();
		void End();
		void Render();
		void Clear();

		bool IsLoaded() const;
		IDirect3DTexture9* GetTexture();


	private:
		IDirect3DTexture9 *m_texture;
		ID3DXRenderToSurface *m_rts;
		IDirect3DSurface9 *m_surface;

		int m_width;
		int m_height;
		int m_mipLevels;
		D3DFORMAT m_texFormat;
		bool m_useDepthBuffer;
		D3DFORMAT m_depthFormat;
		D3DVIEWPORT9 m_viewPort;
		bool m_autoGenMips;
	};


	inline bool cSurface2::IsLoaded() const { return m_texture? true : false; }
	inline IDirect3DTexture9* cSurface2::GetTexture() { return m_texture; }
}
