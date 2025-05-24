#include "FlashParticle.h"

// C++
#include <random>
#include <numbers>

// Engine
#include <Engine/Math/Easing.h>

FlashParticle::FlashParticle(ModelManager::ModelData& model) 
{
	object_.model_ = &model;
	object_.gTransformationMatrices.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices.Create();

	// �r���{�[�h�K�p�ݒ�
	isBillboard_ = { false, false, false };
	// �u�����h���[�h�ݒ�
	blendMode_ = BlendMode::Add;
}

FlashParticleData FlashParticle::CreateParticle(const Float3& pos) 
{
	std::mt19937 rng(std::random_device{}());
	// �����_����]
	std::uniform_real_distribution<float> randAngle(0.0f, std::numbers::pi_v<float> * 2.0f);
	float angle = randAngle(rng);
	// �X�P�[��
	std::uniform_real_distribution<float> randScaleX(0.5f, 1.0f);
	std::uniform_real_distribution<float> randScaleY(0.5f, 1.0f);
	float scaleX = randScaleX(rng);
	float scaleY = randScaleY(rng);
	// �e�N�X�`���̍��[�����_�ɂȂ�悤�␳
	Float3 offset = {
		std::cosf(angle) * (scaleX * 0.5f),
		std::sinf(angle) * (scaleX * 0.5f),
		0.0f
	};
	// Lifetime
	std::uniform_real<float> randLifetime(0.1f, 0.15f);

	FlashParticleData p;
	p.transform.translate = pos + offset;
	p.transform.rotate = { angle, 0.0f, 0.0f};
	p.transform.scale = { scaleX, scaleY, 1.0f};
	p.velocity = { 0.0f, 1.0f, 0.0f };
	p.color = { 1.0f, 0.5f, 0.2f, 1.0f };
	p.lifeTime = randLifetime(rng);
	p.currentTime = 0.0f;
	p.initScale = p.transform.scale;
	p.isUpdate = false;

	return p;
}

void FlashParticle::UpdateParticle(FlashParticleData& p, float dt) 
{
	if (!p.isUpdate) {
		// scale.x�𑝉��i���Ɉ����L�΂��j
		SimpleEasing::Animate(p.transform.scale.x, p.initScale.x, p.initScale.x * 1.4f, Easing::EaseOutExpo, p.lifeTime);
		// scale.y�������i�c�ɒׂ��j
		SimpleEasing::Animate(p.transform.scale.y, p.initScale.y, p.initScale.y * 0.4f, Easing::EaseOutExpo, p.lifeTime);
	}

	// �X�P�[���␳�ɂ��ʒu�̃Y����␳�i���[�𒆐S�ɕۂj
	float scaleDiff = p.transform.scale.x - p.initScale.x;
	float offsetX = std::cosf(p.transform.rotate.x) * (scaleDiff * 0.5f);
	float offsetY = std::sinf(p.transform.rotate.x) * (scaleDiff * 0.5f);
	p.transform.translate = p.transform.translate + Float3(offsetX, offsetY, 0.0f);

	p.isUpdate = true;
}
