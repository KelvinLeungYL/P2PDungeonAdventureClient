#pragma once

#include <memory>
#include <thread>
#include "Camera.h"
#include "FreeCameraController.h"
#include "Graphics/LightManager.h"
#include "Graphics/Sprite.h"
#include "Graphics/Model.h"
#include "Graphics/PostprocessingRenderer.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	// ������
	virtual void Initialize() = 0;
	// �I����
	virtual void Finalize() = 0;

	// �X�V����
	virtual void Update(float elapsedTime) {};
	// �`�揈��
	virtual void Render() {};

	// �����m�F
	bool IsReady() const { return ready; }
	// ��������
	void SetReady() { ready = true; }

protected:
	bool ready = false;
};

// ���f���e�X�g�V�[��
class ModelTestScene : public Scene
{
public:
	ModelTestScene() {};
	~ModelTestScene() override {};

	// ������
	void Initialize() override;
	// �I����
	void Finalize() override {};

	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render() override;
private:

	Camera& camera = Camera::Instance();
	// �V�[��GUI�`��
	void DrawSceneGUI();
	// �v���p�e�BGUI�`��
	void DrawPropertyGUI();
	// �A�j���[�V����GUI�`��
	void DrawAnimationGUI();

	std::unique_ptr<Model> model;

	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };

	Model::Node* selectionNode = nullptr;

	FreeCameraController cameraController;

	bool animationLoop = false;
	float animationBlendSeconds = 0;

	std::unique_ptr<PostprocessingRenderer>	postprocessingRenderer; //	�|�X�g�v���Z�X
};

// ���[�h�V�[��
class SceneLoading : public Scene
{
public:
	SceneLoading(Scene* nextScene) : nextScene(nextScene) {}
	~SceneLoading() override {}
	// ������
	void Initialize() override;
	// �I����
	void Finalize() override;
	// �X�V����
	void Update(float elapsedTime) override;
	// �`�揈��
	void Render() override;
private:
	// ���[�f�B���O�X���b�h
	static void LoadingThread(SceneLoading* scene);
private:
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;
};
