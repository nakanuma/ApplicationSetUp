#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct FlashParticleData
{
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��ŗL�p�����[�^�[�ǉ�
	Float3 initScale;
	bool isUpdate;
};

class FlashParticle : public BaseParticleEffect<FlashParticleData>
{
public:
	FlashParticle(ModelManager::ModelData& model);

protected:
	/// <summary>
	/// �p�[�e�B�N���ŗL�̐�������
	/// </summary>
	FlashParticleData CreateParticle(const Float3& pos) override;

	/// <summary>
	/// �p�[�e�B�N���ŗL�̍X�V����
	/// </summary>
	void UpdateParticle(FlashParticleData& p, float dt) override;
};
