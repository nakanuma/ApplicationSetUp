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

	// その他固有パラメーター追加
};

class CircleParticle : public BaseParticleEffect<CircleParticleData> 
{
public:
	CircleParticle(ModelManager::ModelData& model);

protected:
	CircleParticleData CreateParticle(const Float3& pos) override;

	void UpdateParticle(CircleParticleData& p, float dt) override;
};
