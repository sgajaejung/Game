#pragma once


namespace graphic
{

	class cTerrainCursor
	{
		enum {
			CURSOR_VERTEX_COUNT = 64,
		};

	public:
		cTerrainCursor(void);
		virtual ~cTerrainCursor(void);

		void RenderBrush();
		void RenderModel();
		void UpdateCursor( graphic::cTerrain &terrain, const Vector3 &cursorPos);

		int GetInnerBrushRadius() const;
		int GetOuterBrushRadius() const;
		float GetInnerBrushAlpha() const;
		void SetInnerBrushRadius(int radius);
		void SetOuterBrushRadius(int radius);
		void SetInnerBrushAlpha(float alpha);
		const Vector3& GetCursorPos() const;

		// brush
		void SelectBrushTexture(const string &fileName );
		const cTexture* GetBrushTexture() const;

		// model
		void SelectModel(const string &fileName);
		void SelectModel(cModel *model);
		const cModel* GetSelectModel() const;
		void CancelSelectModel();
		bool IsSelectModel() const;


	private:
		Vector3 m_pos;
		
		// brush
		float m_innerRadius;
		float m_outerRadius;
		float m_innerAlpha;
		cVertexBuffer m_innerCircle;
		cVertexBuffer m_outerCircle;
		cTexture *m_brushTexture; // reference

		// model
		cModel *m_selectModel;
	};


	inline int cTerrainCursor::GetInnerBrushRadius() const { return m_innerRadius; }
	inline int cTerrainCursor::GetOuterBrushRadius() const { return m_outerRadius; }
	inline void cTerrainCursor::SetInnerBrushRadius(int radius) { m_innerRadius = radius; }
	inline void cTerrainCursor::SetOuterBrushRadius(int radius) { m_outerRadius = radius; }
	inline float cTerrainCursor::GetInnerBrushAlpha() const { return m_innerAlpha; }
	inline void cTerrainCursor::SetInnerBrushAlpha(float alpha) { m_innerAlpha = alpha; }
	inline const cTexture* cTerrainCursor::GetBrushTexture() const { return m_brushTexture; }
	inline const Vector3& cTerrainCursor::GetCursorPos() const { return m_pos; }
	inline const cModel* cTerrainCursor::GetSelectModel() const { return m_selectModel; }
	inline bool cTerrainCursor::IsSelectModel() const { return m_selectModel? true : false; }
	inline void cTerrainCursor::SelectModel(cModel *model) { m_selectModel = model; }
}

