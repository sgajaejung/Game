#pragma once


namespace graphic
{

	class cTerrain
	{
	public:
		cTerrain();
		virtual ~cTerrain();

		bool CreateFromHeightMap( const string &heightMapFileName, 
			const string &textureFileName, const float heightFactor=3.f, const float textureUVFactor=1.f );
		bool CreateTerrainTexture( const string &textureFileName );
		bool CreateTerrain( const int rowCellCount=64, const int colCellCount=64, const float cellSize=50.f,
			const float textureUVFactor=1.f );

		float GetHeight(const float x, const float z);
		float GetHeightFromRay( const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out );
		bool Pick(const int x, const int y, const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out);
		void Render();
		void RenderShader(cShader &shader);

		int GetRowCellCount() const;
		int GetColCellCount() const;
		float GetCellSize() const;
		const string& GetTextureName();

		void Clear();


	protected:
		float GetHeightMapEntry( int row, int col );
		bool UpdateHeightMap( const string &heightMapFileName, 
			const string &textureFileName, const float heightFactor );


	private:
		int m_rowCellCount;
		int m_colCellCount;
		float m_cellSize;
		cGrid2 m_grid;
	};


	inline int cTerrain::GetRowCellCount() const { return m_rowCellCount; }
	inline int cTerrain::GetColCellCount() const { return m_colCellCount; }
	inline float cTerrain::GetCellSize() const { return m_cellSize; }
}
