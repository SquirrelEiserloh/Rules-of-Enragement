//-----------------------------------------------------------------------------------------------
// IntVector2.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_IntVector2__
#define __include_IntVector2__
#pragma once
#include "MathBase.hpp"


//-----------------------------------------------------------------------------------------------
// Forward class declarations for classes we need to reference
//
class Vector2;


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	IntVector2
//
/////////////////////////////////////////////////////////////////////////////////////////////////
class IntVector2
{
public:
	int x;
	int y;

	static const IntVector2 ZERO;
	static const IntVector2 ONE;

	// Constructors
	~IntVector2() {}
	IntVector2() {}
	IntVector2( const IntVector2& copy );
	explicit IntVector2( const int initialX, const int initialY );
	explicit IntVector2( const std::string& vectorAsString );
	explicit IntVector2( const Vector2& floatVector2 );

	// Mutators
	IntVector2& SetXY( const int newX, const int newY );
	IntVector2& SetLengthAndHeadingRadians( float magnitude, double headingRadians );
	IntVector2& SetLengthAndHeadingDegrees( float magnitude, double headingDegrees );
	IntVector2& SetFromString( const std::string& vectorAsString );
	const float SetLength( const float newLength );
	IntVector2& Clamp( const IntVector2& min, const IntVector2& max );
	IntVector2& InterpolateTo( const IntVector2& lerpTo, const float lerpToFraction );
	IntVector2& MultiplyComponents( const IntVector2& multiplyByThis );
	IntVector2& Rotate90Degrees();
	IntVector2& RotateMinus90Degrees();
	IntVector2& Reverse();

	// Accessors
	void GetXY( OUTPUT int& getX, OUTPUT int& getY ) const;
	const int CalcLengthSquared() const;
	const float CalcLength() const;
	const float CalcAspect() const;
	const float CalcSlope() const;
	const double CalcHeadingRadians() const;
	const double CalcHeadingDegrees() const;
	const int CalcMaxComponent() const;
	const int CalcMinComponent() const;
	const bool IsZero() const;
	const bool IsOne() const;
	const IntVector2 GetRotated90Degrees() const;
	const IntVector2 GetRotatedMinus90Degrees() const;
	const IntVector2 GetReversed() const;

	// Operators
	IntVector2& operator = ( const IntVector2& vectorToCopyFrom );
	IntVector2& operator += ( const IntVector2& vectorToAdd );
	IntVector2& operator -= ( const IntVector2& vectorToSubtract );
	IntVector2& operator *= ( const float scale );
	IntVector2& operator *= ( const int scale );
	IntVector2& operator /= ( const float divisor );
	const IntVector2 operator - () const;
	const IntVector2 operator - ( const IntVector2& vectorToSubtract ) const;
	const IntVector2 operator + ( const IntVector2& vectorToAdd ) const;
	const IntVector2 operator * ( const float scale ) const;
	const IntVector2 operator * ( const int scale ) const;
	const IntVector2 operator / ( const float divisor ) const;
	const int operator [] ( const int index ) const;
	int& operator [] ( const int index );
	const bool operator == ( const IntVector2& vectorToCompareAgainst ) const;
	const bool operator != ( const IntVector2& vectorToCompareAgainst ) const;

	// Standalone friend functions
	friend const IntVector2 Interpolate( const IntVector2& lerpFrom, const IntVector2& lerpTo, const float lerpToFraction );
	friend const IntVector2 operator * ( const float scale, const IntVector2& vectorToScale );
	friend const IntVector2 operator * ( const int scale, const IntVector2& vectorToScale );
	friend const int DotProduct( const IntVector2& vectorA, const IntVector2& vectorB );
	friend const int CrossProduct( const IntVector2& vectorA, const IntVector2& vectorB );
};


//-----------------------------------------------------------------------------------------------
std::string GetTypedObjectAsString( const IntVector2& typedObject, std::string* out_optionalTypeNameAsString );


//-----------------------------------------------------------------------------------------------
inline IntVector2::IntVector2( const IntVector2& copy )
: x( copy.x )
, y( copy.y )
{
}


//-----------------------------------------------------------------------------------------------
inline IntVector2::IntVector2( const int initialX, const int initialY )
: x( initialX )
, y( initialY )
{
}


//-----------------------------------------------------------------------------------------------
//inline IntVector2::IntVector2( const std::string& vectorAsString )
//{
//}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::SetXY( const int newX, const int newY )
{
	x = newX;
	y = newY;
	return *this ;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::SetLengthAndHeadingRadians( float magnitude, double headingRadians )
{
	x = RoundToNearestInt( magnitude * (float) cos( headingRadians ) );
	y = RoundToNearestInt( magnitude * (float) sin( headingRadians ) );
	return *this ;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::SetLengthAndHeadingDegrees( float magnitude, double headingDegrees )
{
	x = RoundToNearestInt( magnitude * (float) cos( DEG2RAD( headingDegrees ) ) );
	y = RoundToNearestInt( magnitude * (float) sin( DEG2RAD( headingDegrees ) ) );
	return *this ;
}


//-----------------------------------------------------------------------------------------------
inline const float IntVector2::SetLength( const float newLength )
{
	const float oldLength = sqrt( (float)( x*x + y*y ) );
	if( oldLength )
	{
		const float scale = newLength / oldLength;
		x = RoundToNearestInt( scale * (float)x );
		y = RoundToNearestInt( scale * (float)y );
	}

	return oldLength;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::Clamp( const IntVector2& min, const IntVector2& max )
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
inline IntVector2& IntVector2::InterpolateTo( const IntVector2& lerpTo, const float lerpToFraction )
{
	const float lerpFromFraction = 1.0f - lerpToFraction;
	x = RoundToNearestInt( (lerpFromFraction * (float)x) + (lerpToFraction * (float)lerpTo.x) );
	y = RoundToNearestInt( (lerpFromFraction * (float)y) + (lerpToFraction * (float)lerpTo.y) );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::MultiplyComponents( const IntVector2& multiplyByThis )
{
	x *= multiplyByThis.x;
	y *= multiplyByThis.y;
	return *this;
}

/*
//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::RotateDegrees( const double rotateByThisManyDegrees )
{
	const double radians = DEG2RAD( rotateByThisManyDegrees );
	const float cosTheta = (float) cos( radians );
	const float sinTheta = (float) sin( radians );

	const float oldX = (float) x;
	const float oldY = (float) y;

	x = RoundToNearestInt( (oldX*cosTheta) - (oldY*sinTheta) );
	y = RoundToNearestInt( (oldX*sinTheta) + (oldY*cosTheta) );

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::RotateRadians( const double rotateByThisManyRadians )
{
	const float cosTheta = (float) cos( rotateByThisManyRadians );
	const float sinTheta = (float) sin( rotateByThisManyRadians );

	const float oldX = (float) x;
	const float oldY = (float) y;

	x = RoundToNearestInt( (oldX*cosTheta) - (oldY*sinTheta) );
	y = RoundToNearestInt( (oldX*sinTheta) + (oldY*cosTheta) );

	return *this;
}
*/

//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::Rotate90Degrees()
{
	const int newX = -y;
	y = x;
	x = newX;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::RotateMinus90Degrees()
{
	const int newX = y;
	y = -x;
	x = newX;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::Reverse()
{
	x = -x;
	y = -y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline void IntVector2::GetXY( OUTPUT int& getX, OUTPUT int& getY ) const
{
	getX = x;
	getY = y;
}


//-----------------------------------------------------------------------------------------------
inline const int IntVector2::CalcLengthSquared() const
{
	return( x*x + y*y );
}


//-----------------------------------------------------------------------------------------------
inline const float IntVector2::CalcLength() const
{
	return sqrt( (float)( x*x + y*y ) );
}


//-----------------------------------------------------------------------------------------------
inline const float IntVector2::CalcAspect() const
{
	if( y == 0.0f )
		return 0.0f;
	else
		return( (float) x / (float) y );
}


//-----------------------------------------------------------------------------------------------
inline const float IntVector2::CalcSlope() const
{
	if( x == 0.0f )
		return 0.0f;
	else
		return( (float) y / (float) x );
}


//-----------------------------------------------------------------------------------------------
inline const double IntVector2::CalcHeadingRadians() const
{
	return atan2( (float) y, (float) x );
}


//-----------------------------------------------------------------------------------------------
inline const double IntVector2::CalcHeadingDegrees() const
{
	return RAD2DEG( atan2( (float) y, (float) x ) );
}


//-----------------------------------------------------------------------------------------------
inline const int IntVector2::CalcMaxComponent() const
{
	if( x > y )
		return x;
	else
		return y;
}


//-----------------------------------------------------------------------------------------------
inline const int IntVector2::CalcMinComponent() const
{
	if( x < y )
		return x;
	else
		return y;
}

/*
//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::GetRotatedByDegrees( const double returnValueIsRotatedByThisManyDegrees ) const
{
	const double radians = DEG2RAD( returnValueIsRotatedByThisManyDegrees );
	const float cosTheta = (float) cos( radians );
	const float sinTheta = (float) sin( radians );
	return IntVector2( (x*cosTheta) - (y*sinTheta), (x*sinTheta) + (y*cosTheta) );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::GetRotatedByRadians( const double returnValueIsRotatedByThisManyRadians ) const
{
	const float cosTheta = (float) cos( returnValueIsRotatedByThisManyRadians );
	const float sinTheta = (float) sin( returnValueIsRotatedByThisManyRadians );
	return IntVector2( (x*cosTheta) - (y*sinTheta), (x*sinTheta) + (y*cosTheta) );
}
*/

//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::GetRotated90Degrees() const
{
	return IntVector2( -y, x );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::GetRotatedMinus90Degrees() const
{
	return IntVector2( y, -x );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::GetReversed() const
{
	return IntVector2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::operator = ( const IntVector2& vectorToCopyFrom )
{
	x = vectorToCopyFrom.x;
	y = vectorToCopyFrom.y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::operator += ( const IntVector2& vectorToAdd )
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::operator -= ( const IntVector2& vectorToSubtract )
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::operator *= ( const float scale )
{
	x = RoundToNearestInt( scale * (float)x );
	y = RoundToNearestInt( scale * (float)y );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::operator *= ( const int scale )
{
	x *= scale;
	y *= scale;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline IntVector2& IntVector2::operator /= ( const float divisor )
{
	if( divisor == 0.0f )
	{
		x = 0;
		y = 0;
	}
	else
	{
		const float scale = 1.0f / divisor;
		x = RoundToNearestInt( scale * (float)x );
		y = RoundToNearestInt( scale * (float)y );
	}
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator - () const
{
	return IntVector2( -x, -y );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator - ( const IntVector2& vectorToSubtract ) const
{
	return IntVector2( x - vectorToSubtract.x, y - vectorToSubtract.y );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator + ( const IntVector2& vectorToAdd ) const
{
	return IntVector2( x + vectorToAdd.x, y + vectorToAdd.y );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator * ( const float scale ) const
{
	IntVector2 returnValue;
	returnValue.x = RoundToNearestInt( scale * (float)x );
	returnValue.y = RoundToNearestInt( scale * (float)y );
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator * ( const int scale ) const
{
	IntVector2 returnValue( scale * x, scale * y );
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 IntVector2::operator / ( const float divisor ) const
{
	if( divisor == 0.0f )
	{
		return IntVector2::ZERO;
	}
	else
	{
		const float scale = 1.0f / divisor;
		return IntVector2( RoundToNearestInt( scale * (float) x ), RoundToNearestInt( scale * (float) y ) );
	}
}


//-----------------------------------------------------------------------------------------------
inline const int IntVector2::operator [] ( const int index ) const
{
	return (&x)[ index ];
}


//-----------------------------------------------------------------------------------------------
inline int& IntVector2::operator [] ( const int index )
{
	return (&x)[ index ];
}


//-----------------------------------------------------------------------------------------------
inline const bool IntVector2::operator == ( const IntVector2& vectorToCompareAgainst ) const
{
	return( (x == vectorToCompareAgainst.x) && (y == vectorToCompareAgainst.y) );
}


//-----------------------------------------------------------------------------------------------
inline const bool IntVector2::operator != ( const IntVector2& vectorToCompareAgainst ) const
{
	return( (x != vectorToCompareAgainst.x) || (y != vectorToCompareAgainst.y) );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 Interpolate( const IntVector2& lerpFrom, const IntVector2& lerpTo, const float lerpToFraction )
{
	IntVector2 returnValue;
	const float lerpFromFraction = 1.0f - lerpToFraction;
	returnValue.x = RoundToNearestInt( (lerpFromFraction * (float)lerpFrom.x) + (lerpToFraction * (float)lerpTo.x) );
	returnValue.y = RoundToNearestInt( (lerpFromFraction * (float)lerpFrom.y) + (lerpToFraction * (float)lerpTo.y) );
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 operator * ( const float scale, const IntVector2& vectorToScale )
{
	return IntVector2( RoundToNearestInt( scale * (float)vectorToScale.x ), RoundToNearestInt( scale * (float)vectorToScale.y ) );
}


//-----------------------------------------------------------------------------------------------
inline const IntVector2 operator * ( const int scale, const IntVector2& vectorToScale )
{
	return IntVector2( scale * vectorToScale.x, scale * vectorToScale.y );
}


//-----------------------------------------------------------------------------------------------
inline const int DotProduct( const IntVector2& vectorA, const IntVector2& vectorB )
{
	return (vectorA.x * vectorB.x) + (vectorA.y * vectorB.y);
}


//-----------------------------------------------------------------------------------------------
inline const int CrossProduct( const IntVector2& vectorA, const IntVector2& vectorB )
{
	return (vectorA.x * vectorB.y) - (vectorA.y * vectorB.x);
}


#endif // __include_IntVector2__
