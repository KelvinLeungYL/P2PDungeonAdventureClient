#pragma once
#include "Character.h"

#include "Enemy.h"

class Projectile : public ModelObject
{
public:
	Projectile(const char* filename, float scaling = 1.0f, Character* owner = nullptr) : ModelObject(filename, scaling), owner(owner) {}
	~Projectile() = default;

	void Update(float elapsedTime) override;
	void Render(const RenderContext& rc) override;

	virtual void UpdateColliders() {};

	void PointTo(const DirectX::XMFLOAT3& target);
	void SetFront(DirectX::XMFLOAT3 front) { this->front = front; }
	void SetDirection(DirectX::XMFLOAT3 direction) { this->direction = direction; };
	void SetSpeed(float speed) { this->speed = speed; };
	void SetGravity(float gravity) { this->gravity = gravity; }
	Character* GetOwner() { return owner; }
	void SetOwner(Character* character) { owner = character; }

	virtual void Collision();
protected:
	void SendCollision(Enemy* target, int colider_id);
	void Destory();

	virtual void OnDestory() {}
	virtual void OnHitEnemy(HitResult& hit) {}
	virtual void OnHitGround(HitResult& hit) { Destory(); }
	virtual void OnHitWall(HitResult& hit) { Destory(); }

protected:
	enum COLLISION_TARGET {
		STAGE = 1 << 0,
		ENEMY = 1 << 1,
		PLAYER = 1 << 2,
	};
protected:
	uint8_t collisionTarget = 0;
	bool pierce = false;	// �ђ� (�����ڕW�ɓ�����)
	bool power = false;		// �Ռ� �i���E��j
	float coolTime = 0.2f;	// �����㖳�G����
	float force = 0.0f;

	DirectX::XMFLOAT3 front = { 0.0f, -1.0f, 0.0f };
	DirectX::XMFLOAT3 right = { 1.0f, 0.0f, 0.0f };

	// ����
	DirectX::XMFLOAT3 direction = { 0.0f, 1.0f, 0.0f };
	float speed = 0.0f;
	float gravity = -1.0f;

	Character* owner = nullptr;

	int atk = 1;
};
