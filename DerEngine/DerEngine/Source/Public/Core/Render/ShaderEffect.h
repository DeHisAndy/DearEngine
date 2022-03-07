#pragma once
#include <wrl/client.h>
#include <map>
#include <string>
#include "ShaderCompilerWorker.h"
#include <d3dx11effect.h>

template <class T>
using ComPtr = Microsoft::WRL::ComPtr<T>;
class UShaderEffect :public UShaderCompilerWorker
{
public:
	typedef UShaderCompilerWorker Super;
	struct EffectSturct
	{
		ComPtr < ID3DX11Effect> Effect;
	};
	UShaderEffect();
	virtual ~UShaderEffect();
public:
	std::map<FString, EffectSturct*>& GetData();
	virtual void Init();
	void virtual CompileHlSL();
protected:
	std::map<FString, EffectSturct*> effectStruct;

};