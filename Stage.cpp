#include "Stage.h"
#include "SceneChange.h"

void Stage::Initialize(SceneNum* pScene) {
	pScene_ = pScene;
	input_ = Input::GetInstance();
}

void Stage::Update() {
	// RETURNでTITLEシーンに遷移する
	if (input_->PushKey(DIK_RETURN)) {
		SceneChange::GetInstance()->Change(SceneNum::TITLE, pScene_);
	}
}

void Stage::Draw() {}
