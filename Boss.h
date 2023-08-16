#pragma once
#include "Collider.h"
#include "WorldTransform.h"

class Boss : public Collider {
public:
	~Boss();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
private:
	WorldTransform world_;
};
