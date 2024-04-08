#pragma once
#include "BaseState.h"
#include "PlayerState.h"

namespace RogueState
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
	private:
		bool isShot = false;
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
	private:
		bool isShot = false;
	};
	// ����U��(�G�C��)
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
		void Exit() override {};
	};

	// ����U��(����)
	class AttackSpecialStateShot : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		AttackSpecialStateShot(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~AttackSpecialStateShot() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	private:
		bool isShot = false;
	};

	// �X�L��_1 ��֒e
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

	// �X�L��_1�G�C��
	class Skill1StateAim : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		Skill1StateAim(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~Skill1StateAim() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
	// �X�L��_1����
	class Skill1StateAttack : public HierarchicalState<Player>
	{
	public:
		// �R���X�g���N�^
		Skill1StateAttack(Player* player) : HierarchicalState<Player>(player) {};
		// �f�X�g���N�^
		~Skill1StateAttack() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	private:
		bool isShot = false;
	};
}