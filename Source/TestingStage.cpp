#include "TestingStage.h"

#include "MapTileManager.h"
#include "Map001.h"

#include "Enemy.h"
#include "EnemyManager.h"

#include "StageManager.h"
#include "ModelObject.h"
#include "Map001BossRoomTeleport.h"
#include "Map001Key.h"

void TestingStage::Initialize()
{
	Stage::Initialize(); // �f�t�H���g

	STAGES.SetStage(this);

	// ��
	MAPTILES.Register(new Map001);

	key = STAGES.Register(new Map001Key);
	bossRoomTeleport = STAGES.Register(new Map001BossRoomTeleport);

	enemyList.push_back({ PHASE::NORMAL, 0.0, ENEMY_TYPE::CHEST_EMPTY_SKELETON_MINION, { -40.0f, 0.0f, -15.0f }, { 0, DirectX::XMConvertToRadians(180), 0} });

	enemyList.push_back({ PHASE::NORMAL, 0.0, ENEMY_TYPE::SKELETON_MINION, { 0.0f, 20.0f, -12.0f } });
	enemyList.push_back({ PHASE::NORMAL, 0.0, ENEMY_TYPE::SKELETON_MINION, { -1.0f, 20.0f, -18.0f } });
	enemyList.push_back({ PHASE::NORMAL, 0.0, ENEMY_TYPE::SKELETON_MINION, { +1.0f, 20.0f, -22.0f } });
	enemyList.push_back({ PHASE::NORMAL, 0.0, ENEMY_TYPE::SKELETON_MINION, { +1.0f, 20.0f, -22.0f } });

	// KEY�̉��
	DirectX::XMFLOAT3 pos = key->GetPosition();
	float d = 5.0f;
	for (int i = 0; i < 4; i++)
	{
		enemyList.push_back({ PHASE::GET_KEY, 0.0, ENEMY_TYPE::SKELETON_MINION, {
			pos.x + (cosf(DirectX::XMConvertToRadians(90.0f * i)) * d),
			20.0f,
			pos.z + (sinf(DirectX::XMConvertToRadians(90.0f * i)) * d),
			}
			}
		);
	}

	float sec = 0.0f;
	for (int i = 0; i < 1; i++)
	{
		enemyList.push_back({ PHASE::BOSS, sec, ENEMY_TYPE::SKELETON_MINION, { 26.0f, 10.0f, -86.0f } });
		enemyList.push_back({ PHASE::BOSS, sec, ENEMY_TYPE::SKELETON_MINION, { 14.0f, 10.0f, -86.0f } });
		enemyList.push_back({ PHASE::BOSS, sec, ENEMY_TYPE::SKELETON_MINION, { 26.0f, 10.0f, -74.0f } });
		enemyList.push_back({ PHASE::BOSS, sec, ENEMY_TYPE::SKELETON_MINION, { 14.0f, 10.0f, -74.0f } });
		sec += 5.0f;
	}
	// BOSS
	enemyList.push_back({ PHASE::BOSS, 0.0, ENEMY_TYPE::SKELETON_MINION_BOSS, { 20.0f, 40.0f, -80.0f } });
	// ��
	enemyList.push_back({ PHASE::VICKORY, 0.0f, ENEMY_TYPE::CHEST_COIN, { 20.0f, 20.0f, -80.0f } });
}

void TestingStage::Update(float elapsedTime)
{
	Stage::Update(elapsedTime);

	if (phase == PHASE::BOSS && ENEMIES.Count() == 0) // �{�X�����G�S��
	{
		SetPhase(PHASE::VICKORY);
	}
}

void TestingStage::OnPhase()
{
	if (phase == PHASE::BOSS) // �{�X�o�����E�����G���폜
	{
		ENEMIES.Clear();
	}
}