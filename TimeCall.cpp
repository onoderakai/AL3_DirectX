#include "TimeCall.h"

TimeCall::TimeCall(function<void(void)> f, uint32_t time) {
	time_ = time;
	f_ = f;
}

void TimeCall::Update() {
	if (isFinish_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinish_ = true;
		f_();
	}
}
