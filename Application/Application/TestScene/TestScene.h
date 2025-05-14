#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "DebugCamera.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ModelManager.h"
#include "Object3D.h"
#include "SoundManager.h"
#include "Input.h"
#include "LightManager.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"

class TestScene : public BaseScene {
public:
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
#ifdef _DEBUG                       // デバッグカメラ用
	bool useDebugCamera = false;    // デバッグカメラが有効か
	Transform savedCameraTransform; // 通常カメラのTransformを保持

	void DebugCameraUpdate(Input* input);
#endif
private:
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<DebugCamera> debugCamera = nullptr;
	std::unique_ptr<SpriteCommon> spriteCommon = nullptr;
	std::unique_ptr<SoundManager> soundManager = nullptr;
	Input* input = nullptr;
	LightManager* lightManager = nullptr;
	std::unique_ptr<ParticleManager> particleManager_ = nullptr;

	///
	/// ↓ ゲームシーン用
	///

	/* SkyBox */

	// モデルデータ（Skybox）
	ModelManager::ModelData modelSkybox_;
	// 3Dオブジェクト（Skybox）
	std::unique_ptr<Object3D> objectSkybox_;

	/* Animation */

	// モデルデータ（Human）
	ModelManager::ModelData modelHuman_;
	// 3Dオブジェクト（Human）
	std::unique_ptr<Object3D> objectHuman_;
	// アニメーション
	ModelManager::Animation animation_;
	// スケルトン
	ModelManager::Skeleton skeleton_;
	// アニメーション時間
	float animationTime_ = 0.0f;

	// デバッグ用球体モデル
	ModelManager::ModelData modelSphere_;
	// デバッグ用球体オブジェクト
	std::vector<std::unique_ptr<Object3D>> jointSpheres_; 
};
