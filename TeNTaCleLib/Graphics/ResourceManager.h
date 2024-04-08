#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "ModelResource.h"
#include "Sprite.h"

// ���\�[�X�}�l�[�W���[
class ResourceManager
{
private:
	ResourceManager() {}
	~ResourceManager() {}
public:
	// �B��̃C���X�^���X�擾
	static ResourceManager& Instance()
	{
		static ResourceManager instance;
		return instance;
	}

	// ���f�����\�[�X�ǂݍ���
	std::shared_ptr<ModelResource> LoadModelResource(const char* filename);
	std::shared_ptr<Sprite> LoadSpriteResource(const char* filename);

private:
	using ModelMap = std::unordered_map<std::string, std::weak_ptr<ModelResource>>;
	ModelMap models;
	using SpriteMap = std::unordered_map<std::string, std::weak_ptr<Sprite>>;
	SpriteMap sprites;
};

#define RESOURCE ResourceManager::Instance()