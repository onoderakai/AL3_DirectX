#include "Title.h"

void Title::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
}

void Title::Update() {
	if (input_->PushKey(DIK_SPACE)) {
		*pScene_ = SceneNum::STAGE;
	}
}

void Title::Draw() {}
