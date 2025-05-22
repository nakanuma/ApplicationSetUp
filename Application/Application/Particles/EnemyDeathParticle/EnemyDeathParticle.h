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
	float lifeTime;
	float currentTime;

	// その他パラメーター追加
};

class EnemyDeathParticle : public BaseParticleEffect<EnemyDeathParticleData>
{
public:
	EnemyDeathParticle(ModelManager::ModelData& model);

protected:
	/// <summary>
	/// パーティクル固有の生成処理
	/// </summary>
	EnemyDeathParticleData CreateParticle(const Float3& pos) override;

	/// <summary>
	/// パーティクル固有の更新処理
	/// </summary>
	void UpdateParticle(EnemyDeathParticleData& p, float dt) override;
};
