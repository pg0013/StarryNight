/**
 * @file	AMG_Summer_Co_Production_2020\script\MyFadeXAPO.cpp.
 *
 * @brief	サンプル単位のフェード
 * @remark 参考文献 https://tsuchidasama.booth.pm/items/1417202
 */

#include"MyFadeXAPO.h"

namespace sound
{
	XAPO_REGISTRATION_PROPERTIES MyFadeXAPO::xapoRegProp_ =
	{
		__uuidof(MyFadeXAPO),//GUIDを問いあわせ、代入
		L"MyFadeXAPO",
		L"(c)Yohinori Tsuchida All Right reserved.",
		1,0,//メジャーバージョン番号、マイナーバージョン番号
		XAPOBASE_DEFAULT_FLAG,
		1,1,1,1
	};

	MyFadeXAPO::MyFadeXAPO() :CXAPOParametersBase(&xapoRegProp_,
		(BYTE*)FadeParams, sizeof(MyFadeParam), FALSE)
	{
	}

	HRESULT MyFadeXAPO::LockForProcess(
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

	void MyFadeXAPO::SetParameters(
		const void* pParameters,
		UINT32 ParameterByteSize)
	{
		MyFadeParam* param = (MyFadeParam*)pParameters;

		//指定時間が0なら即実行
		if (param->TargetTime == 0)
		{
			NowVolume = param->TargetVolume;
			SlopeVolume = 0.0f;
		}
		else
		{
			SlopeVolume = param->TargetVolume - NowVolume;
			SlopeVolume /= inputFmt_.nSamplesPerSec * inputFmt_.nChannels * param->TargetTime;//時間をサンプル数に変換
		}

		CXAPOParametersBase::SetParameters(pParameters, ParameterByteSize);
	}

	void MyFadeXAPO::GetParameters(
		void* pParameters,
		UINT32 ParameterByteSize)
	{
		*(bool*)pParameters = FadeFlag;
	}

	void MyFadeXAPO::Process(
		UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
		BOOL IsEnabled)
	{
		float* in = (float*)pInputProcessParameters[0].pBuffer;		//入力データの先頭アドレス
		float* out = (float*)pOutputProcessParameters[0].pBuffer;	//出力データの先頭アドレス

		MyFadeParam* param = (MyFadeParam*)BeginProcess();
		for (unsigned int i = 0; i < pInputProcessParameters[0].ValidFrameCount * inputFmt_.nChannels; i++)
		{
			float volume = *in;

			//今の波形にボリュームを乗算した後、volumeにはSlope分足す
			volume *= NowVolume;
			NowVolume += SlopeVolume;
			FadeFlag = false;
			//目標まで到達したら終了（浮動終点の誤差のため正確にやる）

			//フェードアウト
			if (SlopeVolume < 0.0f)
			{
				//フェードアウトのボリュームに到達
				if (NowVolume <= param->TargetVolume)
				{
					NowVolume = param->TargetVolume;
					SlopeVolume = 0.0f;
					FadeFlag = true;
				}
			}
			//フェードイン
			else if (SlopeVolume > 0.0f)
			{
				//フェードインのボリュームに到達
				if (NowVolume >= param->TargetVolume)
				{
					NowVolume = param->TargetVolume;
					SlopeVolume = 0.0f;
					FadeFlag = true;
				}
			}
			//出力データを書き込み
			*out = volume;
			in++;
			out++;

		}
		EndProcess();
	}
}