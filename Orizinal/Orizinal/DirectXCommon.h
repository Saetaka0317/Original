#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>//DXGIを制御するのに必要
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>


#include"Window.h"

class DirectXCommon
{
#pragma region エイリアスとは
	//対象や実体を複数の異なるシンボルや識別子で同じように参照できるようにする仕組み
	//イテレータみたいな感じでいろんなものをまとめて管理する機能かなと思った
#pragma endregion
private://エイリアス
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	void Initialize(Window* win);
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// レンダーターゲットのクリア
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 深度バッファのクリア
	/// </summary>
	void ClearDepthBuffer();
	//デバイスの取得
	ID3D12Device* GetDevice() { return device.Get(); }
	//描画コマンドリストの取得
	ID3D12GraphicsCommandList* GetCommandList() { return commandList.Get(); }

public://メンバ変数
	//ウィンドウ管理
	Window * win;

	//DirectX3D管理
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
private://メンバ関数
#pragma region DXGIとは
	//DirectX Graphics Infrastructure　の略
	//Direct3D APIよりもドライバー側に近い、ディスプレイへの出力に直接関係する機能を制御するためのAPIセット
#pragma endregion
	//DXGIデバイスの初期化
	bool InitializeDXGIDevice();
#pragma region コマンド関連
	//コマンドリスト、GPUに対する命令をまとめるためのオブジェクト
	//DirectX11ではDeviceConextにそのつど送っていたがDirectX12ではひとまとめにして送るように変わった
	//コマンドアロケーターはコマンドリストから送られてきたデータを保存する場所
	//コマンドキューはコマンドリストを使ってコマンドアロケーターにためた命令をGPUに実行していくための物
	//コマンドキューの最後にClose命令がなかったら実行できる状態にならない
#pragma endregion
	//コマンド関連の初期化
	bool InitializeCommand();
#pragma region スワップチェーン
	//スワップチェーンはダブルバッファリングをするための仕組み
	//ダブルバッファリングは描画途中が画面に映らないようにするために画面を裏と表で2つ用意して裏で描画が完成したら表と入れ替える仕組み
	//画面としてメモリ空間(バッファ)これをレンダーターゲットと呼ぶ
#pragma endregion
	//スワップチェーンの生成
	bool CreateSwapChain();
#pragma region レンダーターゲット
	//バッファはそのままだと入れ替えることはできても中身を書き換えることはできないのでそこでレンダーターゲットビューというビューが必要
#pragma endregion
#pragma region バッファとビュー
	//バッファーはメモリの塊、CPUとGPUとのさまざまなやり取りに使用される(頂点バッファ、インデックスバッファ、定数バッファ、テクスチャバッファ、深度バッファなど)
	//ビューは作成したバッファーの使い方を定義したもの(定数バッファ、テクスチャバッファなどでは必須)
#pragma endregion
#pragma region ディスクリプタ
	//GPUリソース(メモリの塊)の用途や使用法について説明しているデータ
	//説明はビューと似ていて、ディスクリプタは各種ビューに加えてサンプラーを一括りにしたような広い概念
	//ShaderResourceView(SRV)　テクスチャバッファーについての説明
	//ConstantBufferView(CBV)　定数バッファーについての説明
	//UnorderedAccessView(UAV)　コンピュートシェーダーで使用するバッファーについての説明
	//Sampler　サンプラーについての説明
	//RenderTargetView(RTV)　レンダーターゲットビューについての説明
	//DepthStencilView(DSV)　深度ステンシルビューについての説明
#pragma endregion
#pragma region ディスクリプタヒープ
	//ディスクリプタのデータをまとめる場所
#pragma endregion
	//レンダーターゲットの生成
	bool CreateFinalRenderTargets();
#pragma region 深度バッファ
	//深度とは、カメラからの距離のことであり、深度値とは深度を示す値のこと
	//深度バッファはオブジェクトをレンダリングする時にオブジェクトの深度値を記録するもの(だと思う)
#pragma endregion
	//深度バッファの生成
	bool CreateDepthBuffer();
#pragma region フェンス
	//GPUへの命令を送るときに非同期問題が発生(タイミングの問題みたいな感じ)
	//それを防ぐためには画面を変えるときは画面の描画がすべて完了するまで待つといった処理が必要
	//そこでフェンスはGPU側の処理が終わったか監視してくれる
	//フェンスを使うことで初めて同期処理ができるようになる
#pragma endregion
	//フェンスの生成
	bool CreateFence();	
};