#include "Window.h"
#include"DirectXCommon.h"
#include"GameScene.h"

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �ėp�@�\
	Window* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	GameScene* gameScene = nullptr;

	// �Q�[���E�B���h�E�̍쐬
	win = new Window();
	win->CreateGameWindow();

	//DirectX����������
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(win);
	// ���͂̏�����
	input = new Input();
	if (!input->Initialize(win->GetInstance(), win->GetHwnd()))
	{
		assert(0);
		return 1;
	}
	// �X�v���C�g�ÓI������
	if (!Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height)) 
	{
		assert(0);
		return 1;
	}
	// 3D�I�u�W�F�N�g�ÓI������
	Object3d::StaticInitialize(dxCommon->GetDevice());

	// �Q�[���V�[���̏�����
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon, input, audio);

	// ���C�����[�v
	while (true)
	{
		// ���b�Z�[�W����
		if (win->ProcessMessage()) { break; }
		// ���͊֘A�̖��t���[������
		input->Update();
		// �Q�[���V�[���̖��t���[������
		gameScene->Update();

		// �`��J�n
		dxCommon->PreDraw();
		// �Q�[���V�[���̕`��
		gameScene->Draw();
		// �`��I��
		dxCommon->PostDraw();
		
	}
	// �e����
	safe_delete(gameScene);
	safe_delete(input);
	safe_delete(dxCommon);
	// �Q�[���E�B���h�E�̔j��
	win->DeleteGameWindow();
	//safe_delete(win);
	delete(win);

	return 0;
}