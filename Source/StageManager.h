#pragma once
#include "Collider.h"

#include "ObjectManager.h"
#include "ModelObject.h"
#include "Singleton.h"
#include "Stage.h"

class StageManager : public ObjectManager<GameObject>, public Singleton<StageManager>
{
	friend class Singleton<StageManager>;
protected:
	StageManager() = default;
	~StageManager() = default;

public:
	void SetStage(Stage* stage) { this->currentStage = stage; }
	Stage* GetStage() { return this->currentStage; }
public:
	Stage* currentStage;
};

#define STAGES StageManager::Instance()