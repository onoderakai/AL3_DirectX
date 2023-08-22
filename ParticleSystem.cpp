#include "ParticleSystem.h"
#include "MathUtility.h"

ParticleSystem::ParticleSystem() { particleModel_ = Model::Create(); }

ParticleSystem::~ParticleSystem() {
	delete particleModel_;
	for (Particle* particle : particles_) {
		delete particle;
	}
}

void ParticleSystem::Initialize() {
	particles_.remove_if([](Particle* particle) {
		delete particle;
		return true;
	});
}

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
	parameter.world_.rotation_.x = float(rand() % 1000);
	parameter.world_.rotation_.y = float(rand() % 1000);
	parameter.world_.rotation_.z = float(rand() % 1000);

	Vector3 velocity = {};
	velocity.x = float(rand() % 1000 - 499);
	velocity.y = float(rand() % 1000 - 499);
	velocity.z = float(rand() % 1000 - 499);
	velocity = Normalize(velocity);
	newParticle->Initialize(parameter, velocity, particleModel_);
	particles_.push_back(newParticle);
}

void ParticleSystem::AddParticle(Particle::Parameter parameter, uint32_t textureHandle) {
	Particle* newParticle = new Particle();
	parameter.world_.rotation_.x = float(rand() % 1000);
	parameter.world_.rotation_.y = float(rand() % 1000);
	parameter.world_.rotation_.z = float(rand() % 1000);

	Vector3 velocity = {};
	velocity.x = float(rand() % 1000 - 499);
	velocity.y = float(rand() % 1000 - 499);
	velocity.z = float(rand() % 1000 - 499);
	velocity = Normalize(velocity);
	newParticle->Initialize(parameter, velocity, particleModel_, textureHandle);
	particles_.push_back(newParticle);
}

void ParticleSystem::Generate(Particle::Parameter parameter, uint32_t howMany) {
	for (uint32_t i = 0; i < howMany; i++) {
		AddParticle(parameter);
	}
}

void ParticleSystem::Generate(
    Particle::Parameter parameter, uint32_t howMany, uint32_t textureHandle) {
	for (uint32_t i = 0; i < howMany; i++) {
		AddParticle(parameter, textureHandle);
	}
}
