#pragma once

// Engine
#include <Engine/Model/ModelManager.h>

class HitEffect
{
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 発生処理
	/// </summary>
	void Emit(const Float3& pos);

private:
	ModelManager::ModelData modelCircleParticle_; // CircleParticle用
	ModelManager::ModelData modelFlareParticle_; // FlareParticle用
	ModelManager::ModelData modelFlashParticle_; // FlashParticle用
	ModelManager::ModelData modelSparkAParicle_; // SparkAParticle用
	ModelManager::ModelData modelSparkBParticle_; // SparkBParticle用
	ModelManager::ModelData modelDotParticle_; // DotParticle用
};

