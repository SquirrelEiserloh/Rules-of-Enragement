//-----------------------------------------------------------------------------------------------
// Graphics.cpp
//-----------------------------------------------------------------------------------------------
#include "Graphics.hpp"


//-----------------------------------------------------------------------------------------------
// Globals
//
const int NUM_CIRCLE_SIDES = 40;
const float LINE_WIDTH = 3.f;
Vector2 g_circlePoints[ NUM_CIRCLE_SIDES ];


//-----------------------------------------------------------------------------------------------
void InitGraphics()
{
	ComputeCirclePoints();

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glShadeModel( GL_SMOOTH );
}


//-----------------------------------------------------------------------------------------------
void ComputeCirclePoints()
{
	for( int i = 0; i < NUM_CIRCLE_SIDES; ++ i )
	{
		float degrees = 360.f * ((float) i / (float) NUM_CIRCLE_SIDES);
		g_circlePoints[ i ].SetUnitLengthAndYawDegrees( degrees );
	}
}


//-----------------------------------------------------------------------------------------------
void SetColor( const Rgba& color, float alpha )
{
	Rgba colorWithAlphaApplied = color;
	colorWithAlphaApplied.ScaleAlpha( alpha );
	glColor4ub( colorWithAlphaApplied.r, colorWithAlphaApplied.g, colorWithAlphaApplied.b, colorWithAlphaApplied.a );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledCircle( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glBegin( GL_TRIANGLE_FAN );
	{
		for( int i = 0; i < NUM_CIRCLE_SIDES; ++ i )
		{
			Vector2 point = center + (radius * g_circlePoints[ i ]);
			glVertex2f( point.x, point.y );
		}
	}
	glEnd();
}


//-----------------------------------------------------------------------------------------------
void DrawOutlinedCircle( const Vector2& center, float radius, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINE_LOOP );
	{
		for( int i = 0; i < NUM_CIRCLE_SIDES; ++ i )
		{
			Vector2 point = center + (radius * g_circlePoints[ i ]);
			glVertex2f( point.x, point.y );
		}
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledOutlinedCircle( const Vector2& center, float radius, const Rgba& fillColor, const Rgba& edgeColor, float alpha )
{
	DrawFilledCircle( center, radius, fillColor, alpha );
	DrawOutlinedCircle( center, radius, edgeColor, alpha );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledArea( const AABB2& area, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glBegin( GL_QUADS );
	{
		glVertex2f( area.mins.x, area.mins.y );
		glVertex2f( area.maxs.x, area.mins.y );
		glVertex2f( area.maxs.x, area.maxs.y );
		glVertex2f( area.mins.x, area.maxs.y );
	}
	glEnd();
}


//-----------------------------------------------------------------------------------------------
void DrawOutlinedArea( const AABB2& area, const Rgba& color, float alpha )
{
	SetColor( color, alpha );
	glLineWidth( LINE_WIDTH );
	glBegin( GL_LINE_LOOP );
	{
		glVertex2f( area.mins.x, area.mins.y );
		glVertex2f( area.maxs.x, area.mins.y );
		glVertex2f( area.maxs.x, area.maxs.y );
		glVertex2f( area.mins.x, area.maxs.y );
	}
	glEnd();
	glLineWidth( 1.f );
}


//-----------------------------------------------------------------------------------------------
void DrawFilledOutlinedArea( const AABB2& area, const Rgba& fillColor, const Rgba& edgeColor, float alpha )
{
	DrawFilledArea( area, fillColor, alpha );
	DrawOutlinedArea( area, edgeColor, alpha );
}

