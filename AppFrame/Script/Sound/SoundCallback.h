/**
 * @file SoundCallback.h
 * @brief  XAudio2のコールバッククラス
 * sourceボイス作成時に挿入する
 *
 * @author Takuya Fujisawa
 * @date   2021/04/28
 */

#pragma once
#include<xaudio2.h>

namespace sound
{
	/**
	 * @brief xaudio2のsourcevoiceコールバッククラス
	 */
	class SoundCallback : public IXAudio2VoiceCallback
	{
	public:
		HANDLE event;

		SoundCallback() : event(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
		~SoundCallback() { CloseHandle(event); }

		/**
		 * @brief 音声がバッファ処理を終了したときに呼び出される
		 *
		 * @param _buffer_context XAUDIO2_BUFFER構造体のpBufferContextメンバに割り当てられたポインタ
		 */
		void STDMETHODCALLTYPE OnBufferEnd(void* _buffer_context) { SetEvent(event); }

		/**
		 * @brief 新規のオーディオバッファの処理を開始するときに呼び出される
		 */
		void STDMETHODCALLTYPE OnBufferStart(void* _buffer_context) {}

		/**
		 * @brief 音声がループ位置の終了地点に到達したときに呼び出される
		 *
		 * @param _buffer_context XAUDIO2_BUFFER構造体のpBufferContextメンバに割り当てられたポインタ
		 */
		void STDMETHODCALLTYPE OnLoopEnd(void* _buffer_context) {}

		/**
		 * @brief 音声が連続したオーディオストリーム再生を終了したときに呼び出される
		 */
		void STDMETHODCALLTYPE OnStreamEnd() {}

		/**
		 * @brief 音声処理中に重大なエラーが発生したときに呼び出される
		 *
		 * @param _buffer_context XAUDIO2_BUFFER構造体のpBufferContextメンバに割り当てられたポインタ
		 * @param _error 発生したエラーのHRESULTコード
		 */
		void STDMETHODCALLTYPE OnVoiceError(void* _buffer_context, HRESULT _error) {}

		/**
		 * @brief 音声の処理パスが終了したときに呼び出される
		 */
		void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}

		/**
		 * @brief XAudio2が音声のバッファキューからデータを読み取る直前に、各音声の処理パス中に呼び出される
		 *
		 * @param _bytes_required 飢餓状態を避けるために、送信しなければならないバイト数
		 */
		void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 _bytes_required) {}
	};
}