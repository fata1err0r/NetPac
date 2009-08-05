#ifndef LEVEL_H
#define LEVEL_H

#include "singleton.h"
#include "graphics.h"
#include <fstream>

//info on where the dots/pills are in the level and if to render them
typedef struct 
{
	cTV_2DVECTOR pos;
	bool isPill;
	bool render;
} DotBuffer;

typedef struct
{
	char cLevelBuf[20][20];
	string sLevelName;
} LevelData;

class Level : public Singleton<Level>
{
	friend Singleton;
public:
	void Init();
	void LoadLevels();
	void StartLevel(int no);
	void Update();
	void AddPlayer(ePlayerType pt, int id);
	bool IsBrick(int x, int y);
	Player* GetPlayer(int i);
	void SetPacman(int playerno);
	cTV_2DVECTOR GetGhostSpawn();
	cTV_2DVECTOR GetGhostStart();
	cTV_2DVECTOR GetPacmanStart();
	cTV_2DVECTOR GetPacmanPos();
	cTV_2DVECTOR GetNextDotPos(cTV_2DVECTOR PlayerPos);
	cTV_2DVECTOR GetClosestGhost(int playerno);
	void EatPill(int no);
	void SendEatenPills(int playerno);
	void PacAlert();
	void PacNormal();
	bool bIsServer;
	void RestartLevel();
	bool bPacAlert;
	void LevelFinish();
	int iCurrentLevel;
	bool bLevelFinish, bServerQuit;
private:
	char cLevelBuf[20][20];
	CTVMesh *MeshBox;
	CTVActor *MeshPacman, *MeshGhost, *MeshArrow, *MeshPill, *MeshDot, *MeshSpawn;
	vector<Player *> PlayerList;
	vector<Player *>::iterator PlayerIter;
	vector<DotBuffer *> DotList;
	vector<DotBuffer *>::iterator DotIter;
	vector<LevelData *> LevelList;
	vector<LevelData *>::iterator LevelIter;
	int iPacmanPlayer; 
	int iDotCount, iTotalLevels;
	cTV_2DVECTOR GhostSpawn,GhostStart,PacmanStart;
	float fTickCount, fTickCount2, fTickCount3;
	int iYellowMaterial, iRedMaterial, iGreenMaterial, iBlueMaterial, iWhiteMaterial;
	string sWinner;
	int iWinner;
	string sLevelName;
	bool bChaseCam;

protected:
	Level(){};

};

#define DLevel Level::Instance()
#endif