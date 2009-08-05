#include "level.h"
#include "stdafx.h"

void Level::Init()
{
	int iDirLight = DGraphics->GetLightEngine()->CreateDirectionalLight( &cTV_3DVECTOR( 1.f, -1.f, 1.f), 1.f, 1.f, 1.f );

	DGraphics->GetLightEngine()->SetLightRange( iDirLight,1500 );
	DGraphics->GetLightEngine()->SetLightProperties(iDirLight, true, true, false);
	DGraphics->GetLightEngine()->EnableLight( iDirLight, true );
	DGraphics->GetLightEngine()->SetSpecularLighting( true );
	DGraphics->GetLightEngine()->SetProjectiveShadowsProperties(iDirLight, cTV_LIGHTSHADOW_PROJECTIVE_GLOBAL, 512);

	iYellowMaterial = DGraphics->GetMaterialFactory()->CreateLightMaterial( 1.0f, 1.0f, 0.0f, 1.f, 0.4f);
	iRedMaterial = DGraphics->GetMaterialFactory()->CreateLightMaterial( 1.0f, 0.0f, 0.0f, 1.f, 0.4f);
	iGreenMaterial = DGraphics->GetMaterialFactory()->CreateLightMaterial( 0.0f, 1.0f, 0.0f, 1.f, 0.4f);
	iBlueMaterial = DGraphics->GetMaterialFactory()->CreateLightMaterial( 0.0f, 0.0f, 1.0f, 1.f, 0.4f);
	iWhiteMaterial = DGraphics->GetMaterialFactory()->CreateLightMaterial( 1.0f, 1.0f, 1.0f, 1.f, 0.4f);
	
	MeshPacman = DGraphics->GetScene()->CreateActor();
	MeshPacman->LoadTVA("pacman.tva");
	MeshPacman->SetMaterial(iYellowMaterial);
	MeshPacman->SetTexture(DGraphics->GetTextureFactory()->LoadTexture("pacman.dds"));
	MeshPacman->SetScale(0.003f, 0.003f, 0.003f);
	MeshPacman->SetLightingMode( cTV_LIGHTING_MANAGED );
	MeshPacman->SetActorMode(cTV_ACTORMODE_BLENDING);
	MeshPacman->PlayAnimation();

	MeshGhost = DGraphics->GetScene()->CreateActor();
	MeshGhost->LoadTVA("ghost.tva");
	MeshGhost->SetMaterial(iRedMaterial);
	MeshGhost->SetScale(0.013f, 0.013f, 0.013f);
	MeshGhost->SetLightingMode( cTV_LIGHTING_MANAGED );
	MeshGhost->SetRotation(0, -15, 0);
	MeshGhost->SetCullMode(cTV_DOUBLESIDED);

	MeshArrow = DGraphics->GetScene()->CreateActor();
	MeshArrow->LoadTVA("arrow.tva");
	MeshArrow->SetMaterial(iGreenMaterial);
	MeshArrow->SetScale(0.008f, 0.008f, 0.008f);
	MeshArrow->SetLightingMode( cTV_LIGHTING_MANAGED );
	MeshArrow->RotateX(-40);
	MeshArrow->PlayAnimation();

	MeshSpawn = DGraphics->GetScene()->CreateActor();
	MeshSpawn->LoadTVA("spawn_beam.tva");
	MeshSpawn->SetMaterial(iWhiteMaterial);
	MeshSpawn->SetScale(0.4f, 0.3f, 0.4f);
	MeshSpawn->SetLightingMode( cTV_LIGHTING_MANAGED );
	MeshSpawn->SetCullMode(cTV_DOUBLESIDED);
	MeshSpawn->PlayAnimation();

	//create mesh assets
	MeshBox = DGraphics->GetScene()->CreateMeshBuilder();
	MeshBox->CreateBox(1, 1, 1, false);
	MeshBox->SetMaterial(iBlueMaterial);
	MeshBox->SetLightingMode( cTV_LIGHTING_MANAGED );

	MeshBox->SetTexture(DGraphics->GetTextureFactory()->LoadTexture("wall.dds"));

	MeshDot = DGraphics->GetScene()->CreateActor();
	MeshDot->LoadTVA("dot.tva");
	MeshDot->SetMaterial(iWhiteMaterial);
	MeshDot->SetScale(0.01f, 0.01f, 0.01f);
	MeshDot->SetLightingMode( cTV_LIGHTING_MANAGED );
	//MeshDot->

	MeshPill = DGraphics->GetScene()->CreateActor();
	MeshPill->LoadTVA("pill.tva");
	MeshPill->SetMaterial(iWhiteMaterial);
	MeshPill->SetScale(0.04f, 0.04f, 0.04f);
	MeshPill->SetLightingMode( cTV_LIGHTING_NONE );

	iPacmanPlayer=0;
	bIsServer=false;
	fTickCount2=0;
	bPacAlert = false;
	iDotCount=0;
	iTotalLevels=0;
	LoadLevels();
	iCurrentLevel=0;
	bLevelFinish=false;
	bServerQuit=false;
	bChaseCam=false;
}

//returns true if a brick is located in position
bool Level::IsBrick(int x, int y)
{
	if (cLevelBuf[x][19-y] == 'X')
		return true;
	
	return false;
}

//copy a level out of the level list and start it
void Level::StartLevel(int no)
{
	bLevelFinish=false;
	DotList.clear();
	iDotCount=0;
	for (int y=0; y<20; y++)
	{
		for (int x=0; x<20; x++)
		{
			cLevelBuf[x][y] = LevelList[no%iTotalLevels]->cLevelBuf[x][y];
			if (cLevelBuf[x][y]=='.')
			{
				DotBuffer *dbuff = new DotBuffer;
				dbuff->isPill=false;
				dbuff->pos = cTV_2DVECTOR(x, 19-y);
				dbuff->render = true;
				DotList.push_back(dbuff);
				iDotCount++;
			}
			else if (cLevelBuf[x][y]=='O')
			{
				DotBuffer *dbuff = new DotBuffer;
				dbuff->isPill=true;
				dbuff->pos = cTV_2DVECTOR(x, 19-y);
				dbuff->render = true;
				DotList.push_back(dbuff);
				iDotCount++;
			}
			else if(cLevelBuf[x][y]=='G')
				GhostSpawn = cTV_2DVECTOR(x, 19-y);
			else if(cLevelBuf[x][y]=='S')
			{
				MeshSpawn->SetPosition(x, 19-y, 0);
				GhostStart = cTV_2DVECTOR(x, 19-y);
			}
			else if(cLevelBuf[x][y]=='P')
				PacmanStart = cTV_2DVECTOR(x, 19-y);
		}
	}
	sLevelName = LevelList[no%iTotalLevels]->sLevelName;

	if(bIsServer)
	{
		//reset scores
		for(PlayerIter = PlayerList.begin(); PlayerIter!=PlayerList.end(); PlayerIter++)
		{
			if((*PlayerIter)->GetType() != PT_NET)
			{
				if ((*PlayerIter)->GetID() == iPacmanPlayer)
					(*PlayerIter)->SetPos(DLevel->GetPacmanStart());
				else
					(*PlayerIter)->Kill(3000);
			}
			(*PlayerIter)->score=0;
		}
	}
	else
	{
		if(GetPlayer(DNetwork->PlayerNo)->bIsPacman)
			GetPlayer(DNetwork->PlayerNo)->SetPos(DLevel->GetPacmanStart());
		else
			GetPlayer(DNetwork->PlayerNo)->Kill(3000);
	}
	DSound->PlayFight();
}

void Level::LevelFinish()
{
	iWinner=0;
	for(PlayerIter = PlayerList.begin(); PlayerIter!=PlayerList.end(); PlayerIter++)
	{
		if ((*PlayerIter)->score>iWinner)
		{
			iWinner = (*PlayerIter)->score;
			sWinner = (*PlayerIter)->nick;
		}
	}
	if(!bLevelFinish)
		DSound->PlayWoohoo();

	bLevelFinish=true;
	
}

//loads the level from a text file and puts it in a 20x20 2d array
//then stores it a vector for easy retieval later
void Level::LoadLevels()
{
	LevelList.clear();
	bool bAllLevels = false;
	while(!bAllLevels)
	{
		stringstream filename;
		filename << "level" << iTotalLevels<< ".txt";
		ifstream in(filename.str().c_str());
		string rd;
		getline(in, rd);
		in.close();
		if (rd == "")
			bAllLevels=true;
		else
			iTotalLevels++;
	}

	for (int z=0; z<iTotalLevels; z++)
	{
		stringstream filename;
		filename << "level" << z << ".txt";
		ifstream in(filename.str().c_str());

		string sLevelData;

		LevelData *ld = new LevelData;
		for (int y=0; y<20; y++)
		{
			getline(in, sLevelData);
			for (int x=0; x<20; x++)
			{
				ld->cLevelBuf[x][y] = sLevelData[x];
				cLevelBuf[x][y] = sLevelData[x];
			}
		}
		getline(in, sLevelData);
		ld->sLevelName = sLevelData;
		LevelList.push_back(ld);
		in.close();
	}
	cout << "Found " << iTotalLevels << " level files!\n";
}
	

void Level::SendEatenPills(int playerno)
{
	int x=0;
	for ( DotIter = DotList.begin(); DotIter != DotList.end(); DotIter++)
	{
		if (!(*DotIter)->render)
			DNetwork->EatPill(x, playerno);
		
		x++;
	}
}

//give player special pacman abilities
void Level::SetPacman(int playerno)
{
	//PlayerList[playerno]->MakePacman();
	iPacmanPlayer = playerno;

}

cTV_2DVECTOR Level::GetPacmanPos()
{
	return GetPlayer(iPacmanPlayer)->GetPos();
}

void Level::AddPlayer(ePlayerType pt, int id)
{
	Player *player = new Player(MeshPacman, MeshGhost, pt, id);
	PlayerList.push_back(player);
}
cTV_2DVECTOR Level::GetGhostSpawn()
{
	return GhostSpawn;
}

cTV_2DVECTOR Level::GetGhostStart()
{
	return GhostStart;
}

cTV_2DVECTOR Level::GetPacmanStart()
{
	return PacmanStart;
}

Player* Level::GetPlayer(int i)
{
	return PlayerList[i];
}

void Level::EatPill(int no)
{
	DotList[no]->render=false;
	//play eat pill sound
	DSound->PlayWaka();
}

//pacman got a pill!
void Level::PacAlert()
{
	if(bIsServer)
		DNetwork->SendCmd(0, "PacAlert!");
	MeshBox->SetMaterial(iRedMaterial);
	bPacAlert=true;
	fTickCount3=0;
	MeshPacman->SetMaterial(iRedMaterial);
	MeshGhost->SetMaterial(iBlueMaterial);
	DSound->PlayPill();
}

//pacman back to normal
void Level::PacNormal()
{
	if(bIsServer)
		DNetwork->SendCmd(0, "PacNormal!");
	MeshBox->SetMaterial(iBlueMaterial);
	bPacAlert=false;
	MeshGhost->SetMaterial(iRedMaterial);
	MeshPacman->SetMaterial(iYellowMaterial);
}

void Level::RestartLevel()
{
	if(bIsServer)
		DNetwork->SendCmd(0, "RestartLevel!");

	for ( DotIter = DotList.begin(); DotIter != DotList.end(); DotIter++)
	{
		iDotCount++;
		(*DotIter)->render=true;
	}
}

cTV_2DVECTOR Level::GetNextDotPos(cTV_2DVECTOR Playerpos)
{	
	float ClosestDot = 100;
	cTV_2DVECTOR DotPos=cTV_2DVECTOR(0,0);

	for ( DotIter = DotList.begin(); DotIter != DotList.end(); DotIter++)
	{
		if((*DotIter)->render)
		{
			if (DGraphics->GetMathLibrary()->GetDistanceVec2D(&Playerpos,&(*DotIter)->pos) < ClosestDot)
			{
				ClosestDot = DGraphics->GetMathLibrary()->GetDistanceVec2D(&Playerpos,&(*DotIter)->pos);
				DotPos = (*DotIter)->pos;
			}
		}
	}
	
	return DotPos;	
}

cTV_2DVECTOR Level::GetClosestGhost(int playerno)
{
	float ClosestGhost = 100;
	cTV_2DVECTOR GhostPos=cTV_2DVECTOR(0,0);

	vector<Player *>::iterator PlayerIter2;
	for(PlayerIter2 = PlayerList.begin(); PlayerIter2!=PlayerList.end(); PlayerIter2++)
	{
		if( (*PlayerIter2)->GetID() != playerno)
		{
			if (DGraphics->GetMathLibrary()->GetDistanceVec2D(&GetPlayer(playerno)->GetPos(),&(*PlayerIter2)->GetPos()) < ClosestGhost)
			{
				ClosestGhost=DGraphics->GetMathLibrary()->GetDistanceVec2D(&GetPlayer(playerno)->GetPos(),&(*PlayerIter2)->GetPos());
				GhostPos = (*PlayerIter2)->GetPos();
			}
		}
	}

	return GhostPos;

}

void Level::Update()
{
	fTickCount = DGraphics->GetEngine()->AccurateTimeElapsed();
	fTickCount2+=fTickCount;
	
	MeshSpawn->Render();
	if (bChaseCam)
	{
		cTV_2DVECTOR playerpos;
		if(bIsServer)
			playerpos = GetPlayer(0)->GetPos();
		else
			playerpos = GetPlayer(DNetwork->PlayerNo)->GetPos();

		if(DInput->IsKey(SDLK_e))
			DGraphics->GetCamera()->MoveRelative(0.01*fTickCount,0,0);
		if(DInput->IsKey(SDLK_q))
			DGraphics->GetCamera()->MoveRelative(-0.01*fTickCount,0,0);
		
		if(DInput->IsKey(SDLK_w))
			DGraphics->GetCamera()->MoveRelative(0,0.01*fTickCount,0);
		if(DInput->IsKey(SDLK_s))
			DGraphics->GetCamera()->MoveRelative(0,-0.01*fTickCount,0);
		if(DInput->IsKey(SDLK_a))
			DGraphics->GetCamera()->MoveRelative(0,0,-0.01*fTickCount);
		if(DInput->IsKey(SDLK_d))
			DGraphics->GetCamera()->MoveRelative(0,0,0.01*fTickCount);

		DGraphics->GetCamera()->SetLookAt(playerpos.x, playerpos.y, 0);
	}
	if(DInput->IsKeySingle(SDLK_c))
	{
		if(bChaseCam)
		{
			DGraphics->GetCamera()->SetPosition( 9.f, 11.f, -21.f );
			DGraphics->GetCamera()->SetLookAt(9.f, 11.f, 0.f );
			bChaseCam=false;
		}
		else
			bChaseCam=true;
	}
	//render walls
	for (int y=0; y<20; y++)
	{
		for (int x=0; x<20; x++)
		{
			if (cLevelBuf[x][y] == 'X')
			{
				MeshBox->SetPosition(x, 19-y, 0);
				MeshBox->Render();
			}

		}	
	}
	
	//render the dots
	for ( DotIter = DotList.begin(); DotIter != DotList.end(); DotIter++)
	{
		if((*DotIter)->render)
		{
			
			if ((*DotIter)->isPill)
			{
				MeshPill->SetPosition((*DotIter)->pos.x, (*DotIter)->pos.y, 0);
				MeshPill->Render();
			}
			else
			{
				MeshDot->SetPosition((*DotIter)->pos.x, (*DotIter)->pos.y, 0);
				MeshDot->Render();
			}

		}
	}
	//manually play the dot animations as they are rendered multiple times
	MeshDot->PlayAnimation();
	MeshDot->SetKeyFrame((MeshDot->GetKeyFrame())+0.5);
	MeshDot->PauseAnimation();
	MeshPill->PlayAnimation();
	MeshPill->SetKeyFrame((MeshPill->GetKeyFrame())+0.3);
	MeshPill->PauseAnimation();
	MeshGhost->PlayAnimation();
	MeshGhost->SetKeyFrame((MeshGhost->GetKeyFrame())+0.3);
	MeshGhost->PauseAnimation();
	
	MeshArrow->SetPosition(GetPlayer(DNetwork->PlayerNo)->GetPos().x, GetPlayer(DNetwork->PlayerNo)->GetPos().y+0.2, -0.4);
	
	MeshArrow->Render();
	//pac alert timer


	if (bIsServer)
	{
		if (DInput->IsKeySingle(SDLK_b))
		{
			AddPlayer(PT_AI, DNetwork->iNumClients);
			GetPlayer(DNetwork->iNumClients)->Kill();
			GetPlayer(DNetwork->iNumClients)->nick = "AI";
			DNetwork->iNumClients++;
			cout << "Added Bot\n";
		}
		else if (DInput->IsKeySingle(SDLK_k))
		{
			for(PlayerIter = PlayerList.begin(); PlayerIter!=PlayerList.end(); PlayerIter++)
			{
				if((*PlayerIter)->GetType() == PT_AI)
				{
					if ((*PlayerIter)->GetPos().x !=400)
					{
						(*PlayerIter)->SetPos(cTV_2DVECTOR(400,400));
						PlayerIter=PlayerList.end()-1;
						cout << "Killed Bot\n";
					}
				}
			}
		}

		cTV_2DVECTOR PacmanPos = PlayerList[iPacmanPlayer]->GetPos();
		//test for collision between pacman and the pills
		int x=0;
		for ( DotIter = DotList.begin(); DotIter != DotList.end(); DotIter++)
		{
			if ( DGraphics->GetMathLibrary()->GetDistanceVec2D(&PacmanPos,&(*DotIter)->pos ) < 0.5)
			{
				if ((*DotIter)->render)
				{
					EatPill(x);
					DNetwork->EatPill(x);
					iDotCount--;
					if ((*DotIter)->isPill)
					{
						PlayerList[iPacmanPlayer]->score+=10;
						PacAlert();
					}
					else
						PlayerList[iPacmanPlayer]->score+=5;

				}
			}
			x++;
		}
	}

	if(bIsServer && fTickCount2>75)
	{
		fTickCount2=0;
		if (!iDotCount)
		{
			LevelFinish();
			DNetwork->SendCmd(0, "LevelFinish!");
			//RestartLevel();
		}

		cTV_2DVECTOR PacmanPos = PlayerList[iPacmanPlayer]->GetPos();
		


		for(PlayerIter = PlayerList.begin(); PlayerIter!=PlayerList.end(); PlayerIter++)
		{
			if ((*PlayerIter)->GetID() != iPacmanPlayer)
			{
				cTV_2DVECTOR PlayerPos = (*PlayerIter)->GetPos();

				if ( DGraphics->GetMathLibrary()->GetDistanceVec2D(&PacmanPos,&PlayerPos) < 0.5)
				{
					DSound->PlayDie();
					DNetwork->SendCmd(0, "DieSound!");
					if (bPacAlert)
					{	
						if ((*PlayerIter)->GetType() == PT_NET)
						{
							DNetwork->SendCmd((*PlayerIter)->GetID(), "DieBitch!");
							(*PlayerIter)->SetPos(GetGhostStart());
						}
						else
							(*PlayerIter)->Kill();
					}
					else
					{
						if (GetPlayer(iPacmanPlayer)->GetType() == PT_NET)
						{
							DNetwork->SendCmd(GetPlayer(iPacmanPlayer)->GetID(), "DieBitch!");
							GetPlayer(iPacmanPlayer)->SetPos(GetGhostStart());
							GetPlayer(iPacmanPlayer)->bIsPacman=false;
						}
						else
							GetPlayer(iPacmanPlayer)->Kill();
						
						(*PlayerIter)->MakePacman();
						SetPacman((*PlayerIter)->GetID());
						PacAlert();
					}
					PlayerList[iPacmanPlayer]->score+=15;
					//collision dealt with, break the loop
					PlayerIter=PlayerList.end()-1;
				}
			}
		}


	}

	//print pacalert
	if (bPacAlert)
	{
		if(fTickCount3>3000)
		{
			if(bIsServer)
				PacNormal();
		}
		else
		{
			fTickCount3+=fTickCount;
			stringstream spacalert;
			spacalert << "PAC ALERT!!! Time Left: " << (int)(4000-fTickCount3)/1000;
			DGraphics->GetScreen2DText()->NormalFont_DrawText(spacalert.str().c_str(), 290, 10, cTV_COLORKEY_RED);
		}
	}

	if (bLevelFinish)
	{
		if (bIsServer)
		{
			stringstream finishalert;
			finishalert << "End of Level: " << sWinner << " wins with " << iWinner << " points!\n          >PRESS SPACE TO CONTINUE<";
			DGraphics->GetScreen2DText()->NormalFont_DrawText(finishalert.str().c_str(), 250, 10, cTV_COLORKEY_WHITE);
			if(DInput->IsKey(SDLK_SPACE))
			{
				iCurrentLevel+=1;
				StartLevel(iCurrentLevel);
				stringstream levno;
				levno<<iCurrentLevel;
				DNetwork->SendCmd(0, levno.str().c_str());
			}
		}
		else
		{
			stringstream finishalert;
			finishalert << "End of Level: " << sWinner << " wins with " << iWinner << " points!\n>WAITING FOR SERVER TO CONTINUE<";
			DGraphics->GetScreen2DText()->NormalFont_DrawText(finishalert.str().c_str(), 250, 10, cTV_COLORKEY_WHITE);
		}
	}

	if(bServerQuit)
	{
		bLevelFinish=false;
		DGraphics->GetScreen2DText()->NormalFont_DrawText("Server Quit! Press Esc to exit." , 250, 20, cTV_COLORKEY_WHITE);
	}

	// print level name
	DGraphics->GetScreen2DText()->NormalFont_DrawText(sLevelName.c_str(), 180, 60, cTV_COLORKEY_WHITE);

	//print scoreboard
	stringstream Scoreboard;
	Scoreboard << "SCORE: \n";
	
	for(PlayerIter = PlayerList.begin(); PlayerIter!=PlayerList.end(); PlayerIter++)
	{
		if ((*PlayerIter)->GetPos().x!=400) //when user has quit dont show their score
		{
			Scoreboard << (*PlayerIter)->nick  << ": " << (*PlayerIter)->score << endl;
			(*PlayerIter)->Update();
		}
		else if ((*PlayerIter)->GetID()==iPacmanPlayer) //if the quited user is pacman, give server pacman ability
		{
			SetPacman(0);
			GetPlayer(0)->bIsPacman=true;
			(*PlayerIter)->bIsPacman=false;
			GetPlayer(0)->SetPos(GetPacmanStart());
			GetPlayer(0)->dead=false;

		}
	}

	DGraphics->GetScreen2DText()->NormalFont_DrawText(Scoreboard.str().c_str(), 10, 30, cTV_COLORKEY_WHITE);

}