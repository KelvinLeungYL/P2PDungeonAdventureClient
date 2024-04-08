#include "ModelObject.h"
#include "UnrotatedBoxCollider.h"
#include "ModelCollider.h"
#include "SphereCollider.h"
#include "BoundingBoxCollider.h"

ModelObject::ModelObject(const char* filename, float scaling)
{
	model = std::make_unique<Model>(T_GRAPHICS.GetDevice(), filename, scaling);
}

void ModelObject::SetShader(ModelShaderId id)
{
	shaderId = id;
}

void ModelObject::SetAnimation(int index, bool loop, float blendSeconds)
{
	model->PlayAnimation(index, loop, blendSeconds);
}

void ModelObject::Update(float elapsedTime)
{
	// �s��X�V
	UpdateTransform();

	// �A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime * animationSpeed);

	// �g�����X�t�H�[���X�V
	model->UpdateTransform(transform);
}

void ModelObject::Render(const RenderContext& rc)
{
	if (!visible) return;
	// �`��
	ModelShader* shader = T_GRAPHICS.GetModelShader(shaderId);
	shader->Begin(rc);
	shader->Draw(rc, model.get(), color);
	shader->End(rc);
}

void ModelObject::SetCollider(Collider::COLLIDER_TYPE collider)
{
	switch (collider)
	{
	case Collider::COLLIDER_TYPE::UNROTATED_BOX:
		this->collider = std::make_unique<UnrotatedBoxCollider>();
		break;
	case Collider::COLLIDER_TYPE::MODEL:
		this->collider = std::make_unique<ModelCollider>(model.get());
		break;
	case Collider::COLLIDER_TYPE::SPHERE:
		this->collider = std::make_unique<SphereCollider>();
		break;
	case Collider::COLLIDER_TYPE::BOUNDING_BOX:
		this->collider = std::make_unique<BoundingBoxCollider>(model.get());
		break;
	default:
		this->collider = nullptr;
		break;
	}
}

DirectX::XMFLOAT3 ModelObject::GetNodePosition(const char* nodeName, const DirectX::XMFLOAT3& offset)
{
	DirectX::XMFLOAT3 pos = {};
	Model::Node* node = model->FindNode(nodeName);
	if (node == nullptr) return pos;

	DirectX::XMVECTOR Offset = DirectX::XMLoadFloat3(&offset);
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&node->worldTransform);
	DirectX::XMStoreFloat3(&pos, DirectX::XMVector3Transform(Offset, W));

	return pos;
}

DirectX::XMFLOAT3 ModelObject::GetNodePosition(const DirectX::XMFLOAT3& offset)
{
	DirectX::XMFLOAT3 pos = {};
	Model::Node* node = model->GetRootNode();

	DirectX::XMVECTOR Offset = DirectX::XMLoadFloat3(&offset);
	DirectX::XMMATRIX W = DirectX::XMLoadFloat4x4(&node->worldTransform);
	DirectX::XMStoreFloat3(&pos, DirectX::XMVector3Transform(Offset, W));

	return pos;
}