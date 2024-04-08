#include "UIManager.h"

void UIManager::Update(float elapsedTime)
{
	this->lpCursorName = IDC_ARROW; // �f�t�H���g
	ObjectManager::Update(elapsedTime);
	SetCursor(::LoadCursor(NULL, this->lpCursorName));
}

void UIManager::SetCursorName(LPWSTR lpCursorName)
{
	this->lpCursorName = lpCursorName;
}