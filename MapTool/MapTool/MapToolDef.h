#pragma once


static const int VIEW_WIDTH = 800;
static const int VIEW_HEIGHT = 600;
static const int REAL_WINDOW_WIDTH = VIEW_WIDTH+18;
static const int REAL_WINDOW_HEIGHT = VIEW_HEIGHT+70;



namespace EDIT_MODE
{
	enum TYPE
	{
		MODE_HEIGHTMAP,
		MODE_TERRAIN,
		MODE_BRUSH,
		MODE_MODEL,
	};
}


namespace NOTIFY_TYPE
{
	enum TYPE
	{
		NOTIFY_CHANGE_TERRAIN = 0,
		NOTIFY_ADD_LAYER,
		NOTIFY_CHANGE_CURSOR,
		NOTIFY_ADD_PLACE_MODEL,
	};
}