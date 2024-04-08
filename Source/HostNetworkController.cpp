#include <iostream>

#include "NetworkController.h"
#include "InputMap.h"
#include "Player.h"
#include "GameData.h"
#include "EnemyManager.h"

bool HostNetworkController::Initialize(const char* address, const char* port)
{
	// TCP�T�[�o�[�N��
	if (!tcpServerSocket.Connect(address, port))
	{
		return false;
	}
	// UDP�T�[�o�[�N��
	if (!udpServerSocket.Connect(address, port))
	{
		return false;
	}

	return ClientNetworkController::Initialize(address, port);
}
void HostNetworkController::Finalize()
{
	tcpServerSocket.Disconnect();
	udpServerSocket.Disconnect();

	ClientNetworkController::Finalize();
}
void HostNetworkController::SendDisconnection()
{
	// TCP���M
	int sendData = TCP_COMMAND::COMMAND_DISCONNECT_CONFIRM;
	for (std::pair<const int, CLIENT_DATA>& client : clients)
	{
		tcpServerSocket.Send(client.first, &sendData, sizeof(sendData));
	}
}

void HostNetworkController::UpdateConnection()
{
	if (!tcpServerSocket.IsConnecting()) return;

	bool tcpSendFlag = false;

	// �ڑ����f�����N���C�A���g����
	for (int id : disconnect_clients)
	{
		clients.erase(id);
	}
	disconnect_clients.clear();

	clientCount = static_cast<int>(clients.size());

	// �ڑ�����
	if (gameState == GAME_STATE::WAITING && clients.size() < MAX_CONNECTION)
	{
		if (tcpServerSocket.Accept())
		{
			int id = tcpServerSocket.GetClientCount() - 1;
			clients[id] = {}; // �V�����N���C�A���g
			clients[id].conn = {
				id,
				GAME_STATE::WAITING,
				PLAYER_CLASS::KNIGHT
			};
			tcpSendFlag = true;
		}
	}
	// TCP��M �N���C�A���g��ԍX�V
	TCP_CONNECTION_DATA recData;
	for (std::pair<const int, CLIENT_DATA>& client : clients)
	{
		if (tcpServerSocket.Receive(client.first, &recData, sizeof(recData)) > 0) // �N���C�A���g�����M
		{
			std::cout << "TCP Receive DATA client_id: " << client.first << " classType:  " << recData.classType << " gameState: " << recData.gameState << std::endl;
			client.second.conn.classType = recData.classType;
			client.second.conn.colorIdx = recData.colorIdx;
			client.second.conn.gameState = recData.gameState;
			strcpy_s(client.second.conn.name, 64, recData.name);

			tcpSendFlag = true;
		}
	}

	tcpSendTimer -= TentacleLib::timer.Delta(); // �������M
	if (tcpSendTimer < 0.0f)
	{
		tcpSendTimer = tcpSendTime;
		tcpSendFlag = true;
	}

	if (tcpSendFlag || startFlag)
	{
		// TCP���M�f�[�^
		TCP_SERVER_DATA sendData = {};
		sendData.clientCount = clientCount;
		sendData.gameState = GAME_STATE::READY;
		int i = 0;
		for (std::pair<const int, CLIENT_DATA>& client : clients)
		{
			sendData.clients[i] = client.second.conn;
			if (client.second.conn.gameState == GAME_STATE::WAITING) sendData.gameState = GAME_STATE::WAITING;
			i++;
		}
		if (startFlag) sendData.gameState = GAME_STATE::GAME;

		// TCP���M
		for (std::pair<const int, CLIENT_DATA>& client : clients)
		{
			sendData.client_id = client.first;
			if (tcpServerSocket.Send(client.first, &sendData, sizeof(sendData)) < 0)
			{
				// ���M���s�A�N���C�A���g�폜
				disconnect_clients.insert(client.first);
			}
		}
	}
	ClientNetworkController::UpdateConnection();
}

void HostNetworkController::Update()
{
	if (!udpServerSocket.IsConnecting()) return;

	// �ڑ��m�F
	CheckAlive();

	// UDP��M
	RecvUDP();

	// UDP���M
	SendUDP();

	ClientNetworkController::Update();
}

void HostNetworkController::CheckAlive()
{
	bool tcpSendFlag = false;

	// �ڑ����f�����N���C�A���g����
	for (int id : disconnect_clients)
	{
		clients[id].player.playerData.connected = false;
		udpServerSocket.Remove(id);
	}
	disconnect_clients.clear();

	// TCP��M �N���C�A���g��ԍX�V
	int recData;	 // COMMAND ��M
	for (std::pair<const int, CLIENT_DATA>& client : clients)
	{
		if (tcpServerSocket.Receive(client.first, &recData, sizeof(recData)) > 0) // �N���C�A���g�����M
		{
			switch (recData)
			{
			case TCP_COMMAND::COMMAND_CONFIRM: break; // �����m�F�p�E�����Ȃ�
			case TCP_COMMAND::COMMAND_DISCONNECT_REQUEST: // �ڑ����f�̃��N�G�X�g
				int command = TCP_COMMAND::COMMAND_DISCONNECT_CONFIRM;
				tcpServerSocket.Send(client.first, &command, sizeof(int)); // ���f�E�Ԏ�
				disconnect_clients.insert(client.first);
				break;
			}
		}
	}

	tcpSendTimer -= TentacleLib::timer.Delta(); // �������M
	if (tcpSendTimer < 0.0f)
	{
		tcpSendTimer = tcpSendTime;
		tcpSendFlag = true;
	}

	if (tcpSendFlag)
	{
		// TCP���M
		int sendData = COMMAND_CONFIRM;
		for (std::pair<const int, CLIENT_DATA>& client : clients)
		{
			if (tcpServerSocket.Send(client.first, &sendData, sizeof(sendData)) < 0)
			{
				// ���M���s�A�N���C�A���g�폜
				disconnect_clients.insert(client.first);
			}
		}
	}
}

// UDP��M
void HostNetworkController::RecvUDP()
{
	struct sockaddr_in addr;
	UDP_CONNECTION_DATA recvData;
	while (udpServerSocket.Receive(&recvData, sizeof(recvData), addr) > 0)
	{
		std::cout << "UDP Receive DATA client_id: " << recvData.playerData.client_id << " Count:  " << recvData.count << std::endl;
		udpServerSocket.Register(recvData.playerData.client_id, addr);

		if (clients[recvData.playerData.client_id].player.count >= recvData.count) continue; // �ߋ��E�����ς݃f�[�^
		CLIENT_DATA& client = clients[recvData.playerData.client_id];

		// �Փ˔��茋�ʎ�M
		for (int i = 0; i < MAX_COLLISION_RESULT; i++)
		{
			if (recvData.collisions[i].count < 0) break; // End Flag

			// �Փ�
			CollsionReceive(recvData.playerData.client_id, recvData.collisions[i]);
		}

		client.player = recvData; // �V�����f�[�^�ۑ�
	}
}
// UDP���M
void HostNetworkController::SendUDP()
{
	// �f�[�^
	UDP_SERVER_DATA sendData;
	sendData.count = sendCount++;
	sendData.gameState = gameState;
	int i = 0;
	for (std::pair<const int, CLIENT_DATA>& client : clients)
	{
		sendData.playerData[i] = client.second.player.playerData;

		for (int j = 0; j < MAX_COLLISION_RESULT; j++)
		{
			sendData.collisions[i][j] = client.second.player.collisions[j];
		}
		i++;
	}
	int j = 0;
	for (Enemy*& enemy : ENEMIES.GetAll())
	{
		enemy->ExportData(sendData.enemyData[j]);
		j++;
	}

	udpServerSocket.SendAll(&sendData, sizeof(sendData));
}

void HostNetworkController::Reset()
{
	clients.clear();
	sendCount = 0;
	for (int i = 0; i < MAX_CONNECTION; i++)
	{
		playersData[i].client_id = -1;
	}
	ClientNetworkController::Reset();
}

void HostNetworkController::GameStart()
{
	startFlag = true;;
}

void HostNetworkController::CollsionReceive(int client_id, ENEMY_COLLISION collision)
{
	CLIENT_DATA& client = clients[client_id];

	if (collision.count < 0) return;

	// ��M�ς݃`�F�b�N
	for (int i = 0; i < MAX_COLLISION_RESULT; i++)
	{
		if (client.player.collisions[i].count == collision.count) return;
	}

	// �V��������
	Enemy* enemy = ENEMIES.GetEnemyById(collision.enemy_id);
	if (enemy)
	{
		enemy->OnDamage(collision);
	}
}