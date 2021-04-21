/**
 * @file	AMG_Summer_Co_Production_2020\script\Sound.cpp.
 *
 * @brief	XAudio2を用いた音声再生
 */

#include"Sound.h"
#include"../Utility/Utility.h"
#include"../Application/ApplicationBase.h"
#include"DxLib.h"

namespace sound
{
	IXAudio2* Sound::xaudio2_ = nullptr;
	IXAudio2MasteringVoice* Sound::mastering_voice_ = nullptr;
	HRESULT Sound::hr_;

	Sound::Sound() {}
	Sound::~Sound()
	{
		if (CheckIsRunning() == false)
			return;

		Pause();
		Destroy();
	}

	void Sound::Init()
	{
		//COMの初期化
		//DXLibでCOMの初期化が行われるので、こちらでは初期化しない
		//CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		//XAudio2の作成
		UINT32 flags = 0;
		hr_ = XAudio2Create(&xaudio2_, flags);
		if (FAILED(hr_))
			CoUninitialize();//終了処理

		//MasteringVoiceの作成
		hr_ = xaudio2_->CreateMasteringVoice(&mastering_voice_);
		if (FAILED(hr_))
		{
			xaudio2_->Release();
			xaudio2_ = nullptr;
			CoUninitialize();
		}
	}

	void Sound::Release()
	{
		mastering_voice_->DestroyVoice();
		xaudio2_->Release();
		//DXLibでCOMの解放処理が行われるので、こちらでは初期化しない
		//CoUninitialize();
	}

	void Sound::Load(const std::string& _filename)
	{
		if (utility::GetExtension(_filename) == ".wav")
		{
			//WAVEファイルの読み込み
			wave_reader_ = resource::ResourceServer::LoadSound(_filename);

			//SourceVoiceの作成
			WAVEFORMATEX wfx = wave_reader_.Getwfx();
			hr_ = xaudio2_->CreateSourceVoice(&source_voice_, &wfx, XAUDIO2_VOICE_USEFILTER, 16.0f);
			if (FAILED(hr_))
			{
				free(wave_reader_.GetpBuffer());
				std::string error = "can not create SourceVoice \n" + _filename;
				utility::DrawDebugError(error.c_str());
			}
		}

		if (utility::GetExtension(_filename) == ".ogg")
		{
			ogg_reader_.LoadOGG(_filename.c_str());

			//SourceVoiceの作成
			WAVEFORMATEX wfx = ogg_reader_.Getwfx();
			hr_ = xaudio2_->CreateSourceVoice(&source_voice_, &wfx, XAUDIO2_VOICE_USEFILTER, 16.0f);
			if (FAILED(hr_))
			{
				free(ogg_reader_.GetpBuffer());
				std::string error = "can not create SourceVoice \n" + _filename;
				utility::DrawDebugError(error.c_str());
			}

		}
	}

	void Sound::SetLoopCount(const int& _count)
	{
		loop_count_ = _count;
	}

	void Sound::SetVolume(const float& _volume)
	{
		source_voice_->SetVolume(_volume);
	}

	void Sound::SetVolume_dB(const float& _db)
	{
		float volume = XAudio2DecibelsToAmplitudeRatio(_db);
		source_voice_->SetVolume(volume);
	}

	void Sound::Pan(const float& _targetPan)
	{
		int in_channel, out_channel;
		XAUDIO2_VOICE_DETAILS details;

		//入出力チャンネル数の取得
		source_voice_->GetVoiceDetails(&details);
		in_channel = details.InputChannels;
		source_voice_->GetVoiceDetails(&details);
		out_channel = details.InputChannels;

		//volumeが0.0f~1.0fの範囲であるため、-90～90から0~90度へ正規化し、度からラジアンへ変換
		float rad = ((_targetPan + 90.0f) / 2.0f) * (static_cast<float>(M_PI) / 180.0f);

		/*
			注:SetOutputMatrixの入出力チャンネルについて

			配列要素数 = 入力チャンネル数 * 出力チャンネル数
			入力→Stereo 2ch 出力→Stereoの場合
								<input>	|	要素番号
			<output>		L		 R		|
					L		   1.0   0.0		|	[0]	[1]
					R		   0.0   1.0		|	[2]	[3]
			のようになる。(行:Output,列:Input)
		*/

		//モノラル
		if (in_channel == 1)
		{
			float pan_volumes[2];
			pan_volumes[0] = cosf(rad);			//Input L > Output L
			pan_volumes[1] = sinf(rad);			//Input R > Output R

			source_voice_->SetOutputMatrix(NULL, in_channel, out_channel, pan_volumes);
		}
		//ステレオ
		if (in_channel == 2)
		{
			float pan_volumes[4];
			pan_volumes[0] = cosf(rad);			//Input L > Output L
			pan_volumes[1] = 0.0f;					//Input R > Output L(いらない)
			pan_volumes[2] = 0.0f;					//Input L > Output R(いらない)
			pan_volumes[3] = sinf(rad);			//Input R > Output R

			source_voice_->SetOutputMatrix(NULL, in_channel, out_channel, pan_volumes);
		}
	}

	void Sound::Pan(const int& _pos_x)
	{
		int in_channel, out_channel;
		XAUDIO2_VOICE_DETAILS details;

		//入出力チャンネル数の取得
		source_voice_->GetVoiceDetails(&details);
		in_channel = details.InputChannels;
		source_voice_->GetVoiceDetails(&details);
		out_channel = details.InputChannels;

		//volumeが0.0f~1.0fの範囲であるため、-90～90から0~90度へ正規化し、度からラジアンへ変換
		float rad = ((static_cast<float>(_pos_x) / static_cast<float>(appframe::SCREEN_WIDTH)) * 90.0f) * (DX_PI_F / 180.0f);

		/*
			注:SetOutputMatrixの入出力チャンネルについて

			配列要素数 = 入力チャンネル数 * 出力チャンネル数
			入力→Stereo 2ch 出力→Stereoの場合
								<input>	|	要素番号
			<output>		L		 R		|
					L		   1.0   0.0	|	[0]	[1]
					R		   0.0   1.0	|	[2]	[3]
			のようになる。(行:Output,列:Input)
		*/

		//モノラル
		if (in_channel == 1)
		{
			float pan_volumes[2];
			pan_volumes[0] = cosf(rad);			//Input L > Output L
			pan_volumes[1] = sinf(rad);			//Input R > Output R

			source_voice_->SetOutputMatrix(NULL, in_channel, out_channel, pan_volumes);
		}
		//ステレオ
		if (in_channel == 2)
		{
			float pan_volumes[4];
			pan_volumes[0] = cosf(rad);			//Input L > Output L
			pan_volumes[1] = 0.0f;					//Input R > Output L(いらない)
			pan_volumes[2] = 0.0f;					//Input L > Output R(いらない)
			pan_volumes[3] = sinf(rad);			//Input R > Output R

			source_voice_->SetOutputMatrix(NULL, in_channel, out_channel, pan_volumes);
		}
	}

	void Sound::Pitch(const float& _pitch)
	{
		source_voice_->SetFrequencyRatio(_pitch);
	}

	void Sound::RandomPitch(const float& _pitch_diff)
	{
		//srand((unsigned)time(NULL));
		float random = ((rand() % 100) - 50) * 0.01f * _pitch_diff;

		source_voice_->SetFrequencyRatio(1.0f + random);
	}

	void Sound::Pause()
	{
		Fade(0.0f, 0.0f);
		source_voice_->DisableEffect(0);
		source_voice_->Stop(XAUDIO2_PLAY_TAILS);
	}

	void Sound::ReStart()
	{
		Fade(1.0f, 0.01f);
		source_voice_->Start();
	}

	void Sound::Fade(const float& _targetVolume, const float& _targetTime)
	{
		//Fadeフィルターの宣言
		IUnknown* pXAPO;
		pXAPO = (IXAPO*)new MyFadeXAPO();

		//EFFECT_DESCRIPTOR構造体の宣言
		XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		descriptor.InitialState = TRUE;
		descriptor.OutputChannels = 2;
		descriptor.pEffect = pXAPO;

		//EFFECT_CHAIN構造体の宣言
		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		//MyFadeXAPOをpSourceVoiceに挿入
		source_voice_->SetEffectChain(&chain);
		//二重管理にならないように破棄
		pXAPO->Release();

		MyFadeXAPO::MyFadeParam fade_params;
		fade_params.TargetVolume = _targetVolume;
		fade_params.TargetTime = _targetTime;

		source_voice_->SetEffectParameters(0, &fade_params, sizeof(fade_params));
	}

	void Sound::FadeWithEQ(const float& _targetVolume, const float& _targetTime, const float& _freqency, const int& _typeEQ)
	{
		//Fadeフィルターの宣言
		IUnknown* FadeXAPO;
		FadeXAPO = (IXAPO*)new MyFadeXAPO();

		//EFFECT_DESCRIPTOR構造体の宣言
		XAUDIO2_EFFECT_DESCRIPTOR descriptor[2];
		descriptor[0].InitialState = TRUE;
		descriptor[0].OutputChannels = 2;
		descriptor[0].pEffect = FadeXAPO;

		//0 : LowPassFilter   1 : HighPassFilter
		IUnknown* EQXAPO;
		if (_typeEQ == 1)
		{
			EQXAPO = (IXAPO*)new MyHighPassFilterXAPO();

			descriptor[1].InitialState = true;
			descriptor[1].OutputChannels = 2;
			descriptor[1].pEffect = EQXAPO;
		}
		else
		{
			EQXAPO = (IXAPO*)new MyLowPassFilterXAPO();

			descriptor[1].InitialState = true;
			descriptor[1].OutputChannels = 2;
			descriptor[1].pEffect = EQXAPO;
		}

		//EFFECT_CHAIN構造体の宣言
		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 2;
		chain.pEffectDescriptors = descriptor;

		//二重管理にならないように破棄
		FadeXAPO->Release();
		EQXAPO->Release();

		MyFadeXAPO::MyFadeParam fade_params;
		fade_params.TargetVolume = _targetVolume;
		fade_params.TargetTime = _targetTime;
		source_voice_->SetEffectParameters(0, &fade_params, sizeof(fade_params));

		WAVEFORMATEX wfx = wave_reader_.Getwfx();
		float freq = 2.0f * sinf(static_cast<float>(M_PI) * _freqency / wfx.nSamplesPerSec);
		source_voice_->SetEffectParameters(1, &freq, sizeof(freq));
	}

	void Sound::LowPassFilter(const float& _freqency)
	{
		WAVEFORMATEX wfx = wave_reader_.Getwfx();

		XAUDIO2_FILTER_PARAMETERS FilterParams;
		FilterParams.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
		float freq = 2.0f * sinf(static_cast<float>(M_PI) * _freqency / wfx.nSamplesPerSec);
		if (freq > XAUDIO2_MAX_FILTER_FREQUENCY)
			freq = XAUDIO2_MAX_FILTER_FREQUENCY;
		FilterParams.Frequency = freq;
		FilterParams.OneOverQ = 1.4142f;

		source_voice_->SetFilterParameters(&FilterParams);
	}

	void Sound::HighPassFilter(const float& _freqency)
	{
		IUnknown* pXAPO;
		pXAPO = (IXAPO*)new MyHighPassFilterXAPO();

		XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		descriptor.InitialState = true;
		descriptor.OutputChannels = 2;
		descriptor.pEffect = pXAPO;

		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		source_voice_->SetEffectChain(&chain);
		pXAPO->Release();

		WAVEFORMATEX wfx = wave_reader_.Getwfx();
		float freq = 2.0f * sinf(static_cast<float>(M_PI) * _freqency / wfx.nSamplesPerSec);
		source_voice_->SetEffectParameters(0, &freq, sizeof(freq));
	}

	void Sound::Echo(const float& _wetdrymix, const float& _delay, const float& _feedback)
	{
		IUnknown* pXAPO;
		::CreateFX(__uuidof(::FXEcho), &pXAPO);

		XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		descriptor.InitialState = true;
		descriptor.OutputChannels = 2;
		descriptor.pEffect = pXAPO;

		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		source_voice_->SetEffectChain(&chain);
		pXAPO->Release();

		//エコーパラメータ
		FXECHO_PARAMETERS EchoParam;
		EchoParam.WetDryMix = _wetdrymix;
		EchoParam.Delay = _delay;
		EchoParam.Feedback = _feedback;

		//エフェクトを挿入
		source_voice_->SetEffectParameters(0, &EchoParam, sizeof(EchoParam));
	}

	void Sound::Reverb()
	{
		IUnknown* pXAPO;
		CreateAudioReverb(&pXAPO);

		XAUDIO2_EFFECT_DESCRIPTOR descriptor;
		descriptor.InitialState = true;
		descriptor.OutputChannels = 2;
		descriptor.pEffect = pXAPO;

		XAUDIO2_EFFECT_CHAIN chain;
		chain.EffectCount = 1;
		chain.pEffectDescriptors = &descriptor;

		source_voice_->SetEffectChain(&chain);
		pXAPO->Release();
	}

	void Sound::Play()
	{
		//XAudio2_BUFFER構造体の作成
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = (BYTE*)wave_reader_.GetpBuffer();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes = wave_reader_.GetDataChunk().chunk_size;
		buffer.PlayBegin = 0;
		buffer.LoopCount = loop_count_;

		//波形データの情報をセット
		if (FAILED(hr_ = source_voice_->SubmitSourceBuffer(&buffer)))
		{
			source_voice_->DestroyVoice();
			std::string error = "can not submitting source buffer";
			utility::DrawDebugError(error.c_str());
		}

		if (SUCCEEDED(hr_ = source_voice_->Start()))
		{
		}
	}

	void Sound::PlayWithLoop(const float& _loopbegin, const float& _looplength)
	{
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.pAudioData = (BYTE*)wave_reader_.GetpBuffer();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.AudioBytes = wave_reader_.GetDataChunk().chunk_size;
		buffer.PlayBegin = 0;
		buffer.LoopCount = loop_count_;
		buffer.LoopBegin = static_cast<int>(_loopbegin * wave_reader_.Getwfx().nSamplesPerSec);
		buffer.LoopLength = static_cast<int>(_looplength * wave_reader_.Getwfx().nSamplesPerSec);

		//波形データの情報をセット
		if (FAILED(hr_ = source_voice_->SubmitSourceBuffer(&buffer)))
		{
			source_voice_->DestroyVoice();
			std::string error = "can not submitting source buffer";
			utility::DrawDebugError(error.c_str());
		}

		if (SUCCEEDED(hr_ = source_voice_->Start()))
		{
		}
	}

	bool Sound::CheckIsRunning()
	{
		if (source_voice_ == nullptr)
			return false;

		XAUDIO2_VOICE_STATE state;
		source_voice_->GetState(&state);
		if (state.BuffersQueued > 0)
			return true;
		else
			return false;
	}

	void Sound::Destroy()
	{
		source_voice_->DestroyVoice();
		source_voice_ = nullptr;
	}

	void Sound::PlayBackGround(Sound& _pw)
	{
		std::thread th(&Sound::Play, &_pw);
		th.detach();
	}

	void Sound::PlayBackGroundWithLoop(Sound& _pw, const float& _loopbegin, const float& _looplength)
	{
		std::thread th(&Sound::PlayWithLoop, &_pw, _loopbegin, _looplength);
		th.detach();
	}
}