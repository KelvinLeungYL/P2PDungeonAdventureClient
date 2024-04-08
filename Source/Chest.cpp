#include <functional>
#include "Chest.h"
#include "ChestState.h"
#include "SphereCollider.h"
#include "StageManager.h"

Chest::Chest(const char* filename) : Enemy(filename, 1.0f)
{
	enemyType = ENEMY_TYPE::CHEST_EMPTY;
	maxHp = hp = 1;
	atk = 0;
	moveSpeed = 0.0f;
	turnSpeed = 0.0f;

	showHp = false;

	// �����蔻��
	colliders[0] = new SphereCollider(1.0f);

	stateMachine->RegisterState(CHEST_STATE::CLOSE, new ChestState::ChestCloseState(this));

	stateMachine->SetState(CHEST_STATE::CLOSE);
}

// �Փ˔���X�V
void Chest::UpdateColliders()
{
	colliders[0]->SetPosition(GetNodePosition(DirectX::XMFLOAT3{ 0.0f, 0.5f, 0.0f }));
}

void Chest::OnDeath()
{
	stateMachine->ChangeState(CHEST_STATE::OPEN);
}

EmptyChest::EmptyChest() : Chest("Data/Model/Object/chest.glb")
{
	stateMachine->RegisterState(CHEST_STATE::OPEN, new ChestState::ChestOpenState(this, this->GetModel()->FindNode("chest_lid"), [chest = this](Enemy*) mutable {}));
}
EmptyChest::EmptyChest(std::function<void(Enemy*)> OnOpen) : Chest("Data/Model/Object/chest.glb")
{
	stateMachine->RegisterState(CHEST_STATE::OPEN, new ChestState::ChestOpenState(this, this->GetModel()->FindNode("chest_lid"), OnOpen));
}
void EmptyChest::SetOnOpen(std::function<void(Enemy*)> OnOpen)
{
	stateMachine->RegisterState(CHEST_STATE::OPEN, new ChestState::ChestOpenState(this, this->GetModel()->FindNode("chest_lid"), OnOpen));
}

VictoryChest::VictoryChest() : Chest("Data/Model/Object/chest_gold.glb")
{
	enemyType = ENEMY_TYPE::CHEST_COIN;
	stateMachine->RegisterState(CHEST_STATE::OPEN, new ChestState::ChestOpenState(this, this->GetModel()->FindNode("chest_gold_lid"), std::bind(&VictoryChest::OnOpen, this)));
}
void VictoryChest::OnOpen()
{
	STAGES.GetStage()->Finish();
}