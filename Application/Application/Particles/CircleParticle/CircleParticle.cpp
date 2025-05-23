#include "CircleParticle.h"

// C++
#include <random>

CircleParticle::CircleParticle(ModelManager::ModelData& model) 
{
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();

	// �r���{�[�h�K�p�ݒ�
	isBillboard_ = {true, true, true};
	// �u�����h���[�h�ݒ�
	blendMode_ = BlendMode::Add;
}

CircleParticleData CircleParticle::CreateParticle(const Float3& pos) 
{ 
	CircleParticleData p;
	p.transform.translate = pos;
	p.transform.rotate = {0.0f, 0.0f, 0.0f};
	p.transform.scale = {1.0f, 1.0f, 1.0f};
	p.velocity = {0.0f, 0.0f, 0.0f};
	p.color = {1.0f, 1.0f, 1.0f, 1.0f};
	p.lifeTime = 1.0f;
	p.currentTime = 0.0f;

	return p;
}

void CircleParticle::UpdateParticle(CircleParticleData& p, float dt) 
{ 
	// �g��
	p.transform.scale *= 1.1f;
	// �c��Lifetime�ɉ����ē�����
	p.color.w = 1.0f - p.currentTime / p.lifeTime;
}
