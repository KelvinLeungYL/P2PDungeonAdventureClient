#pragma once
#include "tentacle_lib.h"
#include <DirectXMath.h>

#include "Collider.h"

// ���f���p�I�u�W�F�N�g
class GameObject
{
public:
	GameObject() = default;
	virtual ~GameObject() = default;

	void UpdateTransform();
	// �X�V����
	virtual void Update(float elapsedTime) = 0;
	// �`�揈��
	virtual void Render(const RenderContext& rc) = 0;

	DirectX::XMFLOAT3 LocalPositiontoWorld(DirectX::XMFLOAT3 v);
	DirectX::XMFLOAT3 GetScreenPosition(
		const D3D11_VIEWPORT& viewport,
		const  DirectX::XMMATRIX& View,
		const  DirectX::XMMATRIX& Projection,
		const DirectX::XMMATRIX World
	);
	DirectX::XMFLOAT3 GetScreenPosition();

	virtual void DrawDebugPrimitive() {}
	virtual void DrawDebugGUI() {};
public:
	// �A�N�Z�T
	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	// �ʒu�ݒ�
	virtual void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	// ��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	// ��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	// ����]
	void RotateAxis(const DirectX::XMFLOAT3 Axis, float Angle);
	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	// �X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	const DirectX::XMFLOAT3 GetFront() const { return { transform._31, transform._32, transform._33 }; }
	const DirectX::XMFLOAT3 GetRight() const { return { transform._11, transform._12, transform._13 }; }
	const DirectX::XMFLOAT3 GetUp() const { return { transform._21, transform._22, transform._23 }; }

protected:

	DirectX::XMFLOAT3 position = { 0, 0, 0 };
	DirectX::XMFLOAT3 angle = { 0, 0, 0 };
	DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	DirectX::XMFLOAT4X4 transform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
};