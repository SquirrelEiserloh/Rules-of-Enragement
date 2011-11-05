//-----------------------------------------------------------------------------------------------
// Scenario_SelfSacrifice.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_Schadenfreude.hpp"
#include "Vector2.hpp"

//-----------------------------------------------------------------------------------------------
void ScenarioStartFunction_Schadenfreude( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );
	
	//1024x576
	//100..412
	const float pLeft = 100;
	const float pRight = 412;
	const float pTop = 0;
	const float pBottom = 566-100;
	const float pGoalBottom = 100;

	const float aLeft = 612;
	const float aRight = 924;
	const float aBottom = 566-100;
	const float aTop = 150;
	const float aGoalTop = 0;
	const float aGoalBottom = 100;
	const float aW = aRight-aLeft;
	const float aH = aBottom-aTop;
	
	const float aOffsetY = 0;

	const int aColumns = 3;
	const int aRows = 6;

	const int aWPerA = (aW/aColumns);
	const int aHPerA = (aH/aRows);

	const float aBaseY = aBottom-aHPerA/2.f-aOffsetY;

	Actor *player=new Actor();
	player->m_position = Vector2(pLeft+(pRight-pLeft)/2., aBaseY);
	player->m_isPlayer = true;
	player->m_baseColor = Rgba::BLUE;
	scenario.m_actors.push_back( player );

	RelationshipToOtherActor followPlayer;
	followPlayer.m_mimicMotionAtOuterDistance = Vector2(1,1);
	followPlayer.m_otherActor = player;
	
	for(int i = 0; i < aColumns; i++) {
		for(int j = 0; j < aRows; j++) {
			Actor *a = new Actor();
			a->m_position = Vector2(aLeft+(aWPerA*i)+(aWPerA/2.f), aBaseY-(aHPerA*j));
			a->m_relationships.push_back(followPlayer);
			a->m_baseColor = Rgba::WHITE;
			scenario.m_actors.push_back(a);
		}
	}

	Area *aMain=new Area();
	aMain->m_bounds.SetFromMinXYMaxXY( aLeft, aBottom, aRight, aTop );
	scenario.m_areas.push_back( aMain );
	Area *aGoal=new Area();
	aGoal->m_bounds.SetFromMinXYMaxXY( aLeft, aGoalBottom, aRight, aGoalTop );
	aGoal->m_color = Rgba::WHITE;
	aGoal->m_alpha = 1.0f;
	scenario.m_areas.push_back( aGoal );
	Area *pMain=new Area();
	pMain->m_bounds.SetFromMinXYMaxXY( pLeft, pBottom, pRight, pTop );
	scenario.m_areas.push_back( pMain );
	Area *pGoal=new Area();
	pGoal->m_bounds.SetFromMinXYMaxXY( pLeft, pGoalBottom, pRight, pTop );
	pGoal->m_color = Rgba::WHITE;
	pGoal->m_alpha = 1.0f;
	scenario.m_areas.push_back( pGoal );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_Schadenfreude( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

