#include <imgui.h>
#include <functional>
#include "tentacle_lib.h"
#include "Scene.h"
#include "TransformUtils.h"
#include "Graphics\/ModelResource.h"

// ������
void ModelTestScene::Initialize()
{
	ID3D11Device* device = Graphics::Instance().GetDevice();
	float screenWidth = Graphics::Instance().GetScreenWidth();
	float screenHeight = Graphics::Instance().GetScreenHeight();

	// �J�����ݒ�
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),	// ��p
		screenWidth / screenHeight,			// ��ʃA�X�y�N�g��
		0.1f,								// �j�A�N���b�v
		10000.0f							// �t�@�[�N���b�v
	);
	camera.SetLookAt(
		{ 0, 8, 15 },	// ���_
		{ 0, 2, 0 },	// �����_
		{ 0, 1, 0 }		// ��x�N�g��
	);

	// ���f���쐬
	//model = std::make_unique<Model>(device, "Data/Model/Slime/Slime.fbx", 1.0f);
	//model = std::make_unique<Model>(device, "Data/Model/Mari.O/Mari-O.fbx", 1.0f);
	model = std::make_unique<Model>(device, "Data/Model/Cube/testCubes.glb", 1.0f);
	//model->PlayAnimation(1, true);
	//model = std::make_unique<Model>(device, "Data/Model/MariLuiHouse/MariLuiHouse.fbx", 1.0f);
	//model->SetLinearGamma(2.2f);

	cameraController.SyncCameraToController(camera);

	LightManager::Instance().SetAmbientColor({ 0, 0, 0, 0 });
	Light* dl = new Light(LightType::Directional);
	dl->SetDirection({ 0.0f, -0.503f, -0.864f });
	LightManager::Instance().Register(dl);

	// �_������ǉ�
	{
		Light* light = new Light(LightType::Point);
		light->SetPosition(DirectX::XMFLOAT3(0, 0, 0));
		light->SetColor(DirectX::XMFLOAT4(1, 1, 1, 1));
		LightManager::Instance().Register(light);
	}

	// �|�X�g�v���Z�X�`��N���X����
	{
		postprocessingRenderer = std::make_unique<PostprocessingRenderer>();
		//postprocessingRenderer->SetBloom
		//(
		//	{
		//		{0.23f, 0.813f},
		//		{2, 10.0f}
		//	}
		//);
		//postprocessingRenderer->SetColoGrading
		//(
		//	{
		//		0,
		//		2.0f,
		//		0.813f
		//	}
		//);
	}
}

// �X�V����
void ModelTestScene::Update(float elapsedTime)
{
	// �J���������X�V
	cameraController.Update();
	cameraController.SyncContrllerToCamera(camera);

	// ���[���h�s��v�Z
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	DirectX::XMFLOAT4X4 worldTransform;
	DirectX::XMStoreFloat4x4(&worldTransform, S * R * T);

	// �A�j���[�V�����X�V
	model->UpdateAnimation(elapsedTime);

	// �g�����X�t�H�[���X�V
	model->UpdateTransform(worldTransform);
}

// �`�揈��
void ModelTestScene::Render()
{
	T_GRAPHICS.GetFrameBuffer(FrameBufferId::Scene)->Clear(T_GRAPHICS.GetDeviceContext(), 0.5f, 0.5f, 0.5f, 1);
	T_GRAPHICS.GetFrameBuffer(FrameBufferId::Scene)->SetRenderTargets(T_GRAPHICS.GetDeviceContext());

	// �`��R���e�L�X�g�ݒ�
	RenderContext rc;
	rc.camera = &camera;
	rc.deviceContext = T_GRAPHICS.GetDeviceContext();
	rc.renderState = T_GRAPHICS.GetRenderState();

	// ���C�g�̏����l�ߍ���
	LightManager::Instance().PushRenderContext(rc);

	// �`��
	ModelShader* shader = T_GRAPHICS.GetModelShader(ModelShaderId::Phong);
	shader->Begin(rc);
	shader->Draw(rc, model.get());
	shader->End(rc);

	// ���C�g�̃f�o�b�O�v���~�e�B�u�̕`��
	LightManager::Instance().DrawDebugPrimitive();
	postprocessingRenderer->DrawDebugGUI();

	// ���C�g�̃f�o�b�O�v���~�e�B�u�̕`��
	LightManager::Instance().DrawDebugGUI();
	// ���C�������_���`����s
	T_GRAPHICS.GetLineRenderer()->Render(T_GRAPHICS.GetDeviceContext(), camera.GetView(), camera.GetProjection());
	// �f�o�b�O�����_���`����s
	T_GRAPHICS.GetDebugRenderer()->Render(T_GRAPHICS.GetDeviceContext(), camera.GetView(), camera.GetProjection());

	// �������ݐ���o�b�N�o�b�t�@�ɕς��ăI�t�X�N���[�������_�����O�̌��ʂ�`�悷��
	{
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::Display)->Clear(T_GRAPHICS.GetDeviceContext(), 0.0f, 0.0f, 0.5f, 1.0f);
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::Display)->SetRenderTargets(T_GRAPHICS.GetDeviceContext());

		//	�|�X�g�v���Z�X�������s��
		postprocessingRenderer->Render(T_GRAPHICS.GetDeviceContext());
	}
	T_TEXT.Begin();

	T_TEXT.TextBox(
		FONT_ID::HGpop,
		std::string("�}���[�E�Z�����C�[�E�f�@�q�o�f�@�}���[�E�Z�����C�[�E�f�@�q�o�f�@�}���[�E�Z�����C�[�E�f�@�q�o�f�@�}���[�E�Z�����C�[�E�f�@�q�o�f�@�}���[�E�Z�����C�[�E�f�@�q�o�f�@�}���[�E�Z�����C�[�E�f�@�q�o�f"),
		10,
		0, 0,
		1, 1, 1, 1,
		0.7f
	);
	T_TEXT.End();

	// �f�o�b�O���j���[�`��
	DrawSceneGUI();
	DrawPropertyGUI();
	DrawAnimationGUI();
}

// �V�[��GUI�`��
void ModelTestScene::DrawSceneGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->Pos;
	ImGui::SetNextWindowPos(ImVec2(pos.x + 10, pos.y + 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	if (ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_None))
	{
		DirectX::XMFLOAT3 eye = camera.GetEye();
		ImGui::DragFloat3("Eye", &eye.x, 0.01f, 100.0f);

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �ʒu
			ImGui::DragFloat3("Position", &position.x, 0.1f);

			// ��]
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(angle.x);
			a.y = DirectX::XMConvertToDegrees(angle.y);
			a.z = DirectX::XMConvertToDegrees(angle.z);
			ImGui::DragFloat3("Angle", &a.x, 1.0f);
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);

			// �X�P�[��
			ImGui::DragFloat3("Scale", &scale.x, 0.01f);
		}

		if (ImGui::CollapsingHeader("Hierarchy", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �m�[�h�c���[���ċA�I�ɕ`�悷��֐�
			std::function<void(Model::Node*)> drawNodeTree = [&](Model::Node* node)
				{
					// �����N���b�N�A�܂��̓m�[�h���_�u���N���b�N�ŊK�w���J��
					ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

					// �q�����Ȃ��ꍇ�͖������Ȃ�
					size_t childCount = node->children.size();
					if (childCount == 0)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
					}

					// �I���t���O
					if (selectionNode == node)
					{
						nodeFlags |= ImGuiTreeNodeFlags_Selected;
					}

					// �c���[�m�[�h��\��
					bool opened = ImGui::TreeNodeEx(node, nodeFlags, node->name.c_str());

					// �t�H�[�J�X���ꂽ�m�[�h��I������
					if (ImGui::IsItemFocused())
					{
						selectionNode = node;
					}

					// �J����Ă���ꍇ�A�q�K�w�������������s��
					if (opened && childCount > 0)
					{
						for (Model::Node* child : node->children)
						{
							drawNodeTree(child);
						}
						ImGui::TreePop();
					}
				};
			// �ċA�I�Ƀm�[�h��`��
			drawNodeTree(model->GetRootNode());
		}

		ImGui::End();
	}
}

// �v���p�e�BGUI�`��
void ModelTestScene::DrawPropertyGUI()
{
	ImVec2 pos = ImGui::GetMainViewport()->Pos;
	ImGui::SetNextWindowPos(ImVec2(pos.x + 970, pos.y + 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Property", nullptr, ImGuiWindowFlags_None);

	if (selectionNode != nullptr)
	{
		if (ImGui::CollapsingHeader("Node", ImGuiTreeNodeFlags_DefaultOpen))
		{
			// �ʒu
			ImGui::DragFloat3("Position", &selectionNode->position.x, 0.1f);

			// ��]
			DirectX::XMFLOAT3 angle;
			TransformUtils::QuaternionToRollPitchYaw(selectionNode->rotation, angle.x, angle.y, angle.z);
			angle.x = DirectX::XMConvertToDegrees(angle.x);
			angle.y = DirectX::XMConvertToDegrees(angle.y);
			angle.z = DirectX::XMConvertToDegrees(angle.z);
			if (ImGui::DragFloat3("Rotation", &angle.x, 1.0f))
			{
				angle.x = DirectX::XMConvertToRadians(angle.x);
				angle.y = DirectX::XMConvertToRadians(angle.y);
				angle.z = DirectX::XMConvertToRadians(angle.z);
				DirectX::XMVECTOR Rotation = DirectX::XMQuaternionRotationRollPitchYaw(angle.x, angle.y, angle.z);
				DirectX::XMStoreFloat4(&selectionNode->rotation, Rotation);
			}
			// �X�P�[��
			ImGui::DragFloat3("Scale", &selectionNode->scale.x, 0.01f);
		}
	}
	ImGui::End();
}

// �A�j���[�V����GUI�`��
void ModelTestScene::DrawAnimationGUI()
{
	ImGui::SetNextWindowPos(ImVec2(10, 350), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

	ImGui::Begin("Animation", nullptr, ImGuiWindowFlags_None);

	ImGui::Checkbox("Loop", &animationLoop);

	int index = 0;
	ImGui::DragFloat("BlendSec", &animationBlendSeconds, 0.01f);

	const ModelResource* resource = model->GetResource();
	for (const ModelResource::Animation& animation : resource->GetAnimations())
	{
		ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf;

		ImGui::TreeNodeEx(&animation, nodeFlags, animation.name.c_str());

		// �_�u���N���b�N�ŃA�j���[�V�����Đ�
		if (ImGui::IsItemClicked())
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				model->PlayAnimation(index, animationLoop, animationBlendSeconds);
			}
		}
		ImGui::TreePop();
		index++;
	}

	ImGui::End();
}