#pragma once
#include "BaseState.h"
#include "Player.h"

// FLAG
const uint32_t flag_Dodge = 1 << 0;
const uint32_t flag_Jump = 1 << 1;
const uint32_t flag_Move = 1 << 2;
const uint32_t flag_Stop = 1 << 3;
const uint32_t flag_Fall = 1 << 4;
const uint32_t flag_Ground = 1 << 5;
const uint32_t flag_AttackN = 1 << 6;
const uint32_t flag_AttackS = 1 << 7;
const uint32_t flag_Skill_1 = 1 << 8;
const uint32_t flag_Skill_2 = 1 << 9;
const uint32_t flag_Skill_3 = 1 << 10;
const uint32_t flag_Skill_4 = 1 << 11;

namespace PlayerState
{
	void PlayerTransition(Player* owner, uint32_t flags);

	// �ҋ@�X�e�[�g�I�u�W�F�N�g
	class IdleState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		IdleState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~IdleState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// �ړ��X�e�[�g�I�u�W�F�N�g
	class MoveState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		MoveState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~MoveState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// �W�����v�X�e�[�g�I�u�W�F�N�g
	class JumpState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		JumpState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~JumpState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// �����X�e�[�g�I�u�W�F�N�g
	class FallState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		FallState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~FallState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// ���n�X�e�[�g�I�u�W�F�N�g
	class LandState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		LandState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~LandState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// ����X�e�[�g�I�u�W�F�N�g
	class DodgeState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		DodgeState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~DodgeState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// ����X�e�[�g�I�u�W�F�N�g
	class HurtState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		HurtState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~HurtState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// ���S�X�e�[�g�I�u�W�F�N�g
	class DeathState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		DeathState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~DeathState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// �ҋ@�p�X�e�[�g�I�u�W�F�N�g
	class WaitState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		WaitState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~WaitState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// �ҋ@�p (��������)�X�e�[�g�I�u�W�F�N�g
	class ReadyState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		ReadyState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~ReadyState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
}