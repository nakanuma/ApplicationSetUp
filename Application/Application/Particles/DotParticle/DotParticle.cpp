#include "DotParticle.h"

// C++
#include <random>

// Engine
#include <Engine/Math/Easing.h>

DotParticle::DotParticle(ModelManager::ModelData& model)
{
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();

	// ビルボード適用設定
	isBillboard_ = { true, true, true };
	// ブレンドモード設定
	blendMode_ = BlendMode::Alpha;
}

DotParticleData DotParticle::CreateParticle(const Float3& pos)
{
	DotParticleData p;
	p.transform.translate = pos;
	p.transform.rotate = { 0.0f, 0.0f, 0.0f };
	p.transform.scale = { 0.5f, 0.5f, 0.5f };
	p.velocity = { 0.0f, 0.0f, 0.0f };
	p.color = { 0.4f, 0.25f, 0.1f, 0.8f };
	p.lifeTime = 0.25f;
	p.currentTime = 0.0f;
	p.initScale = p.transform.scale;
	p.isUpdate = false;

	return p;
}

void DotParticle::UpdateParticle(DotParticleData& p, float dt)
{
	if (!p.isUpdate) {
		SimpleEasing::Animate(p.transform.scale, p.initScale, p.initScale * 5.0f, Easing::EaseOutExpo, p.lifeTime);

		SimpleEasing::Animate(p.color.w, 1.0f, 0.0f, Easing::EaseOutQuad, p.lifeTime);
	}
	p.isUpdate = true;
}
