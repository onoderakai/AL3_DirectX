#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "DarumaType.h"
#include "Input.h"
#include "ViewProjection.h"
#include <cstdint>

class Daruma {
public:
	Daruma();

	~Daruma();

	void Initialize(Model* model,  const Vector3& pos, const DarumaType& type);

	void Update(const uint32_t& arrayNum);

	void Draw(const ViewProjection& view);

	bool GetIsBreak() { return isBreak_; }

private:
	Input* input_ = nullptr;

	DarumaType type_ = DarumaType::RED;
	Model* model_ = nullptr;
	WorldTransform world_;

	uint32_t arrayNum_ = 0;
	bool isBreak_ = false;

	void UpdateGreen();

	void UpdateRed();

	void UpdateBlue();

	void UpdateYellow();

};
