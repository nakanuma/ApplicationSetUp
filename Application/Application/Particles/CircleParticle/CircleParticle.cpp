#include "CircleParticle.h"

// C++
#include <random>

// Engine
#include <Engine/Math/Easing.h>

CircleParticle::CircleParticle(ModelManager::ModelData& model) 
{
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();

	// ビルボード適用設定
	isBillboard_ = {true, true, true};
	// ブレンドモード設定
	blendMode_ = BlendMode::Add;
}

CircleParticleData CircleParticle::CreateParticle(const Float3& pos) 
{ 
	CircleParticleData p;
	p.transform.translate = pos;
	p.transform.rotate = {0.0f, 0.0f, 0.0f};
	p.transform.scale = {0.5f, 0.5f, 0.5f };
	p.velocity = {0.0f, 0.0f, 0.0f};
	p.color = {1.0f, 0.5f, 0.0f, 1.0f};
	p.lifeTime = 0.1f;
	p.currentTime = 0.0f;
	p.initScale = p.transform.scale;
	p.isUpdate = false;

	return p;
}

void CircleParticle::UpdateParticle(CircleParticleData& p, float dt) 
{ 
	if (!p.isUpdate) {
		SimpleEasing::Animate(p.transform.scale, p.initScale, p.initScale * 3.0f, Easing::EaseOutExpo, p.lifeTime);
	}
	p.isUpdate = true;
}
