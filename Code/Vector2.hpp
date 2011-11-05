//-----------------------------------------------------------------------------------------------
// Vector2.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_Vector2__
#define __include_Vector2__
#pragma once

#include "MathBase.hpp"


//-----------------------------------------------------------------------------------------------
// Forward class declarations for classes we need to reference
//
class IntVector2;


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Vector2
//
/////////////////////////////////////////////////////////////////////////////////////////////////
class Vector2
{
public:
	float x;
	float y;

	static const Vector2 ZERO;
	static const Vector2 ONE;

public:
	// Constructors
	~Vector2() {}
	Vector2() {}
	Vector2( const Vector2& copy );
	explicit Vector2( const float initialX, const float initialY );
	explicit Vector2( const std::string& vectorAsString );
	explicit Vector2( const IntVector2& intVector2 );

	// Mutators
	Vector2& SetXY( const float newX, const float newY );
	Vector2& SetUnitLengthAndYawRadians( double yawRadians );
	Vector2& SetUnitLengthAndYawDegrees( double yawDegrees );
	Vector2& SetLengthAndYawRadians( float magnitude, double yawRadians );
	Vector2& SetLengthAndYawDegrees( float magnitude, double yawDegrees );
	Vector2& SetFromString( const std::string& vectorAsString );
	const float Normalize();
	void Normalize_FastApproximate();
	const float SetLength( const float newLength );
	void SetLength_FastApproximate( const float newLength );
	Vector2& Clamp( const Vector2& min, const Vector2& max );
	Vector2& InterpolateTo( const Vector2& lerpTo, const float lerpToFraction );
	Vector2& MultiplyComponents( const Vector2& multiplyByThis );
	Vector2& RotateDegrees( const double rotateByThisManyDegrees );
	Vector2& RotateRadians( const double rotateByThisManyRadians );
	Vector2& Rotate90Degrees();
	Vector2& RotateMinus90Degrees();
	Vector2& Reverse();
	Vector2& StretchToAtLeast( const Vector2& minSize );

	// Accessors
	void GetXY( float& getX_out, float& getY_out ) const;
	const float CalcLengthSquared() const;
	const float CalcLength() const;
	const Vector2 CalcAsNormalized() const;
	const Vector2 CalcAsNormalized_FastApproximate() const;
	const Vector2 CalcAsFixedLength( const float fixedLength ) const;
	const Vector2 CalcAsFixedLength_FastApproximate( const float fixedLength ) const;
	const float CalcAspect() const;
	const float CalcSlope() const;
	const double CalcYawRadians() const;
	const double CalcYawDegrees() const;
	const float CalcMaxComponent() const;
	const float CalcMinComponent() const;
	const bool IsApproximatelyZero() const;
	const bool IsApproximatelyOne() const;
	const bool IsApproximatelyNormalized() const;
	const float CalcScaleRequiredToFitInsideBox( const Vector2& sizeOfBoxIWantToFitInside ) const;
	const Vector2 GetRotatedByDegrees( const double returnValueIsRotatedByThisManyDegrees ) const;
	const Vector2 GetRotatedByRadians( const double returnValueIsRotatedByThisManyRadians ) const;
	const Vector2 GetRotated90Degrees() const;
	const Vector2 GetRotatedMinus90Degrees() const;
	const Vector2 GetReversed() const;

	// Operators
	Vector2& operator = ( const Vector2& vectorToCopyFrom );
	Vector2& operator += ( const Vector2& vectorToAdd );
	Vector2& operator -= ( const Vector2& vectorToSubtract );
	Vector2& operator *= ( const float scale );
	Vector2& operator /= ( const float divisor );
	const Vector2 operator - () const;
	const Vector2 operator - ( const Vector2& vectorToSubtract ) const;
	const Vector2 operator + ( const Vector2& vectorToAdd ) const;
	const Vector2 operator * ( const float scale ) const;
	const Vector2 operator / ( const float divisor ) const;
	const float operator [] ( const int index ) const;
	float& operator [] ( const int index );
	const bool operator == ( const Vector2& vectorToCompareAgainst ) const;
	const bool operator != ( const Vector2& vectorToCompareAgainst ) const;

	// Standalone friend functions
	friend const Vector2 Interpolate( const Vector2& lerpFrom, const Vector2& lerpTo, const float lerpToFraction );
	friend const Vector2 operator * ( const float scale, const Vector2& vectorToScale );
	friend const bool IsEpsilonEqual( const Vector2& vectorA, const Vector2& vectorB, const float epsilon );
	friend const float DotProduct( const Vector2& vectorA, const Vector2& vectorB );
	friend const float CrossProduct( const Vector2& vectorA, const Vector2& vectorB );
	friend const Vector2 MultiplyComponents( const Vector2& vectorA, const Vector2& vectorB );
};


//-----------------------------------------------------------------------------------------------
std::string GetTypedObjectAsString( const Vector2& typedObject, std::string* out_optionalTypeNameAsString );


//-----------------------------------------------------------------------------------------------
inline Vector2::Vector2( const Vector2& copy )
: x( copy.x )
, y( copy.y )
{
}


//-----------------------------------------------------------------------------------------------
inline Vector2::Vector2( const float initialX, const float initialY )
: x( initialX )
, y( initialY )
{
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::SetXY( const float newX, const float newY )
{
	x = newX;
	y = newY;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::SetUnitLengthAndYawRadians( double yawRadians )
{
	x = (float) cos( yawRadians );
	y = (float) sin( yawRadians );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::SetUnitLengthAndYawDegrees( double yawDegrees )
{
	x = (float) cos( DEG2RAD( yawDegrees ) );
	y = (float) sin( DEG2RAD( yawDegrees ) );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::SetLengthAndYawRadians( float magnitude, double yawRadians )
{
	x = magnitude * (float) cos( yawRadians );
	y = magnitude * (float) sin( yawRadians );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::SetLengthAndYawDegrees( float magnitude, double yawDegrees )
{
	x = magnitude * (float) cos( DEG2RAD( yawDegrees ) );
	y = magnitude * (float) sin( DEG2RAD( yawDegrees ) );
	return *this;
}


//-----------------------------------------------------------------------------------------------
//inline Vector2& Vector2::SetFromString( const std::string& vectorAsString )
//{
//}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::Normalize()
{
	const float oldLength = sqrt( x*x + y*y );
	if( oldLength )
	{
		const float inverseLength = 1.0f / oldLength;
		x *= inverseLength;
		y *= inverseLength;
	}

	return oldLength;
}


//-----------------------------------------------------------------------------------------------
inline void Vector2::Normalize_FastApproximate()
{
	const float oldLengthSquared = x*x + y*y;
	if( oldLengthSquared )
	{
		const float oldInvLength = FastApproximateInverseSqrt( oldLengthSquared );
		x *= oldInvLength;
		y *= oldInvLength;
	}
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::SetLength( const float newLength )
{
	const float oldLength = sqrt( x*x + y*y );
	if( oldLength )
	{
		const float scale = newLength / oldLength;
		x *= scale;
		y *= scale;
	}

	return oldLength;
}


//-----------------------------------------------------------------------------------------------
inline void Vector2::SetLength_FastApproximate( const float newLength )
{
	const float oldLengthSquared = x*x + y*y;
	if( oldLengthSquared )
	{
		const float scale = newLength * FastApproximateInverseSqrt( oldLengthSquared );
		x *= scale;
		y *= scale;
	}
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::Clamp( const Vector2& min, const Vector2& max )
{
	if( x > max.x )
		x = max.x;
	else if( x < min.x )
		x = min.x;

	if( y > max.y )
		y = max.y;
	else if( y < min.y )
		y = min.y;

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::InterpolateTo( const Vector2& lerpTo, const float lerpToFraction )
{
	const float lerpFromFraction = 1.0f - lerpToFraction;
	x = (lerpFromFraction * x) + (lerpToFraction * lerpTo.x);
	y = (lerpFromFraction * y) + (lerpToFraction * lerpTo.y);
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::MultiplyComponents( const Vector2& multiplyByThis )
{
	x *= multiplyByThis.x;
	y *= multiplyByThis.y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::RotateDegrees( const double rotateByThisManyDegrees )
{
	const double radians = DEG2RAD( rotateByThisManyDegrees );
	const float cosTheta = (float) cos( radians );
	const float sinTheta = (float) sin( radians );

	const float newX = (x * cosTheta) - (y * sinTheta);
	y = (x * sinTheta) + (y * cosTheta);
	x = newX;

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::RotateRadians( const double rotateByThisManyRadians )
{
	const float cosTheta = (float) cos( rotateByThisManyRadians );
	const float sinTheta = (float) sin( rotateByThisManyRadians );

	const float newX = (x * cosTheta) - (y * sinTheta);
	y = (x * sinTheta) + (y * cosTheta);
	x = newX;

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::Rotate90Degrees()
{
	const float newX = -y;
	y = x;
	x = newX;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::RotateMinus90Degrees()
{
	const float newX = y;
	y = -x;
	x = newX;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::Reverse()
{
	x = -x;
	y = -y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::StretchToAtLeast( const Vector2& minSize )
{
	if( minSize.x > x )
		x = minSize.x;

	if( minSize.y > y )
		y = minSize.y;

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline void Vector2::GetXY( float& getX_out, float& getY_out ) const
{
	getX_out = x;
	getY_out = y;
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::CalcLengthSquared() const
{
	return( x*x + y*y );
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::CalcLength() const
{
	return sqrt( x*x + y*y );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::CalcAsNormalized() const
{
	const float lengthSquared = x*x + y*y;
	if( lengthSquared == 0.f )
	{
		return Vector2::ZERO;
	}
	else
	{
		const float inverseLength = 1.f / sqrt( lengthSquared );
		return Vector2( x * inverseLength, y * inverseLength );
	}
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::CalcAsNormalized_FastApproximate() const
{
	const float lengthSquared = x*x + y*y;
	if( lengthSquared == 0.f )
	{
		return Vector2::ZERO;
	}
	else
	{
		const float inverseLength = FastApproximateInverseSqrt( lengthSquared );
		return Vector2( x * inverseLength, y * inverseLength );
	}
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::CalcAsFixedLength( const float fixedLength ) const
{
	const float oldLengthSquared = x*x + y*y;
	if( oldLengthSquared == 0.f )
	{
		return Vector2::ZERO;
	}
	else
	{
		const float scale = fixedLength / sqrt( oldLengthSquared );
		return Vector2( x * scale, y * scale );
	}
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::CalcAsFixedLength_FastApproximate( const float fixedLength ) const
{
	const float oldLengthSquared = x*x + y*y;
	if( oldLengthSquared == 0.f )
	{
		return Vector2::ZERO;
	}
	else
	{
		const float scale = fixedLength * FastApproximateInverseSqrt( oldLengthSquared );
		return Vector2( x * scale, y * scale );
	}
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::CalcAspect() const
{
	if( y == 0.0f )
		return 0.0f;
	else
		return( x / y );
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::CalcSlope() const
{
	if( x == 0.0f )
		return 0.0f;
	else
		return( y / x );
}


//-----------------------------------------------------------------------------------------------
inline const double Vector2::CalcYawRadians() const
{
	return atan2( y, x );
}


//-----------------------------------------------------------------------------------------------
inline const double Vector2::CalcYawDegrees() const
{
	return RAD2DEG( atan2( y, x ) );
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::CalcMaxComponent() const
{
	if( x > y )
		return x;
	else
		return y;
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::CalcMinComponent() const
{
	if( x < y )
		return x;
	else
		return y;
}


//-----------------------------------------------------------------------------------------------
inline const bool Vector2::IsApproximatelyZero() const
{
	const float epsilon = 0.0001f;
	if( x > epsilon || y > epsilon || x < -epsilon || y < -epsilon )
	{
		return false;
	}
	else
	{
		return true;
	}
}


//-----------------------------------------------------------------------------------------------
inline const bool Vector2::IsApproximatelyOne() const
{
	const float epsilon = 0.0001f;
	const float onePlusEpsilon = 1.0f + epsilon;
	const float oneMinusEpsilon = 1.0f - epsilon;
	if( x > onePlusEpsilon || y > onePlusEpsilon || x < oneMinusEpsilon || y < oneMinusEpsilon )
	{
		return false;
	}
	else
	{
		return true;
	}
}


//-----------------------------------------------------------------------------------------------
inline const bool Vector2::IsApproximatelyNormalized() const
{
	const float lengthSquared = (x*x + y*y);
	if( lengthSquared > 1.0001f || lengthSquared < .9999f )
	{
		return false;
	}
	else
	{
		return true;
	}
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::GetRotatedByDegrees( const double returnValueIsRotatedByThisManyDegrees ) const
{
	const double radians = DEG2RAD( returnValueIsRotatedByThisManyDegrees );
	const float cosTheta = (float) cos( radians );
	const float sinTheta = (float) sin( radians );
	return Vector2( (x*cosTheta) - (y*sinTheta), (x*sinTheta) + (y*cosTheta) );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::GetRotatedByRadians( const double returnValueIsRotatedByThisManyRadians ) const
{
	const float cosTheta = (float) cos( returnValueIsRotatedByThisManyRadians );
	const float sinTheta = (float) sin( returnValueIsRotatedByThisManyRadians );
	return Vector2( (x*cosTheta) - (y*sinTheta), (x*sinTheta) + (y*cosTheta) );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::GetRotated90Degrees() const
{
	return Vector2( -y, x );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::GetRotatedMinus90Degrees() const
{
	return Vector2( y, -x );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::GetReversed() const
{
	return Vector2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::operator = ( const Vector2& vectorToCopyFrom )
{
	x = vectorToCopyFrom.x;
	y = vectorToCopyFrom.y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::operator += ( const Vector2& vectorToAdd )
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::operator -= ( const Vector2& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Vector2& Vector2::operator *= ( const float scale )
{
	x *= scale;
	y *= scale;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator - () const
{
	return Vector2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator - ( const Vector2& vectorToSubtract ) const
{
	return Vector2( x - vectorToSubtract.x, y - vectorToSubtract.y );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator + ( const Vector2& vectorToAdd ) const
{
	return Vector2( x + vectorToAdd.x, y + vectorToAdd.y );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Vector2::operator * ( const float scale ) const
{
	return Vector2( x * scale, y * scale );
}


//-----------------------------------------------------------------------------------------------
inline const float Vector2::operator [] ( const int index ) const
{
	return (&x)[ index ];
}


//-----------------------------------------------------------------------------------------------
inline float& Vector2::operator [] ( const int index )
{
	return (&x)[ index ];
}


//-----------------------------------------------------------------------------------------------
inline const bool Vector2::operator == ( const Vector2& vectorToCompareAgainst ) const
{
	return( (x == vectorToCompareAgainst.x) && (y == vectorToCompareAgainst.y) );
}


//-----------------------------------------------------------------------------------------------
inline const bool Vector2::operator != ( const Vector2& vectorToCompareAgainst ) const
{
	return( (x != vectorToCompareAgainst.x) || (y != vectorToCompareAgainst.y) );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 Interpolate( const Vector2& lerpFrom, const Vector2& lerpTo, const float lerpToFraction )
{
	Vector2 returnValue;
	const float lerpFromFraction = 1.0f - lerpToFraction;
	returnValue.x = (lerpFromFraction * lerpFrom.x) + (lerpToFraction * lerpTo.x);
	returnValue.y = (lerpFromFraction * lerpFrom.y) + (lerpToFraction * lerpTo.y);
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 operator * ( const float scale, const Vector2& vectorToScale )
{
	return Vector2( scale * vectorToScale.x, scale * vectorToScale.y );
}


//-----------------------------------------------------------------------------------------------
inline const bool IsEpsilonEqual( const Vector2& vectorA, const Vector2& vectorB, const float epsilon )
{
	const float xDiff = vectorA.x - vectorB.x;
	if( xDiff > epsilon || xDiff < -epsilon )
		return false;

	const float yDiff = vectorA.y - vectorB.y;
	if( yDiff > epsilon || yDiff < -epsilon )
		return false;

	return true;
}


//-----------------------------------------------------------------------------------------------
inline const float DotProduct( const Vector2& vectorA, const Vector2& vectorB )
{
	return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y);
}


//-----------------------------------------------------------------------------------------------
inline const float CrossProduct( const Vector2& vectorA, const Vector2& vectorB )
{
	return (vectorA.x * vectorB.y) - (vectorA.y * vectorB.x);
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 MultiplyComponents( const Vector2& vectorA, const Vector2& vectorB )
{
	return Vector2( vectorA.x * vectorB.x, vectorA.y * vectorB.y );
}


#endif // __include_Vector2__
