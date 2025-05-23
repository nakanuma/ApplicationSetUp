#pragma once

// Engine
#include <Engine/Math/MyMath.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>
#include <Engine/Model/ModelManager.h>

struct TestParticleData 
{
	Transform transform;
	Float3 velocity;
	Float4 color;
	float lifeTime;
	float currentTime;

	// ���̑��ŗL�p�����[�^�[�ǉ�

};

class TestParticle : public BaseParticleEffect<TestParticleData>
{
public:
	TestParticle(ModelManager::ModelData& model);

protected:
	/// <summary>
	/// �p�[�e�B�N���ŗL�̐�������
	/// </summary>
	TestParticleData CreateParticle(const Float3& pos) override;

	/// <summary>
	/// �p�[�e�B�N���ŗL�̍X�V����
	/// </summary>
	void UpdateParticle(TestParticleData& p, float dt) override;
};
