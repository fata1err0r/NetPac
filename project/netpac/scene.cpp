#include "stdafx.h"
#include "scene.h"

void Scene::Init()
{
	pCamera = new CTVCamera();
	pScene = new CTVScene();
	pTexFactory = new CTVTextureFactory();
	pGlobals = new CTVGlobals();
	pText = new CTVScreen2DText();

	pCamera->SetViewFrustum( 60.f, 1000.f, 1.f );
	pCamera->SetPosition( 0.f, 0.f, 100.f );
	pCamera->SetLookAt( 0.f, 0.f, 0.f );


}