#include "FlareParticle.h"

// C++
#include <random>
#include <numbers>

// Engine
#include <Engine/Math/Easing.h>

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
	p.transform.scale = {2.0f, 0.2f, 0.2f};
	p.velocity = {0.0f, 0.0f, 0.0f};
	p.color = {1.0f, 0.5f, 0.0f, 1.0f};
	p.lifeTime = 0.15f;
	p.currentTime = 0.0f;
	p.initScale = p.transform.scale;
	p.isUpdate = false;

	return p;
}

void FlareParticle::UpdateParticle(FlareParticleData& p, float dt) 
{
	if (!p.isUpdate) {
		// scale.xを増加（横に引き伸ばす）
		SimpleEasing::Animate(p.transform.scale.x, p.initScale.x, p.initScale.x * 3.0f, Easing::EaseOutExpo, p.lifeTime);
		// scale.yを減少（縦に潰す）
		SimpleEasing::Animate(p.transform.scale.y, p.initScale.y, p.initScale.y * 0.8f, Easing::EaseOutExpo, p.lifeTime);
	}
	p.isUpdate = true;
}
