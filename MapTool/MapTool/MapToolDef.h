#pragma once



//const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
//const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;
static const int REAL_WINDOW_WIDTH = WINDOW_WIDTH+18;
static const int REAL_WINDOW_HEIGHT = WINDOW_HEIGHT+115;



namespace EDIT_MODE
{
	enum TYPE
	{
		MODE_HEIGHTMAP,
		MODE_TERRAIN,
		MODE_BRUSH,
	};
}

