#pragma once
#include "Player.h"
class Knight : public Player
{
public:
	Knight();
	~Knight() = default;

	void AttackCollision() override;
	void OnDamage(const HitResult& hit, int damage) override;
private:
	void UpdateColliders() override;
public:
	enum AttackCollider
	{
		Sword_1,
		Sword_2,
		Sword_3,
		Shield
	};

	// ��ʍU���i���j
	enum ATTACK_STATE {
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
	};

	// ����U���i���j
	enum BLOCK_STATE {
		BLOCK_START,
		BLOCK_ATTACK,
		BLOCKING,
		BLOCK_HIT
	};
};
