#pragma once
#include "../Vector/Vector4D.h"
#include "Render.h"
#include "../../ThirdParty/DirectXTex/DirectXTex/DirectXTex.h"
class FRHI
{

public:
    static bool ImGui_ImplWin32_MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void RHI_ExecutePresent();
    static void RHI_ClearSwapChainRenderTargetView(FVector4D color = FVector4D(0.f, 0.f, 0.f, 1.0f));
    static void RHI_ClearSwapChainDepthStencilView(UINT ClearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float Depth = 1.0f, unsigned char Stencil = 0);
    static void RHI_ClearSwapChainRTVAndDSV(FVector4D color = FVector4D(0.f, 0.f, 0.f, 1.0f), UINT ClearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float Depth = 1.0f, unsigned char Stencil = 0);
    static void RHI_SetSwapChainRenderTargetAndDepthStencilView();
    static void RHI_UpdataViewPort(int TopLeftX, int TopLeftY, int Width, int Height, float MidDepth = 0.0f, float MaxDepth = 1.0f, UINT NumViewports = 1);
    static void RHI_SetRenderTargetsAndDepthStencilView(ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT NumViews = 1);
    static void RHI_SetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
    static void RHI_SetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
    static void RHI_SetInputLayout(ID3D11InputLayout* pInputLayout);
    static void RHI_SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef = 0);
    static void RHI_SetRasterizerState(ID3D11RasterizerState* pRasterizerState);
    static void RHI_SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    static  ID3D11Device* RHI_GetD3d11Device();
    static  ID3D11DeviceContext* RHI_GetID3D11DeviceContext();
    static ID3D11RenderTargetView* RHI_GetSwapChainRTV();
    static ID3D11RenderTargetView** RHI_GetSwapChainRTVAddress();
    static class  UShaderCompilerWorker* RHI_GetShaderCompilerWorker();
    static class  FRHIRenderPassInfo* RHI_GetRenderPassInfo();
    static bool RHI_GetRenderStart();
    static void RHI_DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
    static void RHI_CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffe);
    static void RHI_CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
	static void RHI_CreateShaderResourceView(ID3D11Resource* pResource,  const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);
    static void RHI_CreateShaderResourceView(const Image* srcImages, size_t nimages, const TexMetadata& metadata, ID3D11ShaderResourceView** ppSRV);
	static void RHI_CreateShaderResourceViewWIC(const FString& fileName,ID3D11Resource** texture,ID3D11ShaderResourceView** textureView,size_t maxsize=0);
	static void RHI_CreateUnorderedAccessView( ID3D11Resource* pResource,const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView);
	static void RHI_CreateSamplerState( const D3D11_SAMPLER_DESC* pSamplerDesc,ID3D11SamplerState** ppSamplerState);
	static void RHI_CopySubresourceRegion(ID3D11Resource* pDstResource,UINT DstSubresource,UINT DstX,UINT DstY,UINT DstZ,ID3D11Resource * pSrcResource,UINT SrcSubresource, const D3D11_BOX * pSrcBox);
	static void RHI_UpdateSubresource( ID3D11Resource* pDstResource, UINT DstSubresource,const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch=0);
	static void RHI_GenerateMips(ID3D11ShaderResourceView* pShaderResourceView);
	static void RHI_Flush();
    static void RHI_SwapChainResize();
    static void RHI_SetRenderTargets(ID3D11RenderTargetView* const* ppRenderTargetViews); 
	//创建cubeMap资源
	static	void CreateTexture2DCubeFromArray(unsigned int Width, unsigned int height,ID3D11ShaderResourceView** m_pDynamicCubeMapSRV,
		ComPtr<ID3D11RenderTargetView>* m_pDynamicCubeMapRTVs,/* ComPtr<ID3D11DepthStencilView>& m_DSV,*/ DXGI_FORMAT Format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	static DirectX::XMMATRIX CaptureScene(XMFLOAT3 pos, D3D11_TEXTURECUBE_FACE face, ComPtr<ID3D11RenderTargetView>& m_pDynamicCubeMapRTVs,
		unsigned int ViewPortSize_X = 512, unsigned int ViewPortSize_Y = 512);
    //	static FRender::D3D11Struct& GetD3DInterface();
};