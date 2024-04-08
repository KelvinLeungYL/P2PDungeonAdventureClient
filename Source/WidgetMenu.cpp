#include "WidgetMenu.h"

#include "tentacle_lib.h"
#include "Graphics/ResourceManager.h"
#include "ThridPersonCameraController.h"
#include "WidgetButtonText.h"
#include "SceneManager.h"
#include "SceneTitle.h"

WidgetMenu::WidgetMenu()
{
	background = RESOURCE.LoadSpriteResource("Data/Sprites/big_background.png");
	size = background->GetTextureSize();
	// ������ҏW
	float rate = SCREEN_H / size.y; // �g��k����
	size = size * rate;

	position.x = -size.x; // ���ɉB��

	options.push_back(new WidgetButtonText("�I�v�V����", [&](Widget*) {
		if (this->settingWindow == nullptr) this->settingWindow = new WidgetSettingWindow;
		}));

	options.push_back(new WidgetButtonText("�^�C�g����ʂ�", [&](Widget*) {
		SceneManager::Instance().ChangeScene(new SceneLoading(new SceneTitle));
		}));
}
WidgetMenu::~WidgetMenu()
{
	for (WidgetButton*& option : options)
	{
		delete option;
	}
	options.clear();
	if (settingWindow) delete settingWindow;
}

void WidgetMenu::Show()
{
	enable = true;
	CURSOR_ON;
	TPSCamera.SetEnable(false);
}
void WidgetMenu::Hide()
{
	enable = false;
	CURSOR_OFF;
	TPSCamera.SetEnable(true);
}

DirectX::XMFLOAT2 WidgetMenu::GetOptionsSize()
{
	DirectX::XMFLOAT2 optionSize = {};
	for (WidgetButton*& option : options)
	{
		optionSize = option->GetSize();
	}
	return optionSize;
}

void WidgetMenu::Update(float elapsedTime)
{
	if (enable && position.x < 0) // ������o��
	{
		position.x += elapsedTime * (size.x / moveSpeed); // 1�b�ŏo��
		if (position.x > 0) position.x = 0.0f;
	}
	if (!enable && position.x > -size.x) // ���ɏ�����
	{
		position.x -= elapsedTime * (size.x / moveSpeed); // 1�b�ŏ�����
		if (position.x < -size.x) position.x = -size.x;
	}

	DirectX::XMFLOAT2 optionSize = GetOptionsSize();
	DirectX::XMFLOAT2 optionPosition = position + ((size - optionSize) * 0.5f);
	for (WidgetButton*& option : options)
	{
		option->SetPosition(optionPosition);
		option->Update(elapsedTime);
		optionPosition.y += option->GetSize().y + 10.0f; // padding 10
	}
	if (settingWindow)
	{
		settingWindow->Update(elapsedTime);
		if (settingWindow->IsEnd())
		{
			delete settingWindow;
			settingWindow = nullptr;
		}
	}
}
void WidgetMenu::Render(const RenderContext& rc)
{
	T_TEXT.End();
	T_TEXT.Begin();
	if (position.x > -size.x)
	{
		background->Render(
			rc.deviceContext,
			position.x, position.y, 0,
			size.x, size.y,
			20.0f, 20.0f,
			2008, 3012,
			0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		);

		for (size_t i = 0; i < options.size(); i++)
		{
			options.at(i)->Render(rc);
		}
	}
	if (settingWindow) settingWindow->Render(rc);
}