#include "Grenade.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "XMFLOAT.h"

Grenade::Grenade(Character* owner) : Projectile("Data/Model/Object/smokebomb.gltf", 1.0f, owner)
{
	collisionTarget |= COLLISION_TARGET::ENEMY | COLLISION_TARGET::STAGE;

	atk = 0; // ���ڍU�����Ȃ�
	speed = 30.0f;
	gravity = -1.0f;
	front = { 1.0f, 0.0f, 0.0f };

	power = true;
	force = 30.0f;

	// �Փ˔���
	SetCollider(Collider::COLLIDER_TYPE::SPHERE);
	collider->SetScale({ 0.4f, 0.4f, 0.4f });
}

void Grenade::UpdateColliders()
{
	collider->SetPosition(GetNodePosition(DirectX::XMFLOAT3{ 0.0f, 0.2f, 0.0f }));
}

void Grenade::OnDestory()
{
	// ����
	atk = 20;
	float range = 4.0f; // �͈�

	EFFECTS.GetEffect(EffectManager::EFFECT_IDX::BOMB_EFFECT)->Play(position, 1.0f);
	for (Enemy*& enemy : ENEMIES.GetAll())
	{
		for (const std::pair<int, Collider*>& collider : enemy->GetColliders())
		{
			direction = collider.second->GetPosition() - position;
			if (XMFLOAT3LengthSq(direction) < powf(range + collider.second->GetScale().x, 2.0f))
			{
				direction = XMFLOAT3Normalize(direction);
				SendCollision(enemy, collider.first);
				break;
			}
		}
	}
}