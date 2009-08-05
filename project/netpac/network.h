#ifndef NETWORK_H
#define NETWORK_H

typedef struct {
	int id, score;
	int iNumClients;
	char nick[10];
	cTV_2DVECTOR pos;
	bool isPacman;
} PlayerInfo;

typedef struct {
	IPaddress address;
	PlayerInfo *pin;
} Client;

enum eNetworkMode
{
	CLIENT,
	SERVER
};

enum eNetworkMsg
{
	NULLMSG,
	UPDATEPOS,
	QUIT
};

class Network : public Singleton<Network>
{
	friend Singleton;
public:
	void Init();
	void Shutdown();
	void Update();
	void UpdateClient();
	void UpdateServer();
	void SendCmd(int no, const char *cmd);
	void EatPill(int no, int playerno=0);
	int PlayerNo;
	int iNumClients, iNetMsg;
private:
protected:
	Network(){}
	string sIP, sPort;
	eNetworkMode NetworkMode;
	eNetworkMsg NetworkMsg;
	IPaddress ip;
	Uint16 port;
	Uint32 ipaddr;
	UDPsocket sd;
	UDPpacket *p, *p2;
	IPaddress ServerIP, ClientIP;
	cTV_2DVECTOR vecPosition;
	vector<Client *> ClientList;
	vector<Client *>::iterator ClientIter;
	PlayerInfo pin, *pin2;
	char PlayerNick[10];
	char SingleCmd[21];
	Client *newclient;
	string sNick;
	float fTickCount,fTickCount2;
	bool bAddMyself, bAddExisting;
};

#define DNetwork Network::Instance()
#endif