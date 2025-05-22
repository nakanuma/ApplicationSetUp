#pragma once
#include "InstancedObject.h"
#include "ModelManager.h"
#include "Transform.h"

#include <vector>

class EnemyDeadParticle {
public:
	struct Particle {
		Transform transform;
		Float3 velocity;
		Float3 rotationSpeed;
		Float4 color;
		float lifetime;
		float currentTime;

		// 揺れ
		float swayPhaseX;
		float swayPhaseZ;
		float swayAmplitude;
		float swaySpeed;
	};

public:
	void Initialize(ModelManager::ModelData model);
	void Update();
	void Draw();
	void Emit(Float3 emitPos);

private:
	const uint32_t kMaxNum = 100; // パーティクルの最大数
	std::vector<Particle> particles_;
	InstancedObject object_;
	ModelManager::ModelData model_;

	Matrix billboardMatrix_;

	float emissionRate_ = 30.0f;         // 1秒間に生成されるパーティクル数
	float timeSinceLastEmission_ = 0.0f; // 前回の生成からの経過時間

	void UpdateParticles();
	void UpdateSingleParticle(Particle& it);

	const float kDeltaTime = 1.0f / 60.0f;
};