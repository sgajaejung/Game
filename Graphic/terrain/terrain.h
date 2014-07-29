#pragma once


namespace graphic
{

	class cTerrain
	{
	public:
		cTerrain();
		virtual ~cTerrain();

		bool CreateFromHeightMap( const string &heightMapFileName, 
			const string &textureFileName );
		void Render();


	private:
		cGrid2 m_grid;
	};

}
