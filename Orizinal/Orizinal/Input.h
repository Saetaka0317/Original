#pragma once

#include <Windows.h>
#include <wrl.h>

#define DIRECTINPUT_VERSION     0x0800          // DirectInputのバージョン指定
#include <dinput.h>

/// <summary>
/// 入力
/// </summary>
class Input
{
public:
	struct MouseMove 
	{
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // メンバ関数
	//初期化
	bool Initialize(HINSTANCE hInstance, HWND hwnd);

	//毎フレーム処理
	void Update();

	//今ボタンが押されているか
	bool PushKey(BYTE keyNumber);

	//ボタンを押した瞬間だけを取得する
	bool TriggerKey(BYTE keyNumber);

	//マウスの左ボタンをクリックしているか
	bool PushMouseLeft();

	//マウス中央のボタンを押しているか
	bool PushMouseMiddle();

	//マウスの左ボタンを押した瞬間だけを取得する
	bool TriggerMouseLeft();

	//マウスの中央ボタンを押した瞬間だけを取得する
	bool TriggerMouseMiddle();

	//マウスの移動量を取得
	MouseMove GetMouseMove();

private: // メンバ変数
#pragma region インプット8の説明
	//大本となるインターフェイス
	//インプットデバイス8を作成するための関数
	//DirectXを使用するPCがどこまで機能を使いこなせるかをチェックする関数
	//接続されているゲームパッドを検索などができる
#pragma endregion
	ComPtr<IDirectInput8> dinput;
#pragma region インプットデバイス8の説明
	//デバイスの入力を管理するインターフェース
	//入力デバイスの取得関数を持っている
	//ただし作成にはIDirectInput8が必要
#pragma endregion
	ComPtr<IDirectInputDevice8> devKeyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};
	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};
};

