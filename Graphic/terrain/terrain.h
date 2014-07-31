#pragma once


namespace graphic
{

	class cTerrain
	{
	public:
		cTerrain();
		virtual ~cTerrain();

		bool CreateFromHeightMap( const string &heightMapFileName, 
			const string &textureFileName, const float heightFactor=3.f );
		float GetHeight(const float x, const float z);
		float GetHeightFromRay( const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out );
		bool Pick(const int x, const int y, const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out);
		void Render();
		void RenderShader(cShader &shader);


	protected:
		float cTerrain::GetHeightMapEntry( int row, int col );


	private:
		cGrid2 m_grid;
	};

}
