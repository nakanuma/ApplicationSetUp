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
	camera = std::make_unique<Camera>(Float3{0.0f, 0.0f, -10.0f}, Float3{0.0f, 0.0f, 0.0f}, 0.45f);
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

	// Texture読み込み
	uint32_t texture = TextureManager::Load("resources/Images/gradationLine.png", dxBase->GetDevice());

	// モデルの読み込みとテクスチャの設定
	/*model_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());*/
	model_ = ModelManager::CreateRingModel(dxBase->GetDevice());
	model_.material.textureHandle = texture;

	// オブジェクトの生成とモデル設定
	object_ = std::make_unique<Object3D>();
	object_->model_ = &model_;
	object_->transform_.rotate = {-std::numbers::pi_v<float> / 2.0f, 0.0f, 0.0f};
	object_->materialCB_.data_->enableLighting = false;

	/*-------------------------------*/

	// パーティクル用のテクスチャとモデル読み込み
	textureParticle_ = TextureManager::Load("resources/Images/circle2.png", dxBase->GetDevice());

	modelPlane_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelPlane_.material.textureHandle = textureParticle_;

	// エフェクト用のテクスチャとモデル読み込み
	textureRing_ = TextureManager::Load("resources/Images/gradationLine.png", dxBase->GetDevice());

	modelRing_ = ModelManager::CreateRingModel(dxBase->GetDevice());
	modelRing_.material.textureHandle = textureRing_;

	// particleEmitterの生成
	particleEmitter_ = std::make_unique<ParticleEmitter>(*particleManager_);
	particleEmitter_->transform.translate = {0.0f, 0.0f, 0.0f};

	// effectEmitterの生成
	ringEffectEmitter_ = std::make_unique<ParticleEmitter>(*particleManager_);
	ringEffectEmitter_->transform.translate = {0.0f, 0.0f, 0.0f};

	// パーティクルグループを作成
	particleManager_->CreateParticleGroup("particle");
	particleManager_->SetModel("particle", &modelPlane_);
	particleManager_->SetTexture("particle", textureParticle_);

	// エフェクトのグループを作成
	particleManager_->CreateParticleGroup("effect");
	particleManager_->SetModel("effect", &modelRing_);
	particleManager_->SetTexture("effect", textureRing_);
}

void TestScene::Finalize() {}

void TestScene::Update() { 
	#ifdef _DEBUG // デバッグカメラ
	DebugCameraUpdate(input);
	#endif

	object_->UpdateMatrix(); 

	// パーティクルの更新
	particleManager_->Update();

	particleEmitter_->Update("particle", false); // 自動発生しないように
	ringEffectEmitter_->Update("effect", false);
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

	static float scaleU = 1.0f;
	ImGui::DragFloat("scaleU", &scaleU, 0.01f);
	object_->ScaleUV(scaleU);

	ImGui::End();

	// パーティクル //
	ImGui::Begin("particleEmitter");

	//ImGui::DragFloat3("Transform", &particleEmitter_->transform.translate.x, 0.01f);

	if (ImGui::Button("Emit")) {
		particleEmitter_->Emit("particle");
		/*ringEffectEmitter_->Emit("effect");*/
	}

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
