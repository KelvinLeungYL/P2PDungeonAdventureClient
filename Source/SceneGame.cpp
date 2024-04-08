#include <locale.h>
#include <profiler.h>
#include <algorithm>

#include "tentacle_lib.h"
#include "Network/Network.h"
#include "SceneGame.h"
#include "GameData.h"

#include "SceneManager.h"
#include "SceneTitle.h"

#include "PlayerManager.h"

#include "MapTileManager.h"
#include "MapTile.h"

#include "TestingStage.h"

#include "ProjectileManager.h"

#include "UIManager.h"
#include "WidgetCrosshair.h"
#include "WidgetText.h"

#include "Collision.h"

#include "Dialog.h"

#include "SceneGameState.h"

#include "CollisionDataManager.h"

#include "StageManager.h"

#include "Encode.h"

#include "EffectManager.h"

SceneGame::SceneGame(const char* name, const char* host, const char* port, NetworkController* networkController)
{
	this->name = Encode::utf8_to_string((char8_t*)name);

	this->host = host;
	this->port = port;
	this->networkController = networkController;
};

void SceneGame::Initialize()
{
	ID3D11Device* device = T_GRAPHICS.GetDevice();
	float screenWidth = T_GRAPHICS.GetScreenWidth();
	float screenHeight = T_GRAPHICS.GetScreenHeight();

	// Sprite Resource Preload
	for (auto& filename : spriteList)
	{
		spritePreLoad.insert(RESOURCE.LoadSpriteResource(filename));
	}
	// Model Resource Preload
	for (auto& filename : modelList)
	{
		modelPreLoad.insert(RESOURCE.LoadModelResource(filename));
	}

	LightManager::Instance().SetAmbientColor({ 0, 0, 0, 0 });
	Light* dl = new Light(LightType::Directional);
	dl->SetDirection({ 0.0f, -0.503f, -0.864f });
	LightManager::Instance().Register(dl);

	// �J�����ݒ�
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),							// ��p
		T_GRAPHICS.GetScreenWidth() / T_GRAPHICS.GetScreenHeight(),	// ��ʃA�X�y�N�g��
		0.1f,														// �j�A�N���b�v
		10000.0f													// �t�@�[�N���b�v
	);
	camera.SetLookAt(
		{ 0, 5.0f, 10.0f },	// ���_
		{ 0, 0, 0 },	// �����_
		{ 0, 0.969f, -0.248f } // ��x�N�g��
	);
	cameraController = std::make_unique<ThridPersonCameraController>();
	cameraController->SyncCameraToController(camera);
	cameraController->SetEnable(false);

	// Network
	if (networkController != nullptr)
	{
		if (!networkController->Initialize(host.c_str(), port.c_str()))
		{
			SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
			return;
		}
		else
		{
			networkController->Reset();
			GAME_DATA.SetName(name.c_str());
		}
	}

	//�X�e�[�W
	stage = std::make_unique<TestingStage>();
	stage->Initialize();

	stateMachine = std::make_unique<StateMachine<SceneGame>>();
	stateMachine->RegisterState(GAME_STATE::WAITING, new SceneGameState::WaitingState(this));
	stateMachine->RegisterState(GAME_STATE::READY, new SceneGameState::ReadyState(this));
	stateMachine->RegisterState(GAME_STATE::GAME, new SceneGameState::GameState(this));
	stateMachine->RegisterState(GAME_STATE::GAME_OVER, new SceneGameState::GameOverState(this));
	stateMachine->RegisterState(GAME_STATE::WIN, new SceneGameState::WinState(this));
	stateMachine->SetState(GAME_STATE::WAITING);

	WidgetText* ip = new WidgetText(host.c_str(), 0.8f);
	ip->SetBorder(1);
	UI.Register(ip);

	menu = new WidgetMenu;
	UI.Register(menu);

#ifdef _DEBUG
	//T_CONSOLE.Open();
#endif
}

void SceneGame::Finalize()
{
	modelPreLoad.clear();
	spritePreLoad.clear();
	CURSOR_ON;
	PLAYERS.Clear();
	MAPTILES.Clear();
	ENEMIES.Clear();
	PROJECTILES.Clear();
	ENEMYCOLLISIONS.Clear();
	STAGES.Clear();
	UI.Clear();
	T_CONSOLE.Close();

	// ���f�ڑ�
	switch (stateMachine->GetStateIndex())
	{
	case GAME_STATE::GAME:
	case GAME_STATE::GAME_OVER:
	case GAME_STATE::WIN:
		networkController->SendDisconnection();
		break;
	}

	networkController->Finalize();
}

// �X�V����
void SceneGame::Update(float elapsedTime)
{
	if (stateMachine->GetStateIndex() != networkController->GetGameState())
	{
		stateMachine->ChangeState(networkController->GetGameState());
	}

	stateMachine->Update(elapsedTime);

	{
		ProfileScopedSection_2("Map", ImGuiControl::Profiler::Dark);
		// �}�b�v�X�V
		MAPTILES.Update(elapsedTime);
	}

	{
		ProfileScopedSection_2("Player", ImGuiControl::Profiler::Yellow);
		// �v���C���[�X�V
		PLAYERS.Update(elapsedTime);
	}

	{
		ProfileScopedSection_2("Enemy", ImGuiControl::Profiler::Red);
		// �G�l�~�[�X�V
		ENEMIES.Update(elapsedTime);
	}

	{
		ProfileScopedSection_2("Projectiles", ImGuiControl::Profiler::Purple);
		// ���˕��X�V
		PROJECTILES.Update(elapsedTime);
	}

	{
		ProfileScopedSection_2("Collision", ImGuiControl::Profiler::Yellow);
		// �I�u�W�F�N�g�ԏՓ˔���
		COLLISION.Update(elapsedTime);
	}

	{
		ProfileScopedSection_2("Stages", ImGuiControl::Profiler::Green);
		// �X�e�[�W�I�u�W�F�N�g�X�V
		STAGES.Update(elapsedTime);
	}

	{
		ProfileScopedSection_2("Effect", ImGuiControl::Profiler::Dark);
		// �G�t�F�N�g�X�V
		EFFECTS.Update(elapsedTime);
	}

	// �J�����X�V
	cameraController->Update(elapsedTime);
	cameraController->SyncContrllerToCamera(camera);

	{
		ProfileScopedSection_2("Effect", ImGuiControl::Profiler::Purple);

		// ���j���[
		if (T_INPUT.KeyDown(VK_ESCAPE) || T_INPUT.GamePadKeyDown(GAME_PAD_BTN::BACK))
		{
			if (menu->IsShow())
			{
				menu->Hide();
			}
			else
			{
				menu->Show();
			}
		}

		UI.MoveToEnd(menu);
		// �C���^�[�t�F�[�X�X�V
		UI.Update(elapsedTime);
	}
}

void SceneGame::UpdateConnection()
{
	ProfileScopedSection_2("Network", ImGuiControl::Profiler::Blue);
	if (T_INPUT.KeyDown(VK_SPACE) || T_INPUT.GamePadKeyDown(GAME_PAD_BTN::START))
	{
		networkController->ReadyTrigger();
	}
	if (!networkController->IsReady())
	{
		int classType = networkController->GetClassType();
		if (T_INPUT.KeyDown(VK_UP) || T_INPUT.GamePadKeyDown(GAME_PAD_BTN::DPAD_UP))
		{
			classType--;
		}
		if (T_INPUT.KeyDown(VK_DOWN) || T_INPUT.GamePadKeyDown(GAME_PAD_BTN::DPAD_DOWN))
		{
			classType++;
		}
		if (classType < 0) classType = PLAYER_CLASS::End - 1;
		if (classType >= PLAYER_CLASS::End) classType = 0;
		if (classType != networkController->GetClassType()) networkController->SetClassType(classType);

		int colorIdx = networkController->GetColorIndex();
		if (T_INPUT.KeyDown(VK_LEFT) || T_INPUT.GamePadKeyDown(GAME_PAD_BTN::DPAD_LEFT))
		{
			colorIdx--;
		}
		if (T_INPUT.KeyDown(VK_RIGHT) || T_INPUT.GamePadKeyDown(GAME_PAD_BTN::DPAD_RIGHT))
		{
			colorIdx++;
		}
		if (colorIdx < 0) colorIdx = Player::COLOR_PATTERN::END - 1;
		if (colorIdx >= Player::COLOR_PATTERN::END) colorIdx = Player::COLOR_PATTERN::DEFAULT;
		if (colorIdx != networkController->GetColorIndex()) networkController->SetColorIndex(colorIdx);
	}
	networkController->UpdateConnection();
	// �ʒu
	PLAYERS.InitializePosition();
}

bool SceneGame::UpdateGame(float elapsedTime)
{
	ProfileScopedSection_2("Network", ImGuiControl::Profiler::Blue);
	if (networkController->IsHost())
	{
		stage->Update(elapsedTime); // �z�X�g�E�G����

		if (stage->IsFinish())
		{
			networkController->SetGameState(GAME_STATE::WIN);
		}
		bool alive = false;
		for (Player*& player : PLAYERS.GetAll())
		{
			if (player->GetHp() > 0)
			{
				alive = true;
				break;
			}
		}
		if (!alive) networkController->SetGameState(GAME_STATE::GAME_OVER);
	}

	if (cameraController->isEnable())
	{
		T_INPUT.KeepCursorCenter();
	}

	networkController->Update();

	return true;
}

// �`�揈��
void SceneGame::Render()
{
	T_TEXT.Begin();
	T_GRAPHICS.GetFrameBuffer(FrameBufferId::Scene)->Clear(T_GRAPHICS.GetDeviceContext(), 0.2f, 0.2f, 0.2f, 1);
	T_GRAPHICS.GetFrameBuffer(FrameBufferId::Scene)->SetRenderTargets(T_GRAPHICS.GetDeviceContext());

	// �`��R���e�L�X�g�ݒ�
	RenderContext rc;
	rc.camera = &camera;
	rc.deviceContext = T_GRAPHICS.GetDeviceContext();
	rc.renderState = T_GRAPHICS.GetRenderState();

	// ���e�`��
	{
		// ���C�g�̏����l�ߍ���
		LightManager::Instance().PushRenderContext(rc);

		MAPTILES.Render(rc);			// �}�b�v
		PLAYERS.Render(rc);				// �v���C���[
		ENEMIES.Render(rc);				// �G�l�~�[
		PROJECTILES.Render(rc);			// ���˕�
		STAGES.Render(rc);				// �X�e�[�W�I�u�W�F�N�g
		EFFECTS.Render(camera.GetView(), camera.GetProjection()); // �G�t�F�N�g
	}
#ifdef _DEBUG
	{
		PROJECTILES.DrawDebugPrimitive();
	}
#endif // _DEBUG

	// ���C�������_���`����s
	T_GRAPHICS.GetLineRenderer()->Render(T_GRAPHICS.GetDeviceContext(), camera.GetView(), camera.GetProjection());
	// �f�o�b�O�����_���`����s
	T_GRAPHICS.GetDebugRenderer()->Render(T_GRAPHICS.GetDeviceContext(), camera.GetView(), camera.GetProjection());

	//	�|�X�g�v���Z�X�������s��
	{
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::Display)->Clear(T_GRAPHICS.GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::Display)->SetRenderTargets(T_GRAPHICS.GetDeviceContext());
		postprocessingRenderer->Render(T_GRAPHICS.GetDeviceContext());
	}

	UI.Render(rc);						// �C���^�[�t�F�[�X

	T_TEXT.End();

#ifdef _DEBUG
	ProfileDrawUI();
#endif // _DEBUG
}