#include "HitEffect.h"

// Engine
#include <Engine/ParticleEffect/ParticleEffectManager.h>

// Application
#include <Particles/CircleParticle/CircleParticle.h>
#include <Particles/FlareParticle/FlareParticle.h>
#include <Particles/FlashParticle/FlashParticle.h>
#include <Particles/SparkAParticle/SparkAParticle.h>
#include <Particles/SparkBParticle/SparkBParticle.h>
#include <Particles/DotParticle/DotParticle.h>

void HitEffect::Initialize()
{
	DirectXBase* dxBase = DirectXBase::GetInstance();

	// CircleParticle
	uint32_t textureCircleParticle = TextureManager::Load("resources/Images/EffectTexture/circle.png", dxBase->GetDevice());
	modelCircleParticle_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelCircleParticle_.material.textureHandle = textureCircleParticle;

	auto circleParticle = std::make_unique<CircleParticle>(modelCircleParticle_);
	ParticleEffectManager::GetInstance()->Register("Circle", std::move(circleParticle));

	// FlareParticle
	uint32_t textureFlareParticle = TextureManager::Load("resources/Images/EffectTexture/glow1.png", dxBase->GetDevice());
	modelFlareParticle_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelFlareParticle_.material.textureHandle = textureFlareParticle;

	auto flareParticle = std::make_unique<FlareParticle>(modelFlareParticle_);
	ParticleEffectManager::GetInstance()->Register("Flare", std::move(flareParticle));

	// FlashParticle
	uint32_t textureFlashParticle = TextureManager::Load("resources/Images/EffectTexture/muzzle.png", dxBase->GetDevice());
	modelFlashParticle_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelFlashParticle_.material.textureHandle = textureFlashParticle;

	auto flashParticle = std::make_unique<FlashParticle>(modelFlashParticle_);
	ParticleEffectManager::GetInstance()->Register("Flash", std::move(flashParticle));

	// SparkAParticle
	uint32_t textureSparkAPartile = TextureManager::Load("resources/Images/EffectTexture/glow2.png", dxBase->GetDevice());
	modelSparkAParicle_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelSparkAParicle_.material.textureHandle = textureSparkAPartile;

	auto sparkAParticle = std::make_unique<SparkAParticle>(modelSparkAParicle_);
	ParticleEffectManager::GetInstance()->Register("SparkA", std::move(sparkAParticle));

	// SparkBParticle
	uint32_t textureSparkBPartile = TextureManager::Load("resources/Images/EffectTexture/glow1.png", dxBase->GetDevice());
	modelSparkBParticle_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelSparkBParticle_.material.textureHandle = textureSparkBPartile;

	auto sparkBParticle = std::make_unique<SparkBParticle>(modelSparkBParticle_);
	ParticleEffectManager::GetInstance()->Register("SparkB", std::move(sparkBParticle));

	// DotParticle
	uint32_t textureDorParticle = TextureManager::Load("resources/Images/EffectTexture/glow1.png", dxBase->GetDevice());
	modelDotParticle_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelDotParticle_.material.textureHandle = textureDorParticle;

	auto dotParticle = std::make_unique<DotParticle>(modelDotParticle_);
	ParticleEffectManager::GetInstance()->Register("Dot", std::move(dotParticle));
}

void HitEffect::Emit(const Float3& pos)
{
	// 円
	ParticleEffectManager::GetInstance()->Emit("Circle", pos, 1); // 1個生成
	// ドット
	ParticleEffectManager::GetInstance()->Emit("Dot", pos, 1); // 1個生成
	// クロス
	ParticleEffectManager::GetInstance()->Emit("Flare", pos, 2); // 2個生成
	// 放射
	ParticleEffectManager::GetInstance()->Emit("Flash", pos, 10); // 10個生成
	// スパークA
	ParticleEffectManager::GetInstance()->Emit("SparkA", pos, 15); // 15個生成
	// スパークB
	ParticleEffectManager::GetInstance()->Emit("SparkB", pos, 15); // 15個生成
}
