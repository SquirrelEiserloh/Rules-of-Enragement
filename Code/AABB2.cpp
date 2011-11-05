//-----------------------------------------------------------------------------------------------
// AABB2.cpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "AABB2.hpp"

///-----------------------------------------------------------------------
///
///-----------------------------------------------------------------------
AABB2::AABB2( const std::string& asString )
{
	std::string::size_type pos = asString.find_first_of( ')' );
	if ( pos != std::string::npos && pos < asString.length()-2 )
	{
		std::string minsAsString	= asString.substr( 0, pos + 1 );
		std::string maxsAsString	= asString.substr( pos + 2 );
		mins	= Vector2( minsAsString );
		maxs	= Vector2( maxsAsString );
	}
}

//-----------------------------------------------------------------------------------------------
const float AABB2::CalcAspect() const
{
	const Vector2 size = CalcSize();
	return size.CalcAspect();
}


//-----------------------------------------------------------------------------------------------
const float AABB2::CalcScaleRequiredToFitInsideBox( const AABB2& theBoxIWantToFitInside ) const
{
	const Vector2 mySize = CalcSize();
	const Vector2 containerSize = theBoxIWantToFitInside.CalcSize();
	return mySize.CalcScaleRequiredToFitInsideBox( containerSize );
}


//-----------------------------------------------------------------------------------------------
const Vector2 AABB2::GetPointAtNormalizedCoordinates( const Vector2& normalizedCoordinatesWithinBounds ) const
{
	Vector2 returnValue;
	returnValue.x = mins.x + (normalizedCoordinatesWithinBounds.x * (maxs.x - mins.x));
	returnValue.y = mins.y + (normalizedCoordinatesWithinBounds.y * (maxs.y - mins.y));
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
const AABB2 AABB2::CalcLargestCenteredInteriorAABB2OfGivenAspectRatio( float aspectRatio ) const
{
	const float containerAspect = CalcAspect();

	if( aspectRatio == 0.f )
	{
		const float centerX = 0.5f * (maxs.x + mins.x);
		return AABB2( centerX, mins.y, centerX, maxs.y );
	}
	else if( aspectRatio < 0.f )
	{
		aspectRatio *= -1.f;
	}

	if( aspectRatio >= containerAspect )
	{
		// Contained rect is wider-aspected than us, so we would "letterbox" it within (padded on top and bottom)
		const float width = maxs.x - mins.x;
		const float height = width / aspectRatio;
		const float centerY = 0.5f * (maxs.y + mins.y);
		return AABB2( mins.x, centerY - (0.5f * height), maxs.x, centerY + (0.5f * height) );
	}
	else
	{
		// Contained rect is taller-aspected than us, so we would "pillarbox" it within (padded on left and right)
		const float height = maxs.y - mins.y;
		const float width = height * aspectRatio;
		const float centerX = 0.5f * (maxs.x + mins.x);
		return AABB2( centerX - (0.5f * width), mins.y, centerX + (0.5f * width), maxs.y );
	}
}

