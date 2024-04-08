#pragma once

#include "CameraController.h"
#include "Character.h"

class ThridPersonCameraController : public CameraController
{
public:
	ThridPersonCameraController();

	// �X�V����
	void Update(float elapsedTime = 0.0f) override;

	void SetPlayer(Character* player) { this->player = player; }

	DirectX::XMFLOAT2  GetOffset() { return offset; };
	void SetOffset(float x, float y) { offsetLerp = { x, y }; };
	void SetDistance(float d) override { distanceLerp = d; };

	DirectX::XMFLOAT3 GetTargetPt(float distance);

	// �C���X�^���X�擾
	static ThridPersonCameraController& Instance();

	void AimMode(bool enable = true);

	void Shake(float sec, float range) { shakeTimer = sec; shakeRange = range; }
private:
	Character* player;

	float lerpT = 0.2f;

	const float distanceMax = 9.0f;
	const float distanceMin = 3.0f;

	const float angleXMax = DirectX::XM_PI / 3; // 60
	const float scrollSpeed = 0.3f;

	DirectX::XMFLOAT2 offset = { 0, 0 };

	//  �J�����V�F�[�N
	float shakeModify = 1.0f;
	float shakeTimer = 0.0;
	float shakeRange = 0.0f;

	// Smoth Camera
	float angleXLerp = 0;
	float angleYLerp = 0;
	float distanceLerp;
	DirectX::XMFLOAT2 offsetLerp;

	float distanceCache;
};

#define TPSCamera ThridPersonCameraController::Instance()