//-----------------------------------------------------------------------------------------------
// AABB2.hpp
//
// Copyright 2009 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef _include_AABB2_
#define _include_AABB2_
#pragma once
#include "Vector2.hpp"


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		AABB2
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class AABB2
{
public:
	Vector2		mins;
	Vector2		maxs;

	// Construction / Destruction
	~AABB2() {}
	AABB2() {}
	AABB2( const AABB2& copy );
	AABB2( const std::string& asString );
	explicit AABB2( const Vector2& point );
	explicit AABB2( const Vector2& minimums, const Vector2& maximums );
	explicit AABB2( const Vector2& minimums, const Vector2& maximums, const Vector2& padding );
	explicit AABB2( float minX, float minY, float maxX, float maxY );

	// Mutators
	void SetFromMinsMaxs( const Vector2& minimums, const Vector2& maximums );
	void SetFromPoint( const Vector2& point );
	void SetFromTwoPoints( const Vector2& point1, const Vector2& point2 );
	void SetFromMinsMaxsPadding( const Vector2& minimums, const Vector2& maximums, const Vector2& padding );
	void SetFromPointPadding( const Vector2& point, const Vector2& padding );
	void SetFromTwoPointsPadding( const Vector2& point1, const Vector2& point2, const Vector2& padding );
	void SetFromMinXYMaxXY( float minX, float minY, float maxX, float maxY );
	void SetFromMinXYMaxXYPadXY( float minX, float minY, float maxX, float maxY, float padX, float padY );
	AABB2& SetFromStream( std::istringstream& is );
	void AddPadding( const Vector2& padding );
	void AddPadding( float padX, float padY );
	AABB2& StretchBoundsToIncludePoint( const Vector2& point );
	AABB2& StretchBoundsToIncludeBox( const AABB2& boxBounds );
	AABB2& Translate( const Vector2& translation );

	// Accessors
	const AABB2 Lerp( const AABB2& lerpTo, const float lerpTo_fraction ) const;
	const float CalcDiagonalLengthSquared() const;
	const float CalcDiagonalLength() const;
	const Vector2 CalcSize() const;
	const Vector2 CalcCenter() const;
	const bool IsOverlapping( const AABB2& compare ) const;
	const bool IsIntersecting(const Vector2& start, const Vector2& end) const;
	const AABB2 CalcOverlap( const AABB2& compare ) const;
	const bool IsPointInsideBounds( const Vector2& point ) const;
	const Vector2 ClampPointToBounds( const Vector2& point ) const;
	const float CalcAspect() const;
	const float CalcScaleRequiredToFitInsideBox( const AABB2& theBoxIWantToFitInside ) const;
	const AABB2 CalcLargestCenteredInteriorAABB2OfGivenAspectRatio( float aspectRatio ) const;
	const Vector2 GetPointAtNormalizedCoordinates( const Vector2& normalizedCoordinatesWithinBounds ) const;

	// Operators
	const AABB2& operator = ( const AABB2& rhs );
	const AABB2& operator += ( const Vector2& translation );
	const AABB2& operator -= ( const Vector2& antiTranslation );
	const AABB2& operator *= ( const float uniformScale );
	const AABB2& operator *= ( const Vector2& nonUniformScale );
	const AABB2& operator /= ( const float uniformDivisor );
	const AABB2 operator - () const;
	const AABB2 operator + ( const Vector2& translation ) const;
	const AABB2 operator - ( const Vector2& antiTranslation ) const;
	const AABB2 operator * ( const float uniformScale ) const;
	const AABB2 operator * ( const Vector2& nonUniformScale ) const;
	const bool operator == ( const AABB2& rhs ) const;
	const bool operator != ( const AABB2& rhs ) const;

	// Standalone friend functions
	friend const AABB2 operator * ( const float uniformScale, const AABB2& rhs );
	friend const AABB2 operator * ( const Vector2& nonUniformScale, const AABB2& rhs );
	friend const bool IsEpsilonEqual( const AABB2& first, const AABB2& second, const float epsilon );
	friend const AABB2 Interpolate( const AABB2& lerpFrom, const AABB2& lerpTo, const float lerpToFraction );
};


//-----------------------------------------------------------------------------------------------
inline AABB2::AABB2( const AABB2& copy )
: mins( copy.mins )
, maxs( copy.maxs )
{
}


//-----------------------------------------------------------------------------------------------
inline AABB2::AABB2( const Vector2& point )
: mins( point )
, maxs( point )
{
}


//-----------------------------------------------------------------------------------------------
inline AABB2::AABB2( const Vector2& minimums, const Vector2& maximums )
: mins( minimums )
, maxs( maximums )
{
}


//-----------------------------------------------------------------------------------------------
inline AABB2::AABB2( const Vector2& minimums, const Vector2& maximums, const Vector2& padding )
: mins( minimums - padding )
, maxs( maximums + padding )
{
}


//-----------------------------------------------------------------------------------------------
inline AABB2::AABB2( float minX, float minY, float maxX, float maxY )
: mins( minX, minY )
, maxs( maxX, maxY )
{
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromMinsMaxs( const Vector2& minimums, const Vector2& maximums )
{
	mins = minimums;
	maxs = maximums;
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromPoint( const Vector2& point )
{
	mins = point;
	maxs = point;
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromTwoPoints( const Vector2& point1, const Vector2& point2 )
{
	if( point1.x > point2.x )
	{
		mins.x = point2.x;
		maxs.x = point1.x;
	}
	else
	{
		mins.x = point1.x;
		maxs.x = point2.x;
	}

	if( point1.y > point2.y )
	{
		mins.y = point2.y;
		maxs.y = point1.y;
	}
	else
	{
		mins.y = point1.y;
		maxs.y = point2.y;
	}
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromMinsMaxsPadding( const Vector2& minimums, const Vector2& maximums, const Vector2& padding )
{
	mins = minimums - padding;
	maxs = maximums + padding;
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromPointPadding( const Vector2& point, const Vector2& padding )
{
	mins = point - padding;
	maxs = point + padding;
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromTwoPointsPadding( const Vector2& point1, const Vector2& point2, const Vector2& padding )
{
	if( point1.x > point2.x )
	{
		mins.x = point2.x - padding.x;
		maxs.x = point1.x + padding.x;
	}
	else
	{
		mins.x = point1.x - padding.x;
		maxs.x = point2.x + padding.x;
	}

	if( point1.y > point2.y )
	{
		mins.y = point2.y - padding.y;
		maxs.y = point1.y + padding.y;
	}
	else
	{
		mins.y = point1.y - padding.y;
		maxs.y = point2.y + padding.y;
	}
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromMinXYMaxXY( float minX, float minY, float maxX, float maxY )
{
	mins.SetXY( minX, minY );
	maxs.SetXY( maxX, maxY );
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::SetFromMinXYMaxXYPadXY( float minX, float minY, float maxX, float maxY, float padX, float padY )
{
	mins.SetXY( minX - padX, minY - padY );
	maxs.SetXY( maxX + padX, maxY + padY );
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::AddPadding( const Vector2& padding )
{
	mins -= padding;
	maxs += padding;
}


//-----------------------------------------------------------------------------------------------
inline void AABB2::AddPadding( float padX, float padY )
{
	mins.x -= padX;
	mins.y -= padY;
	maxs.x += padX;
	maxs.y += padY;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2& AABB2::operator = ( const AABB2& rhs )
{
	if( this == &rhs )
		return *this;

	mins = rhs.mins;
	maxs = rhs.maxs;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2& AABB2::operator += ( const Vector2& translation )
{
	mins += translation;
	maxs += translation;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2& AABB2::operator -= ( const Vector2& antiTranslation )
{
	mins -= antiTranslation;
	maxs -= antiTranslation;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2& AABB2::operator *= ( const float uniformScale )
{
	mins *= uniformScale;
	maxs *= uniformScale;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2& AABB2::operator *= ( const Vector2& nonUniformScale )
{
	mins.MultiplyComponents( nonUniformScale );
	maxs.MultiplyComponents( nonUniformScale );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2& AABB2::operator /= ( const float divisor )
{
	if( divisor )
	{
		const float invDivisor = 1.f / divisor;
		mins *= invDivisor;
		maxs *= invDivisor;
	}
	else
	{
		mins = Vector2::ZERO;
		maxs = Vector2::ZERO;
	}
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 AABB2::operator - () const
{
	return AABB2( -mins, -maxs );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 AABB2::operator + ( const Vector2& translation ) const
{
	return AABB2( mins + translation, maxs + translation );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 AABB2::operator - ( const Vector2& antiTranslation ) const
{
	return AABB2( mins - antiTranslation, maxs - antiTranslation );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 AABB2::operator * ( const float uniformScale ) const
{
	return AABB2( mins * uniformScale, maxs * uniformScale );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 AABB2::operator * ( const Vector2& nonUniformScale ) const
{
	return AABB2( MultiplyComponents( mins, nonUniformScale ), MultiplyComponents( maxs, nonUniformScale ) );
}


//-----------------------------------------------------------------------------------------------
inline const bool AABB2::operator == ( const AABB2& rhs ) const
{
	return( (mins == rhs.mins) && (maxs == rhs.maxs) );
}


//-----------------------------------------------------------------------------------------------
inline const bool AABB2::operator != ( const AABB2& rhs ) const
{
	return( (mins != rhs.mins) || (maxs != rhs.maxs) );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 AABB2::CalcSize() const
{
	return Vector2( maxs.x - mins.x, maxs.y - mins.y );
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 AABB2::CalcCenter() const
{
	return Vector2( (maxs.x + mins.x) * 0.5f, (maxs.y + mins.y) * 0.5f );
}


//-----------------------------------------------------------------------------------------------
inline const bool AABB2::IsOverlapping( const AABB2& compare ) const
{
	return!(( mins.x > compare.maxs.x ) ||
			( maxs.x < compare.mins.x ) ||
			( mins.y > compare.maxs.y ) ||
			( maxs.y < compare.mins.y ));
}


//-----------------------------------------------------------------------------------------------
inline const bool AABB2::IsIntersecting(const Vector2& start, const Vector2& end) const
{
	Vector2 topLeft;
	Vector2 bottomRight;

	if( start.x < end.x )
	{
		topLeft.x = start.x;
		bottomRight.x = end.x;
	}
	else
	{
		topLeft.x = end.x;
		bottomRight.x = start.x;
	}

	if( start.y < end.y )
	{
		topLeft.y = start.y;
		bottomRight.y = end.y;
	}
	else
	{
		topLeft.y = end.y;
		bottomRight.y = start.y;
	}

	return( mins.x > topLeft.x && bottomRight.x < end.x && maxs.x > topLeft.x && maxs.x < bottomRight.x &&
		    mins.y > topLeft.y && bottomRight.y < end.y && maxs.y > topLeft.y && maxs.y < bottomRight.y );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 AABB2::CalcOverlap( const AABB2& compare ) const
{
	return AABB2(	(mins.x < compare.mins.x ? compare.mins.x : mins.x),
					(mins.y < compare.mins.y ? compare.mins.y : mins.y),
					(maxs.x > compare.maxs.x ? compare.maxs.x : maxs.x),
					(maxs.y > compare.maxs.y ? compare.maxs.y : maxs.y));
}


//-----------------------------------------------------------------------------------------------
inline const float AABB2::CalcDiagonalLengthSquared() const
{
	Vector2 size = CalcSize();
	return (size.x * size.x) + (size.y * size.y);
}


//-----------------------------------------------------------------------------------------------
inline const float AABB2::CalcDiagonalLength() const
{
	Vector2 size = CalcSize();
	return (float) sqrt( (size.x * size.x) + (size.y * size.y) );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 AABB2::Lerp( const AABB2& lerpTo, const float lerpTo_fraction ) const
{
	return AABB2( mins + lerpTo_fraction * (lerpTo.mins - mins),
					maxs + lerpTo_fraction * (lerpTo.maxs - maxs));
}


//-----------------------------------------------------------------------------------------------
inline AABB2& AABB2::StretchBoundsToIncludePoint( const Vector2& point )
{
	if( point.x < mins.x )
		mins.x = point.x;

	if( point.y < mins.y )
		mins.y = point.y;

	if( point.x > maxs.x )
		maxs.x = point.x;

	if( point.y > maxs.y )
		maxs.y = point.y;

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline AABB2& AABB2::StretchBoundsToIncludeBox( const AABB2& boxBounds )
{
	if( boxBounds.mins.x < mins.x )
		mins.x = boxBounds.mins.x;

	if( boxBounds.maxs.x > maxs.x )
		maxs.x = boxBounds.maxs.x;

	if( boxBounds.mins.y < mins.y )
		mins.y = boxBounds.mins.y;

	if( boxBounds.maxs.y > maxs.y )
		maxs.y = boxBounds.maxs.y;

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline AABB2& AABB2::Translate( const Vector2& translation )
{
	mins += translation;
	maxs += translation;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const bool AABB2::IsPointInsideBounds( const Vector2& point ) const
{
	return!(( point.x > maxs.x ) ||
			( point.x < mins.x ) ||
			( point.y > maxs.y ) ||
			( point.y < mins.y ));
}


//-----------------------------------------------------------------------------------------------
inline const Vector2 AABB2::ClampPointToBounds( const Vector2& point ) const
{
	Vector2 clampedPoint;
	clampedPoint.x = Clamp( point.x, mins.x, maxs.x );
	clampedPoint.y = Clamp( point.y, mins.y, maxs.y );
	return clampedPoint;
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 operator * ( const float uniformScale, const AABB2& rhs )
{
	return AABB2( uniformScale * rhs.mins, uniformScale * rhs.maxs );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 operator * ( const Vector2& nonUniformScale, const AABB2& rhs )
{
	return AABB2( MultiplyComponents( rhs.mins, nonUniformScale ), MultiplyComponents( rhs.maxs, nonUniformScale ) );
}


//-----------------------------------------------------------------------------------------------
inline const bool IsEpsilonEqual( const AABB2& first, const AABB2& second, const float epsilon )
{
	return(	( fabs( first.mins.x - second.mins.x ) < epsilon )&&
			( fabs( first.mins.y - second.mins.y ) < epsilon ) &&
			( fabs( first.maxs.x - second.maxs.x ) < epsilon ) &&
			( fabs( first.maxs.y - second.maxs.y ) < epsilon ) );
}


//-----------------------------------------------------------------------------------------------
inline const AABB2 Interpolate( const AABB2& lerpFrom, const AABB2& lerpTo, const float lerpToFraction )
{
	AABB2 returnValue;
	const float lerpFromFraction = 1.0f - lerpToFraction;

	returnValue.mins.x = (lerpFromFraction * lerpFrom.mins.x) + (lerpToFraction * lerpTo.mins.x);
	returnValue.mins.y = (lerpFromFraction * lerpFrom.mins.y) + (lerpToFraction * lerpTo.mins.y);
	returnValue.maxs.x = (lerpFromFraction * lerpFrom.maxs.x) + (lerpToFraction * lerpTo.maxs.x);
	returnValue.maxs.y = (lerpFromFraction * lerpFrom.maxs.y) + (lerpToFraction * lerpTo.maxs.y);

	return returnValue;
}
#endif // _include_AABB2_
