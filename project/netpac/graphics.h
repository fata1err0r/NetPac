#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "singleton.h"
#include <CTVEngine.h>
#include <CTVScene.h>
#include <CTVTextureFactory.h>
#include <CTVLightEngine.h>
#include <CTVMaterialFactory.h>
#include <CTVMathLibrary.h>
#include <CTVScreen2DText.h>

class Graphics : public Singleton<Graphics>
{
	friend Singleton;
public:
	void Init();
	CTVEngine* GetEngine(){return pEngine;}
	CTVScene* GetScene(){return pScene;}
	CTVCamera* GetCamera() {return pCamera;}
	CTVTextureFactory* GetTextureFactory() {return pTextureFactory;}
	CTVLightEngine* GetLightEngine() {return pLightEngine;}
	CTVMaterialFactory* GetMaterialFactory() {return pMaterialFactory;}
	CTVMathLibrary* GetMathLibrary() {return pMathLibrary; }
	CTVScreen2DText* GetScreen2DText() {return pScreen2DText;}
private:
	CTVEngine *pEngine;
	CTVScene *pScene;
	CTVCamera *pCamera;
	CTVTextureFactory *pTextureFactory;
	CTVLightEngine *pLightEngine;
	CTVMaterialFactory *pMaterialFactory;
	CTVMathLibrary *pMathLibrary;
	CTVScreen2DText *pScreen2DText;
protected:
	Graphics(){};

};

#define DGraphics Graphics::Instance()

#endif