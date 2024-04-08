#pragma once

#include <map>
#include <set>
#include "../TeNTaCleLib/Network/TCPSocket.h"
#include "../TeNTaCleLib/Network/UDPSocket.h"
#include "PlayerManager.h"
#include "EnemyManager.h"
#include "CollisionDataManager.h"

#define MAX_CONNECTION 5			// �ő�ڑ���
#define MAX_ENEMIES 20				// �ő�G��
#define MAX_COLLISION_RESULT 5		// �Փ˔��茋�� (�v���C���[ �� �G)

enum GAME_STATE
{
	WAITING = 0,
	READY,
	GAME,
	GAME_OVER,
	WIN,
};

enum TCP_COMMAND
{
	COMMAND_CONFIRM,
	COMMAND_DISCONNECT_REQUEST,
	COMMAND_DISCONNECT_CONFIRM,
};

struct TCP_CONNECTION_DATA
{
	int client_id = -1;
	int gameState = -1;
	int classType = -1;
	int colorIdx = -1;
	char name[64];
};

struct TCP_SERVER_DATA
{
	int client_id = -1;
	int clientCount = 0;
	int gameState = -1;
	TCP_CONNECTION_DATA clients[MAX_CONNECTION];
};

struct UDP_CONNECTION_DATA
{
	int count = -1;
	PLAYER_DATA playerData;
	ENEMY_COLLISION collisions[MAX_COLLISION_RESULT];
};

struct UDP_SERVER_DATA
{
	int count = -1;
	int gameState = -1;
	PLAYER_DATA playerData[MAX_CONNECTION];
	ENEMY_DATA enemyData[MAX_ENEMIES];
	ENEMY_COLLISION collisions[MAX_CONNECTION][MAX_COLLISION_RESULT];
};

struct CLIENT_DATA
{
	TCP_CONNECTION_DATA conn;
	UDP_CONNECTION_DATA player;
};

class NetworkController
{
public:
	NetworkController() = default;
	~NetworkController() = default;

	virtual bool Initialize(const char* address, const char* port) = 0;
	virtual void Finalize() = 0;
	virtual void SendDisconnection() = 0;

	virtual void UpdateConnection() = 0;
	virtual void Update() = 0;
	virtual void Reset() = 0;

	virtual void GameStart() {}

	void SetGameState(int gameState) { this->gameState = gameState; }
	int GetGameState() { return gameState; }
	int GetClientCount() { return clientCount; }
	int GetClassType() { return classType; }
	int GetColorIndex() { return colorIdx; }

	void SetClassType(int type) { classType = type; tcpSendFlag = true; }
	void SetColorIndex(int idx) { colorIdx = idx; tcpSendFlag = true; }

	bool IsHost() { return isHost; }
	void ReadyTrigger() { isReady = !isReady; tcpSendFlag = true; }
	bool IsReady() { return isReady; }
protected:
	int clientCount = 0;
	int gameState = GAME_STATE::WAITING;
	int classType = PLAYER_CLASS::KNIGHT;
	int colorIdx = Player::COLOR_PATTERN::DEFAULT;

	bool isHost = false;
	bool isReady = false;
	bool tcpSendFlag = true;

	float tcpSendTime = 5.0f; // 5�b���Ƃɑ��M
};

class ClientNetworkController : public NetworkController
{
public:
	ClientNetworkController() = default;
	~ClientNetworkController() = default;

	virtual bool Initialize(const char* address, const char* port) override;
	virtual void Finalize() override;
	void SendDisconnection() override;

	virtual void UpdateConnection() override;
	virtual void Update() override;

	virtual void Reset() override;
private:
	// �Q�[�����̐ڑ��m�F TCP
	void CheckAlive();
	// UDP��M
	void RecvUDP();
	// UDP���M
	void SendUDP();

	void UpdatePlayers();
private:
	float tcpSendTimer = tcpSendTime;
	int recvCount = 0;
	int sendCount = 0;

	TCP_CONNECTION_DATA players[MAX_CONNECTION];

	TCPClientSocket tcpClientSocket;
	UDPClientSocket udpClientSocket;
};

class HostNetworkController : public ClientNetworkController
{
public:
	HostNetworkController() { isHost = true; };
	~HostNetworkController() = default;

	virtual bool Initialize(const char* address, const char* port) override;
	virtual void Finalize() override;
	void SendDisconnection() override;

	void GameStart() override;

	// �ڑ��҂��E�L�����I�𓯊�
	virtual void UpdateConnection() override;

	virtual void Update() override;

	void Reset() override;
private:
	// Update
	// �Q�[�����̐ڑ��m�F TCP
	void CheckAlive();
	// UDP��M
	void RecvUDP();
	// UDP���M
	void SendUDP();
	// �Փˌ��ʂ̏���
	void CollsionReceive(int client_id, ENEMY_COLLISION collision);

private:
	float tcpSendTimer = tcpSendTime;
	int sendCount = 0;
	PLAYER_DATA playersData[MAX_CONNECTION];

	bool startFlag = false;
	TCPServerSocket tcpServerSocket = TCPServerSocket(MAX_CONNECTION);
	UDPServerSocket udpServerSocket;
	std::map<int, CLIENT_DATA> clients;
	std::set<int> disconnect_clients;
};