#include "..\..\..\Public\Core\Vector\Quat.h"
#include "..\..\..\Public\Core\Vector\Rotator.h"
#include "..\..\..\Public\Core\Vector\Matrix.h"
#include "..\..\..\Public\ThirdParty\SSE\MathSSE.h"
#include "..\..\..\Public\Core\Vector\String.h"
#include "..\..\..\Public\Core\Helper\Log.h"

const FQuat FQuat::Identity(0, 0, 0, 1);

/* FQuat  functions
 *****************************************************************************/

 FQuat::FQuat(const FMatrix& M)
{
	// If Matrix is NULL, return Identity quaternion. If any of them is 0, you won't be able to construct rotation
	// if you have two plane at least, we can reconstruct the frame using cross product, but that's a bit expensive op to do here
	// for now, if you convert to matrix from 0 scale and convert back, you'll lose rotation. Don't do that. 
	if (M.GetScaledAxis(EAxis::X).IsNearlyZero() || M.GetScaledAxis(EAxis::Y).IsNearlyZero() || M.GetScaledAxis(EAxis::Z).IsNearlyZero())
	{
		*this = FQuat::Identity;
		return;
	}

	// Make sure the Rotation part of the Matrix is unit length.
	// Changed to this (same as RemoveScaling) from RotDeterminant as using two different ways of checking unit length matrix caused inconsistency. 
	if (((FMath::Abs(1.f - M.GetScaledAxis(EAxis::X).SizeSquared()) <= KINDA_SMALL_NUMBER) && (FMath::Abs(1.f - M.GetScaledAxis(EAxis::Y).SizeSquared()) <= KINDA_SMALL_NUMBER) && (FMath::Abs(1.f - M.GetScaledAxis(EAxis::Z).SizeSquared()) <= KINDA_SMALL_NUMBER)))
	{
		*this = FQuat::Identity;
		return;
	}


	//const MeReal *const t = (MeReal *) tm;
	float	s;

	// Check diagonal (trace)
	const float tr = M.M[0][0] + M.M[1][1] + M.M[2][2];

	if (tr > 0.0f)
	{
		float InvS = FMath::InvSqrt(tr + 1.f);
		this->W = 0.5f * (1.f / InvS);
		s = 0.5f * InvS;

		this->X = (M.M[1][2] - M.M[2][1]) * s;
		this->Y = (M.M[2][0] - M.M[0][2]) * s;
		this->Z = (M.M[0][1] - M.M[1][0]) * s;
	}
	else
	{
		// diagonal is negative
		int32 i = 0;

		if (M.M[1][1] > M.M[0][0])
			i = 1;

		if (M.M[2][2] > M.M[i][i])
			i = 2;

		static const int32 nxt[3] = { 1, 2, 0 };
		const int32 j = nxt[i];
		const int32 k = nxt[j];

		s = M.M[i][i] - M.M[j][j] - M.M[k][k] + 1.0f;

		float InvS = FMath::InvSqrt(s);

		float qt[4];
		qt[i] = 0.5f * (1.f / InvS);

		s = 0.5f * InvS;

		qt[3] = (M.M[j][k] - M.M[k][j]) * s;
		qt[j] = (M.M[i][j] + M.M[j][i]) * s;
		qt[k] = (M.M[i][k] + M.M[k][i]) * s;

		this->X = qt[0];
		this->Y = qt[1];
		this->Z = qt[2];
		this->W = qt[3];

		DiagnosticCheckNaN();
	}
}


FQuat::FQuat(const FRotator& R)
{
	*this = R.Quaternion();
	DiagnosticCheckNaN();
}


FVector FQuat::operator*(const FVector& V) const
{
	return RotateVector(V);
}


 FMatrix FQuat::operator*(const FMatrix& M) const
{
	FMatrix Result;
	FQuat VT, VR;
	FQuat Inv = Inverse();
	for (int32 I = 0; I < 4; ++I)
	{
		FQuat VQ(M.M[I][0], M.M[I][1], M.M[I][2], M.M[I][3]);
		VectorQuaternionMultiply(&VT, this, &VQ);
		VectorQuaternionMultiply(&VR, &VT, &Inv);
		Result.M[I][0] = VR.X;
		Result.M[I][1] = VR.Y;
		Result.M[I][2] = VR.Z;
		Result.M[I][3] = VR.W;
	}

	return Result;
}


/* FQuat  functions
 *****************************************************************************/

FQuat::FQuat(EForceInit ZeroOrNot)
	: X(0), Y(0), Z(0), W(ZeroOrNot == ForceInitToZero ? 0.0f : 1.0f)
{ }


FQuat::FQuat(float InX, float InY, float InZ, float InW)
	: X(InX)
	, Y(InY)
	, Z(InZ)
	, W(InW)
{
	DiagnosticCheckNaN();
}


FQuat::FQuat(const __m128& V)
{
	VectorStoreAligned(V, this);
	DiagnosticCheckNaN();
}


FString FQuat::ToString() const
{
	return "(X=" + FString::FormatFloatToString(X) + " Y=" + FString::FormatFloatToString(Y) + "Z=" + FString::FormatFloatToString(Z) + "W=" + FString::FormatFloatToString(W) + ")";
}




FQuat::FQuat(FVector Axis, float AngleRad)
{
	const float half_a = 0.5f * AngleRad;
	float s, c;
	FMath::SinCos(&s, &c, half_a);

	X = s * Axis.X;
	Y = s * Axis.Y;
	Z = s * Axis.Z;
	W = c;

	DiagnosticCheckNaN();
}


FQuat FQuat::operator+(const FQuat& Q) const
{
	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorLoadAligned(&Q);
	return FQuat(VectorAdd(A, B));
}


FQuat FQuat::operator+=(const FQuat& Q)
{

	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorLoadAligned(&Q);
	VectorStoreAligned(VectorAdd(A, B), this);

	DiagnosticCheckNaN();

	return *this;
}


FQuat FQuat::operator-(const FQuat& Q) const
{
	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorLoadAligned(&Q);
	return FQuat(VectorSubtract(A, B));
}


bool FQuat::Equals(const FQuat& Q, float Tolerance) const
{
	const __m128 ToleranceV = VectorLoadFloat1(&Tolerance);
	const __m128 A = VectorLoadAligned(this);
	const __m128 B = VectorLoadAligned(&Q);

	const __m128 RotationSub = VectorAbs(VectorSubtract(A, B));
	const __m128 RotationAdd = VectorAbs(VectorAdd(A, B));
	return !VectorAnyGreaterThan(RotationSub, ToleranceV) || !VectorAnyGreaterThan(RotationAdd, ToleranceV);
}

bool FQuat::IsIdentity(float Tolerance) const
{
	return Equals(FQuat::Identity, Tolerance);
}

FQuat FQuat::operator-=(const FQuat& Q)
{
	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorLoadAligned(&Q);
	VectorStoreAligned(VectorSubtract(A, B), this);

	DiagnosticCheckNaN();

	return *this;
}


FQuat FQuat::operator*(const FQuat& Q) const
{
	FQuat Result;
	VectorQuaternionMultiply(&Result, this, &Q);

	Result.DiagnosticCheckNaN();

	return Result;
}


FQuat FQuat::operator*=(const FQuat& Q)
{
	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorLoadAligned(&Q);
	__m128 Result;
	VectorQuaternionMultiply(&Result, &A, &B);
	VectorStoreAligned(Result, this);

	DiagnosticCheckNaN();

	return *this;
}


FQuat FQuat::operator*=(const float Scale)
{

	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorSetFloat1(Scale);
	VectorStoreAligned(VectorMultiply(A, B), this);

	DiagnosticCheckNaN();

	return *this;
}


FQuat FQuat::operator*(const float Scale) const
{

	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorSetFloat1(Scale);
	return FQuat(VectorMultiply(A, B));
}


FQuat FQuat::operator/=(const float Scale)
{

	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorSetFloat1(Scale);
	VectorStoreAligned(VectorDivide(A, B), this);

	DiagnosticCheckNaN();

	return *this;
}


FQuat FQuat::operator/(const float Scale) const
{
	__m128 A = VectorLoadAligned(this);
	__m128 B = VectorSetFloat1(Scale);
	return FQuat(VectorDivide(A, B));
}

bool FQuat::Identical(const FQuat* Q, const uint32 PortFlags) const
{
	return X == Q->X && Y == Q->Y && Z == Q->Z && W == Q->W;
}

bool FQuat::operator==(const FQuat& Q) const
{
	const __m128 A = VectorLoadAligned(this);
	const __m128 B = VectorLoadAligned(&Q);
	return VectorMaskBits(VectorCompareEQ(A, B)) == 0x0F;
}


bool FQuat::operator!=(const FQuat& Q) const
{
	const __m128 A = VectorLoadAligned(this);
	const __m128 B = VectorLoadAligned(&Q);
	return VectorMaskBits(VectorCompareNE(A, B)) != 0x00;
}


float FQuat::operator|(const FQuat& Q) const
{
	return X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
}


void FQuat::Normalize(float Tolerance)
{

	const __m128 Vector = VectorLoadAligned(this);
	const __m128 SquareSum = VectorDot4(Vector, Vector);
	const __m128 NonZeroMask = VectorCompareGE(SquareSum, VectorLoadFloat1(&Tolerance));
	const __m128 InvLength = VectorReciprocalSqrtAccurate(SquareSum);
	const __m128 NormalizedVector = VectorMultiply(InvLength, Vector);
	__m128 Result = VectorSelect(NonZeroMask, NormalizedVector, GlobalVectorConstants::Float0001);
	VectorStoreAligned(Result, this);

}


FQuat FQuat::GetNormalized(float Tolerance) const
{
	FQuat Result(*this);
	Result.Normalize(Tolerance);
	return Result;
}



bool FQuat::IsNormalized() const
{
	__m128 A = VectorLoadAligned(this);
	__m128 TestValue = VectorAbs(VectorSubtract(VectorOne(), VectorDot4(A, A)));
	return !VectorAnyGreaterThan(TestValue, GlobalVectorConstants::ThreshQuatNormalized);
}


float FQuat::Size() const
{
	return FMath::Sqrt(X * X + Y * Y + Z * Z + W * W);
}


float FQuat::SizeSquared() const
{
	return (X * X + Y * Y + Z * Z + W * W);
}

float FQuat::GetAngle() const
{
	return 2.f * FMath::Acos(W);
}


void FQuat::ToAxisAndAngle(FVector& Axis, float& Angle) const
{
	Angle = GetAngle();
	Axis = GetRotationAxis();
}

FVector FQuat::GetRotationAxis() const
{
	FVector V;
	__m128 A = VectorLoadAligned(this);
	__m128 R = VectorNormalizeSafe(VectorSet_W0(A), GlobalVectorConstants::Float1000);
	VectorStoreFloat3(R, &V);
	return V;
}

float FQuat::AngularDistance(const FQuat& Q) const
{
	float InnerProd = X * Q.X + Y * Q.Y + Z * Q.Z + W * Q.W;
	return FMath::Acos((2 * InnerProd * InnerProd) - 1.f);
}


FVector FQuat::RotateVector(FVector V) const
{
	// http://people.csail.mit.edu/bkph/articles/Quaternions.pdf
	// V' = V + 2w(Q x V) + (2Q x (Q x V))
	// refactor:
	// V' = V + w(2(Q x V)) + (Q x (2(Q x V)))
	// T = 2(Q x V);
	// V' = V + w*(T) + (Q x T)

	const FVector Q(X, Y, Z);
	const FVector T = 2.f * FVector::CrossProduct(Q, V);
	const FVector Result = V + (W * T) + FVector::CrossProduct(Q, T);
	return Result;
}

FVector FQuat::UnrotateVector(FVector V) const
{
	const FVector Q(-X, -Y, -Z); // Inverse
	const FVector T = 2.f * FVector::CrossProduct(Q, V);
	const FVector Result = V + (W * T) + FVector::CrossProduct(Q, T);
	return Result;
}


FQuat FQuat::Inverse() const
{
	if (IsNormalized())
	{
		Log_Error("quaternion is normalized");
	}

	return FQuat(VectorQuaternionInverse(VectorLoadAligned(this)));
}


void FQuat::EnforceShortestArcWith(const FQuat& OtherQuat)
{
	const float DotResult = (OtherQuat | *this);
	const float Bias = FMath::FloatSelect(DotResult, 1.0f, -1.0f);

	X *= Bias;
	Y *= Bias;
	Z *= Bias;
	W *= Bias;
}


FVector FQuat::GetAxisX() const
{
	return RotateVector(FVector(1.f, 0.f, 0.f));
}


FVector FQuat::GetAxisY() const
{
	return RotateVector(FVector(0.f, 1.f, 0.f));
}


FVector FQuat::GetAxisZ() const
{
	return RotateVector(FVector(0.f, 0.f, 1.f));
}


FVector FQuat::GetForwardVector() const
{
	return GetAxisX();
}

FVector FQuat::GetRightVector() const
{
	return GetAxisY();
}

FVector FQuat::GetUpVector() const
{
	return GetAxisZ();
}

FVector FQuat::Vector() const
{
	return GetAxisX();
}


float FQuat::Error(const FQuat& Q1, const FQuat& Q2)
{
	const float cosom = FMath::Abs(Q1.X * Q2.X + Q1.Y * Q2.Y + Q1.Z * Q2.Z + Q1.W * Q2.W);
	return (FMath::Abs(cosom) < 0.9999999f) ? FMath::Acos(cosom) * (1.f / PI) : 0.0f;
}


float FQuat::ErrorAutoNormalize(const FQuat& A, const FQuat& B)
{
	FQuat Q1 = A;
	Q1.Normalize();

	FQuat Q2 = B;
	Q2.Normalize();

	return FQuat::Error(Q1, Q2);
}

/**
 * Fast Linear Quaternion Interpolation.
 * Result is NOT normalized.
 */
FQuat FQuat::FastLerp(const FQuat& A, const FQuat& B, const float Alpha)
{
	// To ensure the 'shortest route', we make sure the dot product between the both rotations is positive.
	const float DotResult = (A | B);
	const float Bias = FMath::FloatSelect(DotResult, 1.0f, -1.0f);
	return (B * Alpha) + (A * (Bias * (1.f - Alpha)));
}


FQuat FQuat::FastBilerp(const FQuat& P00, const FQuat& P10, const FQuat& P01, const FQuat& P11, float FracX, float FracY)
{
	return FQuat::FastLerp(
		FQuat::FastLerp(P00, P10, FracX),
		FQuat::FastLerp(P01, P11, FracX),
		FracY
	);
}


bool FQuat::ContainsNaN() const
{
	return (!FMath::IsFinite(X) ||
		!FMath::IsFinite(Y) ||
		!FMath::IsFinite(Z) ||
		!FMath::IsFinite(W)
		);
}

FRotator FQuat::Rotator() const
{

	DiagnosticCheckNaN();
	const float SingularityTest = Z * X - W * Y;
	const float YawY = 2.f * (W * Z + X * Y);
	const float YawX = (1.f - 2.f * (FMath::Square(Y) + FMath::Square(Z)));

	// reference 
	// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/

	// this value was found from experience, the above websites recommend different values
	// but that isn't the case for us, so I went through different testing, and finally found the case 
	// where both of world lives happily. 
	const float SINGULARITY_THRESHOLD = 0.4999995f;
	const float RAD_TO_DEG = (180.f) / PI;
	FRotator RotatorFromQuat;

	if (SingularityTest < -SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = -90.f;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FRotator::NormalizeAxis(-RotatorFromQuat.Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else if (SingularityTest > SINGULARITY_THRESHOLD)
	{
		RotatorFromQuat.Pitch = 90.f;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FRotator::NormalizeAxis(RotatorFromQuat.Yaw - (2.f * FMath::Atan2(X, W) * RAD_TO_DEG));
	}
	else
	{
		RotatorFromQuat.Pitch = FMath::FastAsin(2.f * (SingularityTest)) * RAD_TO_DEG;
		RotatorFromQuat.Yaw = FMath::Atan2(YawY, YawX) * RAD_TO_DEG;
		RotatorFromQuat.Roll = FMath::Atan2(-2.f * (W * X + Y * Z), (1.f - 2.f * (FMath::Square(X) + FMath::Square(Y)))) * RAD_TO_DEG;
	}
	return RotatorFromQuat;
}

FQuat FQuat::MakeFromEuler(const FVector& Euler)
{
	return FRotator::MakeFromEuler(Euler).Quaternion();
}

void FQuat::ToSwingTwist(const FVector& InTwistAxis, FQuat& OutSwing, FQuat& OutTwist) const
{
	// Vector part projected onto twist axis
	FVector Projection = FVector::DotProduct(InTwistAxis, FVector(X, Y, Z)) * InTwistAxis;

	// Twist quaternion
	OutTwist = FQuat(Projection.X, Projection.Y, Projection.Z, W);

	// Singularity close to 180deg
	if (OutTwist.SizeSquared() == 0.0f)
	{
		OutTwist = FQuat::Identity;
	}
	else
	{
		OutTwist.Normalize();
	}

	// Set swing
	OutSwing = *this * OutTwist.Inverse();
}

float FQuat::GetTwistAngle(const FVector& TwistAxis) const
{
	float XYZ = FVector::DotProduct(TwistAxis, FVector(X, Y, Z));
	return FMath::UnwindRadians(2.0f * FMath::Atan2(XYZ, W));
}
FVector FQuat::Euler() const
{
	return Rotator().Euler();
}
 FQuat FindBetween_Helper(const FVector& A, const FVector& B, float NormAB)
{
	float W = NormAB + FVector::DotProduct(A, B);
	FQuat Result;

	if (W >= 1e-6f * NormAB)
	{
		//Axis = FVector::CrossProduct(A, B);
		Result = FQuat(A.Y * B.Z - A.Z * B.Y,
			A.Z * B.X - A.X * B.Z,
			A.X * B.Y - A.Y * B.X,
			W);
	}
	else
	{
		// A and B point in opposite directions
		W = 0.f;
		Result = FMath::Abs(A.X) > FMath::Abs(A.Y)
			? FQuat(-A.Z, 0.f, A.X, W)
			: FQuat(0.f, -A.Z, A.Y, W);
	}

	Result.Normalize();
	return Result;
}

FQuat FQuat::FindBetweenNormals(const FVector& A, const FVector& B)
{
	const float NormAB = 1.f;
	return FindBetween_Helper(A, B, NormAB);
}

FQuat FQuat::FindBetweenVectors(const FVector& A, const FVector& B)
{
	const float NormAB = FMath::Sqrt(A.SizeSquared() * B.SizeSquared());
	return FindBetween_Helper(A, B, NormAB);
}

FQuat FQuat::Log() const
{
	FQuat Result;
	Result.W = 0.f;

	if (FMath::Abs(W) < 1.f)
	{
		const float Angle = FMath::Acos(W);
		const float SinAngle = FMath::Sin(Angle);

		if (FMath::Abs(SinAngle) >= SMALL_NUMBER)
		{
			const float Scale = Angle / SinAngle;
			Result.X = Scale * X;
			Result.Y = Scale * Y;
			Result.Z = Scale * Z;

			return Result;
		}
	}

	Result.X = X;
	Result.Y = Y;
	Result.Z = Z;

	return Result;
}

FQuat FQuat::Exp() const
{
	const float Angle = FMath::Sqrt(X * X + Y * Y + Z * Z);
	const float SinAngle = FMath::Sin(Angle);

	FQuat Result;
	Result.W = FMath::Cos(Angle);

	if (FMath::Abs(SinAngle) >= SMALL_NUMBER)
	{
		const float Scale = SinAngle / Angle;
		Result.X = Scale * X;
		Result.Y = Scale * Y;
		Result.Z = Scale * Z;
	}
	else
	{
		Result.X = X;
		Result.Y = Y;
		Result.Z = Z;
	}

	return Result;
}

FQuat FQuat::Slerp_NotNormalized(const FQuat& Quat1, const FQuat& Quat2, float Slerp)
{
	// Get cosine of angle between quats.
	const float RawCosom =
		Quat1.X * Quat2.X +
		Quat1.Y * Quat2.Y +
		Quat1.Z * Quat2.Z +
		Quat1.W * Quat2.W;
	// Unaligned quats - compensate, results in taking shorter route.
	const float Cosom = FMath::FloatSelect(RawCosom, RawCosom, -RawCosom);

	float Scale0, Scale1;

	if (Cosom < 0.9999f)
	{
		const float Omega = FMath::Acos(Cosom);
		const float InvSin = 1.f / FMath::Sin(Omega);
		Scale0 = FMath::Sin((1.f - Slerp) * Omega) * InvSin;
		Scale1 = FMath::Sin(Slerp * Omega) * InvSin;
	}
	else
	{
		// Use linear interpolation.
		Scale0 = 1.0f - Slerp;
		Scale1 = Slerp;
	}

	// In keeping with our flipped Cosom:
	Scale1 = FMath::FloatSelect(RawCosom, Scale1, -Scale1);

	FQuat Result;

	Result.X = Scale0 * Quat1.X + Scale1 * Quat2.X;
	Result.Y = Scale0 * Quat1.Y + Scale1 * Quat2.Y;
	Result.Z = Scale0 * Quat1.Z + Scale1 * Quat2.Z;
	Result.W = Scale0 * Quat1.W + Scale1 * Quat2.W;

	return Result;
}

FQuat FQuat::SlerpFullPath_NotNormalized(const FQuat& quat1, const FQuat& quat2, float Alpha)
{
	const float CosAngle = FMath::Clamp(quat1 | quat2, -1.f, 1.f);
	const float Angle = FMath::Acos(CosAngle);

	//UE_LOG(LogUnrealMath, Log,  TEXT("CosAngle: %f Angle: %f"), CosAngle, Angle );

	if (FMath::Abs(Angle) < KINDA_SMALL_NUMBER)
	{
		return quat1;
	}

	const float SinAngle = FMath::Sin(Angle);
	const float InvSinAngle = 1.f / SinAngle;

	const float Scale0 = FMath::Sin((1.0f - Alpha) * Angle) * InvSinAngle;
	const float Scale1 = FMath::Sin(Alpha * Angle) * InvSinAngle;

	return quat1 * Scale0 + quat2 * Scale1;
}

FQuat FQuat::Squad(const FQuat& quat1, const FQuat& tang1, const FQuat& quat2, const FQuat& tang2, float Alpha)
{
	// Always slerp along the short path from quat1 to quat2 to prevent axis flipping.
	// This approach is taken by OGRE engine, amongst others.
	const FQuat Q1 = FQuat::Slerp_NotNormalized(quat1, quat2, Alpha);
	const FQuat Q2 = FQuat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
	const FQuat Result = FQuat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

	return Result;
}

FQuat FQuat::SquadFullPath(const FQuat& quat1, const FQuat& tang1, const FQuat& quat2, const FQuat& tang2, float Alpha)
{
	const FQuat Q1 = FQuat::SlerpFullPath_NotNormalized(quat1, quat2, Alpha);
	const FQuat Q2 = FQuat::SlerpFullPath_NotNormalized(tang1, tang2, Alpha);
	const FQuat Result = FQuat::SlerpFullPath(Q1, Q2, 2.f * Alpha * (1.f - Alpha));

	return Result;
}

void FQuat::CalcTangents(const FQuat& PrevP, const FQuat& P, const FQuat& NextP, float Tension, FQuat& OutTan)
{
	const FQuat InvP = P.Inverse();
	const FQuat Part1 = (InvP * PrevP).Log();
	const FQuat Part2 = (InvP * NextP).Log();

	const FQuat PreExp = (Part1 + Part2) * -0.5f;

	OutTan = P * PreExp.Exp();
}
class FQuat FRotator::Quaternion() const
{
	DiagnosticCheckNaN();
	const VectorRegister Angles = MakeVectorRegister(Pitch, Yaw, Roll, 0.0f);
	const VectorRegister AnglesNoWinding = VectorMod(Angles, GlobalVectorConstants::Float360);
	const VectorRegister HalfAngles = VectorMultiply(AnglesNoWinding, GlobalVectorConstants::DEG_TO_RAD_HALF);

	VectorRegister SinAngles, CosAngles;
	VectorSinCos(&SinAngles, &CosAngles, &HalfAngles);

	// Vectorized conversion, measured 20% faster than using scalar version after VectorSinCos.
	// Indices within VectorRegister (for shuffles): P=0, Y=1, R=2
	const VectorRegister SR = VectorReplicate(SinAngles, 2);
	const VectorRegister CR = VectorReplicate(CosAngles, 2);

	const VectorRegister SY_SY_CY_CY_Temp = VectorShuffle(SinAngles, CosAngles, 1, 1, 1, 1);

	const VectorRegister SP_SP_CP_CP = VectorShuffle(SinAngles, CosAngles, 0, 0, 0, 0);
	const VectorRegister SY_CY_SY_CY = VectorShuffle(SY_SY_CY_CY_Temp, SY_SY_CY_CY_Temp, 0, 2, 0, 2);

	const VectorRegister CP_CP_SP_SP = VectorShuffle(CosAngles, SinAngles, 0, 0, 0, 0);
	const VectorRegister CY_SY_CY_SY = VectorShuffle(SY_SY_CY_CY_Temp, SY_SY_CY_CY_Temp, 2, 0, 2, 0);

	const uint32 Neg = uint32(1 << 31);
	const uint32 Pos = uint32(0);
	const VectorRegister SignBitsLeft = MakeVectorRegister(Pos, Neg, Pos, Pos);
	const VectorRegister SignBitsRight = MakeVectorRegister(Neg, Neg, Neg, Pos);
	const VectorRegister LeftTerm = VectorBitwiseXor(SignBitsLeft, VectorMultiply(CR, VectorMultiply(SP_SP_CP_CP, SY_CY_SY_CY)));
	const VectorRegister RightTerm = VectorBitwiseXor(SignBitsRight, VectorMultiply(SR, VectorMultiply(CP_CP_SP_SP, CY_SY_CY_SY)));

	FQuat RotationQuat;
	const VectorRegister Result = VectorAdd(LeftTerm, RightTerm);
	VectorStoreAligned(Result, &RotationQuat);
	if (RotationQuat.ContainsNaN())
	{
		Log_Error("Invalid input %s to FRotator::Quaternion - generated NaN output: %s");
		RotationQuat = FQuat::Identity;
	}

	return RotationQuat;
}