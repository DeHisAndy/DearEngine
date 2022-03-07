#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include<cmath>
#include "Vector.h"
using namespace  DirectX;
// todo vec.h
class FVector2D
{
public:
	float X;
	float Y;
public:
	static const XMVECTOR Zero;
	static const XMVECTOR Up;
	static const XMVECTOR Down;
	static const XMVECTOR Left;
	static const XMVECTOR Right;

	static const FVector2D ZeroF2;
	static const FVector2D UpF2;
	static const FVector2D DownF2;
	static const FVector2D LeftF2;
	static const FVector2D RightF2;

	FORCEINLINE	FVector2D();
	FORCEINLINE	FVector2D(const XMVECTOR& v_in);
	FORCEINLINE	FVector2D(const FVector& v_in);
	FORCEINLINE FVector2D(float x, float y);
	FORCEINLINE	FVector2D(int x, int y);
	FORCEINLINE	FVector2D(unsigned x, unsigned y);
	FORCEINLINE	FVector2D(float x);
	FORCEINLINE	FVector2D(const XMFLOAT3& f3);
	FORCEINLINE	FVector2D(const XMFLOAT2& f2);
	FORCEINLINE std::string ToString();

	FORCEINLINE DirectX::XMFLOAT2 XMFloat2() const
	{
		DirectX::XMFLOAT2 _XMFloat2;
		_XMFloat2.x = X;
		_XMFloat2.y = Y;
		return _XMFloat2;
	}
	bool operator ==(const FVector2D& Vector) const;
	inline FVector2D operator +=(const FVector2D& v);


	// 	void normalize();
	// 	const FVector2D normalized() const;
};

FORCEINLINE FVector2D::FVector2D()
{
	X = 0.f;
	Y = 0.f;
}

FORCEINLINE FVector2D::FVector2D(const XMVECTOR& v_in)
{
	DirectX::XMFLOAT2 _v;
	XMStoreFloat2(&_v, v_in);
	X = _v.x;
	Y = _v.y;
}

FORCEINLINE FVector2D::FVector2D(const FVector& v_in)
{
	X = v_in.X;
	Y = v_in.Y;
}

FORCEINLINE FVector2D::FVector2D(float x, float y)
{
	X = x;
	Y = y;
}

FORCEINLINE FVector2D::FVector2D(int x, int y)
{
	X= static_cast<float>(x);
	Y = static_cast<float>(y);
}

FORCEINLINE FVector2D::FVector2D(unsigned x, unsigned y)
{
	X = static_cast<float>(x);
	Y = static_cast<float>(y);
}

FORCEINLINE FVector2D::FVector2D(float x)
{
	X = x;
	Y = x;
}

FORCEINLINE FVector2D::FVector2D(const XMFLOAT3& f3)
{
	X = f3.x;
    Y = f3.x;
}

FORCEINLINE FVector2D::FVector2D(const XMFLOAT2& f2)
{
	X = f2.x;
	X = f2.y;
}


FORCEINLINE std::string FVector2D::ToString()
{
	std::string out = "( " + std::to_string(X) + std::to_string(Y) + " )";
	return out;
}

inline bool FVector2D::operator==(const FVector2D& Vector) const
{

	if (std::fabs(X - Vector.X) < KINDA_SMALL_NUMBER && std::fabs(Y - Vector.Y) < KINDA_SMALL_NUMBER)
	{
		return true;
	}
	return false;
}

inline FVector2D  FVector2D::operator+=(const FVector2D& v)
{
	FVector2D(X + v.X,Y + v.Y);
}
