#include "Window.h"


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// 汎用機能
	Window* win = nullptr;
	

	// ゲームウィンドウの作成
	win = new Window();
	win->CreateGameWindow();

	// メインループ
	while (true)
	{
		// メッセージ処理
		if (win->ProcessMessage()) { break; }

		
	}
	// 各種解放
	
	// ゲームウィンドウの破棄
	win->DeleteGameWindow();
	//safe_delete(win);
	delete(win);

	return 0;
}