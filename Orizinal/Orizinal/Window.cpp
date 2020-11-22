#include "Window.h"

const wchar_t Window::windowClassName[] = L"DirectXGame";

LRESULT Window::WindowProcedure(HWND window_handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ���b�Z�[�W�ŕ���
	switch (msg)
	{
	case WM_DESTROY: // �E�B���h�E���j�����ꂽ
		PostQuitMessage(0); // OS�ɑ΂��āA�A�v���̏I����`����
		return 0;
	}
	return DefWindowProc(window_handle, msg, wparam, lparam); // �W���̏������s��
}

void Window::CreateGameWindow()
{
	// �E�B���h�E�N���X�̐ݒ�
	
	wndowClass.cbSize = sizeof(WNDCLASSEX);
	wndowClass.lpfnWndProc = (WNDPROC)WindowProcedure; // �E�B���h�E�v���V�[�W��
	wndowClass.lpszClassName = windowClassName; // �E�B���h�E�N���X��
	wndowClass.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	wndowClass.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	RegisterClassEx(&wndowClass); // �E�B���h�E�N���X��OS�ɓo�^

								// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

														// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd = CreateWindow(wndowClass.lpszClassName, // �N���X��
		windowClassName,			// �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,	// �^�C�g���o�[�Ƌ��E��������E�B���h�E
		CW_USEDEFAULT,			// �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,			// �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,	// �E�B���h�E����
		wrc.bottom - wrc.top,	// �E�B���h�E�c��
		nullptr,				// �e�E�B���h�E�n���h��
		nullptr,				// ���j���[�n���h��
		wndowClass.hInstance,			// �Ăяo���A�v���P�[�V�����n���h��
		nullptr);				// �I�v�V����

	ReSize();
	// �E�B���h�E�\��
	ShowWindow(hwnd, SW_SHOW);
}

void Window::ReSize()
{
	//�E�B���h�E�T�C�Y�擾
	GetWindowRect(hwnd, &window_rect);

	//�N���C�A���g�̈�̃T�C�Y�擾
	GetClientRect(hwnd, &client_rect);

	//�t���[���T�C�Y�Z�o
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	//���T�C�Y�p�T�C�Y�̎Z�o
	int resize_x = frame_size_x + window_width;
	int resize_y = frame_size_y + window_height;

	//�\���ʒu�̍X�V
	SetWindowPos(hwnd, NULL, CW_USEDEFAULT, CW_USEDEFAULT, frame_size_x + window_width, frame_size_y + window_height, SWP_NOMOVE);
}

void Window::DeleteGameWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(wndowClass.lpszClassName, wndowClass.hInstance);
}

bool Window::ProcessMessage()
{
	MSG msg{};	// ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}

	return false;
}
