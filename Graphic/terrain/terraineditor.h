#pragma once


namespace graphic
{

	struct sSplatLayer
	{
		int layer;
		cTexture *texture;
	};


	class cTerrainEditor : public cTerrain
	{
		enum {
			MAX_LAYER = 4,
			ALPHA_TEXTURE_SIZE_W = 200,
			ALPHA_TEXTURE_SIZE_H = 200,
		};

	public:
		cTerrainEditor();
		virtual ~cTerrainEditor();

		bool LoadTerrain( const string &fileName );
		void GetRawTerrain( OUT sRawTerrain &out );

		void Brush( const cTerrainCursor &cursor );
		int GetSplatLayerCount() const;
		const sSplatLayer& GetSplatLayer(int layer) const;
		void SetHeightFactor(const float heightFactor);
		void SetTextureUVFactor(const float textureUVFactor);

		virtual void Render() override;
		virtual void RenderShader(cShader &shader) override;

		virtual void Clear() override;


	protected:
		void InitLayer();
		sSplatLayer& GetTopLayer();
		bool AddLayer();
		void GetTextureUV(const Vector3 &pos, OUT float &tu, OUT float &tv);


	private:
		sSplatLayer m_layer[ MAX_LAYER];
		int m_numLayer;
		cTexture m_alphaTexture;
		cTexture m_emptyTexture;
	};


	inline int cTerrainEditor::GetSplatLayerCount() const { return m_numLayer; }
	inline const sSplatLayer& cTerrainEditor::GetSplatLayer(int layer) const { return m_layer[ layer]; }
}
