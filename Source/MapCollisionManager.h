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
	// デバッグ情報の表示
	void DrawDebugGUI();

	// デバッグプリミティブの表示
	void DrawDebugPrimitive();
};
