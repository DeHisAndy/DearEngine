// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "..\FunctionLibrary\Math.h"


/**
 * Floating point quaternion that can represent a rotation about an axis in 3-D space.
 * The X, Y, Z, W components also double as the Axis/Angle format.
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 * Note that this is the opposite order of FTransform multiplication.
 *
 * Example: LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
 * Example: LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
 */
class FVector;
class FMatrix;
class FString;
MS_ALIGN(16) class FQuat
{
public:

	/** The quaternion's X-component. */
	float X;

	/** The quaternion's Y-component. */
	float Y;

	/** The quaternion's Z-component. */
	float Z;

	/** The quaternion's W-component. */
	float W;

public:

	/** Identity quaternion. */
	static  const FQuat Identity;

public:

	/** Default constructor (no initialization). */
	 FQuat() { }

	/**
	 * Creates and initializes a new quaternion, with the W component either 0 or 1.
	 *
	 * @param EForceInit Force init enum: if equal to ForceInitToZero then W is 0, otherwise W = 1 (creating an identity transform)
	 */
	explicit  FQuat(EForceInit);

	/**
	 * Constructor.
	 *
	 * @param InX X component of the quaternion
	 * @param InY Y component of the quaternion
	 * @param InZ Z component of the quaternion
	 * @param InW W component of the quaternion
	 */
	 FQuat(float InX, float InY, float InZ, float InW);

	/**
	 * Creates and initializes a new quaternion from the XYZW values in the given __m128.
	 *
	 * @param V XYZW components of the quaternion packed into a single __m128
	 */
	explicit  FQuat(const __m128& V);

	/**
	 * Creates and initializes a new quaternion from the given matrix.
	 *
	 * @param M The rotation matrix to initialize from.
	 */
	explicit FQuat(const FMatrix& M);

	/**
	 * Creates and initializes a new quaternion from the given rotator.
	 *
	 * @param R The rotator to initialize from.
	 */
	explicit FQuat(const class FRotator& R);

	/**
	 * Creates and initializes a new quaternion from the a rotation around the given axis.
	 *
	 * @param Axis assumed to be a normalized vector
	 * @param Angle angle to rotate above the given axis (in radians)
	 */
	FQuat(FVector Axis, float AngleRad);

public:

	/**
	 * Gets the result of adding a Quaternion to this.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to add.
	 * @return The result of addition.
	 */
	 FQuat operator+(const FQuat& Q) const;

	/**
	 * Adds to this quaternion.
	 * This is a component-wise addition; composing quaternions should be done via multiplication.
	 *
	 * @param Other The quaternion to add to this.
	 * @return Result after addition.
	 */
	 FQuat operator+=(const FQuat& Q);

	/**
	 * Gets the result of subtracting a Quaternion to this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The Quaternion to subtract.
	 * @return The result of subtraction.
	 */
	 FQuat operator-(const FQuat& Q) const;

	/**
	 * Checks whether another Quaternion is equal to this, within specified tolerance.
	 *
	 * @param Q The other Quaternion.
	 * @param Tolerance Error tolerance for comparison with other Quaternion.
	 * @return true if two Quaternions are equal, within specified tolerance, otherwise false.
	 */
	 bool Equals(const FQuat& Q, float Tolerance = KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether this Quaternion is an Identity Quaternion.
	 * Assumes Quaternion tested is normalized.
	 *
	 * @param Tolerance Error tolerance for comparison with Identity Quaternion.
	 * @return true if Quaternion is a normalized Identity Quaternion.
	 */
	 bool IsIdentity(float Tolerance = SMALL_NUMBER) const;

	/**
	 * Subtracts another quaternion from this.
	 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
	 *
	 * @param Q The other quaternion.
	 * @return reference to this after subtraction.
	 */
	 FQuat operator-=(const FQuat& Q);

	/**
	 * Gets the result of multiplying this by another quaternion (this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q The Quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	 FQuat operator*(const FQuat& Q) const;

	/**
	 * Multiply this by a quaternion (this = this * Q).
	 *
	 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
	 * first applies B then A to any subsequent transformation (right first, then left).
	 *
	 * @param Q the quaternion to multiply this by.
	 * @return The result of multiplication (this * Q).
	 */
	 FQuat operator*=(const FQuat& Q);

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 * @see RotateVector
	 */
	FVector operator*(const FVector& V) const;

	/**
	 * Multiply this by a matrix.
	 * This matrix conversion came from
	 * http://www.m-hikari.com/ija/ija-password-2008/ija-password17-20-2008/aristidouIJA17-20-2008.pdf
	 * used for non-uniform scaling transform.
	 *
	 * @param M Matrix to multiply by.
	 * @return Matrix result after multiplication.
	 */
	FMatrix operator*(const FMatrix& M) const;

	/**
	 * Multiply this quaternion by a scaling factor.
	 *
	 * @param Scale The scaling factor.
	 * @return a reference to this after scaling.
	 */
	 FQuat operator*=(const float Scale);

	/**
	 * Get the result of scaling this quaternion.
	 *
	 * @param Scale The scaling factor.
	 * @return The result of scaling.
	 */
	 FQuat operator*(const float Scale) const;

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return a reference to this after scaling.
	 */
	 FQuat operator/=(const float Scale);

	/**
	 * Divide this quaternion by scale.
	 *
	 * @param Scale What to divide by.
	 * @return new Quaternion of this after division by scale.
	 */
	 FQuat operator/(const float Scale) const;

	/**
	 * Identical implementation for FQuat properties.
	 * Avoids intrinsics to remain consistent with previous per-property comparison.
	 */
	bool Identical(const FQuat* Q, const uint32 PortFlags) const;

	/**
	 * Checks whether two quaternions are identical.
	 * This is an exact comparison per-component;see Equals() for a comparison
	 * that allows for a small error tolerance and flipped axes of rotation.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are identical, otherwise false.
	 * @see Equals
	 */
	bool operator==(const FQuat& Q) const;

	/**
	 * Checks whether two quaternions are not identical.
	 *
	 * @param Q The other quaternion.
	 * @return true if two quaternion are not identical, otherwise false.
	 */
	bool operator!=(const FQuat& Q) const;

	/**
	 * Calculates dot product of two quaternions.
	 *
	 * @param Q The other quaternions.
	 * @return The dot product.
	 */
	float operator|(const FQuat& Q) const;

public:

	/**
	 * Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
	 *
	 * @param Euler the Euler angles
	 * @return constructed FQuat
	 */
	static  FQuat MakeFromEuler(const FVector& Euler);

	/** Convert a Quaternion into floating-point Euler angles (in degrees). */
	 FVector Euler() const;

	/**
	 * Normalize this quaternion if it is large enough.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	 void Normalize(float Tolerance = SMALL_NUMBER);

	/**
	 * Get a normalized copy of this quaternion.
	 * If it is too small, returns an identity quaternion.
	 *
	 * @param Tolerance Minimum squared length of quaternion for normalization.
	 */
	 FQuat GetNormalized(float Tolerance = SMALL_NUMBER) const;

	// Return true if this quaternion is normalized
	bool IsNormalized() const;

	/**
	 * Get the length of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	 float Size() const;

	/**
	 * Get the length squared of this quaternion.
	 *
	 * @return The length of this quaternion.
	 */
	 float SizeSquared() const;


	/** Get the angle of this quaternion */
	 float GetAngle() const;

	/**
	 * get the axis and angle of rotation of this quaternion
	 *
	 * @param Axis{out] vector of axis of the quaternion
	 * @param Angle{out] angle of the quaternion
	 * @warning : assumes normalized quaternions.
	 */
	void ToAxisAndAngle(FVector& Axis, float& Angle) const;

	/**
	 * Get the swing and twist decomposition for a specified axis
	 *
	 * @param InTwistAxis Axis to use for decomposition
	 * @param OutSwing swing component quaternion
	 * @param OutTwist Twist component quaternion
	 * @warning assumes normalized quaternion and twist axis
	 */
	 void ToSwingTwist(const FVector& InTwistAxis, FQuat& OutSwing, FQuat& OutTwist) const;

	/**
	 * Get the twist angle (in radians) for a specified axis
	 *
	 * @param TwistAxis Axis to use for decomposition
	 * @return Twist angle (in radians)
	 * @warning assumes normalized quaternion and twist axis
	 */
	 float GetTwistAngle(const FVector& TwistAxis) const;

	/**
	 * Rotate a vector by this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation
	 */
	FVector RotateVector(FVector V) const;

	/**
	 * Rotate a vector by the inverse of this quaternion.
	 *
	 * @param V the vector to be rotated
	 * @return vector after rotation by the inverse of this quaternion.
	 */
	FVector UnrotateVector(FVector V) const;

	/**
	 * @return quaternion with W=0 and V=theta*v.
	 */
	 FQuat Log() const;

	/**
	 * @note Exp should really only be used after Log.
	 * Assumes a quaternion with W=0 and V=theta*v (where |v| = 1).
	 * Exp(q) = (sin(theta)*v, cos(theta))
	 */
	 FQuat Exp() const;

	/**
	 * @return inverse of this quaternion
	 */
	 FQuat Inverse() const;

	/**
	 * Enforce that the delta between this Quaternion and another one represents
	 * the shortest possible rotation angle
	 */
	void EnforceShortestArcWith(const FQuat& OtherQuat);

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	 FVector GetAxisX() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	 FVector GetAxisY() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	 FVector GetAxisZ() const;

	/** Get the forward direction (X axis) after it has been rotated by this Quaternion. */
	 FVector GetForwardVector() const;

	/** Get the right direction (Y axis) after it has been rotated by this Quaternion. */
	 FVector GetRightVector() const;

	/** Get the up direction (Z axis) after it has been rotated by this Quaternion. */
	 FVector GetUpVector() const;

	/** Convert a rotation into a unit vector facing in its direction. Equivalent to GetForwardVector(). */
	 FVector Vector() const;

	/** Get the FRotator representation of this Quaternion. */
	class FRotator Rotator() const;

	/**
	 * Get the axis of rotation of the Quaternion.
	 * This is the axis around which rotation occurs to transform the canonical coordinate system to the target orientation.
	 * For the identity Quaternion which has no such rotation, FVector(1,0,0) is returned.
	 */
	 FVector GetRotationAxis() const;

	/** Find the angular distance between two rotation quaternions (in radians) */
	 float AngularDistance(const FQuat& Q) const;

	/**
	 * Utility to check if there are any non-finite values (NaN or Inf) in this Quat.
	 *
	 * @return true if there are any non-finite values in this Quaternion, otherwise false.
	 */
	bool ContainsNaN() const;

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	class FString ToString() const;


public:

	 void DiagnosticCheckNaN() const {}
	 void DiagnosticCheckNaN(const wchar_t* Message) const {}


public:

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static  FQuat FindBetween(const FVector& Vector1, const FVector& Vector2)
	{
		return FindBetweenVectors(Vector1, Vector2);
	}

	/**
	 * Generates the 'smallest' (geodesic) rotation between two normals (assumed to be unit length).
	 */
	static  FQuat FindBetweenNormals(const FVector& Normal1, const FVector& Normal2);

	/**
	 * Generates the 'smallest' (geodesic) rotation between two vectors of arbitrary length.
	 */
	static  FQuat FindBetweenVectors(const FVector& Vector1, const FVector& Vector2);

	/**
	 * Error measure (angle) between two quaternions, ranged [0..1].
	 * Returns the hypersphere-angle between two quaternions; alignment shouldn't matter, though
	 * @note normalized input is expected.
	 */
	static  float Error(const FQuat& Q1, const FQuat& Q2);

	/**
	 * FQuat::Error with auto-normalization.
	 */
	static  float ErrorAutoNormalize(const FQuat& A, const FQuat& B);

	/**
	 * Fast Linear Quaternion Interpolation.
	 * Result is NOT normalized.
	 */
	static  FQuat FastLerp(const FQuat& A, const FQuat& B, const float Alpha);

	/**
	 * Bi-Linear Quaternion interpolation.
	 * Result is NOT normalized.
	 */
	static  FQuat FastBilerp(const FQuat& P00, const FQuat& P10, const FQuat& P01, const FQuat& P11, float FracX, float FracY);


	/** Spherical interpolation. Will correct alignment. Result is NOT normalized. */
	static  FQuat Slerp_NotNormalized(const FQuat& Quat1, const FQuat& Quat2, float Slerp);

	/** Spherical interpolation. Will correct alignment. Result is normalized. */
	static  FQuat Slerp(const FQuat& Quat1, const FQuat& Quat2, float Slerp)
	{
		return Slerp_NotNormalized(Quat1, Quat2, Slerp).GetNormalized();
	}

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is NOT normalized.
	 */
	static  FQuat SlerpFullPath_NotNormalized(const FQuat& quat1, const FQuat& quat2, float Alpha);

	/**
	 * Simpler Slerp that doesn't do any checks for 'shortest distance' etc.
	 * We need this for the cubic interpolation stuff so that the multiple Slerps dont go in different directions.
	 * Result is normalized.
	 */
	static  FQuat SlerpFullPath(const FQuat& quat1, const FQuat& quat2, float Alpha)
	{
		return SlerpFullPath_NotNormalized(quat1, quat2, Alpha).GetNormalized();
	}

	/**
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 * This will correct alignment by ensuring that the shortest path is taken.
	 */
	static  FQuat Squad(const FQuat& quat1, const FQuat& tang1, const FQuat& quat2, const FQuat& tang2, float Alpha);

	/**
	 * Simpler Squad that doesn't do any checks for 'shortest distance' etc.
	 * Given start and end quaternions of quat1 and quat2, and tangents at those points tang1 and tang2, calculate the point at Alpha (between 0 and 1) between them. Result is normalized.
	 */
	static  FQuat SquadFullPath(const FQuat& quat1, const FQuat& tang1, const FQuat& quat2, const FQuat& tang2, float Alpha);

	/**
	 * Calculate tangents between given points
	 *
	 * @param PrevP quaternion at P-1
	 * @param P quaternion to return the tangent
	 * @param NextP quaternion P+1
	 * @param Tension @todo document
	 * @param OutTan Out control point
	 */
	static  void CalcTangents(const FQuat& PrevP, const FQuat& P, const FQuat& NextP, float Tension, FQuat& OutTan);

public:


} GCC_ALIGN(16);


template<class U>
static	 FQuat Lerp(const FQuat& A, const FQuat& B, const U& Alpha)
{
	return FQuat::Slerp(A, B, Alpha);
}

template<class U>
static	 FQuat BiLerp(const FQuat& P00, const FQuat& P10, const FQuat& P01, const FQuat& P11, float FracX, float FracY)
{
	FQuat Result;

	Result = Lerp(
		FQuat::Slerp_NotNormalized(P00, P10, FracX),
		FQuat::Slerp_NotNormalized(P01, P11, FracX),
		FracY
	);

	return Result;
}

template<class U>
static	 FQuat CubicInterp(const FQuat& P0, const FQuat& T0, const FQuat& P1, const FQuat& T1, const U& A)
{
	return FQuat::Squad(P0, T0, P1, T1, A);
}