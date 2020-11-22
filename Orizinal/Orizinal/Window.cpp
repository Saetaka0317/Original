#include "Window.h"

const wchar_t Window::windowClassName[] = L"DirectXGame";

LRESULT Window::WindowProcedure(HWND window_handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: // ウィンドウが破棄された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(window_handle, msg, wparam, lparam); // 標準の処理を行う
}

void Window::CreateGameWindow()
{
	// ウィンドウクラスの設定
	
	wndowClass.cbSize = sizeof(WNDCLASSEX);
	wndowClass.lpfnWndProc = (WNDPROC)WindowProcedure; // ウィンドウプロシージャ
	wndowClass.lpszClassName = windowClassName; // ウィンドウクラス名
	wndowClass.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	wndowClass.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	RegisterClassEx(&wndowClass); // ウィンドウクラスをOSに登録

								// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

														// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(wndowClass.lpszClassName, // クラス名
		windowClassName,			// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,			// 表示X座標（OSに任せる）
		CW_USEDEFAULT,			// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		wndowClass.hInstance,			// 呼び出しアプリケーションハンドル
		nullptr);				// オプション

	ReSize();
	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void Window::ReSize()
{
	//ウィンドウサイズ取得
	GetWindowRect(hwnd, &window_rect);

	//クライアント領域のサイズ取得
	GetClientRect(hwnd, &client_rect);

	//フレームサイズ算出
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	//リサイズ用サイズの算出
	int resize_x = frame_size_x + window_width;
	int resize_y = frame_size_y + window_height;

	//表示位置の更新
	SetWindowPos(hwnd, NULL, CW_USEDEFAULT, CW_USEDEFAULT, frame_size_x + window_width, frame_size_y + window_height, SWP_NOMOVE);
}

void Window::DeleteGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(wndowClass.lpszClassName, wndowClass.hInstance);
}

bool Window::ProcessMessage()
{
	MSG msg{};	// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}
