#pragma once

#include "CameraController.h"

class FreeCameraController : public CameraController
{
public:
	// �X�V����
	void Update(float elapsedTime = 0.0f) override;
};
