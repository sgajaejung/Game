#pragma once


namespace graphic
{

	class cVertexBuffer
	{
	public:
		cVertexBuffer();
		virtual ~cVertexBuffer();

		bool Create(const int vertexCount, const int sizeofVertex, DWORD fvf);
		//bool CreateParticle(const int particleCount);
		void* Lock();
		void Unlock();
		void Bind() const;
		void Clear();

		void RenderLineStrip();

		DWORD GetFVF() const;
		int GetSizeOfVertex() const;
		int GetVertexCount() const;

		cVertexBuffer& operator=(cVertexBuffer &rhs);


	private:
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		DWORD m_fvf;
		int m_sizeOfVertex; // stride
		int m_vertexCount;
	};

	
	inline DWORD cVertexBuffer::GetFVF() const { return m_fvf; }
	inline int cVertexBuffer::GetSizeOfVertex() const { return m_sizeOfVertex; }
	inline int cVertexBuffer::GetVertexCount() const { return m_vertexCount; }
}
