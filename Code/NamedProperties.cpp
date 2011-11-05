//-----------------------------------------------------------------------------------------------
// NamedProperties.cpp
//
// Copyright 2009 Jazz Game Technologies.  See Utilities/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "NamedProperties.hpp"


//-----------------------------------------------------------------------------------------------
// Static member initialization
//
const STATIC NamedProperties NamedProperties::s_empty;


/*
//-----------------------------------------------------------------------------------------------
void NamedProperties::GetPropertyNamesAsStrings( std::vector< std::string >& outNameStrings )
{
	TypedPropertyMap::iterator iter;
	for( iter = m_properties.begin(); iter != m_properties.end(); ++ iter )
	{
#if defined( USE_HASHED_PROPERTY_KEYS )
		const CaseInsensitiveStringHashValue& hashValue = iter->first;
		std::string registeredString;
		HashedCaseInsensitiveString::GetRegisteredStringForHashValue( hashValue, registeredString );
		outNameStrings.push_back( registeredString );
#else
		const HashedCaseInsensitiveString& hashedCaseInsensitiveString = iter->first;
		outNameStrings.push_back( hashedCaseInsensitiveString.GetString() );
#endif
	}
}
*/


//-----------------------------------------------------------------------------------------------
void NamedProperties::GetPropertyValuesAsStrings( std::vector< std::string >& outValueStrings )
{
	TypedPropertyMap::iterator iter;
	for( iter = m_properties.begin(); iter != m_properties.end(); ++ iter )
	{
		TypedPropertyBase& property = *iter->second;
		outValueStrings.push_back( property.GetValueAsString() );
	}
}


//-----------------------------------------------------------------------------------------------
void NamedProperties::GetPropertyTypesAsStrings( std::vector< std::string >& outTypeStrings )
{
	TypedPropertyMap::iterator iter;
	for( iter = m_properties.begin(); iter != m_properties.end(); ++ iter )
	{
		TypedPropertyBase& property = *iter->second;
		outTypeStrings.push_back( property.GetTypeAsString() );
	}
}

