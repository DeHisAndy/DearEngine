#pragma once
#include <d3d11.h>
#include <wrl/client.h>
template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

class FRHIRenderPassInfo
{
public:
	enum FGbufferPass
	{
		SkyBox
	};
public:
// 	ID3D11ShaderResourceView* GetGbufferPassSRV(FGbufferPass type);
// 	ID3D11RenderTargetView* GetGbufferPassRTV(FGbufferPass type);
	ID3D11ShaderResourceView** GetHdrCubeMapSrvAddress();
	ID3D11ShaderResourceView* GetHdrCubeMapSrv();

protected:
	//HDR×ª»»cubeMap
	ComPtr<ID3D11ShaderResourceView>	m_HdrpDynamicCubeMapSRV;
};