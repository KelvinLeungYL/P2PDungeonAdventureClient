#pragma once

#include <DirectXMath.h>
#include "Manager.h"
#include "Singleton.h"
#include "Enemy.h"

/*
�N���C�A���g�̏Փ˔���X�g�b�N
���ʂ��z�X�g�ɑ��M����
*/

class CollisionDataManager : public Manager<ENEMY_COLLISION>, public Singleton<CollisionDataManager>
{
	friend class Singleton<CollisionDataManager>;
protected:
	CollisionDataManager() = default;
	~CollisionDataManager() = default;

public:
	ENEMY_COLLISION* Register(ENEMY_COLLISION* item) override;
	ENEMY_COLLISION* Register(Enemy* enemy, int colider_id, int damage, bool power, DirectX::XMFLOAT3 force = {});
	void RemoveById(int id);
private:
	static int CountId;
};

#define ENEMYCOLLISIONS CollisionDataManager::Instance()