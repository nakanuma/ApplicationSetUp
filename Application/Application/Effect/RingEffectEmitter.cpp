#include "RingEffectEmitter.h"

RingEffectEmitter::RingEffectEmitter(ParticleManager& manager) {
	this->particleManager = &manager;

	// �p�[�e�B�N���O���[�v���R�s�[
	for (auto& pair : manager.particleGroups) {
		this->particleGroups[pair.first] = std::move(pair.second);
	}

	// emitter�̏����l��ݒ�
	count = 3;
	frequency = 0.5f;     // 0.5�b���Ƃɔ���
	frequencyTime = 0.0f; // �����p�x�p�̎����A0�ŏ�����
	transform.translate = {0.0f, 0.0f, 0.0f};
	transform.rotate = {0.0f, 0.0f, 0.0f};
	transform.scale = {1.0f, 1.0f, 1.0f};
}

void RingEffectEmitter::Update(std::string name, bool isEmit) {
	frequencyTime += kDeltaTime;      // ������i�߂�
	if (frequency <= frequencyTime) { // �����������傫���Ȃ甭��
		if (isEmit) {
			Emit(name); // ����������
		}
		frequencyTime -= frequency; // �]�v�ɉ߂������Ԃ��������ĕp�x�v�Z����
	}
}

void RingEffectEmitter::Emit(std::string name) { 
	particleManager->Emit(name, transform.translate, count); 
}
