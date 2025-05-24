#include "SparkAParticle.h"

// C++
#include <random>

// Engine
#include <Engine/Math/Easing.h>

SparkAParticle::SparkAParticle(ModelManager::ModelData& model) {
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();

	// ビルボード適用設定
	isBillboard_ = { true, true, true };
	// ブレンドモード設定
	blendMode_ = BlendMode::Add;
}

SparkAParticleData SparkAParticle::CreateParticle(const Float3& pos) {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> rand(-0.2f, 0.2f);

	std::uniform_real_distribution<float> randLifetime(0.2f, 0.4f);

	SparkAParticleData p;
	p.transform.translate = pos;
	p.transform.rotate = { 0.0f, 0.0f, 0.0f };
	p.transform.scale = { 1.0f, 1.0f, 1.0f };
	p.velocity = { rand(rng), rand(rng), rand(rng) };
	p.color = { 1.0f, 0.5f, 0.2f, 1.0f };
	p.lifeTime = randLifetime(rng);
	p.currentTime = 0.0f;

	return p;
}

void SparkAParticle::UpdateParticle(SparkAParticleData& p, float dt) {
	float t = std::clamp(p.currentTime / p.lifeTime, 0.0f, 1.0f);
	Float3 easedMove = p.velocity * (1.0f - t) * 2.0f;
	p.transform.translate += easedMove;
}
