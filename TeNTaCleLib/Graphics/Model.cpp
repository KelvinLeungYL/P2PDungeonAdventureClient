#include <filesystem>
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include "Misc.h"
#include "Model.h"
#include "ResourceManager.h"

// �R���X�g���N�^
Model::Model(ID3D11Device* device, const char* filename, float scaling) : scaling(scaling)
{
	resource = ResourceManager::Instance().LoadModelResource(filename);

	// �m�[�h�L���b�V��
	const std::vector<ModelResource::Node>& resNodes = resource->GetNodes();
	nodeCaches.resize(resNodes.size());
	nodes.resize(resNodes.size());
	for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
	{
		auto&& src = resNodes.at(nodeIndex);
		auto&& dst = nodes.at(nodeIndex);

		dst.name = src.name.c_str();
		dst.parent = src.parentIndex >= 0 ? &nodes.at(src.parentIndex) : nullptr;
		dst.scale = src.scale;
		dst.rotation = src.rotation;
		dst.position = src.position;

		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}
}

// �A�j���[�V�����Đ�
void Model::PlayAnimation(int index, bool loop, float blendSeconds)
{
	currentAnimationIndex = index;
	currentAnimationSeconds = 0;
	animationLoop = loop;
	animationPlaying = true;

	// �u�����h�p�����[�^
	animationBlending = blendSeconds > 0.0f;
	currentAnimationBlendSeconds = 0.0f;
	animationBlendSecondsLength = blendSeconds;

	// ���݂̎p�����L���b�V������
	for (size_t i = 0; i < nodes.size(); ++i)
	{
		const Model::Node& src = nodes.at(i);
		NodeCache& dst = nodeCaches.at(i);

		dst.position = src.position;
		dst.rotation = src.rotation;
		dst.scale = src.scale;
	}
}
// �A�j���[�V�����Đ�����
bool Model::IsPlayAnimation() const
{
	if (currentAnimationIndex < 0) return false;
	if (currentAnimationIndex > resource->GetAnimations().size()) return false;
	return animationPlaying;
}
// �A�j���[�V�����X�V����
void Model::UpdateAnimation(float elapsedTime)
{
	ComputeAnimation(elapsedTime);
	ComputeBlending(elapsedTime);
}
void Model::ComputeAnimation(float elapsedTime)
{
	if (!IsPlayAnimation()) return;

	// �w��̃A�j���[�V�����f�[�^���擾
	const ModelResource::Animation& animation = resource->GetAnimations().at(currentAnimationIndex);

	// �m�[�h���̃A�j���[�V�����f�[�^����
	for (size_t nodeIndex = 0; nodeIndex < animation.nodeAnims.size(); nodeIndex++)
	{
		Node& node = nodes.at(nodeIndex);

		const ModelResource::NodeAnim& nodeAnim = animation.nodeAnims.at(nodeIndex);

		// �ʒu
		for (size_t index = 0; index < nodeAnim.positionKeyframes.size() - 1; index++)
		{
			//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩��������
			const ModelResource::VectorKeyframe& keyframe0 = nodeAnim.positionKeyframes.at(index);
			const ModelResource::VectorKeyframe& keyframe1 = nodeAnim.positionKeyframes.at(index + 1);
			if (currentAnimationSeconds >= keyframe0.seconds && currentAnimationSeconds < keyframe1.seconds)
			{
				// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
				float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);
				// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
				DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
				DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
				DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
				// �v�Z���ʂ��m�[�h�Ɋi�[
				DirectX::XMStoreFloat3(&node.position, V);
			}
		}
		// ��]
		for (size_t index = 0; index < nodeAnim.rotationKeyframes.size() - 1; index++)
		{
			//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩��������
			const ModelResource::QuaternionKeyframe& keyframe0 = nodeAnim.rotationKeyframes.at(index);
			const ModelResource::QuaternionKeyframe& keyframe1 = nodeAnim.rotationKeyframes.at(index + 1);
			if (currentAnimationSeconds >= keyframe0.seconds && currentAnimationSeconds < keyframe1.seconds)
			{
				// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
				float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);
				// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
				DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(&keyframe0.value);
				DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&keyframe1.value);
				DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(Q0, Q1, rate);
				// �v�Z���ʂ��m�[�h�Ɋi�[
				DirectX::XMStoreFloat4(&node.rotation, Q);
			}
		}
		// �X�P�[��
		for (size_t index = 0; index < nodeAnim.scaleKeyframes.size() - 1; index++)
		{
			//���݂̎��Ԃ��ǂ̃L�[�t���[���̊Ԃɂ��邩��������
			const ModelResource::VectorKeyframe& keyframe0 = nodeAnim.scaleKeyframes.at(index);
			const ModelResource::VectorKeyframe& keyframe1 = nodeAnim.scaleKeyframes.at(index + 1);
			if (currentAnimationSeconds >= keyframe0.seconds && currentAnimationSeconds < keyframe1.seconds)
			{
				// �Đ����ԂƃL�[�t���[���̎��Ԃ���⊮�����Z�o����
				float rate = (currentAnimationSeconds - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);
				// �O�̃L�[�t���[���Ǝ��̃L�[�t���[���̎p����⊮
				DirectX::XMVECTOR V0 = DirectX::XMLoadFloat3(&keyframe0.value);
				DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&keyframe1.value);
				DirectX::XMVECTOR V = DirectX::XMVectorLerp(V0, V1, rate);
				// �v�Z���ʂ��m�[�h�Ɋi�[
				DirectX::XMStoreFloat3(&node.scale, V);
			}
		}
	}

	// ���Ԍo��
	currentAnimationSeconds += elapsedTime;

	// �Đ����Ԃ��I�[���Ԃ𒴂�����
	if (currentAnimationSeconds >= animation.secondsLength)
	{
		if (animationLoop)
		{
			// �Đ����Ԃ������߂�
			currentAnimationSeconds -= animation.secondsLength;
		}
		else
		{
			// �Đ��I�����Ԃɂ���
			currentAnimationSeconds = animation.secondsLength;
			animationPlaying = false;
		}
	}
}

// �u�����f�B���O�v�Z����
void Model::ComputeBlending(float elapsedTime)
{
	if (!animationBlending) return;

	// �u�����h���̌v�Z
	float rate = currentAnimationBlendSeconds / animationBlendSecondsLength;
	// �u�����h�v�Z
	int count = static_cast<int>(nodes.size());
	for (int i = 0; i < count; i++)
	{
		const NodeCache& cache = nodeCaches.at(i);
		Node& node = nodes.at(i);

		DirectX::XMVECTOR S0 = DirectX::XMLoadFloat3(&cache.scale);
		DirectX::XMVECTOR S1 = DirectX::XMLoadFloat3(&node.scale);
		DirectX::XMVECTOR R0 = DirectX::XMLoadFloat4(&cache.rotation);
		DirectX::XMVECTOR R1 = DirectX::XMLoadFloat4(&node.rotation);
		DirectX::XMVECTOR T0 = DirectX::XMLoadFloat3(&cache.position);
		DirectX::XMVECTOR T1 = DirectX::XMLoadFloat3(&node.position);

		DirectX::XMVECTOR S = DirectX::XMVectorLerp(S0, S1, rate);
		DirectX::XMVECTOR R = DirectX::XMQuaternionSlerp(R0, R1, rate);
		DirectX::XMVECTOR T = DirectX::XMVectorLerp(T0, T1, rate);

		DirectX::XMStoreFloat3(&node.scale, S);
		DirectX::XMStoreFloat4(&node.rotation, R);
		DirectX::XMStoreFloat3(&node.position, T);
	}

	// ���Ԍo��
	currentAnimationBlendSeconds += elapsedTime;
	if (currentAnimationBlendSeconds >= animationBlendSecondsLength)
	{
		currentAnimationBlendSeconds = animationBlendSecondsLength;
		animationBlending = false;
	}
}

void Model::UpdateTransform(const DirectX::XMFLOAT4X4& worldTransform)
{
	DirectX::XMMATRIX ParentWorldTransform = DirectX::XMLoadFloat4x4(&worldTransform);

	// �E����W�n���獶����W�n�֕ϊ�����s��
	DirectX::XMMATRIX CoordinateSystemTransform = DirectX::XMMatrixScaling(-scaling, scaling, scaling);

	for (Node& node : nodes)
	{
		if (!node.visible) node.scale = {};

		// ���[�J���s��Z�o
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotation));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.position.x, node.position.y, node.position.z);
		DirectX::XMMATRIX LocalTransform = S * R * T;

		// �O���[�o���s��Z�o
		DirectX::XMMATRIX ParentGlobalTransform;
		if (node.parent != nullptr)
		{
			ParentGlobalTransform = DirectX::XMLoadFloat4x4(&node.parent->globalTransform);
		}
		else
		{
			ParentGlobalTransform = DirectX::XMMatrixIdentity();
		}
		DirectX::XMMATRIX GlobalTransform = LocalTransform * ParentGlobalTransform;

		// ���[���h�s��Z�o
		DirectX::XMMATRIX WorldTransform = GlobalTransform * CoordinateSystemTransform * ParentWorldTransform;

		// �v�Z���ʂ��i�[
		DirectX::XMStoreFloat4x4(&node.localTransform, LocalTransform);
		DirectX::XMStoreFloat4x4(&node.globalTransform, GlobalTransform);
		DirectX::XMStoreFloat4x4(&node.worldTransform, WorldTransform);
	}
}

Model::Node* Model::FindNode(const char* name)
{
	// �S�Ẵm�[�h�𑍓�����Ŗ��O��r����
	int nodeCount = static_cast<int>(nodes.size());
	for (Node& node : nodes)
	{
		if (node.name == name)
		{
			return &node;
		}
	}
	// ������Ȃ�����
	return nullptr;
}