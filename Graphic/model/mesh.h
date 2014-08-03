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
		virtual void RenderShader( const Matrix44 &parentTm ) override;
		virtual void RenderShader( cShader &shader, const Matrix44 &parentTm ) override;
		virtual void RenderShadow(cShader &shader, const Matrix44 &parentTm) override;

		void CreateBoundingBox(OUT cCube &out);
		void RenderBoundingBox(const Matrix44 &tm);
		const cCube& GetBoundingBox() const;
		

	protected:
		void CreateMesh( const vector<Vector3> &vertices, 
			const vector<Vector3> &normals, 
			const vector<Vector3> &tex,
			const vector<int> &indices );
		void CreateBoneWeight( const vector<sVertexWeight> &weights );
		void CreateMaterials(const sRawMesh &rawMesh);
		void CreateAttributes(const sRawMesh &rawMesh);


	protected:
		bool m_isSkinned;
		vector<cMaterial> m_mtrls;
		vector<cTexture*>m_textures;  // reference
		vector<sAttribute> m_attributes;
		cVertexBuffer m_vtxBuff;
		cIndexBuffer m_idxBuff;

		// 경계박스
		cCube m_boundingBox;
	};


	inline const cCube& cMesh::GetBoundingBox() const { return m_boundingBox; }
}
