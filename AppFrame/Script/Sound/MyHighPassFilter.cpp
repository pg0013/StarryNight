/**
 * @file	AMG_Summer_Co_Production_2020\script\MyHighPassFilter.cpp.
 *
 * @brief	ハイパスフィルター
 * @detail	遮断周波数の上限がサンプリング周波数の1/2
 * @remark 参考文献 https://tsuchidasama.booth.pm/items/1417202
 */

#include"MyHighPassFilter.h"
#define _USE_MATH_DEFINES
#include<math.h>

namespace sound
{
	XAPO_REGISTRATION_PROPERTIES MyHighPassFilterXAPO::xapoRegProp_ =
	{
		__uuidof(MyHighPassFilterXAPO),//GUIDを問いあわせ、代入
		L"MyHighPassFilterXAPO",
		L"(c)Yohinori Tsuchida All Right reserved.",
		1,0,//メジャーバージョン番号、マイナーバージョン番号
		XAPOBASE_DEFAULT_FLAG,
		1,1,1,1
	};

	MyHighPassFilterXAPO::MyHighPassFilterXAPO() :CXAPOParametersBase(&xapoRegProp_,
		(BYTE*)Freqency, sizeof(float), FALSE)
	{
		//エフェクトパラメータの初期化
		for (int i = 0; i < EFFECT_PARAMS; i++)
			Freqency[i] = 0.0f;
	}

	HRESULT MyHighPassFilterXAPO::LockForProcess(
		UINT32 InputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
		UINT32 OutputLockedParameterCount,
		const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters)
	{
		inputFmt_ = *pInputLockedParameters[0].pFormat;
		outputFmt_ = *pOutputLockedParameters[0].pFormat;

		return CXAPOBase::LockForProcess(InputLockedParameterCount, pInputLockedParameters,
			OutputLockedParameterCount, pOutputLockedParameters);
	}

	void MyHighPassFilterXAPO::SetParameters(
		const void* pParameters,
		UINT32 ParameterByteSize)
	{
		if (ParameterByteSize == sizeof(float))//サイズ一致ならOK
			CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
	}

	void MyHighPassFilterXAPO::Process(
		UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
		BOOL IsEnabled)
	{
		float* in = (float*)pInputProcessParameters[0].pBuffer;
		float* out = (float*)pOutputProcessParameters[0].pBuffer;
		float* freqency = (float*)BeginProcess();

		//ローパス係数の計算
		float a1 = expf(-static_cast<float>(M_PI) * *freqency);			//元の音を残すブレンド率
		float b0 = 1.0f - a1;									//新しい音のブレンド率

		float Volume = LastVolume;				//前回の最終値を取り出す

		for (unsigned int i = 0; i < pInputProcessParameters[0].ValidFrameCount * inputFmt_.nChannels; i++)
		{
			Volume = Volume * a1 + *in * b0;
			*out = *in - Volume;
			in++;
			out++;
		}
		LastVolume = Volume;
		EndProcess();
	}
}