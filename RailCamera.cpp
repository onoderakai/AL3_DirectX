#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 trans, Vector3 rot) {
	world_.Initialize();
	world_.translation_ = trans;
	world_.rotation_ = rot;

	viewProjection_.Initialize();
}

void RailCamera::Update() {

	//world_.rotation_.z += 0.1f;
	world_.translation_.z += 0.1f;
	ImGui::Begin("RailCamera");
	ImGui::DragFloat3("cameraTranslate", &world_.translation_.x, 0.01f);
	ImGui::DragFloat3("cameraRot", &world_.rotation_.x, 0.01f);
	ImGui::End();

	world_.UpdateMatrix();
	viewProjection_.matView = Inverse(world_.matWorld_);
	viewProjection_.TransferMatrix();
}
