#include "MapTileManager.h"

// ���C�L���X�g
bool MapTileManager::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& hit, bool camera)
{
	HitResult temp;
	hit.distance = FLT_MAX;
	for (ModelObject*& item : items)
	{
		if (item->GetCollider() == nullptr) continue;
		if (item->GetCollider()->RayCast(start, end, temp) && (temp.distance < hit.distance))
		{
			hit = temp; // �ŒZ
		}
	}
	return hit.distance < FLT_MAX;
}