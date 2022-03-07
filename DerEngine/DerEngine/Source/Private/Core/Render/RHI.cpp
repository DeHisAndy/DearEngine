#include "../../../Public/Core/Render/RHI.h"
#include "../../../Public/Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../Public/Core/Engine/Game.h"
#include "../../../Public/Core/Render/Render.h"
#include "../../../Public/Core/Object/Object/GameInstance/GameInstance.h"
#include "../../../Public/ThirdParty/DearImgui/imgui.h"
#include "../../../Public/ThirdParty/DearImgui/imgui_impl_win32.h"
#include "../../../Public/ThirdParty/DearImgui/imgui_impl_dx11.h"
#include "../../../Public/Core/Render/ShaderCompilerWorker.h"
#include "../../../Public/Core/Engine/GameViewport.h"
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Public/Core/Vector/Matrix.h"
#include "../../../Public/ThirdParty/DirectXTex/DirectXTex/DirectXTex.h"
#include "../../../Public/ThirdParty/DirectXTex/WICTextureLoader/WICTextureLoader.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
 UGame* GetGameThread()
{
	return UGame::Get();
}

 FRender* GetRenderThread()
{
	return UGame::Get()->GetRenderThread();
}

 void FRHI::RHIDX11_ExecutePresent()
{
	UGame::Get()->GetRenderThread()->ExecutePresent();
}

 UShaderCompilerWorker* FRHI::RHIDX11_GetShaderCompilerWorker()
{
  return	UGame::Get()->GetGameInstance()->GetEffect();
}

 void FRHI::RHIDX11_ClearSwapChainRenderTargetView(FVector4D color/*=FVector4D(0.f,0.f,0.f,1.0f)*/)
{
	GetRenderThread()->ClearSwapChainRenderTargetView(color);
}

 void FRHI::RHIDX11_ClearSwapChainDepthStencilView(UINT ClearFlags/*=D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL*/, float Depth/*=1.0f*/, unsigned char Stencil/*=0*/)
{
	GetRenderThread()->ClearSwapChainDepthStencilView(ClearFlags ,Depth, Stencil);
}

 void FRHI::RHIDX11_ClearSwapChainRTVAndDSV(FVector4D color /*= FVector4D(0.f, 0.f, 0.f, 1.0f)*/, UINT ClearFlags /*= D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL*/, float Depth /*= 1.0f*/, unsigned char Stencil /*= 0*/)
 {
	 FRHI::RHIDX11_ClearSwapChainRenderTargetView(color);
	 FRHI::RHIDX11_ClearSwapChainDepthStencilView(D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, Depth, Stencil);
 }

 void FRHI::RHIDX11_SetSwapChainRenderTargetAndDepthStencilView()
{
	GetRenderThread()->SetSwapChainRenderTargetAndDepthStencilView();
}


 void FRHI::RHIDX11_UpdataViewPort(int TopLeftX, int TopLeftY, int Width, int Height,  float MidDepth /*= 0.0f*/, float MaxDepth /*= 1.0f*/, UINT NumViewports /*= 1*/)
{
	//更新渲染管线
	GetRenderThread()->SetViewPort((float)TopLeftX, (float)TopLeftY, (float)Width, (float)Height, MidDepth, MaxDepth, NumViewports);
	FRHI::RHIDX11_SwapChainResize();
}

 bool FRHI::RHIDX11_GetRenderStart()
{
	return  GetRenderThread()->GetRenderStart();
}


 bool FRHI::ImGui_ImplWin32_MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;
	return false;
}

 void FRHI::RHIDX11_SetRenderTargetsAndDepthStencilView(ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT NumViews/*=1*/)
{
	GetRenderThread()->SetRenderTargetsAndDepthStencilView(ppRenderTargetViews, pDepthStencilView, NumViews);
}

 void FRHI::RHIDX11_CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer)
{
	 GetRenderThread()->CreateBuffer(pDesc, pInitialData, ppBuffer);
}


 void FRHI::RHIDX11_CreateTexture2D(  const D3D11_TEXTURE2D_DESC* pDesc,const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D)
 {
	HR(GetRenderThread()->GetD3DInterface().D3D11Device->CreateTexture2D(pDesc, pInitialData, ppTexture2D));
 }

 void FRHI::RHIDX11_CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView)
 {
	 HR(GetRenderThread()->GetD3DInterface().D3D11Device->CreateShaderResourceView(pResource, pDesc, ppSRView));
 }

 void FRHI::RHIDX11_CreateShaderResourceView(const Image* srcImages, size_t nimages, const TexMetadata& metadata, ID3D11ShaderResourceView** ppSRV)
 {
	HR(CreateShaderResourceView(GetRenderThread()->GetD3DInterface().D3D11Device.Get(), srcImages, nimages, metadata, ppSRV));
 }

 void FRHI::RHIDX11_CreateShaderResourceViewWIC(const FString& fileName, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView, size_t maxsize)
 {
	 HR(CreateWICTextureFromFile(GetRenderThread()->GetD3DInterface().D3D11Device.Get(), *fileName, texture, textureView, maxsize));
 }

 void FRHI::RHIDX11_CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView)
 {
	 HR(GetRenderThread()->GetD3DInterface().D3D11Device->CreateUnorderedAccessView(pResource, pDesc, ppUAView));
 }

 void FRHI::RHIDX11_CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
 {
	 HR(GetRenderThread()->GetD3DInterface().D3D11Device->CreateSamplerState(pSamplerDesc, ppSamplerState));
 }

 void FRHI::RHIDX11_CopySubresourceRegion(ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, const D3D11_BOX* pSrcBox)
 {
	 GetRenderThread()->GetD3DInterface().D3D11ImmediateContext->CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
 }

 void FRHI::RHIDX11_UpdateSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch/*=0*/)
 {
	 GetRenderThread()->GetD3DInterface().D3D11ImmediateContext->UpdateSubresource(
		 pDstResource, DstSubresource
		 , pDstBox //&box
		 , pSrcData			// data
		 , SrcRowPitch		// row pitch
		 , SrcDepthPitch	// depth pitch
	 );
 }

 void FRHI::RHIDX11_GenerateMips(ID3D11ShaderResourceView* pShaderResourceView)
 {
	 GetRenderThread()->GetD3DInterface().D3D11ImmediateContext->GenerateMips(pShaderResourceView);
 }

 void FRHI::RHIDX11_Flush()
 {
	 GetRenderThread()->GetD3DInterface().D3D11ImmediateContext->Flush();
 }

 void FRHI::RHIDX11_SwapChainResize()
 {
	 GetRenderThread()->OnResize();
 }

 

 void FRHI::RHIDX11_SetRenderTargets(ID3D11RenderTargetView* const* ppRenderTargetViews)
 {
	 GetRenderThread()->SetRenderTargets(ppRenderTargetViews);
 }

 ID3D11RenderTargetView* FRHI::RHIDX11_GetSwapChainRTV()
{
	return GetRenderThread()->GetSwapChainRTV();
}

 ID3D11RenderTargetView** FRHI::RHIDX11_GetSwapChainRTVAddress()
{
	return GetRenderThread()->GetSwapChainRTVAddress();
}

 void FRHI::RHIDX11_SetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets)
{
	GetRenderThread()->SetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

 void FRHI::RHIDX11_SetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
	GetRenderThread()->SetIndexBuffer(pIndexBuffer, Format, Offset);
}

 void FRHI::RHIDX11_SetInputLayout(ID3D11InputLayout* pInputLayout)
{
	GetRenderThread()->SetInputLayout(pInputLayout);
}

 void FRHI::RHIDX11_DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
	GetRenderThread()->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

 void FRHI::RHIDX11_SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef/*=0*/)
{
	GetRenderThread()->SetDepthStencilState(pDepthStencilState, StencilRef);
}

 void FRHI::RHIDX11_SetRasterizerState(ID3D11RasterizerState* pRasterizerState)
{
	GetRenderThread()->SetRasterizerState(pRasterizerState);
}

 void FRHI::RHIDX11_SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY Topology/*=D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*/)
{
	GetRenderThread()->SetPrimitiveTopology(Topology);
}

 // FRender::D3D11Struct& FRHI::GetD3DInterface()
// {
// 	return GetRenderThread()->GetD3DInterface();
// }

 ID3D11Device* FRHI::RHIDX11_GetD3d11Device()
{
   return	GetRenderThread()->GetD3DInterface().D3D11Device.Get();
}

 ID3D11DeviceContext* FRHI::RHIDX11_GetID3D11DeviceContext()
{
	return	GetRenderThread()->GetD3DInterface().D3D11ImmediateContext.Get();
}
