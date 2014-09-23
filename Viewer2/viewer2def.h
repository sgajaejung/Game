// Viewer2 에 쓰이는 공용 타입들을 정의 한다.

#pragma once


const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
//const int WINDOW_WIDTH = 800;
//const int WINDOW_HEIGHT = 600;

const int REAL_WINDOW_WIDTH = WINDOW_WIDTH+18;
const int REAL_WINDOW_HEIGHT = WINDOW_HEIGHT+115;



namespace DISP_MODE
{
	enum TYPE
	{
		DISP_800X600_RIGHT,
		DISP_1024X768_RIGHT,
		DISP_800X600_LEFT,
		DISP_1024X768_LEFT,
	};
}


namespace EDIT_MODE
{
	enum TYPE
	{
		FILE,
		MODEL,
		ANIMATION,
		RENDERER,
		LIGHT,
		ARCHEBLADE,
		TERA,
	};
}


namespace NOTIFY_MSG
{
	enum TYPE
	{
		UPDATE_MODEL,
		CHANGE_PANEL,
		UPDATE_LIGHT_DIRECTION,
	};
}