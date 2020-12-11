/**
 * @file	AMG_Summer_Co_Production_2020\script\MyPeakMeterXAPO.cpp.
 *
 * @brief	全区域ピークメータ
 * @remark 参考文献 https://tsuchidasama.booth.pm/items/1417202 
 */

#include"MyPeakMeterXAPO.h"

namespace sound
{
	XAPO_REGISTRATION_PROPERTIES MyPeakMeterXAPO::xapoRegProp_ =
	{
		__uuidof(MyPeakMeterXAPO),//GUIDを問いあわせ、代入
		L"MyPeakMeterXAPO",
		L"(c)Yohinori Tsuchida All Right reserved.",
		1,0,//メジャーバージョン番号、マイナーバージョン番号
		XAPOBASE_DEFAULT_FLAG,
		1,1,1,1
	};

	MyPeakMeterXAPO::MyPeakMeterXAPO() :CXAPOParametersBase(&xapoRegProp_,
		(BYTE*)PeakLevels, sizeof(float), TRUE)
	{
		//エフェクトパラメータの初期化
		for (int i = 0; i < EFFECT_PARAMS; i++)
			PeakLevels[i] = 0;
	}

	HRESULT MyPeakMeterXAPO::LockForProcess(
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

	void MyPeakMeterXAPO::SetParameters(
		const void* pParameters,
		UINT32 ParameterByteSize)
	{
		if (ParameterByteSize == sizeof(float))//サイズ一致ならOK
			CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
	}

	void MyPeakMeterXAPO::Process(
		UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
		BOOL IsEnabled)
	{
		float* in = (float*)pInputProcessParameters[0].pBuffer;
		float* out = (float*)pOutputProcessParameters[0].pBuffer;

		float* param = (float*)BeginProcess();
		for (unsigned int i = 0; i < pInputProcessParameters[0].ValidFrameCount; i++)
		{
			float volume = *in;
			//マイナスなら符号反転
			if (volume < 0.0f)
				volume = -volume;
			/*
			符号となっている先頭1bitを反転させる場合は
			*(UINT32*)&volume &= 0x7fffffff;
			*/

			//過去最大と比較して大きければ差し替え
			if (volume > MaxVolume)
				MaxVolume = volume;

			//波形は何もせずそのままコピーする
			*out = *in;
			in++;
			out++;
		}
		//過去最大値をパラメータエリアに格納する
		*param = MaxVolume;
		EndProcess();
	}
}