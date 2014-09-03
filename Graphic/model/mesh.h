#pragma once

#include "node.h"

namespace graphic
{

	class cMesh : public cNode
	{
	public:
		cMesh(const int id, const sRawMesh &rawMesh);
		cMesh(const int id, const sRawBone &rawBone);
		virtual ~cMesh();

		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &parentTm) override;
		virtual void RenderShadow(const Matrix44 &viewProj, 
			const Vector3 &lightPos, const Vector3 &lightDir, const Matrix44 &parentTm) override;

		void CreateBoundingBox(OUT cCube &out);
		void RenderBoundingBox(const Matrix44 &tm);
		cCube& GetBoundingBox();
		cMeshBuffer* GetMeshBuffer();
		void SetMeshBuffer(cMeshBuffer *buffer);
		

	protected:
		void CreateMesh( const vector<Vector3> &vertices, 
			const vector<Vector3> &normals, 
			const vector<Vector3> &tex,
			const vector<int> &indices );
		void CreateBoneWeight( const vector<sVertexWeight> &weights );
		void CreateMaterials(const sRawMesh &rawMesh);
		void CreateAttributes(const sRawMesh &rawMesh);

		virtual void RenderShader( cShader &shader, const Matrix44 &parentTm );
		virtual void RenderShadow(cShader &shader, const Matrix44 &parentTm);


	protected:
		bool m_isSkinned;
		vector<cMaterial> m_mtrls;
		vector<cTexture*>m_textures;  // reference
		cMeshBuffer *m_buffers; // reference

		// 경계박스
		cCube m_boundingBox;

		// debug 용.
		bool m_isBoneMesh;
	};


	inline cCube& cMesh::GetBoundingBox() { return m_boundingBox; }
	inline cMeshBuffer* cMesh::GetMeshBuffer() { return m_buffers; }
	inline void cMesh::SetMeshBuffer(cMeshBuffer *buffer) { m_buffers = buffer; }
}
