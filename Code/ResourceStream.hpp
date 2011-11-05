//-----------------------------------------------------------------------------------------------
// ResourceStream.hpp
//
// Copyright 2009 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_ResourceStream__
#define __include_ResourceStream__
#pragma once

//-----------------------------------------------------------------------------------------------
// Forward class declarations
//
class ResourceStream;

//#include "JazzBasicTypes.hpp"
//#include "JazzPath.hpp"
#include "Utilities.hpp"
#include "IntVector2.hpp"
#include "Vector2.hpp"
#include "AABB2.hpp"
#include "Rgba.hpp"


extern const int MINIMUM_ALLOC_CHUNK_SIZE_BYTES;


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ResourceStreamFormat
//
/////////////////////////////////////////////////////////////////////////////////////////////////
enum ResourceStreamFormat
{
	RESOURCE_STREAM_FORMAT_INVALID = -1,
	RESOURCE_STREAM_FORMAT_BINARY,
	RESOURCE_STREAM_FORMAT_TEXT_TERSE,
	RESOURCE_STREAM_FORMAT_TEXT_VERBOSE,
};


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ResourceStreamIntegrity
//
/////////////////////////////////////////////////////////////////////////////////////////////////
enum ResourceStreamIntegrity
{
	RESOURCE_STREAM_INTEGRITY_VALID = 53,
	RESOURCE_STREAM_INTEGRITY_CORRUPT_INSUFFICIENT_BYTES,
	RESOURCE_STREAM_INTEGRITY_CORRUPT_OUT_OF_RANGE,
};


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	ResourceStream
//
/////////////////////////////////////////////////////////////////////////////////////////////////
class ResourceStream
{
public:
	~ResourceStream();
	ResourceStream( ResourceStreamFormat formatType, int numInitialBytesToReserve = 1024 );
	ResourceStream( const std::string& fileToOpenForReading, bool failSilently = false );
	void			Initialize( ResourceStreamFormat formatType, int numInitialBytesToReserve = 1024 );
	bool			LoadFromFile( const JazzPath& fileToOpenForReading );
	bool			WriteToFile( const JazzPath& fileToOpenForWriting );
	void			ReserveAtLeast( int newMinimumCapacity );
	void			PrepareForWrite( int numBytesToWrite );
	bool			IsValid() const;
	bool			IsAtEOF() const;
	bool			IsCorrupt() const;
	ResourceStreamIntegrity CheckIntegrity() const;
	void			ResetIntegrity( ResourceStreamIntegrity newIntegrity = RESOURCE_STREAM_INTEGRITY_VALID );
	int				DataBytesRemaining() const;
	void			Rewind();
	const JazzPath	GetJazzPath() const { return m_jazzPath; }
	int				GetBufferWriteSizeSoFar() const { return m_dataBytesAvailable; }
	const unsigned char*	GetDirectBufferAccess() const { return m_startOfData; }

	std::string		GetReadPositionContextIfInTextMode();
	template< typename T_DestinationType > void ReadType( T_DestinationType& destinationObject );
	int				ReadByteBlock( void* destination, int numBytesExpected = -1 );
	bool			ReadBool();
	int				ReadInt();
	short			ReadShort();
	char			ReadChar();
	unsigned int			ReadDword();
	unsigned short			ReadWord();
	unsigned char			ReadByte();
	float			ReadFloat();
	double			ReadDouble();
	std::string		ReadString();
	Rgba			ReadRgba();
	IntVector2		ReadIntVector2();
	Vector2			ReadVector2();
	AABB2			ReadAABB2();
	void			ReadBlockBegin();
	void			ReadBlockEnd();
	void			ReadNewLine();
	void			ReadExpectedToken( char tokenCharacter );
	void			ReadExpectedToken( const std::string& tokenString );
	bool			CheckExpectedToken( char tokenCharacter );
	bool			CheckExpectedToken( const std::string& tokenString );

	template< typename T_SourceType > void WriteType( T_SourceType& sourceObject, const char* optionalLabel = NULL );
	void			WriteByteBlock( const void* source, int numBytesToWrite, const char* optionalLabel = NULL );
	void			WriteBool( bool data, const char* optionalLabel = NULL );
	void			WriteInt( int data, const char* optionalLabel = NULL );
	void			WriteShort( short data, const char* optionalLabel = NULL );
	void			WriteChar( char data, const char* optionalLabel = NULL );
	void			WriteDword( unsigned int data, const char* optionalLabel = NULL );
	void			WriteWord( unsigned short data, const char* optionalLabel = NULL );
	void			WriteByte( unsigned char data, const char* optionalLabel = NULL );
	void			WriteFloat( float data, const char* optionalLabel = NULL );
	void			WriteDouble( double data, const char* optionalLabel = NULL );
	void			WriteString( const std::string& data, const char* optionalLabel = NULL );
	void			WriteRgba( const Rgba& data, const char* optionalLabel = NULL );
	void			WriteIntVector2( const IntVector2& data, const char* optionalLabel = NULL );
	void			WriteVector2( const Vector2& data, const char* optionalLabel = NULL );
	void			WriteAABB2( const AABB2& data, const char* optionalLabel = NULL );
	void			WriteBlockBegin( const char* optionalLabel = NULL );
	void			WriteBlockEnd();
	void			WriteNewLine( const char* optionalLabel = NULL );

	int				FindByteSequenceOffset( int startSearchAtOffset, const unsigned char* byteSequence, int byteSequenceLength );
	int				CalcLineCount() const;
	std::string		FetchLine( const int lineNumberForRequestedLine ) const;

protected:
	bool			IsAtLineStart() const { return m_consecutiveNewLineCount > 0; }
	void			SetFormatTypeBasedOnBufferContents();
	void			ReadPastWhitespaceAndComments();
	void			ReadNumericCharacterString( char* destination, int destinationSize );
	double			ReadTextNumber();
	char			ReadCharDirectly();
	void			ReadBytesDirectly( void* destination, int numBytesToRead );
	char			PeekCharDirectly( int offsetFromCurrentReadPos );
	bool			PeekBytesDirectly( int offsetFromCurrentReadPos, void* destination, int numBytesToRead );
	void			WriteBytesDirectly( const void* source, int numBytesToWrite );
	void			WriteStringDirectly( const std::string& data );
	void			WriteStringIndentedIfNeeded( const std::string& data );
	void			WriteLineEndIfVerbose( const char* optionalLabel = NULL );
	template< typename T_SourceType > void WriteBinaryType( T_SourceType& sourceObject );

private:
	JazzPath				m_jazzPath;
	unsigned char*					m_startOfData;
	int						m_currentReadOffset;
	int						m_dataBytesAvailable;
	int						m_capacity;
	ResourceStreamFormat	m_internalFormat;
	int						m_currentIndentationDepth;
	int						m_consecutiveNewLineCount;
	bool					m_justWroteBlockBegin;
	bool					m_justWroteBlockEnd;
	ResourceStreamIntegrity	m_integrity;
};


//-----------------------------------------------------------------------------------------------
inline bool ResourceStream::IsValid() const
{
	return( m_startOfData != NULL );
}


//-----------------------------------------------------------------------------------------------
inline int ResourceStream::DataBytesRemaining() const
{
	return( m_dataBytesAvailable - m_currentReadOffset );
}


//-----------------------------------------------------------------------------------------------
inline bool ResourceStream::IsAtEOF() const
{
	return( DataBytesRemaining() <= 0 );
}


//-----------------------------------------------------------------------------------------------
inline bool ResourceStream::IsCorrupt() const
{
	return m_integrity != RESOURCE_STREAM_INTEGRITY_VALID;
}


//-----------------------------------------------------------------------------------------------
inline ResourceStreamIntegrity ResourceStream::CheckIntegrity() const
{
	return m_integrity;
}


//-----------------------------------------------------------------------------------------------
inline void ResourceStream::ResetIntegrity( ResourceStreamIntegrity newIntegrity )
{
	m_integrity = newIntegrity;
}



//-----------------------------------------------------------------------------------------------
template< typename T_DestinationType >
void ResourceStream::ReadType( T_DestinationType& destinationObject )
{
	const int size = sizeof( T_DestinationType );

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		ReadBytesDirectly( (unsigned char*) &destinationObject, size );
		return;
	}

	ReadExpectedToken( "Object" );
	ReadExpectedToken( '=' );
	ReadExpectedToken( '{' );
	unsigned char* asBytes = (unsigned char*) &destinationObject;
	for( int i = 0; i < size; ++ i )
	{
		if( i != 0 )
		{
			ReadExpectedToken( ',' );
		}

		asBytes[ i ] = ReadByte();
	}
	ReadExpectedToken( '}' );
}


//-----------------------------------------------------------------------------------------------
template< typename T_SourceType >
void ResourceStream::WriteType( T_SourceType& sourceObject, const char* optionalLabel )
{
	{ int q = 5; } //JAZZ_VALIDATE( m_internalFormat != RESOURCE_STREAM_FORMAT_INVALID , "ResourceStream::WriteInt", "Internal format of ResourceStream being written to was invalid!" );

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		WriteBinaryType( sourceObject );
		return;
	}

	std::string text;
	const int numBytesToWrite = sizeof( sourceObject );
	Sprintf( text, "Object={", numBytesToWrite );
	WriteStringDirectly( text );
	const unsigned char* bytes = (unsigned char*) &sourceObject;
	for( int i = 0; i < numBytesToWrite; ++ i )
	{
		if( i == 0 )
		{
			Sprintf( text, "%d", bytes[ i ] );
		}
		else
		{
			Sprintf( text, ",%d", bytes[ i ] );
		}
		WriteStringDirectly( text );
	}
	WriteStringDirectly( "} " );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
template< typename T_SourceType >
void ResourceStream::WriteBinaryType( T_SourceType& sourceObject )
{
	const int size = sizeof( T_SourceType );
	WriteBytesDirectly( &sourceObject, size );
}


#endif // __include_ResourceStream__
