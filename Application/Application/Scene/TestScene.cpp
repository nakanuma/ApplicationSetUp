#include "TestScene.h"
#include "ImguiWrapper.h" 
#include "DirectXBase.h"
#include "RTVManager.h"
#include "SRVManager.h"
#include "SpriteCommon.h"

void TestScene::Initialize() {
	DirectXBase* dxBase = DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera = std::make_unique<Camera>(Float3{0.0f, 15.0f, -40.0f}, Float3{0.3f, 0.0f, 0.0f}, 0.45f);
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

	// ローダー生成
	loader_ = std::make_unique<Loader>();
	loader_->Initialize();
	Loader::LevelData* levelData = loader_->GetLevelData();

	// テクスチャ読み込み
	uint32_t textureWhite = TextureManager::Load("resources/Images/white.png", dxBase->GetDevice());

	// モデル読み込み
	modelCube_ = ModelManager::LoadModelFile("resources/Models", "cube.obj", dxBase->GetDevice());
	modelCube_.material.textureHandle = textureWhite;

	modelSphere_ = ModelManager::LoadModelFile("resources/Models", "sphere.obj", dxBase->GetDevice());
	modelSphere_.material.textureHandle = textureWhite;

	// ローダーから読み込んだデータのオブジェクトを生成
	for (const auto& objData : levelData->objects) {
		auto object = std::make_unique<Object3D>();

		// fileNameに応じてセットするモデルを変更
		if (objData.fileName == "Cube") {
			object->model_ = &modelCube_;
		} else if (objData.fileName == "Sphere") {
			object->model_ = &modelSphere_;
		}

		object->transform_.translate = objData.translation;
		object->transform_.rotate = objData.rotation;
		object->transform_.scale = objData.scaling;

		objects_.push_back(std::move(object));
	}
}

void TestScene::Finalize() {}

void TestScene::Update() { 
	// オブジェクト更新
	for (auto& obj : objects_) {
		obj->UpdateMatrix();
	}
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

	// オブジェクト描画
	for (auto& obj : objects_) {
		obj->Draw();
	}

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

	ImGui::DragFloat3("t", &objects_[0]->transform_.translate.x, 0.01f);
	ImGui::DragFloat3("r", &objects_[0]->transform_.rotate.x, 0.01f);
	ImGui::DragFloat3("s", &objects_[0]->transform_.scale.x, 0.01f);

	ImGui::End();

	// ImGuiの内部コマンドを生成する
	ImguiWrapper::Render(dxBase->GetCommandList());
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}
