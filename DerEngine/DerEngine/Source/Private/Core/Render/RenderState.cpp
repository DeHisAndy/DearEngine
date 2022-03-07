
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

	// ��ǰ��ʼ�����Ļ���û��Ҫ������
	if (IsInit())
		return;
	// ******************
	// ��ʼ����դ����״̬
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
	// �߿�ģʽ
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(device->CreateRasterizerState(&rasterizerDesc, &RSWireframe));

	// �ޱ����޳�ģʽ
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&rasterizerDesc, &RSNoCull));

	// ******************
	// ��ʼ��������״̬
	//
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	// ���Թ���ģʽ
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(device->CreateSamplerState(&sampDesc, &SSLinearWrap));

	// �������Թ���ģʽ
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
	// ��ʼ�����״̬
	//
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	auto& rtDesc = blendDesc.RenderTarget[0];
	// Alpha-To-Coverageģʽ
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	rtDesc.BlendEnable = false;
	rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HR(device->CreateBlendState(&blendDesc, &BSAlphaToCoverage));

	// ͸�����ģʽ
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

	// ����ɫд����ģʽ
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
		// �ر���Ȳ��Ե����/ģ��״̬
		// �����Ʒ�͸�����壬����ϸ��ջ���˳��
		// ����͸����������Ҫ���Ļ���˳��
		// ��Ĭ�������ģ����Ծ��ǹرյ�
		dsDesc.DepthEnable = false;
		dsDesc.StencilEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		HR(device->CreateDepthStencilState(&dsDesc, &DSSNoDepthTest));

		// �ر���Ȳ���
		// �����Ʒ�͸�����壬����ϸ��ջ���˳��
		// ����͸����������Ҫ���Ļ���˳��
		// ������ģ��ֵ����������Ž��л���
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// ���ڱ���ļ����������ǲ�������Ⱦ�ģ���������������޹ؽ�Ҫ
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		HR(device->CreateDepthStencilState(&dsDesc, &DSSNoDepthTest));

		// ������Ȳ��ԣ�����д�����ֵ��״̬
	// �����Ʒ�͸������ʱ��Ӧʹ��Ĭ��״̬
	// ����͸������ʱ��ʹ�ø�״̬������Чȷ�����״̬�Ľ���
	// ����ȷ����ǰ�ķ�͸����������赲�Ϻ��һ������
		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
		dsDesc.StencilEnable = false;

		HR(device->CreateDepthStencilState(&dsDesc, &DSSNoDepthWrite));

		// ������Ȳ��ԣ�����д�����ֵ��״̬
		// �����Ʒ�͸������ʱ��Ӧʹ��Ĭ��״̬
		// ����͸������ʱ��ʹ�ø�״̬������Чȷ�����״̬�Ľ���
		// ����ȷ����ǰ�ķ�͸����������赲�Ϻ��һ������
		// ������ģ��ֵ����������Ž��л���
		dsDesc.StencilEnable = true;
		dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		// ���ڱ���ļ����������ǲ�������Ⱦ�ģ���������������޹ؽ�Ҫ
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

