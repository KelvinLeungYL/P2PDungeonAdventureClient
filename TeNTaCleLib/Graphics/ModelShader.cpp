#include "Misc.h"
#include "GpuResourceUtils.h"
#include "ModelShader.h"

ModelShader::ModelShader(ID3D11Device* device, const char* vs, const char* ps)
{
	// ���̓��C�A�E�g
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// ���_�V�F�[�_�[
	GpuResourceUtils::LoadVertexShader(
		device,
		vs,
		inputElementDesc,
		_countof(inputElementDesc),
		inputLayout.GetAddressOf(),
		vertexShader.GetAddressOf()
	);

	// �s�N�Z���V�F�[�_�[
	GpuResourceUtils::LoadPixelShader(
		device,
		ps,
		pixelShader.GetAddressOf()
	);
	// �V�[���p�萔�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CbScene),
		sceneConstantBuffer.GetAddressOf()
	);
	// ���b�V���p�萔�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CbMesh),
		meshConstantBuffer.GetAddressOf()
	);
	// �X�P���g���p�萔�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CbSkeleton),
		skeletonConstantBuffer.GetAddressOf()
	);
}

// �`��J�n
void ModelShader::Begin(const RenderContext& rc)
{
	ID3D11DeviceContext* dc = rc.deviceContext;

	// �V�F�[�_�[�ݒ�
	dc->IASetInputLayout(inputLayout.Get());
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	// �萔�o�b�t�@�ݒ�
	ID3D11Buffer* constantBuffers[] =
	{
		sceneConstantBuffer.Get(),
		meshConstantBuffer.Get(),
		skeletonConstantBuffer.Get(),
	};
	rc.deviceContext->VSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);
	rc.deviceContext->PSSetConstantBuffers(0, _countof(constantBuffers), constantBuffers);

	// �T���v���X�e�[�g
	ID3D11SamplerState* samplerStates[] =
	{
		rc.renderState->GetSamplerState(SamplerState::LinearWrap)
	};
	dc->PSSetSamplers(0, _countof(samplerStates), samplerStates);

	// �����_�[�X�e�[�g�ݒ�
	SetRenderState(rc);

	// �V�[���p�萔�o�b�t�@�X�V
	CbScene cbScene{};
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.camera->GetView());
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.camera->GetProjection());
	DirectX::XMStoreFloat4x4(&cbScene.viewProjection, V * P);

	cbScene.directionalLightData = rc.directionalLightData;
	cbScene.ambientLightColor = rc.ambientLightColor;
	memcpy_s(
		cbScene.pointLightData,
		sizeof(cbScene.pointLightData),
		rc.pointLightData,
		sizeof(rc.pointLightData)
	);
	cbScene.pointLightCount = rc.pointLightCount;
	memcpy_s(
		cbScene.spotLightData,
		sizeof(cbScene.spotLightData),
		rc.spotLightData,
		sizeof(rc.spotLightData)
	);
	cbScene.spotLightCount = rc.spotLightCount;

	const DirectX::XMFLOAT3& eye = rc.camera->GetEye();
	cbScene.cameraPosition.x = eye.x;
	cbScene.cameraPosition.y = eye.y;
	cbScene.cameraPosition.z = eye.z;
	dc->UpdateSubresource(sceneConstantBuffer.Get(), 0, 0, &cbScene, 0, 0);
}

// ���f���`��
void ModelShader::Draw(const RenderContext& rc, const Model* model, DirectX::XMFLOAT4 color)
{
	ID3D11DeviceContext* dc = rc.deviceContext;

	const ModelResource* resource = model->GetResource();
	const std::vector<Model::Node>& nodes = model->GetNodes();

	for (const ModelResource::Mesh& mesh : resource->GetMeshes())
	{
		// ���_�o�b�t�@�ݒ�
		UINT stride = sizeof(ModelResource::Vertex);
		UINT offset = 0;
		dc->IASetVertexBuffers(0, 1, mesh.vertexBuffer.GetAddressOf(), &stride, &offset);
		dc->IASetIndexBuffer(mesh.indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// ���b�V���p�萔�o�b�t�@�X�V
		CbMesh cbMesh{};
		::memset(&cbMesh, 0, sizeof(cbMesh));
		cbMesh.materialColor = mesh.material->color;

		cbMesh.materialColor.x *= color.x;
		cbMesh.materialColor.y *= color.y;
		cbMesh.materialColor.z *= color.z;
		cbMesh.materialColor.w *= color.w;

		cbMesh.linearGamma = model->GetLinearGamma();
		dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &cbMesh, 0, 0);

		// �X�P���g���p�萔�o�b�t�@�X�V
		CbSkeleton cbSkeleton{};
		if (mesh.bones.size() > 0)
		{
			for (size_t i = 0; i < mesh.bones.size(); ++i)
			{
				DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.bones.at(i).nodeIndex).worldTransform);
				DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.bones.at(i).offsetTransform);
				DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
				DirectX::XMStoreFloat4x4(&cbSkeleton.boneTransforms[i], boneTransform);
			}
		}
		else
		{
			cbSkeleton.boneTransforms[0] = nodes.at(mesh.nodeIndex).worldTransform;
		}
		rc.deviceContext->UpdateSubresource(skeletonConstantBuffer.Get(), 0, 0, &cbSkeleton, 0, 0);

		// �V�F�[�_�[���\�[�X�r���[�ݒ�
		SetShaderResourceView(mesh, dc);

		// �`��
		dc->DrawIndexed(static_cast<UINT>(mesh.indices.size()), 0, 0);
	}
}