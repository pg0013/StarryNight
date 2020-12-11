/**
 * @file	AMG_Summer_Co_Production_2020\script\MyStereoDownMixXAPO.cpp.
 *
 * @brief	ステレオダウンミックス
 * @remark 参考文献 https://tsuchidasama.booth.pm/items/1417202
 */

#include"MyStereoDownMixXAPO.h"

namespace sound
{
	XAPO_REGISTRATION_PROPERTIES MyStereoDownMixXAPO::xapoRegProp_ =
	{
		__uuidof(MyStereoDownMixXAPO),//GUIDを問いあわせ、代入
		L"MyFadeXAPO",
		L"(c)Yohinori Tsuchida All Right reserved.",
		1,0,//メジャーバージョン番号、マイナーバージョン番号
		XAPOBASE_DEFAULT_FLAG,
		1,1,1,1
	};

	MyStereoDownMixXAPO::MyStereoDownMixXAPO() :CXAPOBase(&xapoRegProp_)
	{
	}

	HRESULT MyStereoDownMixXAPO::LockForProcess(
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

	void MyStereoDownMixXAPO::Process(
		UINT32 InputProcessParameterCount,
		const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
		UINT32 OutputProcessParameterCount,
		XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
		BOOL IsEnabled)
	{
		//6ch→2ch変換テーブル
		static const float L_Mix[6] = { 0.9f,0.1f,0.6f,0.4f,0.9f,0.0f };
		static const float R_Mix[6] = { 0.1f,0.9f,0.6f,0.4f,0.0f,0.9f };

		float* in = (float*)pInputProcessParameters[0].pBuffer;
		float* out = (float*)pOutputProcessParameters[0].pBuffer;

		//出力先初期位置をセット
		float* pLeft = out;
		float* pRight = out + 1;

		//サンプル数分回す
		for (unsigned int i = 0; i < pInputProcessParameters[0].ValidFrameCount; i++)
		{
			*pLeft = 0.0f;
			*pRight = 0.0f;

			//LRにチャンネル数分回して合成する
			for (int ch = 0; ch < inputFmt_.nChannels; ch++)
			{
				//ステレオが入力されたとき用
				if (inputFmt_.nChannels != 6)
				{
					*out = *in;
					in++;
					out++;
					break;
				}

				*pLeft += *in * L_Mix[ch];
				*pRight += *in * R_Mix[ch];
				in++;
			}
			pLeft += 2;
			pRight += 2;
		}
	}
}