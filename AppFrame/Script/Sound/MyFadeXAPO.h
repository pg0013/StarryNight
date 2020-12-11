/**
 * @file	AMG_Summer_Co_Production_2020\script\MyFadeXAPO.h.
 *
 * @brief	サンプル単位のフェード
 * @remark 参考文献 https://tsuchidasama.booth.pm/items/1417202
 */

#include"xapobase.h"
#pragma comment(lib,"xapobase.lib")

namespace sound
{

#define EFFECT_PARAMS 3

	/**
	 * @class	MyFadeXAPO :public CXAPOParametersBase
	 *
	 * @brief	サンプル単位でフェードを行うXAPOFX
	 *
	 */
	class __declspec(uuid("{C42A92CD-1D18-4227-9E84-2FACDD116DB6}"))
		MyFadeXAPO :public CXAPOParametersBase
	{
	public:
		MyFadeXAPO();
		~MyFadeXAPO() {};

		/**
		 * @fn	STDMETHOD(LockForProcess) ( UINT32 InputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters, UINT32 OutputLockedParameterCount, const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters);
		 *
		 * @brief	どういったデータが送られてきたのかの情報が渡される
		 * 	@detail	最初の一度だけ呼ばれ,フォーマットを格納する
		 *
		 * @param 	InputLockedParameterCount		第2引数の要素数、通常は１
		 * @param	pInputLockedParameters			Processで送られてくるデータのフォーマット構造体
		 * @param	OutputLockedParameterCount	第4引数の要素数、通常は1
		 * @param	pOutputLockedParameters			出力するデータのフォーマット構造体、このフォーマットで出力しなくてはならない
		 */
		STDMETHOD(LockForProcess)(
			UINT32 InputLockedParameterCount,
			const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pInputLockedParameters,
			UINT32 OutputLockedParameterCount,
			const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* pOutputLockedParameters);

		/**
		 * @fn	STDMETHOD_(void, SetParameters) ( const void* pParameters, UINT32 ParameterByteSize);
		 *
		 * @brief	エフェクトパラメータを設定
		 *
		 * @param 	pParameters				送られてきたパラメータの先頭アドレス
		 * @param 	ParameterByteSize	送られてきたパラメータのデータサイズ
		 */
		STDMETHOD_(void, SetParameters) (
			const void* pParameters,
			UINT32 ParameterByteSize);

		/**
		 * @fn	STDMETHOD_(void, GetParameters) ( void* pParameters, UINT32 ParameterByteSize);
		 *
		 * @brief	エフェクトパラメータを取得
		 *
		 * @param 	pParameters				パラメータを返す変数の先頭アドレス
		 * @param 	ParameterByteSize	送り返す必要のある、パラメータのデータサイズ
		 */
		STDMETHOD_(void, GetParameters)(
			void* pParameters,
			UINT32 ParameterByteSize);

		/**
		 * @fn	STDMETHOD_(void, Process) ( UINT32 InputProcessParameterCount, const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters, UINT32 OutputProcessParameterCount, XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters, BOOL IsEnabled );
		 *
		 * @brief	エフェクトの信号処理を行う
		 * @detail	1秒間に100回呼ばれる
		 *
		 * @param 	InputProcessParameterCount		第2引数の要素数
		 * @param 	pInputProcessParameters			送られてくる加工前の波形データと、付随情報の入力構造体
		 * @param	OutputProcessParameterCount	第4引数の要素数
		 * @param	pOutputProcessParameters			出力する波形と付随情報の送り先構造体
		 * @param	IsEnabled									有効無効フラグ
		 */
		STDMETHOD_(void, Process)(
			UINT32 InputProcessParameterCount,
			const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
			UINT32 OutputProcessParameterCount,
			XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
			BOOL IsEnabled
			);

		/**
		 * @struct	MyFadeParam
		 *
		 * @brief	EQパラメータ構造体
		 *
		 */
		struct MyFadeParam
		{
			float TargetVolume;	//!< 到達目標ボリューム
			float TargetTime;		//!< フェード時間[s]
		};

	private:
		static XAPO_REGISTRATION_PROPERTIES xapoRegProp_;   //!< XAPOのプロパティ

		WAVEFORMATEX inputFmt_; //!< 入力フォーマットの記憶場所
		WAVEFORMATEX outputFmt_;	//!< 出力フォーマットの記憶場所

		float NowVolume = 1.0f;		//!< 今のボリューム
		float SlopeVolume = 0.0f;		//!< ボリューム変化量

		MyFadeParam FadeParams[EFFECT_PARAMS];  //!<パラメータエリア

		bool FadeFlag = false;  //!< GetParameters用ボリューム格納変数
	};
}