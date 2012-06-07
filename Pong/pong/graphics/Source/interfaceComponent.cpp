#include "GameObjectSystem.h"
#include "interfaceComponent.h"
#include "ScoreManager.h"

const StringHash GfxInterface::INTERFACE_PROPERTY_ID= StringHash("InterfaceProperty");
const StringHash GfxInterface::INTERFACE_PROPERTY_OBJ_ID= StringHash("InterfaceObjProperty");

GfxInterface::GfxInterface():ObjectProperty(INTERFACE_PROPERTY_ID,INTERFACE_PROPERTY_OBJ_ID)
													
{
	
}

const GfxInterfaceText* GfxInterface::getText(ObjectId& id)const{

	GfxTextMapConstIterator it = textMap.find(id);

	if (it != textMap.cend())
	{
		return it->second;
	}

	return nullptr;

}



GfxInterfaceText* GfxInterface::editText(ObjectId& id){

	GfxTextMapIterator it = textMap.find(id);

	if (it != textMap.cend())
	{
		return it->second;
	}

	return nullptr;

}

void GfxInterface::addText(const ObjectId& id,GfxInterfaceText* text ){
	
	textMap[id] = text;
}

void GfxInterface::initializeText(LPDIRECT3DDEVICE9 m_pd3dDevice)  const{
	
	D3DXCreateFont( m_pd3dDevice, 26 , 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_font );
	
}

void GfxInterface::displayText() const
{

	// Create a rectangle to indicate where on the screen it should be drawn

	WCHAR sz[100];
	for (GfxTextMapConstIterator it = textMap.begin(); it != textMap.end();++it)
	{

		GfxInterfaceText* text = it->second;

		if (text->active)
		{
			mbstowcs(sz,(text->text).c_str(),100);
			m_font->DrawText(NULL,sz,-1,&(text->rect.editRectangle()),0,text->fontColor);
		}

	}
}

GfxInterface::~GfxInterface(){

	for (GfxTextMapIterator it = textMap.begin(); it != textMap.end();++it)
	{
		MV_DELETE(it->second);

	}

	m_font->Release();

	textMap.clear();
}