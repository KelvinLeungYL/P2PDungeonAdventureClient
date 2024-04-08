#pragma once

#include "Collider.h"
#include "Graphics\Model.h"

class ModelCollider : public Collider
{
public:
	ModelCollider(Model* model);
	~ModelCollider() = default;

	bool CollisionVsShpere(
		Collider*& other,
		DirectX::XMFLOAT3& direction,
		HitResult& result
	) override;

	virtual bool RayCast(
		const DirectX::XMFLOAT3& start,
		const DirectX::XMFLOAT3& end,
		HitResult& result
	) override;
protected:
	Model* model;
};
