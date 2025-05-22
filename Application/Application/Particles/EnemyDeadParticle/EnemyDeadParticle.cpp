#include "EnemyDeadParticle.h"
#include "Camera.h"

#include <random>

void EnemyDeadParticle::Initialize(ModelManager::ModelData model) {
	object_.gTransformationMatrices.numMaxInstance_ = kMaxNum;
	object_.gTransformationMatrices.Create();

	model_ = model;
	object_.model_ = &model_;

	particles_.reserve(kMaxNum);
}

void EnemyDeadParticle::Update() {
	// パーティクルの生成
	timeSinceLastEmission_ += kDeltaTime;
	while (timeSinceLastEmission_ >= 1.0f / emissionRate_) {
		/*EmitParticle();*/
		timeSinceLastEmission_ -= 1.0f / emissionRate_;
	}

	// パーティクルの更新
	UpdateParticles();

	// インスタンシングデータの更新
	for (size_t i = 0; i < particles_.size(); i++) {
		Matrix world = {
		    Matrix::Scaling({-particles_[i].transform.scale.x, particles_[i].transform.scale.y, particles_[i].transform.scale.z}) * // 反対側を向かないようにscale.xを反転
		    Matrix::RotationRollPitchYaw(particles_[i].transform.rotate.x, particles_[i].transform.rotate.y, particles_[i].transform.rotate.z) *
		    Matrix::Translation(particles_[i].transform.translate)};

		Matrix view = Camera::GetCurrent()->MakeViewMatrix();
		Matrix projection = Camera::GetCurrent()->MakePerspectiveFovMatrix();

		object_.gTransformationMatrices.data_[i].WVP = world * view * projection;
		object_.gTransformationMatrices.data_[i].World = world;
		object_.gTransformationMatrices.data_[i].WorldInverseTranspose = Matrix::Inverse(world);
		object_.gTransformationMatrices.data_[i].color = particles_[i].color;
	}

	// 残りのデータを無効化（不要なデータをクリア）
	for (size_t i = particles_.size(); i < kMaxNum; i++) {
		object_.gTransformationMatrices.data_[i].WVP = Matrix(); // 初期化
		object_.gTransformationMatrices.data_[i].World = Matrix();
		object_.gTransformationMatrices.data_[i].WorldInverseTranspose = Matrix();
		object_.gTransformationMatrices.data_[i].color = {0.0f, 0.0f, 0.0f, 0.0f}; // 透明化
	}

	object_.UpdateMatrix();
}

void EnemyDeadParticle::Draw() {
	DirectXBase* dxBase = DirectXBase::GetInstance();
	dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateInstancedObject());

	object_.InstancedDraw();

	dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineState());
}

void EnemyDeadParticle::Emit(Float3 emitPos) {
	// パーティクル数が上限に達している場合は生成しない
	if (particles_.size() >= kMaxNum) {
		return;
	}

	// 乱数生成器の初期化
	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_real_distribution<float> distPos(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distVel(0.8f, 1.2f);
	std::uniform_real_distribution<float> distPhase(0.0f, 2.0f * 3.14f);
	std::uniform_real_distribution<float> distAmplitude(0.0f, 0.01f);
	std::uniform_real_distribution<float> distSpeed(8.0f, 16.0f);
	std::uniform_real_distribution<float> distRotSpd(2.0f, 4.0f);

	for (int32_t i = 0; i < 5; i++) {
		// パーティクルのパラメーターを設定
		Particle newParticle;
		newParticle.transform.translate = {emitPos.x + distPos(rng), emitPos.y + distPos(rng), emitPos.z + distPos(rng)};
		newParticle.transform.rotate = {0.0f, 0.0f, 0.0f};
		newParticle.transform.scale = {1.0f, 1.0f, 1.0f};
		newParticle.velocity = {0.0f, distVel(rng), 0.0f};
		newParticle.rotationSpeed = {distRotSpd(rng), distRotSpd(rng), distRotSpd(rng)};
		newParticle.lifetime = 2.0f;
		newParticle.currentTime = 0.0f;
		newParticle.color = {1.0f, 1.0f, 1.0f, 1.0f};

		newParticle.swayPhaseX = distPhase(rng);
		newParticle.swayPhaseZ = distPhase(rng);
		newParticle.swayAmplitude = distAmplitude(rng);
		newParticle.swaySpeed = distSpeed(rng);

		particles_.push_back(newParticle);
	}
}

void EnemyDeadParticle::UpdateParticles() {
	for (auto it = particles_.begin(); it != particles_.end();) {
		it->currentTime += kDeltaTime;
		// 寿命に達したパーティクルを削除
		if (it->currentTime >= it->lifetime) {
			it = particles_.erase(it);
			// パーティクルの更新
		} else {
			UpdateSingleParticle(*it);
			++it;
		}
	}
}

void EnemyDeadParticle::UpdateSingleParticle(Particle& it) {
	// 上昇
	float speedFactor = 2.0f + (it.currentTime / it.lifetime);
	it.transform.translate += (it.velocity * speedFactor) * kDeltaTime;

	// sin波で左右に揺らす
	it.transform.translate.x += it.swayAmplitude * sin(it.currentTime * it.swaySpeed + it.swayPhaseX);
	it.transform.translate.z += it.swayAmplitude * cos(it.currentTime * it.swaySpeed + it.swayPhaseZ);

	// 回転の更新
	it.transform.rotate.x += it.rotationSpeed.x * kDeltaTime;
	it.transform.rotate.y += it.rotationSpeed.y * kDeltaTime;
	it.transform.rotate.z += it.rotationSpeed.z * kDeltaTime;

	// スケール変更
	float lifeRatio = 1.0f - (it.currentTime / it.lifetime);
	it.transform.scale = {0.5f * lifeRatio, 0.5f * lifeRatio, 0.5f * lifeRatio};

	// 色変更
	float greenFactor = lifeRatio * lifeRatio;
	it.color = {1.0f, greenFactor, 0.0f, 1.0f};
}