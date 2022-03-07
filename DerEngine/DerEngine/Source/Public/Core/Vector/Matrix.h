#pragma once
#include <vcruntime_string.h>
#include "MathType.h"
#include "Vector.h"
class FVector4D;
class FString;
class FRotator;
class FQuat;
class FMatrix
{
public:
	union
	{
		MS_ALIGN(16) float M[4][4] GCC_ALIGN(16);
	};

	//Identity matrix
	MS_ALIGN(16) static  const FMatrix Identity GCC_ALIGN(16);

	// Constructors.
	 FMatrix();

	/**
	 * Constructor.
	 *
	 * @param EForceInit Force Init Enum.
	 */
	 FMatrix(EForceInit);

	

	/**
	 * Constructor.
	 *
	 * @param InX X vector
	 * @param InY Y vector
	 * @param InZ Z vector
	 * @param InW W vector
	 */
	 FMatrix(const FVector4D& InX, const FVector4D& InY, const FVector4D& InZ, const FVector4D& InW);

	 FMatrix(const FVector& InX, const FVector& InY, const FVector& InZ, const FVector& InW);
	 
	// Set this to the identity matrix
	 void SetIdentity();

	/**
	 * Gets the result of multiplying a Matrix to this.
	 *
	 * @param Other The matrix to multiply this by.
	 * @return The result of multiplication.
	 */
	 FMatrix operator* (const FMatrix& Other) const;

	/**
	 * Multiply this by a matrix.
	 *
	 * @param Other the matrix to multiply by this.
	 * @return reference to this after multiply.
	 */
	 void operator*=(const FMatrix& Other);

	/**
	 * Gets the result of adding a matrix to this.
	 *
	 * @param Other The Matrix to add.
	 * @return The result of addition.
	 */
	 FMatrix operator+ (const FMatrix& Other) const;

	/**
	 * Adds to this matrix.
	 *
	 * @param Other The matrix to add to this.
	 * @return Reference to this after addition.
	 */
	 void operator+=(const FMatrix& Other);

	/**
	  * This isn't applying SCALE, just multiplying the value to all members - i.e. weighting
	  */
	 FMatrix operator* (float Other) const;

	/**
	 * Multiply this matrix by a weighting factor.
	 *
	 * @param other The weight.
	 * @return a reference to this after weighting.
	 */
	 void operator*=(float Other);

	/**
	 * Checks whether two matrix are identical.
	 *
	 * @param Other The other matrix.
	 * @return true if two matrix are identical, otherwise false.
	 */
	 bool operator==(const FMatrix& Other) const;

	/**
	 * Checks whether another Matrix is equal to this, within specified tolerance.
	 *
	 * @param Other The other Matrix.
	 * @param Tolerance Error Tolerance.
	 * @return true if two Matrix are equal, within specified tolerance, otherwise false.
	 */
	 bool Equals(const FMatrix& Other, float Tolerance = KINDA_SMALL_NUMBER) const;

	/**
	 * Checks whether another Matrix is not equal to this, within specified tolerance.
	 *
	 * @param Other The other Matrix.
	 * @return true if two Matrix are not equal, within specified tolerance, otherwise false.
	 */
	 bool operator!=(const FMatrix& Other) const;

	// Homogeneous transform.
	 FVector4D TransformFVector4(const FVector4D& V) const;

	/** Transform a location - will take into account translation part of the FMatrix. */
	 FVector4D TransformPosition(const FVector& V) const;

	/** Inverts the matrix and then transforms V - correctly handles scaling in this matrix. */
	 FVector InverseTransformPosition(const FVector& V) const;

	/**
	 *	Transform a direction vector - will not take into account translation part of the FMatrix.
	 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT.
	 */
	 FVector4D TransformVector(const FVector& V) const;

	/**
	 *	Transform a direction vector by the inverse of this matrix - will not take into account translation part.
	 *	If you want to transform a surface normal (or plane) and correctly account for non-uniform scaling you should use TransformByUsingAdjointT with adjoint of matrix inverse.
	 */
	 FVector InverseTransformVector(const FVector& V) const;

	// Transpose.

	 FMatrix GetTransposed() const;

	// @return determinant of this matrix.

	 float Determinant() const;

	/** @return the determinant of rotation 3x3 matrix */
	 float RotDeterminant() const;

	/** Fast path, doesn't check for nil matrices in final release builds */
	 FMatrix InverseFast() const;

	/** Fast path, and handles nil matrices. */
	 FMatrix Inverse() const;

	 FMatrix TransposeAdjoint() const;

	// NOTE: There is some compiler optimization issues with WIN64 that cause  to cause a crash
	// Remove any scaling from this matrix (ie magnitude of each row is 1) with error Tolerance
	 void RemoveScaling(float Tolerance = SMALL_NUMBER);

	// Returns matrix after RemoveScaling with error Tolerance
	 FMatrix GetMatrixWithoutScale(float Tolerance = SMALL_NUMBER) const;

	/** Remove any scaling from this matrix (ie magnitude of each row is 1) and return the 3D scale vector that was initially present with error Tolerance */
	 FVector ExtractScaling(float Tolerance = SMALL_NUMBER);

	/** return a 3D scale vector calculated from this matrix (where each component is the magnitude of a row vector) with error Tolerance. */
	 FVector GetScaleVector(float Tolerance = SMALL_NUMBER) const;

	// Remove any translation from this matrix
	 FMatrix RemoveTranslation() const;

	/** Returns a matrix with an additional translation concatenated. */
	 FMatrix ConcatTranslation(const FVector& Translation) const;

	/** Returns true if any element of this matrix is NaN */
	 bool ContainsNaN() const;

	/** Scale the translation part of the matrix by the supplied vector. */
	 void ScaleTranslation(const FVector& Scale3D);

	/** @return the maximum magnitude of any row of the matrix. */
	 float GetMaximumAxisScale() const;

	/** Apply Scale to this matrix **/
	 FMatrix ApplyScale(float Scale) const;

	// @return the origin of the co-ordinate system
	 FVector GetOrigin() const;

	/**
	 * get axis of this matrix scaled by the scale of the matrix
	 *
	 * @param i index into the axis of the matrix
	 * @ return vector of the axis
	 */
	 FVector GetScaledAxis(EAxis::Type Axis) const;

	/**
	 * get axes of this matrix scaled by the scale of the matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	 void GetScaledAxes(FVector& X, FVector& Y, FVector& Z) const;

	/**
	 * get unit length axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @return vector of the axis
	 */
	 FVector GetUnitAxis(EAxis::Type Axis) const;

	/**
	 * get unit length axes of this matrix
	 *
	 * @param X axes returned to this param
	 * @param Y axes returned to this param
	 * @param Z axes returned to this param
	 */
	 void GetUnitAxes(FVector& X, FVector& Y, FVector& Z) const;

	/**
	 * set an axis of this matrix
	 *
	 * @param i index into the axis of the matrix
	 * @param Axis vector of the axis
	 */
	 void SetAxis(int i, const FVector& Axis);

	// Set the origin of the coordinate system to the given vector
	 void SetOrigin(const FVector& NewOrigin);

	/**
	 * Update the axes of the matrix if any value is NULL do not update that axis
	 *
	 * @param Axis0 set matrix row 0
	 * @param Axis1 set matrix row 1
	 * @param Axis2 set matrix row 2
	 * @param Origin set matrix row 3
	 */
	 void SetAxes(FVector* Axis0 = NULL, FVector* Axis1 = NULL, FVector* Axis2 = NULL, FVector* Origin = NULL);


	/**
	 * get a column of this matrix
	 *
	 * @param i index into the column of the matrix
	 * @return vector of the column
	 */
	 FVector GetColumn(int i) const;

	/**
	 * Set a column of this matrix
	 *
	 * @param i index of the matrix column
	 * @param Value new value of the column
	 */
	 void SetColumn(int i, FVector Value);

	/** @return rotator representation of this matrix */
	 FRotator Rotator() const;

	/**
	 * Transform a rotation matrix into a quaternion.
	 *
	 * @warning rotation part will need to be unit length for this to be right!
	 */
	 FQuat ToQuat() const;

	// Frustum plane extraction.
	/** @param OutPlane the near plane of the Frustum of this matrix */
	 bool GetFrustumNearPlane(FVector4D& OutPlane) const;

	/** @param OutPlane the far plane of the Frustum of this matrix */
	 bool GetFrustumFarPlane(FVector4D& OutPlane) const;

	/** @param OutPlane the left plane of the Frustum of this matrix */
	 bool GetFrustumLeftPlane(FVector4D& OutPlane) const;

	/** @param OutPlane the right plane of the Frustum of this matrix */
	 bool GetFrustumRightPlane(FVector4D& OutPlane) const;

	/** @param OutPlane the top plane of the Frustum of this matrix */
	 bool GetFrustumTopPlane(FVector4D& OutPlane) const;

	/** @param OutPlane the bottom plane of the Frustum of this matrix */
	 bool GetFrustumBottomPlane(FVector4D& OutPlane) const;

	/**
	 * Utility for mirroring this transform across a certain plane, and flipping one of the axis as well.
	 */
	 void Mirror(EAxis::Type MirrorAxis, EAxis::Type FlipAxis);

	/**
	 * Get a textual representation of the vector.
	 *
	 * @return Text describing the vector.
	 */
	 FString ToString() const;


	/** For debugging purpose, could be changed */
	 uint32 ComputeHash() const;

	/**
	 * Serializes the Matrix.
	 *
	 * @param Ar Reference to the serialization archive.
	 * @param M Reference to the matrix being serialized.
	 * @return Reference to the Archive after serialization.
	 */
	

	/**
	 * Convert this Atom to the 3x4 transpose of the transformation matrix.
	 */
	void To3x4MatrixTranspose(float* Out) const
	{
		const float* RESTRICT Src = &(M[0][0]);
		float* RESTRICT Dest = Out;

		Dest[0] = Src[0];   // [0][0]
		Dest[1] = Src[4];   // [1][0]
		Dest[2] = Src[8];   // [2][0]
		Dest[3] = Src[12];  // [3][0]

		Dest[4] = Src[1];   // [0][1]
		Dest[5] = Src[5];   // [1][1]
		Dest[6] = Src[9];   // [2][1]
		Dest[7] = Src[13];  // [3][1]

		Dest[8] = Src[2];   // [0][2]
		Dest[9] = Src[6];   // [1][2]
		Dest[10] = Src[10]; // [2][2]
		Dest[11] = Src[14]; // [3][2]
	}
};

class FRotationTranslationMatrix
	: public FMatrix
{
public:

	/**
	 * Constructor.
	 *
	 * @param Rot rotation
	 * @param Origin translation to apply
	 */
	FRotationTranslationMatrix(const FRotator& Rot, const FVector& Origin);

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static FMatrix Make(const FRotator& Rot, const FVector& Origin)
	{
		return FRotationTranslationMatrix(Rot, Origin);
	}
};



class FRotationAboutPointMatrix
	: public FRotationTranslationMatrix
{
public:

	/**
	 * Constructor.
	 *
	 * @param Rot rotation
	 * @param Origin about which to rotate.
	 */
	FRotationAboutPointMatrix(const FRotator& Rot, const FVector& Origin);

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static FMatrix Make(const FRotator& Rot, const FVector& Origin)
	{
		return FRotationAboutPointMatrix(Rot, Origin);
	}

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static  FMatrix Make(const FQuat& Rot, const FVector& Origin);
};


/** Rotation and translation matrix using quaternion rotation */
class FQuatRotationTranslationMatrix
	: public FMatrix
{
public:

	/** Constructor
	*
	* @param Q rotation
	* @param Origin translation to apply
	*/
	FQuatRotationTranslationMatrix(const FQuat& Q, const FVector& Origin);

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static FMatrix Make(const FQuat& Q, const FVector& Origin)
	{
		return FQuatRotationTranslationMatrix(Q, Origin);
	}
};


/** Rotation matrix using quaternion rotation */
class FQuatRotationMatrix
	: public FQuatRotationTranslationMatrix
{
public:

	/** Constructor
	*
	* @param Q rotation
	*/
	FQuatRotationMatrix(const FQuat& Q)
		: FQuatRotationTranslationMatrix(Q, FVector::ZeroVector)
	{
	}

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static FMatrix Make(const FQuat& Q)
	{
		return FQuatRotationMatrix(Q);
	}
};




/** Rotation matrix no translation */
class FRotationMatrix
	: public FRotationTranslationMatrix
{
public:

	/**
	 * Constructor.
	 *
	 * @param Rot rotation
	 */
	FRotationMatrix(const FRotator& Rot)
		: FRotationTranslationMatrix(Rot, FVector::ZeroVector)
	{ }

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static FMatrix Make(FRotator const& Rot)
	{
		return FRotationMatrix(Rot);
	}

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static  FMatrix Make(FQuat const& Rot);

	/** Builds a rotation matrix given only a XAxis. Y and Z are unspecified but will be orthonormal. XAxis need not be normalized. */
	static  FMatrix MakeFromX(FVector const& XAxis);

	/** Builds a rotation matrix given only a YAxis. X and Z are unspecified but will be orthonormal. YAxis need not be normalized. */
	static  FMatrix MakeFromY(FVector const& YAxis);

	/** Builds a rotation matrix given only a ZAxis. X and Y are unspecified but will be orthonormal. ZAxis need not be normalized. */
	static  FMatrix MakeFromZ(FVector const& ZAxis);

	/** Builds a matrix with given X and Y axes. X will remain fixed, Y may be changed minimally to enforce orthogonality. Z will be computed. Inputs need not be normalized. */
	static  FMatrix MakeFromXY(FVector const& XAxis, FVector const& YAxis);

	/** Builds a matrix with given X and Z axes. X will remain fixed, Z may be changed minimally to enforce orthogonality. Y will be computed. Inputs need not be normalized. */
	static  FMatrix MakeFromXZ(FVector const& XAxis, FVector const& ZAxis);

	/** Builds a matrix with given Y and X axes. Y will remain fixed, X may be changed minimally to enforce orthogonality. Z will be computed. Inputs need not be normalized. */
	static  FMatrix MakeFromYX(FVector const& YAxis, FVector const& XAxis);

	/** Builds a matrix with given Y and Z axes. Y will remain fixed, Z may be changed minimally to enforce orthogonality. X will be computed. Inputs need not be normalized. */
	static  FMatrix MakeFromYZ(FVector const& YAxis, FVector const& ZAxis);

	/** Builds a matrix with given Z and X axes. Z will remain fixed, X may be changed minimally to enforce orthogonality. Y will be computed. Inputs need not be normalized. */
	static  FMatrix MakeFromZX(FVector const& ZAxis, FVector const& XAxis);

	/** Builds a matrix with given Z and Y axes. Z will remain fixed, Y may be changed minimally to enforce orthogonality. X will be computed. Inputs need not be normalized. */
	static  FMatrix MakeFromZY(FVector const& ZAxis, FVector const& YAxis);
};
/** Combined Scale rotation and translation matrix */
class FScaleRotationTranslationMatrix
	: public FMatrix
{
public:

	/**
	 * Constructor.
	 *
	 * @param Scale scale to apply to matrix
	 * @param Rot rotation
	 * @param Origin translation to apply
	 */
	FScaleRotationTranslationMatrix(const FVector& Scale, const FRotator& Rot, const FVector& Origin);
};

class FScaleMatrix
	: public FMatrix
{
public:

	/**
	 * @param Scale uniform scale to apply to matrix.
	 */
	FScaleMatrix(float Scale);

	/**
	 * @param Scale Non-uniform scale to apply to matrix.
	 */
	FScaleMatrix(const FVector& Scale);

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static FMatrix Make(float Scale)
	{
		return FScaleMatrix(Scale);
	}

	/** Matrix factory. Return an FMatrix so we don't have type conversion issues in expressions. */
	static FMatrix Make(const FVector& Scale)
	{
		return FScaleMatrix(Scale);
	}
};


