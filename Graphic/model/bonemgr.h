#pragma once


namespace graphic
{
	class cBoneNode;

	class cBoneMgr
	{
	public:
		cBoneMgr(const int id, const sRawMeshGroup &rawMeshes);
		virtual ~cBoneMgr();

		void SetAnimation( const sRawAniGroup &rawAnies, int nAniFrame );
		void SetCurrentAnimationFrame(const int curFrame);

		bool Move(const float elapseTime);
		void Render(const Matrix44 &parentTm);
		void RenderBoundingBox(const Matrix44 &parentTm);
		void Clear();
		cBoneNode* GetRoot();
		vector<cBoneNode*>& GetAllBoneNode();
		cBoneNode* FindBone(const int id);
		cBoneNode* FindBone(const string &name);
		vector<Matrix44>& GetPalette();


	protected:
		void SetAnimationRec( cBoneNode *node, const sRawAniGroup &rawAni, int nAniFrame );
		void CreateBoundingBox(const sRawMeshGroup &rawMeshes);
		void SetBoundingBoxIndex(cBoneNode *node, OUT map<int, int> &boneIndices, const int boneIdx=-1);


	private:
		int m_id;
		cBoneNode *m_root;
		vector<cBoneNode*> m_bones;
		vector<Matrix44> m_palette;
		vector<cCube> m_boundingBox;
	};


	inline cBoneNode* cBoneMgr::GetRoot() { return m_root; }
	inline vector<Matrix44>& cBoneMgr::GetPalette() { return m_palette; }
	inline vector<cBoneNode*>& cBoneMgr::GetAllBoneNode() { return m_bones; }
}
