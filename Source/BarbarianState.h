#pragma once
#include "BaseState.h"
#include "PlayerState.h"

namespace BarbarianState
{
	// �ҋ@�X�e�[�g�I�u�W�F�N�g
	class IdleState : public PlayerState::IdleState
	{
	public:
		// �R���X�g���N�^
		IdleState(Player* player) : PlayerState::IdleState(player) {};
		// �f�X�g���N�^
		~IdleState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
	};

	// �ҋ@�p�X�e�[�g�I�u�W�F�N�g
	class WaitState : public PlayerState::WaitState
	{
	public:
		// �R���X�g���N�^
		WaitState(Player* player) : PlayerState::WaitState(player) {};
		// �f�X�g���N�^
		~WaitState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
	};

	// ��ʍU��
	class AttackNormalState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackNormalState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackNormalState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
	// ��ʍU��1
	class AttackNormalState_1 : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackNormalState_1(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackNormalState_1() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};
	// ��ʍU��2
	class AttackNormalState_2 : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackNormalState_2(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackNormalState_2() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};
	// ��ʍU��3
	class AttackNormalState_3 : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackNormalState_3(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackNormalState_3() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};

	// ����U��
	class AttackSpecialState : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackSpecialState(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackSpecialState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// ����U���G�C��
	class AttackSpecialStateAim : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackSpecialStateAim(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackSpecialStateAim() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
	// ����U������
	class AttackSpecialStateAttack : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackSpecialStateAttack(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackSpecialStateAttack() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	private:
		bool isShot = false;
	};

	// �X�L��_1 ��
	class Skill1State : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		Skill1State(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~Skill1State() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		float cacheTurnSpeed = 0.0f;
	};
	class Skill1StateStart : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		Skill1StateStart(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~Skill1StateStart() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};
	class Skill1StateLoop : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		Skill1StateLoop(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~Skill1StateLoop() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
}