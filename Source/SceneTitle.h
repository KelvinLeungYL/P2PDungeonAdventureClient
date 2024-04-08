#pragma once

#include <unordered_set>
#include "Scene.h"
#include "ModelObject.h"
#include "FreeCameraController.h"
#include "StateMachine.h"

class SceneTitle : public Scene
{
public:
	SceneTitle() = default;
	~SceneTitle() = default;

	// 初期化
	void Initialize() override;
	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;
	// 描画処理
	void Render() override;

	StateMachine<SceneTitle>* GetStateMachine() { return stateMachine.get(); }
private:
	// シーンGUI描画
	void DrawSceneGUI();
public:
	enum STATE
	{
		TITLE,
		LOGIN,
		INPUT,
		SETTING,
		LOGIN_CHECK,
	};
private:
	std::unique_ptr<StateMachine<SceneTitle>> stateMachine;

	Camera& camera = Camera::Instance();
	std::unique_ptr<FreeCameraController> cameraController;

	// Sprite Preload
	std::unordered_set<const char*> spriteList = {
		"",											// マスク
		// Setting UI
		"Data/Sprites/bar_ready.png",
		"Data/Sprites/button_frame.png",
		"Data/Sprites/button_ready_on.png",
		"Data/Sprites/button2_ready_on.png",
		"Data/Sprites/button_agree.png",
		"Data/Sprites/button3_ready.png",
		"Data/Sprites/button_cancel.png",
		"Data/Sprites/gear.png",
		"Data/Sprites/barmid_ready.png",
		"Data/Sprites/gem.png",
	};
	std::unordered_set<std::shared_ptr<Sprite>> spritePreLoad;

	// 背景用モデル
	std::unique_ptr<ModelObject> map;
	std::unique_ptr<ModelObject> knight;
	std::unique_ptr<ModelObject> rouge;
	std::unique_ptr<ModelObject> barbarian;
};
