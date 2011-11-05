//-----------------------------------------------------------------------------------------------
// Scenario.cpp
//-----------------------------------------------------------------------------------------------
#include "TheGame.hpp" // for now, we've got a huge ass monolithic header


/////////////////////////////////////////////////////////////////////////////////////////////////
// Scenario

//-----------------------------------------------------------------------------------------------
Scenario::Scenario()
	: m_name( "UNNAMED SCENARIO" )
	, m_state( SCENARIO_STATE_INACTIVE )
	, m_timeEnteredState( 0.0 )
	, m_startFunction( NULL )
	, m_updateFunction( NULL )
{
}


//-----------------------------------------------------------------------------------------------
void Scenario::Start()
{
	ChangeState( SCENARIO_STATE_INTRO );
	m_startFunction( *this );
}


//-----------------------------------------------------------------------------------------------
void Scenario::Update( double deltaSeconds )
{
	m_updateFunction( *this, deltaSeconds );

	// Update players
	for( unsigned int actorIndex = 0; actorIndex < m_actors.size(); ++ actorIndex )
	{
		Actor& actor = m_actors[ actorIndex ];
		if( actor.m_isPlayer )
		{
			actor.UpdateAsPlayer( deltaSeconds );
		}
	}

	// Update all NPCs
	for( unsigned int actorIndex = 0; actorIndex < m_actors.size(); ++ actorIndex )
	{
		Actor& actor = m_actors[ actorIndex ];
		if( !actor.m_isPlayer )
		{
			actor.Update( deltaSeconds );
		}
	}

}


//-----------------------------------------------------------------------------------------------
void Scenario::Render()
{
	// Render all areas (shadows first, then normal)
	unsigned int areaIndex;
	for( areaIndex = 0; areaIndex < m_areas.size(); ++ areaIndex )
	{
		Area& area = m_areas[ areaIndex ];
		RenderArea( area, true );
	}
	for( areaIndex = 0; areaIndex < m_areas.size(); ++ areaIndex )
	{
		Area& area = m_areas[ areaIndex ];
		RenderArea( area, false );
	}

	// Render shadows on all Actors
	unsigned int actorIndex;
	for( actorIndex = 0; actorIndex < m_actors.size(); ++ actorIndex )
	{
		Actor& actor = m_actors[ actorIndex ];
		RenderActor( actor, true );
	}

	// Render all NPCs
	for( actorIndex = 0; actorIndex < m_actors.size(); ++ actorIndex )
	{
		Actor& actor = m_actors[ actorIndex ];
		if( !actor.m_isPlayer )
		{
			RenderActor( actor, false );
		}
	}

	// Render players (separated only so that players are drawn on top of NPCs
	for( unsigned int actorIndex = 0; actorIndex < m_actors.size(); ++ actorIndex )
	{
		Actor& actor = m_actors[ actorIndex ];
		if( actor.m_isPlayer )
		{
			RenderActor( actor, false );
		}
	}
}


//-----------------------------------------------------------------------------------------------
void Scenario::WipeClean()
{
	m_areas.clear();
	m_actors.clear();
	ChangeState( SCENARIO_STATE_INACTIVE );
}


//-----------------------------------------------------------------------------------------------
double Scenario::GetSecondsInCurrentState() const
{
	double timeNow = Clock::GetAbsoluteTimeSeconds();
	return timeNow - m_timeEnteredState;
}


//-----------------------------------------------------------------------------------------------
float Scenario::GetFractionOfSecondsInCurrentState( double benchmarkSeconds ) const
{
	double secondsInCurrentState = GetSecondsInCurrentState();
	return (float)( secondsInCurrentState / benchmarkSeconds );
}


//-----------------------------------------------------------------------------------------------
ScenarioState Scenario::ChangeState( ScenarioState newState )
{
	ScenarioState previousState = m_state;
	m_state = newState;
	m_timeEnteredState = Clock::GetAbsoluteTimeSeconds();
	return previousState;
}


//-----------------------------------------------------------------------------------------------
void Scenario::RenderArea( Area& area, bool isShadowPass )
{
	area.Draw( isShadowPass );
}


//-----------------------------------------------------------------------------------------------
void Scenario::RenderActor( Actor& actor, bool isShadowPass )
{
	actor.Draw( isShadowPass );
}


