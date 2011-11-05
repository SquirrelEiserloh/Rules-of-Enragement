///-----------------------------------------------------------------------
/// ParsingSupport.hpp -- Convenience functions for parsing, including XML.
/// 
/// Copyright 2010 (c) TrueThought LLC
///-----------------------------------------------------------------------
#ifndef __include_ParsingSupport__
#define __include_ParsingSupport__
#pragma once

//#include "JazzBasicTypes.hpp"
#include "xmlParser.h"
#include "ResourceStream.hpp"
//#include "JazzVariables.hpp"


///-----------------------------------------------------------------------
/// XML Utility functions
///-----------------------------------------------------------------------
std::string				GetXMLNodeName( XMLNode xmlNode );
std::string				GetXMLErrorDescForErrorCode( XMLError xmlErrorCode );
XMLNode					CreateXMLDocumentFromResourceStream( const ResourceStream& resourceStream, const std::string& optionalExpectedRootNodeName = "" );
XMLNode					CreateXMLDocumentFromXMLFile( const JazzPath& xmlFile, const std::string& optionalExpectedRootNodeName = "" );
void					ReportXMLDocumentUsage( XMLNode& xmlDocumentToReport, const ResourceStream& resourceStream, bool reportUsed=false );
void					ReportXMLError( const ResourceStream& resourceStream, XMLNode xmlDocumentRootNode, XMLResults errorResult );
void					DestroyXMLDocument( XMLNode& xmlDocumentToDestroy );
std::string				GetStringProperty( const XMLNode& node, const std::string& propertyName, const std::string& defaultValue );
char					GetCharacterProperty( const XMLNode& node, const std::string& propertyName, char defaultValue );
int						GetIntProperty( const XMLNode& node, const std::string& propertyName, int defaultValue );
float					GetFloatProperty( const XMLNode& node, const std::string& propertyName, float defaultValue );
double					GetDoubleProperty( const XMLNode& node, const std::string& propertyName, double defaultValue );
bool					GetBooleanProperty( const XMLNode& node, const std::string& propertyName, bool defaultValue );
Rgba					GetRgbaProperty( const XMLNode& node, const std::string& propertyName, const Rgba& defaultValue );
Vector2					GetVector2Property( const XMLNode& node, const std::string& propertyName, const Vector2& defaultValue=Vector2::ZERO ); 
IntVector2				GetIntVector2Property( const XMLNode& node, const std::string& propertyName, const IntVector2& defaultValue=IntVector2::ZERO ); 

void					SetBooleanProperty( XMLNode& node, const std::string& propertyName, bool value, bool defaultValue, bool createChild );
void					SetIntProperty( XMLNode& node, const std::string& propertyName, int value, int defaultValue, bool createChild );
void					SetFloatProperty( XMLNode& node, const std::string& propertyName, float value, float defaultValue, bool createChild );
void					SetStringProperty( XMLNode& node, const std::string& propertyName, const std::string& value, const std::string& defaultValue, bool createChild );

///-----------------------------------------------------------------------
/// Functions that the XML Utility functions depend upon
///-----------------------------------------------------------------------
bool					GetChildNodeAtPosition( const XMLNode& parentNode, const std::string& childName, int& position, XMLNode& childNode );
bool					GetChildNodeAtPosition( const XMLNode& parentNode, int& position, XMLNode& childNode );
std::string				GetPropertyValueAsString( const XMLNode& node, const std::string& propertyName );
std::string				GetPropertyValueAsStringWithVariableLookup( const XMLNode& node, const std::string& propertyName );
int						GetIntFromString( const std::string& intString );
bool					GetBooleanFromString( const std::string& boolString );
float					GetFloatFromString( const std::string& floatString );
float					GetDoubleProperty( const XMLNode& node, const std::string& propertyName, float defaultValue );
void					SetStringAttribute( XMLNode& node, const std::string& propertyName, const std::string& value );
void					GetUnusedElements( XMLNode& node, std::vector< XMLNode >& unusedElements );


//-----------------------------------------------------------------------------------------------
inline const bool operator > ( const Vector2&, const Vector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation > not supported on this Vector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator >= ( const Vector2&, const Vector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation >= not supported on this Vector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator < ( const Vector2&, const Vector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation < not supported on this Vector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator <= ( const Vector2&, const Vector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation <= not supported on this Vector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator > ( const IntVector2&, const IntVector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation > not supported on this IntVector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator >= ( const IntVector2&, const IntVector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation >= not supported on this IntVector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator < ( const IntVector2&, const IntVector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation < not supported on this IntVector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator <= ( const IntVector2&, const IntVector2& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation <= not supported on this IntVector2" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator > ( const Rgba&, const Rgba& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation > not supported on this Rgba" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator >= ( const Rgba&, const Rgba& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation >= not supported on this Rgba" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator < ( const Rgba&, const Rgba& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation < not supported on this Rgba" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
inline const bool operator <= ( const Rgba&, const Rgba& )
{
	//{ int q = 5; } //JAZZ_ERROR( "Operation unsupported", Stringf( "Operation <= not supported on this Rgba" ) );
	return false;
}


//-----------------------------------------------------------------------------------------------
template< typename ValueType >
ValueType GetXMLPropertyWithVariableLookup( XMLNode& node, const std::string& propertyName )
{
	ValueType	outValue;
	std::string	valueAsString	= GetPropertyValueAsStringWithVariableLookup( node, propertyName );
	SetTypeFromString( outValue, valueAsString );
	return outValue;
}


//-----------------------------------------------------------------------------------------------
template< typename ValueType >
ValueType GetXMLProperty( XMLNode& node, const std::string& propertyName )
{
	ValueType	outValue ;
	outValue = GetXMLPropertyWithVariableLookup< ValueType >( node, propertyName );
	return outValue;
}


#endif // __include_ParsingSupport__
