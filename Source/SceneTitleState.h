#pragma once

#include "Network/HttpRequest.h"
#include "BaseState.h"
#include "SceneTitle.h"
#include "WidgetSettingWindow.h"
#include "WidgetRoomOption.h"
#include "WidgetLogin.h"

namespace SceneTitleState
{
	// �^�C�g���X�e�[�g
	class TitleState : public HierarchicalState<SceneTitle>
	{
	public:
		// �R���X�g���N�^
		TitleState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// �f�X�g���N�^
		~TitleState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		float timer = 0.0f;

		WidgetText* title;
		WidgetText* click;
		WidgetImage* mask;
	};

	// ���O�C���X�e�[�g
	class LoginState : public HierarchicalState<SceneTitle>
	{
	public:
		// �R���X�g���N�^
		LoginState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// �f�X�g���N�^
		~LoginState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		HttpRequest* xhr = nullptr;
		WidgetLogin* widgetLogin = nullptr;
	};

	// ���O�C���m�F�X�e�[�g
	class LoginCheckState : public HierarchicalState<SceneTitle>
	{
	public:
		// �R���X�g���N�^
		LoginCheckState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// �f�X�g���N�^
		~LoginCheckState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		HttpRequest* xhr = nullptr;
	};

	// �C���v�b�g�X�e�[�g
	class InputState : public HierarchicalState<SceneTitle>
	{
	public:
		// �R���X�g���N�^
		InputState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// �f�X�g���N�^
		~InputState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	};

	// �ݒ�X�e�[�g
	class SettingState : public HierarchicalState<SceneTitle>
	{
	public:
		// �R���X�g���N�^
		SettingState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// �f�X�g���N�^
		~SettingState() {}
		// �X�e�[�g�ɓ��������̃��\�b�h
		virtual void Enter() override;
		// �X�e�[�g�Ŏ��s���郁�\�b�h
		void Execute(float elapsedTime) override;
		// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
		void Exit() override;
	private:
		WidgetSettingWindow* settingWindow = nullptr;
	};
}