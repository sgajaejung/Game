#pragma once


namespace graphic
{

	struct sSplatLayer
	{
		cTexture *texture;	
		sSplatLayer() : texture(NULL) {}
	};


	class cTerrainEditor : public cTerrain
	{
		enum {
			MAX_LAYER = 4,
			ALPHA_TEXTURE_SIZE_W = 256,
			ALPHA_TEXTURE_SIZE_H = 256,
		};

	public:
		cTerrainEditor();
		virtual ~cTerrainEditor();

		bool SaveTerrain( const string &fileName );
		bool LoadTerrain( const string &fileName );
		bool LoadTerrain( const sRawTerrain &rawTerrain );
		void GenerateRawTerrain( OUT sRawTerrain &out );

		void BrushTerrain( const cTerrainCursor &cursor, const float elapseT );
		void BrushTexture( const cTerrainCursor &cursor );
		int GetLayerCount() const;
		const sSplatLayer& GetLayer(int layer) const;
		void DeleteLayer(int layer);

		void SetHeightFactor(const float heightFactor);
		void SetTextureUVFactor(const float textureUVFactor);
		cTexture& GetAlphaTexture();

		virtual void Render() override;
		virtual void RenderShader(cShader &shader) override;

		virtual void Clear() override;


	protected:
		void InitLayer();
		sSplatLayer& GetTopLayer();
		bool AddLayer();
		void GetTextureUV(const Vector3 &pos, OUT float &tu, OUT float &tv);
		DWORD GetAlphaMask(const int layer);


	private:
		vector<sSplatLayer> m_layer;
		cTexture m_alphaTexture;
		cTexture m_emptyTexture;
	};


	inline int cTerrainEditor::GetLayerCount() const { return m_layer.size(); }
	inline const sSplatLayer& cTerrainEditor::GetLayer(int layer) const { return m_layer[ layer]; }
	inline cTexture& cTerrainEditor::GetAlphaTexture() { return m_alphaTexture; }
}
