#include "..\..\..\Public\Core\Vector\TransformVectorized.h"
const FTransform FTransform::Identity(FQuat(0.f, 0.f, 0.f, 1.f), FVector(0.f), FVector(1.f));
FString FTransform::ToString() const
{
	const FRotator R(Rotator());
	const FVector T(GetTranslation());
	const FVector S(GetScale3D());
	FString str = R.ToString();
	str += "|" + T.To_String();
	str += "|" + S.To_String();
	return str;
}

FTransform FTransform::GetRelativeTransform(const FTransform& Other) const
{
	// A * B(-1) = VQS(B)(-1) (VQS (A))
	// 
	// Scale = S(A)/S(B)
	// Rotation = Q(B)(-1) * Q(A)
	// Translation = 1/S(B) *[Q(B)(-1)*(T(A)-T(B))*Q(B)]
	// where A = this, B = Other
	FTransform Result;

	if (Other.IsRotationNormalized() == false)
	{
		return FTransform::Identity;
	}

	if (Private_AnyHasNegativeScale(this->Scale3D, Other.Scale3D))
	{
		// @note, if you have 0 scale with negative, you're going to lose rotation as it can't convert back to quat
		GetRelativeTransformUsingMatrixWithScale(&Result, this, &Other);
	}
	else
	{
		// Scale = S(A)/S(B)
		static ScalarRegister STolerance(SMALL_NUMBER);
		VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(Other.Scale3D, STolerance));

		VectorRegister VScale3D = VectorMultiply(Scale3D, VSafeScale3D);

		//VQTranslation = (  ( T(A).X - T(B).X ),  ( T(A).Y - T(B).Y ), ( T(A).Z - T(B).Z), 0.f );
		VectorRegister VQTranslation = VectorSet_W0(VectorSubtract(Translation, Other.Translation));

		// Inverse RotatedTranslation
		VectorRegister VInverseRot = VectorQuaternionInverse(Other.Rotation);
		VectorRegister VR = VectorQuaternionRotateVector(VInverseRot, VQTranslation);

		//Translation = 1/S(B)
		VectorRegister VTranslation = VectorMultiply(VR, VSafeScale3D);

		// Rotation = Q(B)(-1) * Q(A)	
		VectorRegister VRotation = VectorQuaternionMultiply2(VInverseRot, Rotation);

		Result.Scale3D = VScale3D;
		Result.Translation = VTranslation;
		Result.Rotation = VRotation;

		Result.DiagnosticCheckNaN_All();
	}

	return Result;
}

FTransform FTransform::GetRelativeTransformReverse(const FTransform& Other) const
{
	// A (-1) * B = VQS(B)(VQS (A)(-1))
// 
// Scale = S(B)/S(A)
// Rotation = Q(B) * Q(A)(-1)
// Translation = T(B)-S(B)/S(A) *[Q(B)*Q(A)(-1)*T(A)*Q(A)*Q(B)(-1)]
// where A = this, and B = Other
	FTransform Result;

	// Scale = S(B)/S(A)	
	VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(Scale3D));
	VectorRegister VScale3D = VectorMultiply(Other.Scale3D, VSafeScale3D);

	// Rotation = Q(B) * Q(A)(-1)	
	VectorRegister VInverseRot = VectorQuaternionInverse(Rotation);
	VectorRegister VRotation = VectorQuaternionMultiply2(Other.Rotation, VInverseRot);

	// RotatedTranslation
	VectorRegister VR = VectorQuaternionRotateVector(VRotation, Translation);

	// Translation = T(B)-S(B)/S(A) *[Q(B)*Q(A)(-1)*T(A)*Q(A)*Q(B)(-1)]	
	VectorRegister VTranslation = VectorSet_W0(VectorSubtract(Other.Translation, VectorMultiply(VScale3D, VR)));

	Result.Scale3D = VScale3D;
	Result.Translation = VTranslation;
	Result.Rotation = VRotation;

	Result.DiagnosticCheckNaN_All();
	return Result;
}

void FTransform::SetToRelativeTransform(const FTransform& ParentTransform)
{
	// A * B(-1) = VQS(B)(-1) (VQS (A))
	// 
	// Scale = S(A)/S(B)
	// Rotation = Q(B)(-1) * Q(A)
	// Translation = 1/S(B) *[Q(B)(-1)*(T(A)-T(B))*Q(B)]
	// where A = this, B = Other

	checkSlow(ParentTransform.IsRotationNormalized());

	// Scale = S(A)/S(B)	
	VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(ParentTransform.Scale3D, ScalarRegister(SMALL_NUMBER)));
	Scale3D = VectorMultiply(Scale3D, VSafeScale3D);

	//VQTranslation = (  ( T(A).X - T(B).X ),  ( T(A).Y - T(B).Y ), ( T(A).Z - T(B).Z), 0.f );
	VectorRegister VQTranslation = VectorSet_W0(VectorSubtract(Translation, ParentTransform.Translation));

	// Inverse RotatedTranslation
	VectorRegister VInverseParentRot = VectorQuaternionInverse(ParentTransform.Rotation);
	VectorRegister VR = VectorQuaternionRotateVector(VInverseParentRot, VQTranslation);

	// Translation = 1/S(B)
	Translation = VectorMultiply(VR, VSafeScale3D);

	// Rotation = Q(B)(-1) * Q(A)	
	Rotation = VectorQuaternionMultiply2(VInverseParentRot, Rotation);

	DiagnosticCheckNaN_All();

}

void FTransform::GetRelativeTransformUsingMatrixWithScale(FTransform* OutTransform, const FTransform* Base, const FTransform* Relative)
{
	// the goal of using M is to get the correct orientation
	// but for translation, we still need scale
	FMatrix AM = Base->ToMatrixWithScale();
	FMatrix BM = Relative->ToMatrixWithScale();
	// get combined scale
	// Scale = S(A)/S(B)
	static ScalarRegister STolerance(SMALL_NUMBER);
	VectorRegister VSafeScale3D = VectorSet_W0(GetSafeScaleReciprocal(Relative->Scale3D, STolerance));
	VectorRegister VScale3D = VectorMultiply(Base->Scale3D, VSafeScale3D);
	ConstructTransformFromMatrixWithDesiredScale(AM, BM.Inverse(), VScale3D, *OutTransform);
}