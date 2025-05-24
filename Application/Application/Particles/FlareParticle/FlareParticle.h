#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/Model/ModelManager.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>

struct FlareParticleData {
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��ŗL�p�����[�^�[�ǉ�
	Float3 initScale;
	bool isUpdate;
};

class FlareParticle : public BaseParticleEffect<FlareParticleData> {
public:
	FlareParticle(ModelManager::ModelData& model);

protected:
	FlareParticleData CreateParticle(const Float3& pos) override;

	void UpdateParticle(FlareParticleData& p, float dt) override;

private:
	bool isUpdate_;
};