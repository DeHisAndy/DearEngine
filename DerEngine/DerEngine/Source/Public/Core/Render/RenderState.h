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

	static ID3D11RasterizerState* RSDefault;// Ĭ�ϣ�����ü�ģʽ
	static ID3D11RasterizerState* RSDefaultForword;// Ĭ�ϣ�����ü�
	//��դ��״̬
	static ID3D11RasterizerState* RSWireframe;// ��դ����״̬���߿�ģʽ
	static ID3D11RasterizerState* RSNoCull;// ��դ����״̬���ޱ���ü�ģʽ
	//����
	static ID3D11SamplerState* SSLinearWrap;// ������״̬�����Թ���
	static ID3D11SamplerState* SSAnistropicWrap;// ������״̬���������Թ���
	//���
	static ID3D11BlendState* BSNoColorWrite;// ���״̬����д����ɫ
	static ID3D11BlendState* BSTransparent; // ���״̬��͸�����
	static ID3D11BlendState* BSAlphaToCoverage;//���״̬��Alpha-To-Coverage

	static ID3D11BlendState* BSAdditive;			// ���״̬���ӷ����
	static ID3D11DepthStencilState* DSSNoDepthWriteWithStencil;	// ���/ģ��״̬������Ȳ��ԣ���д�����ֵ����ָ��ģ��ֵ��������л���
	static ID3D11DepthStencilState* DSSNoDepthTestWithStencil;	// ���/ģ��״̬���ر���Ȳ��ԣ���ָ��ģ��ֵ��������л���
	static ID3D11DepthStencilState* DSSNoDepthTest;		// ���/ģ��״̬���ر���Ȳ���
	static ID3D11DepthStencilState* DSSNoDepthWrite;		// ���/ģ��״̬������Ȳ��ԣ���д�����ֵ

	static ID3D11DepthStencilState* DSSDrawWithStencil;	// ���/ģ��״̬����ָ��ģ��ֵ��������л���
	static ID3D11DepthStencilState* DSSNoDoubleBlend;	// ���/ģ��״̬���޶��λ������

	static ID3D11DepthStencilState* DSSSkyBox;	// ���/ģ��״̬������� <=1 ��Ϊ���ֵΪ1.0ʱĬ���޷�ͨ����Ȳ���
	static ID3D11DepthStencilState* DSSWriteStencil;		// ���/ģ��״̬��д��ģ��ֵ


	~FRenderState();
};
