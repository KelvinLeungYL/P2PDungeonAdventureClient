#include "SceneTitleState.h"

#include "WidgetGem.h"
#include "UIManager.h"
#include "url.h"

// �^�C�g���X�e�[�g
void SceneTitleState::TitleState::Enter()
{
	title = new WidgetText("P2P�_���W�����E�A�h�x���`���[");
	title->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	title->SetScale(2.0f);
	title->SetBorder(1);
	title->SetBorderColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	title->SetAlign(FONT_ALIGN::CENTER);
	title->SetPosition({ SCREEN_W * 0.5f, SCREEN_H * 0.2f });
	UI.Register(title);

	click = new WidgetText("�N���b�N�Ŏn�܂�");
	click->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	click->SetScale(0.5f);
	click->SetBorder(1);
	click->SetAlign(FONT_ALIGN::TOP);
	click->SetPosition({ SCREEN_W * 0.5f, SCREEN_H * 0.8f });
	UI.Register(click);

	mask = new WidgetImage("");
	mask->SetSize({ SCREEN_W, SCREEN_H });
	mask->SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
	UI.Register(mask);

	SetCursor(::LoadCursor(NULL, IDC_HAND));
}
void SceneTitleState::TitleState::Execute(float elapsedTime)
{
	// �t�F�C�h�C��
	DirectX::XMFLOAT4 color = mask->GetColor();
	color.w -= elapsedTime;
	if (color.w < 0.0f) color.w = 0.0f;
	mask->SetColor(color);

	if (color.w == 0.0f)
	{
		// �t���b�V��
		timer += elapsedTime * 2.0f;
		while (timer >= DirectX::XMConvertToRadians(180)) timer -= DirectX::XMConvertToRadians(180);
		click->SetColor({ 1.0f, 1.0f, 1.0f, sinf(timer) });
		click->SetBorderColor({ 0.0f, 0.0f, 0.0f, sinf(timer) });

		// ���N���b�N
		if (T_INPUT.KeyDown(VK_LBUTTON)) owner->GetStateMachine()->ChangeState(SceneTitle::STATE::LOGIN_CHECK);
	}
}
void SceneTitleState::TitleState::Exit()
{
	UI.Clear();

	SetCursor(::LoadCursor(NULL, IDC_HAND));
}

// �C���v�b�g�X�e�[�g
void SceneTitleState::InputState::Enter()
{
	WidgetImage* mask = new WidgetImage("");
	mask->SetSize({ SCREEN_W, SCREEN_H });
	mask->SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
	UI.Register(mask);

	// �ݒ�{�^��
	WidgetButtonImage* btnSetting = new WidgetButtonImage("", "Data/Sprites/gear.png", [&](WidgetButton*) {
		this->owner->GetStateMachine()->ChangeState(SceneTitle::STATE::SETTING);
		});
	btnSetting->SetPosition({ SCREEN_W - 192.0f, 30.0f });
	btnSetting->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	btnSetting->SetSize({ 128.0f, 128.0f });
	btnSetting->SetHover([btn = btnSetting](WidgetButton*) {
		btn->GetText()->SetText("�ݒ�");
		});
	btnSetting->SetUnhover([btn = btnSetting](WidgetButton*) {
		btn->GetText()->SetText("");
		});
	UI.Register(btnSetting);

	if (GAME_DATA.IsOnline()) // �I�����C�����[�h
	{
		if (GAME_DATA.GetUser().id > 0)
		{
			// ���O�A�E�g�{�^��
			WidgetButtonImage* btnLogout = new WidgetButtonImage("", "Data/Sprites/Logout.png", [&](WidgetButton*) {
				GAME_DATA.Logout();
				this->owner->GetStateMachine()->ChangeState(SceneTitle::STATE::LOGIN_CHECK);
				});
			btnLogout->SetPosition({ SCREEN_W - 350.0f, 32.0f });
			btnLogout->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
			btnLogout->SetSize({ 128.0f, 128.0f });
			btnLogout->SetHover([btn = btnLogout](WidgetButton*) {
				btn->GetText()->SetText("���O�A�E�g");
				});
			btnLogout->SetUnhover([btn = btnLogout](WidgetButton*) {
				btn->GetText()->SetText("");
				});
			UI.Register(btnLogout);

			// �W�F���\��
			WidgetGem* gems = new WidgetGem;
			gems->SetPosition({ 50.0f, SCREEN_H - 50.0f - gems->GetSize().y });
			UI.Register(gems);
		}
		else
		{
			// ���O�C���{�^��
			WidgetButtonImage* btnLogin = new WidgetButtonImage("", "Data/Sprites/Login.png", [&](WidgetButton*) {
				this->owner->GetStateMachine()->ChangeState(SceneTitle::STATE::LOGIN);
				});
			btnLogin->SetPosition({ SCREEN_W - 350.0f, 32.0f });
			btnLogin->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
			btnLogin->SetSize({ 128.0f, 128.0f });
			btnLogin->SetHover([btn = btnLogin](WidgetButton*) {
				btn->GetText()->SetText("���O�C��");
				});
			btnLogin->SetUnhover([btn = btnLogin](WidgetButton*) {
				btn->GetText()->SetText("");
				});
			UI.Register(btnLogin);
		}
	}
	std::string description = "�V���������i�z�X�g�j�F\n�|�[�g���J�����A�ڑ���҂� / �\���v���C����\n\n�����i�N���C�A���g�j�F\n�����̕����ɐڑ�����";
	WidgetText* T = new WidgetText(description.c_str(), 1.0f);
	T->SetPosition({ SCREEN_W * 0.5f, 200.0f });
	T->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	T->SetAlign(FONT_ALIGN::TOP);
	UI.Register(T);

	WidgetText* IP = new WidgetText(GAME_DATA.GetIp().c_str(), 1.0f);
	IP->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	UI.Register(IP);

	WidgetRoomOption* roomOption = new WidgetRoomOption;
	UI.Register(roomOption);
}
void SceneTitleState::InputState::Execute(float elapsedTime)
{
	// �E�N���b�N
	if (T_INPUT.KeyDown(VK_RBUTTON) || T_INPUT.KeyDown(VK_ESCAPE))
		owner->GetStateMachine()->ChangeState(SceneTitle::STATE::TITLE);
}
void SceneTitleState::InputState::Exit()
{
	UI.Clear();
}

// �ݒ�X�e�[�g
void SceneTitleState::SettingState::Enter()
{
	WidgetImage* mask = new WidgetImage("");
	mask->SetSize({ SCREEN_W, SCREEN_H });
	mask->SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
	UI.Register(mask);

	settingWindow = new WidgetSettingWindow;
	UI.Register(settingWindow);
}
void SceneTitleState::SettingState::Execute(float elapsedTime)
{
	// �E�N���b�N
	if (settingWindow->IsEnd() || T_INPUT.KeyDown(VK_RBUTTON) || T_INPUT.KeyDown(VK_ESCAPE))
		owner->GetStateMachine()->ChangeState(SceneTitle::STATE::INPUT);
}
void SceneTitleState::SettingState::Exit()
{
	UI.Clear();
}

// ���O�C���X�e�[�g
void SceneTitleState::LoginState::Enter()
{
	WidgetImage* mask = new WidgetImage("");
	mask->SetSize({ SCREEN_W, SCREEN_H });
	mask->SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
	UI.Register(mask);

	widgetLogin = new WidgetLogin;
	UI.Register(widgetLogin);
}
void SceneTitleState::LoginState::Execute(float elapsedTime)
{
	switch (widgetLogin->GetState())
	{
	case WidgetLogin::STATE::FAIL:
	case WidgetLogin::STATE::INPUT:
		// �E�N���b�N
		if (T_INPUT.KeyDown(VK_RBUTTON) || T_INPUT.KeyDown(VK_ESCAPE))
			owner->GetStateMachine()->ChangeState(SceneTitle::STATE::INPUT);
		break;
	case WidgetLogin::STATE::SUCCESS:
		owner->GetStateMachine()->ChangeState(SceneTitle::STATE::LOGIN_CHECK);
		return;
	}
}
void SceneTitleState::LoginState::Exit()
{
	UI.Clear();
}

void SceneTitleState::LoginCheckState::Enter()
{
	if (!GAME_DATA.IsOnline()) // �I�t���C�����[�h
	{
		owner->GetStateMachine()->ChangeState(SceneTitle::STATE::INPUT);
		return;
	}

	if (GAME_DATA.GetUser().id > 0) // ���O�C���ς�
	{
		owner->GetStateMachine()->ChangeState(SceneTitle::STATE::INPUT);
		return;
	}

	if (strlen(GAME_SETTING.token) == 0) // ���O�C���g�[�N���Ȃ�
	{
		owner->GetStateMachine()->ChangeState(SceneTitle::STATE::INPUT);
		return;
	}

	WidgetImage* mask = new WidgetImage("");
	mask->SetSize({ SCREEN_W, SCREEN_H });
	mask->SetColor({ 1.0f, 1.0f, 1.0f, 0.7f });
	UI.Register(mask);

	xhr = new HttpRequest(URL(Url::USER));
	xhr->SetToken(GAME_DATA.GetSetting().token);
	xhr->Send();
}
void SceneTitleState::LoginCheckState::Execute(float elapsedTime)
{
	if (xhr->IsDone())
	{
		HttpResponse response = xhr->GetResponse();
		if (response.status == 200)
		{
			// ���O�C������
			GAME_DATA.GetUser().id = response.data["id"];
			GAME_DATA.GetUser().useranme = response.data["username"];
			GAME_DATA.GetUser().nickname = response.data["name"];
			GAME_DATA.GetUser().gems = response.data["gems"];
		}
		else
		{
			// ���O�C�����s�g�[�N���폜
			memset(GAME_DATA.GetSetting().token, 0, sizeof(GAME_DATA.GetSetting().token));
			GAME_DATA.SaveGameSetting();
		}
		delete xhr;
		xhr = nullptr;
		owner->GetStateMachine()->ChangeState(SceneTitle::STATE::INPUT);
	}
}
void SceneTitleState::LoginCheckState::Exit()
{
	if (xhr != nullptr)
	{
		delete xhr;
		xhr = nullptr;
	}
	UI.Clear();
}