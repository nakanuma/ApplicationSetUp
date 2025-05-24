#include "SparkBParticle.h"

// C++
#include <random>
#include <numbers>

// Engine
#include <Engine/Math/Easing.h>

SparkBParticle::SparkBParticle(ModelManager::ModelData& model) {
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();

	// ビルボード適用設定
	isBillboard_ = { false, false, false };
	// ブレンドモード設定
	blendMode_ = BlendMode::Add;
}

SparkBParticleData SparkBParticle::CreateParticle(const Float3& pos) {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> rand(-0.2f, 0.2f);

	std::uniform_real_distribution<float> randLifetime(0.2f, 0.4f);

	std::uniform_real_distribution<float> randScaleX(0.2f, 0.6f);

	SparkBParticleData p;
	p.transform.translate = pos;
	p.transform.rotate = { 0.0f, 0.0f, 0.0f};
	p.transform.scale = { 0.2f, 0.2f, 0.2f};
	p.velocity = { rand(rng), rand(rng), rand(rng) };
	p.color = { 1.0f, 0.5f, 0.2f, 1.0f };
	p.lifeTime = randLifetime(rng);
	p.currentTime = 0.0f;

	return p;
}

void SparkBParticle::UpdateParticle(SparkBParticleData& p, float dt) {
	// 減速
	float t = std::clamp(p.currentTime / p.lifeTime, 0.0f, 1.0f);
	Float3 easedMove = p.velocity * (1.0f - t) * 2.0f;
	p.transform.translate += easedMove;
}
