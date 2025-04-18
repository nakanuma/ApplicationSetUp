#pragma once

// Engine
#include <ParticleEmitter.h>
#include <ParticleManager.h>

class RingEffectEmitter {
public:
	RingEffectEmitter(ParticleManager& manager);

	void Update(std::string name, bool isEmit);
	void Emit(std::string name);

	Transform transform; //!< �G�~�b�^��Transform
private:
	ParticleManager* particleManager = nullptr;

	// �p�[�e�B�N���O���[�v�R���e�i
	std::unordered_map<std::string, std::unique_ptr<ParticleManager::ParticleGroup>> particleGroups;

	// ��t���`
	const float kDeltaTime = 1.0f / 60.0f;

	uint32_t count;      //!< ������
	float frequency;     //!< �����p�x
	float frequencyTime; // !< �p�x�p����
};
