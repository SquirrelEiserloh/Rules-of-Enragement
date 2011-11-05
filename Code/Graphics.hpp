//-----------------------------------------------------------------------------------------------
// Graphics.hpp
//-----------------------------------------------------------------------------------------------
#ifndef __include_Graphics__
#define __include_Graphics__

#include "AABB2.hpp"
#include "Vector2.hpp"
#include "Rgba.hpp"
#include "Clock.hpp"



void InitGraphics();
void ComputeCirclePoints();
void SetColor( const Rgba& color, float alpha=1.f );
void DrawFilledCircle( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawOutlinedCircle( const Vector2& center, float radius, const Rgba& color, float alpha=1.f );
void DrawFilledOutlinedCircle( const Vector2& center, float radius, const Rgba& fillColor, const Rgba& edgeColor, float alpha=1.f );
void DrawFilledArea( const AABB2& area, const Rgba& color, float alpha=1.f );
void DrawOutlinedArea( const AABB2& area, const Rgba& color, float alpha=1.f );
void DrawFilledOutlinedArea( const AABB2& area, const Rgba& fillColor, const Rgba& edgeColor, float alpha=1.f );



#endif // __include_Graphics__
