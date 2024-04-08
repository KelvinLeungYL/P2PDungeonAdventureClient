#pragma once

#include <string>
#include "Widget.h"
#include "Enemy.h"

class WidgetBossHp : public Widget
{
public:
	WidgetBossHp(const char* name, Enemy* enemy);
	~WidgetBossHp() {}

	void Update(float elapsedTime) override;
	void Render(const RenderContext& rc) override;
private:
	Enemy* enemy = nullptr;
	std::string name;
	std::shared_ptr<Sprite> gauge;
	std::shared_ptr<Sprite> gaugeFrame;

	// �`��p
	float tempHp = 0;
};
