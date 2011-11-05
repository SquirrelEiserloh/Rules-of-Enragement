///-----------------------------------------------------------------------
/// ParsingSupport.cpp -- Implementation of convenience functions for parsing.
///-----------------------------------------------------------------------
#include "ParsingSupport.hpp"
#include "Clock.hpp"
#include "NamedProperties.hpp"

///====================================================================================
///             CONVENIENCE XML FUNCTIONS
///====================================================================================




///----------------------------------------------------------
/// 
///----------------------------------------------------------
std::string GetXMLNodeName( XMLNode xmlNode )
{
	const char* cString = xmlNode.getName();
	return cString ? cString : "";
}


///----------------------------------------------------------
/// 
///----------------------------------------------------------
std::string GetXMLErrorDescForErrorCode( XMLError xmlErrorCode )
{
	switch( xmlErrorCode )
	{
		case eXMLErrorNone:
		case eXMLErrorMissingEndTag:					return "MissingEndTag";
		case eXMLErrorNoXMLTagFound:					return "NoXMLTagFound";
		case eXMLErrorEmpty:							return "Empty";
		case eXMLErrorMissingTagName:					return "MissingTagName";
		case eXMLErrorMissingEndTagName:				return "MissingEndTagName";
		case eXMLErrorUnmatchedEndTag:					return "UnmatchedEndTag";
		case eXMLErrorUnmatchedEndClearTag:				return "UnmatchedEndClearTag";
		case eXMLErrorUnexpectedToken:					return "UnexpectedToken";
		case eXMLErrorNoElements:						return "NoElements";
		case eXMLErrorFileNotFound:						return "FileNotFound";
		case eXMLErrorFirstTagNotFound:					return "FirstTagNotFound";
		case eXMLErrorUnknownCharacterEntity:			return "UnknownCharacterEntity";
		case eXMLErrorCharConversionError:				return "CharConversionError";
		case eXMLErrorCannotOpenWriteFile:				return "CannotOpenWriteFile";
		case eXMLErrorCannotWriteFile:					return "CannotWriteFile";
		case eXMLErrorBase64DataSizeIsNotMultipleOf4:	return "Base64DataSizeIsNotMultipleOf4";
		case eXMLErrorBase64DecodeIllegalCharacter:		return "Base64DecodeIllegalCharacter";
		case eXMLErrorBase64DecodeTruncatedData:		return "Base64DecodeTruncatedData";
		case eXMLErrorBase64DecodeBufferTooSmall:		return "Base64DecodeBufferTooSmall";
		default:										return Stringf( "Unknown XML error code #%d", xmlErrorCode );
	};
}


///----------------------------------------------------------
/// 
///----------------------------------------------------------
void ReportXMLError( const ResourceStream& resourceStream, XMLNode xmlDocumentRootNode, XMLResults errorResult )
{
	if( errorResult.error == eXMLErrorNone )
		return;

	const std::string shortFileName = resourceStream.GetJazzPath();
	const std::string longFileName = resourceStream.GetJazzPath();
	std::string errorTitle = "XML Data File Error";
	std::string errorText = Stringf( "Error in XML data file \"%s\":\n", longFileName.c_str() );

	bool locateErrorInText = true;
	const std::string errorName = GetXMLErrorDescForErrorCode( errorResult.error );
	std::string errorDescription = Stringf( "\n  ***  %s  ***\n", errorName.c_str() );

//	if( errorResult == eXMLErrorUnexpectedToken )
//	{
//		Set "errorDescription" here to be as specific as you need to be, for each particular error
//	}

	errorText += errorDescription;

	if( locateErrorInText )
	{
		errorText += Stringf( "\nError location: line %d (column %d) of %s\n", errorResult.nLine, errorResult.nColumn, shortFileName.c_str() );

		const std::string errorLine = resourceStream.FetchLine( errorResult.nLine );
		std::string errorLinePartial( errorLine, 0, errorResult.nColumn-1 );

		errorText += Stringf( "\nLine %d is probably:\n%s\n", errorResult.nLine, errorLine.c_str() );
		if( errorLinePartial.length() < errorLine.length() )
		{
			errorText += Stringf( "\n...and the error occurred somewhere in this initial section:\n%s <== ERROR HERE\n", errorLinePartial.c_str() );
		}
	}

	//{ int q = 5; } //JAZZ_ERROR( errorTitle, errorText );
}


///----------------------------------------------------------
/// 
///----------------------------------------------------------
XMLNode CreateXMLDocumentFromResourceStream( const ResourceStream& resourceStream, const std::string& optionalExpectedRootNodeName )
{
	XMLResults results;
	const char* resourceStreamAsciiFileBuffer = (const char*)resourceStream.GetDirectBufferAccess();
	XMLNode xmlDocumentRootNode = XMLNode::parseString( resourceStreamAsciiFileBuffer, NULL, &results );

	// Report errors here if "results" indicates any problems
	if( results.error != eXMLErrorNone )
	{
		ReportXMLError( resourceStream, xmlDocumentRootNode, results );
	}

	// Advance past the "xml declaration" node, if present; e.g.: <?xml version="1.0" encoding="us-ascii"?>
	// ...OR, if the root node is nameless, then it must be an implicit document-root (usually in an XML without a declaration), skip that
	while( (xmlDocumentRootNode.isDeclaration() || !xmlDocumentRootNode.getName()) && xmlDocumentRootNode.nChildNode() > 0 )
	{
		xmlDocumentRootNode = xmlDocumentRootNode.getChildNode( 0 );
	}

	if( !optionalExpectedRootNodeName.empty() )
	{
		const std::string rootNodeName = xmlDocumentRootNode.getName();
		if( Stricmp( optionalExpectedRootNodeName, rootNodeName ) )
		{
			//JAZZ_ERROR( "XML Data File Error", Stringf( "In XML data file %s, was expecting a root node\n type of <%s>, but found <%s> instead!", resourceStream.GetJazzPath()..c_str(), optionalExpectedRootNodeName.c_str(), rootNodeName.c_str() ) );
		}
	}

	return xmlDocumentRootNode;
}


///----------------------------------------------------------
/// 
///----------------------------------------------------------
XMLNode CreateXMLDocumentFromXMLFile( const JazzPath& xmlFile, const std::string& optionalExpectedRootNodeName )
{
//	const double startSeconds = Clock::GetAbsoluteTimeSeconds();
	ResourceStream resourceStream( xmlFile );
//	const double secondsAfterResourceStream = Clock::GetAbsoluteTimeSeconds();
	XMLNode xmlDocumentRootNode = CreateXMLDocumentFromResourceStream( resourceStream, optionalExpectedRootNodeName );
//	const double secondsAfterXMLDocumentCreated = Clock::GetAbsoluteTimeSeconds();
//	const double secondsToCreateResourceStream = (secondsAfterResourceStream - startSeconds);
//	const double secondsToCreateXMLDocument = (secondsAfterXMLDocumentCreated - secondsAfterResourceStream);
//	const double secondsTotal = (secondsAfterXMLDocumentCreated - startSeconds);
//	const float fractionSpentCreatingXMLDocument = secondsTotal > 0.0 ? (float)( secondsToCreateXMLDocument / secondsTotal ) : 0.f;
//	const std::string xmlDocumentType = optionalExpectedRootNodeName.empty() ? "generic" : optionalExpectedRootNodeName;
//	ConsolePrintf( Rgba::GREY, "Created %s XML document from file \"%s\" in %.0fms (%.0f%% for XML construction)\n", xmlDocumentType.c_str(), xmlFile..c_str(), 1000.0 * secondsTotal, 100.f * fractionSpentCreatingXMLDocument );

	return xmlDocumentRootNode;
}


///-----------------------------------------------------------------------
///
///-----------------------------------------------------------------------
void ReportXMLDocumentUsage( XMLNode& xmlDocumentToReport, const ResourceStream& resourceStream, bool reportUsed )
{
	UNUSED( reportUsed );

	std::vector< XMLNode > unusedElements ;
	GetUnusedElements( xmlDocumentToReport, unusedElements );

	if ( !unusedElements.empty() )
	{
		std::string warningMessage = Stringf( "Unused elements in file: %s\n", resourceStream.GetJazzPath().c_str() );
		for ( std::vector< XMLNode >::iterator xmlNodeIter = unusedElements.begin(); xmlNodeIter != unusedElements.end(); ++xmlNodeIter )
		{
			XMLNode& unusedNode = (*xmlNodeIter);
			int stringSize = 0 ;
			char* nodeString = unusedNode.createXMLString( 1, &stringSize );
			if ( nodeString )
			{
				if ( stringSize > 2048 )
				{
					nodeString[ 2040 ] = '.' ;
					nodeString[ 2041 ] = '.' ;
					nodeString[ 2042 ] = '.' ;
					nodeString[ 2043 ] = 0 ;
				}
				delete nodeString ;
			}
		}

		NamedProperties warningProperties ;
		warningProperties.Set( "Warning", warningMessage );
//		FireEvent( "SystemWarningEvent", warningProperties );
	}

}

///-----------------------------------------------------------------------
///
///-----------------------------------------------------------------------
void GetUnusedElements( XMLNode& node, std::vector< XMLNode >& unusedElements )
{
	if ( !node.isUsed() )
		unusedElements.push_back( node );

	int numberOfChildElements = node.nChildNode();
	for ( int childIndex = 0; childIndex < numberOfChildElements ; ++childIndex )
	{
		XMLNode childNode = node.getChildNode( childIndex );
		GetUnusedElements( childNode, unusedElements );
	}

}


///----------------------------------------------------------
/// Destroys all the allocated data for an XMLNode tree. 
///----------------------------------------------------------
void DestroyXMLDocument( XMLNode& xmlDocumentToDestroy )
{
	while ( !xmlDocumentToDestroy.getParentNode().isEmpty() )
	{
		xmlDocumentToDestroy = xmlDocumentToDestroy.getParentNode();
	}

	xmlDocumentToDestroy.deleteNodeContent();
}


///----------------------------------------------------------
/// Retrieves the childNode with the specified name at the 
/// specified position of the parent.  Returns true if the child
/// is not empty (and therefore likely to be a valid child.
///----------------------------------------------------------
bool GetChildNodeAtPosition( const XMLNode& parentNode, const std::string& childName, int& position, XMLNode& childNode )
{
	parentNode.markUsed();

	bool wasFound	= false ;
	childNode		= parentNode.getChildNode( childName.c_str(), position++ );
	if ( !childNode.isEmpty() ) 
	{
		childNode.markUsed();
		wasFound = true ;
	}

	return wasFound ;
}


///----------------------------------------------------------
/// Retrieves the childNode with the specified name at the 
/// specified position of the parent.  Returns true if the child
/// is not empty (and therefore likely to be a valid child.
///----------------------------------------------------------
bool GetChildNodeAtPosition( const XMLNode& parentNode, int& position, XMLNode& childNode )
{
	parentNode.markUsed();

	bool wasFound	= false ;
	childNode		= parentNode.getChildNode( position++ );
	if ( !childNode.isEmpty() ) 
	{
		childNode.markUsed();
		wasFound = true ;
	}

	return wasFound ;
}


///------------------------------------------------------------------
/// Retrieves the named property (as either an attribute or child node)
/// and returns it as a string.
///------------------------------------------------------------------
std::string GetPropertyValueAsString( const XMLNode& node, const std::string& propertyName )
{
	std::string	propertyValue ;
	XMLNode		childNode		= node.getChildNode( propertyName.c_str() );

	// First try to get it as an attribute with that property name
	if ( node.getAttribute( propertyName.c_str() ) )
	{
		propertyValue	= node.getAttribute( propertyName.c_str() );
		node.markUsed();
	}
	// Second try to get it as the text of a child node with that property name
	else if ( childNode.hasText() )
	{
		propertyValue	= childNode.getText();
		node.markUsed();
	}
	// Third, try to get it as an attribute named "Value" of the child node with that property name
	else if ( !childNode.isEmpty() )
	{
		propertyValue	= GetPropertyValueAsString( childNode, "Value" );
		node.markUsed();
	}

	return propertyValue ;
}

///-----------------------------------------------------------------------
/// Retrieves the named property (as either an attribute or child node)
/// and returns it as a string.  Processes the result through the variables
/// system to expand it before returning the expanded value.
///-----------------------------------------------------------------------
std::string GetPropertyValueAsStringWithVariableLookup( const XMLNode& node, const std::string& propertyName )
{
	std::string	valueBeforeVariableLookup	= GetPropertyValueAsString( node, propertyName );
	std::string valueAfterVariableLookup	= valueBeforeVariableLookup; // JazzSubstitutionVariableSet::ExpandJazzVariablesInString( valueBeforeVariableLookup );
	
	return valueAfterVariableLookup ;
}


///------------------------------------------------------------------
/// Retrieves a boolean property from the specified Node
///------------------------------------------------------------------
bool GetBooleanProperty( const XMLNode& node, const std::string& propertyName, bool defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	bool		propertyValue	= defaultValue ;

	if ( propertyString.length() )
		propertyValue			= GetBooleanFromString( propertyString );

	return propertyValue ;
}

///------------------------------------------------------------------
/// Retrieves a float property from the specified Node
///------------------------------------------------------------------
float GetFloatProperty( const XMLNode& node, const std::string& propertyName, float defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	float		propertyValue	= defaultValue ;

	if ( propertyString.length() )
		propertyValue			= GetFloatFromString( propertyString );

	return propertyValue ;
}

///------------------------------------------------------------------
/// Retrieves a double property from the specified Node
///------------------------------------------------------------------
float GetDoubleProperty( const XMLNode& node, const std::string& propertyName, float defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	float		propertyValue	= defaultValue ;

	if ( propertyString.length() )
		ConvertType( propertyString, propertyValue );

	return propertyValue ;
}

///------------------------------------------------------------------
/// Retrieves an integer property from the specified Node
///------------------------------------------------------------------
int GetIntProperty( const XMLNode& node, const std::string& propertyName, int defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	int		propertyValue	= defaultValue ;

	if ( propertyString.length() )
		propertyValue			= GetIntFromString( propertyString );

	return propertyValue ;
}

///------------------------------------------------------------------
/// Retrieves a character property from the specified Node
///------------------------------------------------------------------
char GetCharacterProperty( const XMLNode& node, const std::string& propertyName, char defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	char		propertyValue	= defaultValue ;

	if ( propertyString.length() == 1 )
		propertyValue			= propertyString.c_str()[0];

	return propertyValue ;
}

///------------------------------------------------------------------
/// Retrieves a string property from the specified Node
///------------------------------------------------------------------
std::string GetStringProperty( const XMLNode& node, const std::string& propertyName, const std::string& defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	std::string	propertyValue	= defaultValue ;

	if ( propertyString.length() )
		propertyValue			= propertyString ;

	return propertyValue ;
}


///------------------------------------------------------------------
/// Retrieves an Rgba property from the specified Node
///------------------------------------------------------------------
Rgba GetRgbaProperty( const XMLNode& node, const std::string& propertyName, const Rgba& defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	Rgba		propertyValue	= defaultValue ;

	if ( propertyString.length() )
		propertyValue			= Rgba( propertyString );

	return propertyValue;
}


///-----------------------------------------------------------------------
/// Retrieves a Vector2 property from the specified Node
///-----------------------------------------------------------------------
Vector2 GetVector2Property( const XMLNode& node, const std::string& propertyName, const Vector2& defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	Vector2		propertyValue	= defaultValue ;

	if( propertyString.length() )
		propertyValue = Vector2( propertyString );

	return propertyValue;
}


///-----------------------------------------------------------------------
/// Retrieves a IntVector2 property from the specified Node
///-----------------------------------------------------------------------
IntVector2 GetIntVector2Property( const XMLNode& node, const std::string& propertyName, const IntVector2& defaultValue )
{
	std::string	propertyString	= GetPropertyValueAsString( node, propertyName );
	IntVector2		propertyValue	= defaultValue ;

	if( propertyString.length() )
		propertyValue = IntVector2( propertyString );

	return propertyValue;
}


///-------------------------------------------------------------------
/// Adds an attribute to the specified node with the string value.
///-------------------------------------------------------------------
void SetStringAttribute( XMLNode& node, const std::string& propertyName, const std::string& value )
{
	node.addAttribute( propertyName.c_str(), value.c_str() );
}

///--------------------------------------------------------------------
/// Adds a float property to the node, either as an attribute, or as a 
/// child node with the specified name, and a "Value" attribute.
///--------------------------------------------------------------------
void SetStringProperty( XMLNode& node, const std::string& propertyName, const std::string& value, const std::string& defaultValue, bool createChild )
{
	if ( defaultValue == value ) return ; // Optimization
	if ( createChild )
	{
		XMLNode childNode = node.addChild( propertyName.c_str() );
		SetStringAttribute( childNode, "Value", value );
	}
	else
	{
		SetStringAttribute( node, propertyName, value );
	}
}

///---------------------------------------------------------------------
/// Adds a float property to the node, either as an attribute, or as a 
/// child node with the specified name, and a "Value" attribute.
///---------------------------------------------------------------------
void SetFloatProperty( XMLNode& node, const std::string& propertyName, float value, float defaultValue, bool createChild )
{
	SetStringProperty( node, propertyName, GetFloatAsString( value ), GetFloatAsString( defaultValue ), createChild );
}

///---------------------------------------------------------------------
/// Adds an int property to the node, either as an attribute, or as a 
/// child node with the specified name, and a "Value" attribute.
///---------------------------------------------------------------------
void SetIntProperty( XMLNode& node, const std::string& propertyName, int value, int defaultValue, bool createChild )
{
	SetStringProperty( node, propertyName, GetIntAsString( value ), GetIntAsString( defaultValue), createChild );
}

///---------------------------------------------------------------------
/// Adds a boolean property to the node, either as an attribute, or as a 
/// child node with the specified name, and a "Value" attribute.
///---------------------------------------------------------------------
void SetBooleanProperty( XMLNode& node, const std::string& propertyName, bool value, bool defaultValue, bool createChild )
{
	SetStringProperty( node, propertyName, GetBoolAsString( value ), GetBoolAsString( defaultValue), createChild );
}

/*
//-----------------------------------------------------------------------------------------------
std::string	GetFloatAsString( float value )
{
	char text[ 10 ] ;
	sprintf_s( text, 10, "%3.2f", value );
	return std::string( text );
}


//-----------------------------------------------------------------------------------------------
std::string GetIntAsString( int value )
{
	char text[ 10 ] ;
	sprintf_s( text, 10, "%d", value );
	return std::string( text );
}


//-----------------------------------------------------------------------------------------------
std::string GetBoolAsString( bool value )
{
	return value ? "True" : "False" ;
}
*/

//-----------------------------------------------------------------------------------------------
float GetFloatFromString( const std::string& floatString )
{
	float floatValue = 0.0f ;
#ifdef __APPLE__
	sscanf( floatString.c_str(), "%f", &floatValue );
#else
	sscanf_s( floatString.c_str(), "%f", &floatValue );
#endif
	return floatValue ;
}


//-----------------------------------------------------------------------------------------------
bool GetBooleanFromString( const std::string& boolString )
{
	if ( boolString == "True"  ||
		boolString == "true"  ||
		boolString == "1" )
		return true ;
	return false ;
}


//-----------------------------------------------------------------------------------------------
int	GetIntFromString( const std::string& intString )
{
	int intValue = 0 ;
#ifdef __APPLE__
	sscanf( intString.c_str(), "%d", &intValue );
#else
	sscanf_s( intString.c_str(), "%d", &intValue );
#endif
	return intValue ;
}



