#include "TestScene.h"
#include "ImguiWrapper.h" 
#include "DirectXBase.h"
#include "RTVManager.h"
#include "SRVManager.h"
#include "SpriteCommon.h"

#include <numbers>

void TestScene::Initialize() {
	DirectXBase* dxBase = DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera = std::make_unique<Camera>(Float3{0.0f, 5.0f, -15.0f}, Float3{0.3f, 0.0f, 0.0f}, 0.45f);
	Camera::Set(camera.get()); // 現在のカメラをセット

	// デバッグカメラの生成と初期化
	debugCamera = std::make_unique<DebugCamera>();
	debugCamera->Initialize();

	// SpriteCommonの生成と初期化
	spriteCommon = std::make_unique<SpriteCommon>();
	spriteCommon->Initialize(DirectXBase::GetInstance());

	// TextureManagerの初期化
	TextureManager::Initialize(dxBase->GetDevice(), SRVManager::GetInstance());

	// SoundManagerの初期化
	soundManager = std::make_unique<SoundManager>();
	soundManager->Initialize();

	// Inputの初期化
	input = Input::GetInstance();

	// LightManagerの初期化
	lightManager = LightManager::GetInstance();
	lightManager->Initialize();

	// ParticleManagerの初期化
	particleManager_ = std::make_unique<ParticleManager>();
	particleManager_->Initialize(dxBase, SRVManager::GetInstance());

	///
	///	↓ ゲームシーン用
	///

	/* アニメーションオブジェクト */

	// Texture読み込み
	uint32_t texture = TextureManager::Load("resources/Images/AnimatedCube_BaseColor.png", dxBase->GetDevice());

	// モデルの読み込みとテクスチャの設定
	model_ = ModelManager::LoadModelFile("resources/Models", "AnimatedCube.gltf", dxBase->GetDevice());
	model_.material.textureHandle = texture;

	// アニメーション読み込み
	animation_ = ModelManager::LoadAnimation("resources/Models", "AnimatedCube.gltf");

	// オブジェクトの生成とモデル設定
	object_ = std::make_unique<Object3D>();
	object_->model_ = &model_;

	/* Skybox */

	// ddsファイルの読み込み
	uint32_t dds = TextureManager::Load("resources/Images/rostock_laage_airport_4k.dds", dxBase->GetDevice());

	// モデルの読み込みとテクスチャの設定
	modelSkybox_ = ModelManager::CreateSkyBoxModel(dxBase->GetDevice());
	modelSkybox_.material.textureHandle = dds;

	// オブジェクトの生成とモデル設定
	objectSkybox_ = std::make_unique<Object3D>();
	objectSkybox_->model_ = &modelSkybox_;
	objectSkybox_->materialCB_.data_->enableLighting = false;
	objectSkybox_->transform_.scale = {1000.0f, 1000.0f, 1000.0f};
}

void TestScene::Finalize() {}

void TestScene::Update() { 
	#ifdef _DEBUG // デバッグカメラ
	DebugCameraUpdate(input);
	#endif

#pragma region アニメーション適用
	object_->UpdateMatrix(); 

	// オブジェクトとカメラの行列を取得
	Matrix worldMatrix = object_->transform_.MakeAffineMatrix();
	Matrix viewProjectionMatrix = Camera::GetCurrent()->GetViewProjectionMatrix();

	animationTime_ += 1.0f / 60.0f; // 時間を進める
	animationTime_ = std::fmod(animationTime_, animation_.duration);
	ModelManager::NodeAnimation& rootNodeAnimation = animation_.nodeAnimations[model_.rootNode.name]; // rootNodeのAnimationを取得
	Float3 translate = ModelManager::CalculateValue(rootNodeAnimation.translate, animationTime_);     // 指定時刻の値を取得
	Quaternion rotate = ModelManager::CalculateValue(rootNodeAnimation.rotate, animationTime_);
	Float3 scale = ModelManager::CalculateValue(rootNodeAnimation.scale, animationTime_);
	Transform transform = {scale, Float3{rotate.x, rotate.y, rotate.z}, translate}; // 一旦Transformにする
	Matrix localMatrix = transform.MakeAffineMatrix();

	// RootのMatrixを適用
	object_->wvpCB_.data_->WVP = localMatrix * worldMatrix * viewProjectionMatrix;
	object_->wvpCB_.data_->World = localMatrix * worldMatrix;
#pragma endregion

	objectSkybox_->UpdateMatrix();

	// パーティクルの更新
	particleManager_->Update();
}

void TestScene::Draw() {
	DirectXBase* dxBase = DirectXBase::GetInstance();
	SRVManager* srvManager = SRVManager::GetInstance();

	// 描画前処理
	dxBase->PreDraw();
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = {srvManager->descriptorHeap.heap_.Get()};
	dxBase->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	// ImGuiのフレーム開始処理
	ImguiWrapper::NewFrame();
	// カメラの定数バッファを設定
	Camera::TransferConstantBuffer();
	// ライトの定数バッファを設定
	lightManager->TransferContantBuffer();

	///
	///	↓ ここから3Dオブジェクトの描画コマンド
	///

	// オブジェクトの描画
	object_->Draw();

	// Skyboxの描画（Skybox用PSOに変更）
	dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateSkybox());
	objectSkybox_->Draw();
	dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineState());

	// パーティクル描画
	particleManager_->Draw();

	///
	///	↑ ここまで3Dオブジェクトの描画コマンド
	///

	// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
	spriteCommon->PreDraw();

	///
	/// ↓ ここからスプライトの描画コマンド
	///

	///
	/// ↑ ここまでスプライトの描画コマンド
	///

#ifdef _DEBUG
	// カメラ //
	ImGui::Begin("Camera");

	ImGui::DragFloat3("Translate", &camera->transform.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &camera->transform.rotate.x, 0.01f);

	ImGui::Checkbox("useDebugCamera", &useDebugCamera);

	ImGui::End();

	// オブジェクト //
	ImGui::Begin("object");

	ImGui::DragFloat3("translation", &object_->transform_.translate.x, 0.01f);
	ImGui::DragFloat3("rotation", &object_->transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("scale", &object_->transform_.scale.x, 0.01f);

	ImGui::End();
#endif // _DEBUG

	// ImGuiの内部コマンドを生成する
	ImguiWrapper::Render(dxBase->GetCommandList());
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}

void TestScene::DebugCameraUpdate(Input* input) {
	// 前回のカメラモード状態を保持
	static bool prevUseDebugCamera = false;

	// デバッグカメラが有効になった瞬間に通常カメラのTransformを保存
	if (useDebugCamera && !prevUseDebugCamera) {
		savedCameraTransform = camera->transform;
	}

	// デバッグカメラが有効の場合
	if (useDebugCamera) {
		// デバッグカメラの更新
		debugCamera->Update(input);
		// 通常カメラにデバッグカメラのTransformを適用
		camera->transform = debugCamera->transform_;
	} else if (!useDebugCamera && prevUseDebugCamera) {
		// 通常カメラのTransformを再現
		camera->transform = savedCameraTransform;
	}

	// 現在のカメラモードを保存して次のフレームで使う
	prevUseDebugCamera = useDebugCamera;
}
