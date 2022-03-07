// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "Vector.h"

class FString;

/**
 * Implements a container for rotation information.
 *
 * All rotation values are stored in degrees.
 */
class FRotator
{
public:
	/** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
	float Pitch;

	/** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
	float Yaw;

	/** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
	float Roll;

public:

	/** A rotator of zero degrees on each axis. */
	static  const FRotator ZeroRotator;

public:


	FORCEINLINE void DiagnosticCheckNaN() const {}
	FORCEINLINE void DiagnosticCheckNaN(const WCHAR* Message) const {}

	/**
	 * Default constructor (no initialization).
	 */
	FORCEINLINE FRotator() { }

	/**
	 * Constructor
	 *
	 * @param InF Value to set all components to.
	 */
	explicit FORCEINLINE FRotator(float InF)
		: Pitch(InF), Yaw(InF), Roll(InF)
	{
		DiagnosticCheckNaN();
	}
	/**
	 * Constructor.
	 *
	 * @param InPitch Pitch in degrees.
	 * @param InYaw Yaw in degrees.
	 * @param InRoll Roll in degrees.
	 */
	FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll) 
		: Pitch(InPitch), Yaw(InYaw), Roll(InRoll)
	{
		DiagnosticCheckNaN();
	}
	/**
	 * Constructor.
	 *
	 * @param EForceInit Force Init Enum.
	 */
	explicit FORCEINLINE FRotator(EForceInit)
		: Pitch(0), Yaw(0), Roll(0)
	{}

	/**
	 * Constructor.
	 *
	 * @param Quat Quaternion used to specify rotation.
	 */
	explicit  FRotator(const class FQuat& Quat);

public:

	// Binary arithmetic operators.

	/**
	 * Get the result of adding a rotator to this.
	 *
	 * @param R The other rotator.
	 * @return The result of adding a rotator to this.
	 */
	FRotator operator+(const FRotator& R) const
	{
		return FRotator(Pitch + R.Pitch, Yaw + R.Yaw, Roll + R.Roll);
	}

	/**
	 * Get the result of subtracting a rotator from this.
	 *
	 * @param R The other rotator.
	 * @return The result of subtracting a rotator from this.
	 */
	FRotator operator-(const FRotator& R) const
	{
		return FRotator(Pitch - R.Pitch, Yaw - R.Yaw, Roll - R.Roll);
	}
	/**
	 * Get the result of scaling this rotator.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of scaling.
	 */
	FRotator operator*(float Scale) const {
		return FRotator(Pitch * Scale, Yaw * Scale, Roll * Scale);
	}
	/**
	 * Multiply this rotator by a scaling factor.
	 *
	 * @param Scale The scaling factor.
	 * @return Copy of the rotator after scaling.
	 */
	FRotator operator*=(float Scale) {
		Pitch = Pitch * Scale; Yaw = Yaw * Scale; Roll = Roll * Scale;
		DiagnosticCheckNaN();
		return *this;
	}

	// Binary comparison operators.

	/**
	 * Checks whether two rotators are identical. This checks each component for exact equality.
	 *
	 * @param R The other rotator.
	 * @return true if two rotators are identical, otherwise false.
	 * @see Equals()
	 */
	bool operator==(const FRotator& R) const {
		return Pitch == R.Pitch && Yaw == R.Yaw && Roll == R.Roll;
	}
	/**
	 * Checks whether two rotators are different.
	 *
	 * @param V The other rotator.
	 * @return true if two rotators are different, otherwise false.
	 */
	bool operator!=(const FRotator& V) const {
		return Pitch != V.Pitch || Yaw != V.Yaw || Roll != V.Roll;
	}
	// Assignment operators.

	/**
	 * Adds another rotator to this.
	 *
	 * @param R The other rotator.
	 * @return Copy of rotator after addition.
	 */
	FRotator operator+=(const FRotator& R) {
		Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
		DiagnosticCheckNaN();
		return *this;
	}

	/**
	 * Subtracts another rotator from this.
	 *
	 * @param R The other rotator.
	 * @return Copy of rotator after subtraction.
	 */
	FRotator operator-=(const FRotator& R) {
		Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
		DiagnosticCheckNaN();
		return *this;
	}
public:

	// Functions.

	/**
	 * Checks whether rotator is nearly zero within specified tolerance, when treated as an orientation.
	 * This means that FRotator(0, 0, 360) is "zero", because it is the same final orientation as the zero rotator.
	 *
	 * @param Tolerance Error Tolerance.
	 * @return true if rotator is nearly zero, within specified tolerance, otherwise false.
	 */
	bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const;
	/**
	 * Checks whether this has exactly zero rotation, when treated as an orientation.
	 * This means that FRotator(0, 0, 360) is "zero", because it is the same final orientation as the zero rotator.
	 *
	 * @return true if this has exactly zero rotation, otherwise false.
	 */
	bool IsZero() const {
		return (ClampAxis(Pitch) == 0.f) && (ClampAxis(Yaw) == 0.f) && (ClampAxis(Roll) == 0.f);
	}

	/**
	 * Checks whether two rotators are equal within specified tolerance, when treated as an orientation.
	 * This means that FRotator(0, 0, 360).Equals(FRotator(0,0,0)) is true, because they represent the same final orientation.
	 *
	 * @param R The other rotator.
	 * @param Tolerance Error Tolerance.
	 * @return true if two rotators are equal, within specified tolerance, otherwise false.
	 */
	bool Equals(const FRotator& R, float Tolerance = KINDA_SMALL_NUMBER) const;
	/**
	 * Adds to each component of the rotator.
	 *
	 * @param DeltaPitch Change in pitch. (+/-)
	 * @param DeltaYaw Change in yaw. (+/-)
	 * @param DeltaRoll Change in roll. (+/-)
	 * @return Copy of rotator after addition.
	 */
	FRotator Add(float DeltaPitch, float DeltaYaw, float DeltaRoll);

	/**
	 * Returns the inverse of the rotator.
	 */
	FRotator GetInverse() const;
	/**
	 * Get the rotation, snapped to specified degree segments.
	 *
	 * @param RotGrid A Rotator specifying how to snap each component.
	 * @return Snapped version of rotation.
	 */
	FRotator GridSnap(const FRotator& RotGrid) const;

	/**
	 * Convert a rotation into a unit vector facing in its direction.
	 *
	 * @return Rotation as a unit direction vector.
	 */
	 FVector Vector() const;

	/**
	 * Get Rotation as a quaternion.
	 *
	 * @return Rotation as a quaternion.
	 */
	 class FQuat Quaternion() const;






	/**
	 * Convert a Rotator into floating-point Euler angles (in degrees). Rotator now stored in degrees.
	 *
	 * @return Rotation as a Euler angle vector.
	 */
	 FVector Euler() const
	 {
		 return FVector(Roll, Pitch, Yaw);
	 }

	/**
	 * Rotate a vector rotated by this rotator.
	 *
	 * @param V The vector to rotate.
	 * @return The rotated vector.
	 */
// 	 FVector RotateVector(const FVector& V) const
// 	 {
// 		 return FRotationMatrix(*this).TransformVector(V);
// 	 }

	/**
	 * Returns the vector rotated by the inverse of this rotator.
	 *
	 * @param V The vector to rotate.
	 * @return The rotated vector.
	 */
// 	 FVector UnrotateVector(const FVector& V) const
// 	 {
// 		 return FRotationMatrix(*this).GetTransposed().TransformVector(V);
// 	 }

	/**
	 * Gets the rotation values so they fall within the range [0,360]
	 *
	 * @return Clamped version of rotator.
	 */
	FRotator Clamp() const
	{
		return FRotator(ClampAxis(Pitch), ClampAxis(Yaw), ClampAxis(Roll));
	}

	/**
	 * Create a copy of this rotator and normalize, removes all winding and creates the "shortest route" rotation.
	 *
	 * @return Normalized copy of this rotator
	 */
	FRotator GetNormalized() const {
		FRotator Rot = *this;
		Rot.Normalize();
		return Rot;
	}

	/**
	 * Create a copy of this rotator and denormalize, clamping each axis to 0 - 360.
	 *
	 * @return Denormalized copy of this rotator
	 */
	FRotator GetDenormalized() const
	{
		FRotator Rot = *this;
		Rot.Pitch = ClampAxis(Rot.Pitch);
		Rot.Yaw = ClampAxis(Rot.Yaw);
		Rot.Roll = ClampAxis(Rot.Roll);
		return Rot;
	}

	/** Get a specific component of the vector, given a specific axis by enum */
	float GetComponentForAxis(EAxis::Type Axis) const
	{
		switch (Axis)
		{
		case EAxis::X:
			return Roll;
		case EAxis::Y:
			return Pitch;
		case EAxis::Z:
			return Yaw;
		default:
			return 0.f;
		}
	}

	/** Set a specified componet of the vector, given a specific axis by enum */
	void SetComponentForAxis(EAxis::Type Axis, float Component) {
		switch (Axis)
		{
		case EAxis::X:
			Roll = Component;
			break;
		case EAxis::Y:
			Pitch = Component;
			break;
		case EAxis::Z:
			Yaw = Component;
			break;
		}
	}

	/**
	 * In-place normalize, removes all winding and creates the "shortest route" rotation.
	 */
	void Normalize();


	/**
	 * Decompose this Rotator into a Winding part (multiples of 360) and a Remainder part.
	 * Remainder will always be in [-180, 180] range.
	 *
	 * @param Winding[Out] the Winding part of this Rotator
	 * @param Remainder[Out] the Remainder
	 */
	 void GetWindingAndRemainder(FRotator& Winding, FRotator& Remainder) const
	 {
		 //// YAW
		 Remainder.Yaw = NormalizeAxis(Yaw);

		 Winding.Yaw = Yaw - Remainder.Yaw;

		 //// PITCH
		 Remainder.Pitch = NormalizeAxis(Pitch);

		 Winding.Pitch = Pitch - Remainder.Pitch;

		 //// ROLL
		 Remainder.Roll = NormalizeAxis(Roll);

		 Winding.Roll = Roll - Remainder.Roll;
	 }

	/**
	* Return the manhattan distance in degrees between this Rotator and the passed in one.
	* @param Rotator[In] the Rotator we are comparing with.
	* @return Distance(Manhattan) between the two rotators.
	*/
	 float GetManhattanDistance(const FRotator& Rotator) const;

	/**
	* Return a Rotator that has the same rotation but has different degree values for Yaw, Pitch, and Roll.
	* This rotator should be within -180,180 range,
	* @return A Rotator with the same rotation but different degrees.
	*/
	 FRotator GetEquivalentRotator() const {
		 return FRotator(180.0f - Pitch, Yaw + 180.0f, Roll + 180.0f);
	 }

	/**
	* Modify if necessary the passed in rotator to be the closest rotator to it based upon it's equivalent.
	* This Rotator should be within (-180, 180], usually just constructed from a Matrix or a Quaternion.
	*
	* @param MakeClosest[In/Out] the Rotator we want to make closest to us. Should be between
	* (-180, 180]. This Rotator may change if we need to use different degree values to make it closer.
	*/
	 void SetClosestToMe(FRotator& MakeClosest) const {
		 FRotator OtherChoice = MakeClosest.GetEquivalentRotator();
		 float FirstDiff = GetManhattanDistance(MakeClosest);
		 float SecondDiff = GetManhattanDistance(OtherChoice);
		 if (SecondDiff < FirstDiff)
			 MakeClosest = OtherChoice;
	 }

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	 class	FString ToString() const;

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this Rotator.
	 *
	 * @return true if there are any non-finite values in this Rotator, otherwise false.
	 */
	bool ContainsNaN() const;

public:

	/**
	 * Clamps an angle to the range of [0, 360).
	 *
	 * @param Angle The angle to clamp.
	 * @return The clamped angle.
	 */
	static float ClampAxis(float Angle);

	/**
	 * Clamps an angle to the range of (-180, 180].
	 *
	 * @param Angle The Angle to clamp.
	 * @return The clamped angle.
	 */
	static float NormalizeAxis(float Angle)
	{
		// returns Angle in the range [0,360)
		Angle = ClampAxis(Angle);

		if (Angle > 180.f)
		{
			// shift to (-180,180]
			Angle -= 360.f;
		}

		return Angle;
	}

	/**
	 * Compresses a floating point angle into a byte.
	 *
	 * @param Angle The angle to compress.
	 * @return The angle as a byte.
	 */
	static uint8 CompressAxisToByte(float Angle);
	
	/**
	 * Decompress a word into a floating point angle.
	 *
	 * @param Angle The word angle.
	 * @return The decompressed angle.
	 */
	static float DecompressAxisFromByte(uint8 Angle)
	{
		// map [0->256) to [0->360)
		return (Angle * 360.f / 256.f);
	}


	/**
	 * Compress a floating point angle into a word.
	 *
	 * @param Angle The angle to compress.
	 * @return The decompressed angle.
	 */
	static uint16 CompressAxisToShort(float Angle);

	/**
	 * Decompress a short into a floating point angle.
	 *
	 * @param Angle The word angle.
	 * @return The decompressed angle.
	 */
	static float DecompressAxisFromShort(uint16 Angle)
	{
		// map [0->65536) to [0->360)
		return (Angle * 360.f / 65536.f);
	}


	/**
	 * Convert a vector of floating-point Euler angles (in degrees) into a Rotator. Rotator now stored in degrees
	 *
	 * @param Euler Euler angle vector.
	 * @return A rotator from a Euler angle.
	 */
	static  FRotator MakeFromEuler(const FVector& Euler)
	{
		return FRotator(Euler.Y, Euler.Z, Euler.X);
	}

};

