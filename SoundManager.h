#pragma once
#include "Audio.h"

class SoundManager {
public:
	/// <summary>
	/// コピーコンストラクタの禁止
	/// </summary>
	/// <param name="sceneChange"></param>
	SoundManager(const SoundManager& soundManager) = delete;

	/// <summary>
	/// 代入演算子の禁止
	/// </summary>
	/// <param name="sceneChange"></param>
	/// <returns></returns>
	SoundManager& operator=(const SoundManager& soundManager) = delete;

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static SoundManager* GetInstance();

	enum class Sound {
		SE_DARUMA_BREAK,
		SE_MISS_TOUCH,
		SE_ICHATCH_COUNT,
		SE_ICHATCH_START,
		SE_SELECT_RB_LB,
		SE_EXPLAIN,
		SE_FINISH,
		SE_CONFIRM,
		SE_LINE_UP,
		SE_RESULT,
	};

	enum class BGM {
		BGM_TITLE,
		BGM_TIME_ATTACK,
	};

	void OnPlaySound(const Sound& soundType);

	void OnPlayBGM(const BGM& bgmType);

	//void AllStopSound();

private:
	SoundManager();

	~SoundManager();

	Audio* audio_ = nullptr;

	uint32_t seDarumaBreakData_ = 0;
	uint32_t seMissTouchData_ = 0;
	uint32_t seIchatchCountData_ = 0;
	uint32_t seIchatchStartData_ = 0;
	uint32_t seSelectRBLBData_ = 0;
	uint32_t seLineUpData = 0;
	uint32_t seResultData = 0;
	uint32_t seExplainData_ = 0;
	uint32_t seConfirmData_ = 0;
	uint32_t seFinishData_ = 0;
	uint32_t bgmTitleData_ = 0;
	uint32_t bgmStageData_ = 0;
	Sound sound_ = Sound::SE_DARUMA_BREAK;
};
