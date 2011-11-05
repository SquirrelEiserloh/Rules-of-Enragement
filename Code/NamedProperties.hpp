//-----------------------------------------------------------------------------------------------
// NamedProperties.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_NamedProperties__
#define __include_NamedProperties__
#pragma once

#include "TypeUtilities.hpp"
#include "HashedCaseInsensitiveString.hpp"


#define USE_HASHED_PROPERTY_KEYS // if defined, we should key on the hash values themselves rather than on HashedCaseInsensitiveString objects


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		TypedPropertyBase
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class TypedPropertyBase
{
//protected:
public:
	friend class NamedProperties;

	TypedPropertyBase() {}
	virtual ~TypedPropertyBase() {}
	virtual TypedPropertyBase*		Clone()																= 0;
	virtual bool					IsTypeCompatible( TypedPropertyBase* compareProperty )				= 0;
	virtual std::string				GetValueAsString() const											= 0;
	virtual std::string				GetTypeAsString() const												= 0;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		TypedProperty
//
///////////////////////////////////////////////////////////////////////////////////////////////////
template < typename T_PropertyType >
class TypedProperty : public TypedPropertyBase
{
public:
	T_PropertyType*				GetValue( void )		{ return m_value ; }

//private:
	friend class NamedProperties;

	~TypedProperty()
	{
		delete m_value;
	}

	TypedProperty( const T_PropertyType& initializationValue )
	{
		m_value = new T_PropertyType( initializationValue );
	}

	TypedProperty( const TypedProperty< T_PropertyType >& copy )
	{
		m_value = new T_PropertyType( *copy.m_value );
	}

	virtual TypedPropertyBase* Clone()
	{
		return new TypedProperty< T_PropertyType >( *this );
	}

	virtual bool IsTypeCompatible( TypedPropertyBase* compareProperty )
	{
		TypedProperty< T_PropertyType >* comparePtr = dynamic_cast< TypedProperty< T_PropertyType >* >( compareProperty );
		return ( comparePtr != NULL );
	}

	virtual std::string GetValueAsString() const
	{
		return GetTypedObjectAsString( *m_value, NULL );
	}

	virtual std::string GetTypeAsString() const
	{
		std::string typeAsString;
		GetTypedObjectAsString( *m_value, &typeAsString );
		return typeAsString;
	}

	T_PropertyType*		m_value;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		PropertyFetchResult
//
///////////////////////////////////////////////////////////////////////////////////////////////////
enum PropertyFetchResult
{
	PROPERTY_FETCH_OK,
	PROPERTY_NOT_FOUND,
	NAMED_PROPERTIES_WAS_EMPTY,
	PROPERTY_WAS_MISMATCHED_TYPE
};


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//		NamedProperties
//
///////////////////////////////////////////////////////////////////////////////////////////////////
class NamedProperties
{
public:
	const static NamedProperties s_empty;

#if defined( USE_HASHED_PROPERTY_KEYS )
	typedef std::map< CaseInsensitiveStringHashValue, TypedPropertyBase* > TypedPropertyMap;
#else
	typedef std::map< HashedCaseInsensitiveString, TypedPropertyBase* > TypedPropertyMap;
#endif

	///---------------------------------------------------------------------
	/// Constructors/Destructor
	///---------------------------------------------------------------------
	virtual ~NamedProperties();
	NamedProperties() {}
	NamedProperties( const NamedProperties& copy );
	NamedProperties& operator = ( const NamedProperties& rhs );

	template < typename T_PropertyType >
	explicit NamedProperties( const std::string& propertyName, const T_PropertyType& value );

	template < typename T_PropertyType1, typename T_PropertyType2 >
	explicit NamedProperties( const std::string& propertyName1, const T_PropertyType1& value1, const std::string& propertyName2, const T_PropertyType2& value2 );

	template < typename T_PropertyType1, typename T_PropertyType2, typename T_PropertyType3 >
	explicit NamedProperties( const std::string& propertyName1, const T_PropertyType1& value1, const std::string& propertyName2, const T_PropertyType2& value2, const std::string& propertyName3, const T_PropertyType3& value3 );


	///---------------------------------------------------------------------
	/// Accessors
	///---------------------------------------------------------------------
	template< typename T_PropertyType >
	PropertyFetchResult						Get( const std::string& propertyName, T_PropertyType& outValue ) const;

	template < typename T_PropertyType >
	PropertyFetchResult						Get( CaseInsensitiveStringHashValue hashValue, T_PropertyType& outValue ) const ;

	template< typename T_PropertyType >
	PropertyFetchResult						GetPointer( const std::string& propertyName, T_PropertyType*& outValue );

	template< typename T_PropertyType >
	PropertyFetchResult						GetPointer( CaseInsensitiveStringHashValue propertyNameHashValue, T_PropertyType*& outValue );

	PropertyFetchResult						GetPropertyValueAsString( const std::string& propertyName, std::string& outValueString );
	PropertyFetchResult						GetPropertyTypeAsString( const std::string& propertyName, std::string& outTypeString );

	int										GetNumberOfProperties() const { return (int) m_properties.size(); }
	bool									IsEmpty() const { return m_properties.empty(); }
	TypedPropertyMap&						GetPropertyMap() { return m_properties ; }
//	void									GetPropertyNamesAsStrings( std::vector< std::string >& outNameStrings );
	void									GetPropertyValuesAsStrings( std::vector< std::string >& outValueStrings );
	void									GetPropertyTypesAsStrings( std::vector< std::string >& outTypeStrings );


	///---------------------------------------------------------------------
	/// Mutators
	///---------------------------------------------------------------------
	template < typename T_PropertyType >
	void									Set( const std::string& propertyName, const T_PropertyType& value );
	void									Set( const std::string& propertyName, const char* value ); // Custom override allows more flexible use of strings
	bool									CopyPropertyByName( const std::string& propertyName, const NamedProperties& srcProperties ); // Pull a prop from one set to another
	bool									Remove( const std::string& propertyName ); // Clear out a property; returns false if the property didn't exist to begin with

	///----------------------------------------------------------------------
	/// Methods
	///----------------------------------------------------------------------
	void									Clear( void );
	void									Insert( const NamedProperties& src );


private:
	TypedPropertyMap m_properties;
};


//-----------------------------------------------------------------------------------------------
inline NamedProperties::~NamedProperties()
{
	Clear();
}


//-----------------------------------------------------------------------------------------------
inline NamedProperties::NamedProperties( const NamedProperties& copy )
{
	TypedPropertyMap::const_iterator iter;
	for( iter = copy.m_properties.begin(); iter != copy.m_properties.end(); ++ iter )
	{
		m_properties[ iter->first ]	= iter->second->Clone();
	}
}


//------------------------------------------------------------------------------------------------
template < typename T_PropertyType >
inline NamedProperties::NamedProperties( const std::string& propertyName, const T_PropertyType& value )
{
	Set( propertyName, value );
}


//------------------------------------------------------------------------------------------------
template < typename T_PropertyType1, typename T_PropertyType2 >
inline NamedProperties::NamedProperties( const std::string& propertyName1, const T_PropertyType1& value1, const std::string& propertyName2, const T_PropertyType2& value2 )
{
	Set( propertyName1, value1 );
	Set( propertyName2, value2 );
}


//------------------------------------------------------------------------------------------------
template < typename T_PropertyType1, typename T_PropertyType2, typename T_PropertyType3 >
inline NamedProperties::NamedProperties( const std::string& propertyName1, const T_PropertyType1& value1, const std::string& propertyName2, const T_PropertyType2& value2, const std::string& propertyName3, const T_PropertyType3& value3 )
{
	Set( propertyName1, value1 );
	Set( propertyName2, value2 );
	Set( propertyName3, value3 );
}


//------------------------------------------------------------------------------------------------
inline void NamedProperties::Insert( const NamedProperties& src )
{
	TypedPropertyMap::const_iterator iter;
	for( iter = src.m_properties.begin(); iter != src.m_properties.end(); ++iter )
	{
		m_properties[ iter->first ]	= iter->second->Clone();
	}
}

//-----------------------------------------------------------------------------------------------
inline NamedProperties& NamedProperties::operator = ( const NamedProperties& rhs )
{
	Clear();
	TypedPropertyMap::const_iterator iter;
	for( iter = rhs.m_properties.begin(); iter != rhs.m_properties.end(); ++ iter )
	{
		m_properties[ iter->first ]	= iter->second->Clone();
	}

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline void NamedProperties::Clear()
{
	TypedPropertyMap::iterator iter;
	for( iter = m_properties.begin(); iter != m_properties.end(); ++ iter )
	{
		delete iter->second;
	}
	m_properties.clear();
}


//-----------------------------------------------------------------------------------------------
template < typename T_PropertyType >
inline void NamedProperties::Set( const std::string& propertyName, const T_PropertyType& value )
{
	TypedProperty< T_PropertyType >* newProperty = new TypedProperty< T_PropertyType >( value );
#if defined( USE_HASHED_PROPERTY_KEYS )
	CaseInsensitiveStringHashValue hashValue = HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( propertyName, true );
	TypedPropertyMap::iterator found = m_properties.find( hashValue );
#else
	HashedCaseInsensitiveString hashedPropertyName( propertyName );
	TypedPropertyMap::iterator found = m_properties.find( hashedPropertyName );
#endif
	if( found != m_properties.end() )
	{
		delete found->second;
		found->second = newProperty;
	}
	else
	{
#if defined( USE_HASHED_PROPERTY_KEYS )
		m_properties[ hashValue ] = newProperty;
#else
		m_properties[ hashedPropertyName ] = newProperty;
#endif
	}
}


//-----------------------------------------------------------------------------------------------
// Custom override allows more flexible use of strings
//
inline void NamedProperties::Set( const std::string& propertyName, const char* value )
{
	Set( propertyName, std::string( value ) );
}


//-----------------------------------------------------------------------------------------------
// This is an attempt to migrate properties between two NamedProperties objects, without knowing
// the data type.
//
inline bool NamedProperties::CopyPropertyByName( const std::string& propertyName, const NamedProperties& srcProperties )
{
	bool propertyMigrated = false ;

#if defined( USE_HASHED_PROPERTY_KEYS )
	CaseInsensitiveStringHashValue hashValue = HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( propertyName );
	TypedPropertyMap::const_iterator foundInSource = srcProperties.m_properties.find( hashValue );
#else
	HashedCaseInsensitiveString hashedPropertyName( propertyName );
	TypedPropertyMap::const_iterator foundInSource = srcProperties.m_properties.find( hashedPropertyName );
#endif

	if ( foundInSource != srcProperties.m_properties.end() )
	{
#if defined( USE_HASHED_PROPERTY_KEYS )
		TypedPropertyMap::iterator foundInDestination = m_properties.find( hashValue );
#else
		TypedPropertyMap::iterator foundInDestination = m_properties.find( hashedPropertyName );
#endif
		if( foundInDestination != m_properties.end() )
		{
			delete foundInDestination->second;
			foundInDestination->second = NULL;
		}

#if defined( USE_HASHED_PROPERTY_KEYS )
		m_properties[ hashValue ]	= foundInSource->second->Clone();
#else
		m_properties[ hashedPropertyName ]	= foundInSource->second->Clone();
#endif
		propertyMigrated					= true ;
	}

	return propertyMigrated ;
}


//-----------------------------------------------------------------------------------------------
template < typename T_PropertyType >
inline PropertyFetchResult NamedProperties::Get( const std::string& propertyName, T_PropertyType& outValue ) const
{
	if( m_properties.empty() )
	{
		return NAMED_PROPERTIES_WAS_EMPTY;
	}

#if defined( USE_HASHED_PROPERTY_KEYS )
	CaseInsensitiveStringHashValue hashValue = HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashValue );
#else
	HashedCaseInsensitiveString hashedPropertyName( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashedPropertyName );
#endif
	if( found == m_properties.end() )
	{
		return PROPERTY_NOT_FOUND;
	}

	TypedProperty< T_PropertyType >* propertyPtr = dynamic_cast< TypedProperty< T_PropertyType >* >( found->second );
	if( propertyPtr == NULL )
	{
		return PROPERTY_WAS_MISMATCHED_TYPE;
	}

//	TypedProperty< T_PropertyType >* propertyPtr = (TypedProperty< T_PropertyType >*)( found->second );
	outValue = *(propertyPtr->m_value);
	return PROPERTY_FETCH_OK;
}

//-----------------------------------------------------------------------------------------------
template < typename T_PropertyType >
inline PropertyFetchResult NamedProperties::Get( CaseInsensitiveStringHashValue hashValue, T_PropertyType& outValue ) const
{
	if( m_properties.empty() )
	{
		return NAMED_PROPERTIES_WAS_EMPTY;
	}

#if defined( USE_HASHED_PROPERTY_KEYS )
	TypedPropertyMap::const_iterator found = m_properties.find( hashValue );
#else
//	HashedCaseInsensitiveString hashedPropertyName( propertyName );
//	TypedPropertyMap::const_iterator found = m_properties.find( hashValue );
#endif
	if( found != m_properties.end() )
	{
		TypedProperty< T_PropertyType >* propertyPtr = dynamic_cast< TypedProperty< T_PropertyType >* >( found->second );
		if( propertyPtr == NULL )
		{
			return PROPERTY_WAS_MISMATCHED_TYPE;
		}
		
//		TypedProperty< T_PropertyType >* propertyPtr = (TypedProperty< T_PropertyType >*)( found->second );
		outValue = *(propertyPtr->m_value);
		return PROPERTY_FETCH_OK;
	}
	return PROPERTY_NOT_FOUND;
}




//-----------------------------------------------------------------------------------------------
template < typename T_PropertyType >
inline PropertyFetchResult NamedProperties::GetPointer( const std::string& propertyName, T_PropertyType*& outValue )
{
	if( m_properties.empty() )
	{
		return NAMED_PROPERTIES_WAS_EMPTY;
	}

#if defined( USE_HASHED_PROPERTY_KEYS )
	CaseInsensitiveStringHashValue hashValue = HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashValue );
#else
	HashedCaseInsensitiveString hashedPropertyName( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashedPropertyName );
#endif
	if( found == m_properties.end() )
	{
		return PROPERTY_NOT_FOUND;
	}

	TypedProperty< T_PropertyType >* propertyPtr = dynamic_cast< TypedProperty< T_PropertyType >* >( found->second );
	if( propertyPtr == NULL )
	{
		return PROPERTY_WAS_MISMATCHED_TYPE;
	}

	outValue = propertyPtr->m_value;
	return PROPERTY_FETCH_OK;
}


//-----------------------------------------------------------------------------------------------
#if defined( USE_HASHED_PROPERTY_KEYS )
template < typename T_PropertyType >
inline PropertyFetchResult NamedProperties::GetPointer( CaseInsensitiveStringHashValue propertyNameHashValue, T_PropertyType*& outValue )
{
	if( m_properties.empty() )
	{
		return NAMED_PROPERTIES_WAS_EMPTY;
	}

	TypedPropertyMap::const_iterator found = m_properties.find( propertyNameHashValue );

	if( found == m_properties.end() )
	{
		return PROPERTY_NOT_FOUND;
	}

	TypedProperty< T_PropertyType >* propertyPtr = dynamic_cast< TypedProperty< T_PropertyType >* >( found->second );
	if( propertyPtr == NULL )
	{
		return PROPERTY_WAS_MISMATCHED_TYPE;
	}

	outValue = propertyPtr->m_value;
	return PROPERTY_FETCH_OK;
}
#endif


//-----------------------------------------------------------------------------------------------
inline PropertyFetchResult NamedProperties::GetPropertyValueAsString( const std::string& propertyName, std::string& outValueString )
{
	if( m_properties.empty() )
	{
		return NAMED_PROPERTIES_WAS_EMPTY;
	}

#if defined( USE_HASHED_PROPERTY_KEYS )
	CaseInsensitiveStringHashValue hashValue = HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashValue );
#else
	HashedCaseInsensitiveString hashedPropertyName( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashedPropertyName );
#endif
	if( found == m_properties.end() )
	{
		return PROPERTY_NOT_FOUND;
	}

	outValueString = found->second->GetValueAsString();
	return PROPERTY_FETCH_OK;
}


//-----------------------------------------------------------------------------------------------
inline PropertyFetchResult NamedProperties::GetPropertyTypeAsString( const std::string& propertyName, std::string& outTypeString )
{
	if( m_properties.empty() )
	{
		return NAMED_PROPERTIES_WAS_EMPTY;
	}

#if defined( USE_HASHED_PROPERTY_KEYS )
	CaseInsensitiveStringHashValue hashValue = HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashValue );
#else
	HashedCaseInsensitiveString hashedPropertyName( propertyName );
	TypedPropertyMap::const_iterator found = m_properties.find( hashedPropertyName );
#endif
	if( found == m_properties.end() )
	{
		return PROPERTY_NOT_FOUND;
	}

	outTypeString = found->second->GetTypeAsString();
	return PROPERTY_FETCH_OK;
}


//-----------------------------------------------------------------------------------------------
inline bool NamedProperties::Remove( const std::string& propertyName )
{
	if( m_properties.empty() )
		return false;

#if defined( USE_HASHED_PROPERTY_KEYS )
	CaseInsensitiveStringHashValue hashValue = HashedCaseInsensitiveString::ComputeHashForCaseInsensitiveString( propertyName );
	TypedPropertyMap::iterator found = m_properties.find( hashValue );
#else
	HashedCaseInsensitiveString hashedPropertyName( propertyName );
	TypedPropertyMap::iterator found = m_properties.find( hashedPropertyName );
#endif

	if( found == m_properties.end() )
		return false;

	TypedPropertyBase* value = found->second;
	delete value;
	m_properties.erase( found );
	return true;
}


#endif // __include_NamedProperties__
