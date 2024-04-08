#include "ChestState.h"
#include "StageManager.h"

void ChestState::ChestOpenState::Enter()
{
	timer = 1.0f;
	coverNode->rotation = {}; // �W��] ������
	openAngle /= timer;
}
void ChestState::ChestOpenState::Execute(float elapsedTime)
{
	if (timer > 0.0f)
	{
		timer -= elapsedTime;

		DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&coverNode->rotation);
		DirectX::XMVECTOR R = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), openAngle * elapsedTime);
		Q = DirectX::XMQuaternionMultiply(R, Q);
		DirectX::XMStoreFloat4(&coverNode->rotation, Q);
		if (timer <= 0.0f)
		{
			if (OnOpen) OnOpen(owner);
		}
	}
}