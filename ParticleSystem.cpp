#include "ParticleSystem.h"

ParticleSystem::~ParticleSystem() {
	delete particleModel_;
	//particles_.clear();
	for (Particle* particle : particles_) {
		delete particle;
	}
}

void ParticleSystem::Initialize() { particleModel_ = Model::Create(); }

void ParticleSystem::Update() {
	// デスフラグの立ったパーティクルを削除
	particles_.remove_if([](Particle* particle) {
		if (particle->GetIsDead()) {
			delete particle;
			return true;
		}
		return false;
	});

	for (Particle* particle : particles_) {
		particle->Update();
	}
}

void ParticleSystem::Draw(const ViewProjection& view) {
	for (Particle* particle : particles_) {
		particle->Draw(view);
	}
}

void ParticleSystem::AddParticle(Particle::Parameter parameter) {
	Particle* newParticle = new Particle();
	parameter.velocity_.x = (rand() % 100 - 50) / 100.0f;
	parameter.velocity_.y = (rand() % 100 - 50) / 100.0f;
	parameter.velocity_.z = (rand() % 100 - 50) / 100.0f;
	newParticle->Initialize(parameter, particleModel_);
	particles_.push_back(newParticle);
}

void ParticleSystem::Generate(Particle::Parameter parameter, uint32_t howMany) {
	for (uint32_t i = 0; i < howMany; i++) {
		AddParticle(parameter);
	}
}
