#include "Window.h"
#include"DirectXCommon.h"

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �ėp�@�\
	Window* win = nullptr;
	DirectXCommon* dxCommon = nullptr;

	// �Q�[���E�B���h�E�̍쐬
	win = new Window();
	win->CreateGameWindow();

	//DirectX����������
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);

	// ���C�����[�v
	while (true)
	{
		// ���b�Z�[�W����
		if (win->ProcessMessage()) { break; }

		
	}
	// �e����
	
	// �Q�[���E�B���h�E�̔j��
	win->DeleteGameWindow();
	//safe_delete(win);
	delete(win);

	return 0;
}