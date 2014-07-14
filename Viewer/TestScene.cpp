
#include "stdafx.h"
#include "TestScene.h"



cTestScene::cTestScene(LPD3DXSPRITE sprite) :
	framework::cWindow(sprite, 0, "testScene")
{
	Create("The-Waters-Edge.jpg");

	EventConnect(this, framework::EVENT::BUTTON_CLICK, 1, (framework::EventFunction)&cTestScene::Button1Click);
}

cTestScene::~cTestScene()
{

}


void cTestScene::Button1Click(framework::cEvent &event)
{

}
