//-----------------------------------------------------------------------------------------------
// MathBase.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_MathBase__
#define __include_MathBase__
#pragma once
#include <math.h>
#include "Shared.hpp"


//-----------------------------------------------------------------------------------------------
// Macros
//
#define DEG2RAD( x ) ( x * 0.017453292519943295769236907684886 )
#define RAD2DEG( x ) ( x * 57.295779513082320876798154814105 )
#define fDEG2RAD( x ) ( x * 0.017453292519943295769236907684886f )
#define fRAD2DEG( x ) ( x * 57.295779513082320876798154814105f )


//-----------------------------------------------------------------------------------------------
// Constants
//
const double PI = 3.1415926535897932384626433832795;
const double TWO_PI = 6.283185307179586476925286766559;
const double HALF_PI = 1.5707963267948966192313216916398;
const double THIRD_PI = 1.0471975511965977461542144610932;
const double SQRT_2 = 1.4142135623730950488016887242097;
const double SQRT_3 = 1.7320508075688772935274463415059;
const double HALF_SQRT_2 = 0.70710678118654752440084436210485;
const double HALF_SQRT_3 = 0.86602540378443864676372317075294;

const float fPI = 3.1415926535897932384626433832795f;
const float fTWO_PI = 6.283185307179586476925286766559f;
const float fHALF_PI = 1.5707963267948966192313216916398f;
const float fTHIRD_PI = 1.0471975511965977461542144610932f;
const float fSQRT_2 = 1.4142135623730950488016887242097f;
const float fSQRT_3 = 1.7320508075688772935274463415059f;
const float fHALF_SQRT_2 = 0.70710678118654752440084436210485f;
const float fHALF_SQRT_3 = 0.86602540378443864676372317075294f;


//-----------------------------------------------------------------------------------------------
inline float FastApproximateInverseSqrt( float number )
{
	int temp = *((int*)&number);
	temp = 0x5f3759d5 - (temp >> 1); // Newton's Method, initial estimate
	number = *((float*)&temp);
	number *= (1.0f - (0.5f * number * number * number)); // One iteration of Newton's Method
	return number;
}


//-----------------------------------------------------------------------------------------------
inline int RoundToNearestInt( const float number )
{
	return (int) floor( number + 0.5f );
}


//-------------------------------------------------------------------------------------------------
// Returns true if <value> is an exact (positive) power of two
// 
inline bool IsAPowerOfTwo( unsigned int value )
{
	if( value <= 0 )
		return false;

	return (value & (value-1)) == 0;
}

//-----------------------------------------------------------------------------------------------
template< typename T_ValueType >
T_ValueType Minimum( const T_ValueType& value1, const T_ValueType& value2 )
{
	if( value1 < value2 )
		return value1;
	else
		return value2;
}


//-----------------------------------------------------------------------------------------------
template< typename T_ValueType >
T_ValueType Minimum( const T_ValueType& value1, const T_ValueType& value2, const T_ValueType& value3 )
{
	if( value1 < value2 )
	{
		if( value1 < value3 )
			return value1;
		else
			return value3;
	}
	else
	{
		if( value2 < value3 )
			return value2;
		else
			return value3;
	}
}


//-----------------------------------------------------------------------------------------------
template< typename T_ValueType >
T_ValueType Maximum( const T_ValueType& value1, const T_ValueType& value2 )
{
	if( value1 < value2 )
		return value2;
	else
		return value1;
}


//-----------------------------------------------------------------------------------------------
template< typename T_ValueType >
T_ValueType Maximum( const T_ValueType& value1, const T_ValueType& value2, const T_ValueType& value3 )
{
	if( value1 < value2 )
	{
		if( value2 < value3 )
			return value3;
		else
			return value2;
	}
	else
	{
		if( value1 < value3 )
			return value3;
		else
			return value1;
	}
}


//-----------------------------------------------------------------------------------------------
template< typename T_ValueType >
T_ValueType Clamp( const T_ValueType& incomingValue, const T_ValueType& minimumValue, const T_ValueType& maximumValue )
{
	if( incomingValue < minimumValue )
		return minimumValue;
	else if( maximumValue < incomingValue )
		return maximumValue;
	else
		return incomingValue;
}


//-------------------------------------------------------------------------------------------------
inline const int Interpolate( const int lerpFrom, const int lerpTo, const float lerpToFraction )
{
	const float lerpFromFraction = 1.0f - lerpToFraction;
	int returnValue = RoundToNearestInt( (lerpFromFraction * (float)lerpFrom) + (lerpToFraction * (float)lerpTo) );
	return returnValue ;
}


//-------------------------------------------------------------------------------------------------
inline const unsigned int Interpolate( const unsigned int lerpFrom, const unsigned int lerpTo, const float lerpToFraction )
{
	const float lerpFromFraction = 1.0f - lerpToFraction;
	int returnValue = RoundToNearestInt( (lerpFromFraction * (float)lerpFrom) + (lerpToFraction * (float)lerpTo) );
	return (unsigned int)returnValue ;
}


//-------------------------------------------------------------------------------------------------
inline const double Interpolate( const double& lerpFrom, const double& lerpTo, const float lerpToFraction )
{
	double lerpValue = ((double)lerpFrom * (1.0 - (double)lerpToFraction) ) + ((double)lerpTo * (double)lerpToFraction );
	return lerpValue ;
}


//-------------------------------------------------------------------------------------------------
inline const float Interpolate( const float& lerpFrom, const float& lerpTo, const float lerpToFraction )
{
	double lerpValue = ((double)lerpFrom * (1.0 - (double)lerpToFraction) ) + ((double)lerpTo * (double)lerpToFraction );
	return (float)lerpValue ;
}


#endif // __include_MathBase__
