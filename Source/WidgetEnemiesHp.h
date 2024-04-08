#pragma once
#include <memory>

#include "Widget.h"

#include "Graphics/Sprite.h"

class WidgetEnemiesHp : public Widget
{
public:
	WidgetEnemiesHp(float width = 100.0f, float height = 10.0f);
	WidgetEnemiesHp(DirectX::XMFLOAT2 size);

	virtual void Render(const RenderContext& rc) override;
private:
	std::shared_ptr<Sprite> gauge;
};
