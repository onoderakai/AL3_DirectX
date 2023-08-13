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
	for (Particle* particle : particles_) {
		particle->Update();
	}
}

void ParticleSystem::Draw(const ViewProjection& view) {
	for (Particle* particle : particles_) {
		particle->Draw(view);
	}
}

void ParticleSystem::AddParticle() {
	Particle* newParticle = new Particle();
	newParticle->Initialize(particleModel_);
}

void ParticleSystem::Generate(uint32_t howMany) {
	for (uint32_t i = 0; i < howMany; i++) {
		AddParticle();
	}
}
