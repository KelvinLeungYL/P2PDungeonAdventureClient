#pragma once
#include <string>

#include "Singleton.h"
#include "CameraController.h"

class GameData : public Singleton<GameData>
{
	friend class Singleton <GameData>;
private:
	GameData();
	~GameData() = default;
public:
	void SetClientId(int id) { client_id = id; }
	int GetClientId() { return client_id; }
	const char* GetName() { return name.c_str(); }

	void SetOnline(bool online) { this->online = online; }
	bool IsOnline() { return this->online; }

	void SetHost(bool host) { isHost = host; }
	void SetName(const char* name) { this->name = name; }
	bool IsHost() { return isHost; }

	bool SeverExist() { return serverExisted; }
	void CheckServerExit();
public:
	struct GameSetting
	{
		float CameraSensitivity = 5.0f;		// �J�������x
		bool CamerainvertX = false;			// �J����X���]
		bool CamerainvertY = false;			// �J����Y���]
		bool KeyboardInput = true;			// �L�[�{�[�h / �Q�[���p�b�h

		char token[64];					// ���O�C���g�[�N��
	};
	struct UserData							// ���O�C��
	{
		int id = -1;
		std::string useranme = "";
		std::string nickname = "";
		int gems = 0;
	};

	GameSetting& GetSetting() { return gameSetting; }
	UserData& GetUser() { return userData; }
	void UpdateSetting(const GameSetting& setting) { gameSetting = setting; };
	void UpdateUser(const UserData& user) { userData = user; };
	void Logout();
	bool SaveGameSetting();	// �ݒ�t�@�C���̏����o��(�ۑ�)
	std::string& GetIp() { return ip; }
	void SetIp(std::string ip) { this->ip = ip; }
	void SetIp(const char* ip) { this->ip = ip; }
private:
	bool LoadGameSetting();	// �ݒ�t�@�C���̓ǂݍ���
private:
	bool online = false;
	int client_id = -1;
	bool isHost = false;
	std::string name;
	std::string ip;

	bool serverExisted = false;

	GameSetting gameSetting = {};
	UserData userData = {};
};

#define GAME_DATA GameData::Instance()
#define GAME_SETTING GameData::Instance().GetSetting()