#pragma once


namespace graphic
{
	class cBoneMgr;
	class cBoundingBox;

	class cModel : public cNode 
						, public ICollisionable
	{
	public:
		cModel(const int id);
		virtual ~cModel();

		virtual bool Create(const string &modelName, MODEL_TYPE::TYPE type = MODEL_TYPE::AUTO);
		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &tm) override;
		virtual void RenderShadow(const Matrix44 &viewProj, 
			const Vector3 &lightPos, const Vector3 &lightDir, const Matrix44 &parentTm) override;

		void Clear();

		// Model Infomations
		const string& GetFileName() const;
		cShadow1& GetShadow();
		bool IsRenderShadow() const;
		void SetRenderShadow(const bool show);
		cMesh* GetMesh(const string &name);
		cMesh* GetMesh(const int idx);

		// Animation
		virtual bool SetAnimation(const string &aniFileName);
		cBoneMgr* GetBoneMgr();
		cMesh* FindMesh(const string &meshName);
		sRawAniGroup* GetCurrentAnimation();
		bool SharePalette(vector<Matrix44> *palette);

		bool Pick(const Vector3 &orig, const Vector3 &dir);
		cModel* Clone() const;

		// ICollisionable Interface
		virtual bool IsTest( int testNum ) override;
		virtual int GetCollisionId() override;
		virtual void UpdateCollisionBox() override;
		virtual cBoundingBox* GetCollisionBox() override;
		virtual void Collision( int testNum, ICollisionable *obj ) override;

		// debug 용 함수.
		void SetRenderMesh(const bool isRenderMesh);
		void SetRenderBone(const bool isRenderBone);
		void SetRenderBoundingBox(const bool isRenderBoundingBox);


	protected:
		string m_fileName; //model 파일 명.
		MODEL_TYPE::TYPE m_type;
		vector<cMesh*> m_meshes;
		cBoneMgr *m_bone;
		cBoundingBox m_boundingBox; // only rigid mesh model
		sRawAniGroup *m_curAni; // reference, 애니메이션 정보.
		cShadow1 m_shadow;
		bool m_isRenderShadow; // default = false
		
		// debug 용.
		bool m_isRenderMesh; // default = true
		bool m_isRenderBone; // default = false
		bool m_isRenderBoundingBox; // default = false
	};


	inline cBoneMgr* cModel::GetBoneMgr() { return m_bone; }
	inline sRawAniGroup* cModel::GetCurrentAnimation() { return m_curAni; }
	inline const string& cModel::GetFileName() const { return m_fileName; }
	inline cShadow1& cModel::GetShadow() { return m_shadow; }
	inline bool cModel::IsRenderShadow() const { return m_isRenderShadow; }
	inline void cModel::SetRenderShadow(const bool show) { m_isRenderShadow = show; }
}
