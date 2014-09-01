// cMeshBuffer
// 메쉬를 구성하는 버텍스 버퍼, 인덱스 버퍼, 속성 버퍼를 
// 저장하고, 관리하는 클래스다.

#pragma once


namespace graphic
{
	struct sRawMesh;
	struct sRawBone;

	class cMeshBuffer
	{
	public:
		cMeshBuffer(const sRawMesh &rawMesh);
		cMeshBuffer(const sRawBone &rawBone);

		void Bind();

		vector<sAttribute>& GetAttributes();
		cVertexBuffer& GetVertexBuffer();
		cIndexBuffer& GetIndexBuffer();


	protected:
		void CreateMesh( const vector<Vector3> &vertices, 
			const vector<Vector3> &normals, 
			const vector<Vector3> &tex,
			const vector<int> &indices );
		void CreateBoneWeight( const vector<sVertexWeight> &weights );
		void CreateAttributes(const sRawMesh &rawMesh);


	private:
		vector<sAttribute> m_attributes;
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;
	};


	inline vector<sAttribute>& cMeshBuffer::GetAttributes() { return m_attributes; }
	inline cVertexBuffer& cMeshBuffer::GetVertexBuffer() { return m_vtxBuff; }
	inline cIndexBuffer& cMeshBuffer::GetIndexBuffer() { return m_idxBuff; }
}
