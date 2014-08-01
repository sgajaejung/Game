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
		virtual void RenderShader(cShader &shader);


	private:
		cModel *m_weapon;
		cBoneNode *m_weaponNode; // reference
	};

}
