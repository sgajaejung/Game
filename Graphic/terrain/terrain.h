#pragma once


namespace graphic
{

	struct sSplatLayer
	{
		cTexture *texture;	
		sSplatLayer() : texture(NULL) {}
	};


	class cTerrain
	{
	protected:
		enum {
			MAX_LAYER = 4,
			ALPHA_TEXTURE_SIZE_W = 256,
			ALPHA_TEXTURE_SIZE_H = 256,
		};


	public:
		cTerrain();
		virtual ~cTerrain();

		bool CreateFromTRNFile(const string &fileName);

		bool CreateFromRawTerrain( const sRawTerrain &rawTerrain );

		bool CreateFromHeightMap( const string &heightMapFileName, 
			const string &textureFileName, const float heightFactor=3.f, const float textureUVFactor=1.f,
			 const int rowCellCount=64, const int colCellCount=64, const float cellSize=50.f );

		bool CreateFromGRDFormat( const string &gridFileName, 
			const string &textureFileName, const float heightFactor=3.f, const float textureUVFactor=1.f,
			const int rowCellCount=64, const int colCellCount=64, const float cellSize=50.f );

		bool CreateTerrain( const int rowCellCount=64, const int colCellCount=64, const float cellSize=50.f,
			const float textureUVFactor=1.f );

		bool CreateTerrainTexture( const string &textureFileName );

		float GetHeight(const float x, const float z);
		float GetHeightFromRay( const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out );
		bool Pick(const Vector3 &orig, const Vector3 &dir, OUT Vector3 &out);
		cModel* PickModel(const Vector3 &orig, const Vector3 &dir);

		bool IsLoaded() const;

		// model
		bool AddRigidModel(const cModel &model);
		cModel* AddRigidModel(const string &fileName);
		cModel* FindRigidModel(const int id);
		bool RemoveRigidModel(cModel *model, const bool destruct=true);
		bool RemoveRigidModel(const int id, const bool destruct=true);
		vector<cModel*>& GetRigidModels();

		virtual void Render();

		// info
		int GetRowCellCount() const;
		int GetColCellCount() const;
		float GetCellSize() const;
		float GetTerrainWidth() const;
		float GetTerrainHeight() const;
		const string& GetTextureName();
		float GetTextureUVFactor() const;
		float GetHeightFactor() const;
		const string& GetHeightMapFileName() const;

		// layer
		int GetLayerCount() const;
		const sSplatLayer& GetLayer(int layer) const;
		void DeleteLayer(int layer);
		cTexture& GetAlphaTexture();

		virtual void Clear();


	protected:
		virtual void RenderShader(cShader &shader);
		void RenderRigidModels();

		float GetHeightMapEntry( int row, int col );
		bool UpdateHeightMap( const string &heightMapFileName, 
			const string &textureFileName, const float heightFactor );

		DWORD GetAlphaMask(const int layer);

		// layer
		void InitLayer();
		sSplatLayer& GetTopLayer();
		bool AddLayer();


	protected:
		int m_rowCellCount;
		int m_colCellCount;
		float m_cellSize;
		float m_heightFactor;
		float m_textureUVFactor;
		string m_heightMapFileName;
		cGrid2 m_grid;
		cShader *m_shader; // reference

		// model
		vector<cModel*> m_rigids;
		bool m_isShowModel; // default : true

		// splatting layer
		vector<sSplatLayer> m_layer;
		cTexture m_alphaTexture;
		cTexture m_emptyTexture;
	};


	inline int cTerrain::GetRowCellCount() const { return m_rowCellCount; }
	inline int cTerrain::GetColCellCount() const { return m_colCellCount; }
	inline float cTerrain::GetCellSize() const { return m_cellSize; }
	inline float cTerrain::GetTerrainWidth() const { return m_colCellCount * m_cellSize; }
	inline float cTerrain::GetTerrainHeight() const { return m_rowCellCount * m_cellSize; }
	inline float cTerrain::GetTextureUVFactor() const { return m_textureUVFactor; }
	inline float cTerrain::GetHeightFactor() const { return m_heightFactor; }
	inline vector<cModel*>& cTerrain::GetRigidModels() { return m_rigids; }
	inline bool cTerrain::IsLoaded() const { return m_rowCellCount > 0; }
	inline int cTerrain::GetLayerCount() const { return m_layer.size(); }
	inline const sSplatLayer& cTerrain::GetLayer(int layer) const { return m_layer[ layer]; }
	inline cTexture& cTerrain::GetAlphaTexture() { return m_alphaTexture; }
	inline const string& cTerrain::GetHeightMapFileName() const { return m_heightMapFileName; }
}
