#pragma once
#include "BaseState.h"
#include "PlayerState.h"

namespace KnightState
{
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

	// �������グ
	class AttackSpecialStateStart : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackSpecialStateStart(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackSpecialStateStart() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};
	// �������i���[�v�j
	class AttackSpecialStateBlocking : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackSpecialStateBlocking(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackSpecialStateBlocking() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};
	// ���U��
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
		void Exit() override;
	};
	// ����
	class AttackSpecialStateHit : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackSpecialStateHit(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackSpecialStateHit() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};

	// �X�L��_1 �ːi
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
	};
}