//-----------------------------------------------------------------------------------------------
// Clock.cpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#include "Clock.hpp"


//-----------------------------------------------------------------------------------------------
// Static member initialization
//
STATIC bool Clock::s_isInitialized = false;
STATIC double Clock::s_queryPerformanceSecondsPerTick = 0.0;
STATIC LARGE_INTEGER Clock::s_queryPerformanceStartupValue;
STATIC Clock* Clock::s_masterClock;
STATIC Clock* Clock::s_applicationClock;
STATIC unsigned int Clock::s_frameNumber;


//-----------------------------------------------------------------------------------------------
STATIC void Clock::InitializeClockSystem()
{
	LARGE_INTEGER queryPerformanceTicksPerSecond;
	QueryPerformanceFrequency( &queryPerformanceTicksPerSecond );
	s_queryPerformanceSecondsPerTick = 1.0 / (double) queryPerformanceTicksPerSecond.QuadPart;
	QueryPerformanceCounter( &s_queryPerformanceStartupValue );

	const double timeNow = GetAbsoluteTimeSeconds();
	s_masterClock = new Clock();
	s_applicationClock = new Clock( *s_masterClock );
	s_masterClock->SetCurrentTimeSeconds( timeNow );
	s_applicationClock->SetCurrentTimeSeconds( timeNow );
	s_applicationClock->SetMinimumDeltaSeconds( 0.0 );
	s_applicationClock->SetMaximumDeltaSeconds( 1.0 / 10.0 );	// Min FPS is 10 (max frametime of 100ms)
}


//-----------------------------------------------------------------------------------------------
STATIC double Clock::GetAbsoluteTimeSeconds()
{
	LARGE_INTEGER queryPerformanceCounterTime;
	QueryPerformanceCounter( &queryPerformanceCounterTime );
	queryPerformanceCounterTime.QuadPart -= s_queryPerformanceStartupValue.QuadPart;
	double seconds = s_queryPerformanceSecondsPerTick * (double) queryPerformanceCounterTime.QuadPart;
	return seconds;
}


//-----------------------------------------------------------------------------------------------
STATIC void Clock::Update()
{
	const double timeAtLastBeginFrame = s_masterClock->GetCurrentTimeSeconds();
	const double timeNow = GetAbsoluteTimeSeconds();
	const double deltaSeconds = timeNow - timeAtLastBeginFrame;
	s_masterClock->AdvanceTime( deltaSeconds );
	s_frameNumber ++;
}


//-----------------------------------------------------------------------------------------------
Clock::Clock()
: m_parentClock( NULL )
, m_currentTimeSeconds( 0.0 )
, m_deltaSeconds( 0.0 )
, m_timeScale( 1.0f )
, m_isPaused( false )
, m_minimumDeltaSeconds( 0.0 )
, m_maximumDeltaSeconds( 0.0 )
{
}


//-----------------------------------------------------------------------------------------------
Clock::Clock( Clock& parentClock )
: m_parentClock( &parentClock )
, m_currentTimeSeconds( parentClock.m_currentTimeSeconds )
, m_deltaSeconds( 0.0 )
, m_timeScale( 1.0f )
, m_isPaused( false )
, m_minimumDeltaSeconds( 0.0 )
, m_maximumDeltaSeconds( 0.0 )
{
	// TODO: Validate parentClock
	m_parentClock->AddChildClock( this );

}


//-----------------------------------------------------------------------------------------------
Clock::~Clock()
{
	// Attach all of my children to my parent
	for( unsigned int i = 0; i < m_childClocks.size(); ++ i )
	{
		m_childClocks[ i ]->m_parentClock = m_parentClock; // Child's parent is now my parent (even if that was NULL)
		if( m_parentClock )
		{
			m_parentClock->AddChildClock( m_childClocks[ i ] ); // My parent adopts this child
		}
	}

	// Remove me from my parent's child-list
	if( m_parentClock )
	{
		std::vector< Clock* >& parentsChildClocks = m_parentClock->m_childClocks;
		std::vector< Clock* >::iterator iter;
		for( iter = parentsChildClocks.begin(); iter != parentsChildClocks.end(); ++ iter )
		{
			Clock* iterClock = *iter;
			if( iterClock == this )
			{
				parentsChildClocks.erase( iter );
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------
void Clock::AddChildClock( Clock* childClock )
{
	// TODO: Validate that this clock is not already in our child list
	m_childClocks.push_back( childClock );
}


//-----------------------------------------------------------------------------------------------
STATIC void Clock::DestroyRecursive( Clock* clockTreeToDestroy )
{
	// Destroy all of my children (recursively)
	for( unsigned int i = 0; i < clockTreeToDestroy->m_childClocks.size(); ++ i )
	{
		DestroyRecursive( clockTreeToDestroy->m_childClocks[ i ] );
		clockTreeToDestroy->m_childClocks[ i ] = NULL;
	}

	clockTreeToDestroy->m_childClocks.clear();
	clockTreeToDestroy->m_parentClock = NULL;
	delete clockTreeToDestroy;
}


//-----------------------------------------------------------------------------------------------
void Clock::SetCurrentTimeSeconds( double newCurrentTimeSeconds )
{
	m_currentTimeSeconds = newCurrentTimeSeconds;
	m_deltaSeconds = 0.0; // Is this what we want?
}


//-----------------------------------------------------------------------------------------------
void Clock::AdvanceTime( double deltaSeconds )
{
	if( m_isPaused )
	{
		deltaSeconds = 0.0;
	}
	else
	{
		deltaSeconds *= (double) m_timeScale;

		if( m_minimumDeltaSeconds > 0.0 && deltaSeconds < m_minimumDeltaSeconds )
		{
			deltaSeconds = m_minimumDeltaSeconds;
		}

		if( m_maximumDeltaSeconds > 0.0 && deltaSeconds > m_maximumDeltaSeconds )
		{
			deltaSeconds = m_maximumDeltaSeconds;
		}

	}

	m_deltaSeconds = deltaSeconds;
	m_currentTimeSeconds += deltaSeconds;

	for( unsigned int i = 0; i < m_childClocks.size(); ++ i )
	{
		m_childClocks[ i ]->AdvanceTime( deltaSeconds );
	}
}


//-----------------------------------------------------------------------------------------------
// Gets the specified amount of seconds as a time string, formatted depending on <verbosity>
//	and <numberOfFractionalSecondsDigits>.
//
// verbosity can be any of [0,1,2]
//		verbosity=2 uses words like "years", etc.
//		verbosity=1 uses single letters like "y", etc.
//		verbosity=0 uses numbers packed back-to-back with colon separators
// numberOfFractionalSecondsDigits can be any of [0,1,2,3,4,5,6]
//		numberOfFractionalSecondsDigits=0 means no decimal point or fractional seconds
//		numberOfFractionalSecondsDigits=1 through 6 means forced decimal point and exact number of fractional digits afterwards
// forceLeadingZerosThroughCategory can be any of 'y','w','d','h','m','s' (as a char)
//		forceLeadingZerosThroughCategory='y' means "force leading zeros all the way out to years"
//		forceLeadingZerosThroughCategory='w' means "force leading zeros all the way out to weeks"
//		forceLeadingZerosThroughCategory='d' means "force leading zeros all the way out to days"
//		forceLeadingZerosThroughCategory='h' means "force leading zeros all the way out to hours"
//		forceLeadingZerosThroughCategory='m' means "force leading zeros all the way out to minutes"
//		forceLeadingZerosThroughCategory='s' means "force leading zeros all the way out to seconds" (this is the default)
// Examples:
//		verbosity=0, numberOfFractionalSecondsDigits=0 		"3:19:51:43"
//		verbosity=0, numberOfFractionalSecondsDigits=2 		"3:19:51:43.40"
//		verbosity=0, numberOfFractionalSecondsDigits=6 		"3:19:51:43.394233"
//		verbosity=1, numberOfFractionalSecondsDigits=0 		"3d 19h 51m 43s"
//		verbosity=1, numberOfFractionalSecondsDigits=2 		"3d 19h 51m 43.40s"
//		verbosity=2, numberOfFractionalSecondsDigits=0 		"3 days, 19 hours, 51 minutes, 43 seconds"
//		verbosity=2, numberOfFractionalSecondsDigits=2 		"3 days, 19 hours, 51 minutes, 43.40 seconds"
//
// In any event, "years" will only be shown if the time includes at least one whole year;
//	likewise with weeks, days, minutes.  Seconds are always shown; zero seconds looks like:
//		verbosity=0, numberOfFractionalSecondsDigits=0 		":00"
//		verbosity=0, numberOfFractionalSecondsDigits=2 		":00.00"
//		verbosity=0, numberOfFractionalSecondsDigits=6 		":00.000000"
//		verbosity=1, numberOfFractionalSecondsDigits=0 		"0s"
//		verbosity=1, numberOfFractionalSecondsDigits=2 		"0.00s"
//		verbosity=2, numberOfFractionalSecondsDigits=0 		"0 seconds"
//		verbosity=2, numberOfFractionalSecondsDigits=2 		"0.00 seconds"
//-----------------------------------------------------------------------------------------------
STATIC const std::string Clock::GetTimeAsString( double totalSeconds, int verbosity, int numberOfFractionalSecondsDigits, char forceLeadingZerosThroughCategory )
{
	const int wholeSeconds = (int) totalSeconds;
	const int wholeMinutes = wholeSeconds / 60;
	const int wholeHours = wholeMinutes / 60;
	const int wholeDays = wholeHours / 24;
	const int wholeWeeks = wholeDays / 7;
	const int wholeYears = wholeWeeks / 52; // close enough as we don't really care beyond this point

	const int numYears = wholeYears;
	const int numWeeks = wholeWeeks - (wholeYears * 52);
	const int numDays = wholeDays - (wholeWeeks * 7);
	const int numHours = wholeHours - (wholeDays * 24);
	const int numMinutes = wholeMinutes - (wholeHours * 60);
	const int numSeconds = wholeSeconds - (wholeMinutes * 60);
	const float fractionalSeconds = (float)(totalSeconds - (double)wholeSeconds);
	const float floatSeconds = fractionalSeconds + (float)numSeconds;

	const bool showYears = (numYears > 0) || forceLeadingZerosThroughCategory == 'y';
	const bool showWeeks = showYears || (numWeeks > 0) || forceLeadingZerosThroughCategory == 'w';
	const bool showDays = showWeeks || (numDays > 0) || forceLeadingZerosThroughCategory == 'd';
	const bool showHours = showDays || (numHours > 0) || forceLeadingZerosThroughCategory == 'h';
	const bool showMinutes = showHours || (numMinutes > 0) || forceLeadingZerosThroughCategory == 'm';

	std::string entireString;
	if( showYears )
	{
		if( verbosity >= 2 )
			if( numYears == 1 )
				entireString += Stringf( "%d year, ", numYears );
			else
				entireString += Stringf( "%d years, ", numYears );
		else if( verbosity == 1 )
			entireString += Stringf( "%dy ", numYears );
		else
			entireString += Stringf( "%d:", numYears );
	}

	if( showWeeks )
	{
		if( verbosity >= 2 )
			if( numWeeks == 1 )
				entireString += Stringf( "%d week, ", numWeeks );
			else
				entireString += Stringf( "%d weeks, ", numWeeks );
		else if( verbosity == 1 )
			entireString += Stringf( "%dw ", numWeeks );
		else
		{
			entireString += Stringf( "%02d:", numWeeks );
		}
	}

	if( showDays )
	{
		if( verbosity >= 2 )
			if( numDays == 1 )
				entireString += Stringf( "%d day, ", numDays );
			else
				entireString += Stringf( "%d days, ", numDays );
		else if( verbosity == 1 )
			entireString += Stringf( "%dd ", numDays );
		else
		{
			entireString += Stringf( "%d:", numDays );
		}
	}

	if( showHours )
	{
		if( verbosity >= 2 )
			if( numHours == 1 )
				entireString += Stringf( "%d hour, ", numHours );
			else
				entireString += Stringf( "%d hours, ", numHours );
		else if( verbosity == 1 )
			entireString += Stringf( "%dh ", numHours );
		else
		{
			entireString += Stringf( "%02d:", numHours );
		}
	}

	if( showMinutes )
	{
		if( verbosity >= 2 )
			if( numMinutes == 1 )
				entireString += Stringf( "%d minute, ", numMinutes );
			else
				entireString += Stringf( "%d minutes, ", numMinutes );
		else if( verbosity == 1 )
			entireString += Stringf( "%dm ", numMinutes );
		else
		{
			entireString += Stringf( "%02d:", numMinutes );
		}
	}

	// Always show seconds
	if( verbosity >= 2 )
		if( numSeconds == 1 && numberOfFractionalSecondsDigits < 1 )
			entireString += Stringf( "%.*f second", numberOfFractionalSecondsDigits, floatSeconds );
		else
			entireString += Stringf( "%.*f seconds", numberOfFractionalSecondsDigits, floatSeconds );
	else if( verbosity == 1 )
		entireString += Stringf( "%.*fs", numberOfFractionalSecondsDigits, floatSeconds );
	else
	{
		if( !showMinutes )
			entireString += ":";

		int width = numberOfFractionalSecondsDigits + 3;
		if( numberOfFractionalSecondsDigits == 0 )
			width = 2;

		entireString += Stringf( "%0*.*f", width, numberOfFractionalSecondsDigits, floatSeconds );
	}

	return entireString;
}


//-----------------------------------------------------------------------------------------------
void Clock::SetParent( Clock& newParentClock )
{
	if( m_parentClock )
	{
		m_parentClock->RemoveChildClock( this );
	}

	newParentClock.AddChildClock( this );
}


//-----------------------------------------------------------------------------------------------
void Clock::RemoveChildClock( Clock* childClockToRemove )
{
	std::vector< Clock* >::iterator childClockIter;
	for( childClockIter = m_childClocks.begin(); childClockIter != m_childClocks.end(); ++ childClockIter )
	{
		Clock* childClock = *childClockIter;
		if( childClock == childClockToRemove )
		{
			m_childClocks.erase( childClockIter );
			return;
		}
	}
}


