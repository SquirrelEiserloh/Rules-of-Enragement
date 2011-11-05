//-----------------------------------------------------------------------------------------------
// Scenario_SelfSacrifice.cpp
//-----------------------------------------------------------------------------------------------
#include "Scenario_SelfSacrifice.hpp"
#include "Vector2.hpp"

//-----------------------------------------------------------------------------------------------
void ScenarioStartFunction_SelfSacrifice( Scenario& scenario )
{
	DebuggerPrintf( "Starting scenario '%s'...\n", scenario.m_name.c_str() );
	
	//1024x576
	//100..412
	const float pLeft = 100;
	const float pRight = 412;
	const float pBottom = 566-100;
	const float pTop = 150;
	const float pGoalTop = 0;
	const float pGoalBottom = 100;

	const float aLeft = 612;
	const float aRight = 924;
	const float aW = aRight-aLeft;
	const float aTop = 0;
	const float aBottom = 566-100;
	const float aGoalBottom = 100;
	const float aH = aBottom-aGoalBottom;
	
	const float aOffsetY = 72;

	const int aColumns = 3;
	const int aRows = 6;

	const int aWPerA = (aW/aColumns);
	const int aHPerA = (aH/(aRows+3));

	const float aBaseY = aBottom-aHPerA/2.f-aOffsetY;

	Actor *player=new Actor();
	player->m_position = Vector2(pLeft+(pRight-pLeft)/2., aBaseY+(pTop-pGoalBottom));
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

	Area *pMain=new Area();
	pMain->m_bounds.SetFromMinXYMaxXY( pLeft, pBottom, pRight, pTop );
	scenario.m_areas.push_back( pMain );
	Area *pGoal=new Area();
	pGoal->m_bounds.SetFromMinXYMaxXY( pLeft, pGoalBottom, pRight, pGoalTop );
	pGoal->m_color = Rgba::WHITE;
	pGoal->m_alpha = 1.0f;
	scenario.m_areas.push_back( pGoal );
	Area *aMain=new Area();
	aMain->m_bounds.SetFromMinXYMaxXY( aLeft, aBottom, aRight, aTop );
	scenario.m_areas.push_back( aMain );
	Area *aGoal=new Area();
	aGoal->m_bounds.SetFromMinXYMaxXY( aLeft, aGoalBottom, aRight, aTop );
	aGoal->m_color = Rgba::WHITE;
	aGoal->m_alpha = 1.0f;
	scenario.m_areas.push_back( aGoal );
}


//-----------------------------------------------------------------------------------------------
void ScenarioUpdateFunction_SelfSacrifice( Scenario& scenario, double deltaSeconds )
{
//	DebuggerPrintf( "Updating scenario '%s'...\n", scenario.m_name.c_str() );
}

