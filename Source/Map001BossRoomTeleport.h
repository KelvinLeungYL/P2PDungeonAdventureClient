#pragma once

#include "PlayerCollisionObject.h"

class Map001BossRoomTeleport : public PlayerCollisionObject
{
public:
	Map001BossRoomTeleport();
	~Map001BossRoomTeleport() {};

	// �X�V����
	virtual void Update(float elapsedTime) override;
	// �`�揈��
	void Render(const RenderContext& rc) override;

protected:
	void OnHit(Player* player);
};
