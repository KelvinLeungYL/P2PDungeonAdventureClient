#pragma once

#include "BaseState.h"

#include "SceneGame.h"
#include "WidgetText.h"

namespace SceneGameState
{
	// �ڑ��҂��X�e�[�g
	class WaitingState : public HierarchicalState<SceneGame>
	{
	public:
		// �R���X�g���N�^
		WaitingState(SceneGame* scene) : HierarchicalState<SceneGame>(scene) {};
		// �f�X�g���N�^
		~WaitingState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		WidgetText* connecting;
		WidgetText* connectionCount;
		WidgetText* description;
		WidgetText* charaDescription;
	};
	// ���������X�e�[�g�i�J�E���g�_�E���j
	class ReadyState : public HierarchicalState<SceneGame>
	{
	public:
		// �R���X�g���N�^
		ReadyState(SceneGame* scene) : HierarchicalState<SceneGame>(scene) {};
		// �f�X�g���N�^
		~ReadyState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		float startTimer;
		WidgetText* countDownText;
	};
	// �Q�[���X�e�[�g
	class GameState : public HierarchicalState<SceneGame>
	{
	public:
		// �R���X�g���N�^
		GameState(SceneGame* scene) : HierarchicalState<SceneGame>(scene) {};
		// �f�X�g���N�^
		~GameState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};
	// �Q�[���I�[�o�[�X�e�[�g
	class GameOverState : public HierarchicalState<SceneGame>
	{
	public:
		// �R���X�g���N�^
		GameOverState(SceneGame* scene) : HierarchicalState<SceneGame>(scene) {};
		// �f�X�g���N�^
		~GameOverState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		float timer = 3.0f;
	};
	// ����
	class WinState : public HierarchicalState<SceneGame>
	{
	public:
		// �R���X�g���N�^
		WinState(SceneGame* scene) : HierarchicalState<SceneGame>(scene) {};
		// �f�X�g���N�^
		~WinState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		float timer = 5.0f;
	};
}
