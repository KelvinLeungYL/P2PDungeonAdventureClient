#pragma once

#include "Singleton.h"
#include "ObjectManager.h"
#include "Widget.h"

class UIManager : public ObjectManager<Widget>, public Singleton<UIManager>
{
	friend class Singleton<UIManager>;
protected:
	UIManager() = default;
	~UIManager() = default;

public:
	// XVˆ—
	void Update(float elapsedTime) override;

	void SetCursorName(LPWSTR lpCursorName);
private:
	LPWSTR lpCursorName;
};

#define UI UIManager::Instance()