
#include "../../../Public/Core/FunctionLibrary/KismetSystemLibrary.h"
#include "../../../Public/Core/Render/RenderState.h"

ID3D11DepthStencilState* FRenderState::DSSNoDepthTestWithStencil = nullptr;
ID3D11DepthStencilState* FRenderState::DSSNoDepthTest = nullptr;
ID3D11DepthStencilState* FRenderState::DSSDrawWithStencil = nullptr;
ID3D11DepthStencilState* FRenderState::DSSNoDepthWrite = nullptr;
ID3D11DepthStencilState* FRenderState::DSSNoDepthWriteWithStencil = nullptr;
ID3D11RasterizerState* FRenderState::RSDefault = nullptr;
ID3D11RasterizerState* FRenderState::RSDefaultForword = nullptr;

ID3D11RasterizerState* FRenderState::RSWireframe = nullptr;
ID3D11RasterizerState* FRenderState::RSNoCull = nullptr;

ID3D11SamplerState* FRenderState::SSLinearWrap = nullptr;
ID3D11SamplerState* FRenderState::SSAnistropicWrap = nullptr;

ID3D11BlendState* FRenderState::BSNoColorWrite = nullptr;
ID3D11BlendState* FRenderState::BSTransparent = nullptr;
ID3D11BlendState* FRenderState::BSAlphaToCoverage = nullptr;

ID3D11BlendState* FRenderState::BSAdditive = nullptr;
ID3D11DepthStencilState* FRenderState::DSSWriteStencil = nullptr;
ID3D11DepthStencilState* FRenderState::DSSNoDoubleBlend = nullptr;

ID3D11DepthStencilState* FRenderState::DSSSkyBox = nullptr;


FRenderState::~FRenderState()
{
	if (RSWireframe)
	{
		RSWireframe->Release();
	}
	if (RSNoCull)
	{
		RSNoCull->Release();
	}
	if (SSLinearWrap)
	{
		SSLinearWrap->Release();
	}
	if (SSAnistropicWrap)
	{
		SSAnistropicWrap->Release();
	}
	if (BSNoColorWrite)
	{
		BSNoColorWrite->Release();
	}
	if (BSTransparent)
	{
		BSTransparent->Release();
	}
	if (BSAlphaToCoverage)
	{
		BSAlphaToCoverage->Release();
	}

	if (BSAdditive)
	{
		BSAdditive->Release();
	}

	if (DSSWriteStencil)
	{
		DSSWriteStencil->Release();
	}

	if (DSSDrawWithStencil)
	{
		DSSDrawWithStencil->Release();
	}

	if (DSSNoDoubleBlend)
	{
		DSSNoDoubleBlend->Release();
	}

	if (DSSNoDepthTest)
	{
		DSSNoDepthTest->Release();
	}

	if (DSSNoDepthWrite)
	{
		DSSNoDepthWrite->Release();
	}

	if (DSSNoDepthTestWithStencil)
	{
		DSSNoDepthTestWithStencil->Release();
	}

	if (DSSNoDepthWriteWithStencil)
	{
		DSSNoDepthWriteWithStencil->Release();
	}
	if (DSSSkyBox)
	{
		DSSSkyBox->Release();
	}
	if (RSDefaultForword)
	{
		RSDefaultForword->Release();
	}
}

bool FRenderState::IsInit()
{
	if (FRenderState::RSWireframe && FRenderState::
		RSNoCull && FRenderState::SSLinearWrap &&
		FRenderState::SSAnistropicWrap &&
		FRenderState::BSNoColorWrite &&
		FRenderState::BSTransparent &&
		FRenderState::BSAlphaToCoverage)
	{
		return true;
	}
	return false;
}

void FRenderState::Init(ID3D11Device* device)
{

	// 先前初始化过的话就没必要重来了
	if (IsInit())
		return;
	// ******************
	// 初始化光栅化器状态
	//
	D3D11_RASTERIZER_DESC Default;;
	ZeroMemory(&Default, sizeof(D3D11_RASTERIZER_DESC));
	Default.FillMode = D3D11_FILL_SOLID;
	Default.CullMode = D3D11_CULL_BACK;
	Default.FrontCounterClockwise = false;
	Default.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&Default, &RSDefault));
	Default.CullMode = D3D11_CULL_FRONT;
	HR(device->CreateRasterizerState(&Default, &RSDefaultForword));
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	// 线框模式
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&rasterizerDesc, &RSWireframe));

	// 无背面剔除模式
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&rasterizerDesc, &RSNoCull));

	// ******************
	// 初始化采样器状态
	//
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	// 线性过滤模式
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(device->CreateSamplerState(&sampDesc, &SSLinearWrap));

	// 各向异性过滤模式
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(device->CreateSamplerState(&sampDesc, &SSAnistropicWrap));

	// ******************
	// 初始化混合状态
	//
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	auto& rtDesc = blendDesc.RenderTarget[0];
	// Alpha-To-Coverage模式
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	rtDesc.BlendEnable = false;
	rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(device->CreateBlendState(&blendDesc, &BSAlphaToCoverage));

	// 透明混合模式
	// Color = SrcAlpha * SrcColor + (1 - SrcAlpha) * DestColor 
	// Alpha = SrcAlpha
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	rtDesc.BlendEnable = true;
	rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

	HR(device->CreateBlendState(&blendDesc, &BSTransparent));

	// 无颜色写入混合模式
	// Color = DestColor
	// Alpha = DestAlpha
	rtDesc.SrcBlend = D3D11_BLEND_ZERO;
	rtDesc.DestBlend = D3D11_BLEND_ONE;
	rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtDesc.SrcBlendAlpha = D3D11_BLEND_ZERO;
	rtDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	HR(device->CreateBlendState(&blendDesc, &BSNoColorWrite));

	{
		D3D11_DEPTH_STENCIL_DESC dsDesc;

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		dsDesc.StencilEnable = false;

		HR(device->CreateDepthStencilState(&dsDesc, &DSSSkyBox));
		// 关闭深度测试的深度/模板状态
		// 若绘制非透明物体，务必严格按照绘制顺序
		// 绘制透明物体则不需要担心绘制顺序
		// 而默认情况下模板测试就是关闭的
		dsDesc.DepthEnable = false;
		dsDesc.StencilEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		HR(device->CreateDepthStencilState(&dsDesc, &DSSNoDepthTest));

		// 关闭深度测试
		// 若绘制非透明物体，务必严格按照绘制顺序
		// 绘制透明物体则不需要担心绘制顺序
		// 对满足模板值条件的区域才进行绘制
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		HR(device->CreateDepthStencilState(&dsDesc, &DSSNoDepthTest));

		// 进行深度测试，但不写入深度值的状态
	// 若绘制非透明物体时，应使用默认状态
	// 绘制透明物体时，使用该状态可以有效确保混合状态的进行
	// 并且确保较前的非透明物体可以阻挡较后的一切物体
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = false;

		HR(device->CreateDepthStencilState(&dsDesc, &DSSNoDepthWrite));

		// 进行深度测试，但不写入深度值的状态
		// 若绘制非透明物体时，应使用默认状态
		// 绘制透明物体时，使用该状态可以有效确保混合状态的进行
		// 并且确保较前的非透明物体可以阻挡较后的一切物体
		// 对满足模板值条件的区域才进行绘制
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// 对于背面的几何体我们是不进行渲染的，所以这里的设置无关紧要
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

		HR(device->CreateDepthStencilState(&dsDesc, &DSSNoDepthWriteWithStencil));
	}
}

ID3D11RasterizerState* FRenderState::GetModeRenderState(RenderModeEnum mode)
{
	switch (mode)
	{
	case FRenderState::oneFace:
		return nullptr;
		break;
	case FRenderState::TwoFace:
		return FRenderState::RSNoCull;
		break;
	default:
		return nullptr;
		break;
	}
}

