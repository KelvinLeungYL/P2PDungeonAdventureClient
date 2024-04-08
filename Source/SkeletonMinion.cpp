#include "SkeletonMinion.h"
#include "SphereCollider.h"
#include "SkeletonMinionState.h"

#include "UIManager.h"
#include "WidgetBossHp.h"

SkeletonMinion::SkeletonMinion(float scaling) : Enemy("Data/Model/Enemy/character_skeleton_minion.gltf", scaling)
{
	enemyType = ENEMY_TYPE::SKELETON_MINION;
	maxHp = hp = 50;
	atk = 10;
	moveSpeed = 2.0f;
	turnSpeed = DirectX::XMConvertToRadians(180);

	// �����蔻��
	colliders[HitCollider::Body] = new SphereCollider(scaling * 0.4f);
	colliders[HitCollider::Head] = new SphereCollider(scaling * 0.5f);
	// �U������
	attackColliders[AttackCollider::LeftHand] = new SphereCollider(scaling * 0.2f);
	attackColliders[AttackCollider::RightHand] = new SphereCollider(scaling * 0.2f);
	EnableAttackColliders(false);

	stateMachine->RegisterState(EnemyState::ID::TargetFound, new EnemyState::FollowState(this, 2.0f, SkeletonMinion::State::Attack));
	stateMachine->RegisterState(SkeletonMinion::State::Attack, new SkeletonMinionState::AttackState(this));
	stateMachine->SetState(EnemyState::Idle);
}

// ��ԋ߂��v���C���[���^�[�Q�b�g
void SkeletonMinion::UpdateTarget()
{
	target = GetClosestPlayer(24.0f);
}

// �Փ˔���X�V
void SkeletonMinion::UpdateColliders()
{
	// �����蔻��
	colliders[HitCollider::Body]->SetPosition(GetNodePosition("character_skeleton_minion_body", DirectX::XMFLOAT3{ 0.0f, 0.3f, 0.0f } *scale));
	colliders[HitCollider::Head]->SetPosition(GetNodePosition("character_skeleton_minion_head", DirectX::XMFLOAT3{ 0.0f, 0.4f, 0.0f } *scale));

	// �U������
	attackColliders[AttackCollider::LeftHand]->SetPosition(GetNodePosition("character_skeleton_minion_armLeft", DirectX::XMFLOAT3{ 0.25f, -0.45f, 0.0f } *scale));
	attackColliders[AttackCollider::RightHand]->SetPosition(GetNodePosition("character_skeleton_minion_armRight", DirectX::XMFLOAT3{ -0.25f, -0.45f, 0.0f } *scale));
}

SkeletonMinionBoss::SkeletonMinionBoss() : SkeletonMinion(3.0f)
{
	enemyType = ENEMY_TYPE::SKELETON_MINION_BOSS;
	maxHp *= 10;
	hp = maxHp;
	atk = 20;
	moveSpeed = 3.0f;
	turnSpeed = DirectX::XMConvertToRadians(90);

	showHp = true;

	// �X�[�p�[�A�[�}�[
	armorMaxHp = armorHp = 50;

	stateMachine->RegisterState(EnemyState::ID::Idle, new EnemyState::IdleState(this, 1.0f));
	stateMachine->RegisterState(EnemyState::ID::TargetFound, new EnemyState::FollowState(this, 3.0f, SkeletonMinion::State::Attack));
	stateMachine->SetState(EnemyState::ID::Idle);

	// HP�Q�[�W
	UI.Register(new WidgetBossHp("�X�P���h��", this));
}
void SkeletonMinionBoss::Update(float elaspedTime)
{
	if (armorHp <= 0) // �A�[�}�[�u���C�N
	{
		recoverArmorTimer -= elaspedTime;
		if (recoverArmorTimer <= 0.0f)
		{
			recoverArmorTimer = 0.0f;
			armorHp = armorMaxHp;		// �A�[�}�[��
		}
	}

	Enemy::Update(elaspedTime);
}
void SkeletonMinionBoss::OnDamage(const ENEMY_COLLISION& hit)
{
	hp -= hit.damage;

	if (hp > 0)
	{
		if (armorHp <= 0) { // �A�[�}�[�Ȃ�
			stateMachine->ChangeState(EnemyState::ID::Hurt);
			hp -= hit.damage / 10 * 2;		// �_�E���ǉ��_���[�W
		}
		else if (hit.colider_id == HitCollider::Head)	// �w�b�h�V���b�g �A�[�}�[����
		{
			armorHp -= hit.damage;
			if (armorHp <= 0) {		// �A�[�}�[����
				armorHp = 0;
				recoverArmorTimer = recoverArmorTime;	// �A�[�}�[�񕜃^�C�}�[
			}
		}
		velocity += hit.force * 0.1f;
	}

	if (hp <= 0)
	{
		stateMachine->ChangeState(EnemyState::ID::Death);
	}
}