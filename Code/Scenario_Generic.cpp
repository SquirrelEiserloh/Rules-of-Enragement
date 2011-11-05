//-----------------------------------------------------------------------------------------------
// Scenario_Generic.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_Generic.hpp"


//-----------------------------------------------------------------------------------------------
void ScenarioStartFunction_Generic( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );

	Actor testActor;
	testActor.m_position = Vector2( 150.f, 350.f );
	scenario.m_actors.push_back( testActor );

	testActor.m_position = Vector2( 180.f, 380.f );
	scenario.m_actors.push_back( testActor );

	Actor player;
	player.m_position = Vector2( 250.f, 300.f );
	player.m_isPlayer = true;
	player.m_baseColor = Rgba::PURPLE;
	scenario.m_actors.push_back( player );

	Area testArea;
	testArea.m_bounds.SetFromMinXYMaxXY( 100.f, 150.f, 400.f, 500.f );
	scenario.m_areas.push_back( testArea );
	testArea.m_bounds.SetFromMinXYMaxXY( 400.f, 250.f, 600.f, 350.f );
	scenario.m_areas.push_back( testArea );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_Generic( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

