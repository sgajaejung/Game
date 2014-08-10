#pragma once


namespace graphic
{

	class cCharacter : public cModel
	{
	public:
		cCharacter(const int id);
		virtual ~cCharacter();

		bool Create(const string &modelName);
		void LoadWeapon(const string &fileName);
		virtual bool Move(const float elapseTime) override;
		virtual void Render() override;
		virtual void RenderShader(cShader &shader) override;
		virtual void RenderShadow(cShader &shader) override;

		virtual void SetTM(const Matrix44 &tm) override;
		virtual void MultiplyTM(const Matrix44 &tm) override;

		// debug ¿ë ÇÔ¼ö.
		void SetRenderWeaponBoundingBox(const bool isRenderBoundingBox);


	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode1; // reference
		cBoneNode *m_weaponNode2; // reference
		cBoneNode *m_weaponBoneNode1; // reference
		cBoneNode *m_weaponBoneNode2; // reference
	};

}
