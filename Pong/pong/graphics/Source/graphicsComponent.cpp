#include "graphicsComponent.h"
#include "GameObjectSystem.h"
#include "gfxGeometry.h"
#include "gfxTexture.h"
#include "position.h"
#include <glog/logging.h>
#include "graphicsDebugger.h"
#include "physicsBody.h"
#include "KeyboardInputManager.h"

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

const StringHash GraphicsComponent::GRAPHICS_COMPONENT_ID= StringHash("GraphicsComponent");

GraphicsComponent::GraphicsComponent(HWND hWnd)
	:Component(GRAPHICS_COMPONENT_ID, 2)
	,m_pD3D(NULL)
	,m_pd3dDevice(NULL)
	,m_graphicsDebugger(NULL)
{
	// Create the D3D object.
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		DLOG(FATAL) << "error initializing directx3d";
	}

	// Set up the structure used to create the D3DDevice
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.Windowed= TRUE;
	d3dpp.SwapEffect= D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat= D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval= D3DPRESENT_INTERVAL_DEFAULT;

	//ZBuffer 1 Setting the Appropriate Presentation Parameters
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									  &d3dpp, &m_pd3dDevice ) ) )
	{
		DLOG(FATAL) << "error creating directx device";
	}

	//ZBuffer 2 Turning On Z-Buffering
	m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false); // disable lightning.. important to see line color

	//enable alpha
	
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)50);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, 1);

	m_textureManager= MV_NEW TextureMgr(m_pd3dDevice);
	GameObjectSystem& gameObjectSystem= GameObjectSystem::GetSingleton();
	const ObjectPropertyTable* gfxGeometryMap =  gameObjectSystem.getProperties(GfxGeometry::GFX_GEOMETRY_ID);
	if(gfxGeometryMap!=NULL)
	{
		for(ObjectPropertyTable::const_iterator opIt= gfxGeometryMap->begin(); opIt != gfxGeometryMap->end(); ++opIt)
		{
			const GfxGeometry* gfxGeometry= static_cast<const GfxGeometry*>(opIt->second);
		
			//Create texture
			HTexture texture;
			const ObjectProperty* objProperty= gameObjectSystem.getProperty(GfxTexture::GFX_TEXTURE_ID, gfxGeometry->getObjectId());
			if(objProperty != NULL)
			{
				const GfxTexture* gfxTexture= static_cast<const GfxTexture*>(objProperty);
				texture= m_textureManager->getCreateTexture(gfxTexture->getTextureName());
			}
			m_pTextureB.push_back(texture);

			//Get position
			const Position* position= NULL;
			objProperty= gameObjectSystem.getProperty(Position::POSITION_ID, gfxGeometry->getObjectId());
			if(objProperty != NULL)
			{
				position= static_cast<const Position*>(objProperty);
			}			
			m_position.push_back(position);

			//Create vertex buffer
			VERTEXTEX* vertices= MV_NEW VERTEXTEX[gfxGeometry->getVerticesCount()];
			for(uint32 v= 0; v < gfxGeometry->getVerticesCount(); ++v)
			{
				const vector4& vertex= gfxGeometry->getVertices()[v];
				vertices[v].x= XMVectorGetX(vertex);
				vertices[v].y= XMVectorGetY(vertex);
				vertices[v].z= XMVectorGetZ(vertex);
				vertices[v].color= 0xff000000;
				vertices[v].tu= gfxGeometry->getTextureCoord()[v].x;
				vertices[v].tv= gfxGeometry->getTextureCoord()[v].y;
			}
		
			LPDIRECT3DVERTEXBUFFER9 vertexBuffer= NULL;
			if( FAILED( m_pd3dDevice->CreateVertexBuffer( gfxGeometry->getVerticesCount() * sizeof( *vertices ),
														  0, D3DFVF_CUSTOMVERTEX,
														  D3DPOOL_DEFAULT, &vertexBuffer, NULL ) ) )
			{
				DLOG(FATAL) << "error creating vertex buffer";
			}

			VOID* pVertices;
			if( FAILED( vertexBuffer->Lock( 0, sizeof( *vertices )*gfxGeometry->getVerticesCount(), ( void** )&pVertices, 0 ) ) )	//lock buffer
			{
				DLOG(FATAL) << "error copying vertices";
			}
			memcpy( pVertices, vertices, sizeof( *vertices )*gfxGeometry->getVerticesCount() );		//copy data
			vertexBuffer->Unlock();		//unlock buffer
			m_verticesCount.push_back(gfxGeometry->getVerticesCount());
			m_pVB.push_back(vertexBuffer);
			MV_DELETE(vertices);

			//create index buffer
			const uint32* indices= gfxGeometry->getIndices();
			LPDIRECT3DINDEXBUFFER9 indexBuffer= NULL;
			if( FAILED( m_pd3dDevice->CreateIndexBuffer(sizeof(uint32)*gfxGeometry->getIndicesCount(), D3DUSAGE_WRITEONLY,
														D3DFMT_INDEX32, D3DPOOL_MANAGED, &indexBuffer, NULL)))
			{
				DLOG(FATAL) << "error creating index buffer";
			}
		
			VOID* pIndices;
			if( FAILED(indexBuffer->Lock(0, sizeof(uint32)*gfxGeometry->getIndicesCount(),(void**)&pIndices,0)))	//lock buffer
			{
				DLOG(FATAL) << "error copying indices";
			}
			memcpy(pIndices, indices, sizeof(uint32)*gfxGeometry->getIndicesCount());							//copy data
			indexBuffer->Unlock();																				//unlock buffer
			m_primitivesCount.push_back(gfxGeometry->getIndicesCount()/3);
			m_pIB.push_back(indexBuffer);
		}
	}

	//add graphics debugger
	ObjectProperty* debugGfxProperty= gameObjectSystem.editProperty(GraphicsDebugger::GRAPHICS_DEBUGGER_ID, GraphicsDebugger::GRAPHICS_DEBUGGER_OBJ_ID);
	if(debugGfxProperty!=NULL)
	{
		m_graphicsDebugger= static_cast<GraphicsDebugger*>(debugGfxProperty);
	}
}

GraphicsComponent::~GraphicsComponent()
{
	m_pTextureB.clear();
	MV_DELETE(m_textureManager); //unload all textures
	while(!m_pVB.empty())
	{
		LPDIRECT3DVERTEXBUFFER9 vertexBuffer= m_pVB.back();
		vertexBuffer->Release();
		m_pVB.pop_back();
	}

	while(!m_pIB.empty())
	{
		LPDIRECT3DINDEXBUFFER9 indexBuffer= m_pIB.back();
		indexBuffer->Release();
		m_pIB.pop_back();
	}

	if( m_pd3dDevice != NULL )
		m_pd3dDevice->Release();

	if( m_pD3D != NULL )
		m_pD3D->Release();
}

void GraphicsComponent::update(real frametime, real timestep)
{
	// Clear the backbuffer to a blue color
 	m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 255, 255 ), 1.0f, 0 );
	//ZBuffer 3: Clearing the Z-Buffer
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
 	
	// Begin the scene
 	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
 	{
		setupCamera();

		if(m_graphicsDebugger!= NULL)
		{
			m_graphicsDebugger->drawDebug(m_pd3dDevice);
		}
 		// Draw the triangles in the vertex buffer. This is broken into a few
		// steps. We are passing the Vertices down a "stream", so first we need
		// to specify the source of that stream, which is our vertex buffer. Then
		// we need to let D3D know what vertex shader to use. Full, custom vertex
		// shaders are an advanced topic, but in most cases the vertex shader is
		// just the FVF, so that D3D knows what type of Vertices we are dealing
		// with. Finally, we call DrawPrimitive() which does the actual rendering
		// of our geometry (in this case, just one triangle).
		for(uint32 i= 0; i < m_pVB.size(); ++i)
		{
			uint32 numVertices= m_verticesCount[i];
			uint32 primitiveCount= m_primitivesCount[i];
			
			const Position* position= m_position[i];
			setPosition(position);

			if(!m_pTextureB[i].isNull())
			{
				m_pd3dDevice->SetTexture( 0, m_textureManager->getTexture(m_pTextureB[i]) );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
				m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			}
			else
			{
				m_pd3dDevice->SetTexture( 0, NULL);
			}

 			m_pd3dDevice->SetStreamSource( 0, m_pVB[i], 0, sizeof( VERTEXTEX ) );
 			m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
			m_pd3dDevice->SetIndices(m_pIB[i]);
			m_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST,	//primitive type 
												0,					//base vertex index
												0,					//min vertex index
												numVertices,		//num vertices
												0,					//start index
												primitiveCount);	//primitive count
		}
		
 		// End the scene
 		m_pd3dDevice->EndScene();
 	}
 
 	// Present the backbuffer contents to the display
 	m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void GraphicsComponent::setPosition(const Position* position)
{
	matrix matWorld= MAT_IDENTITY;
	if(position != NULL)
	{
		matWorld= position->getPosition();
	}
	m_pd3dDevice->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)(&matWorld));

}

void GraphicsComponent::setupCamera()
{
	// For our world matrix, we will just rotate the object about the y-axis.
// 	D3DXMATRIXA16 matWorld;
// 
// 	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
// 	// every 1000 ms. To avoid the loss of precision inherent in very high 
// 	// floating point numbers, the system time is modulated by the rotation 
// 	// period before conversion to a radian angle.
// 	UINT iTime = timeGetTime() % 1000;
// 	FLOAT fAngle = iTime * ( 2.0f * D3DX_PI ) / 1000.0f;
// 	D3DXMatrixRotationY( &matWorld, fAngle );
// 	m_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up
	static real tx= 0.f;
	static real ty= 0.f;

// 	if(KeyboardInputManager::GetSingleton().getKeyState('L')>0)
// 		tx+= 1.f;
// 	if(KeyboardInputManager::GetSingleton().getKeyState('J')>0)
// 		tx-= 1.f;
// 	if(KeyboardInputManager::GetSingleton().getKeyState('I')>0)
// 		ty+= 1.f;
// 	if(KeyboardInputManager::GetSingleton().getKeyState('K')>0)
// 		ty-= 1.f;

	vector3 vEyePt( tx, ty, 250.f );
	static vector3 vLookatPt( 0.0f, 0.0f, 0.0f );
	static vector3 vUpVec( 0.0f, -1.0f, 0.0f );
	matrix matView= XMMatrixLookAtLH( XMLoadFloat3(&vEyePt), XMLoadFloat3(&vLookatPt), XMLoadFloat3(&vUpVec) );
	m_pd3dDevice->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)(&matView));

	// For the projection matrix, we set up an orthographic transform
  	D3DXMATRIXA16 matProj;
	D3DXMatrixOrthoLH(&matProj, 200.f, 200.f, 1.f, 560.f); //Zn and Zf in camera space
  	m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}