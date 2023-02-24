#pragma once
#pragma once

//			Effekseer使用クラス
// 使用時はプロパティの「C/C++」→「全般」→「追加のインクルードディレクトリ」と
// 「リンカー」→「全般」→「追加のライブラリディレクトリ」に
// 「Effekseerに必要なもの」を追加してください。
// CEffekseerクラスはstaticなので使用する時だけプロジェクトに追加してください。
// また、デバッグ版は警告が出るので「リンカー」→「コマンドライン」→「追加のオプション」に
// 「/ignore:4099 」と記述してください。

//			使用時の注意事項
// CEffekseerCtrl::Draw()前に何かしらDxLibの2D描画関数(半透明処理)が呼ばれてないと
// 描画されない可能性があります。
// また、3Dの透明or半透明の物体の後ろにエフェクトを出してしまうとおかしな見た目になることもあります。

// 現在はVisualStudio2017と2019にのみ対応しています。

#include <DxLib.h>
#include <Effekseer.h>
#include <EffekseerRendererDX9.h>
#include <EffekseerRendererDX11.h>
#include <vector>


class CEffekseer;


//		エフェクト管理クラス
//		こっち使って
class CEffekseerCtrl {
public:
	static const int EFFECT_NUM = 120;
protected:
	static std::vector<::Effekseer::Effect*>	m_effect;		// エフェクトクラス
	static CEffekseer m_eff[EFFECT_NUM];			// エフェクト単品管理クラス
	static ::Effekseer::Manager* m_manager;		// エフェクトマネージャ
	static ::EffekseerRendererDX9::Renderer* m_renderer9;	// レンダラー(DirectX9用)
	static ::EffekseerRendererDX11::Renderer* m_renderer11;	// レンダラー(DirectX11用)
	static int									m_allNum;		// 使用エフェクト総数
	static int									m_useID;		// 次に使用予定のID
	static int									m_dxVersion;	// DirectXのバージョン
public:
	//	コンストラクタ・デストラクタ
	CEffekseerCtrl();
	~CEffekseerCtrl();

	static std::vector<::Effekseer::Effect*> GetEffect() { return m_effect; }

	//	初期化
	//	effNum		:	エフェクト総数
	//	particleNum	:	パーティクル総数
	static void Init(int effNum, int particleNum);
	//	終了処理
	static void Exit(void);

	//	エフェクトデータロード
	//	name	:	エフェクト名(データ名・パス含め日本語、全角文字は未対応)
	//	@return	:	成功=エフェクトID,失敗=-1
	static int	LoadData(const char* name);
	//	投影行列設定
	//	pers		:	カメラのパース
	//	aspect		:	アスペクト比
	//	nearLength	:	カメラのニアー
	//	farLength	:	カメラのファー
	//	@memo		:	設定が変わるたびに呼んでください
	static void SetProjectionMtx(float pers, float aspect, float nearLength, float farLength);
	//	投影行列自動設定
	//	@memo	:	DxLibの関数で自動設定(なるべく使わない)
	//				画面分割などには対応不可
	static void SetAutoProjectionMtx(void);
	//	カメラ行列設定
	//	eyePos		:	カメラの視点	
	//	focusPos	:	カメラの注視点
	//	up			:	カメラの上方向
	//	@memo		:	設定が変わるたびに呼んでください
	static void SetCameraMtx(VECTOR eyePos, VECTOR focusPos, VECTOR up);
	//	カメラ行列自動設定
	//	@memo	:	DxLibの関数で自動設定(なるべく使わない)
	static void SetAutoCameraMtx();
	//	カメラ行列設定
	//	eyePos		:	カメラの視点	
	//	rot			:	カメラの角度
	//	up			:	カメラの上方向
	//	@memo		:	設定が変わるたびに呼んでください
	static void SetCameraRotMtx(VECTOR eyePos, VECTOR rot, VECTOR up);
	//	更新処理
	static void Update(void);
	//	更新処理
	//	@memo	: カメラ設定も自動(なるべく使わない)
	//			  ProjectionMtx,CameraMtxの設定も不要
	static void UpdateAutoCamera(void);
	//	描画処理
	static void Draw(void);


	//	エフェクト呼び出し
	//	ID		:	エフェクトID
	//	pos		:	初期位置
	//	isLoop	:	エフェクトがループするか
	static int Request(int ID, VECTOR pos, bool isLoop);
	static int Request2(int ID, VECTOR pos, int start_frame, bool isLoop);
	static int Request3(int ID, VECTOR pos, int start_frame, int end_frame, bool isLoop);
	//	指定エフェクト停止
	static void Stop(int hndl);
	//	全エフェクト停止
	static void StopAll(void);

	//	座標変更
	static void SetPosition(int hndl, VECTOR pos);
	//	拡大縮小率変更
	static void SetScale(int hndl, VECTOR scale);
	//	回転角度変更
	static void SetRot(int hndl, VECTOR rot);
	//	エフェクトアクティブ判定
	static bool IsActive(int hndl);
	//	再生速度
	static void SetSpeed(int handle, float speed);

private:
	//	デバイスロスト時のコールバック関数
	static void DeviceLostFunction(void* data);
	//	デバイス復帰時のコールバック関数
	static void DeviceRestoreFunction(void* data);
};



//		エフェクト単品管理クラス
//		こちらは基本使わない
class CEffekseer {
protected:
	VECTOR				m_pos;			// エフェクトの再生位置
	int					m_hndl;			// エフェクトハンドル
	int					m_ID;			// 再生中のエフェクトID
	int					m_endFrame;		// 最終フレーム
	bool				m_isLoop;		// ループフラグ

	VECTOR				m_scale;
	float				m_speed;

public:
	//	コンストラクタ・デストラクタ
	CEffekseer();
	~CEffekseer();

	void Reset(void);

	//	座標設定
	inline void	SetTrans(VECTOR pos) { m_pos = pos; }
	//	座標取得
	inline VECTOR	GetTrans(void) { return m_pos; }
	//	エフェクトハンドル設定
	inline void	SetHndl(int hndl) { m_hndl = hndl; }
	//	エフェクトハンドル取得
	inline int	GetHndl(void) { return m_hndl; }
	//	エフェクトID設定
	inline void	SetID(int ID) { m_ID = ID; }
	//	エフェクトID取得
	inline int	GetID(void) { return m_ID; }
	//	ループ情報設定
	inline void	SetLoop(bool isLoop) { m_isLoop = isLoop; }
	//	ループ情報取得
	inline bool	IsLoop(void) { return m_isLoop; }
	//	エンドフレーム
	inline void SetEndFrame(int frame) { m_endFrame = frame; }


	inline VECTOR	GetScale(void) { return m_scale; }
	inline float	GetSpeed() { return m_speed; }
	void SetScale(VECTOR scale) { m_scale = scale; }
	void SetSpeed(float speed) { m_speed = speed; }
};