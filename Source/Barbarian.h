#pragma once
#include "Player.h"
#include "BarbarianState.h"

class Barbarian : public Player
{
public:
	Barbarian();
	~Barbarian() = default;

	void AttackCollision() override;
	DirectX::XMFLOAT3 GetShotPosition() override;
private:

	void UpdateColliders() override;
public:
	enum AttackCollider
	{
		Axe
	};

	// ��ʍU���i����A�b�N�X�j
	enum ATTACK_STATE {
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
	};

	// ����U���i�����A�b�N�X�j
	enum THROW_STATE {
		THROW_AIM,
		THROW_ATTACK,
	};

	// �X�L��1 (��)
	enum SKILL_1_STATE {
		SKILL_1_START,
		SKILL_1_LOOP
	};
};
