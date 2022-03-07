#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <string>
#include "MathType.h"
#include "..\FunctionLibrary\Math.h"
using namespace DirectX;
class FVector4D;
class FVector
{
public:
	float X;
	float Y;
	float Z;

public:

	/** A zero vector (0,0,0) */
	static  const FVector ZeroVector;

	/** One vector (1,1,1) */
	static  const FVector OneVector;

	/** Unreal up vector (0,0,1) */
	static  const FVector UpVector;

	/** Unreal down vector (0,0,-1) */
	static  const FVector DownVector;

	/** Unreal forward vector (1,0,0) */
	static  const FVector ForwardVector;

	/** Unreal backward vector (-1,0,0) */
	static  const FVector BackwardVector;

	/** Unreal right vector (0,1,0) */
	static  const FVector RightVector;

	/** Unreal left vector (0,-1,0) */
	static  const FVector LeftVector;

	/** Unit X axis vector (1,0,0) */
	static  const FVector XAxisVector;

	/** Unit Y axis vector (0,1,0) */
	static  const FVector YAxisVector;

	/** Unit Z axis vector (0,0,1) */
	static  const FVector ZAxisVector;
public:
	FORCEINLINE FVector() {

		Zero();
	}
	FORCEINLINE FVector(float InX, float InY, float InZ)
		: X(InX), Y(InY), Z(InZ) {}
	FORCEINLINE FVector(DirectX::XMFLOAT3 _XMFLOAT3)
	{
		this->X = _XMFLOAT3.x;
		this->Y = _XMFLOAT3.y;
		this->Z = _XMFLOAT3.z;
	}
	FORCEINLINE FVector(DirectX::XMVECTOR _XMVECTOR)
	{
		DirectX::XMFLOAT3 _XMFloat3;
		DirectX::XMStoreFloat3(&_XMFloat3, _XMVECTOR);
		this->X = _XMFloat3.x;
		this->Y = _XMFloat3.y;
		this->Z = _XMFloat3.z;
	}

	//向量点乘
	FORCEINLINE float operator|(const FVector& V) const
	{
		return X * V.X + Y * V.Y + Z * V.Z;
	}
	//向量叉乘
	FORCEINLINE FVector operator^(const FVector& V) const
	{
		return FVector
		(
			Y * V.Z - Z * V.Y,
			Z * V.X - X * V.Z,
			X * V.Y - Y * V.X
		);
	}
	FORCEINLINE FVector operator*(float A)const
	{
		return FVector(this->X*A, this->Y*A, this->Z*A);

	}
	FORCEINLINE FVector operator*(FVector A)const
	{
		return FVector(this->X*A.X, this->Y*A.Y, this->Z*A.Z);

	}
	FORCEINLINE FVector operator/(float A)const
	{
		return FVector(this->X / A, this->Y / A, this->Z / A);

	}
	FORCEINLINE FVector operator/(FVector A)const
	{
		return FVector(this->X / A.X, this->Y / A.Y, this->Z / A.Z);

	}
	FORCEINLINE FVector operator+(float A)const
	{
		return FVector(this->X + A, this->Y + A, this->Z + A);
	}
	FORCEINLINE FVector operator+(FVector A)const
	{
		return FVector(this->X + A.X, this->Y + A.Y, this->Z + A.Z);
	}
	FORCEINLINE FVector operator-(float A)const
	{
		return FVector(this->X + -A, this->Y + -A, this->Z + -A);
	}
	FORCEINLINE FVector operator-(FVector A)const
	{
		return FVector(this->X + -A.X, this->Y + -A.Y, this->Z + -A.Z);
	}
	FORCEINLINE FVector operator-() const
	{
		return FVector(-X, -Y, -Z);
	}
	FORCEINLINE FVector(float InF)
		: X(InF), Y(InF), Z(InF)
	{
	}
	float& operator[](int32 Index);
	FORCEINLINE static float DotProduct(const FVector& A, const FVector& B)
	{
		return A | B;
	}
	FORCEINLINE float DotProduct(const FVector& A)
	{
		return *this | A;
	}
	FORCEINLINE  FVector CrossProduct(const FVector& A)
	{
		return *this ^ A;
	}
	FORCEINLINE static FVector CrossProduct(const FVector& A, const FVector& B)
	{
		return A ^ B;
	}
	FORCEINLINE float SizeSquared() const
	{
		return X * X + Y * Y + Z * Z;
	}
	FORCEINLINE FVector GetSafeNormal(float Tolerance = SMALL_NUMBER) const
	{
		const float SquareSum = X * X + Y * Y + Z * Z;

		// Not sure if it's safe to add tolerance in there. Might introduce too many errors
		if (SquareSum == 1.f)
		{
			return *this;
		}
		else if (SquareSum < Tolerance)
		{
			return FVector::ZeroVector;
		}
		const float Scale = FMath::InvSqrt(SquareSum);
		return FVector(X * Scale, Y * Scale, Z * Scale);

	}
	bool Normalize(float Tolerance = SMALL_NUMBER);
	FORCEINLINE void Zero()
	{
		X = 0.f;
		Y = 0.f;
		Z = 0.f;

	}
	FORCEINLINE DirectX::XMFLOAT3 XMFloat3() const
	{
		DirectX::XMFLOAT3 _XMFloat3;
		_XMFloat3.x = this->X;
		_XMFloat3.y = this->Y;
		_XMFloat3.z = this->Z;
		return _XMFloat3;
	}
	FORCEINLINE DirectX::XMVECTOR XMVECTOR() const
	{
		DirectX::XMFLOAT3 _XMFloat3;
		_XMFloat3.x = this->X;
		_XMFloat3.y = this->Y;
		_XMFloat3.z = this->Z;
		return XMLoadFloat3(&_XMFloat3);
	}
	FORCEINLINE DirectX::XMVECTOR Set(DirectX::XMVECTOR A)
	{
		DirectX::XMFLOAT3 _XMFloat3;
		XMStoreFloat3(&_XMFloat3, A);
		this->X = _XMFloat3.x;
		this->Y = _XMFloat3.y;
		this->Z = _XMFloat3.z;
	}
	FORCEINLINE std::string To_String() const
	{
		std::string str;
		str = "( X:" + std::to_string(X) + "," + "Y:" + std::to_string(Y) + "," + "Z:" + std::to_string(Z) + ")";
		return str;
	}
	FORCEINLINE bool Equals(FVector V, float Tolerance= 1.e-4f) {
	return	std::abs(X - V.X) <= Tolerance && std::abs(Y - V.Y) <= Tolerance && std::abs(Z - V.Z) <= Tolerance;
	}
	FORCEINLINE bool ContainsNaN() const
	{
		return (!FMath::IsFinite(X) ||
			!FMath::IsFinite(Y) ||
			!FMath::IsFinite(Z));
	}

	bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const;

};



FORCEINLINE FVector operator*(float Scale, const FVector& V)
{
	return V.operator*(Scale);
}
FORCEINLINE bool FVector::IsNearlyZero(float Tolerance) const
{
	return
		FMath::Abs(X) <= Tolerance
		&& FMath::Abs(Y) <= Tolerance
		&& FMath::Abs(Z) <= Tolerance;
}
FORCEINLINE float& FVector::operator[](int32 Index)
{
	assert(Index >= 0 && Index < 3);
	return (&X)[Index];
}

FORCEINLINE bool FVector::Normalize(float Tolerance)
{
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum > Tolerance)
	{
		const float Scale = FMath::InvSqrt(SquareSum);
		X *= Scale; Y *= Scale; Z *= Scale;
		return true;
	}
	return false;
}