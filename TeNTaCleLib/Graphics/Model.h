#pragma once

#include <memory>
#include <vector>
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d11.h>
#include "ModelResource.h"

class Model
{
public:
	Model(ID3D11Device* device, const char* filename, float scaling = 1.0f);

	struct Node
	{
		std::string name;
		std::string path;
		int parentIndex;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 rotation;
		DirectX::XMFLOAT3 scale;

		Node* parent = nullptr;
		std::vector<Node*> children;

		DirectX::XMFLOAT4X4 localTransform;
		DirectX::XMFLOAT4X4 globalTransform;
		DirectX::XMFLOAT4X4 worldTransform;

		bool visible = true;
	};

	// メッシュデータ取得
	const std::vector<Node>& GetNodes() const { return nodes; }

	// トランスフォーム更新処理
	void UpdateTransform(const DirectX::XMFLOAT4X4& worldTransform);

	// ルートノード取得
	Node* GetRootNode() { return nodes.data(); }
	// ノード検索
	Node* FindNode(const char* name);

	// リソース取得
	const ModelResource* GetResource() const { return resource.get(); }

	// アニメーション再生
	void PlayAnimation(int index, bool loop, float blendSeconds = 0.2f);
	// アニメーション再生中か
	bool IsPlayAnimation() const;
	// アニメーション更新処理
	void UpdateAnimation(float elapsedTime);
	// アニメーション取得
	int GetAnimationId() { return currentAnimationIndex; }

	int GetCurrentAnimationIndex() const { return  currentAnimationIndex; }
	// 現在のアニメーション再生時間取得
	float GetCurrentAnimationSeconds() const { return  currentAnimationSeconds; }
	float GetAnimationRate() const { return currentAnimationSeconds / resource->GetAnimations().at(currentAnimationIndex).secondsLength; }
	void SetAnimationRate(float rate) { currentAnimationSeconds = resource->GetAnimations().at(currentAnimationIndex).secondsLength * rate; }

	void SetLinearGamma(float g) { linearGamma = g; }
	float GetLinearGamma() const { return linearGamma; }
private:
	// アニメーション計算処理
	void ComputeAnimation(float elapsedTime);
	// ブレンディング計算処理
	void ComputeBlending(float elapsedTime);
private:
	std::shared_ptr<ModelResource>	resource;
	std::vector<Node>				nodes;

	int   currentAnimationIndex = -1;
	float currentAnimationSeconds = 0.0f;
	bool animationPlaying = false;
	bool animationLoop = false;

	struct NodeCache
	{
		DirectX::XMFLOAT3 position = { 0, 0, 0 };
		DirectX::XMFLOAT4 rotation = { 0, 0, 0, 0 };
		DirectX::XMFLOAT3 scale = { 1, 1, 1 };
	};
	std::vector<NodeCache> nodeCaches;

	float currentAnimationBlendSeconds = 0.0f;
	float animationBlendSecondsLength = -1.0f;
	bool animationBlending = false;

	float scaling = 1.0f;

	float linearGamma = 1.0f;
};