#include "Arrow.h"
#include "CollisionDataManager.h"
#include "PlayerManager.h"
#include "EffectManager.h"
#include "EnemyManager.h"
#include "MapTileManager.h"

Arrow::Arrow(Character* owner) : Projectile("Data/Model/Object/arrow.gltf", 1.0f, owner)
{
	collisionTarget |= COLLISION_TARGET::ENEMY | COLLISION_TARGET::STAGE;

	atk = 7;
	speed = 40.0f;
	gravity = 0.0f;
	front = { 0.0f, -1.0f, 0.0f };
	pierce = false;

	force = 10.0f;
	coolTime = 0.1f;
}

void Arrow::Update(float elapsedTime)
{
	tempPosition = position; // 1�t���[���O�̈ʒu�X�V

	Projectile::Update(elapsedTime); // �ʒu�X�V

	timer -= elapsedTime;
	if (timer < 0.0f)
	{
		Destory();
	}
}

void Arrow::Collision()
{
	Collider* col = collider.get();
	for (ModelObject*& map : MAPTILES.GetAll())
	{
		HitResult hit;
		DirectX::XMFLOAT3 direction = {};
		if (map->GetCollider()->RayCast(tempPosition, position, hit))
		{
			OnHitGround(hit);
			break;
		}
	}

	Enemy* hitTarget = nullptr;
	int hitCollider = -1;
	HitResult hit;
	for (Enemy*& enemy : ENEMIES.GetAll())
	{
		for (std::pair<int, Collider*> enemyCollider : enemy->GetColliders())
		{
			HitResult temp;
			if (enemyCollider.second->RayCast(tempPosition, position, temp)) // ���C�Փ�
			{
				if (temp.distance < hit.distance) // ��Ԑڋߌv�Z
				{
					hit = temp;
					hitTarget = enemy;
					hitCollider = enemyCollider.first;
				}
				if (pierce) // �ђʏ����F���������G�S��
				{
					if (owner == PLAYERS.GetPlayerById(GAME_DATA.GetClientId())) // �N���C�A���g�̍U�������������Ȃ�
					{
						OnHitEnemy(hit);
						SendCollision(hitTarget, enemyCollider.first);
					}
				}
				continue; // ���̓G
			}
		}
	}
	if (hitTarget != nullptr && !pierce) // ��ђʏ����F�Փ˂̈�ԋ߂��ڕW
	{
		if (owner == PLAYERS.GetPlayerById(GAME_DATA.GetClientId())) // �N���C�A���g�̍U�������������Ȃ�
		{
			OnHitEnemy(hit);
			SendCollision(hitTarget, hitCollider);
		}
		Destory();
	}
}

void Arrow::OnHitEnemy(HitResult& hit)
{
	EFFECTS.GetEffect(EffectManager::EFFECT_IDX::HIT_SHOT_EFFECT)->Play(hit.position, 0.5f);
}