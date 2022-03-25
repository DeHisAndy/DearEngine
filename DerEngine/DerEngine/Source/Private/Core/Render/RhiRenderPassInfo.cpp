#include "../../../Core/Render/RhiRenderPassInfo.h"

ID3D11ShaderResourceView** FRHIRenderPassInfo::GetHdrCubeMapSrvAddress()
{
	return m_HdrpDynamicCubeMapSRV.GetAddressOf();
}

ID3D11ShaderResourceView* FRHIRenderPassInfo::GetHdrCubeMapSrv()
{
	return m_HdrpDynamicCubeMapSRV.Get();
}
