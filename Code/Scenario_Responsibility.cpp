//-----------------------------------------------------------------------------------------------
// Scenario_Responsibility.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_Responsibility.hpp"
#include "Vector2.hpp"

#define AR(n, l, t, w, h) { \
	Area *n = new Area();\
	n->m_bounds.SetFromMinXYMaxXY( l, t, l+w, t+h );\
	scenario.m_areas.push_back(n);\
	}

#define AC(n, l, t) { \
	Actor *n = new Actor();\
	n->m_position = Vector2(l, t);\
	n->m_relationships.push_back(followPlayer);\
	n->m_baseColor = Rgba::WHITE;\
	scenario.m_actors.push_back(n);\
	}

//-----------------------------------------------------------------------------------------------
void ScenarioStartFunction_Responsibility( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );
	
	//1024x576
	Actor *player=new Actor();
	player->m_position = Vector2(200, 200);
	player->m_isPlayer = true;
	player->m_baseColor = Rgba::BLUE;
	scenario.m_actors.push_back( player );

	RelationshipToOtherActor followPlayer;
	followPlayer.m_attractionRepulsionAtOuterDistance = Vector2(0.75f, 0.75f);
	followPlayer.m_attractionRepulsionAtInnerDistance = Vector2(-0.25f, -0.25f);
	followPlayer.m_outerDistance = 256;
	followPlayer.m_innerDistance = 16;
	followPlayer.m_otherActor = player;

	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++) {
			if(i == 2 && j == 2) { continue; }
			AC(a, player->m_position.x-(i-2)*30, player->m_position.y-(j-2)*30);
		}
	}

	for(int i = 0; i < scenario.m_actors.size(); i++) {
		Actor *a = scenario.m_actors[i];
		RelationshipToOtherActor dontBumpA;
		dontBumpA.m_innerDistance = 0;
		dontBumpA.m_outerDistance = 64;
		dontBumpA.m_attractionRepulsionAtOuterDistance = Vector2(0,0);
		dontBumpA.m_attractionRepulsionAtInnerDistance = Vector2(-3,-3);
		dontBumpA.m_otherActor = a;
		for(int j = 0; j < scenario.m_actors.size(); j++) {
			if(i == j) { continue; }
			Actor *b = scenario.m_actors[j];
			b->m_relationships.push_back(dontBumpA);
		}
	}

	AR(ar1, 0, 0, 1024, 576);

	Area *pGoal=new Area();
	pGoal->m_bounds.SetFromMinXYMaxXY( 928, 32, 1024, 32+96 );
	pGoal->m_color = Rgba::WHITE;
	pGoal->m_alpha = 1.0f;
	scenario.m_areas.push_back( pGoal );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_Responsibility( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

