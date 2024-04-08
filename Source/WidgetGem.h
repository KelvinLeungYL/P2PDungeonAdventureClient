#pragma once

#include <memory>
#include "WidgetButtonImage.h"
#include "WidgetButtonText.h"
#include "Graphics/Sprite.h"
#include "Network/HttpRequest.h"

class WidgetGem : public WidgetButtonImage
{
public:
	WidgetGem();
	~WidgetGem() {};

	void Update(float elapsedTime) override;
	void Render(const RenderContext& rc) override;
public:
private:
	std::shared_ptr<Sprite> gem;
	WidgetButtonText* chargeBtn;
	HttpRequest* xhr = nullptr;
};
