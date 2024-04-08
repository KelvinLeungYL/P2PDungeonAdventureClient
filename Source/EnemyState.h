#pragma once

#include "BaseState.h"
#include "Enemy.h"

namespace EnemyState
{
	enum ID {
		Idle,
		TargetFound,

		Hurt,
		Death,

		End,
	};

	// �ҋ@�X�e�[�g�I�u�W�F�N�g
	class IdleState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		IdleState(Enemy* enemy, float waitTime = 2.0f) : HierarchicalState<Enemy>(enemy), waitTime(waitTime) {};
		// �f�X�g���N�^
		~IdleState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		float waitTime;
		float waitTimer = 0.0f;
	};

	// �ړ��X�e�[�g
	class MoveState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		MoveState(Enemy* enemy, DirectX::XMFLOAT3 position, int nextState = EnemyState::ID::Idle) : HierarchicalState<Enemy>(enemy), position(position), nextState(nextState) {};
		// �f�X�g���N�^
		~MoveState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		DirectX::XMFLOAT3 position;
		int nextState;
	};

	// �ǐՃX�e�[�g
	class FollowState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		FollowState(Enemy* enemy, float distance, int nextState = EnemyState::ID::Idle) : HierarchicalState<Enemy>(enemy), distance(distance), nextState(nextState) {};
		// �f�X�g���N�^
		~FollowState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		float distance;
		int nextState;
	};

	class HurtState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		HurtState(Enemy* enemy) : HierarchicalState<Enemy>(enemy) {};
		// �f�X�g���N�^
		~HurtState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	class DeathState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		DeathState(Enemy* enemy) : HierarchicalState<Enemy>(enemy) {};
		// �f�X�g���N�^
		~DeathState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
}