#include "EnemyState.h"

// �ҋ@�X�e�[�g
void EnemyState::IdleState::Enter()
{
	owner->GetModel()->PlayAnimation(Enemy::Animation::Idle, true);
	waitTimer = waitTime;
}
void EnemyState::IdleState::Execute(float elapsedTime)
{
	owner->UpdateTarget();
	waitTimer -= elapsedTime;

	if (owner->GetTarget() != nullptr)
	{
		owner->TurnTo(elapsedTime, owner->GetTarget()->GetPosition());
		if (waitTimer <= 0.0f)
		{
			owner->GetStateMachine()->ChangeState(EnemyState::ID::TargetFound);
		}
	}
}
void EnemyState::IdleState::Exit()
{
}

// �ړ��X�e�[�g
void EnemyState::MoveState::Enter()
{
	owner->GetModel()->PlayAnimation(Enemy::Animation::Walk, true);
}
void EnemyState::MoveState::Execute(float elapsedTime)
{
	if (owner->MoveTo(elapsedTime, position))
	{
		owner->GetStateMachine()->ChangeState(nextState);
	}
}
void EnemyState::MoveState::Exit()
{
}

// �ǐՃX�e�[�g
void EnemyState::FollowState::Enter()
{
	owner->GetModel()->PlayAnimation(Enemy::Animation::Walk, true);
}
void EnemyState::FollowState::Execute(float elapsedTime)
{
	Player* target = owner->GetTarget();
	if (!target)
	{
		owner->GetStateMachine()->ChangeState(EnemyState::ID::Idle);
		return;
	}

	owner->MoveTo(elapsedTime, owner->GetTarget()->GetPosition());

	DirectX::XMFLOAT3 diff = owner->GetTarget()->GetPosition() - owner->GetPosition();
	diff.y = 0; // Y������
	if (XMFLOAT3LengthSq(diff) < distance * distance)
	{
		owner->GetStateMachine()->ChangeState(nextState);
	}
}
void EnemyState::FollowState::Exit()
{
}

// ����X�e�[�g
void EnemyState::HurtState::Enter()
{
	owner->SetAnimationSpeed(3.0f);
	owner->GetModel()->PlayAnimation(Enemy::Animation::Dash_Back, false);
}
void EnemyState::HurtState::Execute(float elapsedTime)
{
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->GetStateMachine()->ChangeState(EnemyState::ID::Idle);
	}
}
void EnemyState::HurtState::Exit()
{
	owner->SetAnimationSpeed(1.0f);
}

// ���S�X�e�[�g
void EnemyState::DeathState::Enter()
{
	owner->GetModel()->PlayAnimation(Enemy::Animation::Defeat, false);
	for (std::pair<int, Collider*> collider : owner->GetColliders()) collider.second->SetEnable(false);
}
void EnemyState::DeathState::Execute(float elapsedTime)
{
	if (!owner->GetModel()->IsPlayAnimation())
	{
		owner->OnDeath();
	}
}
void EnemyState::DeathState::Exit()
{
}