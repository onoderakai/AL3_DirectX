#include "Title.h"
#include "ImGuiManager.h"
#include "SceneChange.h"
#include "TextureManager.h"

Title::Title() {
	titleEasing_ = new Easing();
	titleCatEasing_ = new Easing();
	sound_ = SoundManager::GetInstance();

	// 画像を読み込み、スプライトを生成する
	titleTextureHandle_ = TextureManager::Load("title2.png");
	uint32_t titleCatTex = TextureManager::Load("title_cat.png");

	uint32_t titleBgTextureHandle_ = TextureManager::Load("title_bg.png");
	uint32_t titleDarumaTextureHandle_ = TextureManager::Load("title_daruma.png");
	titleSprite_ = Sprite::Create(
	    titleTextureHandle_, Vector2{640.0f, 240.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f},
	    Vector2{0.5f, 0.5f});
	titleCatSprite_ = Sprite::Create(
	    titleCatTex, Vector2{640.0f, 240.0f}, Vector4{1.0f, 1.0f, 1.0f, 1.0f}, Vector2{0.5f, 0.5f});
	titleBgSprite_ = Sprite::Create(titleBgTextureHandle_, Vector2{0.0f, 0.0f});
	titleDarumaSprite_ = Sprite::Create(titleDarumaTextureHandle_, Vector2{0.0f, 0.0f});

	uint32_t pushNextTextureHandle = TextureManager::Load("push_next.png");
	pushNextSprite_ =
	    Sprite::Create(pushNextTextureHandle, Vector2{640.0f, 550.0f}, {1, 1, 1, 1}, {0.5f, 0.5f});

	// 開始座標を保存
	titleStartPos_ = titleSprite_->GetPosition() - Vector2{0.0f, 5.0f};
	titleEndPos_ = titleSprite_->GetPosition() + Vector2{0.0f, 5.0f};
	titleCatStartPos_ = titleCatSprite_->GetPosition() + Vector2{0.0f, 40.0f};
	titleCatEndPos_ = titleCatSprite_->GetPosition() - Vector2{0.0f, 10.0f};

	tmpTitleStartPos_ = titleStartPos_;
	tmpTitleEndPos_ = titleEndPos_;
	tmpTitleCatStartPos_ = titleCatStartPos_;
	tmpTitleCatEndPos_ = titleCatEndPos_;
}

Title::~Title() {
	delete titleEasing_;
	delete titleCatEasing_;
	delete titleSprite_;
	delete titleCatSprite_;
	delete titleBgSprite_;
	delete pushNextSprite_;
	delete titleDarumaSprite_;
}

void Title::Initialize(SceneNum* pScene) {
	input_ = Input::GetInstance();
	pScene_ = pScene;
	isTitleEase_ = true;
	isTitleCatEase_ = true;
	isTitleCatEaseChange_ = false;
	titleEasing_->Initialize();
	titleCatEasing_->Initialize();

	titleStartPos_ = tmpTitleStartPos_;
	titleEndPos_ = tmpTitleEndPos_;
	titleCatStartPos_ = tmpTitleCatStartPos_;
	titleCatEndPos_ = tmpTitleCatEndPos_;
}

void Title::Update() {
	// 接続状態を確認
	Input::GetInstance()->GetJoystickState(0, joyState_);
	Input::GetInstance()->GetJoystickStatePrevious(0, preJoyState_);

	// SPACEかABXYボタンでSTAGE_SELECTシーンに遷移する
	if (input_->TriggerKey(DIK_SPACE) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) == 0) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_B) == 0) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_X) == 0) ||
	    (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y &&
	     (preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_Y) == 0)) {
		sound_->OnPlaySound(SoundManager::Sound::SE_CONFIRM);
		SceneChange::GetInstance()->Change(SceneNum::STAGE_SELECT, pScene_);
	}

	// タイトル画像のイージング
	titleSprite_->SetPosition(titleEasing_->ConstantEase(
	    titleSprite_->GetPosition(), titleStartPos_, titleEndPos_, 40, isTitleEase_));
	if (isTitleCatEaseChange_) {
		titleCatSprite_->SetPosition(titleCatEasing_->EaseInSine(
		    titleCatSprite_->GetPosition(), titleCatStartPos_, titleCatEndPos_, 60,
		    isTitleCatEase_));
	} else {
		titleCatSprite_->SetPosition(titleCatEasing_->EaseOutSine(
		    titleCatSprite_->GetPosition(), titleCatStartPos_, titleCatEndPos_, 60,
		    isTitleCatEase_));
	}
	if (!isTitleEase_) {
		Vector2 tmpStartPos = titleStartPos_;
		titleStartPos_ = titleEndPos_;
		titleEndPos_ = tmpStartPos;
		isTitleEase_ = true;
	}
	if (!isTitleCatEase_) {
		Vector2 tmpStartPos = titleCatStartPos_;
		titleCatStartPos_ = titleCatEndPos_;
		titleCatEndPos_ = tmpStartPos;
		isTitleCatEaseChange_ = !isTitleCatEaseChange_;
		isTitleCatEase_ = true;
	}

	isChange_ = SceneChange::GetInstance()->GetIsLoading();
	// シーン遷移中なら透明度を変えない
	if (isChange_) {
		pushNextSprite_->SetColor(Vector4{1.0f, 1.0f, 1.0f, 1.0f});
		return;
	}
	// 透明度を徐々に変える
	Vector4 tmpColor = pushNextSprite_->GetColor();
	tmpColor.w += flashSpeed_;
	if (tmpColor.w <= 0.1f) {
		tmpColor.w = 0.1f;
		flashSpeed_ *= -1.0f;
	} else if (tmpColor.w >= 1.0f) {
		tmpColor.w = 1.0f;
		flashSpeed_ *= -1.0f;
	}
	pushNextSprite_->SetColor(tmpColor);
}

void Title::DrawBackground() {
	titleBgSprite_->Draw();
	titleDarumaSprite_->Draw();
	titleCatSprite_->Draw();
	titleSprite_->Draw();
	pushNextSprite_->Draw();
}
