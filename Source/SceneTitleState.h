#pragma once

#include "Network/HttpRequest.h"
#include "BaseState.h"
#include "SceneTitle.h"
#include "WidgetSettingWindow.h"
#include "WidgetRoomOption.h"
#include "WidgetLogin.h"

namespace SceneTitleState
{
	// タイトルステート
	class TitleState : public HierarchicalState<SceneTitle>
	{
	public:
		// コンストラクタ
		TitleState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// デストラクタ
		~TitleState() {}
		// ステートに入った時のメソッド
		virtual void Enter() override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime) override;
		// ステートから出ていくときのメソッド
		void Exit() override;
	private:
		float timer = 0.0f;

		WidgetText* title;
		WidgetText* click;
		WidgetImage* mask;
	};

	// ログインステート
	class LoginState : public HierarchicalState<SceneTitle>
	{
	public:
		// コンストラクタ
		LoginState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// デストラクタ
		~LoginState() {}
		// ステートに入った時のメソッド
		virtual void Enter() override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime) override;
		// ステートから出ていくときのメソッド
		void Exit() override;
	private:
		HttpRequest* xhr = nullptr;
		WidgetLogin* widgetLogin = nullptr;
	};

	// ログイン確認ステート
	class LoginCheckState : public HierarchicalState<SceneTitle>
	{
	public:
		// コンストラクタ
		LoginCheckState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// デストラクタ
		~LoginCheckState() {}
		// ステートに入った時のメソッド
		virtual void Enter() override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime) override;
		// ステートから出ていくときのメソッド
		void Exit() override;
	private:
		HttpRequest* xhr = nullptr;
	};

	// インプットステート
	class InputState : public HierarchicalState<SceneTitle>
	{
	public:
		// コンストラクタ
		InputState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// デストラクタ
		~InputState() {}
		// ステートに入った時のメソッド
		virtual void Enter() override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime) override;
		// ステートから出ていくときのメソッド
		void Exit() override;
	};

	// 設定ステート
	class SettingState : public HierarchicalState<SceneTitle>
	{
	public:
		// コンストラクタ
		SettingState(SceneTitle* scene) : HierarchicalState<SceneTitle>(scene) {};
		// デストラクタ
		~SettingState() {}
		// ステートに入った時のメソッド
		virtual void Enter() override;
		// ステートで実行するメソッド
		void Execute(float elapsedTime) override;
		// ステートから出ていくときのメソッド
		void Exit() override;
	private:
		WidgetSettingWindow* settingWindow = nullptr;
	};
}