#pragma once

#include "ObjectManager.h"
#include "Singleton.h"
#include "Projectile.h"

class ProjectileManager : public ObjectManager<Projectile>, public Singleton<ProjectileManager>
{
	friend class Singleton<ProjectileManager>;
protected:
	ProjectileManager() = default;
	~ProjectileManager() = default;
};

#define PROJECTILES ProjectileManager::Instance()