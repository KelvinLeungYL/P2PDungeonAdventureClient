#include "WidgetInputBool.h"
#include "WidgetDragFloat.h"
#include "WidgetSettingWindow.h"
#include "UIManager.h"
#include "Graphics/ResourceManager.h"

WidgetSettingWindow::WidgetSettingWindow()
{
	background = RESOURCE.LoadSpriteResource("Data/Sprites/bar_ready.png");
	size = background->GetTextureSize() * 0.2f;
	position = {
		(SCREEN_W - size.x) / 2.0f,
		(SCREEN_H - size.y) / 2.0f
	};

	// ��
	closeBtn = new WidgetButtonImage("", "Data/Sprites/button_cancel.png", [&](Widget*)
		{
			this->isEnd = true;
		});
	DirectX::XMFLOAT2 s = closeBtn->GetSize();
	s.x = size.x / 12.0f;
	s.y *= (s.x / closeBtn->GetSize().x);
	closeBtn->SetSize(s);
	closeBtn->SetPosition({ position.x + size.x - s.x - 20.0f, position.y + 20.0f });

	// �m�F
	saveBtn = new WidgetButtonImage("�ۑ�", "Data/Sprites/button_ready_on.png", [&](Widget*)
		{
			GAME_DATA.UpdateSetting(cacheGameSetting);
			GAME_DATA.SaveGameSetting();
			this->isEnd = true;
		});
	s = saveBtn->GetSize();
	s.x = size.x / 2.0f;
	s.y *= (s.x / saveBtn->GetSize().x);
	saveBtn->SetSize(s);
	saveBtn->SetPosition({ position.x + (size.x - s.x) * 0.5f, position.y + size.y - s.y - 20.0f });

	cacheGameSetting = GAME_SETTING;
	// �I�v�V�����B
	OptionsList.push_back(new WidgetDragFloat("�J�������x", &cacheGameSetting.CameraSensitivity, 3.0f, 20.0f));
	OptionsList.push_back(new WidgetInputBool("X�����]", &cacheGameSetting.CamerainvertX));
	OptionsList.push_back(new WidgetInputBool("Y�����]", &cacheGameSetting.CamerainvertY));
	OptionsList.push_back(new WidgetInputBool("�L�[�{�[�h����", &cacheGameSetting.KeyboardInput));
	float optionHeight = size.y / 10.0f;
	DirectX::XMFLOAT2 optionPos = { position.x + 20.0f, closeBtn->GetPosition().y + closeBtn->GetSize().y + 20.0f };
	for (Widget*& option : OptionsList)
	{
		option->SetSize({ size.x - 40.0f , optionHeight });
		option->SetPosition(optionPos);
		optionPos.y += option->GetSize().y + 5.0f;
	}
}
WidgetSettingWindow::~WidgetSettingWindow()
{
	delete closeBtn;
	delete saveBtn;
	for (Widget*& option : OptionsList)
	{
		delete option;
	}
	OptionsList.clear();
}

void WidgetSettingWindow::Update(float elapsedTime)
{
	if (T_INPUT.KeyDown(VK_ESCAPE) || T_INPUT.GamePadKeyDown(GAME_PAD_BTN::BACK))
	{
		isEnd = true;
		return;
	}

	closeBtn->Update(elapsedTime);
	saveBtn->Update(elapsedTime);
	for (Widget*& option : OptionsList)
	{
		option->Update(elapsedTime);
	}
}

void WidgetSettingWindow::Render(const RenderContext& rc)
{
	background->Render(rc.deviceContext, position.x, position.y, 0, size.x, size.y);

	closeBtn->Render(rc);
	saveBtn->Render(rc);
	for (Widget*& option : OptionsList)
	{
		option->Render(rc);
	}
}