#include"DirectXCommon.h"
#include <vector>
#include <cassert>

#pragma comment(lib, "d3d12.lib")//無駄な読み込むを減らすためにcppに書く
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;
void DirectXCommon::Initialize(Window* win)
{
	// nullptrチェック
	assert(win);

	this->win = win;

	// DXGIデバイス初期化
	if (!InitializeDXGIDevice()) {
		assert(0);
	}

	// コマンド関連初期化
	if (!InitializeCommand()) {
		assert(0);
	}

	// スワップチェーンの生成
	if (!CreateSwapChain()) {
		assert(0);
	}

	// レンダーターゲット生成
	if (!CreateFinalRenderTargets()) {
		assert(0);
	}

	// 深度バッファ生成
	if (!CreateDepthBuffer()) {
		assert(0);
	}

	// フェンス生成
	if (!CreateFence()) {
		assert(0);
	}
}

bool DirectXCommon::InitializeDXGIDevice()
{
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;
	//デバッグレイヤーをオンに	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	// 対応レベルの配列 //一部のグラフィックボードによっては使えなくなるのでフューチャーレベルを1つずつ落として合致するものを探す
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));//ここでDXGIFactoryオブジェクトがdxgiFactoryに入る
	if (FAILED(result)) {
		assert(0);
		return false;//失敗していたらfalseを返す
	}

	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (int i = 0;dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;i++)
	{
		adapters.push_back(tmpAdapter);	// 動的配列に追加する　//ここで利用可能なアダプターが代入された
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);	// アダプターの情報を取得

		// ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) 
		{
			continue;
		}

		std::wstring strDesc = adesc.Description;	// アダプター名

		// Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];	// 採用
			break;
		}
	}

	D3D_FEATURE_LEVEL featureLevel;
#pragma region クリエイトデバイス説明
	//第一引数はアダプターを表すポインター、グラフィックスボードが一種類だった場合第一引数がnullptrでもいい今回は複数に対応している
	//第二引数は列挙型でD3D_FEATURE_LEVEL、選択したものが対応していなかったら1つずつ下げて合致するものを探すか別のドライバーを探す
	//第三引数はREFIIDというもので受け取りたいオブジェクトの型を識別するためのID、本来なら第四引数の型から計算することになる
	//第四引数はポインターのアドレス(デバイスの実体)
	//今回は第三と第四の計算をしてくれるマクロ、IID_PPV_ARGSを使う
	//IID_PPV_ARGSは引数にポインターのアドレスを渡すとREFIIDとポインターのアドレスに解釈される
#pragma endregion
	result = S_FALSE;
	for (int i = 0; i < _countof(levels); i++)
	{
		// デバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
		if (SUCCEEDED(result))
		{
			// デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

bool DirectXCommon::InitializeCommand()
{
	HRESULT result = S_FALSE;

	// コマンドアロケータを生成
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// コマンドリストを生成
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	return true;
}

bool DirectXCommon::CreateSwapChain()
{
	HRESULT result = S_FALSE;

	// 各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = Window::window_width;
	swapchainDesc.Height = Window::window_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 色情報の書式を一般的なものに
	swapchainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;	// バックバッファとして使えるように
	swapchainDesc.BufferCount = 2;	// バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // フリップ後は速やかに破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // フルスクリーン切り替えを許可
	ComPtr<IDXGISwapChain1> swapchain1;
	HWND hwnd = win->GetHwnd();
	result = dxgiFactory->CreateSwapChainForHwnd(commandQueue.Get(),win->GetHwnd(),&swapchainDesc,nullptr,nullptr,&swapchain1);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	swapchain1.As(&swapchain);

	return true;
}

bool DirectXCommon::CreateFinalRenderTargets()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result)) 
	{
		assert(0);
		return result;
	}

	// 各種設定をしてディスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;	// レンダーターゲットビューなのでRTV
	heapDesc.NumDescriptors = swcDesc.BufferCount;//裏表の2つが入っている
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	if (FAILED(result)) 
	{
		assert(0);
		return result;
	}

	// 裏表の２つ分について
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++)
	{
		// スワップチェーンからバッファを取得
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		if (FAILED(result)) 
		{
			assert(0);
			return result;
		}

		// ディスクリプタヒープのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// レンダーターゲットビューの生成
		device->CreateRenderTargetView(backBuffers[i].Get(),nullptr,handle);
	}

	return true;
}

bool DirectXCommon::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	// リソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT,Window::window_width,Window::window_height,1, 0,1, 0,D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	// リソースの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));
	if (FAILED(result))
	{
		assert(0);
		return result;
	}

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビューは深度値とステンシル値を保存するためのバッファ
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(result)) 
	{
		assert(0);
		return result;
	}

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView(depthBuffer.Get(),&dsvDesc,dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return true;
}

bool DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;

	// フェンスの生成
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}
