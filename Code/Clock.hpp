//-----------------------------------------------------------------------------------------------
// Clock.hpp
//
// Copyright 2008 Jazz Game Technologies.  See Common/Documentation/License.txt for usage
//	and license details.
//-----------------------------------------------------------------------------------------------
#ifndef __include_Clock__
#define __include_Clock__
#pragma once
#include "Utilities.hpp"


/////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Clock
//
/////////////////////////////////////////////////////////////////////////////////////////////////
class Clock
{
private:
	Clock* m_parentClock;
	std::vector< Clock* > m_childClocks;
	double m_currentTimeSeconds;
	double m_deltaSeconds;
	float m_timeScale;
	bool m_isPaused;
	double m_minimumDeltaSeconds; // if nonzero, this clock clamps incoming deltas from its parent
	double m_maximumDeltaSeconds; // if nonzero, this clock clamps incoming deltas from its parent

private:
	static bool s_isInitialized;
	static double s_queryPerformanceSecondsPerTick;
	static LARGE_INTEGER s_queryPerformanceStartupValue;
	static Clock* s_masterClock;
	static Clock* s_applicationClock;
	static unsigned int s_frameNumber;

private:
	static void DestroyRecursive( Clock* clockTreeToDestroy );

private:
	Clock();
	void AddChildClock( Clock* childClock );
	void RemoveChildClock( Clock* childClockToRemove );

public:
	static void InitializeClockSystem();
	static Clock& GetMasterClock() { return *s_masterClock; }
	static Clock& GetApplicationClock() { return *s_applicationClock; }

public:
	Clock( Clock& parentClock );
	virtual ~Clock();
	static void Update();
	static double GetAbsoluteTimeSeconds();
	double GetCurrentTimeSeconds() const { return m_currentTimeSeconds; }
	double GetDeltaSeconds() const { return( m_deltaSeconds ); }
	float GetTimeScale() const { return m_timeScale; }
	bool IsPaused() const { return m_isPaused; }
	void SetCurrentTimeSeconds( double newCurrentTimeSeconds );
	void Pause() { m_isPaused = true; }
	void UnPause() { m_isPaused = false; }
	void SetTimeScale( float newTimeScale ) { m_timeScale = newTimeScale; }
	void AdvanceTime( double deltaSeconds );
	void SetMinimumDeltaSeconds( double newMinimumDeltaSeconds ) { m_minimumDeltaSeconds = newMinimumDeltaSeconds; }
	void SetMaximumDeltaSeconds( double newMaximumDeltaSeconds ) { m_maximumDeltaSeconds = newMaximumDeltaSeconds; }
	void SetParent( Clock& newParentClock );

	static const std::string GetTimeAsString( double totalSeconds, int verbosity = 0, int numberOfFractionalSecondsDigits = 0, char forceLeadingZerosThroughCategory = 's' );
};


#endif // __include_Clock__
