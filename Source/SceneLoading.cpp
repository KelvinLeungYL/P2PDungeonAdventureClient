#include "tentacle_lib.h"
#include "Scene.h"
#include "SceneManager.h"

// ������
void SceneLoading::Initialize()
{
	thread = new std::thread(LoadingThread, this);
}
// �I����
void SceneLoading::Finalize()
{
	if (thread != nullptr)
	{
		thread->join();
		delete thread;
		thread = nullptr;
	}
}
// �X�V����
void SceneLoading::Update(float elapsedTime)
{
	if (nextScene->IsReady())
	{
		SceneManager::Instance().ChangeScene(nextScene);
	}
}
// �`�揈��
void SceneLoading::Render()
{
	// ��ʃN���A
	T_GRAPHICS.GetFrameBuffer(FrameBufferId::Display)->Clear(T_GRAPHICS.GetDeviceContext(), 0, 0, 0, 1);

	T_TEXT.Begin();

	T_TEXT.Render(
		FONT_ID::HGpop,
		"Now Loading...",
		T_GRAPHICS.GetScreenWidth(), T_GRAPHICS.GetScreenHeight(),
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f,
		FONT_ALIGN::BOTTOM_RIGHT
	);

	T_TEXT.End();
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	srand(static_cast <unsigned> (time(NULL)));

	// COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	// ���̃V�[���̏��������s��
	scene->nextScene->Initialize();

	// �X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	// ���̃V�[���̏��������ݒ�
	scene->nextScene->SetReady();
}