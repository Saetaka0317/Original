#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>//DXGI�𐧌䂷��̂ɕK�v
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>


#include"Window.h"

class DirectXCommon
{
#pragma region �G�C���A�X�Ƃ�
	//�Ώۂ���̂𕡐��̈قȂ�V���{���⎯�ʎq�œ����悤�ɎQ�Ƃł���悤�ɂ���d�g��
	//�C�e���[�^�݂����Ȋ����ł����Ȃ��̂��܂Ƃ߂ĊǗ�����@�\���ȂƎv����
#pragma endregion
private://�G�C���A�X
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://�����o�֐�
	void Initialize(Window* win);
	/// <summary>
	/// �`��O����
	/// </summary>
	void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	void PostDraw();

	/// <summary>
	/// �����_�[�^�[�Q�b�g�̃N���A
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// �[�x�o�b�t�@�̃N���A
	/// </summary>
	void ClearDepthBuffer();
	//�f�o�C�X�̎擾
	ID3D12Device* GetDevice() { return device.Get(); }
	//�`��R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

public://�����o�ϐ�
	//�E�B���h�E�Ǘ�
	Window * win;

	//DirectX3D�Ǘ�
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> device;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandAllocator> commandAllocator;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
private://�����o�֐�
#pragma region DXGI�Ƃ�
	//DirectX Graphics Infrastructure�@�̗�
	//Direct3D API�����h���C�o�[���ɋ߂��A�f�B�X�v���C�ւ̏o�͂ɒ��ڊ֌W����@�\�𐧌䂷�邽�߂�API�Z�b�g
#pragma endregion
	//DXGI�f�o�C�X�̏�����
	bool InitializeDXGIDevice();
#pragma region �R�}���h�֘A
	//�R�}���h���X�g�AGPU�ɑ΂��閽�߂��܂Ƃ߂邽�߂̃I�u�W�F�N�g
	//DirectX11�ł�DeviceConext�ɂ��̂Ǒ����Ă�����DirectX12�ł͂ЂƂ܂Ƃ߂ɂ��đ���悤�ɕς����
	//�R�}���h�A���P�[�^�[�̓R�}���h���X�g���瑗���Ă����f�[�^��ۑ�����ꏊ
	//�R�}���h�L���[�̓R�}���h���X�g���g���ăR�}���h�A���P�[�^�[�ɂ��߂����߂�GPU�Ɏ��s���Ă������߂̕�
	//�R�}���h�L���[�̍Ō��Close���߂��Ȃ���������s�ł����ԂɂȂ�Ȃ�
#pragma endregion
	//�R�}���h�֘A�̏�����
	bool InitializeCommand();
#pragma region �X���b�v�`�F�[��
	//�X���b�v�`�F�[���̓_�u���o�b�t�@�����O�����邽�߂̎d�g��
	//�_�u���o�b�t�@�����O�͕`��r������ʂɉf��Ȃ��悤�ɂ��邽�߂ɉ�ʂ𗠂ƕ\��2�p�ӂ��ė��ŕ`�悪����������\�Ɠ���ւ���d�g��
	//��ʂƂ��ă��������(�o�b�t�@)����������_�[�^�[�Q�b�g�ƌĂ�
#pragma endregion
	//�X���b�v�`�F�[���̐���
	bool CreateSwapChain();
#pragma region �����_�[�^�[�Q�b�g
	//�o�b�t�@�͂��̂܂܂��Ɠ���ւ��邱�Ƃ͂ł��Ă����g�����������邱�Ƃ͂ł��Ȃ��̂ł����Ń����_�[�^�[�Q�b�g�r���[�Ƃ����r���[���K�v
#pragma endregion
#pragma region �o�b�t�@�ƃr���[
	//�o�b�t�@�[�̓������̉�ACPU��GPU�Ƃ̂��܂��܂Ȃ����Ɏg�p�����(���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�A�萔�o�b�t�@�A�e�N�X�`���o�b�t�@�A�[�x�o�b�t�@�Ȃ�)
	//�r���[�͍쐬�����o�b�t�@�[�̎g�������`��������(�萔�o�b�t�@�A�e�N�X�`���o�b�t�@�Ȃǂł͕K�{)
#pragma endregion
#pragma region �f�B�X�N���v�^
	//GPU���\�[�X(�������̉�)�̗p�r��g�p�@�ɂ��Đ������Ă���f�[�^
	//�����̓r���[�Ǝ��Ă��āA�f�B�X�N���v�^�͊e��r���[�ɉ����ăT���v���[���ꊇ��ɂ����悤�ȍL���T�O
	//ShaderResourceView(SRV)�@�e�N�X�`���o�b�t�@�[�ɂ��Ă̐���
	//ConstantBufferView(CBV)�@�萔�o�b�t�@�[�ɂ��Ă̐���
	//UnorderedAccessView(UAV)�@�R���s���[�g�V�F�[�_�[�Ŏg�p����o�b�t�@�[�ɂ��Ă̐���
	//Sampler�@�T���v���[�ɂ��Ă̐���
	//RenderTargetView(RTV)�@�����_�[�^�[�Q�b�g�r���[�ɂ��Ă̐���
	//DepthStencilView(DSV)�@�[�x�X�e���V���r���[�ɂ��Ă̐���
#pragma endregion
#pragma region �f�B�X�N���v�^�q�[�v
	//�f�B�X�N���v�^�̃f�[�^���܂Ƃ߂�ꏊ
#pragma endregion
	//�����_�[�^�[�Q�b�g�̐���
	bool CreateFinalRenderTargets();
#pragma region �[�x�o�b�t�@
	//�[�x�Ƃ́A�J��������̋����̂��Ƃł���A�[�x�l�Ƃ͐[�x�������l�̂���
	//�[�x�o�b�t�@�̓I�u�W�F�N�g�������_�����O���鎞�ɃI�u�W�F�N�g�̐[�x�l���L�^�������(���Ǝv��)
#pragma endregion
	//�[�x�o�b�t�@�̐���
	bool CreateDepthBuffer();
#pragma region �t�F���X
	//GPU�ւ̖��߂𑗂�Ƃ��ɔ񓯊���肪����(�^�C�~���O�̖��݂����Ȋ���)
	//�����h�����߂ɂ͉�ʂ�ς���Ƃ��͉�ʂ̕`�悪���ׂĊ�������܂ő҂Ƃ������������K�v
	//�����Ńt�F���X��GPU���̏������I��������Ď����Ă����
	//�t�F���X���g�����Ƃŏ��߂ē����������ł���悤�ɂȂ�
#pragma endregion
	//�t�F���X�̐���
	bool CreateFence();	
};