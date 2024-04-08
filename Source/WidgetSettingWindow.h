#pragma once

#include <memory>
#include <vector>

#include "GameData.h"
#include "Widget.h"
#include "WidgetButton.h"
#include "WidgetImage.h"
#include "WidgetText.h"
#include "WidgetButtonImage.h"
#include "Graphics/Sprite.h"

class WidgetSettingWindow : public Widget
{
public:
	WidgetSettingWindow();
	~WidgetSettingWindow();

	void Update(float elapsedTime) override;
	void Render(const RenderContext& rc)override;

	bool IsEnd() { return isEnd; }
private:
	bool isEnd = false; // Š®Œ‹ƒtƒ‰ƒO

	GameData::GameSetting cacheGameSetting;

	std::shared_ptr<Sprite> background;

	WidgetButton* closeBtn;
	WidgetButton* saveBtn;

	std::vector<Widget*> OptionsList;
};
