#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "input.h"

typedef enum
{
	PT_LOCAL,
	PT_SERVER,
	PT_NET,
	PT_AI,
} ePlayerType;

class Player
{
public:
	Player(CTVActor *MPacman, CTVActor *MGhost, ePlayerType pt, int id);
	void Update();
	void UpdateLocal();
	void UpdateNet();
	void UpdateAI();
	cTV_2DVECTOR GetPos();
	void SetPos(cTV_2DVECTOR pos);
	void MakePacman();
	void Move();
	ePlayerType GetType();
	int GetID();
	void Kill( float fTick = 0 );
	bool dead, bIsPacman;
	int score;
	string nick;
private:
protected:
	cTV_2DVECTOR pos, lastpos, targetpos, movevec, movevecWant, movevecGoing;
	CTVActor *MeshPacman, *MeshGhost;
	float fTickCount, fTickCount2;
	ePlayerType PlayerType;
	int MyID;
};

#endif