// pong.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "pong.h"
#include "GameObjectSystem.h"
#include "KeyboardInputManager.h"
#include "positionControllerProperty.h"
#include "background.h"
#include "bumper.h"
#include "ball.h"
#include "graphicsComponent.h"
#include "interfaceComponent.h"
#include "physicsComponent.h"
#include "gameRulesComponent.h"
#include "PositionControllerComponent.h"
#include <strsafe.h>
#include "CustomMemory.h"
#include "graphicsDebugger.h"
#include "ScoreManager.h"
#include "ComboControllerProperty.h"
#include "SoundComponent.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

#include <string>

#include <glog/logging.h>

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void InitGlog()
{
	// Initialize Google's logging library. See http://google-glog.googlecode.com/svn/trunk/doc/glog.html
	google::SetLogDestination(google::GLOG_INFO,	"./logs/info.log");
	google::SetLogDestination(google::GLOG_WARNING, "./logs/warning.log");
	google::SetLogDestination(google::GLOG_FATAL,	"./logs/fatal.log");
	google::SetStderrLogging(google::GLOG_WARNING);

	google::InitGoogleLogging("");
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	
	//init systems
	allocator::CreateInstance();
	InitGlog();
	GameObjectSystem::CreateInstance(10 /*maxObjectsCount*/, 4 /*maxPhaseCount*/);
	KeyboardInputManager::CreateInstance();
	

	MSG msg;
	ZeroMemory( &msg, sizeof( msg ) );
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PONG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PONG));

	static const real TIMESTEP= 1.f/60.f;
	LARGE_INTEGER frequency;			// ticks per second
	LARGE_INTEGER currentTime, newTime; // ticks
	real frametime= 0.f;

	// get ticks per second
	QueryPerformanceFrequency(&frequency);
	real freq= 1.f/frequency.QuadPart;
	QueryPerformanceCounter(&currentTime);

	PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE);
	while (msg.message!=WM_QUIT) 
	{
		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			if(!KeyboardInputManager::GetSingleton().handleOsInputEvent(msg))
			{
				DispatchMessage(&msg);
			}
		}

		{
			QueryPerformanceCounter(&newTime);
			frametime= (newTime.QuadPart - currentTime.QuadPart) * freq; //sec
			
			LOG_EVERY_N(INFO, 60) << "fps" << 1.f/frametime;
			
			currentTime= newTime;
			GameObjectSystem::GetSingleton().update(frametime, TIMESTEP);
		}
	}
	GameObjectSystem::Release();
	KeyboardInputManager::Release();
	ScoreManager::Release();
#ifdef _DEBUG
	StringHash::clearDebug();
#endif
	allocator::Release();
	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PONG));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PONG);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_EX_TOPMOST,
      CW_USEDEFAULT, 0, 800, 800, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //create objects
   Background background("test");
   Bumper bumperPlayer1("bump1", vector3(-90.f, 0.f, 0.f), true);
   //create controller
   PositionControllerProperty* controller= MV_NEW PositionControllerProperty("bump1", 'A', 'Z');
   ComboControllerProperty* combo = MV_NEW ComboControllerProperty("bump1",'E','D');
   GameObjectSystem::GetSingleton().addProperty(controller);
   GameObjectSystem::GetSingleton().addProperty(combo);

   Bumper bumperPlayer2("bump2", vector3(90.f, 0.f, 0.f), true);
   PositionControllerProperty* controller2= MV_NEW PositionControllerProperty("bump2", 'O', 'L');
   ComboControllerProperty* combo2 = MV_NEW ComboControllerProperty("bump2",'E','D');
   GameObjectSystem::GetSingleton().addProperty(controller2);
   GameObjectSystem::GetSingleton().addProperty(combo2);


   Ball ball("ball1", vector3(0.f, 0.f, 0.5f));

   ScoreManager::CreateInstance(ObjectId("ball1"),ObjectId("wall2"),ObjectId("wall1"));

#ifdef _DEBUG
   GraphicsDebugger* gfxDebugger= MV_NEW GraphicsDebugger();
   GameObjectSystem::GetSingleton().addProperty(gfxDebugger);
#endif

   // Add interface property
   GfxInterface* gfxInterface = MV_NEW GfxInterface();
   GameObjectSystem::GetSingleton().addProperty(gfxInterface);
  

   //create components 
   PositionControllerComponent* positionControllerComponent= MV_NEW PositionControllerComponent();
   PhysicsComponent* physicsComponent= MV_NEW PhysicsComponent(1, 1);
   GraphicsComponent* graphicsComponent= MV_NEW GraphicsComponent(hWnd);
  
   //Sound Component
   //SoundComponent*	soundComponent = MV_NEW SoundComponent();
   
   GameRulesComponent* gameRulesComponent= MV_NEW GameRulesComponent();


  
   ScoreManager::GetSingleton().setCollisionPublisher(&physicsComponent->getCollisionPublisher());
   //soundComponent->setCollisionPublisher(&physicsComponent->getCollisionPublisher());
   gameRulesComponent->setScorePublisher(&(ScoreManager::GetSingletonPtr()->getScorePublisher()));
  

   GameObjectSystem::GetSingleton().addComponent(positionControllerComponent);
   GameObjectSystem::GetSingleton().addComponent(physicsComponent);
   GameObjectSystem::GetSingleton().addComponent(graphicsComponent);

   //GameObjectSystem::GetSingleton().addComponent(soundComponent);

   GameObjectSystem::GetSingleton().addComponent(gameRulesComponent);

   // Initialize Direct3D
   //create graphics component here
   // Create the vertex buffer
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_KILLFOCUS:
		KeyboardInputManager::GetSingleton().reset();
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		//release systems
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
