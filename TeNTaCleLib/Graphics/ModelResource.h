#pragma once

#include <string>
#include <vector>
#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class ModelResource
{
public:
	ModelResource() {}
	virtual ~ModelResource() {}

	//using NodeId = UINT64;

	struct Node
	{
		//NodeId id;
		std::string name;
		std::string path;
		int parentIndex;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 rotation;
		DirectX::XMFLOAT3 scale;

		template<class Archive>
		void serialize(Archive& archive);
	};

	struct Material
	{
		std::string name;
		std::string diffuseTextureFileName;
		DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseMap;
		std::string normalTextureFileName;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap;

		template<class Archive>
		void serialize(Archive& archive);
	};

	struct Vertex
	{
		DirectX::XMFLOAT3 position = { 0, 0, 0 };
		DirectX::XMFLOAT4 boneWeight = { 1, 0, 0, 0 };
		DirectX::XMUINT4 boneIndex = { 0, 0, 0, 0 };
		DirectX::XMFLOAT2 texcoord = { 0, 0 };
		DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
		DirectX::XMFLOAT3 normal = { 0, 0, 0 };
		DirectX::XMFLOAT3 tangent = { 0, 0, 0 };

		template<class Archive>
		void serialize(Archive& archive);
	};

	struct Bone
	{
		int nodeIndex;
		DirectX::XMFLOAT4X4 offsetTransform;
		Node* node = nullptr;

		template<class Archive>
		void serialize(Archive& archive);
	};

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

		int materialIndex = 0;

		Material* material = nullptr;

		int nodeIndex = 0;
		Node* node = nullptr;

		std::vector<Bone> bones;

		template<class Archive>
		void serialize(Archive& archive);
	};

	struct VectorKeyframe
	{
		float seconds;
		DirectX::XMFLOAT3 value;

		template<class Archive>
		void serialize(Archive& archive);
	};
	struct QuaternionKeyframe
	{
		float seconds;
		DirectX::XMFLOAT4 value;

		template<class Archive>
		void serialize(Archive& archive);
	};
	struct NodeAnim
	{
		std::vector <VectorKeyframe> positionKeyframes;
		std::vector<QuaternionKeyframe> rotationKeyframes;
		std::vector<VectorKeyframe> scaleKeyframes;

		template<class Archive>
		void serialize(Archive& archive);
	};
	struct Animation
	{
		std::string name;
		float secondsLength;
		std::vector<NodeAnim> nodeAnims;

		template<class Archive>
		void serialize(Archive& archive);
	};

	// 各種データ取得
	const std::vector<Mesh>& GetMeshes() const { return meshes; }
	const std::vector<Node>& GetNodes() const { return nodes; }
	const std::vector<Animation>& GetAnimations() const { return animations; }
	const std::vector<Material>& GetMaterials() const { return materials; }

	// 読み込み
	void Load(ID3D11Device* device, const char* filename);

protected:

	// シリアライズ
	void Serialize(const char* filename);

	// デシリアライズ
	void Deserialize(const char* filename);

	// ノードインデックスを取得する
	//int FindNodeIndex(NodeId nodeId) const;
public:
	// コピーアニメーション
	void ImportAnimations(const char* filename);
protected:
	std::vector<Node>		nodes;
	std::vector<Material>	materials;
	std::vector<Mesh>		meshes;
	std::vector<Animation>	animations;
};