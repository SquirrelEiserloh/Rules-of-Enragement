//-----------------------------------------------------------------------------------------------
// ResourceStream.cpp
//
// Copyright 2009 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "ResourceStream.hpp"

const int MINIMUM_ALLOC_CHUNK_SIZE_BYTES = (1 * 1024);
const unsigned int RESOURCE_STREAM_BINARY_HEADER = 0xff2a4201;
const char* INLINE_LABEL_FORMAT_STRING = "/*%s*/";
const char* ENDLINE_LABEL_FORMAT_STRING = "// %s";


//-----------------------------------------------------------------------------------------------
ResourceStream::~ResourceStream()
{
	if( m_startOfData )
	{
		delete[] m_startOfData;
	}
}


//-----------------------------------------------------------------------------------------------
ResourceStream::ResourceStream( ResourceStreamFormat formatType, int numInitialBytesToReserve )
: m_startOfData( NULL )
, m_currentReadOffset( 0 )
, m_dataBytesAvailable( 0 )
, m_capacity( 0 )
, m_internalFormat( RESOURCE_STREAM_FORMAT_INVALID )
, m_currentIndentationDepth( 0 )
, m_consecutiveNewLineCount( 1 )
, m_justWroteBlockBegin( false )
, m_justWroteBlockEnd( false )
, m_integrity( RESOURCE_STREAM_INTEGRITY_VALID )
{
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream is being constructed from formatType=%d, initialBytesReserved=%d\n", formatType, numInitialBytesToReserve );
	if( numInitialBytesToReserve > 0 )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "Initializing ResourceStream...\n" );
		Initialize( formatType, numInitialBytesToReserve );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "Finished initializing ResourceStream.  Internal format type=%d; bytesReserved=%d\n", m_internalFormat, m_capacity );
	}
}


//-----------------------------------------------------------------------------------------------
ResourceStream::ResourceStream( const std::string& fileToOpenForReading, bool failSilently )
: m_jazzPath( fileToOpenForReading )
, m_startOfData( NULL )
, m_currentReadOffset( 0 )
, m_dataBytesAvailable( 0 )
, m_capacity( 0 )
, m_internalFormat( RESOURCE_STREAM_FORMAT_INVALID )
, m_currentIndentationDepth( 0 )
, m_consecutiveNewLineCount( 1 )
, m_justWroteBlockBegin( false )
, m_justWroteBlockEnd( false )
, m_integrity( RESOURCE_STREAM_INTEGRITY_VALID )
{
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream is being constructed from JazzPath...\n" );
//	{ int q = 5; } //ConsolePrintf( "ResourceSteam being contructed from JazzPath, relative \"%s\" to marker #%d, resolved \"%s\".\n", fileToOpenForReading.GetRelativePathAsString().c_str(), fileToOpenForReading.GetFileFolderID(), fileToOpenForReading..c_str() );
	LoadFromFile( fileToOpenForReading );
//	{ int q = 5; } //ConsolePrintf( "Finished loading ResourceStream from file \"%s\"; resource stream contains %d bytes (capacity=%d).\n", m_jazzPath..c_str(), m_dataBytesAvailable, m_capacity );
	if( IsValid() )
	{
		{ int q = 5; } //ConsolePrintf( Rgba::GREY, "Loaded ResourceStream from file \"%s\"; resource stream contains %d bytes (capacity=%d).\n", m_jazzPath.c_str(), m_dataBytesAvailable, m_capacity );
	}
	else if( !failSilently )
	{
		{ int q = 5; } //ConsolePrintf( Rgba::RED, "FAILED TO LOAD ResourceStream from file \"%s\".\n", m_jazzPath..c_str() );
	}
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::Initialize( ResourceStreamFormat formatType, int numInitialBytesToReserve )
{
	m_internalFormat = formatType;
	m_currentReadOffset = 0;
	m_dataBytesAvailable = 0;
	m_capacity = numInitialBytesToReserve;
	if( m_startOfData )
	{
		{ int q = 5; } //ConsolePrintf( "ResourceStream::Initialize: m_startOfData pre-exists!  Calling delete[]...\n" );
		delete[] m_startOfData;
	}
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream::Initialize: About to allocate %d bytes...\n", numInitialBytesToReserve );
	m_startOfData = numInitialBytesToReserve > 0 ? new unsigned char[ numInitialBytesToReserve ] : NULL;

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream::Initialize: ResourceStream is BINARY, writing binary header dword.\n" );
		WriteDword( RESOURCE_STREAM_BINARY_HEADER );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream::Initialize: ResourceStream is BINARY, done writing binary header dword.\n" );
	}
}


//-----------------------------------------------------------------------------------------------
bool ResourceStream::LoadFromFile( const JazzPath& fileToOpenForReading )
{
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream::LoadFromFile: calling Initialize() with args RESOURCE_STREAM_FORMAT_INVALID, 0\n" );
	Initialize( RESOURCE_STREAM_FORMAT_INVALID, 0 );

	bool isText = true ;
/*
	std::string extension = fileToOpenForReading.GetFileExtension();
	if ( Stricmp( extension, "xml" )==0 )
		isText = true ;
	else if ( Stricmp( extension, "AnimTextureDefinition" )==0 )
		isText = true ;
	else if ( Stricmp( extension, "FontDef" )==0 )
		isText = true ;
	else if ( Stricmp( extension, "txt" )==0 )
		isText = true ;
	else if ( Stricmp( extension, "log" )==0 )
		isText = true ;
*/
	
	// FIXME: Add other known text file types here
	
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream::LoadFromFile: calling LoadBinaryFileToNewBuffer() with this JazzPath:\n" );
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "..which is relative \"%s\" to marker #%d, resolved \"%s\".\n", fileToOpenForReading.GetRelativePathAsString().c_str(), fileToOpenForReading.GetFileFolderID(), fileToOpenForReading..c_str() );
	m_dataBytesAvailable = LoadBinaryFileToNewBuffer( fileToOpenForReading, m_startOfData, isText );
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream::LoadFromFile: done calling LoadBinaryFileToNewBuffer(); %d bytes are available.\n", m_dataBytesAvailable );

	if( m_dataBytesAvailable < 0 )
	{
		Initialize( RESOURCE_STREAM_FORMAT_INVALID, 0 );
		return false;
	}

	m_jazzPath = fileToOpenForReading;
	m_capacity = m_dataBytesAvailable;
	SetFormatTypeBasedOnBufferContents();
	return true;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReserveAtLeast( int newMinimumCapacity )
{
	if( newMinimumCapacity <= m_capacity )
		return;

	{ int q = 5; } //ConsolePrintf( "ResourceStream::ReserveAtLeast() called; current capacity is %d, extending to at least %d\n", m_capacity, newMinimumCapacity );

	// Grow at least 50% bigger and by at least a minimum amount
	const int fiftyPercentIncreasedCapacity = (m_capacity * 3) / 2;
	const int minimumIncreasedCapacity = (newMinimumCapacity + MINIMUM_ALLOC_CHUNK_SIZE_BYTES);
	const int newCapacity = MaxInt( fiftyPercentIncreasedCapacity, minimumIncreasedCapacity );

	{ int q = 5; } //ConsolePrintf( "ResourceStream::ReserveAtLeast: new capacity will be expanded to %d bytes\n", newCapacity );

	unsigned char* oldData = m_startOfData;
	m_startOfData = new unsigned char[ newCapacity ];
	memcpy( m_startOfData, oldData, m_dataBytesAvailable );
	delete[] oldData;
	m_capacity = newCapacity;
	{ int q = 5; } //ConsolePrintf( "ResourceStream::ReserveAtLeast: finished relocating into newly expanded memory buffer.\n" );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::SetFormatTypeBasedOnBufferContents()
{
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ResourceStream::SetFormatTypeBasedOnBufferContents() called for RS \"%s\".\n", m_jazzPath..c_str() );

	//const int numBytesToPrint = MinInt( m_dataBytesAvailable, 12 );
	//{ int q = 5; } //ConsolePrintf( "RS appears to have %d bytes in it; the first %d are:\n", m_dataBytesAvailable, numBytesToPrint );
	//for( int i = 0; i < numBytesToPrint; ++ i )
	//{
	//	unsigned char thisByte = m_startOfData[ i ];
	//	{ int q = 5; } //ConsolePrintf( "0x%02x ", thisByte );
	//}
	//{ int q = 5; } //ConsolePrintf( "End of first-%d-bytes list.\n", numBytesToPrint );

	// Check if the file is at least 4 bytes in size (minimum size for binary format)
	if( m_dataBytesAvailable < 4 )
	{
		m_internalFormat = RESOURCE_STREAM_FORMAT_TEXT_VERBOSE;
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "RS is shorter than 4 bytes, assuming format TEXT (assuming TEXT_VERBOSE)\n" );
		return;
	}

	// Check if the Binary header/marker is present
	unsigned int headerMarker;
	memcpy( &headerMarker, m_startOfData, sizeof( headerMarker ) );
	if( headerMarker == RESOURCE_STREAM_BINARY_HEADER )
	{
		m_internalFormat = RESOURCE_STREAM_FORMAT_BINARY;
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "RS appears to be BINARY; about to read binary format primer dword\n" );
		ReadType( headerMarker );
		return;
	}

	m_internalFormat = RESOURCE_STREAM_FORMAT_TEXT_VERBOSE;
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "RS appears to be TEXT; assuming TEXT_VERBOSE\n" );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::Rewind()
{
	m_currentReadOffset = 0;
}


//-----------------------------------------------------------------------------------------------
std::string ResourceStream::GetReadPositionContextIfInTextMode()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
		return std::string( "(Context unavailable; resource stream was in binary format)" );

	int numCharsToReadBackward = 320;
	int numCharsToReadForward = 80;
	std::string currentLocationMarker( "[CURRENT READ LOCATION]" );

	if( numCharsToReadBackward > m_currentReadOffset )
		numCharsToReadBackward = m_currentReadOffset;

	if( numCharsToReadForward > DataBytesRemaining() )
		numCharsToReadForward = DataBytesRemaining();

	int startOffsetForReadBackward = m_currentReadOffset - numCharsToReadBackward;
	int startOffsetForReadForward = m_currentReadOffset;

	char* startOfCharacters = (char*) m_startOfData;
	std::string readBackward( &startOfCharacters[ startOffsetForReadBackward ], numCharsToReadBackward );
	std::string readForward( &startOfCharacters[ startOffsetForReadForward ], numCharsToReadForward );

	std::string fileName = m_jazzPath;
	std::string printFileName = fileName.length() == 0 ? "(unknown filename)" : fileName;

	std::string returnValue = Stringf( "Near line #%d of %s:\n\n%s%s%s\n", m_consecutiveNewLineCount, printFileName.c_str(), readBackward.c_str(), currentLocationMarker.c_str(), readForward.c_str() );
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadPastWhitespaceAndComments()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
		return;

	enum CommentParseState
	{
		CPS_NORMAL,
		CPS_COMMENT_SLASHSLASH,
		CPS_COMMENT_SLASHSTAR,
	};

	CommentParseState commentParseState = CPS_NORMAL;

	while( m_currentReadOffset < m_dataBytesAvailable )
	{
		char nextChar = m_startOfData[ m_currentReadOffset ];

		if( commentParseState == CPS_NORMAL )
		{
			if( isspace( nextChar ) )
			{
				m_currentReadOffset ++;
				continue;
			}

			if( nextChar == '/' )
			{
				{ int q = 5; } //JAZZ_VALIDATE( DataBytesRemaining() >= 2, "ResourceStream::ReadPastWhitespaceAndComments", "Found '/' character outside of comment right before EOF!" );
				m_currentReadOffset ++;
				nextChar = m_startOfData[ m_currentReadOffset ];
				if( nextChar == '/' )
				{
					commentParseState = CPS_COMMENT_SLASHSLASH;
					m_currentReadOffset ++;
					continue;
				}
				else if( nextChar == '*' )
				{
					commentParseState = CPS_COMMENT_SLASHSTAR;
					m_currentReadOffset ++;
					continue;
				}
				else
				{
					{ int q = 5; } //JAZZ_ERROR( "ResourceStream::ReadPastWhitespaceAndComments", "Found illegal character '%c' (0x%02x) after '/' character (must be either \"//\" or \"/*\")." );
				}
			}
			else // found non-slash, non-whitespace character in normal (non-comment) mode; we're finished scanning
			{
				break;
			}
		}
		else if( commentParseState == CPS_COMMENT_SLASHSLASH )
		{
			if( nextChar == '\r' || nextChar == '\n' )
			{
				commentParseState = CPS_NORMAL;
			}
			m_currentReadOffset ++;
		}
		else if( commentParseState == CPS_COMMENT_SLASHSTAR )
		{
			{ int q = 5; } //JAZZ_VALIDATE( DataBytesRemaining() >= 2, "ResourceStream::ReadPastWhitespaceAndComments", "Was inside /*comment*/ when reached EOF!" );
			const char lastChar = nextChar;
			nextChar = m_startOfData[ m_currentReadOffset ];
			if( lastChar == '*' && nextChar == '/' )
			{
				m_currentReadOffset ++; // put us past the comment-ending "*/"
				commentParseState = CPS_NORMAL;
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadNumericCharacterString( char* destination, int destinationSize )
{
	char* writePtr = destination;
	while( m_currentReadOffset < m_dataBytesAvailable )
	{
		if( writePtr - destination >= destinationSize-1 )
		{
			{ int q = 5; } //JAZZ_ERROR( "ResourceStream::ReadNumericCharacterString", Stringf( "Buffer size was insufficient at %d bytes!", destinationSize ) );
		}

		char nextChar = m_startOfData[ m_currentReadOffset ];
		if( nextChar == '-' && writePtr != destination )
		{
			break;
		}

		int lowerChar = tolower( nextChar );
		if( lowerChar == 'e' && writePtr == destination )
		{
			break;
		}

		if( nextChar == '.' || nextChar == '-' || lowerChar == 'e' || isdigit( nextChar ) )
		{
			*writePtr = nextChar;
			writePtr ++;
			m_currentReadOffset ++;
		}
		else
		{
			break;
		}
	}

	*writePtr = '\0';
	return;
}


//-----------------------------------------------------------------------------------------------
double ResourceStream::ReadTextNumber()
{
	{ int q = 5; } //JAZZ_VALIDATE( m_internalFormat != RESOURCE_STREAM_FORMAT_BINARY, "ResourceStream::ReadTextNumber", "Method was called on ResourceStream in Binary mode (supported in Text mode(s) only!)" );
	ReadPastWhitespaceAndComments();
	char asText[ 1024 ];
	ReadNumericCharacterString( asText, sizeof( asText ) );
	return atof( asText );
}


//-----------------------------------------------------------------------------------------------
char ResourceStream::ReadCharDirectly()
{
	char returnValue;
	ReadBytesDirectly( &returnValue, 1 );
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
char ResourceStream::PeekCharDirectly( int offsetFromCurrentReadPos )
{
	char returnValue = '\0';
	PeekBytesDirectly( offsetFromCurrentReadPos, &returnValue, 1 );
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadBytesDirectly( void* destination, int numBytesToRead )
{
	{ int q = 5; } //JAZZ_VALIDATE( IsValid(), "ResourceStream::ReadBytesDirectly", Stringf( "ResourceStream is invalid; path was '%s'", GetJazzPath().GetRelativePathAsString().c_str() ) );
//	{ int q = 5; } //JAZZ_VALIDATE( DataBytesRemaining() >= numBytesToRead, "ResourceStream::ReadBytesDirectly", Stringf( "Tried to read %d bytes; only %d remain in buffer!", numBytesToRead, DataBytesRemaining() ) );

///////////////////////////////////////////
static int numCallsToRBD = 0;
++ numCallsToRBD;
///////////////////////////////////////////

	if( m_integrity != RESOURCE_STREAM_INTEGRITY_VALID )
		return;

	if( DataBytesRemaining() < numBytesToRead )
	{
		m_integrity = RESOURCE_STREAM_INTEGRITY_CORRUPT_INSUFFICIENT_BYTES;
		return;
	}

	const unsigned char* readPosition = m_startOfData + m_currentReadOffset;
	memcpy( destination, readPosition, numBytesToRead );
	m_currentReadOffset += numBytesToRead;
}


//-----------------------------------------------------------------------------------------------
bool ResourceStream::PeekBytesDirectly( int offsetFromCurrentReadPos, void* destination, int numBytesToRead )
{
	if( DataBytesRemaining() < (numBytesToRead + offsetFromCurrentReadPos) )
		return false;

	const unsigned char* readPosition = m_startOfData + m_currentReadOffset + offsetFromCurrentReadPos;
	memcpy( destination, readPosition, numBytesToRead );
	return true;
}


//-----------------------------------------------------------------------------------------------
int ResourceStream::ReadByteBlock( void* destination, int numBytesExpected )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		const int byteBlockSize = ReadInt();
		if( numBytesExpected >= 0 )
		{
			{ int q = 5; } //JAZZ_VALIDATE( byteBlockSize == numBytesExpected, "ResourceStream::ReadByteBlock", Stringf( "Expected %d bytes; block in stream was %d bytes.", numBytesExpected, byteBlockSize ) );
		}
		ReadBytesDirectly( destination, byteBlockSize );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadByteBlock: read %d bytes (was expecting %d bytes) at offset %d\n", byteBlockSize, numBytesExpected, m_currentReadOffset );
		return byteBlockSize;
	}

	ReadExpectedToken( "Bytes" );
	ReadExpectedToken( '[' );

	int byteBlockSize = ReadInt();
	if( numBytesExpected >= 0 )
	{
		{ int q = 5; } //JAZZ_VALIDATE( byteBlockSize == numBytesExpected, "ResourceStream::ReadByteBlock", Stringf( "Expected %d bytes; block in stream was %d bytes.", numBytesExpected, byteBlockSize ) );
	}

	ReadExpectedToken( ']' );
	ReadExpectedToken( '=' );
	ReadExpectedToken( '{' );

	unsigned char* asBytes = (unsigned char*) destination;

	for( int i = 0; i < byteBlockSize; ++ i )
	{
		if( i != 0 )
		{
			ReadExpectedToken( ',' );
		}

		asBytes[ i ] = ReadByte();
	}
	ReadExpectedToken( '}' );
	return byteBlockSize;
}


//-----------------------------------------------------------------------------------------------
bool ResourceStream::ReadBool()
{
	ReadPastWhitespaceAndComments();
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		unsigned char asByte;
		ReadType( asByte );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadBool at offset %d, value was %d\n", m_currentReadOffset, asByte );
		return( asByte != 0 );
	}

	char nextChar = ReadCharDirectly();
	if( nextChar == 'f' )
	{
		ReadExpectedToken( "alse" );
		return false;
	}
	else if( nextChar == 't' )
	{
		ReadExpectedToken( "rue" );
		return true;
	}
	else
	{
		{ int q = 5; } //JAZZ_ERROR( "ResourceStream::ReadBool", Stringf( "Expected either \"true\" or \"false\" (lower case), found illegal character '%c'.", nextChar ) );
	}
}


//-----------------------------------------------------------------------------------------------
int ResourceStream::ReadInt()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		int returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadInt at offset %d, value was %d\n", m_currentReadOffset, returnValue );
		return returnValue;
	}

	double asDouble = ReadTextNumber();
	return (int) asDouble;
}


//-----------------------------------------------------------------------------------------------
unsigned int ResourceStream::ReadDword()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		unsigned int returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadDword at offset %d, value was 0x%08x\n", m_currentReadOffset, returnValue );
		return returnValue;
	}

	double asDouble = ReadTextNumber();
	return (unsigned int) asDouble;
}


//-----------------------------------------------------------------------------------------------
short ResourceStream::ReadShort()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		short returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadShort at offset %d, value was %d\n", m_currentReadOffset, returnValue );
		return returnValue;
	}

	double asDouble = ReadTextNumber();
	return (short) asDouble;
}


//-----------------------------------------------------------------------------------------------
unsigned short ResourceStream::ReadWord()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		unsigned short returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadWord at offset %d, value was 0x%04x\n", m_currentReadOffset, returnValue );
		return returnValue;
	}

	double asDouble = ReadTextNumber();
	return (unsigned short) asDouble;
}


//-----------------------------------------------------------------------------------------------
char ResourceStream::ReadChar()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		char returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadChar at offset %d, value was %d, or '%c' (0x%02x)\n", m_currentReadOffset, returnValue, returnValue, returnValue );
		return returnValue;
	}

	// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadChar() called...\n" );
	double asDouble = ReadTextNumber();
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "Read text number as double %.15Lf (%g), converting to char 0x%02x\n", asDouble, asDouble, (char) asDouble );
	return (char) asDouble;
}


//-----------------------------------------------------------------------------------------------
unsigned char ResourceStream::ReadByte()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		unsigned char returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadByte at offset %d, value was 0x%02x\n", m_currentReadOffset, returnValue );
		return returnValue;
	}

	double asDouble = ReadTextNumber();
	return (unsigned char) asDouble;
}


//-----------------------------------------------------------------------------------------------
float ResourceStream::ReadFloat()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		float returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadFloat at offset %d, value was %g\n", m_currentReadOffset, returnValue );
		return returnValue;
	}

	double asDouble = ReadTextNumber();
	return (float) asDouble;
}


//-----------------------------------------------------------------------------------------------
double ResourceStream::ReadDouble()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		double returnValue;
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadFloat at offset %d, value was %.15Lf\n", m_currentReadOffset, returnValue );
		return returnValue;
	}

	double asDouble = ReadTextNumber();
	return asDouble;
}


//-----------------------------------------------------------------------------------------------
std::string ResourceStream::ReadString()
{
	const int maxCharsToRead = 32768;

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		const int stringLength = ReadInt();
//		{ int q = 5; } //JAZZ_VALIDATE( stringLength >= 0 && stringLength < maxCharsToRead, "ResourceStream::ReadString()", Stringf( "Got an absurd string-length to read (%d) in ResourceStream \"%s\".", stringLength, m_jazzPath..c_str() ) );
		if( stringLength < 0 || stringLength >= maxCharsToRead )
		{
			m_integrity = RESOURCE_STREAM_INTEGRITY_CORRUPT_OUT_OF_RANGE;
			return "";
		}

		const unsigned char* readPosition = m_startOfData + m_currentReadOffset;
		{ int q = 5; } //JAZZ_VALIDATE( stringLength <= DataBytesRemaining(), "ResourceStream::ReadString()", Stringf( "Trying to read %d chars into a string, but only %d bytes remain in RS buffer!", stringLength, DataBytesRemaining() ) );
		const char* characterArrayStart = (const char*)( readPosition );
		std::string returnValue( characterArrayStart, stringLength );
		m_currentReadOffset += stringLength;
		return returnValue;
	}

	ReadExpectedToken( '\"' );
	char nextChar = ReadCharDirectly();
	char destinationBuffer[ maxCharsToRead ];
	char* writePtr = destinationBuffer;
	while( nextChar != '\"' )
	{
		{ int q = 5; } //JAZZ_VALIDATE( (writePtr - destinationBuffer) < maxCharsToRead, "ResourceStream::ReadString()", Stringf( "Exceeded limit of %d bytes to read for text-mode string!", maxCharsToRead ) );

		*writePtr = nextChar;
		writePtr ++;
		nextChar = ReadCharDirectly();
	}

	*writePtr = '\0';
	return std::string( destinationBuffer );
}


//-----------------------------------------------------------------------------------------------
Rgba ResourceStream::ReadRgba()
{
	Rgba returnValue;

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadRgba at offset %d, value was (%d,%d,%d,%d)\n", m_currentReadOffset, returnValue.r, returnValue.g, returnValue.b, returnValue.a );
		return returnValue;
	}

	ReadExpectedToken( "Rgba" );
	ReadExpectedToken( '(' );
	returnValue.r = ReadByte();
	ReadExpectedToken( ',' );
	returnValue.g = ReadByte();
	ReadExpectedToken( ',' );
	returnValue.b = ReadByte();
	ReadExpectedToken( ',' );
	returnValue.a = ReadByte();
	ReadExpectedToken( ')' );

	return returnValue;
}


//-----------------------------------------------------------------------------------------------
IntVector2 ResourceStream::ReadIntVector2()
{
	IntVector2 returnValue;

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadIntVector2 at offset %d, value was (%d,%d)\n", m_currentReadOffset, returnValue.x, returnValue.y );
		return returnValue;
	}

	ReadExpectedToken( "IntVector2" );
	ReadExpectedToken( '(' );
	returnValue.x = ReadInt();
	ReadExpectedToken( ',' );
	returnValue.y = ReadInt();
	ReadExpectedToken( ')' );

	return returnValue;
}


//-----------------------------------------------------------------------------------------------
Vector2 ResourceStream::ReadVector2()
{
	Vector2 returnValue;

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadVector2 at offset %d, value was (%g,%g)\n", m_currentReadOffset, returnValue.x, returnValue.y );
		return returnValue;
	}

	ReadExpectedToken( "Vector2" );
	ReadExpectedToken( '(' );
	returnValue.x = ReadFloat();
	ReadExpectedToken( ',' );
	returnValue.y = ReadFloat();
	ReadExpectedToken( ')' );

	return returnValue;
}


//-----------------------------------------------------------------------------------------------
AABB2 ResourceStream::ReadAABB2()
{
	AABB2 returnValue;

	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		ReadType( returnValue );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "ReadAABB2 at offset %d, value was (%g,%g),(%g,%g)\n", m_currentReadOffset, returnValue.mins.x, returnValue.mins.y, returnValue.maxs.x, returnValue.maxs.y );
		return returnValue;
	}

	ReadExpectedToken( "AABB2" );
	ReadExpectedToken( '[' );
	ReadExpectedToken( '(' );
	returnValue.mins.x = ReadFloat();
	ReadExpectedToken( ',' );
	returnValue.mins.y = ReadFloat();
	ReadExpectedToken( ')' );
	ReadExpectedToken( ',' );
	ReadExpectedToken( '(' );
	returnValue.maxs.x = ReadFloat();
	ReadExpectedToken( ',' );
	returnValue.maxs.y = ReadFloat();
	ReadExpectedToken( ')' );
	ReadExpectedToken( ']' );

	return returnValue;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadBlockBegin()
{
	if( m_internalFormat != RESOURCE_STREAM_FORMAT_BINARY )
	{
		ReadExpectedToken( '{' );
	}
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadBlockEnd()
{
	if( m_internalFormat != RESOURCE_STREAM_FORMAT_BINARY )
	{
		ReadExpectedToken( '}' );
	}
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadNewLine()
{
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadExpectedToken( char tokenCharacter )
{
	ReadPastWhitespaceAndComments();
	char readChar = ReadCharDirectly();
	
	if( readChar != tokenCharacter )
	{
		std::string fileName = m_jazzPath;
		std::string context = GetReadPositionContextIfInTextMode();
		{ int q = 5; } //JAZZ_ERROR( "ResourceStream::ReadExpectedToken", Stringf( "In file %s,\n Expected token '%c'; found token '%c'.  Context was:\n\n%s", fileName.length() == 0 ? "unknown" : fileName.c_str(), tokenCharacter, readChar, context.c_str() ) );
	}
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::ReadExpectedToken( const std::string& tokenString )
{
	ReadPastWhitespaceAndComments();
	const int tokenLength = (int)tokenString.size();
	const char* cString = tokenString.c_str();
	for( int i = 0; i < tokenLength; ++ i )
	{
		char readChar = ReadCharDirectly();
		if( readChar != cString[ i ] )
		{
			std::string fileName = m_jazzPath;
			std::string context = GetReadPositionContextIfInTextMode();
			{ int q = 5; } //JAZZ_ERROR( "ResourceStream::ReadExpectedToken", Stringf( "In file %s,\n Expected token-string \"%s\"; found token '%c' when expecting '%c'.  Context was:\n\n%s", fileName.length() == 0 ? "unknown" : fileName.c_str(), cString, readChar, cString[ i ], context.c_str() ) );
		}

	}
}


//-----------------------------------------------------------------------------------------------
bool ResourceStream::CheckExpectedToken( char tokenCharacter )
{
	return CheckExpectedToken( Stringf( "%c", tokenCharacter ) );
}


//-----------------------------------------------------------------------------------------------
bool ResourceStream::CheckExpectedToken( const std::string& tokenString )
{
	ReadPastWhitespaceAndComments();
	const int tokenLength = (int)tokenString.size();
	const char* cString = tokenString.c_str();
	for( int i = 0; i < tokenLength; ++ i )
	{
		char readChar = PeekCharDirectly( i );
		if( readChar != cString[ i ] )
			return false;
	}

	return true;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::PrepareForWrite( int numBytesToWrite )
{
	{ int q = 5; } //JAZZ_VALIDATE( m_internalFormat != RESOURCE_STREAM_FORMAT_INVALID, "ResourceStream::PrepareForWrite", "Internal format has not yet been set!" );
	{ int q = 5; } //JAZZ_VALIDATE( m_startOfData != NULL, "ResourceStream::PrepareForWrite", "No data buffer has yet been allocated to write into!" );

//	const int roomForWriting = m_capacity - m_dataBytesAvailable;
//	if( roomForWriting < numBytesToWrite )
	{
		ReserveAtLeast( m_dataBytesAvailable + numBytesToWrite );
	}
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteBytesDirectly( const void* source, int numBytesToWrite )
{
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteBytesDirectly: Writing %d bytes at offset %d\n", numBytesToWrite, m_dataBytesAvailable );
	PrepareForWrite( numBytesToWrite );
	unsigned char* writePosition = m_startOfData + m_dataBytesAvailable;
	memcpy( writePosition, source, numBytesToWrite );
	m_dataBytesAvailable += numBytesToWrite;
	m_consecutiveNewLineCount = 0;
	m_justWroteBlockBegin = false;
	m_justWroteBlockEnd = false;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteStringDirectly( const std::string& data )
{
	WriteBytesDirectly( data.c_str(), (int)data.size() );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteStringIndentedIfNeeded( const std::string& data )
{
	if( m_justWroteBlockEnd )
	{
		WriteNewLine();
	}

	if( IsAtLineStart() )
	{
		for( int i = 0; i < m_currentIndentationDepth; ++ i )
		{
			WriteStringDirectly( "\t" );
		}
	}

	WriteStringDirectly( data );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteByteBlock( const void* source, int numBytesToWrite, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteByteBlock: Writing %d bytes at offset %d (\"%s\")\n", numBytesToWrite, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteInt( numBytesToWrite );
		WriteBytesDirectly( source, numBytesToWrite );
		return;
	}

	std::string text;
	Sprintf( text, "Bytes[%d]={", numBytesToWrite );
	WriteStringIndentedIfNeeded( text );
	const unsigned char* bytes = (unsigned char*) source;
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
void ResourceStream::WriteBool( bool data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteBool: Writing bool, value=%d, at offset %d (\"%s\")\n", (int) data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteByte( (unsigned char) data, optionalLabel );
		return;
	}

	std::string asText;
	Sprintf( asText, "%s ", data ? "true" : "false" );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteInt( int data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteInt: value=%d, at offset %d (\"%s\")\n", data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%d ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteDword( unsigned int data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteDword: value=0x%08x, at offset %d (\"%s\")\n", data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%u ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteShort( short data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteShort: value=%d, at offset %d (\"%s\")\n", data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%d ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteWord( unsigned short data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteWord: value=0x%04x, at offset %d (\"%s\")\n", data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%u ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteChar( char data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteChar: value=%d (or '%c' or 0x%02x), at offset %d (\"%s\")\n", data, data, data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%d ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteByte( unsigned char data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteWord: value=0x%02x, at offset %d (\"%s\")\n", data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%d ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteFloat( float data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteFloat: value=%g, at offset %d (\"%s\")\n", data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%g ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteDouble( double data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteDouble: value=%.15Lf, at offset %d (\"%s\")\n", data, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "%.15Lf ", data );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteString( const std::string& data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		int numChars = (int) data.length();
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteString: value=\"%s\" (%d chars), at offset %d (\"%s\")\n", data.c_str(), numChars, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteInt( numChars );
		WriteStringDirectly( data );
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteString: finished writing binary string.\n" );
		return;
	}

	std::string asText;
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteString: about to write text-mode string, \"%s\" (label=\"%s\").\n", data.c_str(), optionalLabel ? optionalLabel : "" );
	Sprintf( asText, "\"%s\" ", data.c_str() );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
	// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteString: wrote text-mode string, %s.\n", asText.c_str() );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteRgba( const Rgba& data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteRgba: value=(%d,%d,%d,%d), at offset %d (\"%s\")\n", data.r, data.g, data.b, data.a, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "Rgba(%d,%d,%d,%d) ", data.r, data.g, data.b, data.a );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteIntVector2( const IntVector2& data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteIntVector2: value=(%d,%d), at offset %d (\"%s\")\n", data.x, data.y, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "IntVector2(%d,%d) ", data.x, data.y );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteVector2( const Vector2& data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteVector2: value=(%g,%g), at offset %d (\"%s\")\n", data.x, data.y, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "Vector2(%g,%g) ", data.x, data.y );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteAABB2( const AABB2& data, const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
	{
		// DEBUGGING - { int q = 5; } //ConsolePrintf( "WriteAABB2: value=(%g,%g),(g,%g), at offset %d (\"%s\")\n", data.mins.x, data.mins.y, data.maxs.x, data.maxs.y, m_dataBytesAvailable, optionalLabel ? optionalLabel : "" );
		WriteBinaryType( data );
		return;
	}

	std::string asText;
	Sprintf( asText, "AABB2[(%g,%g),(%g,%g)] ", data.mins.x, data.mins.y, data.maxs.x, data.maxs.y );
	WriteStringIndentedIfNeeded( asText );
	WriteLineEndIfVerbose( optionalLabel );
}


//-----------------------------------------------------------------------------------------------
bool ResourceStream::WriteToFile( const JazzPath& fileToOpenForWriting )
{
	{ int q = 5; } //ConsolePrintf( "Writing resource stream (%d bytes) to file \"%s\"...\n", m_dataBytesAvailable, fileToOpenForWriting..c_str() );
	int bytesWritten = WriteBufferToBinaryFile( fileToOpenForWriting, m_startOfData, m_dataBytesAvailable );
	const bool success = (bytesWritten == m_dataBytesAvailable);
	if( !success )
	{
		{ int q = 5; } //ConsolePrintf( "ERROR writing resource stream to file \"%s\"; wrote %d bytes, should have been %d!\n", fileToOpenForWriting..c_str(), bytesWritten, m_dataBytesAvailable );
	}

	return success;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteBlockBegin( const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
		return; // absent in compact binary format

	if( !m_justWroteBlockBegin )
	{
		if( m_consecutiveNewLineCount == 0 )
		{
			WriteNewLine(); // end the previous line (if we were in the middle of one)
		}

		if( m_consecutiveNewLineCount <= 1 )
		{
			WriteNewLine(); // make sure there's a blank line before each block
		}
	}

	if( optionalLabel )
	{
		WriteStringIndentedIfNeeded( Stringf( ENDLINE_LABEL_FORMAT_STRING, optionalLabel ) );
		WriteNewLine();
	}

	WriteStringIndentedIfNeeded( "{" );

	m_currentIndentationDepth ++;
	WriteNewLine();
	m_justWroteBlockBegin = true;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteBlockEnd()
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
		return; // absent in compact binary format

	m_currentIndentationDepth --;

	if( m_consecutiveNewLineCount == 0 )
	{
		WriteNewLine(); // end the previous line (if we were in the middle of one)
	}

	m_justWroteBlockEnd = false;
	WriteStringIndentedIfNeeded( "}" );
	WriteNewLine(); // end the closing-brace line
	m_justWroteBlockBegin = false;
	m_justWroteBlockEnd = true;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteNewLine( const char* optionalLabel )
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY )
		return; // absent in compact binary format

	if( optionalLabel && m_internalFormat == RESOURCE_STREAM_FORMAT_TEXT_TERSE )
//	if( optionalLabel )
	{
		WriteStringIndentedIfNeeded( Stringf( ENDLINE_LABEL_FORMAT_STRING, optionalLabel ) );
	}

	WriteStringDirectly( "\r\n" );
	m_consecutiveNewLineCount ++;
}


//-----------------------------------------------------------------------------------------------
void ResourceStream::WriteLineEndIfVerbose( const char* optionalLabel )
{
	if( m_internalFormat != RESOURCE_STREAM_FORMAT_TEXT_VERBOSE )
		return;

	if( optionalLabel )
	{
		WriteStringDirectly( Stringf( ENDLINE_LABEL_FORMAT_STRING, optionalLabel ) );
	}

	WriteNewLine();
}


//-----------------------------------------------------------------------------------------------
// Returns the first offset in the stream where "byteSequence" occurs, starting at an initial
// offset of "startSearchAtOffset" and searching forward from there.
//
// Returns -1 if the byte sequence was not found anywhere in the stream at or
// after "startSearchAtOffset".
//-----------------------------------------------------------------------------------------------
int ResourceStream::FindByteSequenceOffset( int startSearchAtOffset, const unsigned char* byteSequence, int byteSequenceLength )
{
	for( int searchOffset = startSearchAtOffset; searchOffset + byteSequenceLength <= m_dataBytesAvailable; ++ searchOffset )
	{
		// See if the byte sequence is present at this exact offset
		unsigned char* comparePoint = &m_startOfData[ byteSequenceLength ];
		if( !memcmp( byteSequence, comparePoint, byteSequenceLength ) )
			return searchOffset;
	}

	return -1;
}


//-----------------------------------------------------------------------------------------------
int ResourceStream::CalcLineCount() const
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY || m_internalFormat == RESOURCE_STREAM_FORMAT_INVALID || !IsValid() || IsCorrupt() || m_dataBytesAvailable == 0 )
		return 0;

	int lineCount = 1;
	int charIndex = 0;
	const char* scan = (const char*) m_startOfData;
	for( ;; )
	{
		if( charIndex >= m_dataBytesAvailable )
			break;

		if( *scan == '\n' )
			++ lineCount;

		++ scan;
		++ charIndex;
	}

	return lineCount;
}


//-----------------------------------------------------------------------------------------------
std::string ResourceStream::FetchLine( const int lineNumberForRequestedLine ) const
{
	if( m_internalFormat == RESOURCE_STREAM_FORMAT_BINARY || m_internalFormat == RESOURCE_STREAM_FORMAT_INVALID || !IsValid() || IsCorrupt() || m_dataBytesAvailable == 0 )
		return 0;

	int currentLine = 1;
	int charIndex = 0;
	const char* lineBeginning = (const char*) m_startOfData;
	const char* scan = lineBeginning;
	for( ;; )
	{
		if( charIndex >= m_dataBytesAvailable )
			break;

		if( *scan == '\n' )
		{
			if( currentLine == lineNumberForRequestedLine )
			{
				const char* lineEnding = scan - 1;
				const int lineLength = (int)(lineEnding - lineBeginning);
				const std::string lineText( lineBeginning, lineLength );
				return lineText;
			}

			lineBeginning = scan + 1;
			++ currentLine;
		}

		++ scan;
		++ charIndex;
	}

	return "";
}

