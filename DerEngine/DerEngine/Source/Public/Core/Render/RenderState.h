#pragma once
#include <memory>
#include <d3d11_1.h>


class FRenderState
{
public:
	enum RenderModeEnum
	{
		oneFace,
		TwoFace,
	};
public:
	static bool IsInit();
	static void Init(ID3D11Device* device);
	static void Destroy();
	static	ID3D11RasterizerState* GetModeRenderState(RenderModeEnum mode);
public:

	static ID3D11RasterizerState* RSDefault;// 默认：背面裁剪模式
	static ID3D11RasterizerState* RSDefaultForword;// 默认：正面裁剪
	//光栅化状态
	static ID3D11RasterizerState* RSWireframe;// 光栅化器状态：线框模式
	static ID3D11RasterizerState* RSNoCull;// 光栅化器状态：无背面裁剪模式
	//采样
	static ID3D11SamplerState* SSLinearWrap;// 采样器状态：线性过滤
	static ID3D11SamplerState* SSAnistropicWrap;// 采样器状态：各项异性过滤
	//混合
	static ID3D11BlendState* BSNoColorWrite;// 混合状态：不写入颜色
	static ID3D11BlendState* BSTransparent; // 混合状态：透明混合
	static ID3D11BlendState* BSAlphaToCoverage;//混合状态：Alpha-To-Coverage

	static ID3D11BlendState* BSAdditive;			// 混合状态：加法混合
	static ID3D11DepthStencilState* DSSNoDepthWriteWithStencil;	// 深度/模板状态：仅深度测试，不写入深度值，对指定模板值的区域进行绘制
	static ID3D11DepthStencilState* DSSNoDepthTestWithStencil;	// 深度/模板状态：关闭深度测试，对指定模板值的区域进行绘制
	static ID3D11DepthStencilState* DSSNoDepthTest;		// 深度/模板状态：关闭深度测试
	static ID3D11DepthStencilState* DSSNoDepthWrite;		// 深度/模板状态：仅深度测试，不写入深度值

	static ID3D11DepthStencilState* DSSDrawWithStencil;	// 深度/模板状态：对指定模板值的区域进行绘制
	static ID3D11DepthStencilState* DSSNoDoubleBlend;	// 深度/模板状态：无二次混合区域

	static ID3D11DepthStencilState* DSSSkyBox;	// 深度/模板状态：天空球 <=1 因为深度值为1.0时默认无法通过深度测试
	static ID3D11DepthStencilState* DSSWriteStencil;		// 深度/模板状态：写入模板值


	~FRenderState();
};
