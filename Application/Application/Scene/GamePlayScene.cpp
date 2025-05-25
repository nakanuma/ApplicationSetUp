#include "GamePlayScene.h" 
#include "ImguiWrapper.h" 
#include "DirectXBase.h"
#include "SRVManager.h"
#include "SpriteCommon.h"

#include <Engine/ParticleEffect/ParticleEffectManager.h>

#include <Particles/TestParticle/TestParticle.h>

void GamePlayScene::Initialize()
{
	DirectXBase* dxBase = DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera = std::make_unique<Camera>(Float3{0.0f, 0.0f, -35.0f}, Float3{0.0f, 0.0f, 0.0f}, 0.45f);
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

	///
	///	↓ ゲームシーン用 
	///	
	
	// Texture読み込み
	uint32_t uvCheckerGH = TextureManager::Load("resources/Images/uvChecker.png", dxBase->GetDevice());

	// モデルの読み込みとテクスチャの設定
	model_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	model_.material.textureHandle = uvCheckerGH;

	// オブジェクトの生成とモデル設定
	object_ = std::make_unique<Object3D>();
	object_->model_ = &model_;
	object_->transform_.rotate = {0.0f, 3.14f, 0.0f};

	///
	///	パーティクル生成
	/// 
	
	// パーティクル生成と登録
	uint32_t textureParticle = TextureManager::Load("resources/Images/white.png", dxBase->GetDevice());
	modelTestParticle_ = ModelManager::LoadModelFile("resources/Models", "plane.obj", dxBase->GetDevice());
	modelTestParticle_.material.textureHandle = textureParticle;

	auto testParticle = std::make_unique<TestParticle>(modelTestParticle_);
	ParticleEffectManager::GetInstance()->Register("Test", std::move(testParticle));

	///
	///	エフェクト生成
	/// 
	
	hitEffect_ = std::make_unique<HitEffect>();
	hitEffect_->Initialize();
}

void GamePlayScene::Finalize()
{
}

void GamePlayScene::Update() { 
	object_->UpdateMatrix();

	ParticleEffectManager::GetInstance()->Update(kDeltaTime);



	// ヒットエフェクト自動発生
	static float timer = 0.0f;
	const float kInterval = 1.0f;
	timer += kDeltaTime;
	if (timer > kInterval) {
		hitEffect_->Emit({0.0f, 0.0f, 0.0f});
		timer = 0.0f;
	}
}

void GamePlayScene::Draw()
{
	DirectXBase* dxBase = DirectXBase::GetInstance();
	SRVManager* srvManager = SRVManager::GetInstance();

	// 描画前処理
	dxBase->PreDraw();
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager->descriptorHeap.heap_.Get() };
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
	/*object_->Draw();*/

	ParticleEffectManager::GetInstance()->Draw();

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
	
#endif // _DEBUG

	ImGui::Begin("window");

	ImGui::Text("fps : %.2f", ImGui::GetIO().Framerate);

	ImGui::DragFloat3("t", &camera->transform.translate.x, 0.1f);

	ImGui::DragFloat3("translation", &object_->transform_.translate.x, 0.01f);
	ImGui::DragFloat3("rotation", &object_->transform_.rotate.x, 0.01f);

	const Float3 emitPos = {0.0f, 0.0f, 0.0f};

	if (ImGui::Button("Emit Test")) {
		ParticleEffectManager::GetInstance()->Emit("Dot", emitPos, 1); // 1個生成
	}


	if (ImGui::Button("Emit All")) {
		hitEffect_->Emit(emitPos);
	}

	ImGui::End();

	// ImGuiの内部コマンドを生成する
	ImguiWrapper::Render(dxBase->GetCommandList());
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}
