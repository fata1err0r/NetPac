#include "player.h"
#include "stdafx.h"

Player::Player(CTVActor *MPacman, CTVActor *MGhost, ePlayerType pt, int id)
{
	MeshPacman = MPacman; MeshGhost = MGhost;
	PlayerType = pt;
	MyID=id;
	movevec.x=0; movevec.y=0;
	fTickCount2 = 0;
	dead=false;
	bIsPacman = true;
	score=0;

	movevec.x=0;
	movevec.y=0;
}

void Player::Update()
{

	fTickCount = DGraphics->GetEngine()->AccurateTimeElapsed();

	switch (PlayerType)
	{
	case PT_LOCAL:
		UpdateLocal();
		break;
	case PT_SERVER:
		UpdateLocal();
		break;
	case PT_NET:
		UpdateNet();
		break;
	case PT_AI:
		UpdateAI();
		break;
	default:
		break;
	}

	if(bIsPacman)
	{
		MeshPacman->SetMaterial((MyID+1)%4);
		MeshPacman->SetPosition(pos.x, pos.y, 0);
		if(lastpos.x<pos.x)
			MeshPacman->SetRotation(0, 0, 0);
		else if(lastpos.x>pos.x)
			MeshPacman->SetRotation(0, 0, 180);
		else if(lastpos.y<pos.y)
			MeshPacman->SetRotation(0, 0, 90);
		else if(lastpos.y>pos.y)
			MeshPacman->SetRotation(0, 0, 270);
		MeshPacman->Render();
	}
	else
	{
		MeshGhost->SetMaterial((MyID+1)%4);
		MeshGhost->SetPosition(pos.x, pos.y+0.7, 0.2);
		MeshGhost->Render();
	}

	lastpos = pos;
}
void Player::MakePacman()
{
	bIsPacman = true;
}

void Player::Kill( float fTick /* = 0 */ )
{
	dead=true;
	bIsPacman = false;
	fTickCount2=fTick;
	pos = DLevel->GetGhostSpawn();
	targetpos = DLevel->GetGhostSpawn();
	movevec.x=0; movevec.y=0;
}

cTV_2DVECTOR Player::GetPos()
{
	return pos;
}

void Player::SetPos(cTV_2DVECTOR p)
{
	pos=p;
}
ePlayerType Player::GetType()
{
	return PlayerType;
}

int Player::GetID()
{
	return MyID;
}

void Player::UpdateLocal()
{

	if(DLevel->bLevelFinish || DLevel->bServerQuit)
	{
		movevec.x=0;
		movevec.y=0;
		return;
	}

	if (dead)
	{
		stringstream yourdead;
		yourdead << "DEAD! Respawn in " << (int)(6000-fTickCount2)/1000 << " sec";

		DGraphics->GetScreen2DText()->NormalFont_DrawText(yourdead.str().c_str(),  240, 30, cTV_COLORKEY_RED);
		fTickCount2 += fTickCount;
		if (fTickCount2 > 5000)
		{
			pos = DLevel->GetGhostStart();
			targetpos = DLevel->GetGhostStart();
			dead=false;
		}
	}

	if (pos.x == 19)
	{
		pos.x=0;
		movevec.x=1;
		targetpos.x=1;
	}
	else if (pos.x == 0)
	{
		pos.x=19;
		movevec.x=-1;
		targetpos.x=18;
	}

	if (pos.y == 19)
	{
		pos.y=0;
		movevec.y=1;
		targetpos.y=1;
	}
	else if (pos.y == 0)
	{
		pos.y=19;
		movevec.y=-1;
		targetpos.y=18;
	}


	if(DInput->IsKey(SDLK_UP))
	{
		movevecWant.x = 0;
		movevecWant.y = 1;
	}
	else if(DInput->IsKey(SDLK_DOWN))
	{
		movevecWant.x = 0;
		movevecWant.y = -1;
	}
	else if(DInput->IsKey(SDLK_RIGHT))
	{
		movevecWant.y = 0;
		movevecWant.x = 1;
	}
	else if(DInput->IsKey(SDLK_LEFT))
	{
		movevecWant.y = 0;
		movevecWant.x = -1;
	}

	if (movevec.y==0 && movevec.x==0)
	{
		bool goWant=false;
		if(movevecWant.x || movevecWant.y)
		{
			targetpos = pos+movevecWant;
			if(!DLevel->IsBrick(targetpos.x, targetpos.y))
			{
				movevec = movevecWant;
				movevecGoing = movevec;
				goWant=true;
			}
		}
		
		if(!goWant)
		{
			targetpos = pos + movevecGoing;
			if(!DLevel->IsBrick(targetpos.x, targetpos.y))
			{
				movevec = movevecGoing;
			}
		}

	}

	Move();


}

void Player::UpdateAI()
{

	if(DLevel->bLevelFinish || DLevel->bServerQuit)
	{
		movevec.x=0;
		movevec.y=0;
		return;
	}

	if (dead)
	{
		fTickCount2 += fTickCount;
		if (fTickCount2 > 5000)
		{
			pos = DLevel->GetGhostStart();
			targetpos = DLevel->GetGhostStart();
			dead=false;
		}
		return;
	}

	if (pos.x == 19)
	{
		pos.x=0;
		movevec.x=1;
		targetpos.x=1;
	}
	else if (pos.x == 0)
	{
		pos.x=19;
		movevec.x=-1;
		targetpos.x=18;
	}

	if (pos.y == 19)
	{
		pos.y=0;
		movevec.y=1;
		targetpos.y=1;
	}
	else if (pos.y == 0)
	{
		pos.y=19;
		movevec.y=-1;
		targetpos.y=18;
	}

	cTV_2DVECTOR goal;

	if(bIsPacman)
	{
		if(DLevel->bPacAlert)
			goal = DLevel->GetClosestGhost(MyID);
		else
			goal = DLevel->GetNextDotPos(pos);
	}
	else
	{
		if(DLevel->bPacAlert)
		{
			goal=DLevel->GetPacmanPos();
			goal.x -=10;
			goal.y -=10;
		}
		else
			goal=DLevel->GetPacmanPos();
	}


	srand(rand());
	int irand = rand() %100;

	if (movevec.y==0 && movevec.x==0)
	{
		if(irand>50)
		{
			if(pos.x < goal.x)
			{
				targetpos.x = (int)pos.x+1;
				if(!DLevel->IsBrick(targetpos.x, pos.y))
					movevec.x=1;
				else
				{
					targetpos.x = (int)pos.x-1;
					if (!DLevel->IsBrick(targetpos.x, pos.y))
						movevec.x=-1;
				}

			}
			
			else if(pos.x > goal.x)
			{
				targetpos.x = (int)pos.x-1;
				if(!DLevel->IsBrick(targetpos.x, pos.y))
					movevec.x=-1;
				else
				{
					targetpos.x = (int)pos.x+1;
					if (!DLevel->IsBrick(targetpos.x, pos.y))
						movevec.x=+1;
				}
			}
		}


	}

	if (movevec.y==0 && movevec.x==0)
	{
		if(irand<51)
		{
			if(pos.y < goal.y)
			{
				targetpos.y = (int)pos.y+1;
				if(!DLevel->IsBrick(pos.x, targetpos.y))
					movevec.y=1;
				else
				{
					targetpos.y = (int)pos.y-1;
					if(!DLevel->IsBrick(pos.x, targetpos.y))
						movevec.y=-1;
				}
			}

			else if(pos.y > goal.y)
			{
				targetpos.y = (int)pos.y-1;
				if(!DLevel->IsBrick(pos.x, targetpos.y))
					movevec.y=-1;
				else
				{	
					targetpos.y = (int)pos.y+1;
					if(!DLevel->IsBrick(pos.x, targetpos.y))
					movevec.y=+1;
				}

			}
		}
	}

	Move();

	
}

void Player::Move()
{

	pos.x+=movevec.x*(0.005*fTickCount);
	pos.y+=movevec.y*(0.005*fTickCount);
	
	bool bReachX=false;

	if (movevec.x < 0)
	{
		if (pos.x - (int)targetpos.x < 0)
		{
			pos.x+=1;
			bReachX=true;
		}
	}

	if (movevec.x > 0)
	{
		if (pos.x - (int)targetpos.x > 0)
			bReachX=true;
	}

	if (bReachX)
	{
		pos.x=(int)targetpos.x;
		movevec.x = 0;
	}
		


	bool bReachY=false;

	if (movevec.y < 0)
	{
		if (pos.y - (int)targetpos.y < 0)
		{
			pos.y+=1;
			bReachY=true;
		}
	}

	if (movevec.y > 0)
	{
		if (pos.y - (int)targetpos.y > 0)
			bReachY=true;
	}

	if (bReachY)
	{
		pos.y=(int)targetpos.y;
		movevec.y = 0;
	}

}

void Player::UpdateNet()
{
}