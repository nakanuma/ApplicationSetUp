#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct EnemyDeathParticleData 
{
	Transform transform;
	Float3 velocity;
	Float3 rotationSpeed;
	Float4 color;
	float lifeTime = 2.0f;
	float currentTime = 0.0f;
};

class EnemyDeathParticle : public BaseParticleEffect<EnemyDeathParticleData>
{
public:
	EnemyDeathParticle(ModelManager::ModelData& model);

protected:
	EnemyDeathParticleData CreateParticle(const Float3& pos) override;

	void UpdateParticle(EnemyDeathParticleData& p, float dt) override;
};
