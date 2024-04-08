#include "Graphics.h"
#include "ResourceManager.h"

// ���f�����\�[�X�ǂݍ���
std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
	// ���f������
	ModelMap::iterator it = models.find(filename);
	if (it != models.end())
	{
		// �����N(����)���؂�Ă��Ȃ����m�F
		if (!it->second.expired())
		{
			// �ǂݍ��ݍς݃��f���̃��\�[�X��Ԃ�
			return it->second.lock();
		}
	}

	// �V�K���f�����\�[�X�쐬&�ǂݍ���
	std::shared_ptr<ModelResource> model = std::make_shared<ModelResource>();
	model->Load(Graphics::Instance().GetDevice(), filename);

	// �}�b�v�ɓo�^
	models[filename] = model;

	return model;
}

std::shared_ptr<Sprite> ResourceManager::LoadSpriteResource(const char* filename)
{
	// �X�v���C�g����
	SpriteMap::iterator it = sprites.find(filename);
	if (it != sprites.end())
	{
		// �����N(����)���؂�Ă��Ȃ����m�F
		if (!it->second.expired())
		{
			// �ǂݍ��ݍς݃X�v���C�g�̃��\�[�X��Ԃ�
			return it->second.lock();
		}
	}

	// �V�K�X�v���C�g���\�[�X�쐬&�ǂݍ���
	std::shared_ptr<Sprite> sprite;
	if (std::string(filename).length() == 0)
	{
		sprite = std::make_shared<Sprite>(Graphics::Instance().GetDevice());
	}
	else
	{
		sprite = std::make_shared<Sprite>(Graphics::Instance().GetDevice(), filename);
	}

	// �}�b�v�ɓo�^
	sprites[filename] = sprite;

	return sprite;
}