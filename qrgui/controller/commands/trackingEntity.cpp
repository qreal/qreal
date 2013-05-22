#include "trackingEntity.h"

using namespace qReal::commands;

void TrackingEntity::startTracking()
{
	mTrackStarted = true;
	mTrackStopped = false;
}

void TrackingEntity::stopTracking()
{
	mTrackStarted = false;
	mTrackStopped = true;
}

void TrackingEntity::rejectTracking()
{
	mTrackStarted = false;
	mTrackStopped = false;
}
