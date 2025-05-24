#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct SparkAParticleData
{
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��ŗL�p�����[�^�[�ǉ�

};

class SparkAParticle : public BaseParticleEffect<SparkAParticleData>
{
public:
	SparkAParticle(ModelManager::ModelData& model);

protected:
	/// <summary>
	/// �p�[�e�B�N���ŗL�̐�������
	/// </summary>
	SparkAParticleData CreateParticle(const Float3& pos) override;

	/// <summary>
	/// �p�[�e�B�N���ŗL�̍X�V����
	/// </summary>
	void UpdateParticle(SparkAParticleData& p, float dt) override;
};
