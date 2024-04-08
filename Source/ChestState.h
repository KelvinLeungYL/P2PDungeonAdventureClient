#pragma once
#include "BaseState.h"
#include "Chest.h"

namespace ChestState
{
	// �W��
	class ChestCloseState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		ChestCloseState(Enemy* enemy) : HierarchicalState<Enemy>(enemy) {};
		// �f�X�g���N�^
		~ChestCloseState() {};
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override {};
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override {};
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	};
	// �W�J��
	class ChestOpenState : public HierarchicalState<Enemy>
	{
	public:
		// �R���X�g���N�^
		ChestOpenState(Enemy* enemy, Model::Node* coverNode, std::function<void(Enemy*)> OnOpen = nullptr) : HierarchicalState<Enemy>(enemy), coverNode(coverNode), OnOpen(OnOpen) {};
		// �f�X�g���N�^
		~ChestOpenState() {};
		// �X�e�[�g�ɓ��������̃��\�b�h
		void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override {};
	protected:
		Model::Node* coverNode; // �W�m�[�h
		std::function<void(Enemy*)> OnOpen = nullptr;
		float timer;
		float openAngle = DirectX::XMConvertToRadians(-120.0f);
	};
}
