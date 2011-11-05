//-----------------------------------------------------------------------------------------------
// IntVector2.cpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "IntVector2.hpp"
#include "Vector2.hpp"
#include "Utilities.hpp"


//-----------------------------------------------------------------------------------------------
// Static member initialization
//
STATIC const IntVector2 IntVector2::ZERO( 0, 0 );
STATIC const IntVector2 IntVector2::ONE( 1, 1 );


//-----------------------------------------------------------------------------------------------
IntVector2::IntVector2( const Vector2& floatVector2 )
: x( static_cast< int >( floatVector2.x ) )
, y( static_cast< int >( floatVector2.y ) )
{
}


//-----------------------------------------------------------------------------------------------
std::string GetTypedObjectAsString( const IntVector2& typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "IntVector2";
	}
	return Stringf( "%d,%d", typedObject.x, typedObject.y );
}


//-----------------------------------------------------------------------------------------------
IntVector2::IntVector2( const std::string& vectorAsString )
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
	x = atoi( temp );

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
	y = atoi( temp );
}


