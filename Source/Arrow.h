#pragma once
#include "Projectile.h"
class Arrow : public Projectile
{
public:
	Arrow(Character* owner);
	~Arrow() = default;

	void Update(float elapsedTime) override;

	void SetPosition(const DirectX::XMFLOAT3& position) override { tempPosition = this->position = position; };

	void Collision() override;

	void OnHitEnemy(HitResult& hit) override;
private:
	DirectX::XMFLOAT3 tempPosition = {}; // 1�t���[���O�̈ʒu
	float timer = 2.0f; // ����
};
