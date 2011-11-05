//-----------------------------------------------------------------------------------------------
// TheGame.hpp
//-----------------------------------------------------------------------------------------------
#ifndef __include_TheGame__
#define __include_TheGame__

#include "AABB2.hpp"
#include "Vector2.hpp"
#include "Rgba.hpp"
#include "Clock.hpp"

class Actor;
class RelationshipToOtherActor;
class Scenario;

//-----------------------------------------------------------------------------------------------
// Global variables
extern const Rgba DEFAULT_NPC_COLOR;
extern const float DEFAULT_NPC_RADIUS;


//-----------------------------------------------------------------------------------------------
// Typedefs
typedef void (*ScenarioStartFunctionPointer)( Scenario& scenario );
typedef void (*ScenarioUpdateFunctionPointer)( Scenario& scenario, double deltaSeconds );


// Actor behaviors
/*
Player
	Move around
	Grab / release

NPC
	Follow prescribed path
	Spawning from spawnpoint ala Gauntlet
	Attraction / repulsion
	Radius trigger
	Ungroup / regroup?
	Group-based destination / coordination
	Falling into a pit
	Satisfaction upon reaching a goal
	Mimic another actor's motion (with a weight?)
	Touching/bumping trigger
	Wander, healthy
	Sick / wounded

World / areas
	Pit
	Encroaching darkness?
	Goal region
*/


/////////////////////////////////////////////////////////////////////////////////////////////////
enum ActorResponse
{
	ACTOR_RESPONSE_NONE,
	ACTOR_RESPONSE_FOLLOW_SUBJECT,
	ACTOR_RESPONSE_MIMIC_SUBJECT,
	ACTOR_RESPONSE_FLEE_SUBJECT,
	NUM_ACTOR_RESPONSES
};


/////////////////////////////////////////////////////////////////////////////////////////////////
enum AreaResponse
{
	AREA_RESPONSE_NONE,
	AREA_RESPONSE_KILL_EXPLODE,
	AREA_RESPONSE_KILL_FALL,
	AREA_RESPONSE_WIN_ASCEND,
	NUM_AREA_RESPONSES
};


/////////////////////////////////////////////////////////////////////////////////////////////////
class Area
{
public:
	Area();
	void Draw( bool isShadowPass );

	AABB2 m_bounds;
	Rgba m_color;
	float m_alpha;

	bool m_impassableToPlayer;
	bool m_impassableToNPC;

	AreaResponse m_onPlayerTouch;
	AreaResponse m_onPlayerEnter;
	AreaResponse m_onNPCTouch;
	AreaResponse m_onNPCEnter;
};


/////////////////////////////////////////////////////////////////////////////////////////////////
enum ActorState
{
	ACTOR_STATE_IDLE,
	NUM_ACTOR_STATES
};


/////////////////////////////////////////////////////////////////////////////////////////////////
class RelationshipToOtherActor
{
public:
	RelationshipToOtherActor();
//	RelationshipToOtherActor( const RelationshipToOtherActor& copyFrom );

	Actor* m_otherActor;
	float m_innerDistance;
	float m_outerDistance;
	Vector2 m_attractionRepulsionAtInnerDistance; // positive number for attraction, negative for repulsion
	Vector2 m_attractionRepulsionAtOuterDistance; // positive number for attraction, negative for repulsion
	Vector2 m_mimicMotionAtInnerDistance; // 0.5f means we move 10 feet north if they move 20 north, can be negative for counter-movement
	Vector2 m_mimicMotionAtOuterDistance; // 0.5f means we move 10 feet north if they move 20 north, can be negative for counter-movement
	double m_timeRelationshipWillExpireAtInnerDistance;
	double m_timeRelationshipWillExpireAtOuterDistance;
	Rgba m_colorAtInnerDistance;
	Rgba m_colorAtOuterDistance;
	float m_alphaScaleAtInnerDistance;
	float m_alphaScaleAtOuterDistance;
	float m_radiusScaleAtInnerDistance;
	float m_radiusScaleAtOuterDistance;
};


/////////////////////////////////////////////////////////////////////////////////////////////////
class Actor
{
public:
	Vector2 m_position;
	float m_movementSpeed;
	float m_movementHeadingDegrees;
	float m_viewHeadingDegrees;
	std::vector< RelationshipToOtherActor > m_relationships;
	bool m_isPlayer;
	Rgba m_baseColor;
	float m_baseAlpha;
	float m_alphaScaleFromRelationships;
	float m_baseRadius;
	float m_radiusScaleFromRelationships;
	ActorState m_state;
	double m_timeEnteredState;

	ActorResponse m_responseIfTouchedByNPC;
	ActorResponse m_responseIfTouchedByPlayer;
	ActorResponse m_responseIfWithinRadiusOfNPC;
	ActorResponse m_responseIfWithinRadiusOfPlayer;

	Actor();
	void Draw( bool isShadowPass ) const;
	float CalcRadius() const;
	float CalcAlpha() const;
	Rgba CalcColor() const;
	double GetSecondsInCurrentState() const;
	float GetFractionOfSecondsInCurrentState( double benchmarkSeconds ) const;
	ActorState ChangeState( ActorState newState );
	void Update( double deltaSeconds );
	void UpdateAsPlayer( double deltaSeconds );
	bool DoesStateRunPhysics( ActorState state );
	void RunPhysics( double deltaSeconds );
	void RunEmotions( double deltaSeconds );
	void RunRelationship( RelationshipToOtherActor& relationship, Actor& otherActor );
};


/////////////////////////////////////////////////////////////////////////////////////////////////
enum ScenarioState
{
	SCENARIO_STATE_INACTIVE,
	SCENARIO_STATE_INTRO,
	SCENARIO_STATE_RUNNING,
	SCENARIO_STATE_ENDING,
	NUM_SCENARIO_STATES
};


/////////////////////////////////////////////////////////////////////////////////////////////////
class Scenario
{
public:
	std::string m_name;	
	std::vector< Area > m_areas;
	std::vector< Actor > m_actors;
	ScenarioState m_state;
	double m_timeEnteredState;
	ScenarioStartFunctionPointer m_startFunction;
	ScenarioUpdateFunctionPointer m_updateFunction;

	Scenario();
	void Start();
	void Update( double deltaSeconds );
	void Render();
	void RenderArea( Area& area, bool isShadowPass );
	void RenderActor( Actor& actor, bool isShadowPass );
	void WipeClean();
	double GetSecondsInCurrentState() const;
	float GetFractionOfSecondsInCurrentState( double benchmarkSeconds ) const;
	ScenarioState ChangeState( ScenarioState newState );
};


/////////////////////////////////////////////////////////////////////////////////////////////////
class TheGame
{
public:
	TheGame();
	~TheGame();

	void Startup( const std::string& appCommandLine );
	void Shutdown();
	void RunFrame();
	void SetUpView();
	void DrawDebugGraphics();
	void Update( double deltaSeconds );
	bool IsRunning() const { return m_isRunning; }
	bool HandleWin32Message( UINT wmMessageCode, WPARAM wParam, LPARAM lParam );
	bool ProcessKeyDownEvent( unsigned char keyCode );
	bool ProcessKeyUpEvent( unsigned char keyCode );
	bool IsKeyDown( unsigned char keyCode );

	void CreateScenarios();
	void CreateScenario( const std::string& scenarioName, ScenarioStartFunctionPointer startFunction, ScenarioUpdateFunctionPointer updateFunction );
	void LoadScenarioDataFiles();
	void StartScenarioByName( const std::string& scenarioName );
	void StartScenario( Scenario* scenarioToStart );

private:
	bool m_isRunning;
	bool m_keyDownStates[ 256 ];
	std::vector< Scenario* > m_scenarios;
	Scenario* m_currentScenario;
};


extern TheGame* theGame;


#endif // __include_TheGame__
