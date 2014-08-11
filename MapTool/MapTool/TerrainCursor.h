#pragma once


class cTerrainCursor
{
	enum {
		CURSOR_VERTEX_COUNT = 64,
	};

public:
	cTerrainCursor(void);
	virtual ~cTerrainCursor(void);

	void Render();
	void UpdateCursor( graphic::cTerrain &terrain, const Vector3 &cursorPos);

	int GetInnerBrushRadius() const;
	int GetOuterBrushRadius() const;
	float GetInnerBrushAlpha() const;
	void SetInnerBrushRadius(int radius);
	void SetOuterBrushRadius(int radius);
	void SetInnerBrushAlpha(float alpha);


private:
	Vector3 m_pos;
	float m_innerRadius;
	float m_outerRadius;
	float m_innerAlpha;
	graphic::cVertexBuffer m_innerCircle;
	graphic::cVertexBuffer m_outerCircle;
};


inline int cTerrainCursor::GetInnerBrushRadius() const { return m_innerRadius; }
inline int cTerrainCursor::GetOuterBrushRadius() const { return m_outerRadius; }
inline void cTerrainCursor::SetInnerBrushRadius(int radius) { m_innerRadius = radius; }
inline void cTerrainCursor::SetOuterBrushRadius(int radius) { m_outerRadius = radius; }
inline float cTerrainCursor::GetInnerBrushAlpha() const { return m_innerAlpha; }
inline void cTerrainCursor::SetInnerBrushAlpha(float alpha) { m_innerAlpha = alpha; }
