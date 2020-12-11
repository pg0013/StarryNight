/**
 * @file	AMG_Summer_Co_Production_2020\script\MyHighPassFilter.h.
 *
 * @brief	ハイパスフィルター
 * @detail	遮断周波数の上限がサンプリング周波数の1/2
 * @remark 参考文献 https://tsuchidasama.booth.pm/items/1417202
 */

#include"xapobase.h"
#pragma comment(lib,"xapobase.lib")
namespace sound
{
#define EFFECT_PARAMS 3

	/**
	 * @class	MyHighPassFilterXAPO :public CXAPOParametersBase
	 *
	 * @brief	ハイパスフィルターのXAPOFX
	 * @detail	遮断周波数の上限がサンプリング周波数の1/2
	 *
	 */
	class __declspec(uuid("{5D22ECC9-4095-4694-82A8-BB5237CB348A}"))
		MyHighPassFilterXAPO :public CXAPOParametersBase
	{
	public:
		MyHighPassFilterXAPO();
		~MyHighPassFilterXAPO() {};

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

	private:
		static XAPO_REGISTRATION_PROPERTIES xapoRegProp_;    //!< XAPOのプロパティ

		WAVEFORMATEX inputFmt_; //!< 入力フォーマットの記憶場所
		WAVEFORMATEX outputFmt_;	//!< 出力フォーマットの記憶場所

		float LastVolume = 0.0f;	//!< 最後の演算ボリューム

		float Freqency[EFFECT_PARAMS];  //!< カットオフ周波数
	};
}