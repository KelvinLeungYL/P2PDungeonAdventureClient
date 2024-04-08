#include "Map001Key.h"
#include "PlayerManager.h"
#include "StageManager.h"
#include "TestingStage.h"

Map001Key::Map001Key() : ModelObject("Data/Model/Object/key.gltf.glb")
{
	position = { -38.0f, 2.0f, -48.0f };
	SetCollider(Collider::COLLIDER_TYPE::SPHERE);
	collider->SetScale({ 1.0f, 1.0f, 1.0f });
	collider->SetPosition(position);
}
// �X�V����
void Map001Key::Update(float elapsedTime)
{
	angle.y += DirectX::XMConvertToRadians(360) * elapsedTime;
	angle.x += DirectX::XMConvertToRadians(90) * elapsedTime;
	ModelObject::Update(elapsedTime);

	Collider* c = collider.get();
	HitResult hit;
	for (Player*& player : PLAYERS.GetAll())
	{
		if (player->GetCollider()->Collision(c, {}, hit))
		{
			STAGES.GetStage()->SetPhase(TestingStage::PHASE::GET_KEY);
			STAGES.Remove(this);
			break;
		}
	}
}
// �`�揈��
void Map001Key::Render(const RenderContext& rc)
{
	ModelObject::Render(rc);
#ifdef _DEBUG
	{
		collider->DrawDebugPrimitive({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
#endif // DEBUG
}