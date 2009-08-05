#include "stdafx.h"
#include "graphics.h"

void Graphics::Init()
{
	SDL_SetVideoMode( 800, 600, 32, SDL_HWSURFACE | SDL_RESIZABLE );
	SDL_WM_SetCaption("NetPac", "NetPac");
	SDL_WM_SetIcon(SDL_LoadBMP("Media/icon.bmp"), NULL);

	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);
	HWND hWnd = wmInfo.window;

	pEngine = new CTVEngine();
	pScene = new CTVScene();
	pCamera = new CTVCamera();
	pTextureFactory = new CTVTextureFactory();
	pLightEngine = new CTVLightEngine();
	pMaterialFactory = new CTVMaterialFactory();
	pMathLibrary = new CTVMathLibrary();
	pScreen2DText = new CTVScreen2DText();

	pEngine->SetSearchDirectory( "./" );
	pEngine->SetSearchDirectory( "./media/" );
	pEngine->SetDebugFile( "Debug.txt" );
	pEngine->SetDebugMode( true, false );
	// register our beta key, so true vision will run
	pEngine->SetBetaKey("PhunkeyMonkey", "6F7F499C-E426526F-2178630E-68283824");
	// Init the TV renderer and pass it the handle to the window
	if( !pEngine->Init3DWindowed( hWnd, true ) )
	{
		MessageBox( NULL, L"Init TrueVision Engine Failed", L"ERROR", MB_OK | MB_ICONEXCLAMATION );
	}
	pEngine->SetWatermarkParameters(cTV_WATERMARK_BOTTOMRIGHT, 10000000);
	// set misc values for the TV engine
	pEngine->GetViewport()->SetAutoResize( true );
	pEngine->DisplayFPS( true );
	pEngine->SetVSync(true);
	pEngine->SetAngleSystem( cTV_ANGLE_DEGREE );

	pCamera->SetViewFrustum( 60.f, 100000.f, 1.f );
	//pCamera->SetViewIsometric(40, 100000);
	pCamera->SetPosition( 9.f, 11.f, -21.f );
	pCamera->SetLookAt( 9.f, 11.f, 0.f );
	
}