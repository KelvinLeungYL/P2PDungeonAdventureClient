#include <filesystem>
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Misc.h"
#include "ModelResource.h"
#include "ResourceManager.h"

#include "AssimpImporter.h"
#include "GpuResourceUtils.h"

namespace DirectX
{
	template<class Archive>
	void serialize(Archive& archive, XMUINT4& v)
	{
		archive(
			cereal::make_nvp("X", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}
	template<class Archive>
	void serialize(Archive& archive, XMFLOAT2& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y)
		);
	}
	template<class Archive>
	void serialize(Archive& archive, XMFLOAT3& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z)
		);
	}
	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4& v)
	{
		archive(
			cereal::make_nvp("x", v.x),
			cereal::make_nvp("y", v.y),
			cereal::make_nvp("z", v.z),
			cereal::make_nvp("w", v.w)
		);
	}
	template<class Archive>
	void serialize(Archive& archive, XMFLOAT4X4& m)
	{
		archive(
			cereal::make_nvp("_11", m._11),
			cereal::make_nvp("_12", m._12),
			cereal::make_nvp("_13", m._13),
			cereal::make_nvp("_14", m._14),
			cereal::make_nvp("_21", m._21),
			cereal::make_nvp("_22", m._22),
			cereal::make_nvp("_23", m._23),
			cereal::make_nvp("_24", m._24),
			cereal::make_nvp("_31", m._31),
			cereal::make_nvp("_32", m._32),
			cereal::make_nvp("_33", m._33),
			cereal::make_nvp("_34", m._34),
			cereal::make_nvp("_41", m._41),
			cereal::make_nvp("_42", m._42),
			cereal::make_nvp("_43", m._43),
			cereal::make_nvp("_44", m._44)
		);
	}
}

template<class Archive>
void ModelResource::Node::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(path),
		CEREAL_NVP(parentIndex),
		CEREAL_NVP(position),
		CEREAL_NVP(rotation),
		CEREAL_NVP(scale)
	);
}
template<class Archive>
void ModelResource::Material::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(diffuseTextureFileName),
		CEREAL_NVP(normalTextureFileName),
		CEREAL_NVP(color)
	);
}
template<class Archive>
void ModelResource::Vertex::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(position),
		CEREAL_NVP(boneWeight),
		CEREAL_NVP(boneIndex),
		CEREAL_NVP(texcoord),
		CEREAL_NVP(color),
		CEREAL_NVP(normal),
		CEREAL_NVP(tangent)
	);
}
template<class Archive>
void ModelResource::Bone::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(nodeIndex),
		CEREAL_NVP(offsetTransform)
	);
}
template<class Archive>
void ModelResource::Mesh::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(vertices),
		CEREAL_NVP(indices),
		CEREAL_NVP(bones),
		CEREAL_NVP(nodeIndex),
		CEREAL_NVP(materialIndex)
	);
}
template<class Archive>
void ModelResource::VectorKeyframe::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(seconds),
		CEREAL_NVP(value)
	);
}
template<class Archive>
void ModelResource::QuaternionKeyframe::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(seconds),
		CEREAL_NVP(value)
	);
}
template<class Archive>
void ModelResource::NodeAnim::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(positionKeyframes),
		CEREAL_NVP(rotationKeyframes),
		CEREAL_NVP(scaleKeyframes)
	);
}
template<class Archive>
void ModelResource::Animation::serialize(Archive& archive)
{
	archive(
		CEREAL_NVP(name),
		CEREAL_NVP(secondsLength),
		CEREAL_NVP(nodeAnims)
	);
}

// �ǂݍ���
void ModelResource::Load(ID3D11Device* device, const char* filename)
{
	std::filesystem::path filepath(filename);
	std::filesystem::path dirpath(filepath.parent_path());

	// �Ǝ��`���̃��f���t�@�C���̑��݊m�F
	filepath.replace_extension(".cereal");
	if (std::filesystem::exists(filepath))
	{
		// �Ǝ��`���̃��f���t�@�C���̓ǂݍ���
		Deserialize(filepath.string().c_str());
	}
	else
	{
		// �ėp���f���t�@�C���ǂݍ���
		AssimpImporter importer(filename);

		// �}�e���A���f�[�^�ǂݎ��
		importer.LoadMaterials(materials);

		// �m�[�h�f�[�^�ǂݎ��
		importer.LoadNodes(nodes);

		// ���b�V���f�[�^�ǂݎ��
		importer.LoadMeshes(meshes, nodes);

		// �A�j���[�V�����f�[�g�ǂݎ��
		importer.LoadAnimations(animations, nodes);

		// �Ǝ��`���̃��f���t�@�C����ۑ�
		Serialize(filepath.string().c_str());
	}

	//// �m�[�h�\�z
	//for (size_t nodeIndex = 0; nodeIndex < nodes.size(); nodeIndex++)
	//{
	//	Node& node = nodes.at(nodeIndex);

	//	//// �e�q�֌W���\�z
	//	//node.parent = node.parentIndex >= 0 ? &nodes.at(node.parentIndex) : nullptr;
	//	//if (node.parent != nullptr)
	//	//{
	//	//	node.parent->children.emplace_back(&node);
	//	//}
	//}

	// �}�e���A���\�z
	for (Material& material : materials)
	{
		if (material.diffuseTextureFileName.empty())
		{
			// �_�~�[�e�N�X�`���쐬
			HRESULT hr = GpuResourceUtils::CreateDummyTexture(device, 0xFFFFFFFF, material.diffuseMap.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}
		else
		{
			// �f�B�t���[�Y�e�N�X�`���ǂݍ���
			std::filesystem::path diffuseTexturePath(dirpath / material.diffuseTextureFileName);
			HRESULT hr = GpuResourceUtils::LoadTexture(device, diffuseTexturePath.string().c_str(), material.diffuseMap.GetAddressOf());

			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}
		if (material.normalTextureFileName.empty())
		{
			// �@���_�~�[�e�N�X�`���쐬
			HRESULT hr = GpuResourceUtils::CreateDummyTexture(device, 0xFFFF7F7F, material.normalMap.GetAddressOf());

			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}
		else
		{
			// �@���e�N�X�`���ǂݍ���
			std::filesystem::path texturePath(dirpath / material.normalTextureFileName);
			HRESULT hr = GpuResourceUtils::LoadTexture(device, texturePath.string().c_str(), material.normalMap.GetAddressOf());
		}
	}

	// ���b�V���\�z
	for (Mesh& mesh : meshes)
	{
		// �Q�ƃm�[�h�ݒ�
		mesh.node = &nodes.at(mesh.nodeIndex);

		// �Q�ƃ}�e���A���ݒ�
		mesh.material = &materials.at(mesh.materialIndex);

		// ���_�o�b�t�@
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			D3D11_SUBRESOURCE_DATA subresourceData = {};

			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * mesh.vertices.size());
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			subresourceData.pSysMem = mesh.vertices.data();
			subresourceData.SysMemPitch = 0;
			subresourceData.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.vertexBuffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}

		// �C���f�b�N�X�o�b�t�@
		{
			D3D11_BUFFER_DESC bufferDesc = {};
			D3D11_SUBRESOURCE_DATA subresourceData = {};

			bufferDesc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * mesh.indices.size());
			bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDesc.CPUAccessFlags = 0;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			subresourceData.pSysMem = mesh.indices.data();
			subresourceData.SysMemPitch = 0;
			subresourceData.SysMemSlicePitch = 0;

			HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, mesh.indexBuffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
		}

		// �{�[���\�z
		for (Bone& bone : mesh.bones)
		{
			// �Q�ƃm�[�h�ݒ�
			bone.node = &nodes.at(bone.nodeIndex);
		}
	}
}

// �V���A���C�Y
void ModelResource::Serialize(const char* filename)
{
	std::ofstream ostream(filename, std::ios::binary);
	if (ostream.is_open())
	{
		cereal::BinaryOutputArchive archive(ostream);

		try
		{
			archive(
				CEREAL_NVP(nodes),
				CEREAL_NVP(materials),
				CEREAL_NVP(meshes),
				CEREAL_NVP(animations)
			);
		}
		catch (...)
		{
			_ASSERT_EXPR_A(false, "Model serialize failed.");
		}
	}
}

// �f�V���A���C�Y
void ModelResource::Deserialize(const char* filename)
{
	std::ifstream istream(filename, std::ios::binary);
	if (istream.is_open())
	{
		cereal::BinaryInputArchive archive(istream);
		try
		{
			archive(
				CEREAL_NVP(nodes),
				CEREAL_NVP(materials),
				CEREAL_NVP(meshes),
				CEREAL_NVP(animations)
			);
		}
		catch (...)
		{
			_ASSERT_EXPR_A(false, "Model deserialize failed.");
		}
	}
	else
	{
		_ASSERT_EXPR_A(false, "Model file not found.");
	}
}

void ModelResource::ImportAnimations(const char* filename)
{
	animations = ResourceManager::Instance().LoadModelResource(filename)->animations;
}

// �m�[�h�C���f�b�N�X���擾����
//int ModelResource::FindNodeIndex(NodeId nodeId) const
//{
//	int nodeCount = static_cast<int>(nodes.size());
//	for (int i = 0; i < nodeCount; ++i)
//	{
//		if (nodes[i].id == nodeId)
//		{
//			return i;
//		}
//	}
//	return -1;
//}