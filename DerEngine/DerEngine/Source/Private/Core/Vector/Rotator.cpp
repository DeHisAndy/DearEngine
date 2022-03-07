#include "..\..\..\Public\Core\Vector\Rotator.h"
#include "..\..\..\Public\Core\FunctionLibrary\Math.h"
#include "..\..\..\Public\ThirdParty\SSE\MathSSE.h"
#include "..\..\..\Public\Core\Helper\Log.h"
#include "..\..\..\Public\Core\FunctionLibrary\KismetSystemLibrary.h"
#include "..\..\..\Public\Core\Vector\Quat.h"
const FRotator FRotator::ZeroRotator(0.f, 0.f, 0.f);
FRotator::FRotator(const  FQuat& Quat)
{
	*this = Quat.Rotator();
	DiagnosticCheckNaN();
}

FRotator FRotator::GetInverse() const
{
	return Quaternion().Inverse().Rotator();
}

FRotator FRotator::GridSnap(const FRotator& RotGrid) const
{
	return FRotator
	(
		FMath::GridSnap(Pitch, RotGrid.Pitch),
		FMath::GridSnap(Yaw, RotGrid.Yaw),
		FMath::GridSnap(Roll, RotGrid.Roll)
	);
}

FVector FRotator::Vector() const
{

	if (FMath::Abs(Pitch) > FLOAT_NON_FRACTIONAL ||
		FMath::Abs(Yaw) > FLOAT_NON_FRACTIONAL ||
		FMath::Abs(Roll) > FLOAT_NON_FRACTIONAL)
	{
		Log_Error("FRotator::Vector() provided with unreasonably large input values, possible use of uninitialized variable?");
		checkSlow(FMath::Abs(Pitch) > FLOAT_NON_FRACTIONAL ||
			FMath::Abs(Yaw) > FLOAT_NON_FRACTIONAL ||
			FMath::Abs(Roll) > FLOAT_NON_FRACTIONAL);
	}


	// Remove winding and clamp to [-360, 360]
	const float PitchNoWinding = FMath::Fmod(Pitch, 360.0f);
	const float YawNoWinding = FMath::Fmod(Yaw, 360.0f);

	float CP, SP, CY, SY;
	FMath::SinCos(&SP, &CP, FMath::DegreesToRadians(PitchNoWinding));
	FMath::SinCos(&SY, &CY, FMath::DegreesToRadians(YawNoWinding));
	FVector V = FVector(CP * CY, CP * SY, SP);

	if (V.ContainsNaN())
	{
		Log_Error("FRotator::Vector() resulted in NaN/Inf with input:  output: ");
		V = FVector::ForwardVector;
	}


	return V;
}

bool FRotator::IsNearlyZero(float Tolerance) const
{
	const VectorRegister RegA = VectorLoadFloat3_W0(this);
	const VectorRegister Norm = VectorNormalizeRotator(RegA);
	const VectorRegister AbsNorm = VectorAbs(Norm);
	return !VectorAnyGreaterThan(AbsNorm, VectorLoadFloat1(&Tolerance));

}
bool FRotator::Equals(const FRotator& R, float Tolerance) const
{
	const VectorRegister RegA = VectorLoadFloat3_W0(this);
	const VectorRegister RegB = VectorLoadFloat3_W0(&R);
	const VectorRegister NormDelta = VectorNormalizeRotator(VectorSubtract(RegA, RegB));
	const VectorRegister AbsNormDelta = VectorAbs(NormDelta);
	return !VectorAnyGreaterThan(AbsNormDelta, VectorLoadFloat1(&Tolerance));

}

FRotator FRotator::Add(float DeltaPitch, float DeltaYaw, float DeltaRoll)
{
	Yaw += DeltaYaw;
	Pitch += DeltaPitch;
	Roll += DeltaRoll;
	DiagnosticCheckNaN();
	return *this;
}
void FRotator::Normalize()
{
	VectorRegister VRotator = VectorLoadFloat3_W0(this);
	VRotator = VectorNormalizeRotator(VRotator);
	VectorStoreFloat3(VRotator, this);
	DiagnosticCheckNaN();
}
float FRotator::GetManhattanDistance(const FRotator& Rotator) const
{
	return FMath::Abs(Yaw - Rotator.Yaw) + FMath::Abs(Pitch - Rotator.Pitch) + FMath::Abs(Roll - Rotator.Roll);
}
 bool FRotator::ContainsNaN() const
{
	return (!FMath::IsFinite(Pitch) ||
		!FMath::IsFinite(Yaw) ||
		!FMath::IsFinite(Roll));
}

 float FRotator::ClampAxis(float Angle)
{
	// returns Angle in the range (-360,360)
	Angle = FMath::Fmod(Angle, 360.f);

	if (Angle < 0.f)
	{
		// shift to [0,360) range
		Angle += 360.f;
	}

	return Angle;
}

  uint8 FRotator::CompressAxisToByte(float Angle)
 {
	 // map [0->360) to [0->256) and mask off any winding
	 return FMath::RoundToInt(Angle * 256.f / 360.f) & 0xFF;
 }

  uint16 FRotator::CompressAxisToShort(float Angle)
 {
	 // map [0->360) to [0->65536) and mask off any winding
	 return FMath::RoundToInt(Angle * 65536.f / 360.f) & 0xFFFF;
 }

 FString FRotator::ToString() const
 {
	 return	"(P=" + FString::FormatFloatToString(Pitch) + " Y=" + FString::FormatFloatToString(Yaw) + "R=" + FString::FormatFloatToString(Roll) + ")";
 }