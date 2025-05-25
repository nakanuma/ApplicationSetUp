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

#include <Effects/HitEffect/HitEffect.h>

// ゲームプレイシーン
class GamePlayScene : public BaseScene
{
public:
	// 初期化
	void Initialize() override;

	// 終了
	void Finalize() override;

	// 毎フレーム更新
	void Update() override;

	// 描画
	void Draw() override;

private:
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<SpriteCommon> spriteCommon = nullptr;
	std::unique_ptr<SoundManager> soundManager = nullptr;
	Input* input = nullptr;
	LightManager* lightManager = nullptr;

	const float kDeltaTime = 1.0f / 60.0f;

	///
	/// ↓ ゲームシーン用
	///

	// モデルデータ
	ModelManager::ModelData model_;
	// 3Dオブジェクト
	std::unique_ptr<Object3D> object_;

	uint32_t dummy_;

	uint32_t dummy2_;


	// パーティクル用モデルデータ
	ModelManager::ModelData modelTestParticle_;

	// ヒットエフェクト
	std::unique_ptr<HitEffect> hitEffect_;
};

