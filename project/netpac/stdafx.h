// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// #define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;


//SDL includes
#include "SDL/SDL.h"
#include "SDL/SDL_syswm.h"
#include "SDL/SDL_net.h"


// TrueVision 3D includes
#include <CTVActor.h>
#include <CTVAI.h>
#include <CTVAtmosphere.h>
#include <CTVBitmapParts.h>
#include <CTVCameraFactory.h>
#include <CTVCollisionResult.h>
#include <CTVDeviceInfo.h>
#include <CTVGameController.h>
#include <CTVGameControllers.h>
#include <CTVGlobals.h>
#include <CTVGraphicEffect.h>
#include <CTVInputEngine.h>
#include <CTVInternalObjects.h>
#include <CTVLandscape.h>
#include <CTVMathLibrary.h>
#include <CTVMesh.h>
#include <CTVMiniMesh.h>
#include <CTVNode.h>
#include <CTVOctree.h>
#include <CTVParticleSystem.h>
#include <CTVPath.h>
#include <CTVPhysics.h>
#include <CTVRenderSurface.h>
#include <CTVScreen2DImmediate.h>
#include <CTVShader.h>
#include <CTVTileMap.h>
#include <CTVViewport.h>
#include <HelperFunctions.h>
#include <tv_types.h>


#include "singleton.h"
#include "player.h"
#include "level.h"
#include "graphics.h"
#include "network.h"
#include "sound.h"