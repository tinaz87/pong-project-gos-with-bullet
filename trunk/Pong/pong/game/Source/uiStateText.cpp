#include "gfxFont.h"
#include "uiStateText.h"
#include "GameObjectSystem.h"


uiStateText::uiStateText(void){

	GfxFont* font = MV_NEW GfxFont(ObjectId("CENTER_MAIN_ID"),"Arial",48,"MAIN",GfxColor(255,0,255,255),GfxRectangle(280,325,500,200),false);

	GameObjectSystem::GetSingleton().addProperty(font);

	font = MV_NEW GfxFont(ObjectId("CENTER_SECOND_ID"),"Arial",26,"INFO",GfxColor(255,255,0,255),GfxRectangle(300,455,300,200),false);

	GameObjectSystem::GetSingleton().addProperty(font);

	font = MV_NEW GfxFont(ObjectId("SCORE_SX"),"Arial",32,"--",GfxColor(255,255,0,0),GfxRectangle(20,20,200,200),false);

	GameObjectSystem::GetSingleton().addProperty(font);

	font = MV_NEW GfxFont( ObjectId("SCORE_DX"),"Arial",32,"--",GfxColor(255,0,255,0),GfxRectangle(670,20,200,200),false);

	GameObjectSystem::GetSingleton().addProperty(font);


}