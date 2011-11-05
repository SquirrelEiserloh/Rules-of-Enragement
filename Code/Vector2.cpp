//-----------------------------------------------------------------------------------------------
// Vector2.cpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "Vector2.hpp"
#include "Utilities.hpp"


//-----------------------------------------------------------------------------------------------
// Static member initialization
//
STATIC const Vector2 Vector2::ZERO( 0.0f, 0.0f );
STATIC const Vector2 Vector2::ONE( 1.0f, 1.0f );


//-----------------------------------------------------------------------------------------------
std::string GetTypedObjectAsString( const Vector2& typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "Vector2";
	}
	return Stringf( "%g,%g", typedObject.x, typedObject.y );
}


//-----------------------------------------------------------------------------------------------
const float Vector2::CalcScaleRequiredToFitInsideBox( const Vector2& sizeOfBoxIWantToFitInside ) const
{
	if( x == 0.0f )
	{
		if( y == 0.0f )
		{
			return 0.0f;
		}
		else
		{
			return( fabs( sizeOfBoxIWantToFitInside.y / y ) );
		}
	}
	else
	{
		if( y == 0.0f )
		{
			return( sizeOfBoxIWantToFitInside.x / x );
		}
		else
		{
			const float xScale = fabs( sizeOfBoxIWantToFitInside.x / x );
			const float yScale = fabs( sizeOfBoxIWantToFitInside.y / y );
			return( xScale < yScale ? xScale : yScale );
		}
	}
}


//-----------------------------------------------------------------------------------------------
Vector2& Vector2::operator /= ( const float divisor )
{
	if( divisor == 0.0f )
	{
		x = 0.0f;
		y = 0.0f;
	}
	else
	{
		const float scale = 1.0f / divisor;
		x *= scale;
		y *= scale;
	}
	return *this;
}


//-----------------------------------------------------------------------------------------------
const Vector2 Vector2::operator / ( const float divisor ) const
{
	if( divisor == 0.0f )
	{
		return Vector2::ZERO;
	}
	else
	{
		const float scale = 1.0f / divisor;
		return Vector2( x * scale, y * scale );
	}
}


//-----------------------------------------------------------------------------------------------
Vector2::Vector2( const std::string& vectorAsString )
{
	char temp[ 1024 ];
	char openingBrace = '\0';
	char closingBrace = '\0';

	// Grab the opening brace, if any
	const char* scan = vectorAsString.c_str();
	while( isspace( *scan ) ) // skip leading spaces
		++ scan;

	if( *scan == '(' /* || *scan == '<' || *scan == '[' || *scan == '{' */ )
	{
		openingBrace = *scan;
		if( openingBrace == '(' )
			closingBrace = ')';

		++ scan;

		while( isspace( *scan ) ) // skip spaces following opening brace, if any
			++ scan;
	}

	// Grab the "x" component
	char* write = temp;
	while( *scan != ',' && *scan != ' ' && *scan != '\t' && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	x = (float) atof( temp );

	// Skip the delimiter
	while( *scan == ',' || *scan == ' ' || *scan == '\t' )
		++ scan;

	// Grab the "y" component
	write = temp;
	while( *scan != closingBrace && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	y = (float) atof( temp );
}


