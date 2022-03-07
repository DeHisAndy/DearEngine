#pragma once
#include <DirectXMath.h>
#include <string>
#include "Vector.h"
#include "String.h"
class FVector4D
{
public:
	__forceinline DirectX::XMFLOAT4 XMFloat4() const
	{
		DirectX::XMFLOAT4 _XMFloat4;
		_XMFloat4.x = this->X;
		_XMFloat4.y = this->Y;
		_XMFloat4.z = this->Z;
		_XMFloat4.w = this->W;
		return _XMFloat4;
	}
	__forceinline DirectX::XMVECTOR XMVERCTOR() const
	{
		DirectX::XMVECTOR xmvectorx= DirectX::XMVECTOR();
		DirectX::XMFLOAT4 matrix =this-> XMFloat4();
		DirectX::XMStoreFloat4(&matrix, xmvectorx);
		return xmvectorx;
	}

    FString  To_String()
	{

		return "( X:" + std::to_string(X) + "," + "Y:" + std::to_string(Y) + "," + "Z:" + std::to_string(Z) + "," + "W:" + std::to_string(W) + " )";
	}
public:
	float X;
	float Y;
	float Z;
	float W;
	FVector4D(float x, float y, float z) {
		this->X = x;
		this->Y = y;
		this->Z = z;
		this->W = 1.0f;
	}
	FVector4D() { X = 0.f; Y = 0.f; Z = 0.f, W = 0.f; }
	FVector4D(float x, float y, float z, float w)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
		this->W = w;
	}
	FVector4D(FVector fv, float w=0.f)
	{
		this->X = fv.X;
		this->Y = fv.Y;
		this->Z = fv.Z;
		this->W = w;
	}
	FVector4D(DirectX::XMFLOAT3 fv, float w=0.f)
	{
		this->X = fv.x;
		this->Y = fv.y;
		this->Z = fv.z;
		this->W = w;
	}

};




