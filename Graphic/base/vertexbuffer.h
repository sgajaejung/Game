#pragma once


namespace graphic
{

	class cVertexBuffer
	{
	public:
		cVertexBuffer();
		virtual ~cVertexBuffer();

		bool Create(const int vertexCount, const int sizeofVertex, DWORD fvf);
		bool CreateVMem(const int vertexCount, const int sizeofVertex, DWORD fvf);

		void* Lock();
		void* LockDiscard(const int idx=0, const int size=0);
		void* LockNooverwrite(const int idx=0, const int size=0);
		void Unlock();
		void Bind() const;
		void Clear();

		void RenderLineStrip();
		void RenderPointList(const int count=0);

		DWORD GetFVF() const;
		int GetSizeOfVertex() const;
		int GetVertexCount() const;

		cVertexBuffer& operator=(cVertexBuffer &rhs);


	private:
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
		DWORD m_fvf;
		int m_sizeOfVertex; // stride
		int m_vertexCount;
		bool m_isManagedPool;
	};

	
	inline DWORD cVertexBuffer::GetFVF() const { return m_fvf; }
	inline int cVertexBuffer::GetSizeOfVertex() const { return m_sizeOfVertex; }
	inline int cVertexBuffer::GetVertexCount() const { return m_vertexCount; }
}
