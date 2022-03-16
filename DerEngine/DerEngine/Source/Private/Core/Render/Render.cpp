
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Public/Core/Helper/Log.h"
#include <d3dcompiler.h>
#include "../../../Public/Core/FunctionLibrary/KismetStringLibrary.h"
#include "../../../Public/Core/Render/RenderState.h"
#include "../../../Public/Core/Render/Render.h"
#include "../../../Public/Core/Render/RHI.h"
#include "../../../Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../Core/Object/Object/GameInstance/GameInstance.h"

FRender::FRender()
{
	m_Enable4xMsaa = false;
	m_4xMsaaQuality = false;
	F_RenderStart = false;
	Log_Info("RenderThread New")
}

FRender::~FRender()
{
	Log_Info("RenderThread delete")
}

void FRender::Init()
{
	Log_Info("RenderThread Init");
	InitDirect3D();
	Log_Info("init RenderStateClass");
	SetRenderState();

}

void FRender::BeginPaly()
{
	Log_Info("Set DrawMode D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST");
	FRHI::RHI_SetPrimitiveTopology();
	Log_Info("RenderThread BeginPaly")
}

void FRender::OnResize()
{
	if (FRHI::RHI_GetRenderStart())
	{
		assert(GetD3DInterface().D3D11ImmediateContext);
		assert(GetD3DInterface().D3D11Device);
		assert(GetD3DInterface().D3D11SwapChain);


		// �ͷ���Ⱦ��������õ��������Դ
		GetD3DInterface().m_pRenderTargetView.Reset();
		GetD3DInterface().m_pDepthStencilView.Reset();

		// ���轻�����������´�����ȾĿ����ͼ
		ComPtr<ID3D11Texture2D> backBuffer;
		HR(GetD3DInterface().D3D11SwapChain->ResizeBuffers(1, UKismetSystemLibrary::GetWinSize().X,UKismetSystemLibrary::GetWinSize().Y, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		HR(GetD3DInterface().D3D11SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf())));
		HR(GetD3DInterface().D3D11Device->CreateRenderTargetView(backBuffer.Get(), nullptr, GetD3DInterface().m_pRenderTargetView.GetAddressOf()));
		

		backBuffer.Reset();


		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width = UKismetSystemLibrary::GetWinSize().X;
		depthStencilDesc.Height = UKismetSystemLibrary::GetWinSize().Y;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		// Ҫʹ�� 4X MSAA? --��Ҫ������������MASS����
		if (m_Enable4xMsaa)
		{
			depthStencilDesc.SampleDesc.Count = 4;
			depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
		}



		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		ComPtr<ID3D11Texture2D>m_pDepthStencilBuffer;
		// ������Ȼ������Լ����ģ����ͼ
		HR(GetD3DInterface().D3D11Device->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf()));
		HR(GetD3DInterface().D3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, GetD3DInterface().m_pDepthStencilView.GetAddressOf()));


		// ����ȾĿ����ͼ�����/ģ�建������ϵ�����
		FRHI::RHI_SetRenderTargetsAndDepthStencilView(GetD3DInterface().m_pRenderTargetView.GetAddressOf(), GetD3DInterface().m_pDepthStencilView.Get());

	}
}

void FRender::UpdateScene(float DeltaSeconds)
{

}

void FRender::Draw(float DeltaSeconds)
{

}

void FRender::Destroy()
{
	Log_Info("RenderThread Destroy");
}

void FRender::InitDirect3D()
{
	ComPtr<ID3D11Device1>m_pd3dDevice1;
	ComPtr<ID3D11DeviceContext1> m_pd3dImmediateContext1;
	ComPtr<IDXGISwapChain1> m_pSwapChain1;

	HRESULT hr = S_OK;
	// ����D3D�豸 �� D3D�豸������
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif
	// ������������
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// ���Եȼ�����
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	D3D_FEATURE_LEVEL featureLevel;
	D3D_DRIVER_TYPE d3dDriverType;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		d3dDriverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, GetD3DInterface().D3D11Device.GetAddressOf(), &featureLevel, GetD3DInterface().D3D11ImmediateContext.GetAddressOf());

		if (hr == E_INVALIDARG)
		{
			// Direct3D 11.0 ��API������D3D_FEATURE_LEVEL_11_1������������Ҫ�������Եȼ�11.0�Լ����µİ汾
			hr = D3D11CreateDevice(nullptr, d3dDriverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, GetD3DInterface().D3D11Device.GetAddressOf(), &featureLevel, GetD3DInterface().D3D11ImmediateContext.GetAddressOf());
		}

		if (SUCCEEDED(hr))
		{
			break;
		}

	}
	if (FAILED(hr))
	{
		UKismetSystemLibrary::MessageBox_Win(std::wstring(L"DirctX �豸��ʼ��ʧ�� �����Կ�����DirectX ����֧��"));
	}
	HR(hr);


	// ����Ƿ�֧�����Եȼ�11.0��11.1
	if (featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1)
	{
		UKismetSystemLibrary::MessageBox_Win(std::wstring(L"Direct3D Feature Level 11 unsupported"));
	}

	GetD3DInterface().D3D11Device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	assert(m_4xMsaaQuality > 0);

	ComPtr<IDXGIDevice> dxgiDevice = nullptr;
	ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
	ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;	// D3D11.0(����DXGI1.1)�Ľӿ���
	ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;	// D3D11.1(����DXGI1.2)���еĽӿ���

	// Ϊ����ȷ���� DXGI������������������Ҫ��ȡ���� D3D�豸 �� DXGI�������������������
	// "IDXGIFactory::CreateSwapChain: This function is being called with a device from a different IDXGIFactory."
	HR(GetD3DInterface().D3D11Device.As(&dxgiDevice));
	HR(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf())));
	DXGI_ADAPTER_DESC adapterDesc;
	dxgiAdapter->GetDesc(&adapterDesc);
	std::wstring des_ws(adapterDesc.Description);
//	Log_Info(FString("Running on DXGI Adapte " + des_ws));
	// �鿴�ö����Ƿ����IDXGIFactory2�ӿ�
	hr = dxgiFactory1.As(&dxgiFactory2);
	// �����������˵��֧��D3D11.1
	if (dxgiFactory2 != nullptr)
	{
 		HR(GetD3DInterface().D3D11Device.As(&m_pd3dDevice1));
 		HR(GetD3DInterface().D3D11ImmediateContext.As(&m_pd3dImmediateContext1));
		// �����ֽṹ����������������
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = UKismetSystemLibrary::GetWinSize().X;
		sd.Height = UKismetSystemLibrary::GetWinSize().Y;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		// �Ƿ���4�����ز�����
		if (m_Enable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd;
		fd.RefreshRate.Numerator = 60;
		fd.RefreshRate.Denominator = 1;
		fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		fd.Windowed = TRUE;
		// Ϊ��ǰ���ڴ���������
		HR(dxgiFactory2->CreateSwapChainForHwnd(GetD3DInterface().D3D11Device.Get(), UKismetSystemLibrary::GetHWND(), &sd, &fd, nullptr, m_pSwapChain1.GetAddressOf()));
		HR(m_pSwapChain1.As(&GetD3DInterface().D3D11SwapChain));
	}
	else
	{
		// ���DXGI_SWAP_CHAIN_DESC��������������
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferDesc.Width = UKismetSystemLibrary::GetWinSize().X;
		sd.BufferDesc.Height =UKismetSystemLibrary::GetWinSize().Y;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		// �Ƿ���4�����ز�����
		if (m_Enable4xMsaa)
		{
			sd.SampleDesc.Count = 4;
			sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = UKismetSystemLibrary::GetHWND();
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;
		HR(dxgiFactory1->CreateSwapChain(GetD3DInterface().D3D11Device.Get(), &sd, GetD3DInterface().D3D11SwapChain.GetAddressOf()));
	}
	//D3D��ʼ�����
	F_RenderStart = true;
	// ���Խ�ֹalt+enterȫ��
	HR(dxgiFactory1->MakeWindowAssociation(UKismetSystemLibrary::GetHWND(), DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES));
	//����Ĭ���ӿ�
	UGamePlayStatics::GetGameInstance()->OnResize();
	Log_Info("Render.cpp-> SetDefluat ViewPortSize"+UKismetSystemLibrary::GetWinSize().ToString());
}

bool FRender::GetRenderStart()
{
	return F_RenderStart;
}
