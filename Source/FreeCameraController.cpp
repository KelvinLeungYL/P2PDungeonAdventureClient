#include <imgui.h>
#include "FreeCameraController.h"

// �X�V����
void FreeCameraController::Update(float elapsedTime)
{
	if (!enable) return;
	// �f�o�b�O�E�B���h���쒆�͏������Ȃ�
	if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow)) return;

	// IMGUI�̃}�E�X���͒l���g���ăJ�������삷��B
	ImGuiIO io = ImGui::GetIO();

	// �}�E�X�J�[�\���̈ړ��ʂ����߂�
	float moveX = io.MouseDelta.x * 0.02f;
	float moveY = io.MouseDelta.y * 0.02f;

	// �}�E�X�{�^��������
	if (io.MouseDown[ImGuiMouseButton_Left])
	{
		// Y����]
		angleY += moveX * 0.5f;
		if (angleY > DirectX::XM_PI)
		{
			angleY -= DirectX::XM_2PI;
		}
		else if (angleY < -DirectX::XM_PI)
		{
			angleY += DirectX::XM_2PI;
		}
		// X����]
		angleX += moveY * 0.5f;
		if (angleX > DirectX::XM_PI)
		{
			angleX -= DirectX::XM_2PI;
		}
		else if (angleX < -DirectX::XM_PI)
		{
			angleX += DirectX::XM_2PI;
		}
	}
	// �}�E�X���{�^��������
	else if (io.MouseDown[ImGuiMouseButton_Middle])
	{
		// ���s�ړ�
		float s = distance * 0.035f;
		float x = moveX * s;
		float y = moveY * s;

		focus.x -= right.x * x;
		focus.y -= right.y * x;
		focus.z -= right.z * x;

		focus.x += up.x * y;
		focus.y += up.y * y;
		focus.z += up.z * y;
	}
	// �}�E�X�E�{�^��������
	else if (io.MouseDown[ImGuiMouseButton_Right])
	{
		// �Y�[��
		distance += (-moveY - moveX) * distance * 0.1f;
	}
	// �}�E�X�ف[����
	else if (io.MouseWheel != 0)
	{
		// �Y�[��
		distance -= io.MouseWheel * distance * 0.1f;
	}

	float sx = ::sinf(angleX);
	float cx = ::cosf(angleX);
	float sy = ::sinf(angleY);
	float cy = ::cosf(angleY);

	// �J�����̕������Z�o
	DirectX::XMVECTOR Front = DirectX::XMVectorSet(-cx * sy, -sx, -cx * cy, 0.0f);
	DirectX::XMVECTOR Right = DirectX::XMVectorSet(cy, 0, -sy, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVector3Cross(Right, Front);
	// �J�����̎��_&�����_���Z�o
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Distance = DirectX::XMVectorSet(distance, distance, distance, 0);
	DirectX::XMVECTOR Eye = DirectX::XMVectorSubtract(Focus, DirectX::XMVectorMultiply(Front, Distance));
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