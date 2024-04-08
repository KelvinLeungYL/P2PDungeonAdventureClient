#include "Stage.h"

#include "MapTileManager.h"
#include "EnemyManager.h"

void Stage::Initialize()
{
	timer = 0.0f;
	finish = false;
	pointer = 0;

	MAPTILES.Clear();
	ENEMIES.Clear();
	enemyList.clear();
}

void Stage::Update(float elapsedTime)
{
	timer += elapsedTime;

	// �X�|�i�[
	for (int i = pointer; i < enemyList.size(); i++) {
		ENEMY_LIST_DATA& enemyData = enemyList.at(i);
		if (enemyData.phase == phase && enemyData.time <= timer)
		{
			// �G����
			Enemy* enemy = ENEMIES.Register(Enemy::EnemyFactory(enemyData.enemyType));
			enemy->SetPosition(enemyData.position);
			enemy->SetAngle(enemyData.rotation);
			pointer++;
		}
	}
}

void Stage::SetPhase(int phase)
{
	this->phase = phase;
	timer = 0.0f;
	OnPhase();
}
void Stage::NextPhase()
{
	SetPhase(++phase);
}