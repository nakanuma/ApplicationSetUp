#include "TestScene.h"
#include "ImguiWrapper.h" 
#include "DirectXBase.h"
#include "RTVManager.h"
#include "SRVManager.h"
#include "SpriteCommon.h"

void TestScene::Initialize() {
	DirectXBase* dxBase = DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera = std::make_unique<Camera>(Float3{0.0f, 10.0f, -25.0f}, Float3{0.3f, 0.0f, 0.0f}, 0.45f);
	Camera::Set(camera.get()); // 現在のカメラをセット

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
	uint32_t texture = TextureManager::Load("resources/Images/grass.png", dxBase->GetDevice());

	// モデルの読み込みとテクスチャの設定
	model_ = ModelManager::LoadModelFile("resources/Models", "terrain.obj", dxBase->GetDevice());
	model_.material.textureHandle = texture;

	// オブジェクトの生成とモデル設定
	object_ = std::make_unique<Object3D>();
	object_->model_ = &model_;

	/*-------------------------------*/

	// パーティクル用のテクスチャとモデル読み込み
	textureParticle_ = TextureManager::Load("resources/Images/circle2.png", dxBase->GetDevice());

	modelPlane_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelPlane_.material.textureHandle = textureParticle_;

	// particleEmitterの生成
	particleEmitter_ = std::make_unique<ParticleEmitter>(*particleManager_);
	particleEmitter_->transform.translate = {0.0f, 8.0f, 8.0f};

	// パーティクルグループを作成
	particleManager_->CreateParticleGroup("particle");
	particleManager_->SetModel("particle", &modelPlane_);
	particleManager_->SetTexture("particle", textureParticle_);
}

void TestScene::Finalize() {}

void TestScene::Update() { 
	object_->UpdateMatrix(); 

	// パーティクルの更新
	particleManager_->Update();

	particleEmitter_->Update("particle", false); // 自動発生しないように
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

	ImGui::End();

	// オブジェクト //
	ImGui::Begin("object");

	ImGui::DragFloat3("translation", &object_->transform_.translate.x, 0.01f);
	ImGui::DragFloat3("rotation", &object_->transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("scale", &object_->transform_.scale.x, 0.01f);

	ImGui::End();

	// パーティクル //
	ImGui::Begin("particleEmitter");

	ImGui::DragFloat3("Transform", &particleEmitter_->transform.translate.x, 0.01f);

	if (ImGui::Button("Emit")) {
		particleEmitter_->Emit("particle");
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
