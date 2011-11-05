//-----------------------------------------------------------------------------------------------
// HashedCaseInsensitiveString.hpp
//
// Copyright 2009 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_HashedCaseInsensitiveString__
#define __include_HashedCaseInsensitiveString__
#pragma once

#include "Utilities.hpp"


typedef unsigned int	CaseInsensitiveStringHashValue;


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	HashedCaseInsensitiveString
//
// Stores a standard string, case intact.  On creation, generates and stores a 32-bit
//	nearly-unique hash value based on the caseless (all lowercase) version of that string.
//
// Comparisons involving HashedCaseInsensitiveString report false if the hash values do not
//	match (fast), and true if the has values match AND the strings are identical (without
//	regard to case).
//
// Since negative matches are fast (int compare) and positive matches are not (string compare),
//	this class is best suited for cases where negative compares are more common than positive
//	compares, e.g. searching for a matched string in a list, or as the "first" key in a map.
//
/////////////////////////////////////////////////////////////////////////////////////////////////
class HashedCaseInsensitiveString
{
private:
	std::string m_originalString; // original case remains preserved here
	CaseInsensitiveStringHashValue m_hashValue; // nearly-unique 32-bit hash of caseless (tolower'd) string
//	static std::map< CaseInsensitiveStringHashValue, std::string >* s_hashedStringRegistry; // mostly for debugging purposes, this (can) register a string when its hash is computed.  This can be used (a) to fetch strings-from-hashes (e.g. for error messages involving NamedProperties, JazzUI, etc.) and (b) to enable paranoia-level debug checks to make sure we're not getting hash-generation collisions

public:
	~HashedCaseInsensitiveString() {}
	HashedCaseInsensitiveString();
	HashedCaseInsensitiveString( const std::string& originalString );
	HashedCaseInsensitiveString( const HashedCaseInsensitiveString& copy );

	static CaseInsensitiveStringHashValue ComputeHashForCaseInsensitiveString( const std::string& stringWhoseCaseWillBeIgnoredForHashGeneration, bool registerThisString = false );
//	static bool GetRegisteredStringForHashValue( const CaseInsensitiveStringHashValue hashValueToLookUp, std::string& out_registeredStringIfFound );
/*
	static void SetRegisteredStringForHashValue( const CaseInsensitiveStringHashValue hasValueForRegisteredString, const std::string& registeredString )
	{
		// if( paranoia checks enabled )
		// {
		//    find it in the map, and see if the strings pass !Stricmp
		// }
		if ( !s_hashedStringRegistry )
			s_hashedStringRegistry = new std::map< CaseInsensitiveStringHashValue, std::string >();

		(*s_hashedStringRegistry)[ hasValueForRegisteredString ] = registeredString ;
	}
*/

	void SetString( const std::string& newString );
	const std::string& GetString() const { return m_originalString; }
	CaseInsensitiveStringHashValue GetHashValue() const { return m_hashValue; }
	bool operator == ( const HashedCaseInsensitiveString& rhs ) const;
	bool operator != ( const HashedCaseInsensitiveString& rhs ) const;
	bool operator < ( const HashedCaseInsensitiveString& rhs ) const;
};


//-----------------------------------------------------------------------------------------------
STATIC inline CaseInsensitiveStringHashValue HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( const std::string& stringWhoseCaseWillBeIgnoredForHashGeneration, bool registerThisString )
{
	CaseInsensitiveStringHashValue hashValue = 0;
	size_t stringLength = stringWhoseCaseWillBeIgnoredForHashGeneration.size();
	for( unsigned int i = 0; i < stringLength; ++ i )
	{
		hashValue &= 0x07ffffff;
		hashValue *= 31;
		hashValue += tolower( stringWhoseCaseWillBeIgnoredForHashGeneration[i] );
	}

	UNUSED( registerThisString );
/*
	if( registerThisString )
	{
		SetRegisteredStringForHashValue( hashValue, stringWhoseCaseWillBeIgnoredForHashGeneration );
	}
*/

	return hashValue;
}


//-----------------------------------------------------------------------------------------------
inline HashedCaseInsensitiveString::HashedCaseInsensitiveString()
	: m_hashValue( 0 )
{
}


//-----------------------------------------------------------------------------------------------
inline HashedCaseInsensitiveString::HashedCaseInsensitiveString( const std::string& originalString )
	: m_originalString( originalString )
{
	m_hashValue = ComputeHashForCaseInsensitiveString( originalString );
}


//-----------------------------------------------------------------------------------------------
inline HashedCaseInsensitiveString::HashedCaseInsensitiveString( const HashedCaseInsensitiveString& copy )
	: m_originalString( copy.m_originalString )
	, m_hashValue( copy.m_hashValue )
{
}


//-----------------------------------------------------------------------------------------------
inline void HashedCaseInsensitiveString::SetString( const std::string& newString )
{
	m_originalString = newString;
	m_hashValue = ComputeHashForCaseInsensitiveString( newString );
}


//-----------------------------------------------------------------------------------------------
inline bool HashedCaseInsensitiveString::operator == ( const HashedCaseInsensitiveString& rhs ) const
{
	// Check hash values first
	if( m_hashValue != rhs.m_hashValue )
		return false;

	// If hash values are equal, do a case-insensitive compare of the strings
	if( !Stricmp( m_originalString, rhs.m_originalString ) )
		return true;

	return false;
}


//-----------------------------------------------------------------------------------------------
inline bool HashedCaseInsensitiveString::operator != ( const HashedCaseInsensitiveString& rhs ) const
{
	// Check hash values first
	if( m_hashValue != rhs.m_hashValue )
		return true;

	// If hash values are equal, do a case-insensitive compare of the strings
	if( !Stricmp( m_originalString, rhs.m_originalString ) )
		return false;

	return true;
}


//-----------------------------------------------------------------------------------------------
inline bool HashedCaseInsensitiveString::operator < ( const HashedCaseInsensitiveString& rhs ) const
{
	// Check if our hash value is less than rhs's
	if( m_hashValue < rhs.m_hashValue )
		return true;

	// Check if our hash value is more than rhs's
	if( m_hashValue > rhs.m_hashValue )
		return false;

	// Hash values were equal; compare strings (case-insensitive)
	if( Stricmp( m_originalString, rhs.m_originalString ) < 0 )
		return true;

	return false;
}


#endif // __include_HashedCaseInsensitiveString__
