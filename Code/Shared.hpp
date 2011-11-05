//-----------------------------------------------------------------------------------------------
// Shared.hpp
//
// Stuff that is included and shared by everybody for simplicity.
//-----------------------------------------------------------------------------------------------
#ifndef __include_Shared__
#define __include_Shared__


//-----------------------------------------------------------------------------------------------
// Definitions
//
#define JAZZ_PLATFORM_WIN32
#define STATIC // A do-nothing marker to denote static methods in .cpp files
#define OUTPUT // A do-nothing marker to denote output arguments
#define UNUSED( x ) (void)(x) // A warning-silencing macro denoting an intentionally unused argument


//-----------------------------------------------------------------------------------------------
// Windows includes
//
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


//-----------------------------------------------------------------------------------------------
// OpenGL includes
//
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "Glu32" ) // Link in the Glu32.lib static library


//-----------------------------------------------------------------------------------------------
// Common standard template library (STL) includes
//
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>

typedef std::string JazzPath;
typedef std::vector< std::string > Strings;

#endif // __include_Shared__
