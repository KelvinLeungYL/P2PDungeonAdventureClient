#include "Map001BossRoomTeleport.h"
#include "StageManager.h"
#include "PlayerManager.h"
#include "GameData.h"
#include "TestingStage.h"

Map001BossRoomTeleport::Map001BossRoomTeleport() : PlayerCollisionObject(5.0f)
{
	position = { 20.0f, 1.0f, -65.6f };
	collider->SetPosition(position);
}

void Map001BossRoomTeleport::Update(float elapsedTime)
{
	if (STAGES.GetStage()->GetPhase() == TestingStage::PHASE::GET_KEY)
	{
		for (Player*& player : PLAYERS.GetAll())
		{
			HitResult hit;
			Collider* col = collider.get();
			if (player->GetCollider()->Collision(col, {}, hit))
			{
				STAGES.GetStage()->SetPhase(TestingStage::PHASE::BOSS);
			}
		}
	}

	PlayerCollisionObject::Update(elapsedTime);
}

void Map001BossRoomTeleport::OnHit(Player* player)
{
	if (STAGES.GetStage()->GetPhase() >= TestingStage::PHASE::GET_KEY)
	{
		STAGES.Remove(this);
		player->SetPosition({ 20.0f, 0.2f, -67.0f });
	}
}

void Map001BossRoomTeleport::Render(const RenderContext& rc)
{
	PlayerCollisionObject::Render(rc);
	if (isHit)
	{
		T_TEXT.Render(
			FONT_ID::HGpop,
			"Œ®‚ª•K—v‚¾",
			SCREEN_W * 0.5f, SCREEN_H * 0.1f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f,
			FONT_ALIGN::TOP,
			0.5f,
			1
		);
	}
}