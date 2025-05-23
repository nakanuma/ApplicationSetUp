#include "FlareParticle.h"

// C++
#include <random>
#include <numbers>

FlareParticle::FlareParticle(ModelManager::ModelData& model) 
{
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();

	// ビルボード適用設定
	isBillboard_ = {false, false, false};
	// ブレンドモード設定
	blendMode_ = BlendMode::Add;
}

FlareParticleData FlareParticle::CreateParticle(const Float3& pos) 
{ 
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> rand(0.0f, std::numbers::pi_v<float>);

	FlareParticleData p;
	p.transform.translate = pos;
	p.transform.rotate = {rand(rng), 0.0f, 0.0f};
	p.transform.scale = {5.0f, 0.2f, 0.2f};
	p.velocity = {0.0f, 0.0f, 0.0f};
	p.color = {1.0f, 1.0f, 1.0f, 1.0f};
	p.lifeTime = 1.0f;
	p.currentTime = 0.0f;

	return p;
}

void FlareParticle::UpdateParticle(FlareParticleData& p, float dt) 
{
	// Xのみ拡大
	/*p.transform.scale.x *= 1.05f;*/
}
