#include "profiler.h"
#include "SceneGameState.h"

#include "UIManager.h"
#include "ThridPersonCameraController.h"

#include "WidgetCrosshair.h"
#include "WidgetPlayerHP.h"
#include "WidgetEnemiesHp.h"
#include "WidgetSkillTimer.h"

#include "SceneManager.h"
#include "SceneTitle.h"

#include "PlayerManager.h"
#include "GameData.h"

// �ڑ��҂��X�e�[�g
void SceneGameState::WaitingState::Enter()
{
	connecting = new WidgetText("�ڑ��҂��c�c");
	connecting->SetPosition({ 1.0f, SCREEN_H - 1.0f });
	connecting->SetScale(0.7f);
	connecting->SetBorder(1);
	connecting->SetAlign(FONT_ALIGN::BOTTOM_LEFT);
	UI.Register(connecting);

	connectionCount = new WidgetText;
	connectionCount->SetPosition({ SCREEN_W - 1.0f, 1.0f });
	connectionCount->SetScale(1.0f);
	connecting->SetBorder(1);
	connectionCount->SetAlign(FONT_ALIGN::TOP_RIGHT);
	UI.Register(connectionCount);

	description = new WidgetText("�����F�L�����N�^�[�I���@�����F�F�I���@�X�y�[�X�F��������\n�S��������������΃Q�[���J�n");
	description->SetPosition({ SCREEN_W * 0.5f, 200.0f });
	description->SetBorder(1);
	description->SetAlign(FONT_ALIGN::TOP);
	UI.Register(description);

	charaDescription = new WidgetText("");
	charaDescription->SetPosition({ SCREEN_W * 0.5f, SCREEN_H - 300.0f });
	charaDescription->SetBorder(1);
	charaDescription->SetAlign(FONT_ALIGN::TOP);
	UI.Register(charaDescription);
}
void SceneGameState::WaitingState::Execute(float elapsedTime)
{
	owner->UpdateConnection();

	std::string text = std::to_string(owner->GetNetworkController()->GetClientCount());
	text += "/" + std::to_string(MAX_CONNECTION);
	connectionCount->SetText(text.c_str());

	// �L�����Љ�
	Player* player = PLAYERS.GetPlayerById(GAME_DATA.GetClientId());
	if (player)
	{
		std::string d = "�����L�[�F�ړ��@�X�y�[�X�F�W�����v�@��CTRL�F���";
		switch (player->GetClassType())
		{
		case PLAYER_CLASS::BARBARIAN:
			d = "�o�[�o���A���F\n���N���b�N�F��ʍU���@�@�E�N���b�N�F������\n" + d;
			break;
		case PLAYER_CLASS::KNIGHT:
			d = "�i�C�g�F\n���N���b�N�F��ʍU���@�@�E�N���b�N�F�u���b�N\n" + d;
			break;
		case PLAYER_CLASS::ROGUE:
			d = "�����W���[�F\n���N���b�N�F��ʃV���[�g�@�@�E�N���b�N�F�G�C���A��\n" + d;
			break;
		}
		charaDescription->SetText(d.c_str());
	}
}
void SceneGameState::WaitingState::Exit()
{
	UI.Remove(connecting);
	UI.Remove(connectionCount);
	UI.Remove(description);
	UI.Remove(charaDescription);
}

// ���������X�e�[�g�i�J�E���g�_�E���j
void SceneGameState::ReadyState::Enter()
{
	// �J�E���g�_�E���^�C�}�[
	startTimer = START_TIME;

	// UI�\��
	countDownText = new WidgetText;
	countDownText->SetPosition({ SCREEN_W / 2.0f, SCREEN_H / 2.0f });
	countDownText->SetAlign(FONT_ALIGN::CENTER);
	countDownText->SetColor({ 1, 1, 1, 1 });
	countDownText->SetBorder(3);

	UI.Register(countDownText);
}
void SceneGameState::ReadyState::Execute(float elapsedTime)
{
	startTimer -= elapsedTime;
	if (startTimer <= 0) {
		startTimer = 0;
		owner->GetNetworkController()->GameStart();
	}
	owner->UpdateConnection();

	countDownText->SetText(std::to_string(static_cast<int>(ceil(startTimer))).c_str());
}
void SceneGameState::ReadyState::Exit()
{
	UI.Remove(countDownText);
}

// �Q�[���X�e�[�g
void SceneGameState::GameState::Enter()
{
	Player* player = PLAYERS.GetPlayerById(GAME_DATA.GetClientId());
	TPSCamera.SetPlayer(player);
	TPSCamera.SetEnable(true);
	player->GetStateMachine()->ChangeState(static_cast<int>(Player::State::Idle));

	CURSOR_OFF;

	UI.Register(new WidgetCrosshair);
	UI.Register(new WidgetEnemiesHp);
	UI.Register(new WidgetPlayerHP);
	UI.Register(new WidgetSkillTimer);
}
void SceneGameState::GameState::Execute(float elapsedTime)
{
	owner->UpdateGame(elapsedTime);
}
void SceneGameState::GameState::Exit()
{
}

// �Q�[���I�[�o�[�X�e�[�g
void SceneGameState::GameOverState::Enter()
{
	UI.Clear();
	WidgetText* text = new WidgetText("GAME OVER");
	text->SetPosition({ SCREEN_W * 0.5f, SCREEN_H * 0.5f });
	text->SetScale(2.0f);
	text->SetAlign(FONT_ALIGN::CENTER);
	text->SetBorder(2);
	text->SetBorderColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	UI.Register(text);
	timer = 3.0f;
}
void SceneGameState::GameOverState::Execute(float elapsedTime)
{
	timer -= elapsedTime;

	if (timer < 0.0f) SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
}
void SceneGameState::GameOverState::Exit()
{
}

// �Q�[���I�[�o�[�X�e�[�g
void SceneGameState::WinState::Enter()
{
	UI.Clear();
	WidgetText* text = new WidgetText("VICTORY");
	text->SetPosition({ SCREEN_W * 0.5f, SCREEN_H * 0.5f });
	text->SetScale(2.0f);
	text->SetAlign(FONT_ALIGN::CENTER);
	text->SetBorder(2);
	text->SetBorderColor({ 1.0f, 1.0f, 0.0f, 1.0f });
	UI.Register(text);
	timer = 3.0f;
}
void SceneGameState::WinState::Execute(float elapsedTime)
{
	timer -= elapsedTime;

	if (timer < 0.0f) SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
}
void SceneGameState::WinState::Exit()
{
}