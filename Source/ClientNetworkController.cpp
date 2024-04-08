#include "NetworkController.h"

#include "Knight.h"
#include "Mage.h"
#include "Barbarian.h"
#include "Rogue.h"
#include "GameData.h"
#include <iostream>
#include "SceneManager.h"
#include "SceneTitle.h"
#include "CollisionDataManager.h"

Player* PlayerFactor(int playerClass)
{
	Player* player = nullptr;
	switch (playerClass)
	{
	case PLAYER_CLASS::KNIGHT:
		player = new Knight;
		break;
	case PLAYER_CLASS::MAGE:
		player = new Mage;
		break;

	case PLAYER_CLASS::BARBARIAN:
		player = new Barbarian;
		break;
	case PLAYER_CLASS::ROGUE:
		player = new Rogue;
		break;
	}
	return player;
}

bool ClientNetworkController::Initialize(const char* address, const char* port)
{
	// TCP�T�[�o�[�֐ڑ�
	if (!tcpClientSocket.Connect(address, port))
	{
		return false;
	}
	// UDP
	if (!udpClientSocket.Connect(address, port))
	{
		return false;
	}

	return true;
}
void ClientNetworkController::Finalize()
{
	tcpClientSocket.Disconnect();
	udpClientSocket.Disconnect();
}
void ClientNetworkController::SendDisconnection()
{
	// TCP���M
	if (!tcpClientSocket.IsConnecting()) return;
	int sendData = TCP_COMMAND::COMMAND_DISCONNECT_REQUEST;
	tcpClientSocket.Send(&sendData, sizeof(sendData));
}

void ClientNetworkController::UpdateConnection()
{
	if (!tcpClientSocket.IsConnecting()) return;

	// TCP��M
	TCP_SERVER_DATA recvData;
	if (tcpClientSocket.Receive(&recvData, sizeof(recvData)) > 0) // �f�[�^��M
	{
		// �S�̃f�[�^
		GAME_DATA.SetClientId(recvData.client_id);
		gameState = recvData.gameState;
		clientCount = recvData.clientCount;

		// ClientData
		for (int i = 0; i < MAX_CONNECTION; i++)
		{
			players[i].client_id = recvData.clients[i].client_id;

			players[i].gameState = recvData.clients[i].gameState;
			if (recvData.clients[i].client_id == recvData.client_id)
			{
				// ����
				players[i].classType = classType;
				players[i].colorIdx = colorIdx;
				strcpy_s(players[i].name, 64, GAME_DATA.GetName());
			}
			else
			{
				players[i].classType = recvData.clients[i].classType;
				players[i].colorIdx = recvData.clients[i].colorIdx;
				strcpy_s(players[i].name, 64, recvData.clients[i].name);
			}
		}
	}

	tcpSendTimer -= TentacleLib::timer.Delta(); // �������M
	if (tcpSendTimer < 0.0f)
	{
		tcpSendTimer = tcpSendTime;
		tcpSendFlag = true;
	}

	// TCP���M
	if (GAME_DATA.GetClientId() >= 0 && tcpSendFlag) // client
	{
		TCP_CONNECTION_DATA sendData = {
			GAME_DATA.GetClientId(),
			(isReady) ? GAME_STATE::READY : GAME_STATE::WAITING,
			classType,
			colorIdx,
		};
		strcpy_s(sendData.name, 64, GAME_DATA.GetName());
		if (tcpClientSocket.Send(&sendData, sizeof(sendData)) < 0)
		{
			// �T�[�o�[�ڑ����s
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}
		tcpSendFlag = false;
	}

	UpdatePlayers();
}
void ClientNetworkController::UpdatePlayers() // �N���C�A���g�L�����X�V
{
	for (int i = 0; i < clientCount; i++)
	{
		if (players[i].client_id < 0) {
			continue;
		}
		Player* player = nullptr;
		if (PLAYERS.Count() < (i + 1)) // ���݂��Ȃ�
		{
			player = PLAYERS.Register(PlayerFactor(players[i].classType));
		}
		else {
			player = PLAYERS.get(i);
			if (player->GetClassType() != players[i].classType)
			{
				player = PLAYERS.Replace(i, PlayerFactor(players[i].classType));
			}
		}
		player->SetClientId(players[i].client_id);
		player->SetColor(Player::GetColorSet(players[i].colorIdx));
		player->SetName(players[i].name);

		// ���������_���X
		if (players[i].gameState == GAME_STATE::READY && player->GetState() != static_cast<int>(Player::State::Ready))
		{
			player->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Ready));
		}
		// �����ҋ@
		if (players[i].gameState == GAME_STATE::WAITING && player->GetState() != static_cast<int>(Player::State::Waiting))
		{
			player->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Waiting));
		}
	}
	PLAYERS.Resize(clientCount);
}

void ClientNetworkController::Update()
{
	if (!udpClientSocket.IsConnecting()) return;

	CheckAlive();

	// UDP���M
	SendUDP();

	// UDP��M
	RecvUDP();
}
//
void ClientNetworkController::CheckAlive()
{
	bool tcpSendFlag = false;
	// TCP��M
	int recvData;
	if (tcpClientSocket.Receive(&recvData, sizeof(recvData)) > 0) // �f�[�^��M
	{
		switch (recvData)
		{
		case TCP_COMMAND::COMMAND_CONFIRM: break; // �����m�F�p�E�����Ȃ�
		case TCP_COMMAND::COMMAND_DISCONNECT_CONFIRM: // �ڑ����f�̃��N�G�X�g�̕Ԏ�
			// ���f�m�F
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
			return;
		}
	}

	// TCP���M
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

		if (tcpClientSocket.Send(&sendData, sizeof(sendData)) < 0) // �m�F
		{
			// ���M���s�A�ڑ����f
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
			return;
		}
	}
}

// UDP��M
void ClientNetworkController::RecvUDP()
{
	UDP_SERVER_DATA recvData;
	if (udpClientSocket.Receive(&recvData, sizeof(recvData)) > 0)
	{
		if (recvData.count > recvCount)
		{
			gameState = recvData.gameState;
			std::cout << "Receive DATA from Server: " << recvData.count << std::endl;
			// �V�����f�[�^
			for (int i = 0; i < MAX_CONNECTION; i++) // �v���C���[�f�[�^
			{
				PLAYER_DATA& data = recvData.playerData[i];
				if (data.client_id < 0) continue;

				if (data.client_id != GAME_DATA.GetClientId()) // �����ȊO
				{
					Player* player = PLAYERS.GetPlayerById(data.client_id);
					if (player != nullptr)
					{
						if (data.connected)
						{
							PLAYERS.GetPlayerById(data.client_id)->ImportData(data); // �f�[�^�X�V
						}
						else
						{
							// ���f�ڑ��ς�
							PLAYERS.Remove(player);
						}
					}
				}
				else // ����
				{
					// �����ςݏՓˌ��ʂ��폜
					for (int j = 0; j < MAX_COLLISION_RESULT; j++)
					{
						if (recvData.collisions[i][j].count < 0) break;
						ENEMYCOLLISIONS.RemoveById(recvData.collisions[i][j].count);
					}
				}
			}

			for (int i = 0; i < MAX_ENEMIES; i++) // �G�l�~�[�f�[�^�[
			{
				ENEMY_DATA& data = recvData.enemyData[i];
				if (data.enemy_id < 0) break; // �G���h�t���O
				// �G�l�~�[�X�V
				if (!isHost)
				{
					ENEMIES.UpdateOrCreate(data);
				}
			}

			recvCount = recvData.count;
		}
	}
}

// UDP���M
void ClientNetworkController::SendUDP()
{
	UDP_CONNECTION_DATA sendData;
	sendData.count = sendCount++;
	PLAYERS.GetPlayerById(GAME_DATA.GetClientId())->ExportData(sendData.playerData);
	for (int i = 0; i < min(MAX_COLLISION_RESULT, ENEMYCOLLISIONS.Count()); i++)
	{
		int count = ENEMYCOLLISIONS.Count();

		ENEMY_COLLISION* collision = ENEMYCOLLISIONS.get(i);
		sendData.collisions[i] = *collision;
	}
	udpClientSocket.Send(&sendData, sizeof(sendData));
}

void ClientNetworkController::Reset()
{
	recvCount = 0;
	for (int i = 0; i < MAX_CONNECTION; i++)
	{
		players[i].client_id = -1;
		players[i].classType = -1;
		players[i].colorIdx = -1;
		players[i].gameState = -1;
	}
}