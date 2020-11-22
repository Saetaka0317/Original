#pragma once
#include<Windows.h>

class Window
{
public://静的メンバ変数
	   // ウィンドウサイズ
	static const int window_width = 1280;	// 横幅
	static const int window_height = 720;	// 縦幅
	static const char windowClassName[];
public://静的メンバ関数

#pragma region ウィンドウプロシージャ説明
	//CALLBACKはWINAPI関連に必要なもの
	//戻り値　　メッセージに対する処理を行ったか
	//第一引数　メッセージが送信されたウィンドウのハンドル
	//第二引数　送信されたメッセージ
	//第三引数　メッセージに関する付加情報
	//第四引数　メッセージに関する付加情報
#pragma endregion
	//ウィンドウプロシージャ作成	
	static LRESULT WindowProcedure(HWND window_handle, UINT msg, WPARAM wparam, LPARAM lparam);

public://メンバ関数

	//ウィンドウの生成
	void CreateGameWindow();
	//ウィンドウのリサイズ
	void ReSize();
	//ウィンドウの削除
	void DeleteGameWindow();
	//終了かどうか
	bool ProcessMessage();

private://メンバ変数

	// ウィンドウハンドル
	HWND hwnd = nullptr;	
#pragma region WNDCLASSEX説明
	//cbSize　       　UINT     　構造体のサイズ
	//style　　        UINT　　　 ウィンドウスタイル
	//lpfWndProc　   　WNDPROC　　ウィンドウプロシージャのアドレス
	//cbClsExtra　　   Int　　　　予備メモリ(構造体側で確保される)基本的に0
	//cbWndExtra　   　Int　　　　予備メモリ(ウィンドウ側で確保される)基本的に0
	//hInstance　    　HINSTANCE　インスタンスハンドル
	//hicon　        　HICON　    アプリのショートカットなどで使用されるアイコンの設定(デフォルトはNULL)
	//hCursor　      　HCURSOR　  ウィンドウ上のマウスカーソルの指定(デフォルトはNULL)
	//hbrBackground　　HBRUSH　   ウィンドウのクライアント領域の背景色
	//lpszMenuName　   LPCTSTR　  ウィンドウ上段に設定するメニューの名前(メニューがなければNULL)
	//lpszClassName　　LPCTSTR　  登録時に使用するウィンドウクラスの名前
	//hIconSm　        HICON　    タイトルバーで使用されるアイコンの指定(デフォルトはNULL)
#pragma endregion
	//ウィンドウクラス
	WNDCLASSEX wndowClass{};

	//ウィンドウサイズ
	RECT window_rect;
	//クライアント領域のサイズ
	RECT client_rect;
};