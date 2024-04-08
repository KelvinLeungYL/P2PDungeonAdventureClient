#pragma once

#include "EnemyState.h"
#include "SkeletonMinion.h"

namespace SkeletonMinionState {
	// �U���X�e�[�g
	class AttackState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		AttackState(Enemy* enemy) : HierarchicalState<Enemy>(enemy) {};
		// �f�X�g���N�^
		~AttackState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
}
