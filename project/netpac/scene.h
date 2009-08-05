#ifndef DSCENE_H
#define DSCENE_H

#include "singleton.h"
#include <CTVCamera.h>
#include <CTVTextureFactory.h>
#include <CTVScene.h>
#include <CTVScreen2DText.h>


class Scene : public Singleton<Scene>
{
	friend Singleton;
	
public:
	void Init();
	CTVScene* GetScene() { return pScene; }
	CTVCamera* GetCamera() { return pCamera; }
	CTVTextureFactory* GetTextureFactory() { return pTexFactory; }
	CTVGlobals*	GetGlobals() { return pGlobals; }
	CTVScreen2DText* GetText() { return	pText; }
private:
	CTVScene			*pScene;
	CTVCamera			*pCamera;
	CTVTextureFactory	*pTexFactory;
	CTVGlobals			*pGlobals;
	CTVScreen2DText		*pText;
protected:
	Scene(){}
};

#endif