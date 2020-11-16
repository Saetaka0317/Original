#pragma once
#include<Windows.h>

class Window
{
public://�ÓI�����o�ϐ�
	   // �E�B���h�E�T�C�Y
	static const int window_width = 1280;	// ����
	static const int window_height = 720;	// �c��
	static const char windowClassName[];
public://�ÓI�����o�֐�

#pragma region �E�B���h�E�v���V�[�W������
	//CALLBACK��WINAPI�֘A�ɕK�v�Ȃ���
	//�߂�l�@�@���b�Z�[�W�ɑ΂��鏈�����s������
	//�������@���b�Z�[�W�����M���ꂽ�E�B���h�E�̃n���h��
	//�������@���M���ꂽ���b�Z�[�W
	//��O�����@���b�Z�[�W�Ɋւ���t�����
	//��l�����@���b�Z�[�W�Ɋւ���t�����
#pragma endregion
	//�E�B���h�E�v���V�[�W���쐬	
	static LRESULT WindowProcedure(HWND window_handle, UINT msg, WPARAM wparam, LPARAM lparam);

public://�����o�֐�

	//�E�B���h�E�̐���
	void CreateGameWindow();
	//�E�B���h�E�̃��T�C�Y
	void ReSize();
	//�E�B���h�E�̍폜
	void DeleteGameWindow();
	//�I�����ǂ���
	bool ProcessMessage();

private://�����o�ϐ�

	// �E�B���h�E�n���h��
	HWND hwnd = nullptr;	
#pragma region WNDCLASSEX����
	//cbSize�@       �@UINT     �@�\���̂̃T�C�Y
	//style�@�@        UINT�@�@�@ �E�B���h�E�X�^�C��
	//lpfWndProc�@   �@WNDPROC�@�@�E�B���h�E�v���V�[�W���̃A�h���X
	//cbClsExtra�@�@   Int�@�@�@�@�\��������(�\���̑��Ŋm�ۂ����)��{�I��0
	//cbWndExtra�@   �@Int�@�@�@�@�\��������(�E�B���h�E���Ŋm�ۂ����)��{�I��0
	//hInstance�@    �@HINSTANCE�@�C���X�^���X�n���h��
	//hicon�@        �@HICON�@    �A�v���̃V���[�g�J�b�g�ȂǂŎg�p�����A�C�R���̐ݒ�(�f�t�H���g��NULL)
	//hCursor�@      �@HCURSOR�@  �E�B���h�E��̃}�E�X�J�[�\���̎w��(�f�t�H���g��NULL)
	//hbrBackground�@�@HBRUSH�@   �E�B���h�E�̃N���C�A���g�̈�̔w�i�F
	//lpszMenuName�@   LPCTSTR�@  �E�B���h�E��i�ɐݒ肷�郁�j���[�̖��O(���j���[���Ȃ����NULL)
	//lpszClassName�@�@LPCTSTR�@  �o�^���Ɏg�p����E�B���h�E�N���X�̖��O
	//hIconSm�@        HICON�@    �^�C�g���o�[�Ŏg�p�����A�C�R���̎w��(�f�t�H���g��NULL)
#pragma endregion
	//�E�B���h�E�N���X
	WNDCLASSEX wndowClass{};

	//�E�B���h�E�T�C�Y
	RECT window_rect;
	//�N���C�A���g�̈�̃T�C�Y
	RECT client_rect;
};