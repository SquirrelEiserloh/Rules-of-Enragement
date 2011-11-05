//-----------------------------------------------------------------------------------------------
// Rgba.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------

// Forward declarations
class Rgba ;

#ifndef __include_Rgba__
#define __include_Rgba__
#pragma once
#include "Utilities.hpp"

typedef std::vector< Rgba >		RgbaList ;

//========================================================
//
//========================================================
enum ColorFormat
{
	COLOR_FORMAT_RGBA,
	COLOR_FORMAT_HSV,
};



/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Rgba
//
/////////////////////////////////////////////////////////////////////////////////////////////////
class Rgba
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	unsigned char m_h ;
	unsigned char m_s ;
	unsigned char m_v ; // value or lightness (see http://en.wikipedia.org/wiki/HSL_and_HSV)
	unsigned char m_format ;

	static const float COLOR_BYTE_TO_FLOAT_CONVERSION_RATIO ;
	static const Rgba WHITE;
	static const Rgba BLACK;
	static const Rgba RED;
	static const Rgba GREEN;
	static const Rgba BLUE;
	static const Rgba YELLOW;
	static const Rgba PURPLE;
	static const Rgba CYAN;
	static const Rgba ORANGE;
	static const Rgba GOLD;
	static const Rgba GREY;
	static const Rgba DARKRED;
	static const Rgba DARKGREEN;
	static const Rgba DARKBLUE;
	static const Rgba DARKYELLOW;
	static const Rgba DARKPURPLE;
	static const Rgba DARKCYAN;
	static const Rgba DARKORANGE;
	static const Rgba DARKGREY;
	static const Rgba LIGHTRED;
	static const Rgba LIGHTGREEN;
	static const Rgba LIGHTBLUE;
	static const Rgba LIGHTYELLOW;
	static const Rgba LIGHTPURPLE;
	static const Rgba LIGHTCYAN;
	static const Rgba LIGHTORANGE;
	static const Rgba LIGHTGREY;
	static const Rgba TRANSPARENTWHITE;
	static const Rgba TRANSPARENTBLACK;
	static const RgbaList EMPTY_LIST ;

public:
	///-----------------------------------------------------
	/// Statics
	///-----------------------------------------------------
	static const float ByteToFloat( const unsigned char b );
	static const unsigned char FloatToByte( const float f );
	static const int FloatToByteAsInt( const float f );
	static const int ScaleByte( const unsigned char unscaledByte, const float scale );

	///-----------------------------------------------------
	// Constructors
	///-----------------------------------------------------
	~Rgba() {}
	Rgba() 
	: m_format( COLOR_FORMAT_RGBA ) {}
	Rgba( const Rgba& copy );
	explicit Rgba( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = (unsigned char)0xff );
	explicit Rgba( const std::string& rgbaDescription );

	///-----------------------------------------------------
	/// Queries
	///-----------------------------------------------------
	bool				IsHSV()	const																{ return ( m_format == COLOR_FORMAT_HSV ); }
	bool				IsRGBA() const																{ return ( m_format == COLOR_FORMAT_RGBA ); }

	///-----------------------------------------------------
	/// Accessors
	///-----------------------------------------------------
	ColorFormat			GetColorFormat() const														{ return (ColorFormat)m_format ; }
	void				GetRgbaFloats( OUTPUT float& red, OUTPUT float& green, OUTPUT float& blue, OUTPUT float& alpha ) const;
	void				GetRgbFloats( OUTPUT float& red, OUTPUT float& green, OUTPUT float& blue ) const;
	void				GetRgbaBytes( OUTPUT unsigned char& red, OUTPUT unsigned char& green, OUTPUT unsigned char& blue, OUTPUT unsigned char& alpha ) const;
	void				GetRgbBytes( OUTPUT unsigned char& red, OUTPUT unsigned char& green, OUTPUT unsigned char& blue ) const;
	const unsigned int		GetAsDwordRGBA() const;
	const unsigned int		GetAsDwordBGRA() const;
	const unsigned int		GetAsDwordARGB() const;
	const unsigned int		GetAsDwordABGR() const;

	///-----------------------------------------------------
	/// Mutators
	///-----------------------------------------------------
	Rgba&				SetRgbaFloats( const float red, const float green, const float blue, const float alpha = 1.0f );
	Rgba&				SetRgbaBytes( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = (unsigned char)0xff );
	Rgba&				SetFromDwordRGBA( unsigned int rgba );
	Rgba&				SetFromDwordBGRA( unsigned int bgra );
	Rgba&				SetFromDwordARGB( unsigned int argb );
	Rgba&				SetFromDwordABGR( unsigned int abgr );
	Rgba&				SetFromStringDescription( const std::string& colorAsString );
	Rgba&				SetFromStringRGBA( const std::string& colorAsStringRGBA );
	Rgba&				SetFromStringHSV( const std::string& colorAsStringHSV );
	Rgba&				SetLuminance( const unsigned char luminance );
	Rgba&				SetHueDegrees( const float hueDegrees );

	///-----------------------------------------------------
	/// Methods
	///-----------------------------------------------------
	Rgba&				RandomizeRGB();
	Rgba&				RandomizeRGBA();
	Rgba&				InterpolateTo( const Rgba& lerpTo, const float lerpToFraction );
	Rgba&				InterpolateToThroughHSV( const Rgba& lerpToAsHSV, const float lerpToFraction );
	Rgba&				ScaleAlpha( const float scale );
	const unsigned char		CalcLuminance() const;
	const float			CalcHueDegrees() const;
	void				CalcHSV( float& h, float& s, float&v ) const;
	Rgba&				SetFromHSV( float h, float s, float v );

	///-----------------------------------------------------
	/// Operators
	///-----------------------------------------------------
	Rgba& operator = ( const Rgba& rgbaToCopyFrom );
	Rgba& operator += ( const Rgba& rgbaToAdd );
	Rgba& operator -= ( const Rgba& rgbaToSubtract );
	Rgba& operator *= ( const float scale ); // NOTE: this ONLY scales the RGB components, not Alpha!
	Rgba& operator *= ( const Rgba& tint );
	const Rgba operator + ( const Rgba& rgbaToAdd ) const;
	const Rgba operator - ( const Rgba& rgbaToSubtract ) const;
	const Rgba operator * ( const float scale ) const; // NOTE: this ONLY scales the RGB components, not Alpha!
	const Rgba operator * ( const Rgba& tint ) const;
	bool operator == ( const Rgba& rgbaToCompareAgainst ) const;
	bool operator != ( const Rgba& rgbaToCompareAgainst ) const;
	unsigned char operator [] ( const int index ) const;
	unsigned char& operator [] ( const int index );

	// Standalone friend functions
	friend const Rgba operator * ( const float scale, const Rgba& rgbaToBeScaled ); // NOTE: this ONLY scales the RGB components, not Alpha!
	friend const Rgba Interpolate( const Rgba& lerpFrom, const Rgba& lerpTo, const float lerpToFraction );
	friend const Rgba InterpolateThroughHSV( const Rgba& lerpFrom, const Rgba& lerpTo, const float lerpToFraction );
	friend const Rgba InterpolateHSV( const Rgba& lerpFrom, const Rgba& lerpTo, const float lerpToFraction );
	friend const bool IsEpsilonEqual( const Rgba& colorOne, const Rgba& colorTwo, const unsigned char tolerance );
};


//-----------------------------------------------------------------------------------------------
std::string GetTypedObjectAsString( const Rgba& typedObject, std::string* out_optionalTypeNameAsString );


//-----------------------------------------------------------------------------------------------
inline STATIC const float Rgba::ByteToFloat( const unsigned char b )
{
	return( float( b ) * (1.0f / 255.0f) );
}


//-----------------------------------------------------------------------------------------------
inline STATIC const unsigned char Rgba::FloatToByte( const float f )
{
	return( (unsigned char) (f * 255.99f) );
}


//-----------------------------------------------------------------------------------------------
inline STATIC const int Rgba::FloatToByteAsInt( const float f )
{
	return( (int) (f * 255.99f) );
}


//-----------------------------------------------------------------------------------------------
inline STATIC const int Rgba::ScaleByte( const unsigned char unscaledByte, const float scale )
{
	const float asFloat = ByteToFloat( unscaledByte );
	return FloatToByteAsInt( scale * asFloat );
}


//-----------------------------------------------------------------------------------------------
inline Rgba::Rgba( const Rgba& copy )
: r( copy.r )
, g( copy.g )
, b( copy.b )
, a( copy.a )
, m_format( copy.m_format )
, m_h( copy.m_h )
, m_s( copy.m_s )
, m_v( copy.m_v )
{
}


//-----------------------------------------------------------------------------------------------
inline Rgba::Rgba( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha )
: r( red )
, g( green )
, b( blue )
, a( alpha )
, m_format( COLOR_FORMAT_RGBA )
{
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::SetRgbaFloats( const float red, const float green, const float blue, const float alpha )
{
	r = FloatToByte( red );
	g = FloatToByte( green );
	b = FloatToByte( blue );
	a = FloatToByte( alpha );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::SetRgbaBytes( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha )
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::SetFromDwordRGBA( unsigned int rgba )
{
	r = (unsigned char)( (rgba & 0xff000000) >> 24 );
	g = (unsigned char)( (rgba & 0x00ff0000) >> 16 );
	b = (unsigned char)( (rgba & 0x0000ff00) >> 8 );
	a = (unsigned char)( (rgba & 0x000000ff) >> 0 );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::SetFromDwordBGRA( unsigned int bgra )
{
	b = (unsigned char)( (bgra & 0xff000000) >> 24 );
	g = (unsigned char)( (bgra & 0x00ff0000) >> 16 );
	r = (unsigned char)( (bgra & 0x0000ff00) >> 8 );
	a = (unsigned char)( (bgra & 0x000000ff) >> 0 );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::SetFromDwordARGB( unsigned int argb )
{
	a = (unsigned char)( (argb & 0xff000000) >> 24 );
	r = (unsigned char)( (argb & 0x00ff0000) >> 16 );
	g = (unsigned char)( (argb & 0x0000ff00) >> 8 );
	b = (unsigned char)( (argb & 0x000000ff) >> 0 );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::SetFromDwordABGR( unsigned int abgr )
{
	a = (unsigned char)( (abgr & 0xff000000) >> 24 );
	b = (unsigned char)( (abgr & 0x00ff0000) >> 16 );
	g = (unsigned char)( (abgr & 0x0000ff00) >> 8 );
	r = (unsigned char)( (abgr & 0x000000ff) >> 0 );
	return *this;
}


//-----------------------------------------------------------------------------------------------
//inline Rgba& Rgba::SetFromStringDescription( const std::string& rgbaDescription )
//{
//}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::RandomizeRGB()
{
	r = unsigned char( RandomIntLessThan( 256 ) );
	g = unsigned char( RandomIntLessThan( 256 ) );
	b = unsigned char( RandomIntLessThan( 256 ) );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::RandomizeRGBA()
{
	r = unsigned char( RandomIntLessThan( 256 ) );
	g = unsigned char( RandomIntLessThan( 256 ) );
	b = unsigned char( RandomIntLessThan( 256 ) );
	a = unsigned char( RandomIntLessThan( 256 ) );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::InterpolateTo( const Rgba& lerpTo, const float lerpToFraction )
{
	if ( IsHSV() && lerpTo.IsHSV() )
		return InterpolateToThroughHSV( lerpTo, lerpToFraction );

	const float lerpFromFraction = 1.0f - lerpToFraction;
	const float fR = (lerpFromFraction * ByteToFloat( r )) + (lerpToFraction * ByteToFloat( lerpTo.r ));
	const float fG = (lerpFromFraction * ByteToFloat( g )) + (lerpToFraction * ByteToFloat( lerpTo.g ));
	const float fB = (lerpFromFraction * ByteToFloat( b )) + (lerpToFraction * ByteToFloat( lerpTo.b ));
	const float fA = (lerpFromFraction * ByteToFloat( a )) + (lerpToFraction * ByteToFloat( lerpTo.a ));

	r = FloatToByte( fR );
	g = FloatToByte( fG );
	b = FloatToByte( fB );
	a = FloatToByte( fA );

	return *this;
}

///-----------------------------------------------------
///
///-----------------------------------------------------
inline Rgba& Rgba::InterpolateToThroughHSV( const Rgba& lerpTo, const float lerpToFraction )
{
	const float lerpFromFraction = 1.0f - lerpToFraction;
	const float fh = (lerpFromFraction * ByteToFloat( m_h )) + (lerpToFraction * ByteToFloat( lerpTo.m_h ));
	const float fs = (lerpFromFraction * ByteToFloat( m_s )) + (lerpToFraction * ByteToFloat( lerpTo.m_s ));
	const float fv = (lerpFromFraction * ByteToFloat( m_v )) + (lerpToFraction * ByteToFloat( lerpTo.m_v ));
	const float fA = (lerpFromFraction * ByteToFloat( a )) + (lerpToFraction * ByteToFloat( lerpTo.a ));

	a = FloatToByte( fA );
	SetFromHSV( fh, fs, fv );

	return *this;

}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::ScaleAlpha( const float scale )
{
	const int scaledA = ScaleByte( a, scale );
	a = (unsigned char) ClampInt( scaledA, 0, 255 );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline void Rgba::GetRgbaFloats( OUTPUT float& red, OUTPUT float& green, OUTPUT float& blue, OUTPUT float& alpha ) const
{
	red = ByteToFloat( r );
	green = ByteToFloat( g );
	blue = ByteToFloat( b );
	alpha = ByteToFloat( a );
}


//-----------------------------------------------------------------------------------------------
inline void Rgba::GetRgbFloats( OUTPUT float& red, OUTPUT float& green, OUTPUT float& blue ) const
{
	red = ByteToFloat( r );
	green = ByteToFloat( g );
	blue = ByteToFloat( b );
}


//-----------------------------------------------------------------------------------------------
inline void Rgba::GetRgbaBytes( OUTPUT unsigned char& red, OUTPUT unsigned char& green, OUTPUT unsigned char& blue, OUTPUT unsigned char& alpha ) const
{
	red = r;
	green = g;
	blue = b;
	alpha = a;
}


//-----------------------------------------------------------------------------------------------
inline void Rgba::GetRgbBytes( OUTPUT unsigned char& red, OUTPUT unsigned char& green, OUTPUT unsigned char& blue ) const
{
	red = r;
	green = g;
	blue = b;
}


//-----------------------------------------------------------------------------------------------
inline const unsigned int Rgba::GetAsDwordRGBA() const
{
	return(	(unsigned int)( r << 24 ) |
			(unsigned int)( g << 16 ) |
			(unsigned int)( b <<  8 ) |
			(unsigned int)( a <<  0 ) );
}


//-----------------------------------------------------------------------------------------------
inline const unsigned int Rgba::GetAsDwordBGRA() const
{
	return(	(unsigned int)( b << 24 ) |
			(unsigned int)( g << 16 ) |
			(unsigned int)( r <<  8 ) |
			(unsigned int)( a <<  0 ) );
}


//-----------------------------------------------------------------------------------------------
inline const unsigned int Rgba::GetAsDwordARGB() const
{
	return(	(unsigned int)( a << 24 ) |
			(unsigned int)( r << 16 ) |
			(unsigned int)( g <<  8 ) |
			(unsigned int)( b <<  0 ) );
}


//-----------------------------------------------------------------------------------------------
inline const unsigned int Rgba::GetAsDwordABGR() const
{
	return(	(unsigned int)( a << 24 ) |
			(unsigned int)( b << 16 ) |
			(unsigned int)( g <<  8 ) |
			(unsigned int)( r <<  0 ) );
}


//-----------------------------------------------------------------------------------------------
inline const unsigned char Rgba::CalcLuminance() const
{
//	const int milliLuminance = (299 * (int)r) + (587 * (int)g) + (114 * (int)b); // WTF?  This doesn't seem consistent at all... (is gamma the problem here?)
//	const int milliLuminance = (280 * (int)r) + (465 * (int)g) + (255 * (int)b);
	const int milliLuminance = (290 * (int)r) + (526 * (int)g) + (184 * (int)b);
	return (unsigned char) ClampInt( milliLuminance / 1000, 0, 255 );
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::operator = ( const Rgba& rgbaToCopyFrom )
{
	r			= rgbaToCopyFrom.r;
	g			= rgbaToCopyFrom.g;
	b			= rgbaToCopyFrom.b;
	a			= rgbaToCopyFrom.a;
	m_h			= rgbaToCopyFrom.m_h ;
	m_s			= rgbaToCopyFrom.m_s ;
	m_v			= rgbaToCopyFrom.m_v ;
	m_format	= rgbaToCopyFrom.m_format ;
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::operator += ( const Rgba& rgbaToAdd )
{
	// FIXME: If we're both HSV, do this in HSV space
	r = (unsigned char) MinInt( r + rgbaToAdd.r, 255 );
	g = (unsigned char) MinInt( g + rgbaToAdd.g, 255 );
	b = (unsigned char) MinInt( b + rgbaToAdd.b, 255 );
	a = (unsigned char) MinInt( a + rgbaToAdd.a, 255 );

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::operator -= ( const Rgba& rgbaToSubtract )
{
	// FIXME: If we're both HSV, do this in HSV space
	r = (unsigned char) MaxInt( r + rgbaToSubtract.r, 0 );
	g = (unsigned char) MaxInt( g + rgbaToSubtract.g, 0 );
	b = (unsigned char) MaxInt( b + rgbaToSubtract.b, 0 );
	a = (unsigned char) MaxInt( a + rgbaToSubtract.a, 0 );
	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::operator *= ( const float scale ) // NOTE: this ONLY scales the RGB components, not Alpha
{
	// FIXME: If I'm HSV, do this in HSV space
	const int scaledR = ScaleByte( r, scale );
	const int scaledG = ScaleByte( g, scale );
	const int scaledB = ScaleByte( b, scale );

	r = (unsigned char) ClampInt( scaledR, 0, 255 );
	g = (unsigned char) ClampInt( scaledG, 0, 255 );
	b = (unsigned char) ClampInt( scaledB, 0, 255 );

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline Rgba& Rgba::operator *= ( const Rgba& tint )
{
	// FIXME: If we're both HSV, do this in HSV space
	int productR = (int)r * (int)tint.r;
	int productG = (int)g * (int)tint.g;
	int productB = (int)b * (int)tint.b;
	int productA = (int)a * (int)tint.a;

	r = (unsigned char) ClampInt( productR / 255, 0, 255 );
	g = (unsigned char) ClampInt( productG / 255, 0, 255 );
	b = (unsigned char) ClampInt( productB / 255, 0, 255 );
	a = (unsigned char) ClampInt( productA / 255, 0, 255 );

	return *this;
}


//-----------------------------------------------------------------------------------------------
inline const Rgba Rgba::operator + ( const Rgba& rgbaToAdd ) const
{
	// FIXME: If we're both HSV, do this in HSV space
	return Rgba(	(unsigned char) MinInt( r + rgbaToAdd.r, 255 ),
					(unsigned char) MinInt( g + rgbaToAdd.g, 255 ),
					(unsigned char) MinInt( b + rgbaToAdd.b, 255 ),
					(unsigned char) MinInt( a + rgbaToAdd.a, 255 ) );
}


//-----------------------------------------------------------------------------------------------
inline const Rgba Rgba::operator - ( const Rgba& rgbaToSubtract ) const
{
	// FIXME: If we're both HSV, do this in HSV space
	return Rgba(	(unsigned char) MaxInt( r - rgbaToSubtract.r, 0 ),
					(unsigned char) MaxInt( g - rgbaToSubtract.g, 0 ),
					(unsigned char) MaxInt( b - rgbaToSubtract.b, 0 ),
					(unsigned char) MaxInt( a - rgbaToSubtract.a, 0 ) );
}				


//-----------------------------------------------------------------------------------------------
inline const Rgba Rgba::operator * ( const float scale ) const // NOTE: this ONLY scales the RGB components, not Alpha
{
	// FIXME: If I'm HSV, do this in HSV space
	int scaledR = ScaleByte( r, scale );
	int scaledG = ScaleByte( g, scale );
	int scaledB = ScaleByte( b, scale );

	Rgba returnValue;
	returnValue.r = (unsigned char) ClampInt( scaledR, 0, 255 );
	returnValue.g = (unsigned char) ClampInt( scaledG, 0, 255 );
	returnValue.b = (unsigned char) ClampInt( scaledB, 0, 255 );
	returnValue.a = a;
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
inline const Rgba Rgba::operator * ( const Rgba& tint ) const
{
	// FIXME: If we're both HSV, do this in HSV space
	int productR = (int)r * (int)tint.r;
	int productG = (int)g * (int)tint.g;
	int productB = (int)b * (int)tint.b;
	int productA = (int)a * (int)tint.a;

	return Rgba(	(unsigned char) ClampInt( productR / 255, 0, 255 ),
					(unsigned char) ClampInt( productG / 255, 0, 255 ),
					(unsigned char) ClampInt( productB / 255, 0, 255 ),
					(unsigned char) ClampInt( productA / 255, 0, 255 ) );
}


//-----------------------------------------------------------------------------------------------
inline bool Rgba::operator == ( const Rgba& rgbaToCompareAgainst ) const
{
	return( (r == rgbaToCompareAgainst.r) && (g == rgbaToCompareAgainst.g) && (b == rgbaToCompareAgainst.b) && (a == rgbaToCompareAgainst.a) );
}


//-----------------------------------------------------------------------------------------------
inline bool Rgba::operator != ( const Rgba& rgbaToCompareAgainst ) const
{
	return( (r != rgbaToCompareAgainst.r) || (g != rgbaToCompareAgainst.g) || (b != rgbaToCompareAgainst.b) || (a != rgbaToCompareAgainst.a) );
}


//-----------------------------------------------------------------------------------------------
inline unsigned char Rgba::operator [] ( const int index ) const
{
	return ( &r )[ index ]; // TODO: Validate index?
}


//-----------------------------------------------------------------------------------------------
inline unsigned char& Rgba::operator [] ( const int index )
{
	return ( &r )[ index ]; // TODO: Validate index?
}


//-----------------------------------------------------------------------------------------------
inline const Rgba operator * ( const float scale, const Rgba& rgbaToBeScaled ) // NOTE: this ONLY scales the RGB components, not Alpha!
{
	// FIXME: If I'm HSV, do this in HSV space
	int scaledR = Rgba::ScaleByte( rgbaToBeScaled.r, scale );
	int scaledG = Rgba::ScaleByte( rgbaToBeScaled.g, scale );
	int scaledB = Rgba::ScaleByte( rgbaToBeScaled.b, scale );

	Rgba returnValue;
	returnValue.r = (unsigned char) ClampInt( scaledR, 0, 255 );
	returnValue.g = (unsigned char) ClampInt( scaledG, 0, 255 );
	returnValue.b = (unsigned char) ClampInt( scaledB, 0, 255 );
	returnValue.a = rgbaToBeScaled.a;
	return returnValue;
}


//-----------------------------------------------------------------------------------------------
inline const Rgba Interpolate( const Rgba& lerpFrom, const Rgba& lerpTo, const float lerpToFraction )
{
	if ( lerpFrom.IsHSV() && lerpTo.IsHSV() )
		return InterpolateThroughHSV( lerpFrom, lerpTo, lerpToFraction );

	const float lerpFromFraction = 1.0f - lerpToFraction;
	const float fR = (lerpFromFraction * Rgba::ByteToFloat( lerpFrom.r )) + (lerpToFraction * Rgba::ByteToFloat( lerpTo.r ));
	const float fG = (lerpFromFraction * Rgba::ByteToFloat( lerpFrom.g )) + (lerpToFraction * Rgba::ByteToFloat( lerpTo.g ));
	const float fB = (lerpFromFraction * Rgba::ByteToFloat( lerpFrom.b )) + (lerpToFraction * Rgba::ByteToFloat( lerpTo.b ));
	const float fA = (lerpFromFraction * Rgba::ByteToFloat( lerpFrom.a )) + (lerpToFraction * Rgba::ByteToFloat( lerpTo.a ));

	return Rgba(	Rgba::FloatToByte( fR ),
					Rgba::FloatToByte( fG ),
					Rgba::FloatToByte( fB ),
					Rgba::FloatToByte( fA ) );
}


//-----------------------------------------------------------------------------------------------
inline const Rgba InterpolateThroughHSV( const Rgba& lerpFrom, const Rgba& lerpTo, const float lerpToFraction ) 
{
	const float lerpFromFraction	= 1.0f - lerpToFraction ;
	float		fromH				= Rgba::ByteToFloat( lerpFrom.m_h	);
	float		fromS				= Rgba::ByteToFloat( lerpFrom.m_s );
	float		fromV				= Rgba::ByteToFloat( lerpFrom.m_v );
	float		toH					= Rgba::ByteToFloat( lerpTo.m_h );
	float		toS					= Rgba::ByteToFloat( lerpTo.m_s );
	float		toV					= Rgba::ByteToFloat( lerpTo.m_v );

	float lerpH = ( lerpFromFraction * fromH ) + ( lerpToFraction * toH );
	float lerpS = ( lerpFromFraction * fromS ) + ( lerpToFraction * toS );
	float lerpV = ( lerpFromFraction * fromV ) + ( lerpToFraction * toV );
	const float fA = (lerpFromFraction * Rgba::ByteToFloat( lerpFrom.a )) + (lerpToFraction * Rgba::ByteToFloat( lerpTo.a ));

	Rgba returnColor;
	returnColor.a = Rgba::FloatToByte( fA );
	returnColor.SetFromHSV( lerpH, lerpS, lerpV );
	return returnColor; 
}



//-----------------------------------------------------------------------------------------------
inline const Rgba InterpolateHSV( const Rgba& lerpFrom, const Rgba& lerpTo, const float lerpToFraction ) 
{
	// FIXME: Remove this and use InterpolateThroughHSV instead.  Code is calling this in project Dawn.
	const float lerpFromFraction = 1.0f - lerpToFraction;
	float fromH, fromS, fromV, toH, toS, toV;
	lerpFrom.CalcHSV( fromH, fromS, fromV );
	lerpTo.CalcHSV( toH, toS, toV );
	float lerpH = ( lerpFromFraction * fromH ) + ( lerpToFraction * toH );
	float lerpS = ( lerpFromFraction * fromS ) + ( lerpToFraction * toS );
	float lerpV = ( lerpFromFraction * fromV ) + ( lerpToFraction * toV );
	const float fA = (lerpFromFraction * Rgba::ByteToFloat( lerpFrom.a )) + (lerpToFraction * Rgba::ByteToFloat( lerpTo.a ));

	Rgba returnColor;
	returnColor.a = Rgba::FloatToByte( fA );
	returnColor.SetFromHSV( lerpH, lerpS, lerpV );
	return returnColor; 
}


//-----------------------------------------------------------------------------------------------
inline const bool IsEpsilonEqual( const Rgba& colorOne, const Rgba& colorTwo, const unsigned char tolerance )
{
	int diffR = (int)colorOne.r - (int)colorTwo.r;
	int diffG = (int)colorOne.g - (int)colorTwo.g;
	int diffB = (int)colorOne.b - (int)colorTwo.b;
	int diffA = (int)colorOne.a - (int)colorTwo.a;

	if( abs( diffR ) > tolerance )
		return false;
	else if( abs( diffG ) > tolerance )
		return false;
	else if( abs( diffB ) > tolerance )
		return false;
	else if( abs( diffA ) > tolerance )
		return false;
	else
		return true;
}

#endif // __include_Rgba__
