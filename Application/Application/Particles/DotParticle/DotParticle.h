#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct DotParticleData {
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��ŗL�p�����[�^�[�ǉ�
	Float3 initScale;
	bool isUpdate;
};

class DotParticle : public BaseParticleEffect<DotParticleData>
{
public:
	DotParticle(ModelManager::ModelData& model);

protected:
	DotParticleData CreateParticle(const Float3& pos) override;

	void UpdateParticle(DotParticleData& p, float dt) override;
};
