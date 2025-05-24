#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct CircleParticleData {
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��ŗL�p�����[�^�[�ǉ�
	Float3 initScale;
	bool isUpdate;
};

class CircleParticle : public BaseParticleEffect<CircleParticleData> 
{
public:
	CircleParticle(ModelManager::ModelData& model);

protected:
	CircleParticleData CreateParticle(const Float3& pos) override;

	void UpdateParticle(CircleParticleData& p, float dt) override;
};
