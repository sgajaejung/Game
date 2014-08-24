#pragma once


namespace graphic
{

	class cCharacterAnalyzer : public cCharacter
	{
	public:
		cCharacterAnalyzer(const int id);
		virtual ~cCharacterAnalyzer();

		virtual bool Move(const float elapseTime) override;
		virtual void Render(const Matrix44 &tm) override;


	protected:


	private:
		
	};

}
