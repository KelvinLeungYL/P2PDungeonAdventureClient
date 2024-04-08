#include <fstream>
#include "GameData.h"
#include "url.h"

GameData::GameData()
{
	if (!LoadGameSetting()) // ���[�h���s�E�V�����t�@�C��
	{
		SaveGameSetting();
	}
}

bool GameData::SaveGameSetting()
{
	std::ofstream ofs;
	ofs.open("setting.bin", std::ios::binary);
	if (!ofs)	// �t�@�C�����s
	{
		return false;
	}
	ofs.write((const char*)&gameSetting, sizeof(GameData::GAME_SETTING));
	ofs.close();

	return true;
}
bool GameData::LoadGameSetting()
{
	std::ifstream ifs;
	ifs.open("setting.bin", std::ios::binary);
	if (!ifs) { // �I�[�v�����s��
		gameSetting = {}; // �f�t�H���g�f�[�^
		return false;
	}

	ifs.read((char*)&gameSetting, sizeof(GameData::GAME_SETTING));
	ifs.close();

	return true;
}

void GameData::Logout()
{
	userData = {};
	memset(GAME_DATA.GetSetting().token, 0, sizeof(GAME_DATA.GetSetting().token));
	SaveGameSetting();
}

void GameData::CheckServerExit()
{
}