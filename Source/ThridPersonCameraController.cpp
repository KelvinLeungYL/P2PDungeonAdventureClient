#include <imgui.h>
#include <algorithm>

#include "GameData.h"

#include "ThridPersonCameraController.h"
#include "MapTileManager.h"

#include "WidgetCrosshair.h"

static ThridPersonCameraController* instance = nullptr;

// �C���X�^���X�擾
ThridPersonCameraController& ThridPersonCameraController::Instance()
{
	return *instance;
}

ThridPersonCameraController::ThridPersonCameraController()
{
	// �C���X�^���X�|�C���^�ݒ�
	instance = this;

	distance = (distanceMax + distanceMin) * 0.5f;
	distanceLerp = distance;
	offsetLerp = offset;
}

// �X�V����
void ThridPersonCameraController::Update(float elapsedTime)
{
	if (player == nullptr) return; // �X�V���Ȃ��t���O
	if (!TentacleLib::isFocus()) return; // �t�H�[�J�X���Ȃ�
	shakeTimer -= elapsedTime;
	if (shakeTimer < 0.0f) shakeTimer = 0.0f;

	float moveX = 0.0f;
	float moveY = 0.0f;
	float wheelScroll = 0.0f;

	if (GAME_SETTING.KeyboardInput)
	{
		// �}�E�X���͒l���g���ăJ�������삷��B
		wheelScroll = T_INPUT.GetMouseScrollY();

		// �}�E�X�J�[�\���̈ړ��ʂ����߂�
		moveX = T_INPUT.GetMouseDeltaX() / SCREEN_W * GAME_SETTING.CameraSensitivity;
		moveY = T_INPUT.GetMouseDeltaY() / SCREEN_H * GAME_SETTING.CameraSensitivity;
	}
	else
	{
		if (T_INPUT.GamePadKeyPress(GAME_PAD_BTN::DPAD_DOWN)) wheelScroll += -1.0f;
		if (T_INPUT.GamePadKeyPress(GAME_PAD_BTN::DPAD_UP)) wheelScroll += +1.0f;

		moveX = T_INPUT.GetGamePadRAxis().x * 0.01f * GAME_SETTING.CameraSensitivity;
		moveY = -T_INPUT.GetGamePadRAxis().y * 0.01f * GAME_SETTING.CameraSensitivity;
	}

	if (!enable)
	{
		moveX = moveY = wheelScroll = 0.0f;
	}

	if (wheelScroll != 0.0f)
	{
		distanceLerp -= wheelScroll * scrollSpeed;
		if (distanceLerp > distanceMax) distanceLerp = distanceMax;
		if (distanceLerp < distanceMin) distanceLerp = distanceMin;
	}
	distance = std::lerp(distance, distanceLerp, lerpT);

	// ���]�ݒ�
	if (GAME_SETTING.CamerainvertX) moveX *= -1.0f;
	if (GAME_SETTING.CamerainvertY) moveY *= -1.0f;

	// Y����]
	angleYLerp += moveX;
	angleY = std::lerp(angleY, angleYLerp, lerpT);

	// X����]
	angleXLerp += moveY;

	angleXLerp = std::clamp(angleXLerp, -angleXMax, angleXMax);
	angleX = std::lerp(angleX, angleXLerp, lerpT);

	// OFFSET
	if (offset.x != offsetLerp.x) offset.x = std::lerp(offset.x, offsetLerp.x, lerpT);
	if (offset.y != offsetLerp.y) offset.y = std::lerp(offset.y, offsetLerp.y, lerpT);

	float sx = ::sinf(angleX);
	float cx = ::cosf(angleX);
	float sy = ::sinf(angleY);
	float cy = ::cosf(angleY);

	// �J�����̕������Z�o
	DirectX::XMVECTOR Front = DirectX::XMVectorSet(-cx * sy, -sx, -cx * cy, 0.0f);
	DirectX::XMVECTOR Right = DirectX::XMVectorSet(cy, 0, -sy, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVector3Cross(Right, Front);

	// offset����
	DirectX::XMFLOAT3 Offset;
	DirectX::XMStoreFloat3(&Offset, Up);
	Offset = Offset * offset.y;
	Offset = Offset + (DirectX::XMFLOAT3(cy, 0, -sy) * -offset.x);

	focus = player->GetPosition() + DirectX::XMFLOAT3(0, player->GetHeight() * 1.1f, 0);
	// RayCast �Ǒ΍�
	{
		HitResult hit;
		DirectX::XMFLOAT3 end = focus + Offset;
		if (MAPTILES.RayCast(focus, end, hit, true))
		{
			// �J�������ǂɓ�������
			focus = hit.position;
		}
		else
		{
			focus = focus + Offset;
		}
	}

	// �J�����h��
	if (shakeTimer > 0.0f)
	{
		float shakeY = shakeRange * (shakeTimer);
		shakeY *= shakeModify;
		shakeModify *= -1.0f; // �t

		focus.y += shakeY;
	}

	// �J�����̎��_&�����_���Z�o
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);

	DirectX::XMVECTOR Distance = DirectX::XMVectorSet(distance, distance, distance, 0);

	DirectX::XMVECTOR Eye = DirectX::XMVectorSubtract(Focus, DirectX::XMVectorMultiply(Front, Distance));

	// RayCast �Ǒ΍�
	{
		HitResult hit;
		DirectX::XMFLOAT3 end;
		DirectX::XMStoreFloat3(&end, Eye);
		if (MAPTILES.RayCast(focus, end, hit, true))
		{
			// �J�������ǂɓ�������
			float d = hit.distance * 0.95f;
			if (d < 0.01f) d = 0.01f;
			Eye = DirectX::XMVectorSubtract(Focus, DirectX::XMVectorScale(Front, d));
		}
	}

	// �r���[�s�񂩂烏�[���h�s����Z�o
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMMATRIX World = DirectX::XMMatrixTranspose(View);
	// ���[���h�s�񂩂�������Z�o
	Right = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), World);
	Up = DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), World);
	// ���ʂ��i�[
	DirectX::XMStoreFloat3(&eye, Eye);
	DirectX::XMStoreFloat3(&up, Up);
	DirectX::XMStoreFloat3(&right, Right);
}

DirectX::XMFLOAT3 ThridPersonCameraController::GetTargetPt(float distance)
{
	return eye + (XMFLOAT3Normalize(focus - eye) * distance);
}

void ThridPersonCameraController::AimMode(bool enable)
{
	if (enable)
	{
		distanceCache = distanceLerp;
		offsetLerp = { 1.5f, 0.5f };
		distanceLerp = 3.0f;

		CROSS_HAIR.Show();
	}
	else
	{
		offsetLerp = { 0.0, 0.0f };
		distanceLerp = distanceCache;

		CROSS_HAIR.Hide();
	}
}