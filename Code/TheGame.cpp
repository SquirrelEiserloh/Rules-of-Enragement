//-----------------------------------------------------------------------------------------------
// TheGame.cpp
//-----------------------------------------------------------------------------------------------
#include "TheGame.hpp"
#include "Main_Win32.hpp"
#include "Graphics.hpp"
#include "Scenario_Generic.hpp"
#include "Scenario_SelfDoubt.hpp"
#include "Scenario_SelfSacrifice.hpp"
#include "Scenario_Popularity.hpp"
#include "Scenario_Schadenfreude.hpp"
#include "Scenario_Responsibility.hpp"
#include "Scenario_Claustrophobia.hpp"


//-----------------------------------------------------------------------------------------------
// Global variables
TheGame* theGame = NULL;
const Rgba DEFAULT_NPC_COLOR = Rgba::GREEN;
const float DEFAULT_NPC_RADIUS = 10.f;


//-----------------------------------------------------------------------------------------------
TheGame::TheGame()
	: m_isRunning( true )
	, m_currentScenario( NULL )
{
}


//-----------------------------------------------------------------------------------------------
TheGame::~TheGame()
{
}


//-----------------------------------------------------------------------------------------------
void TheGame::Startup( const std::string& appCommandLine )
{
	UNUSED( appCommandLine );
	DebuggerPrintf( "TheGame::Startup...\n" );

	Clock::InitializeClockSystem();
	InitGraphics();
	for( unsigned int i = 0; i < 256; ++ i )
	{
		m_keyDownStates[ i ] = false;
	}

	CreateScenarios();
	StartScenarioByName( "Claustrophobia" );
}

//-----------------------------------------------------------------------------------------------
void TheGame::CreateScenarios()
{
	CreateScenario( "SelfDoubt", ScenarioStartFunction_SelfDoubt, ScenarioUpdateFunction_SelfDoubt );
	CreateScenario( "SelfSacrifice", ScenarioStartFunction_SelfSacrifice, ScenarioUpdateFunction_SelfSacrifice );
	CreateScenario( "Popularity", ScenarioStartFunction_Popularity, ScenarioUpdateFunction_Popularity );
	CreateScenario( "Schadenfreude", ScenarioStartFunction_Schadenfreude, ScenarioUpdateFunction_Schadenfreude );
	CreateScenario( "Responsibility", ScenarioStartFunction_Responsibility, ScenarioUpdateFunction_Responsibility );
	CreateScenario( "Claustrophobia", ScenarioStartFunction_Claustrophobia, ScenarioUpdateFunction_Claustrophobia );
}

//-----------------------------------------------------------------------------------------------
void TheGame::Shutdown()
{
	DebuggerPrintf( "TheGame::Shutdown...\n" );
}


//-----------------------------------------------------------------------------------------------
// Returns true if the app is ready to exit.
//
void TheGame::RunFrame()
{
	SetUpView();
	DrawDebugGraphics();

	static double timeLastFrameBegan = Clock::GetAbsoluteTimeSeconds();
	double timeNow = Clock::GetAbsoluteTimeSeconds();
	double deltaSeconds = timeNow - timeLastFrameBegan;
	timeLastFrameBegan = timeNow;

	Update( deltaSeconds );

	Sleep( 1 );
	SwapBuffers( g_displayDeviceContext );
}


//-----------------------------------------------------------------------------------------------
void TheGame::SetUpView()
{
	glLoadIdentity();
	glOrtho( 0.f, 1024.f, 576.f, 0.f, -1.f, 1.f );
	//	glClearColor( 1.f, 1.f, 1.f, 1.f );
	glClearColor( 0.1f, 0.3f, 1.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//-----------------------------------------------------------------------------------------------
void TheGame::DrawDebugGraphics()
{
/*
	glColor3f( 1.f, 0.f, 0.f );
	glBegin( GL_LINES );
	{
		glVertex3f( -.8f, -.8f, 0.f );
		glVertex3f( .8f, .8f, 0.f );

		glVertex3f( 10.f, 10.f, 0.f );
		glVertex3f( 1014.f, 566.f, 0.f );
	}
	glEnd();

	DrawFilledCircle( Vector2( 100.f, 100.f ), 30.f, Rgba::RED );
	DrawOutlinedCircle( Vector2( 100.f, 100.f ), 30.f, Rgba::BLACK );
	DrawFilledOutlinedCircle( Vector2( 200.f, 200.f ), 10.f, Rgba::BLUE, Rgba::BLACK, 0.5f );

	AABB2 testArea( 400.f, 100.f, 600.f, 500.f );
	DrawFilledOutlinedArea( testArea, Rgba::GREEN, Rgba::BLACK, 0.2f );
*/
}


//-----------------------------------------------------------------------------------------------
void TheGame::Update( double deltaSeconds )
{
	if( m_currentScenario )
	{
		m_currentScenario->Update( deltaSeconds );
		m_currentScenario->Render();
	}
}


//-----------------------------------------------------------------------------------------------
bool TheGame::HandleWin32Message( UINT wmMessageCode, WPARAM wParam, LPARAM lParam )
{
	UNUSED( lParam );

	unsigned char asKey = (unsigned char) wParam;
	switch( wmMessageCode )
	{
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			m_isRunning = false;
			return true;

		case WM_KEYDOWN:			
			return ProcessKeyDownEvent( asKey );

		case WM_KEYUP:
			return ProcessKeyUpEvent( asKey );

		default:
			return false;
	};
}


//-----------------------------------------------------------------------------------------------
bool TheGame::ProcessKeyDownEvent( unsigned char keyCode )
{
	m_keyDownStates[ keyCode ] = true;

	if( keyCode == VK_ESCAPE )
	{
		m_isRunning = false;
		return true;
	}

	DebuggerPrintf( "KeyDown for #%d\n", keyCode );

	return false;
}


//-----------------------------------------------------------------------------------------------
bool TheGame::ProcessKeyUpEvent( unsigned char keyCode )
{
	m_keyDownStates[ keyCode ] = false;
	return false;
}


//-----------------------------------------------------------------------------------------------
bool TheGame::IsKeyDown( unsigned char keyCode )
{
	return m_keyDownStates[ keyCode ];
}


//-----------------------------------------------------------------------------------------------
void TheGame::CreateScenario( const std::string& scenarioName, ScenarioStartFunctionPointer startFunction, ScenarioUpdateFunctionPointer updateFunction )
{
	Scenario* newScenario = new Scenario();
	newScenario->m_name = scenarioName;
	newScenario->m_startFunction = startFunction;
	newScenario->m_updateFunction = updateFunction;
	m_scenarios.push_back( newScenario );
}


//-----------------------------------------------------------------------------------------------
void TheGame::StartScenarioByName( const std::string& scenarioName )
{
	for( unsigned int scenarioIndex = 0; scenarioIndex < m_scenarios.size(); ++ scenarioIndex )
	{
		Scenario& thisScenario = *m_scenarios[ scenarioIndex ];
		if( !Stricmp( scenarioName, thisScenario.m_name ) )
		{
			StartScenario( &thisScenario );
			return;
		}
	}

	StartScenario( NULL );
}


//-----------------------------------------------------------------------------------------------
void TheGame::StartScenario( Scenario* scenarioToStart )
{
	if( m_currentScenario )
	{
		m_currentScenario->WipeClean();
	}

	m_currentScenario = scenarioToStart;

	if( m_currentScenario )
	{
		m_currentScenario->Start();
	}
}


//-----------------------------------------------------------------------------------------------
void TheGame::LoadScenarioDataFiles()
{
	// NOTE: not yet supported!  (If it's just Squirrel & Joe authoring scenarios, we're punting on data files and just hardcoding scenarios in big init functions)
}


//-----------------------------------------------------------------------------------------------
Vector2 FindClosestPointInBoundsToTarget( const AABB2& bounds, const Vector2& target, bool allowResultsWithinBounds )
{
	if( target.x < bounds.mins.x )
	{
		if( target.y < bounds.mins.y )
		{
			return bounds.mins;
		}
		else if( target.y > bounds.maxs.y )
		{
			return Vector2( bounds.mins.x, bounds.maxs.y );
		}
		else
		{
			return Vector2( bounds.mins.x, target.y );
		}
	}
	else if( target.x > bounds.maxs.x )
	{
		if( target.y < bounds.mins.y )
		{
			return Vector2( bounds.maxs.x, bounds.mins.y );
		}
		else if( target.y > bounds.maxs.y )
		{
			return bounds.maxs;
		}
		else
		{
			return Vector2( bounds.maxs.x, target.y );
		}
	}
	else
	{
		if( target.y < bounds.mins.y )
		{
			return Vector2( target.x, bounds.mins.y );
		}
		else if( target.y > bounds.maxs.y )
		{
			return Vector2( target.x, bounds.maxs.y );
		}
		else
		{
			if( allowResultsWithinBounds )
			{
				return target;
			}
			else
			{
				float distanceFromLeft = target.x - bounds.mins.x;
				float distanceFromRight = bounds.maxs.x - target.x;
				float distanceFromTop = target.y - bounds.mins.y;
				float distanceFromBottom = bounds.maxs.y - target.y;
				float distanceFromLeftOrRight = MinFloat( distanceFromLeft, distanceFromRight );
				float distanceFromTopOrBottom = MinFloat( distanceFromTop, distanceFromBottom );
				float distanceFromAnyEdge = MinFloat( distanceFromLeftOrRight, distanceFromTopOrBottom );
				if( distanceFromAnyEdge == distanceFromLeft )
				{
					return Vector2( bounds.mins.x, target.y );
				}
				else if( distanceFromAnyEdge == distanceFromRight )
				{
					return Vector2( bounds.maxs.x, target.y );
				}
				else if( distanceFromAnyEdge == distanceFromTop )
				{
					return Vector2( target.x, bounds.mins.y );
				}
				else
				{
					return Vector2( target.x, bounds.maxs.y );
				}
			}
		}
	}
}


