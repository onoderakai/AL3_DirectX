#pragma once
#include "Model.h"
#include "Particle.h"
#include <list>

class ParticleSystem {
public:
	ParticleSystem();

	~ParticleSystem();

	void Initialize();

	void Update();

	void Draw(const ViewProjection& view);

	void Generate(Particle::Parameter parameter, uint32_t howMany);

private:
	// パーティクルの包含
	std::list<Particle*> particles_;

	// パーティクルのモデル
	Model* particleModel_ = nullptr;

	void AddParticle(Particle::Parameter parameter);
};
