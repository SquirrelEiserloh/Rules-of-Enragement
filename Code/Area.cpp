//-----------------------------------------------------------------------------------------------
// Area.cpp
//-----------------------------------------------------------------------------------------------
#include "TheGame.hpp"
#include "Graphics.hpp"


//-----------------------------------------------------------------------------------------------
Area::Area()
	: m_bounds( 100.f, 100.f, 200.f, 200.f )
	, m_color( Rgba::WHITE )
	, m_alpha( 0.5f )
	, m_impassableToPlayer( false )
	, m_impassableToNPC( false )
	, m_onPlayerTouch( AREA_RESPONSE_NONE )
	, m_onPlayerEnter( AREA_RESPONSE_NONE )
	, m_onNPCTouch( AREA_RESPONSE_NONE )
	, m_onNPCEnter( AREA_RESPONSE_NONE )
{
}


//-----------------------------------------------------------------------------------------------
void Area::Draw( bool isShadowPass )
{
	if( isShadowPass )
	{
		const Vector2 areaShadowOffset( 10.f, 10.f );
		AABB2 shadow1Area( m_bounds );
		AABB2 shadow2Area( m_bounds );
		AABB2 shadow3Area( m_bounds );
		shadow1Area.AddPadding( 0.f, 0.f );
		shadow2Area.AddPadding( 4.f, 4.f );
		shadow3Area.AddPadding( 8.f, 8.f );
		shadow1Area.Translate( areaShadowOffset );
		shadow2Area.Translate( areaShadowOffset );
		shadow3Area.Translate( areaShadowOffset );

		DrawFilledArea( shadow1Area, Rgba::BLACK, 0.1f * m_alpha );
		DrawFilledArea( shadow2Area, Rgba::BLACK, 0.1f * m_alpha );
		DrawFilledArea( shadow3Area, Rgba::BLACK, 0.1f * m_alpha );
	}
	else
	{
		DrawFilledArea( m_bounds, m_color, m_alpha );
	}
}


