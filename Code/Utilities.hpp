//-----------------------------------------------------------------------------------------------
// Utilities.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_Utilities__
#define __include_Utilities__
#pragma once

#include "Shared.hpp"


//-----------------------------------------------------------------------------------------------
// File I/O utility functions
//
// TODO: Move all File I/O stuff to its own engine module
//
void SysCreateFolder( const JazzPath& filePath );
void SysDeleteFile( const JazzPath& filePath );
int LoadBinaryFileToNewBuffer( const JazzPath& filePath, OUTPUT unsigned char*& buffer, bool isText=false );
int WriteBufferToBinaryFile( const JazzPath& filePath, const unsigned char* sourceBuffer, int bufferSize );


//-----------------------------------------------------------------------------------------------
// Random number utility functions
//
unsigned char RandomByte();
int RandomIntLessThan( const int ceiling );
int RandomIntInRangeInclusive( const int lowestPossibleResult, const int highestPossibleResult );
float RandomNonNegativeFloatLessThanOne();
float RandomFloatBetweenZeroAndOneInclusive();
float RandomFloatInRangeInclusive( float lowestPossibleResult, float highestPossibleResult );


//-----------------------------------------------------------------------------------------------
// String-related utility functions
//
void Sprintf( std::string& destination, const char* format, ... );
void Sprintf( const int maxLength, std::string& destination, const char* format, ... );
const std::string Stringf( const char* format, ... );
const std::string Stringf( const int maxLength, const char* format, ... );
int Stricmp( const std::string& one, const std::string& two );
int Stricmp( const char* one, const std::string& two );
int Stricmp( const std::string& one, const char* two );
int Stricmp( const char* one, const char* two );
std::wstring PromoteToWString( const std::string& sourceString );
std::string DemoteToAsciiString( const std::wstring& sourceWString );
std::string ReturnAsLowerCase( const std::string& sourceString );
std::string ReturnAsUpperCase( const std::string& sourceString );
const std::string GetTrimmedString( const std::string& originalString );
void GetTrimmedSubstring( std::string& substring_out, const std::string& sourceString, int startPos, unsigned int size );
template< typename T_DestinationType > void SetTypeFromString( T_DestinationType& destination_out, const std::string& asString );
void DebuggerPrintf( const char* messageFormat, ... );


//-----------------------------------------------------------------------------------------------
// Math utility functions
//
int MinInt( const int a, const int b );
int MaxInt( const int a, const int b );
int MinInt( const int a, const int b, const int c );
int MaxInt( const int a, const int b, const int c );
float MinFloat( const float a, const float b );
float MaxFloat( const float a, const float b );
float MinFloat( const float a, const float b, const float c );
float MaxFloat( const float a, const float b, const float c );
int ClampInt( const int valueToBeClamped, const int minimum, const int maximum );
float ClampFloat( const float valueToBeClamped, const float minimum, const float maximum );
float RangeMapFloat( const float incomingRangeStart, const float incomingRangeEnd, const float incomingValue, const float outgoingRangeStart, const float outgoingRangeEnd );


//-----------------------------------------------------------------------------------------------
// Debugging and logging utility functions
//
void DebuggerPrintf( const char* messageFormat, ... );


//-----------------------------------------------------------------------------------------------
// Inline utility function and macro definitions
//
#define BIT( bitNumber ) ( 1 << (bitNumber) )


//-----------------------------------------------------------------------------------------------
inline int RandomIntInRangeInclusive( int lowestPossibleResult, int highestPossibleResult )
{
	const int range = 1 + highestPossibleResult - lowestPossibleResult;
	return lowestPossibleResult + RandomIntLessThan( range );
}


//-----------------------------------------------------------------------------------------------
inline float RandomNonNegativeFloatLessThanOne()
{
	return (float)rand() / (float)(RAND_MAX + 1);
}


//-----------------------------------------------------------------------------------------------
inline float RandomFloatBetweenZeroAndOneInclusive()
{
	return (float)rand() / (float)(RAND_MAX + 1);
}


//-----------------------------------------------------------------------------------------------
inline float RandomFloatInRangeInclusive( float lowestPossibleResult, float highestPossibleResult )
{
	const float randomZeroToOne = (float)rand() / (float)RAND_MAX;
	return lowestPossibleResult + randomZeroToOne * (highestPossibleResult - lowestPossibleResult);
}


//-----------------------------------------------------------------------------------------------
inline int Stricmp( const std::string& one, const std::string& two )
{
	return _stricmp( one.c_str(), two.c_str() );
}


//-----------------------------------------------------------------------------------------------
inline int Stricmp( const char* one, const std::string& two )
{
	return _stricmp( one, two.c_str() );
}


//-----------------------------------------------------------------------------------------------
inline int Stricmp( const std::string& one, const char* two )
{
	return _stricmp( one.c_str(), two );
}


//-----------------------------------------------------------------------------------------------
inline int Stricmp( const char* one, const char* two )
{
	return _stricmp( one, two );
}


//-----------------------------------------------------------------------------------------------
inline std::wstring PromoteToWString( const std::string& sourceString )
{
	const size_t numActualChars = sourceString.size();
	std::wstring returnValueWString( numActualChars, L' ' );
	for( unsigned int i = 0; i < numActualChars; ++ i )
	{
		returnValueWString[ i ] = sourceString[ i ];
	}

	return returnValueWString;
}


//-----------------------------------------------------------------------------------------------
inline std::string DemoteToAsciiString( const std::wstring& sourceWString )
{
	const size_t numActualChars = sourceWString.size();
	std::string returnValueString( numActualChars, ' ' );
	for( unsigned int i = 0; i < numActualChars; ++ i )
	{
		returnValueString[ i ] = (char) sourceWString[ i ];
	}

	return returnValueString;
}


//-----------------------------------------------------------------------------------------------
inline std::string ReturnAsLowerCase( const std::string& sourceString )
{
	std::string returnValue( sourceString );
	const size_t numChars = sourceString.size();
	for( unsigned int i = 0; i < numChars; ++ i )
	{
		returnValue[ i ] = (char) tolower( returnValue[ i ] );
	}

	return returnValue;
}


//-----------------------------------------------------------------------------------------------
inline std::string ReturnAsUpperCase( const std::string& sourceString )
{
	std::string returnValue( sourceString );
	const size_t numChars = sourceString.size();
	for( unsigned int i = 0; i < numChars; ++ i )
	{
		returnValue[ i ] = (char) toupper( returnValue[ i ] );
	}

	return returnValue;
}




//-----------------------------------------------------------------------------------------------
template< typename T_DestinationType >
void SetTypeFromString( T_DestinationType& destination_out, const std::string& asString )
{
	const T_DestinationType constructedFromString( asString ); // Type used MUST support "construct from string".  If not, specialize this template for that type.
	destination_out = constructedFromString;
}


//-----------------------------------------------------------------------------------------------
template<> inline void SetTypeFromString< float >( float& destination_out, const std::string& asString )
{
	destination_out = (float) atof( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
template<> inline void SetTypeFromString< double >( double& destination_out, const std::string& asString )
{
	destination_out = atof( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
template<> inline void SetTypeFromString< int >( int& destination_out, const std::string& asString )
{
	destination_out = atoi( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
template<> inline void SetTypeFromString< long >( long& destination_out, const std::string& asString )
{
	destination_out = atol( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
template<> inline void SetTypeFromString< unsigned int >( unsigned int& destination_out, const std::string& asString )
{
	destination_out = atoi( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
template<> inline void SetTypeFromString< time_t >( time_t& destination_out, const std::string& asString )
{
	destination_out = atol( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
template<> inline void SetTypeFromString< bool >( bool& destination_out, const std::string& asString )
{
	destination_out = false;
	if( asString == "1" )
	{
		destination_out = true;
	}
	else
	{
		const std::string asLowerCaseString = ReturnAsLowerCase( asString );
		if( asLowerCaseString == "true" )
		{
			destination_out = true;
		}
	}
}


///-----------------------------------------------------------------------
/// Splits the source string apart at every occurrence of the delimiter.
/// Returns a vector of all strings found.
///-----------------------------------------------------------------------
inline std::vector< std::string > SplitStringOnDelimiter( const std::string& sourceString, char delimiter )
{
	std::vector< std::string > splitStrings ;

	size_t startingIndex			= 0 ;
	size_t delimiterIndex			= sourceString.find_first_of( delimiter, startingIndex );
	int		numberOfDelimitersFound	= 0 ;

	while ( delimiterIndex != std::string::npos )
	{
		numberOfDelimitersFound++ ;

		/// Add the string, if non-empty
		std::string splitString(sourceString, startingIndex, delimiterIndex - startingIndex); 
		splitStrings.push_back( splitString );

		// Advance to the next split
		startingIndex = delimiterIndex + 1;
		delimiterIndex	= sourceString.find_first_of( delimiter, startingIndex );
	}

	// Now grab whatever's left.  If there is nothing left, but the number of strings is == number of delimiters found, then the last delimiter was end of the string.  So add a blank one to represent other side of the split.
	std::string remainderString( sourceString, startingIndex );
	if ( !remainderString.empty() || ( splitStrings.size() <= (unsigned int) numberOfDelimitersFound )  )
		splitStrings.push_back( remainderString );

	return splitStrings ;
}

///-----------------------------------------------------------------------
/// Splits the source string apart at every occurrence of the delimiter.
/// Stores the split strings in the specified splitResults vector.
/// Returns the number of strings actually stored.
///-----------------------------------------------------------------------
inline int SplitStringOnDelimiter( const std::string& sourceString, char delimiter, std::vector< std::string >& splitResults )
{
	int numberOfSubstringsFound			= 0 ;
	int numberAvailable		= (int)splitResults.size();

	size_t startingIndex	= 0 ;
	size_t delimiterIndex	= sourceString.find_first_of( delimiter, startingIndex );
	while ( delimiterIndex != std::string::npos && numberOfSubstringsFound < numberAvailable )
	{
		/// Add the string, if non-empty
		std::string splitString(sourceString, startingIndex, delimiterIndex - startingIndex); 
		splitResults[ numberOfSubstringsFound ] = splitString ;
		numberOfSubstringsFound++ ;

		// Advance to the next split
		startingIndex = delimiterIndex + 1;
		delimiterIndex	= sourceString.find_first_of( delimiter, startingIndex );
	}

	if ( numberOfSubstringsFound < numberAvailable )
	{
		// Now grab whatever's left
		std::string remainderString( sourceString, startingIndex );
		splitResults[ numberOfSubstringsFound ] = remainderString ;
		numberOfSubstringsFound++ ;
	}

	return numberOfSubstringsFound ;
}


//-----------------------------------------------------------------------------------------------
template< typename T >
inline T GetTypeFromString( const std::string& asString )
{
	T type;
	SetTypeFromString( type, asString );
	return type;
}


//-----------------------------------------------------------------------------------------------
inline int MinInt( int a, int b )
{
	if( a < b )
		return a;
	else
		return b;
}


//-----------------------------------------------------------------------------------------------
inline int MaxInt( int a, int b )
{
	if( a > b )
		return a;
	else
		return b;
}


//-----------------------------------------------------------------------------------------------
inline int MinInt( int a, int b, int c )
{
	if( a < b )
	{
		if( a < c )
			return a;
		else
			return c;
	}
	else
	{
		if( b < c )
			return b;
		else
			return c;
	}
}


//-----------------------------------------------------------------------------------------------
inline int MaxInt( int a, int b, int c )
{
	if( a > b )
	{
		if( a > c )
			return a;
		else
			return c;
	}
	else
	{
		if( b > c )
			return b;
		else
			return c;
	}
}


//-----------------------------------------------------------------------------------------------
inline float MinFloat( float a, float b )
{
	if( a < b )
		return a;
	else
		return b;
}


//-----------------------------------------------------------------------------------------------
inline float MaxFloat( float a, float b )
{
	if( a > b )
		return a;
	else
		return b;
}


//-----------------------------------------------------------------------------------------------
inline float MinFloat( float a, float b, float c )
{
	if( a < b )
	{
		if( a < c )
			return a;
		else
			return c;
	}
	else
	{
		if( b < c )
			return b;
		else
			return c;
	}
}


//-----------------------------------------------------------------------------------------------
inline float MaxFloat( float a, float b, float c )
{
	if( a > b )
	{
		if( a > c )
			return a;
		else
			return c;
	}
	else
	{
		if( b > c )
			return b;
		else
			return c;
	}
}


//-----------------------------------------------------------------------------------------------
template< typename T_ClampValueType >
inline T_ClampValueType ReturnClamped( const T_ClampValueType& valueToBeClamped, const T_ClampValueType& minimumValue, const T_ClampValueType& maximumValue )
{
	if( valueToBeClamped < minimumValue )
	{
		return minimumValue;
	}
	else if( valueToBeClamped > maximumValue )
	{
		return maximumValue;
	}
	else
	{
		return valueToBeClamped;
	}
}


//-----------------------------------------------------------------------------------------------
inline int ClampInt( const int valueToBeClamped, const int minimum, const int maximum )
{
	if( valueToBeClamped < minimum )
	{
		return minimum;
	}
	else if( valueToBeClamped > maximum )
	{
		return maximum;
	}
	else
	{
		return valueToBeClamped;
	}
}


//-----------------------------------------------------------------------------------------------
inline float ClampFloat( const float valueToBeClamped, const float minimum, const float maximum )
{
	if( valueToBeClamped < minimum )
	{
		return minimum;
	}
	else if( valueToBeClamped > maximum )
	{
		return maximum;
	}
	else
	{
		return valueToBeClamped;
	}
}


//-----------------------------------------------------------------------------------------------
inline float RangeMapFloat( const float incomingRangeStart, const float incomingRangeEnd, const float incomingValue, const float outgoingRangeStart, const float outgoingRangeEnd )
{
	if( incomingRangeStart == incomingRangeEnd )
		return outgoingRangeEnd;

	const float rangeFraction = (incomingValue - incomingRangeStart) / (incomingRangeEnd - incomingRangeStart);
	return outgoingRangeStart + rangeFraction * (outgoingRangeEnd - outgoingRangeStart);
}


//-----------------------------------------------------------------------------------------------
inline bool IsFloat1EpsilonLessThanFloat2( const float float1, const float float2, const float epsilon )
{
	return( float1 + epsilon < float2 );
}


//-----------------------------------------------------------------------------------------------
inline bool IsDouble1EpsilonLessThanDouble2( const double double1, const double double2, const double epsilon )
{
	return( double1 + epsilon < double2 );
}


//-----------------------------------------------------------------------------------------------
inline bool IsFloat1EpsilonEqualToFloat2( const float float1, const float float2, const float epsilon )
{
	return( float1 + epsilon >= float2 && float2 + epsilon >= float1 );
}


//-----------------------------------------------------------------------------------------------
inline bool IsDouble1EpsilonEqualToDouble2( const double double1, const double double2, const double epsilon )
{
	return( double1 + epsilon >= double2 && double2 + epsilon >= double1 );
}


#endif // __include_JazzUtilities__
