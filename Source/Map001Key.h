#pragma once
#include "ModelObject.h"
class Map001Key : public ModelObject
{
public:
	Map001Key();
	~Map001Key() {}

	// �X�V����
	virtual void Update(float elapsedTime) override;
	// �`�揈��
	virtual void Render(const RenderContext& rc) override;
};
