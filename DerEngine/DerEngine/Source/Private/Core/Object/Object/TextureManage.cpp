#include "..\..\..\..\Public\Core\Object\Object\TextureManage.h"
#include "..\..\..\..\Public\Core\Render\RHI.h"
#include "..\..\..\..\Public\Core\Helper\Log.h"
#include "..\..\..\..\Public\Core\FunctionLibrary\KismetSystemLibrary.h"
#include "..\..\..\..\Public\ThirdParty\DirectXTex\DirectXTex\DirectXTex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "..\..\..\..\Public\ThirdParty\Stb\stb_image.h"
#include<utility>
#include <array>
#include "..\..\..\Core\FunctionLibrary\KismetStringLibrary.h"
TextureManage* TextureManage::Instance = nullptr;
std::vector<std::string> split(const char* s, char c)
{
	std::vector<std::string> result;
	do
	{
		const char* begin = s;

		if (*begin == c || *begin == '\0')
			continue;	// skip delimiter character

		while (*s != c && *s)
			s++;	// iterate until delimiter is found

		result.push_back(std::string(begin, s));

	} while (*s++);
	return result;
}

std::string GetFileExtension(const FString& filePath)
{
	auto v = split(filePath.GetString().c_str(), '.');
	return v.empty() ? "" : v.back();
}

Texture* Texture::CreateTextureFromFile(const  FString& texFileName, bool bGenerateMips /*= false /*= false*/)
{
	if (texFileName.Empty() || texFileName.GetString() == "\"\"")
	{
		Log_Error(texFileName + "path not find ");
		return nullptr;
	}
	auto& mTextures = TextureManage::Get()->GetArray();
	auto found = mTextures.find(texFileName);
	if (found != mTextures.end())
	{
		return (found->second);
	}
	//获取文件后缀
	FString suffix;
	if (!UKismetStringLibrary::GetFileSuffix(texFileName, suffix))
	{
		Log_Error(texFileName + "The text format is incorrect");
		return nullptr;
	}
	if (suffix == "hdr" || suffix == "HDR")
		{
			//加载HDR图
			return CreateHDRTexture(texFileName);
		}
	//加载png和jpg图片
	const FString path = texFileName;
#if _DEBUG

#endif
	Texture* tex=new Texture();
	tex->_name = texFileName;
	std::unique_ptr<DirectX::ScratchImage> img = std::make_unique<DirectX::ScratchImage>();
	Log_Info("Loading texture: " + path);
	HRESULT error = LoadFromWICFile(*path, WIC_FLAGS_NONE, nullptr, *img);
	if (SUCCEEDED(error))
	{
		auto meta = img->GetMetadata();
		FRHI::RHI_CreateShaderResourceView(img->GetImages(), img->GetImageCount(), meta, &tex->_srv);
		if (bGenerateMips)
		{

			ID3D11Resource* resource = nullptr;
			tex->_srv->GetResource(&resource);
			if (tex->_srv) tex->_srv->Release();
			if (tex->_tex2D) tex->_tex2D->Release();

			meta.mipLevels = min(
				static_cast<size_t>(std::log2(meta.width))
				, static_cast<size_t>(std::log2(meta.height)));

			TextureDesc texDesc = {};
			texDesc.bGenerateMips = true;
			texDesc.bIsCubeMap = meta.depth != 1; // false?
			texDesc.arraySize = static_cast<int>(meta.arraySize);
			texDesc.format = static_cast<EImageFormat>(meta.format);
			texDesc.width = static_cast<int>(meta.width);
			texDesc.height = static_cast<int>(meta.height);
			texDesc.mipCount = static_cast<int>(meta.mipLevels);
			texDesc.usage = ETextureUsage::RENDER_TARGET_RW;
			texDesc.texFileName = texFileName;


			//texDesc.pData = img->GetPixels();
			//texDesc.dataPitch = img->GetImages()->rowPitch;
			//texDesc.dataSlicePitch = img->GetImages()->slicePitch;

			tex = TextureManage::Get()->FindIndex(texDesc.texFileName);

			FRHI::RHI_CopySubresourceRegion(tex->_tex2D, 0, 0, 0, 0, resource, 0, NULL);
			FRHI::RHI_GenerateMips(tex->_srv);
			FRHI::RHI_Flush();
			//#if _DEBUG
			//			SaveTextureToDisk(tex->_id, Application::s_WorkspaceDirectory + "/DEBUG.png", false);
			//#endif
			resource->Release();
		}
		else
		{

			// get srv from img
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			tex->_srv->GetDesc(&srvDesc);

			// read width & height
			ID3D11Resource* resource = nullptr;
			tex->_srv->GetResource(&resource);
			if (SUCCEEDED(resource->QueryInterface(&tex->_tex2D)))
			{
				D3D11_TEXTURE2D_DESC desc;
				tex->_tex2D->GetDesc(&desc);
				tex->_width = desc.Width;
				tex->_height = desc.Height;
			}
			resource->Release();

			tex->_id = static_cast<int>(mTextures.size());
			std::pair<TMap<FString, Texture*>::iterator, bool> success = mTextures.insert(std::pair<FString, Texture*>(path, tex));
			if (success.second == false)
			{
				Log_Error("Texture2D Craete Fail Path=" + texFileName);
				return nullptr;
			}
		}
		return tex;
	}
	else
	{
		FString Name = "Cannot load texture file: %s\n";
		Log_Error(Name + texFileName);
		return nullptr;
	}
}

void Texture::CreateWICTexture2DCubeFromFile(const std::vector<FString>& cubeMapFileNames, ID3D11Texture2D** textureArray, ID3D11ShaderResourceView** textureCubeView, bool generateMips /*= false*/)
{
	// 检查设备与设备上下文是否非空
// 文件名数目需要不小于6
// 纹理数组和资源视图只要有其中一个非空即可
	UINT arraySize = (UINT)cubeMapFileNames.size();

	if ( arraySize < 6 || !(textureArray || textureCubeView))
		return checkSlow(false);

	// ******************
	// 读取纹理
	//

	std::vector<ID3D11Texture2D*> srcTexVec(arraySize, nullptr);
	std::vector<ID3D11ShaderResourceView*> srcTexSRVVec(arraySize, nullptr);
	std::vector<D3D11_TEXTURE2D_DESC> texDescVec(arraySize);
	for (UINT i = 0; i < arraySize; ++i)
	{

		// 该资源用于GPU复制
		FRHI::RHI_CreateShaderResourceViewWIC(
			cubeMapFileNames[i],
			(ID3D11Resource**)&srcTexVec[i],
			(generateMips ? &srcTexSRVVec[i] : nullptr));
		// 读取创建好的纹理信息
		srcTexVec[i]->GetDesc(&texDescVec[i]);

		// 需要检验所有纹理的mipLevels，宽度和高度，数据格式是否一致，
		// 若存在数据格式不一致的情况，请使用dxtex.exe(DirectX Texture Tool)
		// 将所有的图片转成一致的数据格式
		if (texDescVec[i].MipLevels != texDescVec[0].MipLevels || texDescVec[i].Width != texDescVec[0].Width ||
			texDescVec[i].Height != texDescVec[0].Height || texDescVec[i].Format != texDescVec[0].Format)
		{
			for (UINT j = 0; j < i; ++j)
			{
				srcTexVec[j]->Release();
				srcTexVec[j] = nullptr;
				srcTexSRVVec[j]->Release();
				srcTexSRVVec[j] = nullptr;
			}
			return ;
		}
	}

	// ******************
	// 创建纹理数组
	//
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texDescVec[0].Width;
	texArrayDesc.Height = texDescVec[0].Height;
	texArrayDesc.MipLevels = (generateMips ? texDescVec[0].MipLevels : 1);
	texArrayDesc.ArraySize = arraySize;
	texArrayDesc.Format = texDescVec[0].Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;	// 允许从中创建TextureCube

	ID3D11Texture2D* texArray = nullptr;
	 FRHI::RHI_CreateTexture2D (&texArrayDesc, nullptr, &texArray);

	if (!texArray)
	{
		for (UINT i = 0; i < arraySize; ++i)
		{
			srcTexVec[i]->Release();
			srcTexVec[i] = nullptr;
			srcTexSRVVec[i]->Release();
			srcTexSRVVec[i] = nullptr;
		}

		return ;
	}

	// ******************
	// 将原纹理的所有子资源拷贝到该数组中
	//
	texArray->GetDesc(&texArrayDesc);

	for (UINT i = 0; i < arraySize; ++i)
	{
		for (UINT j = 0; j < texArrayDesc.MipLevels; ++j)
		{
			FRHI::RHI_CopySubresourceRegion(
				texArray,
				D3D11CalcSubresource(j, i, texArrayDesc.MipLevels),
				0, 0, 0,
				srcTexVec[i],
				j,
				nullptr);
		}
	}

	// ******************
	// 创建立方体纹理的SRV
	//
	if (textureCubeView)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = texArrayDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		viewDesc.TextureCube.MostDetailedMip = 0;
		viewDesc.TextureCube.MipLevels = texArrayDesc.MipLevels;

		FRHI::RHI_CreateShaderResourceView(texArray, &viewDesc, textureCubeView);
	}

	// 检查是否需要纹理数组
	if (textureArray)
	{
		*textureArray = texArray;
	}
	else
	{
		texArray->Release();
		texArray = nullptr;
	}

	// 释放所有资源
	for (UINT i = 0; i < arraySize; ++i)
	{
		srcTexVec[i]->Release();
		srcTexVec[i] = nullptr;
		srcTexSRVVec[i]->Release();
		srcTexSRVVec[i] = nullptr;
	}

	
}

int Texture::CreateCubemapFromFaceTextures(const std::vector<FString>& textureFiles, bool bGenerateMips, unsigned mipLevels /*= 1*/)
{
	constexpr size_t FACE_COUNT = 6;

	TexMetadata meta = {};

	// get subresource data for each texture to initialize the cubemap
	std::vector<D3D11_SUBRESOURCE_DATA> pSubresourceData(FACE_COUNT * mipLevels);
	std::vector<std::array<DirectX::ScratchImage, FACE_COUNT>> faceImageArray(mipLevels);
	for (unsigned mip = 0; mip < mipLevels; ++mip)
	{
		std::array<DirectX::ScratchImage, FACE_COUNT>& faceImages = faceImageArray[mip];
		for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < FACE_COUNT; cubeMapFaceIndex++)
		{
			const size_t index = mip * FACE_COUNT + cubeMapFaceIndex;
			const FString path = textureFiles[index];


			const std::string extension = GetFileExtension(path);
			const bool bHDRTexture = extension == "hdr" || extension == "HDR";

			DirectX::ScratchImage* img = &faceImages[cubeMapFaceIndex];

			const bool bLoadSuccess = bHDRTexture
				? SUCCEEDED(LoadFromHDRFile(*path, nullptr, *img))
				: SUCCEEDED(LoadFromWICFile(*path, WIC_FLAGS_NONE, nullptr, *img));

			if (!bLoadSuccess)
			{
				Log::Get()->Error(textureFiles[index]);
				continue;
			}

			pSubresourceData[index].pSysMem = img->GetPixels(); // Pointer to the pixel data
			pSubresourceData[index].SysMemPitch = static_cast<UINT>(img->GetImage(0, 0, 0)->rowPitch); // Line width in bytes
			pSubresourceData[index].SysMemSlicePitch = static_cast<UINT>(img->GetImages()->slicePitch); // This is only used for 3d textures

			if (cubeMapFaceIndex == 0 && mip == 0)
			{
				meta = faceImages[0].GetMetadata();
			}
		}
	}

#if _DEBUG
	Log_Error("tLoading Cubemap Texture:" + textureFiles.back());
#endif

	// initialize the destination texture desc
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = static_cast<UINT>(meta.width);
	texDesc.Height = static_cast<UINT>(meta.height);
	texDesc.MipLevels = bGenerateMips ? mipLevels : static_cast<UINT>(meta.mipLevels);
	texDesc.ArraySize = FACE_COUNT;
	texDesc.Format = meta.format;
	texDesc.CPUAccessFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	if (bGenerateMips)
	{
		texDesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}


	// create the resource
	ID3D11Texture2D* finalCubemapTexture=nullptr;
	const D3D11_SUBRESOURCE_DATA* pData = bGenerateMips ? nullptr : pSubresourceData.data();
	FRHI::RHI_CreateTexture2D(&texDesc, pData, &finalCubemapTexture);
	if (!finalCubemapTexture)
	{
		Log_Error(FString("Cannot create cubemap texture: ") + split(textureFiles.front().GetString().c_str(), '_').front().c_str());
		return -1;
	}

	// create cubemap srv
	ID3D11ShaderResourceView* cubeMapSRV=nullptr;
	D3D11_SHADER_RESOURCE_VIEW_DESC cubemapDesc;
	cubemapDesc.Format = texDesc.Format;
	cubemapDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	cubemapDesc.TextureCube.MipLevels = texDesc.MipLevels;
	cubemapDesc.TextureCube.MostDetailedMip = 0;
	FRHI::RHI_CreateShaderResourceView(finalCubemapTexture, &cubemapDesc, &cubeMapSRV);
	if (!cubeMapSRV)
	{
		Log::Get()->Error(std::string("Cannot create Shader Resource View for ") + split(textureFiles.front().GetString().c_str(), '_').front());
		return -1;
	}
	// copy the mip levels into the final resource
	if (bGenerateMips)
	{
		//https://www.gamedev.net/forums/topic/599837-dx11-createtexture2d-automatic-mips-initial-data/
		FRHI::RHI_GenerateMips(cubeMapSRV);
		for (unsigned mip = 0; mip < mipLevels; ++mip)
		{
			for (int cubeMapFaceIndex = 0; cubeMapFaceIndex < FACE_COUNT; cubeMapFaceIndex++)
			{
				const size_t index = mip * FACE_COUNT + cubeMapFaceIndex;
				 FRHI::RHI_UpdateSubresource(
					finalCubemapTexture, D3D11CalcSubresource(mip, cubeMapFaceIndex, mipLevels)
					, nullptr //&box
					, pSubresourceData[index].pSysMem			// data
					, pSubresourceData[index].SysMemPitch		// row pitch
					, pSubresourceData[index].SysMemSlicePitch	// depth pitch
				);
			}
		}
	}

	// return param
	Texture* cubemapOut = new Texture;;
	cubemapOut->_srv = cubeMapSRV;
	cubemapOut->_name = "todo:Skybox file name";
	cubemapOut->_tex2D = finalCubemapTexture;
	cubemapOut->_height = texDesc.Height;
	cubemapOut->_width = texDesc.Width;
	cubemapOut->_id = static_cast<int>(TextureManage::Get()->Size());
	FString out;
	//cubeMap使用所有6张图片拼接作为name
	for (size_t i = 0; i < textureFiles.size(); i++)
	{
		out = out + textureFiles[i];
	}
	TextureManage::Get()->Add(out,cubemapOut);
	return cubemapOut->_id;
}

Texture* Texture::CreateHDRTexture(const FString& texFileName)
{
	// cache lookup, return early if the texture already exists
	auto& mTextures = TextureManage::Get()->GetArray();
	auto found = mTextures.find(texFileName);
	if (found != mTextures.end())
	{
		return (found->second);
	}

	FString path =texFileName;
	int width = 0;
	int height = 0;
	int numComponents = 0;
	Log::Get()->Info(" create HDR Texture from data: %s" + path);
	float* data = stbi_loadf(path.GetString().c_str(), &width, &height, &numComponents, 4);

	if (!data)
	{
		Log_Error("Cannot load HDR Texture: %s" + path);
		return nullptr;
	}

	TextureDesc texDesc = {};
	texDesc.width = width;
	texDesc.height = height;
	texDesc.format = EImageFormat::RGBA32F;
	texDesc.texFileName = texFileName;
	texDesc.pData = data;
	texDesc.dataPitch = sizeof(FVector4D) * width;
	texDesc.mipCount = 1;
	texDesc.bGenerateMips = false;

	Texture* newTex = CreateTexture2D(texDesc);
	if (newTex == nullptr)
	{
		Log_Error("Cannot create HDR Texture from data: %s" + path);
	}
	stbi_image_free(data);
	return newTex;
}



Texture* Texture::CreateTexture2D(const TextureDesc& texDesc)
{
	
	auto& mTextures = TextureManage::Get()->GetArray();
	auto found = mTextures.find(texDesc.texFileName);
	if (found != mTextures.end())
	{
		return (found->second);
	}


	Texture* tex = new Texture;
	tex->_width = texDesc.width;
	tex->_height = texDesc.height;
	tex->_name = texDesc.texFileName;


	// check multi sampling quality level
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173072(v=vs.85).aspx
	//UINT maxMultiSamplingQualityLevel = 0;
	//m_device->CheckMultisampleQualityLevels(, , &maxMultiSamplingQualityLevel);
	//---


	// Texture2D Resource
	UINT miscFlags = 0;
	miscFlags |= texDesc.bIsCubeMap ? D3D11_RESOURCE_MISC_TEXTURECUBE : 0;
	miscFlags |= texDesc.bGenerateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	UINT arrSize = texDesc.arraySize;
	const bool bIsTextureArray = texDesc.arraySize > 1;
	arrSize = texDesc.bIsCubeMap ? 6 * arrSize : arrSize;

	D3D11_TEXTURE2D_DESC desc = {};
	desc.Format = (DXGI_FORMAT)texDesc.format;
	desc.Height = max(texDesc.height, 1);
	desc.Width = max(texDesc.width, 1);
	desc.ArraySize = arrSize;
	desc.MipLevels = texDesc.mipCount;
	desc.SampleDesc = { 1, 0 };
	desc.BindFlags = texDesc.usage;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = static_cast<D3D11_CPU_ACCESS_FLAG>(texDesc.cpuAccessMode);
	desc.MiscFlags = miscFlags;

	D3D11_SUBRESOURCE_DATA dataDesc = {};
	D3D11_SUBRESOURCE_DATA* pDataDesc = nullptr;
	if (texDesc.pData)
	{
		dataDesc.pSysMem = texDesc.pData;
		dataDesc.SysMemPitch = texDesc.dataPitch;
		dataDesc.SysMemSlicePitch = texDesc.dataSlicePitch;
		pDataDesc = &dataDesc;
	}
	FRHI::RHI_CreateTexture2D(&desc, pDataDesc, &tex->_tex2D);

#if defined(_DEBUG) || defined(PROFILE)

#endif

	// Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = (DXGI_FORMAT)texDesc.format;
	switch (texDesc.format)
	{
		// caution: if initializing for depth texture, and the depth texture
		//			has stencil defined (d24s8), we have to check for 
		//			DXGI_FORMAT_R24_UNORM_X8_TYPELESS vs R32F
	case EImageFormat::R24G8:
		srvDesc.Format = (DXGI_FORMAT)EImageFormat::R24_UNORM_X8_TYPELESS;
		break;
	case EImageFormat::R32:
		srvDesc.Format = (DXGI_FORMAT)EImageFormat::R32F;
		break;
	}

	if (texDesc.bIsCubeMap)
	{
		if (bIsTextureArray)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
			srvDesc.TextureCubeArray.NumCubes = arrSize / 6;
			srvDesc.TextureCubeArray.MipLevels = texDesc.mipCount;
			srvDesc.TextureCubeArray.MostDetailedMip = 0;
			srvDesc.TextureCubeArray.First2DArrayFace = 0;
			FRHI::RHI_CreateShaderResourceView(tex->_tex2D, &srvDesc, &tex->_srv);
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			srvDesc.TextureCube.MipLevels = texDesc.mipCount;
			srvDesc.TextureCube.MostDetailedMip = 0;
			FRHI::RHI_CreateShaderResourceView(tex->_tex2D, &srvDesc, &tex->_srv);
		}
#if _DEBUG
		if (!texDesc.texFileName.Empty())
		{

		}
#endif
	}
	else
	{
		if (bIsTextureArray)
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			srvDesc.Texture2DArray.MipLevels = texDesc.mipCount;
			srvDesc.Texture2DArray.MostDetailedMip = 0;

			tex->_srvArray.resize(desc.ArraySize, nullptr);
			tex->_depth = desc.ArraySize;
			for (unsigned i = 0; i < desc.ArraySize; ++i)
			{
				srvDesc.Texture2DArray.FirstArraySlice = i;
				srvDesc.Texture2DArray.ArraySize = desc.ArraySize - i;
				FRHI::RHI_CreateShaderResourceView(tex->_tex2D, &srvDesc, &tex->_srvArray[i]);
				if (i == 0)
					tex->_srv = tex->_srvArray[i];
#if _DEBUG
				if (!texDesc.texFileName.Empty())
				{

				}
#endif
			}

			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.Format = (DXGI_FORMAT)texDesc.format;
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
				uavDesc.Texture2D.MipSlice = 0;

				tex->_uavArray.resize(desc.ArraySize, nullptr);
				tex->_depth = desc.ArraySize;
				for (unsigned i = 0; i < desc.ArraySize; ++i)
				{
					uavDesc.Texture2DArray.FirstArraySlice = i;
					uavDesc.Texture2DArray.ArraySize = desc.ArraySize - i;
					FRHI::RHI_CreateUnorderedAccessView(tex->_tex2D, &uavDesc, &tex->_uavArray[i]);
					if (i == 0)
						tex->_uav = tex->_uavArray[i];
				}
			}
		}
		else
		{
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = texDesc.mipCount;
			srvDesc.Texture2D.MostDetailedMip = 0;
			FRHI::RHI_CreateShaderResourceView(tex->_tex2D, &srvDesc, &tex->_srv);

			if (desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
			{
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.Format = (DXGI_FORMAT)texDesc.format;
				uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
				uavDesc.Texture2D.MipSlice = 0;

				FRHI::RHI_CreateUnorderedAccessView(tex->_tex2D, &uavDesc, &tex->_uav);
			}
		}
	}

	//之前用数组存储 所以判断下标是否等于-1 如果等于-1直接覆盖内存不需要重新创建
// 	INT retID = -1;
// 	auto itTex = std::find_if(mTextures.begin(), mTextures.end(), [](const Texture* tex1) {return tex1->_id == -1; });
// 	if (itTex != mTextures.end())
// 	{
// 		*itTex = tex;
// 		(*itTex)->_id = static_cast<int>((int)std::distance(mTextures.begin(), itTex));
// 		retID = (*itTex)->_id;
// 	}
//	else
	{
		tex->_id = static_cast<int>(mTextures.size());
		std::pair<TMap<FString, Texture*>::iterator, bool> success = mTextures.insert(std::pair<FString, Texture*>(texDesc.texFileName, tex));
		if (success.second == false)
		{
			Log_Error("Texture2D Craete Fail Path=" + texDesc.texFileName);
			return nullptr;
		}
	}
	return tex;
}

TextureManage* TextureManage::Get()
{
	if (!TextureManage::Instance)
	{
		TextureManage::Instance = new TextureManage();
	}
	return TextureManage::Instance;
}

TextureManage::~TextureManage()
{
	TMap<FString, Texture*>::iterator iter = mTextures.begin();
	while (iter != mTextures.end()) {
		if (iter->second)
		{
			delete iter->second;
			iter->second = nullptr;
		}
		iter++;
	}
}

void TextureManage::Add(const FString& name,Texture* texture)
{
	if (texture==nullptr)
	{
		auto& mTextures = TextureManage::Get()->GetArray();
		auto found = mTextures.find(name);
		if (found == mTextures.end())
		{
			std::pair<TMap<FString, Texture*>::iterator, bool> success = mTextures.insert(std::pair<FString, Texture*>(name, texture));
			if (success.second == false)
			{
				Log_Error("Texture2D Craete Fail Path=" + name);
			}
		}
	}
	else
	{
		Log_Error("Texture2D Add Fail Path=" + name+"Texture Address==null");
	}

}

Texture* TextureManage::FindIndex(const FString& name)
{
	auto& mTextures = TextureManage::Get()->GetArray();
	auto found = mTextures.find(name);
	if (found != mTextures.end())
	{
		return (found->second);
	}
	else
	{
		return nullptr;
	}
}

int TextureManage::Size()
{
	return  static_cast<int>(mTextures.size());
}

TMap<FString, Texture*>& TextureManage::GetArray()
{
	return mTextures;
}

ComPtr<ID3D11SamplerState> TextureManage::LinerSamplerState_WRAP = nullptr;//线性采样
ComPtr<ID3D11SamplerState>  TextureManage::LinerSamplerState_Clamp = nullptr;//三线性采样
ComPtr<ID3D11SamplerState>  TextureManage::LinerSamplerState_NEVER = nullptr;//三线性采样
ComPtr<ID3D11SamplerState> TextureManage::PointSamplerState_WRAP = nullptr;//点采样
ComPtr<ID3D11SamplerState>  TextureManage::PointSamplerState_Clamp = nullptr;//点采样

void TextureManage::Destroy()
{
	if (TextureManage::Instance)
	{
		delete TextureManage::Instance;
		TextureManage::Instance = nullptr;
	}
}

TextureManage::TextureManage()
{
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		FRHI::RHI_CreateSamplerState(&sampDesc, LinerSamplerState_WRAP.GetAddressOf());
	}

	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		FRHI::RHI_CreateSamplerState(&sampDesc, LinerSamplerState_Clamp.GetAddressOf());
	}

	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		FRHI::RHI_CreateSamplerState(&sampDesc, PointSamplerState_WRAP.GetAddressOf());
	}

	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		//sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		FRHI::RHI_CreateSamplerState(&sampDesc, PointSamplerState_Clamp.GetAddressOf());
	}
	{
		D3D11_SAMPLER_DESC sampDesc;
		ZeroMemory(&sampDesc, sizeof(sampDesc));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		FRHI::RHI_CreateSamplerState(&sampDesc, LinerSamplerState_NEVER.GetAddressOf());
	}
}

Texture::Texture() :
	_srv(nullptr),	// assigned and deleted by renderer
	_uav(nullptr),	// assigned and deleted by renderer
	_tex2D(nullptr),
	_width(0),
	_height(0),
	_depth(0),
	_name(""),
	_id(-1)
{}

Texture::~Texture()
{
	Release();
}

bool Texture::InitializeTexture2D(const D3D11_TEXTURE2D_DESC& descriptor, bool initializeSRV)
{
	 FRHI::RHI_CreateTexture2D(&descriptor, nullptr, &this->_tex2D);
#if defined(_DEBUG)
	if (!this->_name.Empty())
	{
		this->_tex2D->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(this->_name.Len()), this->_name.GetData());
	}
#endif
	this->_width = descriptor.Width;
	this->_height = descriptor.Height;

	if (initializeSRV)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = descriptor.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;	// array maybe? check descriptor.
		srvDesc.Texture2D.MipLevels = descriptor.MipLevels;
		FRHI::RHI_CreateShaderResourceView(this->_tex2D, &srvDesc, &this->_srv);
#if defined(_DEBUG) 
		if (!this->_name.Empty())
		{
			this->_srv->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(this->_name.Len()), this->_name.GetData());
		}
#endif
	}
	return true;
}

template<class T>
void ReleaseResources(std::vector<T*>& resources, T*& pResourceView)
{
	if (pResourceView)
	{
		pResourceView->Release();
		pResourceView = nullptr;
	}
	if (resources.size() > 0)
	{
		for (size_t i = 0; i < resources.size(); ++i)
		{
			if (resources[i])
			{
				resources[i]->Release();
				resources[i] = nullptr;
			}
		}
	}
}

void Texture::Release()
{
	ReleaseResources(_srvArray, _srv);
	ReleaseResources(_uavArray, _uav);
	if (_tex2D)
	{
		_tex2D->Release();
		_tex2D = nullptr;
	}
	_width = _height = _depth = 0;
	_name = "";
	_id = -1;

}
