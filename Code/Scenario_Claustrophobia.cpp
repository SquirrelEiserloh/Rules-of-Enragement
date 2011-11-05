//-----------------------------------------------------------------------------------------------
// Scenario_Claustrophobia.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_Claustrophobia.hpp"
#include "Vector2.hpp"

#define AR(n, l, t, w, h) { \
	Area *n = new Area();\
	n->m_bounds.SetFromMinXYMaxXY( l, t, l+w, t+h );\
	scenario.m_areas.push_back(n);\
	}

#define AC(n, l, t) { \
	Actor *n = new Actor();\
	n->m_position = Vector2(l, t);\
	n->m_baseColor = Rgba::WHITE;\
	scenario.m_actors.push_back(n);\
	}

//-----------------------------------------------------------------------------------------------
void ScenarioStartFunction_Claustrophobia( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );
	
	//1024x576
	Actor *player=new Actor();
	player->m_position = Vector2(100, 576/2);
	player->m_isPlayer = true;
	player->m_baseColor = Rgba::BLUE;
	scenario.m_actors.push_back( player );

	for(int i = 0; i < 11; i++) {
		for(int j = 0; j < 9; j++) {
			AC(a, (1024/2)-(i-5)*50, (576/2)+(j-4)*30+(i%2*16));
		}
	}

	for(int i = 0; i < scenario.m_actors.size(); i++) {
		Actor *a = scenario.m_actors[i];
		RelationshipToOtherActor dontBumpA;
		dontBumpA.m_innerDistance = 0;
		dontBumpA.m_outerDistance = 20;
		dontBumpA.m_attractionRepulsionAtOuterDistance = Vector2(0,0);
		dontBumpA.m_attractionRepulsionAtInnerDistance = Vector2(-5,-5);
		dontBumpA.m_otherActor = a;
		for(int j = 0; j < scenario.m_actors.size(); j++) {
			if(i == j) { continue; }
			Actor *b = scenario.m_actors[j];
			b->m_relationships.push_back(dontBumpA);
		}
	}

	AR(ar1, 0, 100, 1024, 376);

	Area* lWall = new Area;
	lWall->m_bounds.SetFromMinXYMaxXY( 0, 100, 10, 576-100);
	lWall->m_color = Rgba::DARKGREY;
	lWall->m_alpha = 1.f;
	lWall->m_impassableToNPC = true;
	lWall->m_impassableToPlayer = true;
	lWall->m_deepShadow = false;
	scenario.m_areas.push_back( lWall );
	Area* rWall = new Area;
	rWall->m_bounds.SetFromMinXYMaxXY( 1014, 10, 1024, 576-100 );
	rWall->m_color = Rgba::DARKGREY;
	rWall->m_alpha = 1.f;
	rWall->m_impassableToNPC = true;
	rWall->m_impassableToPlayer = true;
	rWall->m_deepShadow = false;
	scenario.m_areas.push_back( rWall );
	Area* tWall = new Area;
	tWall->m_bounds.SetFromMinXYMaxXY( 0, 100, 1024, 100 );
	tWall->m_color = Rgba::DARKGREY;
	tWall->m_alpha = 1.f;
	tWall->m_impassableToNPC = true;
	tWall->m_impassableToPlayer = true;
	tWall->m_deepShadow = false;
	scenario.m_areas.push_back( tWall );
	Area* bWall = new Area;
	bWall->m_bounds.SetFromMinXYMaxXY( 0, 576-100, 1024, 576-100 );
	bWall->m_color = Rgba::DARKGREY;
	bWall->m_alpha = 1.f;
	bWall->m_impassableToNPC = true;
	bWall->m_impassableToPlayer = true;
	bWall->m_deepShadow = false;
	scenario.m_areas.push_back( bWall );

	Area *pGoal=new Area();
	pGoal->m_bounds.SetFromMinXYMaxXY( 928, 576/2-64, 1024, 576/2+64 );
	pGoal->m_color = Rgba::WHITE;
	pGoal->m_alpha = 1.0f;
	scenario.m_areas.push_back( pGoal );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_Claustrophobia( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

