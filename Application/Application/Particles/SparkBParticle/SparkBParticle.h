#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct SparkBParticleData
{
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��ŗL�p�����[�^�[�ǉ�
	Float3 initScale;
	bool isUpdate_;
};

class SparkBParticle : public BaseParticleEffect<SparkBParticleData>
{
public:
	SparkBParticle(ModelManager::ModelData& model);

protected:
	/// <summary>
	/// �p�[�e�B�N���ŗL�̐�������
	/// </summary>
	SparkBParticleData CreateParticle(const Float3& pos) override;

	/// <summary>
	/// �p�[�e�B�N���ŗL�̍X�V����
	/// </summary>
	void UpdateParticle(SparkBParticleData& p, float dt) override;
};
