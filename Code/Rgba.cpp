//-----------------------------------------------------------------------------------------------
// Rgba.cpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "Rgba.hpp"
#include <cmath>

//-----------------------------------------------------------------------------------------------
// Static member initialization
//
STATIC const float Rgba::COLOR_BYTE_TO_FLOAT_CONVERSION_RATIO	= 1.0f / 255.0f ;

STATIC const Rgba Rgba::WHITE		( 255,	255,	255	);
STATIC const Rgba Rgba::BLACK		( 0,	0,		0	);
STATIC const Rgba Rgba::RED			( 255,	0,		0	);
STATIC const Rgba Rgba::GREEN		( 0,	255,	0	);
STATIC const Rgba Rgba::BLUE		( 0,	0,		255	);
STATIC const Rgba Rgba::YELLOW		( 255,	255,	0	);
STATIC const Rgba Rgba::PURPLE		( 255,	0,		255	);
STATIC const Rgba Rgba::CYAN		( 0,	255,	255	);
STATIC const Rgba Rgba::ORANGE		( 255,	128,	0	);
STATIC const Rgba Rgba::GOLD		( 255,	180,	0	);
STATIC const Rgba Rgba::GREY		( 128,	128,	128	);

STATIC const Rgba Rgba::DARKRED		( 128,	0,		0	);
STATIC const Rgba Rgba::DARKGREEN	( 0,	128,	0	);
STATIC const Rgba Rgba::DARKBLUE	( 0,	0,		128	);
STATIC const Rgba Rgba::DARKYELLOW	( 128,	128,	0	);
STATIC const Rgba Rgba::DARKPURPLE	( 128,	0,		128	);
STATIC const Rgba Rgba::DARKCYAN	( 0,	128,	128	);
STATIC const Rgba Rgba::DARKORANGE	( 128,	64,		0	);
STATIC const Rgba Rgba::DARKGREY	( 64,	64,		64	);

STATIC const Rgba Rgba::LIGHTRED	( 255,	80,		80	);
STATIC const Rgba Rgba::LIGHTGREEN	( 80,	255,	80	);
STATIC const Rgba Rgba::LIGHTBLUE	( 80,	80,		255	);
STATIC const Rgba Rgba::LIGHTYELLOW	( 255,	255,	80	);
STATIC const Rgba Rgba::LIGHTPURPLE	( 255,	80,		255	);
STATIC const Rgba Rgba::LIGHTCYAN	( 80,	255,	255	);
STATIC const Rgba Rgba::LIGHTORANGE	( 255,	160,	80	);
STATIC const Rgba Rgba::LIGHTGREY	( 192,	192,	192	);

STATIC const Rgba Rgba::TRANSPARENTWHITE ( 255, 255, 255, 0 );
STATIC const Rgba Rgba::TRANSPARENTBLACK ( 0, 0, 0, 0 );

STATIC const RgbaList Rgba::EMPTY_LIST ;


//-----------------------------------------------------------------------------------------------
std::string GetTypedObjectAsString( const Rgba& typedObject, std::string* out_optionalTypeNameAsString )
{
	if( out_optionalTypeNameAsString )
	{
		*out_optionalTypeNameAsString = "Rgba";
	}
	return Stringf( "%d,%d,%d,%d", typedObject.r, typedObject.g, typedObject.b, typedObject.a );
}


//-----------------------------------------------------------------------------------------------
Rgba& Rgba::SetLuminance( const unsigned char targetLuminance )
{
	if( targetLuminance == (unsigned char)0xff )
	{
		// Setting maximum luminance; must be white
		r = (unsigned char)0xff;
		g = (unsigned char)0xff;
		b = (unsigned char)0xff;
		return *this;
	}
	else if( targetLuminance == 0 )
	{
		// Setting mimimum luminance; must be black
		r = 0;
		g = 0;
		b = 0;
		return *this;
	}

	const unsigned char currentLuminance = CalcLuminance();
	if( currentLuminance == 0 )
	{
		// Color is currently black; nothing to scale, so just jump directly to grey luminance
		r = targetLuminance;
		g = targetLuminance;
		b = targetLuminance;
		return *this;
	}

	// See how much we could scale this RGB linearly before losing saturation or hue
	const float maximumScaleForR = (r > 0.0f) ? (255.0f / r) : (255.0f);
	const float maximumScaleForG = (g > 0.0f) ? (255.0f / g) : (255.0f);
	const float maximumScaleForB = (b > 0.0f) ? (255.0f / b) : (255.0f);
	const float maximumScale = MinFloat( maximumScaleForR, maximumScaleForG, maximumScaleForB );
	const float fCurrentLuminance = (float) currentLuminance;
	const float fTargetLuminance = (float) targetLuminance;
	const float idealScale = fTargetLuminance / fCurrentLuminance;

	if( idealScale <= maximumScale )
	{
		(*this) *= idealScale;
		return *this;
	}

	// Scale as far up as we can without de-saturating
	(*this) *= maximumScale;

	const unsigned char luminanceAfterScale = CalcLuminance();
	if( luminanceAfterScale == (unsigned char)0xff )
	{
		// already scaled to white; done!
		return *this;
	}

	const float fLuminanceAfterScale = (float)luminanceAfterScale;
	const float lerpFraction = (fTargetLuminance - fLuminanceAfterScale) / (255.0f - fLuminanceAfterScale);
	InterpolateTo( Rgba::WHITE, lerpFraction );

	return *this;
}


//-----------------------------------------------------------------------------------------------
Rgba::Rgba( const std::string& rgbaDescription )
: m_format( COLOR_FORMAT_RGBA )
{
	SetFromStringDescription( rgbaDescription );
}


//-----------------------------------------------------------------------------------------------
Rgba& Rgba::SetFromStringDescription( const std::string& colorAsString )
{
	std::string colorAsStringLowered	= ReturnAsLowerCase( colorAsString );
	Strings parts = SplitStringOnDelimiter( colorAsStringLowered, ':' );
	if ( parts.size() == 2 )
	{
		if (parts[ 0 ] == "hsv" )
			return SetFromStringHSV( parts[ 1 ] );
		if (parts[ 0 ] == "rgb" )
			return SetFromStringRGBA( parts[ 1 ] );
		if ( parts[ 0 ] == "rgba" )
			return SetFromStringRGBA( parts[1 ] );
		//{ int q = 5; } //JAZZ_ERROR( "Rgba::SetFromStringDescription", Stringf( "Unknown colorspace: %s in %s\n", parts[ 0 ].c_str(), colorAsString.c_str() ).c_str() );
	}

	return SetFromStringRGBA( colorAsStringLowered );
}

///-----------------------------------------------------
///
///-----------------------------------------------------
Rgba& Rgba::SetFromStringRGBA( const std::string& colorAsStringRGBA )
{
	char temp[ 1024 ];
	char openingBrace	= '\0';
	char closingBrace	= '\0';

	// Grab the opening brace, if any
	const char* scan = colorAsStringRGBA.c_str();
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

	// Grab the "r" component
	char* write = temp;
	while( *scan != ',' && *scan != ' ' && *scan != '\t' && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	r = (unsigned char) atoi( temp );

	// Skip the delimiter(s)
	while( *scan == ',' || *scan == ' ' || *scan == '\t' )
		++ scan;

	// Grab the "g" component
	write = temp;
	while( *scan != ',' && *scan != ' ' && *scan != '\t' && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	g = (unsigned char) atoi( temp );

	// Skip the delimiter(s)
	while( *scan == ',' || *scan == ' ' || *scan == '\t' )
		++ scan;

	// Grab the "b" component
	write = temp;
	while( *scan != ',' && *scan != ' ' && *scan != '\t' && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	b = (unsigned char) atoi( temp );

	// Skip the delimiter(s)
	while( *scan == ',' || *scan == ' ' || *scan == '\t' )
		++ scan;

	// Grab the "a" component
	write = temp;
	while( *scan != closingBrace && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';

	// If the alpha component has been left off, default to opaque (255) FIXME: This doesn't protect from whitespace at the end
	if ( write != temp )
		a = (unsigned char) atoi( temp );
	else
		a = 255 ;

	return *this;
}

///-----------------------------------------------------
///
///-----------------------------------------------------
Rgba& Rgba::SetFromStringHSV( const std::string& colorAsStringHSL )
{
	char temp[ 1024 ];
	char openingBrace	= '\0';
	char closingBrace	= '\0';
	unsigned char	h ;
	unsigned char	s ;
	unsigned char	v ; 

	// Grab the opening brace, if any
	const char* scan = colorAsStringHSL.c_str();
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

	// Grab the "r" component
	char* write = temp;
	while( *scan != ',' && *scan != ' ' && *scan != '\t' && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	h = (unsigned char) atoi( temp );

	// Skip the delimiter(s)
	while( *scan == ',' || *scan == ' ' || *scan == '\t' )
		++ scan;

	// Grab the "g" component
	write = temp;
	while( *scan != ',' && *scan != ' ' && *scan != '\t' && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	s = (unsigned char) atoi( temp );

	// Skip the delimiter(s)
	while( *scan == ',' || *scan == ' ' || *scan == '\t' )
		++ scan;

	// Grab the "b" component
	write = temp;
	while( *scan != ',' && *scan != ' ' && *scan != '\t' && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';
	v = (unsigned char) atoi( temp );

	// Skip the delimiter(s)
	while( *scan == ',' || *scan == ' ' || *scan == '\t' )
		++ scan;

	// Grab the "a" component
	write = temp;
	while( *scan != closingBrace && *scan != '\0' )
	{
		*write = *scan;
		++ write;
		++ scan;
	}
	*write = '\0';

	// If the alpha component has been left off, default to opaque (255) FIXME: This doesn't protect from whitespace at the end
	if ( write != temp )
		a = (unsigned char) atoi( temp );
	else
		a = 255 ;

	SetFromHSV( ByteToFloat( h ), ByteToFloat( s ), ByteToFloat( v ) );
	return *this;
}



//-----------------------------------------------------------------------------------------------
void Rgba::CalcHSV( float& h, float& s, float& v ) const
{
	float fr = ByteToFloat( r );
	float fg = ByteToFloat( g );
	float fb = ByteToFloat( b );
	float min = MinFloat( fr, MinFloat( fg, fb ) );
	float max = MaxFloat( fr, MaxFloat( fg, fb ) );

	v = max;
	float delta = max - min;

	if( max != 0.0f ) 
		s = delta / max;
	else 
	{
		s = 0.0f;
		h = -1.0f;
		return;
	}

	if( fr == max ) 
		h = ( fg - fb ) / delta;
	else if( fg == max )
		h = 2.0f + ( fb - fr ) / delta;
	else 
		h = 4.0f + ( fr - fg ) / delta;
	h *= 60.0f;
	if( h < 0.0f ) 
		h += 360;
	h /= 360.0f;
}

///-----------------------------------------------------
/// This function assumes all 3 inputs are between [0.0f,1.0f]
/// NOTE: It converts h into a float between [0.0f,360.0f] before continuing
/// because the algorithm depends on that...
///-----------------------------------------------------
Rgba& Rgba::SetFromHSV( float h, float s, float v )
{
	// Store off the untouched HSV that we used (h is modified below)
	m_h			= FloatToByte( h );
	m_s			= FloatToByte( s );
	m_v			= FloatToByte( v );

	int i;
	float f, p, q, t, fr, fg, fb;
	if( s == 0.0f )
		r = g = b = FloatToByte( v );

	///-----------------------------------------------------
	/// NOTE: Assuming input is normalized, we put it into a spherical (360 degree) space
	///-----------------------------------------------------
	h *= 360.0f ;

	h /= 60.0f;
	i = (int)std::floor( h );
	f = h - i;
	p = v * ( 1.0f - s );
	q = v * ( 1.0f - s * f );
	t = v * ( 1.0f - s * ( 1.0f - f ) );

	switch( i ) {
	case 0:
		fr = v;
		fg = t;
		fb = p;
		break;
	case 1:
		fr = q;
		fg = v;
		fb = p;
		break;
	case 2:
		fr = p;
		fg = v;
		fb = t;
		break;
	case 3:
		fr = p;
		fg = q;
		fb = v;
		break;
	case 4:
		fr = t;
		fg = p;
		fb = v;
		break;
	default:
		fr = v;
		fg = p;
		fb = q;
		break;
	}
	r			= FloatToByte( fr );
	g			= FloatToByte( fg );
	b			= FloatToByte( fb );
	m_format	= COLOR_FORMAT_HSV ;

	return *this;
}
