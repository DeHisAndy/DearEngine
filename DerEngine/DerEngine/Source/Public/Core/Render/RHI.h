#include "../Vector/Vector4D.h"
#include "Render.h"
#include "../../ThirdParty/DirectXTex/DirectXTex/DirectXTex.h"
class FRHI
{

public:
    static bool ImGui_ImplWin32_MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void RHIDX11_ExecutePresent();
    static void RHIDX11_ClearSwapChainRenderTargetView(FVector4D color = FVector4D(0.f, 0.f, 0.f, 1.0f));
    static void RHIDX11_ClearSwapChainDepthStencilView(UINT ClearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float Depth = 1.0f, unsigned char Stencil = 0);
    static void RHIDX11_ClearSwapChainRTVAndDSV(FVector4D color = FVector4D(0.f, 0.f, 0.f, 1.0f), UINT ClearFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, float Depth = 1.0f, unsigned char Stencil = 0);
    static void RHIDX11_SetSwapChainRenderTargetAndDepthStencilView();
    static void RHIDX11_UpdataViewPort(int TopLeftX, int TopLeftY, int Width, int Height, float MidDepth = 0.0f, float MaxDepth = 1.0f, UINT NumViewports = 1);
    static void RHIDX11_SetRenderTargetsAndDepthStencilView(ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT NumViews = 1);
    static void RHIDX11_SetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
    static void RHIDX11_SetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
    static void RHIDX11_SetInputLayout(ID3D11InputLayout* pInputLayout);
    static void RHIDX11_SetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef = 0);
    static void RHIDX11_SetRasterizerState(ID3D11RasterizerState* pRasterizerState);
    static void RHIDX11_SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY Topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    static  ID3D11Device* RHIDX11_GetD3d11Device();
    static  ID3D11DeviceContext* RHIDX11_GetID3D11DeviceContext();
    static ID3D11RenderTargetView* RHIDX11_GetSwapChainRTV();
    static ID3D11RenderTargetView** RHIDX11_GetSwapChainRTVAddress();
    static class  UShaderCompilerWorker* RHIDX11_GetShaderCompilerWorker();
    static bool RHIDX11_GetRenderStart();
    static void RHIDX11_DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation);
    static void RHIDX11_CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffe);
    static void RHIDX11_CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D);
	static void RHIDX11_CreateShaderResourceView(ID3D11Resource* pResource,  const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);
    static void RHIDX11_CreateShaderResourceView(const Image* srcImages, size_t nimages, const TexMetadata& metadata, ID3D11ShaderResourceView** ppSRV);
	static void RHIDX11_CreateShaderResourceViewWIC(const FString& fileName,ID3D11Resource** texture,ID3D11ShaderResourceView** textureView,size_t maxsize=0);
	static void RHIDX11_CreateUnorderedAccessView( ID3D11Resource* pResource,const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView);
	static void RHIDX11_CreateSamplerState( const D3D11_SAMPLER_DESC* pSamplerDesc,ID3D11SamplerState** ppSamplerState);
	static void RHIDX11_CopySubresourceRegion(ID3D11Resource* pDstResource,UINT DstSubresource,UINT DstX,UINT DstY,UINT DstZ,ID3D11Resource * pSrcResource,UINT SrcSubresource, const D3D11_BOX * pSrcBox);
	static void RHIDX11_UpdateSubresource( ID3D11Resource* pDstResource, UINT DstSubresource,const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch=0);
	static void RHIDX11_GenerateMips(ID3D11ShaderResourceView* pShaderResourceView);
	static void RHIDX11_Flush();
    static void RHIDX11_SwapChainResize();
    static void RHIDX11_SetRenderTargets(ID3D11RenderTargetView* const* ppRenderTargetViews);
    //	static FRender::D3D11Struct& GetD3DInterface();
};