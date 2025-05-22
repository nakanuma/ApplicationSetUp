#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct EnemyDeathParticleData 
{
	Transform transform;
	Float3 velocity;
	Float3 rotationSpeed;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��p�����[�^�[�ǉ�
};

class EnemyDeathParticle : public BaseParticleEffect<EnemyDeathParticleData>
{
public:
	EnemyDeathParticle(ModelManager::ModelData& model);

protected:
	/// <summary>
	/// �p�[�e�B�N���ŗL�̐�������
	/// </summary>
	EnemyDeathParticleData CreateParticle(const Float3& pos) override;

	/// <summary>
	/// �p�[�e�B�N���ŗL�̍X�V����
	/// </summary>
	void UpdateParticle(EnemyDeathParticleData& p, float dt) override;
};
