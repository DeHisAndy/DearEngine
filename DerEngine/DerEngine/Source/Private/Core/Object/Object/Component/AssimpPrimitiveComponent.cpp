#include "../../../Core/Object/Object/Component/AssimpPrimitiveComponent.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../../Core/Object/Object/Component/PrimitiveComponent.h"
#include "../../../Core/Render/RHI.h"
#include "../../../Core/Render/ShaderEffect.h"
#include "../../../Core/Object/Object/TextureManage.h"
#include "../../../Core/FunctionLibrary/GamePlayStatics.h"
#include "../../../Core/GameMode/Pawn.h"
#include "../../../Core/Engine/GameViewport.h"

using namespace Assimp;
#define RANGE(c) std::begin(c), std::end(c)

void ProcessMesh(aiMesh* mesh, const aiScene* scene, UPrimitiveComponent* MeshPrimitiveComponent)
{
	if (!MeshPrimitiveComponent)
	{
		Log_Error("AssimpPrimitiveComponent==NULL");
		return;
	}

	UPrimitiveComponent::FMeshPrimitiveComponent& nowData = MeshPrimitiveComponent->AddDefultPrimitiveData();
	nowData.name = mesh->mName.C_Str();
	FVector MinBound;
	FVector MaxBound;
	// Walk through each of the mesh's vertices
	UShaderCompilerWorker::Shader_InputInfo Vert;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
// 		if (MinBound.X > mesh->mVertices[i].x)
// 		{
// 			MinBound.X = mesh->mVertices[i].x;
// 		}
// 		if (MinBound.Y > mesh->mVertices[i].y)
// 		{
// 			MinBound.Y = mesh->mVertices[i].y;
// 		}
// 		if (MinBound.Z > mesh->mVertices[i].z)
// 		{
// 			MinBound.Z = mesh->mVertices[i].z;
// 		}
// 
// 		if (MaxBound.X < mesh->mVertices[i].x)
// 		{
// 			MaxBound.X = mesh->mVertices[i].x;
// 		}
// 		if (MaxBound.Y < mesh->mVertices[i].y)
// 		{
// 			MaxBound.Y = mesh->mVertices[i].y;
// 		}
// 		if (MaxBound.Z < mesh->mVertices[i].z)
// 		{
// 			MaxBound.Z = mesh->mVertices[i].z;
// 		}
		// POSITIONS
		Vert.position = FVector(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);
		// NORMALS
		if (mesh->mNormals)
		{
			Vert.normal = FVector(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
		}

		// TEXTURE COORDINATES
		// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
		// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
		Vert.uv = mesh->mTextureCoords[0]
			? FVector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
			: FVector2D(0, 0);

		// TANGENT
		if (mesh->mTangents)
		{
			Vert.tangent = FVector4D(
				mesh->mTangents[i].x,
				mesh->mTangents[i].y,
				mesh->mTangents[i].z,
				0
			);
		}


		// BITANGENT ( NOT USED )
		// Vert.bitangent = vec3(
		// 	mesh->mBitangents[i].x,
		// 	mesh->mBitangents[i].y,
		// 	mesh->mBitangents[i].z
		// );
		nowData.vertices.push_back(Vert);
	}

	// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		//		HR(face.mNumIndices == 3);
				// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			nowData.indices.push_back(face.mIndices[j]);
	}
	//计算AABB包围盒
	nowData.MinBound = FVector(mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z);
	nowData.MaxBound = FVector(mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z);
	// TODO: mesh name
}

void ProcessNode(aiNode* const pNode, const aiScene* pAiScene, UAssimpPrimitiveComponent* AssimpPrimitiveComponent)
{
	if (!AssimpPrimitiveComponent)
	{
		Log_Error("AssimpPrimitiveComponent==NULL");
		return;
	}
	Log_Info("Loading:"+AssimpPrimitiveComponent->GetMeshPathName() + " , NumMeshs=" + FString::FormatIntToString(pNode->mNumMeshes));
	for (unsigned int i = 0; i < pNode->mNumMeshes; i++)
	{	// process all the node's meshes (if any)
		
		aiMesh* pAiMesh = pAiScene->mMeshes[pNode->mMeshes[i]];
		aiMaterial* material = pAiScene->mMaterials[pAiMesh->mMaterialIndex];
		Log_Info("Loading:" + AssimpPrimitiveComponent->GetMeshPathName() + " meshName=" + pAiMesh->mName.C_Str());
		//	std::vector<int> diffuseMaps = LoadMaterialTextures(pAiScene, material, aiTextureType_DIFFUSE, "texture_diffuse", modelDirectory);
			// 		std::vector<int> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", modelDirectory);
			// 		std::vector<int> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal", modelDirectory);
			// 		std::vector<int> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height", modelDirectory);
			// 		std::vector<int> alphaMaps = LoadMaterialTextures(material, aiTextureType_OPACITY, "texture_alpha", modelDirectory);
			// 		assert(diffuseMaps.size() <= 1);
			// 		assert(normalMaps.size() <= 1);
			// 		assert(specularMaps.size() <= 1);
			// 		assert(heightMaps.size() <= 1);
			// 		assert(alphaMaps.size() <= 1);
			// 		if (!diffuseMaps.empty())	mesh.matrial.diffuseMap[0] = diffuseMaps[0];
			// 		if (!normalMaps.empty())	mesh.matrial.normalMap[0] = normalMaps[0];
			// 		if (!specularMaps.empty())	mesh.matrial.specularMap[0] = specularMaps[0];
			// 		if (!heightMaps.empty())	mesh.matrial.heightMap[0] = heightMaps[0];
			// 		if (!alphaMaps.empty())		mesh.matrial.mask[0] = alphaMaps[0];

		aiString name;
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_NAME, name))
		{
			// we don't store names for materials. probably best to store them in a lookup somewhere,
			// away from the material data.
			//
			// pBRDF->
		}
		aiColor3D color(0.f, 0.f, 0.f);
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
		{
			//mesh.matrial.diffuse = FVector4D(color.r, color.g, color.b);
		}

		aiColor3D specular(0.f, 0.f, 0.f);
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specular))
		{
			//mesh.matrial.specular = FVector4D(specular.r, specular.g, specular.b);
		}

		aiColor3D transparent(0.0f, 0.0f, 0.0f);
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent))
		{

		}

		float opacity = 0.0f;
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_OPACITY, opacity))
		{

			//mesh.matrial._alpha = opacity;
		}

		float shininess = 0.0f;
		if (aiReturn_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess))
		{
			//mesh.matrial._roughness = sqrtf(2.0f / (2.0f + shininess));
		}

		std::string fileName(pAiScene->mRootNode->mName.C_Str());
		std::transform(RANGE(fileName), fileName.begin(), ::tolower);
		TArray<FString> tokens;
		FString::Split(fileName, FString("."), tokens);
		if (!tokens.empty() && (tokens[0] == "ironman" || tokens[0] == "zen_orb"))
		{
			//pBRDF->metalness = 1.0f;
		}


		ProcessMesh(pAiMesh, pAiScene, AssimpPrimitiveComponent);
	}
	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{	// then do the same for each of its children
		ProcessNode(pNode->mChildren[i], pAiScene, AssimpPrimitiveComponent);
	}
}

//加载自带fbx 图片
// std::vector<int> LoadMaterialTextures(const aiScene* pAiScene, aiMaterial* pMaterial, aiTextureType type, const std::string& textureName, const std::string& modelDirectory)
// {
// 	std::vector<int> textures;
// 	for (unsigned int i = 0; i < pMaterial->GetTextureCount(type); i++)
// 	{
// 		aiString str;
// 		pMaterial->GetTexture(type, i, &str);
// 		std::string path = str.C_Str();
// 		{
// 			const aiTexture* texture = pAiScene->GetEmbeddedTexture(path.c_str());
// 			texture->achFormatHint;
// 			textures.push_back(FMath::CreateTextureFromFile(path, modelDirectory, true));
// 		}
// 	}
// 	return textures;
// }
// 

UAssimpPrimitiveComponent::UAssimpPrimitiveComponent()
{

}

UAssimpPrimitiveComponent::UAssimpPrimitiveComponent(TArray<FMeshPrimitiveComponent>& arrayData, FString shaderName/*=""*/)
{
	SetArrayMeshPrimitiveComponent(arrayData);
	SetEffectShader(shaderName);
	//调用父类初始化参数
	Super::ImportModel("Defulat");
			InitBuffer();
}

UAssimpPrimitiveComponent::UAssimpPrimitiveComponent(FString meshPath, FString shaderName /*= FString()*/)
{
	if (!meshPath.Empty()|| shaderName.Empty())
	{

		TMap<FString, UPrimitiveComponent*>::iterator key = UPrimitiveComponent::GetProyxRenderData().find(meshPath);
		if (key!= UPrimitiveComponent::GetProyxRenderData().end())
		{
			if (key->second)
			{
				Log_Info("Already Load mesh=" + meshPath + " So Dirct Copy Memory")
					//内存拷贝
				key->second->CopyPrimitiveData(this);
				//调用父类初始化参数
				Super::ImportModel(meshPath);
				//设置着色器参数
				SetEffectShader(shaderName);
			}
			else
			{
				Log_Error(meshPath+"load fail")
			}

		}
		else
		{
			//本地加载模型
			ImportModel(meshPath);
			SetEffectShader(shaderName);
		//更新已经加载的模型路径和资源到容器里面 暂时不用
		//	UpdataMeshPath();
		}
		//初始化buffer
		InitBuffer();
	}

}

UAssimpPrimitiveComponent::~UAssimpPrimitiveComponent()
{

}

void UAssimpPrimitiveComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAssimpPrimitiveComponent::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
	
void UAssimpPrimitiveComponent::Draw()
{
	FMatrix g_world = GetComponentWorldTransform().ToMatrixWithScale();
	FMatrix g_View = UGamePlayStatics::GetPlayerPawn()->GetActorTransform().ToMatrixWithScale().Inverse();
	FMatrix g_Proj = UGamePlayStatics::GetGameViewport()->GetPerspectiveMatirx();
	SetMaterixParameterValue("g_World", g_world);
	SetMaterixParameterValue("g_View", g_View);
	SetMaterixParameterValue("g_Proj", g_Proj);
	Super::Draw();
}

void UAssimpPrimitiveComponent::BeginDestroy()
{
	Super::BeginDestroy();
}

void UAssimpPrimitiveComponent::OnRegister()
{
	Super::OnRegister();
}

void UAssimpPrimitiveComponent::OnUnregister()
{
	Super::OnUnregister();
}

void UAssimpPrimitiveComponent::ImportModel(const FString& modelPath)
{
	Importer importer;
	Log_Info(modelPath.GetString());
	const aiScene* scene = importer.ReadFile(modelPath.GetString(), aiProcess_Triangulate
		| aiProcess_CalcTangentSpace
		| aiProcess_MakeLeftHanded
		| aiProcess_FlipUVs
		| aiProcess_FlipWindingOrder
		| aiProcess_TransformUVCoords
		| aiProcess_FixInfacingNormals
		| aiProcess_JoinIdenticalVertices
		| aiProcess_GenSmoothNormals);
	HR(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode);
	if (!scene)
	{
		Log::Get()->Info("load mesh Error: " + modelPath);
		Log_Error(importer.GetErrorString());
	}
	else
	{
		Log::Get()->Info("loading mesh: " + modelPath);
		ProcessNode(scene->mRootNode, scene,this);
		Super::ImportModel(modelPath);
	}

}