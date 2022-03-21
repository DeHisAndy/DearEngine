#pragma once
#include <d3d11.h>
#include "..\..\Vector\String.h"
#include "..\..\Vector\Vector.h"
#include <wrl/client.h>
#include "Object.h"
enum EImageFormat
{
	// RGBA
	RGBA32F = DXGI_FORMAT_R32G32B32A32_FLOAT,
	RGBA16F = DXGI_FORMAT_R16G16B16A16_FLOAT,
	RGBA8UN = DXGI_FORMAT_R8G8B8A8_UNORM,

	// RGB
	RGB32F = DXGI_FORMAT_R32G32B32_FLOAT,
	R11G11B10F = DXGI_FORMAT_R11G11B10_FLOAT,

	// RG
	RG32F = DXGI_FORMAT_R32G32_FLOAT,
	RG16F = DXGI_FORMAT_R16G16_FLOAT,

	// R
	R32F = DXGI_FORMAT_R32_FLOAT,
	R32U = DXGI_FORMAT_R32_UINT,

	R8U = DXGI_FORMAT_R8_UINT,
	R8UN = DXGI_FORMAT_R8_UNORM,

	// Typeless
	R32 = DXGI_FORMAT_R32_TYPELESS,
	R24G8 = DXGI_FORMAT_R24G8_TYPELESS,

	// 
	R24_UNORM_X8_TYPELESS = DXGI_FORMAT_R24_UNORM_X8_TYPELESS,

	// Depth
	D32F = DXGI_FORMAT_D32_FLOAT,
	D24UNORM_S8U = DXGI_FORMAT_D24_UNORM_S8_UINT,

	IMAGE_FORMAT_UNKNOWN,
	IMAGE_FORMAT_COUNT
};
enum ETextureUsage : unsigned
{
	RESOURCE = D3D11_BIND_SHADER_RESOURCE,
	RENDER_TARGET = D3D11_BIND_RENDER_TARGET,
	RENDER_TARGET_RW = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
	DEPTH_TARGET = D3D11_BIND_DEPTH_STENCIL,
	COMPUTE_RW_TEXTURE = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,

	TEXTURE_USAGE_COUNT
};
enum ECPUAccess : unsigned
{
	NONE = 0,
	CPU_R = D3D11_CPU_ACCESS_READ,
	CPU_W = D3D11_CPU_ACCESS_WRITE,
	CPU_RW = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE
};
struct TextureDesc
{
	int width;
	int height;
	EImageFormat format;
	ETextureUsage usage;
	FString	 texFileName;
	void* pData;
	int dataPitch;		// used for 2D/3D Textures - how many bytes per row of pixels
	int dataSlicePitch;	// used for 3D Textures
	int mipCount;
	int arraySize;
	bool bIsCubeMap;
	bool bGenerateMips;
	ECPUAccess cpuAccessMode;

	TextureDesc() :
		width(1),
		height(1),
		format(RGBA16F),
		usage(RENDER_TARGET_RW),
		texFileName(""),
		pData(nullptr),
		dataSlicePitch(0),
		mipCount(1),
		arraySize(1),
		bIsCubeMap(false),
		bGenerateMips(false),
		cpuAccessMode(ECPUAccess::NONE)
	{}

	D3D11_TEXTURE2D_DESC dxDesc;
};

class Texture:public UObject
{
public:
	Texture();
	~Texture();

	bool InitializeTexture2D(const D3D11_TEXTURE2D_DESC& descriptor, bool initializeSRV);
public:
	static  Texture* CreateTexture2D(const TextureDesc& texDesc);
	static  Texture* CreateHDRTexture(const FString& texFileName);
	static Texture* CreateTextureFromFile(const  FString& texFileName, bool bGenerateMips = false /*= false*/);
	static void CreateWICTexture2DCubeFromFile(const std::vector<FString>& cubeMapFileNames, ID3D11Texture2D** textureArray, ID3D11ShaderResourceView** textureCubeView, bool generateMips = false);
	static int CreateCubemapFromFaceTextures(const std::vector<FString>& textureFiles, bool bGenerateMips, unsigned mipLevels = 1);

private:
	void Release();
public:
	ID3D11ShaderResourceView* _srv;
	ID3D11UnorderedAccessView* _uav;
	TArray<ID3D11ShaderResourceView*>  _srvArray;
	TArray<ID3D11UnorderedAccessView*> _uavArray;
	union
	{
		ID3D11Texture3D* _tex3D;
		ID3D11Texture2D* _tex2D;
	};

	unsigned			_width;
	unsigned			_height;
	unsigned			_depth;
	FString				_name;
	int				    _id;
};

class TextureManage:public UObject
{
public:
	static TextureManage* Get();
	static	ComPtr<ID3D11SamplerState> LinerSamplerState_WRAP;//线性采样
	static	ComPtr<ID3D11SamplerState> LinerSamplerState_Clamp;//三线性采样
	static	ComPtr<ID3D11SamplerState> LinerSamplerState_NEVER;//三线性采样
	static	ComPtr<ID3D11SamplerState> PointSamplerState_WRAP;//点采样
	static	ComPtr<ID3D11SamplerState> PointSamplerState_Clamp;//点采样
	void Destroy();
public:
	virtual	~TextureManage();
private:
	static TextureManage* Instance;
	TMap<FString, Texture*> mTextures;
	TextureManage();
public:
	
	void  Add(const FString& name, Texture* texture);
	Texture* FindIndex(const FString& name);
	int Size();
	TMap<FString,Texture*>& GetArray();
};

