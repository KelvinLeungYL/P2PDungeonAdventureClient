#include "BarbarianState.h"
#include "Barbarian.h"
#include "Player.h"

#include "ProjectileManager.h"
#include "TwoHandedAxe.h"

#include "ThridPersonCameraController.h"

// �ҋ@�X�e�[�g
void BarbarianState::IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::Animation::TwoHanded_Melee_Idle, true, 0.1f);
}
void BarbarianState::WaitState::Enter()
{
	owner->GetModel()->PlayAnimation(Player::Animation::TwoHanded_Melee_Idle, true, 0.1f);
}

// ��ʍU���X�e�[�g
void BarbarianState::AttackNormalState::Enter()
{
	owner->SetAnimationSpeed(2.0f);
	SetSubState(Barbarian::ATTACK_STATE::ATTACK_1);
}
void BarbarianState::AttackNormalState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);

	// ���d��
	owner->StopFall();
	owner->StopMove();

	if (!owner->GetModel()->IsPlayAnimation()) // �U�����[�V�����I���
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}
void BarbarianState::AttackNormalState::Exit()
{
	owner->SetAnimationSpeed(1.0f);
}
//  ��ʍU��1
void BarbarianState::AttackNormalState_1::Enter()
{
	owner->GetModel()->PlayAnimation(Player::Animation::TwoHanded_Melee_Attack_Slice, false, 0.05f);
}
void BarbarianState::AttackNormalState_1::Execute(float elapsedTime)
{
	if (!owner->IsPlayer()) return;

	if (owner->GetModel()->GetAnimationRate() > 0.2f)
	{
		owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(true);
	}

	if (owner->GetModel()->GetAnimationRate() > 0.75f) // �A�j���[�V����75%����
	{
		owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(false);
		if (owner->InputAttackNormal()) {
			owner->GetStateMachine()->ChangeSubState(Barbarian::ATTACK_STATE::ATTACK_2);
		}
	}
}
//  ��ʍU��2
void BarbarianState::AttackNormalState_2::Enter()
{
	owner->GetModel()->PlayAnimation(Player::Animation::TwoHanded_Melee_Attack_Chop, false, 0.05f);
}
void BarbarianState::AttackNormalState_2::Execute(float elapsedTime)
{
	if (!owner->IsPlayer()) return;
	if (owner->GetModel()->GetAnimationRate() > 0.2f)
	{
		owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(true);
	}

	if (owner->GetModel()->GetAnimationRate() > 0.75f) // �A�j���[�V����75%����
	{
		owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(false);
		if (owner->InputAttackNormal()) {
			owner->GetStateMachine()->ChangeSubState(Barbarian::ATTACK_STATE::ATTACK_3);
		}
	}
}
//  ��ʍU��3
void BarbarianState::AttackNormalState_3::Enter()
{
	owner->GetModel()->PlayAnimation(Player::Animation::TwoHanded_Melee_Attack_Spin, false, 0.05f);
}
void BarbarianState::AttackNormalState_3::Execute(float elapsedTime)
{
	if (!owner->IsPlayer()) return;
	if (owner->GetModel()->GetAnimationRate() > 0.2f)
	{
		owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(true);
	}

	if (owner->GetModel()->GetAnimationRate() > 0.75f) // �A�j���[�V����75%����
	{
		owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(false);
	}
}

// ����U���X�e�[�g
void BarbarianState::AttackSpecialState::Enter()
{
	owner->SetAnimationSpeed(1.2f);
	SetSubState(Barbarian::THROW_STATE::THROW_AIM);
}
void BarbarianState::AttackSpecialState::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);

	if (!owner->GetModel()->IsPlayAnimation()) // �A�j���[�V�����I��
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}
void BarbarianState::AttackSpecialState::Exit()
{
	owner->SetAnimationSpeed(1.0f);
	owner->GetModel()->FindNode("2H_Axe")->visible = true;
	if (owner->IsPlayer()) TPSCamera.AimMode(false);	// AIM
}
void BarbarianState::AttackSpecialStateAim::Enter()
{
	owner->GetModel()->PlayAnimation(Player::Animation::Throw, false, 0.05f);
	if (owner->IsPlayer()) TPSCamera.AimMode(true);	// AIM
}
void BarbarianState::AttackSpecialStateAim::Execute(float elapsedTime)
{
	if (owner->GetModel()->GetAnimationRate() > 0.375f)
	{
		owner->GetModel()->SetAnimationRate(0.375f);
	}

	// ���@�I�����[
	if (!owner->IsPlayer()) return;

	owner->FaceToCamera();

	if (owner->ReleaseAttackSpecial()) // ����U���{�^�������[�X
	{
		owner->GetStateMachine()->ChangeSubState(Barbarian::THROW_ATTACK);
	}
}
void BarbarianState::AttackSpecialStateAim::Exit()
{
}

void BarbarianState::AttackSpecialStateAttack::Enter()
{
	isShot = false;
}
void BarbarianState::AttackSpecialStateAttack::Execute(float elapsedTime)
{
	if (owner->GetModel()->GetAnimationRate() > 0.4f && !isShot)
	{
		// ����
		Projectile* axe = PROJECTILES.Register(new TwoHandedAxe(owner));
		axe->SetPosition(owner->GetShotPosition());
		axe->PointTo(owner->GetTarget());

		DirectX::XMFLOAT3 direction = XMFLOAT3Normalize(owner->GetTarget() - owner->GetShotPosition());
		DirectX::XMVECTOR Direction = DirectX::XMLoadFloat3(&direction);
		Direction = DirectX::XMVector3Rotate(Direction, DirectX::XMQuaternionRotationAxis(DirectX::XMVector3Cross(Direction, DirectX::XMVectorSet(0, 1, 0, 0)), DirectX::XMConvertToRadians(15.0f)));
		DirectX::XMStoreFloat3(&direction, Direction);
		axe->SetDirection(direction);
		axe->SetOwner(owner);

		isShot = true;
		owner->GetModel()->FindNode("2H_Axe")->visible = false;
	}
}

// �X�L��_1�X�e�[�g
void BarbarianState::Skill1State::Enter()
{
	owner->SetAnimationSpeed(1.5f);
	SetSubState(Barbarian::SKILL_1_STATE::SKILL_1_START);
	cacheTurnSpeed = owner->GetTurnSpeed();
	owner->SetTurnSpeed(0);

	owner->SkillCost(static_cast<int>(Player::State::Skill_1));
}
void BarbarianState::Skill1State::Execute(float elapsedTime)
{
	subState->Execute(elapsedTime);

	if (owner->GetMp() <= 0.0f || owner->ReleaseSkill1())
	{
		owner->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));
	}
}
void BarbarianState::Skill1State::Exit()
{
	owner->SetAnimationSpeed(1.0f);
	owner->SetTurnSpeed(cacheTurnSpeed);
}
void BarbarianState::Skill1StateStart::Enter()
{
	owner->SetAnimation(Player::Animation::TwoHanded_Melee_Attack_Spin, false);
}
void BarbarianState::Skill1StateStart::Execute(float elapsedTime)
{
	if (owner->GetModel()->GetAnimationRate() > 0.15f)
	{
		owner->GetStateMachine()->ChangeSubState(Barbarian::SKILL_1_STATE::SKILL_1_LOOP);
	}
}
void BarbarianState::Skill1StateLoop::Enter()
{
	owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(true);
	owner->SetAnimation(Player::Animation::TwoHanded_Melee_Attack_Spinning, true);
}
void BarbarianState::Skill1StateLoop::Execute(float elapsedTime)
{
	owner->InputMove(elapsedTime);
	owner->ModifyMp(-elapsedTime * 15.0f);
}
void BarbarianState::Skill1StateLoop::Exit()
{
	owner->GetAttackCollider(Barbarian::AttackCollider::Axe)->SetEnable(false);
}