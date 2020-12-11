/**
 * @file	AMG_Summer_Co_Production_2020\script\PlayWAVE.h.
 *
 * @brief	XAudio2を用いた音声再生
 * @author	Takuya Fujisawa
 */

#pragma once
#define XAUDIO2_HELPER_FUNCTIONS
#define _USE_MATH_DEFINES
#include<math.h>
#include<cstdlib>
#include<ctime>
#include<thread>
#include<xaudio2.h>
#include"MyFadeXAPO.h"
#include"MyLowPassFilterXAPO.h"
#include"MyHighPassFilter.h"
#include"MyPeakMeterXAPO.h"
#include"../Resource/ResourceServer.h"

namespace sound
{

	namespace
	{
		constexpr float PAN_LEFT = -90.0f;
		constexpr float PAN_RIGHT = 90.0f;
		constexpr float PAN_CENTER = 0.0f;
	}

	/**
	 * @class	PlayWAVE
	 *
	 * @brief	wavファイルの再生
	 *
	 */
	class PlayWAVE
	{
	public:
		PlayWAVE();
		~PlayWAVE();

		/**
		 * @fn	static void PlayWAVE::Init();
		 *
		 * @brief	XAudio2の初期化処理
		 *
		 */
		static void Init();

		/**
		 * @fn	static void PlayWAVE::Release();
		 *
		 * @brief	XAudio2の解放処理
		 *
		 */
		static void Release();

		/**
		 * @fn	void PlayWAVE::Load(const char* _fileName);
		 *
		 * @brief	wavファイルを読み込み、ソースボイス作成
		 *
		 * @param 	_fileName	音声ファイル名
		 */
		void Load(const char* _fileName);

		/**
		 * @fn	void PlayWAVE::SetLoopCount(const int _count);
		 *
		 * @brief	ループ回数を設定
		 *
		 * @param 	_count	_countは折り返す回数
		 * 					ex) _count = 1 　→　2回再生
		 * 					無限ループは XAUDIO2_LOOP_INFINITEを設定
		 */
		void SetLoopCount(const int _count);

		/**
		 * @fn	void PlayWAVE::SetVolume(const float _volume);
		 *
		 * @brief	SourceVoiceの音量を設定
		 *
		 * @param 	_volume	MAX : 1.0f
		 * 									MIN  : 0.0f
		 */
		void SetVolume(const float _volume);

		/**
		 * @fn	void PlayWAVE::SetVolume_dB(const float _db);
		 *
		 * @brief	デシベル値でボリュームを設定
		 *
		 * @param 	_db	デシベル		MAX : 0[dB]
		 * 												MIN  : -40[dB]
		 * 												1/2	: -6[dB]
		 */
		void SetVolume_dB(const float _db);

		/**
		 * @fn	void PlayWAVE::Pan(const float _targetPan);
		 *
		 * @brief	出力チャンネルの音量バランスの設定 (-90.0f <= _targetPan <= 90.0f)
		 * 			Lのみ→PAN_LEFT　センター→PAN_CENTER　Rのみ→PAN_RIGHT
		 *
		 * @param 	_targetPan	音量バランス
		 * 										Lのみ→PAN_LEFT
		 * 										センター→PAN_CENTER
		 * 										Rのみ→PAN_RIGHT
		 *
		 */
		void Pan(const float _targetPan);

		/**
		 * @fn	void PlayWAVE::Pan(const int _pos_x);
		 *
		 * @brief	画面の幅に対する左右の音量バランスの設定
		 *
		 * @param 	_pos_x	x座標
		 */
		void Pan(const int _pos_x);

		/**
		 * @fn	void PlayWAVE::Pitch(const float _pitch);
		 *
		 * @brief	再生速度を設定
		 *
		 * @param 	_pitch	再生速度
		 * 								MAX	:	16.0f
		 * 								MIN	:	0.0f
		 */
		void Pitch(const float _pitch);

		/**
		 * @fn	void PlayWAVE::RandomPitch(const float _pitch_diff);
		 *
		 * @brief	SE用ランダムピッチ設定
		 *
		 * @param 	_pitch_diff	等倍再生から±_pitch_diff の範囲の乱数で再生される.
		 */
		void RandomPitch(const float _pitch_diff);

		/**
		 * @fn	void PlayWAVE::Pause();
		 *
		 * @brief	一時停止
		 *
		 */
		void Pause();

		/**
		 * @fn	void PlayWAVE::Start();
		 *
		 * @brief	一時停止後の再生
		 * @detail	一時停止時にFadeによって音量が0になっているため、Fadeinで戻す
		 *
		 */
		void ReStart();

		/**
		 * @fn	void PlayWAVE::Fade(const float _targetVolume, const float _targetTime);
		 *
		 * @brief	フェード処理
		 *
		 * @param 	_targetVolume	フェードイン		:	1.0f
		 * 											フェードアウト	:	0.0f
		 * @param 	_targetTime  	フェード時間
		 */
		void Fade(const float _targetVolume, const float _targetTime);

		/**
		 * @fn	void PlayWAVE::FadeWithEQ(const float _targetVolume, const float _targetTime, const float _freqency, const int _typeEQ);
		 *
		 * @brief	EQをかけながらフェード処理を行う
		 *
		 * @param 	_targetVolume	フェードイン		:	1.0f
		 * 											フェードアウト	:	0.0f
		 * @param 	_targetTime  	フェード時間
		 * @param 	_freqency			遮断周波数
		 * @param 	_typeEQ		 	LPF	:	0
		 * 											HPF	:	1
		 */
		void FadeWithEQ(const float _targetVolume, const float _targetTime, const float _freqency, const int _typeEQ);

		/**
		 * @fn	void PlayWAVE::LowPassFilter(const float _freqency);
		 *
		 * @brief	ローパスフィルター.
		 *
		 * @param 	_freqency	遮断周波数
		 */
		void LowPassFilter(const float _freqency);

		/**
		 * @fn	void PlayWAVE::HighPassFilter(const float _freqency);
		 *
		 * @brief	ハイパスフィルター
		 *
		 * @param 	_freqency	遮断周波数
		 */
		void HighPassFilter(const float _freqency);

		/**
		 * @fn	void PlayWAVE::Play();
		 *
		 * @brief	WAVEファイルをはじめから再生
		 *
		 */
		void Play();

		/**
		 * @fn	void PlayWAVE::PlayWithLoop(float _loopbegin, float looplength);
		 *
		 * @brief	ループ再選
		 *
		 * @param 	_loopbegin	ループ開始地点[s]
		 * @param 	looplength		ループ継続時間[s]
		 */
		void PlayWithLoop(float _loopbegin, float looplength);

		/**
		 * @fn	void PlayWAVE::PlayBackGround(PlayWAVE& _pw);
		 *
		 * @brief	バックグラウンド再生
		 *
		 * @param [in,out]	PlayWAVEの参照
		 */
		void PlayBackGround(PlayWAVE& _pw);

		/**
		 * @fn	void PlayWAVE::PlayBackGroundWithLoop(PlayWAVE& _pw, float _loopbegin, float looplength);
		 *
		 * @brief	バックグラウンドでループ再生
		 *
		 * @param [in,out]	PlayWAVEの参照
		 * 	@param 	_loopbegin	ループ開始地点[s]
		 * @param 	looplength		ループ継続時間[s]
		 */
		void PlayBackGroundWithLoop(PlayWAVE& _pw, float _loopbegin, float looplength);

		/**
		 * @fn	void PlayWAVE::Destroy();
		 *
		 * @brief	ソースボイスの破棄
		 *
		 */
		void Destroy();

		/**
		 * @fn	bool PlayWAVE::CheckIsRunning();
		 *
		 * @brief	再生中か確認
		 *
		 * @returns	再生中の可否
		 */
		bool CheckIsRunning();

	private:
		static IXAudio2* m_pXAudio2;	//!< XAudio2の作成
		static IXAudio2MasteringVoice* m_pMasteringVoice;   //!< MasteringVoiceの作成
		static HRESULT hr;
		WAVEReader m_WaveReader;	//!< WAVEファイル読み込みクラス
		IXAudio2SourceVoice* m_pSourceVoice = nullptr;  //!< SourceVoiceの作成
		int m_LoopCount = 0;	//!< ループ回数
	};
}