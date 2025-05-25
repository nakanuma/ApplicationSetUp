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
	std::uniform_real_distribution<float> randLifetime(0.2f, 0.5f);
	// ランダム回転
	std::uniform_real_distribution<float> randAngle(0.0f, std::numbers::pi_v<float> *2.0f);
	std::uniform_real_distribution<float> randSpeed(0.1f, 0.2f);

	float angle = randAngle(rng);
	float speed = randSpeed(rng);

	Float3 velocity = {
		std::cos(angle) * speed,
		std::sin(angle) * speed,
		0.0f
	};

	SparkBParticleData p;
	p.transform.translate = pos;
	p.transform.rotate = { angle + 2.0f * std::numbers::pi_v<float> / 2.0f , 0.0f, 0.0f };
	p.transform.scale = { 1.0f, 0.1f, 0.1f};
	p.velocity = velocity;
	p.color = { 1.0f, 0.5f, 0.2f, 1.0f };
	p.lifeTime = randLifetime(rng);
	p.currentTime = 0.0f;

	p.initScale = p.transform.scale;
	p.isUpdate_ = false;

	return p;
}

void SparkBParticle::UpdateParticle(SparkBParticleData& p, float dt) {
	// 減速
	float t = std::clamp(p.currentTime / p.lifeTime, 0.0f, 1.0f);
	Float3 easedMove = p.velocity * (1.0f - t) * 2.0f;
	p.transform.translate += easedMove;

	if (!p.isUpdate_) {
		SimpleEasing::Animate(p.transform.scale.x, p.initScale.x, p.initScale.x * 0.1f, Easing::EaseInQuad, p.lifeTime);

		SimpleEasing::Animate(p.color.w, 1.0f, 0.0f, Easing::EaseInExpo, p.lifeTime);
	}
	p.isUpdate_ = true;
}
