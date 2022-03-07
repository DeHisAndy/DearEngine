#include "..\..\..\Public\Core\Vector\Matrix.h"
#include "..\..\..\Public\Core\Vector\Vector.h"
#include "..\..\..\Public\ThirdParty\SSE\MathSSE.h"
#include "..\..\..\Public\Core\Vector\Vector4D.h"
#include "..\..\..\Public\Core\FunctionLibrary\KismetSystemLibrary.h"
#include "..\..\..\Public\Core\Vector\Rotator.h"
#include "..\..\..\Public\Core\Vector\Quat.h"
const FMatrix FMatrix::Identity(FVector4D(1, 0, 0, 0), FVector4D(0, 1, 0, 0), FVector4D(0, 0, 1, 0), FVector4D(0, 0, 0, 1));
 FMatrix::FMatrix()
{

}
 FMatrix::FMatrix(EForceInit)
 {
	 memset(this, 0, sizeof(*this));
 }
 FMatrix::FMatrix(const FVector4D& InX, const FVector4D& InY, const FVector4D& InZ, const FVector4D& InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = InX.W;
	M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = InY.W;
	M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = InZ.W;
	M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = InW.W;
}

 FMatrix::FMatrix(const FVector& InX, const FVector& InY, const FVector& InZ, const FVector& InW)
{
	M[0][0] = InX.X; M[0][1] = InX.Y;  M[0][2] = InX.Z;  M[0][3] = 0.0f;
	M[1][0] = InY.X; M[1][1] = InY.Y;  M[1][2] = InY.Z;  M[1][3] = 0.0f;
	M[2][0] = InZ.X; M[2][1] = InZ.Y;  M[2][2] = InZ.Z;  M[2][3] = 0.0f;
	M[3][0] = InW.X; M[3][1] = InW.Y;  M[3][2] = InW.Z;  M[3][3] = 1.0f;
}


 void FMatrix::SetIdentity()
{
	M[0][0] = 1; M[0][1] = 0;  M[0][2] = 0;  M[0][3] = 0;
	M[1][0] = 0; M[1][1] = 1;  M[1][2] = 0;  M[1][3] = 0;
	M[2][0] = 0; M[2][1] = 0;  M[2][2] = 1;  M[2][3] = 0;
	M[3][0] = 0; M[3][1] = 0;  M[3][2] = 0;  M[3][3] = 1;
}


 void FMatrix::operator*=(const FMatrix& Other)
{
	VectorMatrixMultiply(this, this, &Other);
}


 FMatrix FMatrix::operator*(const FMatrix& Other) const
{
	FMatrix Result;
	VectorMatrixMultiply(&Result, this, &Other);
	return Result;
}


 FMatrix	FMatrix::operator+(const FMatrix& Other) const
{
	FMatrix ResultMat;

	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			ResultMat.M[X][Y] = M[X][Y] + Other.M[X][Y];
		}
	}

	return ResultMat;
}

 void FMatrix::operator+=(const FMatrix& Other)
{
	*this = *this + Other;
}

 FMatrix	FMatrix::operator*(float Other) const
{
	FMatrix ResultMat;

	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			ResultMat.M[X][Y] = M[X][Y] * Other;
		}
	}

	return ResultMat;
}

 void FMatrix::operator*=(float Other)
{
	*this = *this * Other;
}

// Comparison operators.

 bool FMatrix::operator==(const FMatrix& Other) const
{
	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			if (M[X][Y] != Other.M[X][Y])
			{
				return false;
			}
		}
	}

	return true;
}

// Error-tolerant comparison.
 bool FMatrix::Equals(const FMatrix& Other, float Tolerance/*=KINDA_SMALL_NUMBER*/) const
{
	for (int32 X = 0; X < 4; X++)
	{
		for (int32 Y = 0; Y < 4; Y++)
		{
			if (FMath::Abs(M[X][Y] - Other.M[X][Y]) > Tolerance)
			{
				return false;
			}
		}
	}

	return true;
}

 bool FMatrix::operator!=(const FMatrix& Other) const
{
	return !(*this == Other);
}


// Homogeneous transform.

 FVector4D FMatrix::TransformFVector4(const FVector4D& P) const
{
    FVector4D Result;
	VectorRegister VecP = VectorLoadAligned(&P);
	VectorRegister VecR = VectorTransformVector(VecP, this);
	VectorStoreAligned(VecR, &Result);
	return Result;
}


// Transform position

/** Transform a location - will take into account translation part of the FMatrix. */
 FVector4D FMatrix::TransformPosition(const FVector& V) const
{
	return TransformFVector4(FVector4D(V.X, V.Y, V.Z, 1.0f));
}

/** Inverts the matrix and then transforms V - correctly handles scaling in this matrix. */
 FVector FMatrix::InverseTransformPosition(const FVector& V) const
{
	FMatrix InvSelf = this->InverseFast();
	FVector4D Fl4 = InvSelf.TransformPosition(V);
	return FVector(Fl4.X, Fl4.Y, Fl4.Z);
}

// Transform vector

/**
 *	Transform a direction vector - will not take into account translation part of the FMatrix.
 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT.
 */
 FVector4D FMatrix::TransformVector(const FVector& V) const
{
	return TransformFVector4(FVector4D(V.X, V.Y, V.Z, 0.0f));
}

/** Faster version of InverseTransformVector that assumes no scaling. WARNING: Will NOT work correctly if there is scaling in the matrix. */
 FVector FMatrix::InverseTransformVector(const FVector& V) const
{
	FMatrix InvSelf = this->InverseFast();

	FVector4D Fl4 = InvSelf.TransformVector(V);
	return FVector(Fl4.X, Fl4.Y, Fl4.Z);

}


// Transpose.

 FMatrix FMatrix::GetTransposed() const
{
	FMatrix	Result;

	Result.M[0][0] = M[0][0];
	Result.M[0][1] = M[1][0];
	Result.M[0][2] = M[2][0];
	Result.M[0][3] = M[3][0];

	Result.M[1][0] = M[0][1];
	Result.M[1][1] = M[1][1];
	Result.M[1][2] = M[2][1];
	Result.M[1][3] = M[3][1];

	Result.M[2][0] = M[0][2];
	Result.M[2][1] = M[1][2];
	Result.M[2][2] = M[2][2];
	Result.M[2][3] = M[3][2];

	Result.M[3][0] = M[0][3];
	Result.M[3][1] = M[1][3];
	Result.M[3][2] = M[2][3];
	Result.M[3][3] = M[3][3];

	return Result;
}

// Determinant.

 float FMatrix::Determinant() const
{
	return	M[0][0] * (
		M[1][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
		M[2][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) +
		M[3][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2])
		) -
		M[1][0] * (
			M[0][1] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
			M[2][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2])
			) +
		M[2][0] * (
			M[0][1] * (M[1][2] * M[3][3] - M[1][3] * M[3][2]) -
			M[1][1] * (M[0][2] * M[3][3] - M[0][3] * M[3][2]) +
			M[3][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			) -
		M[3][0] * (
			M[0][1] * (M[1][2] * M[2][3] - M[1][3] * M[2][2]) -
			M[1][1] * (M[0][2] * M[2][3] - M[0][3] * M[2][2]) +
			M[2][1] * (M[0][2] * M[1][3] - M[0][3] * M[1][2])
			);
}

/** Calculate determinant of rotation 3x3 matrix */
 float FMatrix::RotDeterminant() const
{
	return
		M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) -
		M[1][0] * (M[0][1] * M[2][2] - M[0][2] * M[2][1]) +
		M[2][0] * (M[0][1] * M[1][2] - M[0][2] * M[1][1]);
}

// Inverse.
/** Fast path, doesn't check for nil matrices in final release builds */
 FMatrix FMatrix::InverseFast() const
{
	// If we're in non final release, then make sure we're not creating NaNs

	// Check for zero scale matrix to invert
	if (GetScaledAxis(EAxis::X).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Y).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Z).IsNearlyZero(SMALL_NUMBER))
	{
		Log_Error(TEXT("FMatrix::InverseFast(), trying to invert a NIL matrix, this results in NaNs! Use Inverse() instead."));
	}
	else
	{
		const float	Det = Determinant();

		if (Det == 0.0f || !FMath::IsFinite(Det))
		{
			Log_Error(TEXT("FMatrix::InverseFast(), trying to invert a non-invertible matrix, this results in NaNs! Use Inverse() instead."));
		}
	}

	FMatrix Result;
	VectorMatrixInverse(&Result, this);
	return Result;
}

// Inverse.
 FMatrix FMatrix::Inverse() const
{
	FMatrix Result;

	// Check for zero scale matrix to invert
	if (GetScaledAxis(EAxis::X).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Y).IsNearlyZero(SMALL_NUMBER) &&
		GetScaledAxis(EAxis::Z).IsNearlyZero(SMALL_NUMBER))
	{
		// just set to zero - avoids unsafe inverse of zero and duplicates what QNANs were resulting in before (scaling away all children)
		Result = FMatrix::Identity;
	}
	else
	{
		const float	Det = Determinant();

		if (Det == 0.0f)
		{
			Result = FMatrix::Identity;
		}
		else
		{
			VectorMatrixInverse(&Result, this);
		}
	}

	return Result;
}

 FMatrix FMatrix::TransposeAdjoint() const
{
	FMatrix TA;

	TA.M[0][0] = this->M[1][1] * this->M[2][2] - this->M[1][2] * this->M[2][1];
	TA.M[0][1] = this->M[1][2] * this->M[2][0] - this->M[1][0] * this->M[2][2];
	TA.M[0][2] = this->M[1][0] * this->M[2][1] - this->M[1][1] * this->M[2][0];
	TA.M[0][3] = 0.f;

	TA.M[1][0] = this->M[2][1] * this->M[0][2] - this->M[2][2] * this->M[0][1];
	TA.M[1][1] = this->M[2][2] * this->M[0][0] - this->M[2][0] * this->M[0][2];
	TA.M[1][2] = this->M[2][0] * this->M[0][1] - this->M[2][1] * this->M[0][0];
	TA.M[1][3] = 0.f;

	TA.M[2][0] = this->M[0][1] * this->M[1][2] - this->M[0][2] * this->M[1][1];
	TA.M[2][1] = this->M[0][2] * this->M[1][0] - this->M[0][0] * this->M[1][2];
	TA.M[2][2] = this->M[0][0] * this->M[1][1] - this->M[0][1] * this->M[1][0];
	TA.M[2][3] = 0.f;

	TA.M[3][0] = 0.f;
	TA.M[3][1] = 0.f;
	TA.M[3][2] = 0.f;
	TA.M[3][3] = 1.f;

	return TA;
}

// NOTE: There is some compiler optimization issues with WIN64 that cause  to cause a crash
// Remove any scaling from this matrix (ie magnitude of each row is 1)
 void FMatrix::RemoveScaling(float Tolerance/*=SMALL_NUMBER*/)
{
	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);
	const float Scale0 = FMath::FloatSelect(SquareSum0 - Tolerance, FMath::InvSqrt(SquareSum0), 1.0f);
	const float Scale1 = FMath::FloatSelect(SquareSum1 - Tolerance, FMath::InvSqrt(SquareSum1), 1.0f);
	const float Scale2 = FMath::FloatSelect(SquareSum2 - Tolerance, FMath::InvSqrt(SquareSum2), 1.0f);
	M[0][0] *= Scale0;
	M[0][1] *= Scale0;
	M[0][2] *= Scale0;
	M[1][0] *= Scale1;
	M[1][1] *= Scale1;
	M[1][2] *= Scale1;
	M[2][0] *= Scale2;
	M[2][1] *= Scale2;
	M[2][2] *= Scale2;
}

// Returns matrix without scale information
 FMatrix FMatrix::GetMatrixWithoutScale(float Tolerance/*=SMALL_NUMBER*/) const
{
	FMatrix Result = *this;
	Result.RemoveScaling(Tolerance);
	return Result;
}

/** Remove any scaling from this matrix (ie magnitude of each row is 1) and return the 3D scale vector that was initially present. */
 FVector FMatrix::ExtractScaling(float Tolerance/*=SMALL_NUMBER*/)
{
	FVector Scale3D(0, 0, 0);

	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	const float SquareSum0 = (M[0][0] * M[0][0]) + (M[0][1] * M[0][1]) + (M[0][2] * M[0][2]);
	const float SquareSum1 = (M[1][0] * M[1][0]) + (M[1][1] * M[1][1]) + (M[1][2] * M[1][2]);
	const float SquareSum2 = (M[2][0] * M[2][0]) + (M[2][1] * M[2][1]) + (M[2][2] * M[2][2]);

	if (SquareSum0 > Tolerance)
	{
		float Scale0 = FMath::Sqrt(SquareSum0);
		Scale3D[0] = Scale0;
		float InvScale0 = 1.f / Scale0;
		M[0][0] *= InvScale0;
		M[0][1] *= InvScale0;
		M[0][2] *= InvScale0;
	}
	else
	{
		Scale3D[0] = 0;
	}

	if (SquareSum1 > Tolerance)
	{
		float Scale1 = FMath::Sqrt(SquareSum1);
		Scale3D[1] = Scale1;
		float InvScale1 = 1.f / Scale1;
		M[1][0] *= InvScale1;
		M[1][1] *= InvScale1;
		M[1][2] *= InvScale1;
	}
	else
	{
		Scale3D[1] = 0;
	}

	if (SquareSum2 > Tolerance)
	{
		float Scale2 = FMath::Sqrt(SquareSum2);
		Scale3D[2] = Scale2;
		float InvScale2 = 1.f / Scale2;
		M[2][0] *= InvScale2;
		M[2][1] *= InvScale2;
		M[2][2] *= InvScale2;
	}
	else
	{
		Scale3D[2] = 0;
	}

	return Scale3D;
}

/** return a 3D scale vector calculated from this matrix (where each component is the magnitude of a row vector). */
 FVector FMatrix::GetScaleVector(float Tolerance/*=SMALL_NUMBER*/) const
{
	FVector Scale3D(1, 1, 1);

	// For each row, find magnitude, and if its non-zero re-scale so its unit length.
	for (int32 i = 0; i < 3; i++)
	{
		const float SquareSum = (M[i][0] * M[i][0]) + (M[i][1] * M[i][1]) + (M[i][2] * M[i][2]);
		if (SquareSum > Tolerance)
		{
			Scale3D[i] = FMath::Sqrt(SquareSum);
		}
		else
		{
			Scale3D[i] = 0.f;
		}
	}

	return Scale3D;
}
// Remove any translation from this matrix
 FMatrix FMatrix::RemoveTranslation() const
{
	FMatrix Result = *this;
	Result.M[3][0] = 0.0f;
	Result.M[3][1] = 0.0f;
	Result.M[3][2] = 0.0f;
	return Result;
}

 FMatrix FMatrix::ConcatTranslation(const FVector& Translation) const
{
	FMatrix Result;

	float* RESTRICT Dest = &Result.M[0][0];
	const float* RESTRICT Src = &M[0][0];
	const float* RESTRICT Trans = &Translation.X;

	Dest[0] = Src[0];
	Dest[1] = Src[1];
	Dest[2] = Src[2];
	Dest[3] = Src[3];
	Dest[4] = Src[4];
	Dest[5] = Src[5];
	Dest[6] = Src[6];
	Dest[7] = Src[7];
	Dest[8] = Src[8];
	Dest[9] = Src[9];
	Dest[10] = Src[10];
	Dest[11] = Src[11];
	Dest[12] = Src[12] + Trans[0];
	Dest[13] = Src[13] + Trans[1];
	Dest[14] = Src[14] + Trans[2];
	Dest[15] = Src[15];

	return Result;
}

/** Returns true if any element of this matrix is not finite */
 bool FMatrix::ContainsNaN() const
{
	for (int32 i = 0; i < 4; i++)
	{
		for (int32 j = 0; j < 4; j++)
		{
			if (!FMath::IsFinite(M[i][j]))
			{
				return true;
			}
		}
	}

	return false;
}

/** @return the maximum magnitude of any row of the matrix. */
 float FMatrix::GetMaximumAxisScale() const
{
	const float MaxRowScaleSquared = FMath::Max(
		GetScaledAxis(EAxis::X).SizeSquared(),
		FMath::Max(
			GetScaledAxis(EAxis::Y).SizeSquared(),
			GetScaledAxis(EAxis::Z).SizeSquared()
		)
	);
	return FMath::Sqrt(MaxRowScaleSquared);
}

 void FMatrix::ScaleTranslation(const FVector& InScale3D)
{
	M[3][0] *= InScale3D.X;
	M[3][1] *= InScale3D.Y;
	M[3][2] *= InScale3D.Z;
}

// GetOrigin

 FVector FMatrix::GetOrigin() const
{
	return FVector(M[3][0], M[3][1], M[3][2]);
}

 FVector FMatrix::GetScaledAxis(EAxis::Type InAxis) const
{
	switch (InAxis)
	{
	case EAxis::X:
		return FVector(M[0][0], M[0][1], M[0][2]);

	case EAxis::Y:
		return FVector(M[1][0], M[1][1], M[1][2]);

	case EAxis::Z:
		return FVector(M[2][0], M[2][1], M[2][2]);

	default:
		checkSlow(0);
		return FVector(0.0f, 0.0f, 0.0f);
	}
}

 void FMatrix::GetScaledAxes(FVector& X, FVector& Y, FVector& Z) const
{
	X.X = M[0][0]; X.Y = M[0][1]; X.Z = M[0][2];
	Y.X = M[1][0]; Y.Y = M[1][1]; Y.Z = M[1][2];
	Z.X = M[2][0]; Z.Y = M[2][1]; Z.Z = M[2][2];
}

 FVector FMatrix::GetUnitAxis(EAxis::Type InAxis) const
{
	return GetScaledAxis(InAxis).GetSafeNormal();
}

 void FMatrix::GetUnitAxes(FVector& X, FVector& Y, FVector& Z) const
{
	GetScaledAxes(X, Y, Z);
	X.Normalize();
	Y.Normalize();
	Z.Normalize();
}

 void FMatrix::SetAxis(int32 i, const FVector& Axis)
{
	checkSlow(i >= 0 && i <= 2);
	M[i][0] = Axis.X;
	M[i][1] = Axis.Y;
	M[i][2] = Axis.Z;
}


 void FMatrix::SetOrigin(const FVector& NewOrigin)
{
	M[3][0] = NewOrigin.X;
	M[3][1] = NewOrigin.Y;
	M[3][2] = NewOrigin.Z;
}

 void FMatrix::SetAxes(FVector* Axis0 /*= NULL*/, FVector* Axis1 /*= NULL*/, FVector* Axis2 /*= NULL*/, FVector* Origin /*= NULL*/)
{
	if (Axis0 != NULL)
	{
		M[0][0] = Axis0->X;
		M[0][1] = Axis0->Y;
		M[0][2] = Axis0->Z;
	}
	if (Axis1 != NULL)
	{
		M[1][0] = Axis1->X;
		M[1][1] = Axis1->Y;
		M[1][2] = Axis1->Z;
	}
	if (Axis2 != NULL)
	{
		M[2][0] = Axis2->X;
		M[2][1] = Axis2->Y;
		M[2][2] = Axis2->Z;
	}
	if (Origin != NULL)
	{
		M[3][0] = Origin->X;
		M[3][1] = Origin->Y;
		M[3][2] = Origin->Z;
	}
}


FVector FMatrix::GetColumn(int i) const
{
	checkSlow(i >= 0 && i <= 3);
	return FVector(M[0][i], M[1][i], M[2][i]);
}

void FMatrix::SetColumn(int i, FVector Value)
{
	checkSlow(i >= 0 && i <= 3);
	M[0][i] = Value.X;
	M[1][i] = Value.Y;
	M[2][i] = Value.Z;
}

FRotator FMatrix::Rotator() const
{
	const FVector		XAxis = GetScaledAxis(EAxis::X);
	const FVector		YAxis = GetScaledAxis(EAxis::Y);
	const FVector		ZAxis = GetScaledAxis(EAxis::Z);

	FRotator	Rotator = FRotator(
		FMath::Atan2(XAxis.Z, FMath::Sqrt(FMath::Square(XAxis.X) + FMath::Square(XAxis.Y))) * 180.f / PI,
		FMath::Atan2(XAxis.Y, XAxis.X) * 180.f / PI,
		0
	);

	const FVector		SYAxis = FRotationMatrix(Rotator).GetScaledAxis(EAxis::Y);
	Rotator.Roll = FMath::Atan2(ZAxis | SYAxis, YAxis | SYAxis) * 180.f / PI;

	Rotator.DiagnosticCheckNaN();
	return Rotator;
}

FQuat FMatrix::ToQuat() const
{
	FQuat Result(*this);
	return Result;
}

bool MakeFrustumPlane(float A, float B, float C, float D, FVector4D& OutPlane)
{
	const float	LengthSquared = A * A + B * B + C * C;
	if (LengthSquared > DELTA * DELTA)
	{
		const float	InvLength = FMath::InvSqrt(LengthSquared);
		OutPlane = FVector4D(-A * InvLength, -B * InvLength, -C * InvLength, D * InvLength);
		return 1;
	}
	else
		return 0;
}

// Frustum plane extraction.
 bool FMatrix::GetFrustumNearPlane(FVector4D& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][2],
		M[1][2],
		M[2][2],
		M[3][2],
		OutPlane
	);
}

 bool FMatrix::GetFrustumFarPlane(FVector4D& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][2],
		M[1][3] - M[1][2],
		M[2][3] - M[2][2],
		M[3][3] - M[3][2],
		OutPlane
	);
}

 bool FMatrix::GetFrustumLeftPlane(FVector4D& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] + M[0][0],
		M[1][3] + M[1][0],
		M[2][3] + M[2][0],
		M[3][3] + M[3][0],
		OutPlane
	);
}

 bool FMatrix::GetFrustumRightPlane(FVector4D& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][0],
		M[1][3] - M[1][0],
		M[2][3] - M[2][0],
		M[3][3] - M[3][0],
		OutPlane
	);
}

 bool FMatrix::GetFrustumTopPlane(FVector4D& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] - M[0][1],
		M[1][3] - M[1][1],
		M[2][3] - M[2][1],
		M[3][3] - M[3][1],
		OutPlane
	);
}

 bool FMatrix::GetFrustumBottomPlane(FVector4D& OutPlane) const
{
	return MakeFrustumPlane(
		M[0][3] + M[0][1],
		M[1][3] + M[1][1],
		M[2][3] + M[2][1],
		M[3][3] + M[3][1],
		OutPlane
	);
}

/**
 * Utility for mirroring this transform across a certain plane,
 * and flipping one of the axis as well.
 */
 void FMatrix::Mirror(EAxis::Type MirrorAxis, EAxis::Type FlipAxis)
{
	if (MirrorAxis == EAxis::X)
	{
		M[0][0] *= -1.f;
		M[1][0] *= -1.f;
		M[2][0] *= -1.f;

		M[3][0] *= -1.f;
	}
	else if (MirrorAxis == EAxis::Y)
	{
		M[0][1] *= -1.f;
		M[1][1] *= -1.f;
		M[2][1] *= -1.f;

		M[3][1] *= -1.f;
	}
	else if (MirrorAxis == EAxis::Z)
	{
		M[0][2] *= -1.f;
		M[1][2] *= -1.f;
		M[2][2] *= -1.f;

		M[3][2] *= -1.f;
	}

	if (FlipAxis == EAxis::X)
	{
		M[0][0] *= -1.f;
		M[0][1] *= -1.f;
		M[0][2] *= -1.f;
	}
	else if (FlipAxis == EAxis::Y)
	{
		M[1][0] *= -1.f;
		M[1][1] *= -1.f;
		M[1][2] *= -1.f;
	}
	else if (FlipAxis == EAxis::Z)
	{
		M[2][0] *= -1.f;
		M[2][1] *= -1.f;
		M[2][2] *= -1.f;
	}
}

FString FMatrix::ToString() const
{
	FString Output;

	Output += FVector4D( M[0][0], M[0][1], M[0][2], M[0][3]).To_String();
	Output += FVector4D(M[1][0], M[1][1], M[1][2], M[1][3]).To_String();
	Output += FVector4D(M[2][0], M[2][1], M[2][2], M[2][3]).To_String();
	Output += FVector4D(M[3][0], M[3][1], M[3][2], M[3][3]).To_String();

	return Output;
}

uint32 FMatrix::ComputeHash() const
{
	uint32 Ret = 0;

	const uint32* Data = (uint32*)this;

	for (uint32 i = 0; i < 16; ++i)
	{
		Ret ^= Data[i] + i;
	}

	return Ret;
}

/**
 * Apply Scale to this matrix
 */
 FMatrix FMatrix::ApplyScale(float Scale) const
{
	FMatrix ScaleMatrix(
		FVector4D(Scale, 0.0f, 0.0f, 0.0f),
		FVector4D(0.0f, Scale, 0.0f, 0.0f),
		FVector4D(0.0f, 0.0f, Scale, 0.0f),
		FVector4D(0.0f, 0.0f, 0.0f, 1.0f)
	);
	return ScaleMatrix * (*this);
}



FRotationTranslationMatrix::FRotationTranslationMatrix(const FRotator& Rot, const FVector& Origin)
{


	const VectorRegister Angles = MakeVectorRegister(Rot.Pitch, Rot.Yaw, Rot.Roll, 0.0f);
	const VectorRegister HalfAngles = VectorMultiply(Angles, GlobalVectorConstants::DEG_TO_RAD);

	union { VectorRegister v; float f[4]; } SinAngles, CosAngles;
	VectorSinCos(&SinAngles.v, &CosAngles.v, &HalfAngles);

	const float	SP = SinAngles.f[0];
	const float	SY = SinAngles.f[1];
	const float	SR = SinAngles.f[2];
	const float	CP = CosAngles.f[0];
	const float	CY = CosAngles.f[1];
	const float	CR = CosAngles.f[2];


	M[0][0] = CP * CY;
	M[0][1] = CP * SY;
	M[0][2] = SP;
	M[0][3] = 0.f;

	M[1][0] = SR * SP * CY - CR * SY;
	M[1][1] = SR * SP * SY + CR * CY;
	M[1][2] = -SR * CP;
	M[1][3] = 0.f;

	M[2][0] = -(CR * SP * CY + SR * SY);
	M[2][1] = CY * SR - CR * SP * SY;
	M[2][2] = CR * CP;
	M[2][3] = 0.f;

	M[3][0] = Origin.X;
	M[3][1] = Origin.Y;
	M[3][2] = Origin.Z;
	M[3][3] = 1.f;
}

FRotationAboutPointMatrix::FRotationAboutPointMatrix(const FRotator& Rot, const FVector& Origin)
	: FRotationTranslationMatrix(Rot, Origin)
{
	// FRotationTranslationMatrix generates R * T.
	// We need -T * R * T, so prepend that translation:
	FVector XAxis(M[0][0], M[1][0], M[2][0]);
	FVector YAxis(M[0][1], M[1][1], M[2][1]);
	FVector ZAxis(M[0][2], M[1][2], M[2][2]);

	M[3][0] -= XAxis | Origin;
	M[3][1] -= YAxis | Origin;
	M[3][2] -= ZAxis | Origin;
}
FQuatRotationTranslationMatrix::FQuatRotationTranslationMatrix(const FQuat& Q, const FVector& Origin)
{

	checkSlow(Q.IsNormalized());

	const float x2 = Q.X + Q.X;  const float y2 = Q.Y + Q.Y;  const float z2 = Q.Z + Q.Z;
	const float xx = Q.X * x2;   const float xy = Q.X * y2;   const float xz = Q.X * z2;
	const float yy = Q.Y * y2;   const float yz = Q.Y * z2;   const float zz = Q.Z * z2;
	const float wx = Q.W * x2;   const float wy = Q.W * y2;   const float wz = Q.W * z2;

	M[0][0] = 1.0f - (yy + zz);	M[1][0] = xy - wz;				M[2][0] = xz + wy;			M[3][0] = Origin.X;
	M[0][1] = xy + wz;			M[1][1] = 1.0f - (xx + zz);		M[2][1] = yz - wx;			M[3][1] = Origin.Y;
	M[0][2] = xz - wy;			M[1][2] = yz + wx;				M[2][2] = 1.0f - (xx + yy);	M[3][2] = Origin.Z;
	M[0][3] = 0.0f;				M[1][3] = 0.0f;					M[2][3] = 0.0f;				M[3][3] = 1.0f;
}


FMatrix FRotationAboutPointMatrix::Make(const FQuat& Rot, const FVector& Origin)
 {
	 return FRotationAboutPointMatrix(Rot.Rotator(), Origin);
 }
FMatrix FRotationMatrix::Make(FQuat const& Rot)
{
	return FQuatRotationTranslationMatrix(Rot, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromX(FVector const& XAxis)
{
	FVector const NewX = XAxis.GetSafeNormal();

	// try to use up if possible
	FVector const UpVector = (FMath::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);

	const FVector NewY = (UpVector ^ NewX).GetSafeNormal();
	const FVector NewZ = NewX ^ NewY;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromY(FVector const& YAxis)
{
	FVector const NewY = YAxis.GetSafeNormal();

	// try to use up if possible
	FVector const UpVector = (FMath::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);

	const FVector NewZ = (UpVector ^ NewY).GetSafeNormal();
	const FVector NewX = NewY ^ NewZ;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromZ(FVector const& ZAxis)
{
	FVector const NewZ = ZAxis.GetSafeNormal();

	// try to use up if possible
	FVector const UpVector = (FMath::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);

	const FVector NewX = (UpVector ^ NewZ).GetSafeNormal();
	const FVector NewY = NewZ ^ NewX;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromXY(FVector const& XAxis, FVector const& YAxis)
{
	FVector NewX = XAxis.GetSafeNormal();
	FVector Norm = YAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (FMath::IsNearlyEqual(FMath::Abs(NewX | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (FMath::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);
	}

	const FVector NewZ = (NewX ^ Norm).GetSafeNormal();
	const FVector NewY = NewZ ^ NewX;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromXZ(FVector const& XAxis, FVector const& ZAxis)
{
	FVector const NewX = XAxis.GetSafeNormal();
	FVector Norm = ZAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (FMath::IsNearlyEqual(FMath::Abs(NewX | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (FMath::Abs(NewX.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);
	}

	const FVector NewY = (Norm ^ NewX).GetSafeNormal();
	const FVector NewZ = NewX ^ NewY;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromYX(FVector const& YAxis, FVector const& XAxis)
{
	FVector const NewY = YAxis.GetSafeNormal();
	FVector Norm = XAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (FMath::IsNearlyEqual(FMath::Abs(NewY | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (FMath::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);
	}

	const FVector NewZ = (Norm ^ NewY).GetSafeNormal();
	const FVector NewX = NewY ^ NewZ;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromYZ(FVector const& YAxis, FVector const& ZAxis)
{
	FVector const NewY = YAxis.GetSafeNormal();
	FVector Norm = ZAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (FMath::IsNearlyEqual(FMath::Abs(NewY | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (FMath::Abs(NewY.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);
	}

	const FVector NewX = (NewY ^ Norm).GetSafeNormal();
	const FVector NewZ = NewX ^ NewY;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromZX(FVector const& ZAxis, FVector const& XAxis)
{
	FVector const NewZ = ZAxis.GetSafeNormal();
	FVector Norm = XAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (FMath::IsNearlyEqual(FMath::Abs(NewZ | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (FMath::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);
	}

	const FVector NewY = (NewZ ^ Norm).GetSafeNormal();
	const FVector NewX = NewY ^ NewZ;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}

FMatrix FRotationMatrix::MakeFromZY(FVector const& ZAxis, FVector const& YAxis)
{
	FVector const NewZ = ZAxis.GetSafeNormal();
	FVector Norm = YAxis.GetSafeNormal();

	// if they're almost same, we need to find arbitrary vector
	if (FMath::IsNearlyEqual(FMath::Abs(NewZ | Norm), 1.f))
	{
		// make sure we don't ever pick the same as NewX
		Norm = (FMath::Abs(NewZ.Z) < (1.f - KINDA_SMALL_NUMBER)) ? FVector(0, 0, 1.f) : FVector(1.f, 0, 0);
	}

	const FVector NewX = (Norm ^ NewZ).GetSafeNormal();
	const FVector NewY = NewZ ^ NewX;

	return FMatrix(NewX, NewY, NewZ, FVector::ZeroVector);
}
void GetSinCos(float& S, float& C, float Degrees)
{
	if (Degrees == 0.f)
	{
		S = 0.f;
		C = 1.f;
	}
	else if (Degrees == 90.f)
	{
		S = 1.f;
		C = 0.f;
	}
	else if (Degrees == 180.f)
	{
		S = 0.f;
		C = -1.f;
	}
	else if (Degrees == 270.f)
	{
		S = -1.f;
		C = 0.f;
	}
	else
	{
		FMath::SinCos(&S, &C, FMath::DegreesToRadians(Degrees));
	}
}
FScaleRotationTranslationMatrix::FScaleRotationTranslationMatrix(const FVector& Scale, const FRotator& Rot, const FVector& Origin)
{
	float SP, SY, SR;
	float CP, CY, CR;
	GetSinCos(SP, CP, Rot.Pitch);
	GetSinCos(SY, CY, Rot.Yaw);
	GetSinCos(SR, CR, Rot.Roll);

	M[0][0] = (CP * CY) * Scale.X;
	M[0][1] = (CP * SY) * Scale.X;
	M[0][2] = (SP)*Scale.X;
	M[0][3] = 0.f;

	M[1][0] = (SR * SP * CY - CR * SY) * Scale.Y;
	M[1][1] = (SR * SP * SY + CR * CY) * Scale.Y;
	M[1][2] = (-SR * CP) * Scale.Y;
	M[1][3] = 0.f;

	M[2][0] = (-(CR * SP * CY + SR * SY)) * Scale.Z;
	M[2][1] = (CY * SR - CR * SP * SY) * Scale.Z;
	M[2][2] = (CR * CP) * Scale.Z;
	M[2][3] = 0.f;

	M[3][0] = Origin.X;
	M[3][1] = Origin.Y;
	M[3][2] = Origin.Z;
	M[3][3] = 1.f;
}


/* FScaleMatrix inline functions
 *****************************************************************************/

 FScaleMatrix::FScaleMatrix(float Scale)
	: FMatrix(
		FVector4D(Scale, 0.0f, 0.0f, 0.0f),
		FVector4D(0.0f, Scale, 0.0f, 0.0f),
		FVector4D(0.0f, 0.0f, Scale, 0.0f),
		FVector4D(0.0f, 0.0f, 0.0f, 1.0f)
	)
{ }


 FScaleMatrix::FScaleMatrix(const FVector& Scale)
	: FMatrix(
		FVector4D(Scale.X, 0.0f, 0.0f, 0.0f),
		FVector4D(0.0f, Scale.Y, 0.0f, 0.0f),
		FVector4D(0.0f, 0.0f, Scale.Z, 0.0f),
		FVector4D(0.0f, 0.0f, 0.0f, 1.0f)
	)
{ }
