#pragma once
#include "Particle.h"
#include "Model.h"
#include <list>

class ParticleSystem {
public:

	~ParticleSystem();

	void Initialize();

	void Update();

	void Draw(const ViewProjection& view);

	void AddParticle();

	void Generate(uint32_t howMany);

private:
	//パーティクルの包含
	std::list<Particle*> particles_;

	//パーティクルのモデル
	Model* particleModel_ = nullptr;
};
