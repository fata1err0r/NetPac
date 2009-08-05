#include "stdafx.h"

void Network::Init()
{
	if(SDLNet_Init()==-1)
	{
		printf("SDLNet_Init: %s\n",SDLNet_GetError());
		SDL_Quit();
		exit(2);
	}

	bool validselection=false;
	char selection;
	while(!validselection)
	{
		cout << "Please select client or server:\n1.Client\n2.Server\n";
		cin >> selection;
		if (selection == '1')
		{
			NetworkMode = CLIENT;
			validselection=true;
		}

		else if (selection == '2')
		{
			NetworkMode = SERVER;
			validselection=true;
		}
		else
			cout << "invalid selection\n";
	}


	switch (NetworkMode)
	{

	case CLIENT:
		cout<<"Client selected.\n";
		//cout << "Please enter the port to connect to (0=2000): ";
		//cin >> sPort;
		//if (sPort=="0")
		//{
		//	cout << "Defaulting to port 2000\n";
			sPort = "2000";
		//}
		port=(Uint16)strtol(sPort.c_str(),NULL,0);

		//cout << "Please enter the ip/name to connect to (0=serverip.txt): ";
		//cin >> sIP;

		//if (sIP=="0")
		{
			ifstream in("media/serverip.txt");
			getline(in, sIP);
			in.close();
			cout << "Connecting to address in media/serverip.txt: " << sIP << endl;
		}
		/* Open a socket on random port */
		if (!(sd = SDLNet_UDP_Open(0)))
		{
			fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		/* Resolve server name  */

		if (SDLNet_ResolveHost(&ServerIP, sIP.c_str(),port))
		{
			fprintf(stderr, "SDLNet_ResolveHost:");
			exit(EXIT_FAILURE);
		}
		else
		{
			if (!(p = SDLNet_AllocPacket(512)))
			{
				fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
				exit(EXIT_FAILURE);
			}
			if (!(p2 = SDLNet_AllocPacket(512)))
			{
				fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
				exit(EXIT_FAILURE);
			}
			cout << "Found Server!\nEnter Nick (9 Characters Max): ";
			string nick;
			
			cin >>nick;
			if (nick.length()>9)
				nick.replace(9, nick.size()-9, "");
			
			nick.copy(PlayerNick, nick.size());
			PlayerNick[nick.size()]='\0';
			p->data = (Uint8*)&PlayerNick;
			p->address.host = ServerIP.host;	/* Set the destination host */
			p->address.port = ServerIP.port;	/* And destination host */
			p->len = sizeof(PlayerNick);
			if(SDLNet_UDP_Bind(sd, 0, &ServerIP)==-1)
			{
				printf("SDLNet_UDP_Bind: %s\n",SDLNet_GetError());
				exit(7);
			}
			//send nick to request login
			SDLNet_UDP_Send(sd, 0, p);
			bool verified=false;
			
			//loop until server replys with user no.
			while(!verified)
			{
				if (SDLNet_UDP_Recv(sd, p2))
				{
					PlayerNo = *(int*)p2->data;
					verified = true;
					cout << "Connected to server as player " << PlayerNo << "!\n";
				}
			}
				//cout << "error connecting to server";
		}
	
		

		/* Allocate memory for the packet */

		break;

	case SERVER:
		cout<<"Server selected.\n";
		//cout << "Please enter the port to listen to (0=2000): ";
		//cin >> sPort;
		//if (sPort=="0")
		{
			cout << "Defaulting to port 2000\n";
			sPort = "2000";
		}
		port=(Uint16)strtol(sPort.c_str(),NULL,0);

		/* Open a socket */
		if (!(sd = SDLNet_UDP_Open(port)))
		{
			fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		SDLNet_ResolveHost(&ServerIP, NULL, 2000);
		const char *pcname = SDLNet_ResolveIP(&ServerIP);
		cout << pcname << " open for clients...\n";
		/* Make space for the packet */
		if (!(p = SDLNet_AllocPacket(512)))
		{
			fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		if (!(p2 = SDLNet_AllocPacket(512)))
		{
			fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
		}
		PlayerNo=0;
		break;
		
	}
	
	iNumClients=0;
	
	pin.pos.x=1; pin.pos.y=1;
	fTickCount2 = DGraphics->GetEngine()->AccurateTimeElapsed();
	newclient=NULL;
	bAddMyself=false; bAddExisting=false;
	
}

//send a command to a player. valid commands are:
// "DieBitch!" - kill the player
// "PuckYou!" - make player pacman

void Network::SendCmd(int no, const char *cmd)
{
	string scmd=cmd;
	int length = scmd.copy(SingleCmd, 21);
	SingleCmd[length]='\0';
	p->data = (Uint8*)&SingleCmd;
	p->len = sizeof(SingleCmd);
	//send nick to request login
	if (!no)
	{
		for (int x=1; x<iNumClients; x++)
			SDLNet_UDP_Send(sd, x, p);
	}
	else
		SDLNet_UDP_Send(sd, no, p);
}

//tell network players a pill has been eaten, if a player no is given it will send only to that player
void Network::EatPill(int no, int playerno)
{
	p2->data = (Uint8*)&no;
	p2->len = sizeof(int);
	
	if (!playerno)
	{
		for (int x=1; x<iNumClients; x++)
			SDLNet_UDP_Send(sd, x, p2);
	}
	else
		SDLNet_UDP_Send(sd, playerno, p2);
}

void Network::Update(){
	//get the time, and add to it so we can send data every n milliseconds
	fTickCount = DGraphics->GetEngine()->AccurateTimeElapsed();
	fTickCount2 += fTickCount;
	switch (NetworkMode)
	{
	case CLIENT:
		UpdateClient();
		break;
	case SERVER:
		UpdateServer();
		break;
	}
}

void Network::UpdateClient()
{
	//done once when connected and other classes loaded
	if (!bAddExisting)
	{
		//add others
		for(int i=0; i<PlayerNo; i++)
		{
			DLevel->AddPlayer(PT_NET, i);
			iNumClients++;
		}
		//add yourself
		DLevel->AddPlayer(PT_LOCAL, PlayerNo);
		iNumClients++;
		bAddExisting=true;
	}

	if (fTickCount2 >20) // dont send too much crap
	{
		pin.pos = DLevel->GetPlayer(PlayerNo)->GetPos();
		pin.id = PlayerNo;
		p->data=(Uint8*)&pin;
		p->len = sizeof(PlayerInfo);
		SDLNet_UDP_Send(sd, 0, p);
		fTickCount2=0;
	}

	while (SDLNet_UDP_Recv(sd, p2))
	{
		switch(p2->len)
		{
		case sizeof(PlayerInfo):
			pin2 = ((PlayerInfo*)p2->data);
			//has a new player connected?
			while (pin2->iNumClients > iNumClients)
			{
				DLevel->AddPlayer(PT_NET, iNumClients);
				iNumClients++;
			}
			DLevel->GetPlayer(pin2->id)->nick=pin2->nick;
			DLevel->GetPlayer(pin2->id)->score = pin2->score;
			DLevel->GetPlayer(pin2->id)->bIsPacman = pin2->isPacman;
			//take info about the other players
			if (pin2->id != PlayerNo)
			{
				DLevel->GetPlayer(pin2->id)->SetPos(pin2->pos);
				
				//cout << DLevel->GetPlayer(pin2->id)->nick << DLevel->GetPlayer(pin2->id)->score;
			}
			continue;
			break;
		case sizeof(SingleCmd):
			if (!strcmp((char*)p2->data, "DieBitch!"))
				DLevel->GetPlayer(PlayerNo)->Kill();
			else if (!strcmp((char*)p2->data, "PacAlert!"))
				DLevel->PacAlert();
			else if (!strcmp((char*)p2->data, "PacNormal!"))
				DLevel->PacNormal();
			else if (!strcmp((char*)p2->data, "RestartLevel!"))
				DLevel->RestartLevel();
			else if (!strcmp((char*)p2->data, "LevelFinish!"))
				DLevel->LevelFinish();
			else if (!strcmp((char*)p2->data, "ServerQuit!"))
				DLevel->bServerQuit=true;
			else if (!strcmp((char*)p2->data, "DieSound!"))
				DSound->PlayDie();
			//must be a level no to load
			else
				DLevel->StartLevel(atoi((char*)p2->data));
			
			break;
		//eat pill command
		case sizeof(int):
			DLevel->EatPill(*(int *)p2->data);
			break;
		
		default:
			break;
		}
	}


}

void Network::UpdateServer()
{
	//used to store current level no to send to client
	stringstream levno;

	//done once when connected and other classes loaded
	if(!bAddMyself)
	{
		DLevel->bIsServer=true;
		DLevel->AddPlayer(PT_SERVER,iNumClients);
		DLevel->SetPacman(0);
		DLevel->StartLevel(0);
		DLevel->GetPlayer(0)->SetPos(DLevel->GetPacmanStart());
		DLevel->GetPlayer(0)->nick = "Server";
		iNumClients++;
		bAddMyself=true;
	}

	/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
	while (SDLNet_UDP_Recv(sd, p))
	{
		switch(p->len)
		{
		case sizeof(PlayerNick):
			//get players nick ready to assign them a no.
			cout << ((char *)p->data) << " connected!\n";
			newclient = new Client;
			newclient->pin = new PlayerInfo;
			newclient->address = p->address;
			newclient->pin->id = iNumClients;
			DLevel->AddPlayer(PT_NET,iNumClients);
			DLevel->GetPlayer(iNumClients)->bIsPacman=false;
			DLevel->GetPlayer(iNumClients)->nick = (char *)p->data;
			ClientList.push_back(newclient);
			if(SDLNet_UDP_Bind(sd, iNumClients, &newclient->address)==-1)
			{
				printf("SDLNet_UDP_Bind: %s\n",SDLNet_GetError());
				exit(7);
			}
			//send player their no.
			PlayerNo = iNumClients;
			p2->data=(Uint8*)&iNumClients;
			p2->len = sizeof(PlayerNo);
			SDLNet_UDP_Send(sd, iNumClients, p2);
			//tell player current level details
			levno<<DLevel->iCurrentLevel;
			DNetwork->SendCmd(iNumClients, levno.str().c_str());
			DLevel->SendEatenPills(iNumClients);
			DNetwork->SendCmd(iNumClients, "DieBitch!");

			PlayerNo=0;
			iNumClients++;
			break;
		case sizeof(PlayerInfo):
			pin2 = ((PlayerInfo*)p->data);
			DLevel->GetPlayer(pin2->id)->SetPos(pin2->pos);
			break;
		default:
			break;
		}
	}
	if (fTickCount2 >20) // dont send too much crap
	{
		for (int i=1; i<iNumClients; i++)
		{
			//p2->address=(*ClientList[i]).address;
			for (int x=0; x<iNumClients; x++)
			{
				strcpy(pin.nick, DLevel->GetPlayer(x)->nick.c_str());
				pin.score = DLevel->GetPlayer(x)->score;
				pin.pos = DLevel->GetPlayer(x)->GetPos();
				pin.id = DLevel->GetPlayer(x)->GetID();
				pin.isPacman = DLevel->GetPlayer(x)->bIsPacman;
				pin.iNumClients = iNumClients;
				p2->data=(Uint8*)&pin;
				p2->len = sizeof(PlayerInfo);
				SDLNet_UDP_Send(sd, i, p2);
			}
		}
		fTickCount2=0;
	}
}

void Network::Shutdown()
{
	switch (NetworkMode)
	{
	case CLIENT:
		DLevel->GetPlayer(PlayerNo)->SetPos(cTV_2DVECTOR(400,400));
		Update();
		break;
	case SERVER:
		SendCmd(0, "ServerQuit!");
		break;
	}
}