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

// Application
#include <Loader.h>

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

	///
	/// ↓ ゲームシーン用
	///

	// ローダー
	std::unique_ptr<Loader> loader_;

	// 使用するモデル
	ModelManager::ModelData modelCube_;
	ModelManager::ModelData modelSphere_;

	// オブジェクトのコンテナ
	std::vector<std::unique_ptr<Object3D>> objects_;
};
