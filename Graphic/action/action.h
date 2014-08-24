#pragma once


namespace graphic
{

	namespace CHARACTER_ACTION
	{
		enum TYPE
		{
			NORMAL,
			RUN,
			DASH,
			ATTACK,
			GUARD,
		};
	}


	struct sActionData
	{
		CHARACTER_ACTION::TYPE type;
		string animationFile;

		sActionData() {}
		sActionData(const CHARACTER_ACTION::TYPE type0, const string &animationFile0) :
		type(type0), animationFile(animationFile0) { 
		}

	};


}
