#pragma once

#include <vector>
#include "Collider.h"
#include "Manager.h"
#include "Singleton.h"

class MapCollisionManager : public Manager<Collider>, public Singleton<MapCollisionManager>
{
	friend class Singleton<MapCollisionManager>;
protected:
	MapCollisionManager() {}
	~MapCollisionManager();
public:
	// �f�o�b�O���̕\��
	void DrawDebugGUI();

	// �f�o�b�O�v���~�e�B�u�̕\��
	void DrawDebugPrimitive();
};
