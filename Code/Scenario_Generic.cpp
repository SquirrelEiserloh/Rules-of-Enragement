//-----------------------------------------------------------------------------------------------
// Scenario_Generic.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_Generic.hpp"


//-----------------------------------------------------------------------------------------------
void ScenarioStartFunction_Generic( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );

	Actor* testActor = new Actor();
	testActor->m_position = Vector2( 150.f, 350.f );
	scenario.m_actors.push_back( testActor );

	Actor* player = new Actor();
	player->m_position = Vector2( 250.f, 300.f );
	player->m_isPlayer = true;
	player->m_baseColor = Rgba::PURPLE;
	scenario.m_actors.push_back( player );

	RelationshipToOtherActor mimicPlayerSomewhat;
	mimicPlayerSomewhat.m_otherActor = player;
	mimicPlayerSomewhat.m_mimicMotionAtOuterDistance = Vector2( 0.5f, 0.5f );

	Actor* testActor2 = new Actor();
	testActor2->m_baseColor	= Rgba::GOLD;
	testActor2->m_position = Vector2( 180.f, 380.f );
	testActor2->m_relationships.push_back( mimicPlayerSomewhat );
	scenario.m_actors.push_back( testActor2 );

	Area* testArea = new Area;
	testArea->m_bounds.SetFromMinXYMaxXY( 100.f, 150.f, 400.f, 500.f );
	scenario.m_areas.push_back( testArea );

	Area* testArea2 = new Area;
	testArea2->m_bounds.SetFromMinXYMaxXY( 400.f, 250.f, 600.f, 350.f );
	scenario.m_areas.push_back( testArea2 );

	Area* testImpedement = new Area;
	testImpedement->m_bounds.SetFromMinXYMaxXY( 210.f, 170.f, 220.f, 380.f );
	testImpedement->m_color = Rgba::DARKGREY;
	testImpedement->m_alpha = 1.f;
	testImpedement->m_impassableToNPC = true;
	testImpedement->m_impassableToPlayer = true;
	testImpedement->m_deepShadow = false;
	scenario.m_areas.push_back( testImpedement );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_Generic( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

