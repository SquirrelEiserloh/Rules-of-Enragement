//-----------------------------------------------------------------------------------------------
// Actor.cpp
//-----------------------------------------------------------------------------------------------
#include "TheGame.hpp" // for now, we've got a huge ass monolithic header
#include "Graphics.hpp"


//-----------------------------------------------------------------------------------------------
// Globals
const float g_playerAcceleration = 200000.f;
const float g_playerMaxMoveSpeedUnitsPerSecond = 100.f;
const float g_secondsToDragToStop = 0.1f;
const float g_fullMeanderMaxDegreesPerSecond = 360.f;
const double g_numberOfSecondsToFall = 3.0;


/////////////////////////////////////////////////////////////////////////////////////////////////
// RelationshipToOtherActor

//-----------------------------------------------------------------------------------------------
RelationshipToOtherActor::RelationshipToOtherActor()
	: m_otherActor( NULL )
	, m_attractionRepulsionAtInnerDistance( Vector2::ZERO )
	, m_attractionRepulsionAtOuterDistance( Vector2::ZERO )
	, m_mimicMotionAtInnerDistance( Vector2::ZERO )
	, m_mimicMotionAtOuterDistance( Vector2::ZERO )
	, m_timeRelationshipWillExpireAtInnerDistance( 0.0 )
	, m_timeRelationshipWillExpireAtOuterDistance( 0.0 )
	, m_innerDistance( 0.f )
	, m_outerDistance( 0.f )
	, m_colorAtInnerDistance( DEFAULT_NPC_COLOR )
	, m_colorAtOuterDistance( DEFAULT_NPC_COLOR )
	, m_alphaScaleAtInnerDistance( 1.f )
	, m_alphaScaleAtOuterDistance( 1.f )
	, m_radiusScaleAtInnerDistance( 1.f )
	, m_radiusScaleAtOuterDistance( 1.f )
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////
// Actor

//-----------------------------------------------------------------------------------------------
Actor::Actor()
	: m_position( Vector2::ZERO )
	, m_movementSpeed( 0.f )
	, m_movementHeadingDegrees( 0.f )
	, m_viewHeadingDegrees( 0.f )
	, m_isPlayer( false )
	, m_baseColor( DEFAULT_NPC_COLOR )
	, m_baseAlpha( 1.f )
	, m_alphaScaleFromRelationships( 1.f )
	, m_baseRadius( DEFAULT_NPC_RADIUS )
	, m_radiusScaleFromRelationships( 1.f )
	, m_meanderFactor( 0.2f )
	, m_confusionFactor( 0.0f )
	, m_state( ACTOR_STATE_ACTIVE )
	, m_timeEnteredState( 0.0 )

	, m_responseIfTouchedByNPC( ACTOR_RESPONSE_NONE )
	, m_responseIfTouchedByPlayer( ACTOR_RESPONSE_NONE )
	, m_responseIfWithinRadiusOfNPC( ACTOR_RESPONSE_NONE )
	, m_responseIfWithinRadiusOfPlayer( ACTOR_RESPONSE_NONE )
{
	m_timeEnteredState = Clock::GetAbsoluteTimeSeconds();
}


//-----------------------------------------------------------------------------------------------
double Actor::GetSecondsInCurrentState() const
{
	double timeNow = Clock::GetAbsoluteTimeSeconds();
	return timeNow - m_timeEnteredState;
}


//-----------------------------------------------------------------------------------------------
float Actor::GetFractionOfSecondsInCurrentState( double benchmarkSeconds ) const
{
	double secondsInCurrentState = GetSecondsInCurrentState();
	return (float)( secondsInCurrentState / benchmarkSeconds );
}


//-----------------------------------------------------------------------------------------------
ActorState Actor::ChangeState( ActorState newState )
{
	ActorState previousState = m_state;
	m_state = newState;
	m_timeEnteredState = Clock::GetAbsoluteTimeSeconds();
	return previousState;
}

//-----------------------------------------------------------------------------------------------
void Actor::Update( double deltaSeconds, Scenario& scenario )
{
	if( m_isPlayer )
	{
		// Do nothing; we update the player actor directly
		return;
	}

	m_previousPosition = m_position;
	RunEmotions( deltaSeconds );

	if( DoesStateRunPhysics( m_state ) )
	{
		RunPhysics( deltaSeconds, scenario );
	}
	else
	{
		if( m_state == ACTOR_STATE_FALLING )
		{
			ContinueFalling( deltaSeconds, scenario );
		}
	}
}


//-----------------------------------------------------------------------------------------------
void Actor::ContinueFalling( double deltaSeconds, Scenario& scenario )
{
	double secondsInState = Clock::GetAbsoluteTimeSeconds() - m_timeEnteredState;
	float fractionFallen = (float)( secondsInState / g_numberOfSecondsToFall );
	fractionFallen = ClampFloat( fractionFallen, 0.f, 1.f );
	m_radiusScaleFromRelationships *= (1.f - fractionFallen);
	if( fractionFallen >= 1.f )
	{
		ChangeState( ACTOR_STATE_DEAD );
	}
}


//-----------------------------------------------------------------------------------------------
void Actor::UpdateAsPlayer( double deltaSeconds, Scenario& scenario )
{
	if( !m_isPlayer )
	{
		// Do nothing; this is for player-actors only
		return;
	}

	m_previousPosition = m_position;
	if( m_state == ACTOR_STATE_ACTIVE )
	{
		Vector2 moveIntention = Vector2::ZERO;
		bool isAccelerating = false;
		if( theGame->IsKeyDown( VK_UP ) )
		{
			moveIntention += Vector2( 0.f, -g_playerAcceleration );
			isAccelerating = true;
		}
		if( theGame->IsKeyDown( VK_DOWN ) )
		{
			moveIntention += Vector2( 0.f, +g_playerAcceleration );
			isAccelerating = true;
		}
		if( theGame->IsKeyDown( VK_LEFT ) )
		{
			moveIntention += Vector2( -g_playerAcceleration, 0.f );
			isAccelerating = true;
		}
		if( theGame->IsKeyDown( VK_RIGHT ) )
		{
			moveIntention += Vector2( +g_playerAcceleration, 0.f );
			isAccelerating = true;
		}

		moveIntention.SetLength( deltaSeconds * g_playerAcceleration );
		Vector2 currentVelocity;
		currentVelocity.SetLengthAndYawDegrees( m_movementSpeed, m_movementHeadingDegrees );
		currentVelocity += (deltaSeconds * moveIntention);
		m_movementSpeed = currentVelocity.CalcLength();
		m_movementHeadingDegrees = currentVelocity.CalcYawDegrees();
		if( m_movementSpeed > g_playerMaxMoveSpeedUnitsPerSecond )
		{
			currentVelocity.SetLength( g_playerMaxMoveSpeedUnitsPerSecond );
			m_movementSpeed = currentVelocity.CalcLength();
			m_movementHeadingDegrees = currentVelocity.CalcYawDegrees();
		}

		if( moveIntention == Vector2::ZERO )
		{
			float fractionOfDragToStopTime = (float) deltaSeconds / g_secondsToDragToStop;
			m_movementSpeed -= (fractionOfDragToStopTime * g_playerMaxMoveSpeedUnitsPerSecond);
			if( m_movementSpeed < 0.f )
			{
				m_movementSpeed = 0.f;
			}
		}
	}

	if( DoesStateRunPhysics( m_state ) )
	{
		RunPhysics( deltaSeconds, scenario );
	}
	else
	{
		if( m_state == ACTOR_STATE_FALLING )
		{
			ContinueFalling( deltaSeconds, scenario );
		}
	}
}


//-----------------------------------------------------------------------------------------------
bool Actor::DoesStateRunPhysics( ActorState state )
{
	switch( state )
	{
	case ACTOR_STATE_ACTIVE:
		return true;

	case ACTOR_STATE_FALLING:
	default:
		return false;
	}
}


//-----------------------------------------------------------------------------------------------
void Actor::RunPhysics( double deltaSeconds, Scenario& scenario )
{
	// Update the actor's kinematics
	Vector2 velocity;
	velocity.SetLengthAndYawDegrees( m_movementSpeed, m_movementHeadingDegrees );
	Vector2 movement = velocity * (float) deltaSeconds;
	Vector2 proposedPosition = m_position + movement;
	m_position = proposedPosition;

	bool isInsideAtLeastOnePassableArea = false;
	unsigned int areaIndex;
	for( areaIndex = 0; areaIndex < scenario.m_areas.size(); ++ areaIndex )
	{
		Area& area = *scenario.m_areas[ areaIndex ];
		if( (m_isPlayer && area.m_impassableToPlayer) || ((!m_isPlayer) && area.m_impassableToNPC) )
		{
			scenario.ForceActorOutsideOfArea( *this, area );
		}

		if( scenario.IsActorAtAllInsideArea( *this, area ) )
		{
			isInsideAtLeastOnePassableArea = true;
		}
	}

	if( !isInsideAtLeastOnePassableArea )
	{
		StartFalling();
	}
}


//-----------------------------------------------------------------------------------------------
void Actor::Draw( bool isShadowPass ) const
{
	if( m_state == ACTOR_STATE_DEAD )
		return;

	if( isShadowPass )
	{
		const Vector2 actorShadowOffset( 3.f, 3.f );
		DrawFilledCircle( m_position + actorShadowOffset, 1.2f * CalcRadius(), Rgba::BLACK, 0.1f * CalcAlpha() );
		DrawFilledCircle( m_position + actorShadowOffset, 1.1f * CalcRadius(), Rgba::BLACK, 0.1f * CalcAlpha() );
		DrawFilledCircle( m_position + actorShadowOffset, 1.0f * CalcRadius(), Rgba::BLACK, 0.1f * CalcAlpha() );
	}
	else
	{
		DrawFilledOutlinedCircle( m_position, CalcRadius(), CalcColor(), Rgba::BLACK, CalcAlpha() );
	}
}


//-----------------------------------------------------------------------------------------------
void Actor::RunEmotions( double deltaSeconds )
{
	// Run relationships
	m_alphaScaleFromRelationships = 1.f;
	m_radiusScaleFromRelationships = 1.f;
	for( unsigned int relationshipIndex = 0; relationshipIndex < m_relationships.size(); ++ relationshipIndex )
	{
		RelationshipToOtherActor& relationship = m_relationships[ relationshipIndex ];
		if( relationship.m_otherActor )
		{
			RunRelationship( relationship, *relationship.m_otherActor, deltaSeconds );
		}
	}

	// Meandering
	if( m_meanderFactor > 0.f )
	{
	}

	// Confusion
	if( m_confusionFactor > 0.f )
	{
	}
}


//-----------------------------------------------------------------------------------------------
void Actor::RunRelationship( RelationshipToOtherActor& relationship, Actor& otherActor, double deltaSeconds )
{
	// Compute raw distance and abstract closeness parameter (1 at/less than inner distance, 0 at/greater than outer distance)
	Vector2 displacementToOther = otherActor.m_position - m_position;
	float distanceToOtherCenterToCenter = displacementToOther.CalcLength();
	float distanceToOtherEdgeToEdge = distanceToOtherCenterToCenter - (CalcRadius() + otherActor.CalcRadius());
	float closenessFactor = RangeMapFloat( relationship.m_innerDistance, relationship.m_outerDistance, distanceToOtherEdgeToEdge, 1.f, 0.f );
	closenessFactor = ClampFloat( closenessFactor, 0.f, 1.f );

	// Compute attraction / repulsion
	Vector2 attraction2d	= Interpolate( relationship.m_attractionRepulsionAtOuterDistance, relationship.m_attractionRepulsionAtInnerDistance, closenessFactor );
	Vector2 mimic2d			= Interpolate( relationship.m_mimicMotionAtOuterDistance, relationship.m_mimicMotionAtInnerDistance, closenessFactor );
	double expireAtTime		= Interpolate( relationship.m_timeRelationshipWillExpireAtOuterDistance, relationship.m_timeRelationshipWillExpireAtInnerDistance, closenessFactor );
	// TODO: color at closeness
	float alphaScale		= Interpolate( relationship.m_alphaScaleAtOuterDistance, relationship.m_alphaScaleAtInnerDistance, closenessFactor );
	float radiusScale		= Interpolate( relationship.m_radiusScaleAtOuterDistance, relationship.m_radiusScaleAtInnerDistance, closenessFactor );

	m_alphaScaleFromRelationships *= alphaScale;
	m_radiusScaleFromRelationships *= radiusScale;

	Vector2 otherActorDisplacement = otherActor.m_position - otherActor.m_previousPosition;
	Vector2 mimicDisplacement = otherActorDisplacement;
	mimicDisplacement.x *= mimic2d.x;
	mimicDisplacement.y *= mimic2d.y;
	m_position += mimicDisplacement;

	Vector2 distance = (otherActor.m_position - m_position);
	m_position += Vector2(distance.x*attraction2d.x, distance.y*attraction2d.y)*deltaSeconds;
}


//-----------------------------------------------------------------------------------------------
float Actor::CalcRadius() const
{
	return m_baseRadius * m_radiusScaleFromRelationships;
}

	
//-----------------------------------------------------------------------------------------------
float Actor::CalcAlpha() const
{
	return m_baseAlpha * m_alphaScaleFromRelationships;
}


//-----------------------------------------------------------------------------------------------
Rgba Actor::CalcColor() const
{
	return m_baseColor;
}


//-----------------------------------------------------------------------------------------------
void Actor::StartFalling()
{
	ChangeState( ACTOR_STATE_FALLING );
	m_baseColor = Rgba::WHITE;
}


