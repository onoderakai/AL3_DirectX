#pragma once

#include <functional>
using namespace std;

class TimeCall {
public:
	TimeCall(function<void(void)> f, uint32_t time);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 完了ならtrueを返す
	/// </summary>
	/// <returns></returns>
	bool IsFinished() { return isFinish_; }

private:
	function<void(void)> f_;
	//残り時間
	uint32_t time_;
	//完了フラグ
	bool isFinish_ = false;
};
