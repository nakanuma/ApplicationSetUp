#pragma once
#include "BaseScene.h"
#include "Camera.h"
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
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<SpriteCommon> spriteCommon = nullptr;
	std::unique_ptr<SoundManager> soundManager = nullptr;
	Input* input = nullptr;
	LightManager* lightManager = nullptr;
	std::unique_ptr<ParticleManager> particleManager_ = nullptr;

	///
	/// ↓ ゲームシーン用
	///

	// モデルデータ
	ModelManager::ModelData model_;
	// 3Dオブジェクト
	std::unique_ptr<Object3D> object_;

	// particleEmitter
	std::unique_ptr<ParticleEmitter> particleEmitter_;
	// パーティクル用モデル
	ModelManager::ModelData modelPlane_;
	// パーティクル用テクスチャ
	uint32_t textureParticle_;
};
