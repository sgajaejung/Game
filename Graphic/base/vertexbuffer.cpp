
#include "stdafx.h"
#include "vertexbuffer.h"

using namespace graphic;


cVertexBuffer::cVertexBuffer() : 
	m_fvf(0),
	m_sizeOfVertex(0),
	m_vertexCount(0),
	m_pVtxBuff(NULL)
{

}

cVertexBuffer::~cVertexBuffer()
{
	Clear();
}


bool cVertexBuffer::Create(int vertexCount, int sizeofVertex, DWORD fvf)
{
	SAFE_RELEASE(m_pVtxBuff);

	if (FAILED(GetDevice()->CreateVertexBuffer( vertexCount*sizeofVertex,
		D3DUSAGE_WRITEONLY, 
		fvf,
		D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		return false;
	}

	m_fvf = fvf;
	m_vertexCount = vertexCount;
	m_sizeOfVertex = sizeofVertex;
	return true;
}


void* cVertexBuffer::Lock()
{
	if (!m_pVtxBuff)
		return NULL;

	void *vertices = NULL;
	if (FAILED(m_pVtxBuff->Lock( 0, 0, (void**)&vertices, 0)))
		return NULL;

	return vertices;
}


void cVertexBuffer::Unlock()
{
	if (!m_pVtxBuff)
		return;

	m_pVtxBuff->Unlock();
}


void cVertexBuffer::Bind() const
{
	GetDevice()->SetStreamSource( 0, m_pVtxBuff, 0, m_sizeOfVertex );
	GetDevice()->SetFVF( m_fvf );
}


void cVertexBuffer::RenderLineStrip()
{
	Matrix44 matIdentity;
	GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&matIdentity );

	DWORD lighting;
	GetDevice()->GetRenderState( D3DRS_LIGHTING, &lighting );
	GetDevice()->SetRenderState( D3DRS_LIGHTING, FALSE );
	GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
	Bind();
	GetDevice()->DrawPrimitive( D3DPT_LINESTRIP, 0, m_vertexCount-1);
	GetDevice()->SetRenderState( D3DRS_LIGHTING, lighting );
	GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
}



void cVertexBuffer::Clear()
{
	m_fvf = 0;
	m_vertexCount = 0;
	m_sizeOfVertex = 0;
	SAFE_RELEASE(m_pVtxBuff);	
}


cVertexBuffer& cVertexBuffer::operator=(cVertexBuffer &rhs)
{
	if (this != &rhs)
	{
		m_fvf = rhs.m_fvf;
		m_sizeOfVertex = rhs.m_sizeOfVertex;
		m_vertexCount = rhs.m_vertexCount;
		
		if (Create(rhs.m_vertexCount, rhs.m_sizeOfVertex, rhs.m_fvf))
		{
			if (BYTE* dest = (BYTE*)Lock())
			{
				if (BYTE *src = (BYTE*)rhs.Lock())
				{
					memcpy(dest, src, rhs.m_vertexCount*m_sizeOfVertex);
					rhs.Unlock();
				}
				Unlock();
			}
		}
	}
	return *this;
}
