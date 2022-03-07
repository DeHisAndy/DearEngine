#pragma once
#include "../Object/Object/Object.h"
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include "../Vector/Vector4D.h"
#include "../Vector/Vector2D.h"
class UShaderCompilerWorker :public UObject
{
public:
	typedef UObject Super;
	struct Shader_InputInfo
	{
		Shader_InputInfo() {};
		Shader_InputInfo(float x, float y, float z, float r, float g, float b, float a,float ux,float uy) : position(x, y, z), color(r, g, b, a) ,uv(ux, uy){};
		Shader_InputInfo(FVector pos, FVector4D colors) {
			position = pos;
			color = colors;
		};
		FVector position;
		FVector4D color;
		FVector normal;
		FVector4D tangent;
		FVector2D uv;
		static D3D11_INPUT_ELEMENT_DESC Inputlayout[5];
	};
	UShaderCompilerWorker();
	virtual ~UShaderCompilerWorker();
	virtual void Init();
	void virtual CompileHlSL();
public:
	TArray<class FString>& GetShaderArry();
	void AddShaderName(class FString pathName);
	void RemoveShaderName(class FString pathName);
	void ClearShaderName();
private:
	TArray<class FString> shaderName;
};