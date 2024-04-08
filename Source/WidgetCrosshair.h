#pragma once

#include "WidgetImage.h"

class WidgetCrosshair : public WidgetImage
{
public:
	WidgetCrosshair();

	void Update(float elapsedTime) override;

	void Show() { this->enable = true; }
	void Hide() { this->enable = false; }

	// �C���X�^���X�擾
	static WidgetCrosshair& Instance();
private:
	bool enable = false;
};

#define CROSS_HAIR WidgetCrosshair::Instance()