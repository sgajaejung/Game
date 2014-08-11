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


private:
	Vector3 m_pos;
	float m_innerRadius;
	float m_outerRadius;
	graphic::cVertexBuffer m_innerCircle;
	graphic::cVertexBuffer m_outerCircle;
};

