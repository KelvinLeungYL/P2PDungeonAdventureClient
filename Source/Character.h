#pragma once

#include "ModelObject.h"
#include "Graphics/RenderContext.h"
#include "Collider.h"
#include "StateMachine.h"

class Character : public ModelObject
{
public:
	Character(const char* filename, float scaling = 1.0f) : ModelObject(filename, scaling) {}
	virtual ~Character() = default;

	virtual void Update(float elapsedTime) override;
	void UpdateModel(float elapsedTime);
	virtual void Render(const RenderContext& rc) override;

	virtual bool MoveTo(float elapsedtime, const DirectX::XMFLOAT3& target, float moveSpeed, float turnSpeed);
	void Stop() { velocity = { 0.0f, 0.0f, 0.0f }; }

	void Turn(float elapsedTime, float vx, float vz, float speed);

	virtual void AttackCollision() {}

protected:
	// �ړ�
	void Move(float vx, float vz, float speed);
	void Jump(float speed);

	// ����
	virtual void UpdateVelocity(float elapsedTime);
	// �������͍X�V����
	void UpdateVerticalVelocity(float elapsedFrame);
	// �����ړ��X�V����
	void UpdateVerticalMove(float elapsedTime);
	// �������͍X�V����
	void UpdateHorizontalVelocity(float elapsedFrame);
	// �����ړ��X�V����
	virtual void UpdateHorizontalMove(float elapsedTime);

	virtual void OnLanding() {};
	virtual void OnWall() {};
public:
	// �e��
	void AddImpulse(const DirectX::XMFLOAT3& impulse);

	// �A�N�Z�T
	// �ʒu�擾
	const DirectX::XMFLOAT3& GetPosition() const { return position; }
	// �ʒu�ݒ�
	void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }
	// ��]�擾
	const DirectX::XMFLOAT3& GetAngle() const { return angle; }
	// ��]�ݒ�
	void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }
	// �X�P�[���擾
	const DirectX::XMFLOAT3& GetScale() const { return scale; }
	// �X�P�[���ݒ�
	void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

	const float GetMaxMoveSpeed() { return maxMoveSpeed; }

	float GetHeight() { return height; }

	// �n�ʔ���
	bool IsGround() const { return isGround; }

	float GetHurtCoolTime() { return hurtCoolTime; }
	void SetHurtCoolTime(float second) { hurtCoolTime = second; }

	// HP
	int GetHp() { return hp; }
	int GetMaxHp() { return maxHp; }
	void ModifyHp(int hp);
protected:
	virtual void UpdateColliders() {};
protected:
	// ���C�L���X�g�p
	float stepOffset = 0.5f;
	float height = 2.0f;

	float gravity = -1.0f;
	DirectX::XMFLOAT3 velocity = { 0, 0, 0 };
	bool isGround = false;
	bool isWall = false;
	float friction = 0.5f;

	float acceleration = 100.0f;
	float maxMoveSpeed = 100.0f;
	float moveVecX = 0.0f;
	float moveVecZ = 0.0f;
	float airControl = 0.1f;

	float hurtCoolTime = 0.0f;

	// HP
	int hp = 1;
	int maxHp = 1;
};
