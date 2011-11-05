//-----------------------------------------------------------------------------------------------
// Scenario_Popularity.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_Popularity.hpp"
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
void ScenarioStartFunction_Popularity( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );
	
	//1024x576
	Actor *player=new Actor();
	player->m_position = Vector2(118, 502);
	player->m_isPlayer = true;
	player->m_baseColor = Rgba::BLUE;
	scenario.m_actors.push_back( player );

	RelationshipToOtherActor followPlayer;
	followPlayer.m_innerDistance = 75;
	followPlayer.m_outerDistance = 125;
	followPlayer.m_attractionRepulsionAtOuterDistance = Vector2(0,0);
	followPlayer.m_attractionRepulsionAtInnerDistance = Vector2(1.5f,1.5f);
	followPlayer.m_otherActor = player;

	AC(a1, 150, 374);
	AC(a2, 86, 182);
	AC(a3, 246, 54);
	AC(a4, 278, 54);
	AC(a5, 534, 86);
	AC(a6, 406, 374);
	AC(a7, 470, 374);
	AC(a8, 502, 502);
	AC(a9, 598, 438);
	AC(a10, 694, 502);
	AC(a11, 854, 502);
	AC(a12, 790, 406);
	AC(a13, 854, 406);
	AC(a14, 790, 310);
	AC(a15, 854, 310);
	AC(a16, 790, 214);
	AC(a17, 854, 214);
	AC(a18, 790, 118);
	AC(a19, 854, 118);

	for(int i = 0; i < scenario.m_actors.size(); i++) {
		Actor *a = scenario.m_actors[i];
		RelationshipToOtherActor dontBumpA;
		dontBumpA.m_innerDistance = 0;
		dontBumpA.m_outerDistance = 32;
		dontBumpA.m_attractionRepulsionAtOuterDistance = Vector2(0,0);
		dontBumpA.m_attractionRepulsionAtInnerDistance = Vector2(-2,-2);
		dontBumpA.m_otherActor = a;
		for(int j = 0; j < scenario.m_actors.size(); j++) {
			if(i == j) { continue; }
			Actor *b = scenario.m_actors[j];
			b->m_relationships.push_back(dontBumpA);
		}
	}

	AR(ar1, 64, 32, 128, 512);
	AR(ar2, 64, 32, 512, 128);
	AR(ar3, 384, 32, 128, 416);
	AR(ar4, 384, 416, 512, 128);
	AR(ar5, 768, 64, 128, 352);
	AR(ar6, 768, 32, 256, 96);

	Area *pGoal=new Area();
	pGoal->m_bounds.SetFromMinXYMaxXY( 928, 32, 1024, 32+96 );
	pGoal->m_color = Rgba::WHITE;
	pGoal->m_alpha = 1.0f;
	scenario.m_areas.push_back( pGoal );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_Popularity( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

