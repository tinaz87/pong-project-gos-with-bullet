#pragma once
#include "d3dx9core.h"
#include "ObjectProperty.h"
#include "Containers.h"



class InterfaceRectangle{

public :
	InterfaceRectangle(){};

	InterfaceRectangle(const LONG x,const LONG y,const LONG width,const LONG height){

		m_rectangle.left = x;
		m_rectangle.top = y;
		m_rectangle.right = x + width;
		m_rectangle.bottom = y + height;
	}

	const RECT& getRectangle() const{

		return m_rectangle;
	}

	RECT& editRectangle(){

		return m_rectangle;
	}
private:

	RECT m_rectangle;

};

struct GfxInterfaceText{

	InterfaceRectangle rect;
	std::string text;
	bool active;

};

typedef MvMap<ObjectId,GfxInterfaceText*> GfxTextMap;
typedef MvMap<ObjectId,GfxInterfaceText*>::iterator GfxTextMapIterator;
typedef MvMap<ObjectId,GfxInterfaceText*>::const_iterator GfxTextMapConstIterator;

class GfxInterface: public ObjectProperty
{

public:



	static const StringHash INTERFACE_PROPERTY_ID;
	static const StringHash INTERFACE_PROPERTY_OBJ_ID;

	GfxInterface();
	~GfxInterface();

	

	


	void initializeText(LPDIRECT3DDEVICE9 m_pd3dDevice) const;

	void displayText() const;

	const GfxInterfaceText* getText(ObjectId& id) const;

	GfxInterfaceText* editText(ObjectId& id);

	void addText(const ObjectId& id,GfxInterfaceText* text );


private:	

	mutable LPD3DXFONT m_font;

	mutable D3DCOLOR fontColor;

	mutable RECT rct;

	GfxTextMap textMap;

	
};