#include "Window.h"


// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �ėp�@�\
	Window* win = nullptr;
	

	// �Q�[���E�B���h�E�̍쐬
	win = new Window();
	win->CreateGameWindow();

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