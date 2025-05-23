#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct TestParticleData 
{
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// その他固有パラメーター追加

};

class TestParticle : public BaseParticleEffect<TestParticleData>
{
public:
	TestParticle(ModelManager::ModelData& model);

protected:
	/// <summary>
	/// パーティクル固有の生成処理
	/// </summary>
	TestParticleData CreateParticle(const Float3& pos) override;

	/// <summary>
	/// パーティクル固有の更新処理
	/// </summary>
	void UpdateParticle(TestParticleData& p, float dt) override;
};
