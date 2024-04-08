#pragma once

#include <DirectXMath.h>

struct HitResult
{
	DirectX::XMFLOAT3 position = { 0, 0, 0 };	// ���C�ƃ|���S���̌�_
	DirectX::XMFLOAT3 normal = { 0, 0, 0 };		// �Փ˂����|���S���̖@���x�N�g��
	DirectX::XMFLOAT3 rotation = { 0, 0, 0 };	// ��]��
	float distance = FLT_MAX;						// ���C�̎n�_�����_�܂ł̋���
	int materialIndex = -1;						// �Փ˂����|���S���̃}�e���A���ԍ�
};

class Collider
{
public:
	enum class COLLIDER_TYPE
	{
		DEFAULT,
		UNROTATED_BOX,			// ���Ȃ��{�b�N�X
		MODEL,					// ���f��
		SPHERE,					// ����
		BOUNDING_BOX,			// �o�E���f�B���O�{�b�N�X

		enum_count
	};
public:
	Collider() = default;
	~Collider() = default;

	bool Collision(
		Collider*& other,
		DirectX::XMFLOAT3 direction,
		HitResult& result
	);

	virtual bool CollisionVsUnrotatedBox(
		Collider*& other,
		DirectX::XMFLOAT3& direction,
		HitResult& result
	) {
		return false;
	};

	virtual bool CollisionVsModel(
		Collider*& other,
		DirectX::XMFLOAT3& direction,
		HitResult& result
	) {
		return false;
	};

	virtual bool CollisionVsShpere(
		Collider*& other,
		DirectX::XMFLOAT3& direction,
		HitResult& result
	) {
		return false;
	};

	virtual bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& result
	) {
		return false;
	}

	virtual DirectX::XMFLOAT3 GetTop() { return {}; };

	virtual void DrawDebugPrimitive(DirectX::XMFLOAT4 color = { 1, 1, 1, 1 }) {};
	virtual void DrawDebugGUI() {};

	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	DirectX::XMFLOAT3 GetPosition() const { return position; };

	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	DirectX::XMFLOAT3 GetRotation() const { return rotation; };

	void SetScale(DirectX::XMFLOAT3 s) { scale = s; }
	DirectX::XMFLOAT3 GetScale() const { return scale; }

	COLLIDER_TYPE GetType() { return type; };

	bool IsEnable() { return enable; }
	void SetEnable(bool e) { enable = e; }
protected:
	COLLIDER_TYPE type = COLLIDER_TYPE::DEFAULT;

	DirectX::XMFLOAT3 position = { 0.0f, 0.0f, 0.0f }; // ���[���h�ʒu
	DirectX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f }; // ���[���h���[�e�[�V����
	DirectX::XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	bool enable = true;
};
