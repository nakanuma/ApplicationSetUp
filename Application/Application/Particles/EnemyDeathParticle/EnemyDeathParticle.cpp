#include "EnemyDeathParticle.h"

// C++
#include < random>

EnemyDeathParticle::EnemyDeathParticle(ModelManager::ModelData& model) 
{
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();
}

EnemyDeathParticleData EnemyDeathParticle::CreateParticle(const Float3& pos) 
{ 
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> rand(-1.0f, 1.0f);

	EnemyDeathParticleData p;
	p.transform.translate = pos + Float3(rand(rng), rand(rng), rand(rng));
	p.transform.rotate = { 0.0f, 0.0f, 0.0f };
	p.transform.scale = { 1.0f, 1.0f, 1.0f };
	p.velocity = { 0.0f, 1.0f, 0.0f };
	p.rotationSpeed = { 0.0f, 0.0f, 0.0f };
	p.color = { 1.0f, 0.5f, 0.2f, 1.0f };
	p.lifeTime = 2.0f;
	p.currentTime = 0.0f;
	return p;
}

void EnemyDeathParticle::UpdateParticle(EnemyDeathParticleData& p, float dt) 
{
	p.transform.translate += p.velocity * dt;
	float lifeRatio = 1.0f - (p.currentTime / p.lifeTime);
	p.transform.scale = {lifeRatio, lifeRatio, lifeRatio};
	p.color = {1.0f, lifeRatio, 0.0f, 1.0f};
	p.transform.rotate.z += 0.01f;
	p.transform.rotate.x += 0.01f;
}
