//-----------------------------------------------------------------------------------------------
// TypeUtilities.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_TypeUtilities__
#define __include_TypeUtilities__
#pragma once

#include "Shared.hpp"
#include "Utilities.hpp"
#include "math.h"

//-----------------------------------------------------------------------------------------------
// Functions declared in TypeUtilities.cpp
//-----------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const std::string& in_asString, float& out_asFloat )
{
	out_asFloat = (float) atof( in_asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const std::string& in_asString, int& out_asInt )
{
	out_asInt = atoi( in_asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const std::string& in_asString, bool& out_asBool )
{
	out_asBool = !Stricmp( in_asString, "true" );
}

//------------------------------------------------------------------------------------------------
inline void ConvertType( const std::string& in_asString, double& out_asDouble )
{
	out_asDouble = atof( in_asString.c_str() );
}

//-----------------------------------------------------------------------------------------------
inline void ConvertType( const bool in_asBool, int& out_asInt )
{
	out_asInt = in_asBool ? 1 : 0;
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const bool in_asBool, float& out_asFloat )
{
	out_asFloat = in_asBool ? 1.f : 0.f;
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const bool in_asBool, std::string& out_asString )
{
	if( in_asBool )
		out_asString = "true";
	else
		out_asString = "false";
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const int in_asInt, bool& out_asBool )
{
	out_asBool = ( in_asInt != 0 );
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const int in_asInt, float& out_asFloat )
{
	out_asFloat = (float) in_asInt;
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const int in_asInt, std::string& out_asString )
{
	out_asString = Stringf( "%d", in_asInt );
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const float& in_asFloat, bool& out_asBool )
{
	out_asBool = ( in_asFloat != 0.f );
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const float& in_asFloat, int& out_asInt )
{
	out_asInt = (int) floor( in_asFloat + 0.5f );
}


//-----------------------------------------------------------------------------------------------
inline void ConvertType( const float& in_asFloat, std::string& out_asString )
{
	out_asString = Stringf( "%f", in_asFloat );
}

//------------------------------------------------------------------------------------------------
inline void ConvertType( const float& in_asFloat, float& out_asFloat )
{
	out_asFloat = in_asFloat ;
}

#if defined( JAZZ_PLATFORM_IOS )
#include <Foundation/NSString.h>
///----------------------------------------------------------------------------------
///
///----------------------------------------------------------------------------------
inline void ConvertType( const std::string& inValue, NSString*& outValue )
{
	outValue = [NSString stringWithCString:inValue.c_str() encoding:-[NSString defaultCStringEncoding]];
}

///----------------------------------------------------------------------------------
///
///----------------------------------------------------------------------------------
inline void ConvertType( NSString* inValue, std::string& outValue)
{
	
	outValue = std::string( [inValue UTF8String ] );	
	
}

#endif






































//-----------------------------------------------------------------------------------------------
inline float GetStringAsFloat( const std::string& asString )
{
	return (float) atof( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
inline int GetStringAsInt( const std::string& asString )
{
	return atoi( asString.c_str() );
}


//-----------------------------------------------------------------------------------------------
inline bool GetStringAsBool( const std::string& asString )
{
	return !Stricmp( asString, "true" );
}


//-----------------------------------------------------------------------------------------------
inline int GetBoolAsInt( const bool asBool )
{
	return asBool ? 1 : 0;
}


//-----------------------------------------------------------------------------------------------
inline float GetBoolAsFloat( const bool asBool )
{
	return asBool ? 1.f : 0.f;
}


//-----------------------------------------------------------------------------------------------
inline std::string GetBoolAsString( const bool asBool )
{
	return Stringf( "%s", asBool ? "true" : "false" );
}


//-----------------------------------------------------------------------------------------------
inline bool GetIntAsBool( const int asInt )
{
	return( asInt != 0 );
}


//-----------------------------------------------------------------------------------------------
inline float GetIntAsFloat( const int asInt )
{
	return (float) asInt;
}


//-----------------------------------------------------------------------------------------------
inline std::string GetIntAsString( const int asInt )
{
	return Stringf( "%d", asInt );
}


//-----------------------------------------------------------------------------------------------
inline bool GetFloatAsBool( const float asFloat )
{
	return( asFloat != 0.f );
}


//-----------------------------------------------------------------------------------------------
inline int GetFloatAsInt( const float asFloat )
{
	return (int) floor( asFloat + 0.5f );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetFloatAsString( const float asFloat )
{
	return Stringf( "%f", asFloat );
}


//-----------------------------------------------------------------------------------------------
template< typename T_ObjectType >
inline std::string GetTypedObjectAsString( const T_ObjectType& typedObject, std::string* out_optionalTypeNameAsString )
{
	UNUSED_ARG( typedObject );
#ifndef __APPLE__
	const type_info& typeInfo = typeid( T_ObjectType );
	const int typeSize = sizeof( T_ObjectType );

	if( out_optionalTypeNameAsString )
	{
		Sprintf( *out_optionalTypeNameAsString, "Unknown %d-byte type \"%s\"", typeSize, typeInfo.name() );
	}
#endif
	return "?";
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const std::string& typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "string";
	}
	return typedObject;
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( char* typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "string";
	}
	return std::string( typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const char* typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "string";
	}
	return std::string( typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const float& typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "float";
	}
	return Stringf( "%g", typedObject );
}

//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const double typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "double";
	}
	return Stringf( "%g", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const bool typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "bool";
	}

	if( typedObject )
	{
		return "true";
	}
	else
	{
		return "false";
	}
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const char typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "char";
	}
	return Stringf( "%c", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const unsigned char typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "unsigned char";
	}
	return Stringf( "0x%02x", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const short typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "short";
	}
	return Stringf( "%d", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const unsigned short typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "unsigned short";
	}
	return Stringf( "%u", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const int typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "int";
	}
	return Stringf( "%d", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const unsigned int typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "unsigned int";
	}
	return Stringf( "%u", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const long typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "long";
	}
	return Stringf( "%d", typedObject );
}


//-----------------------------------------------------------------------------------------------
inline std::string GetTypedObjectAsString( const unsigned long typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "unsigned long";
	}
	return Stringf( "%u", typedObject );
}


//-----------------------------------------------------------------------------------------------
template < typename T_IncomingType, typename T_OutgoingType >
void ConvertType( const T_IncomingType& in_rawData, T_OutgoingType& out_convertedData )
{
	const std::string asString = GetTypedObjectAsString( in_rawData, NULL );
	SetTypeFromString( out_convertedData, asString );
	//	{ int q = 5; } //ConsolePrintf( Rgba::ORANGE, "WARNING: Used base template version of ConvertType function - was this expected?\n" );
}


#endif // __include_TypeUtilities__
