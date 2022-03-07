#include "../../../Public/Core/Render/RenderBase.h"
#include "../../../Public/Core/Render/RenderState.h"
#include "../../../Public/Core/Helper/Log.h"
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"



FRenderBase::FRenderBase()
{

}

FRenderBase::~FRenderBase()
{

}

void FRenderBase::ClearSwapChainRenderTargetView(FVector4D color /*= FVector4D(0.f, 0.f, 0.f, 1.0f)*/)
{
	static float colorArry[4] = { color.X,  color.Y,  color.Z,  color.W };
	GetD3DInterface().D3D11ImmediateContext->ClearRenderTargetView(GetD3DInterface().m_pRenderTargetView.Get(), colorArry);
}

void FRenderBase::ClearSwapChainDepthStencilView(UINT ClearFlags /*= D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL*/, float Depth /*= 1.0f*/, unsigned char Stencil /*= 0*/)
{
	GetD3DInterface().D3D11ImmediateContext->ClearDepthStencilView(GetD3DInterface().m_pDepthStencilView.Get(), ClearFlags, Depth, Stencil);
}

void FRenderBase::SetViewPort(D3D11_VIEWPORT view, UINT NumViewports/*=1*/)
{
	GetD3DInterface().D3D11ImmediateContext->RSSetViewports(NumViewports, &view);
}

void FRenderBase::SetViewPort(float TopLeftX, float TopLeftY, float Width, float Height, float MidDepth /*= 0.0f*/, float MaxDepth /*= 1.0f*/, UINT NumViewports /*= 1*/)
{
	D3D11_VIEWPORT _ViewPort;
	_ViewPort.TopLeftX = TopLeftX;
	_ViewPort.TopLeftY = TopLeftY;
	_ViewPort.Width = Width;
	_ViewPort.Height = Height;
	_ViewPort.MinDepth = MidDepth;
	_ViewPort.MaxDepth = MaxDepth;
	GetD3DInterface().D3D11ImmediateContext->RSSetViewports(1, &_ViewPort);
}

void FRenderBase::CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
{
	HR(GetD3DInterface().D3D11Device->CreateBuffer(pDesc, pInitialData, ppBuffer));
}

void FRenderBase::SetRenderTargetsAndDepthStencilView(ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT NumViews /*= 1*/)
{
	GetD3DInterface().D3D11ImmediateContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

void FRenderBase::SetRenderTargets(ID3D11RenderTargetView* const* ppRenderTargetViews)
{
	GetD3DInterface().D3D11ImmediateContext->OMSetRenderTargets(1, ppRenderTargetViews, nullptr);
}

void FRenderBase::SetSwapChainRenderTargetAndDepthStencilView()
{
	GetD3DInterface().D3D11ImmediateContext->OMSetRenderTargets(1, GetD3DInterface().m_pRenderTargetView.GetAddressOf(), GetD3DInterface().m_pDepthStencilView.Get());
}

void FRenderBase::SetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{
	GetD3DInterface().D3D11ImmediateContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

void FRenderBase::SetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	GetD3DInterface().D3D11ImmediateContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

void FRenderBase::SetInputLayout(ID3D11InputLayout* pInputLayout)
{
	GetD3DInterface().D3D11ImmediateContext->IASetInputLayout(pInputLayout);
}

void FRenderBase::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	GetD3DInterface().D3D11ImmediateContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

void FRenderBase::SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef/*=0*/)
{
	GetD3DInterface().D3D11ImmediateContext->OMSetDepthStencilState(pDepthStencilState, StencilRef);
}

void FRenderBase::SetRasterizerState(ID3D11RasterizerState* pRasterizerState)
{
	GetD3DInterface().D3D11ImmediateContext->RSSetState(pRasterizerState);
}

void FRenderBase::SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY Topology /*= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*/)
{
	GetD3DInterface().D3D11ImmediateContext->IASetPrimitiveTopology(Topology);
}

void FRenderBase::ExecutePresent()
{
	HR(GetD3DInterface().D3D11SwapChain->Present(1, 0));
}

void FRenderBase::SetRenderState()
{
	FRenderState::Init(GetD3DInterface().D3D11Device.Get());
	Log_Info("FenderState Init");
	GetD3DInterface().D3D11ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetD3DInterface().D3D11ImmediateContext->OMSetDepthStencilState(nullptr, 0);
}

FRenderBase::D3D11Struct& FRenderBase::GetD3DInterface()
{
	return D3DStruct;
}

ID3D11RenderTargetView* FRenderBase::GetSwapChainRTV()
{
	return  GetD3DInterface().m_pRenderTargetView.Get();
}

ID3D11RenderTargetView** FRenderBase::GetSwapChainRTVAddress()
{
	return  GetD3DInterface().m_pRenderTargetView.GetAddressOf();
}