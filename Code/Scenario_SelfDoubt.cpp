//-----------------------------------------------------------------------------------------------
// Scenario_SelfDoubt.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_SelfDoubt.hpp"


//-----------------------------------------------------------------------------------------------
void ScenarioStartFunction_SelfDoubt( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );
	
	//1024x576

	Actor *left=new Actor();
	left->m_position = Vector2( 412.f, 50.f );
	left->m_isPlayer = true;
	left->m_baseColor = Rgba::WHITE;
	scenario.m_actors.push_back( left );
	
	Actor *right=new Actor();
	right->m_position = Vector2( 612.f, 50.f );
	right->m_isPlayer = true;
	right->m_baseColor = Rgba::WHITE;
	scenario.m_actors.push_back( right );

	Actor *player=new Actor();
	player->m_position = Vector2( 512.f, 100.f );
	player->m_isPlayer = true;
	player->m_baseColor = Rgba::BLUE;
	scenario.m_actors.push_back( player );

	Area *zig1=new Area();
	zig1->m_bounds.SetFromMinXYMaxXY( 0.f, 376.f, 1024.f, 576.f );
	scenario.m_areas.push_back( zig1 );
	Area *zig2=new Area();
	zig2->m_bounds.SetFromMinXYMaxXY( 200.f, 176.f, 824.f, 376.f );
	scenario.m_areas.push_back( zig2 );
	Area *zig3=new Area();
	zig3->m_bounds.SetFromMinXYMaxXY( 400.f, -176.f, 624.f, 176.f );
	scenario.m_areas.push_back( zig3 );
	Area *zig4=new Area();
	zig4->m_bounds.SetFromMinXYMaxXY( 470.f, -576.f, 554.f,-376.f );
	scenario.m_areas.push_back( zig4 );
	Area *zig5=new Area();
	zig5->m_bounds.SetFromMinXYMaxXY( 624.f, -76.f, 554.f,76.f );
	scenario.m_areas.push_back( zig5 );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_SelfDoubt( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

