#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <map>
#include <string>
#include "../Vector/Vector4D.h"


class FRenderBase 
{
public:
	struct D3D11Struct
	{
		ComPtr<ID3D11Device> D3D11Device;                    // D3D11设备
		ComPtr<ID3D11DeviceContext> D3D11ImmediateContext;   // D3D11设备上下文
		ComPtr<IDXGISwapChain> D3D11SwapChain;                  // D3D11交换链
		ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;    
		ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	};
public:
	FRenderBase();
	virtual~FRenderBase();
public:
	void ClearSwapChainRenderTargetView(FVector4D color = FVector4D(0.f, 0.f, 0.f, 1.0f));
	void ClearSwapChainDepthStencilView(UINT ClearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float Depth = 1.0f, unsigned char Stencil = 0);
	void SetViewPort(D3D11_VIEWPORT view, UINT NumViewports=1);
	void SetViewPort(float TopLeftX, float TopLeftY, float Width, float Height, float MidDepth /*= 0.0f*/, float MaxDepth /*= 1.0f*/, UINT NumViewports = 1);
	void CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer);
	void SetRenderTargetsAndDepthStencilView(ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT NumViews = 1);
	void SetRenderTargets(ID3D11RenderTargetView* const* ppRenderTargetViews);
	void SetSwapChainRenderTargetAndDepthStencilView();
	void SetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
	void SetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
	void SetInputLayout(ID3D11InputLayout* pInputLayout);
	void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
	void SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef=0);
	void SetRasterizerState(ID3D11RasterizerState* pRasterizerState);
	void SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void ExecutePresent();
	void SetRenderState();
	D3D11Struct& GetD3DInterface();
	ID3D11RenderTargetView* GetSwapChainRTV();
	ID3D11RenderTargetView** GetSwapChainRTVAddress();

private:
	D3D11Struct D3DStruct;

};
