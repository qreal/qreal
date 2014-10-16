#pragma once

#include "qrgui/controller/controllerDeclSpec.h"

namespace qReal {
namespace commands {

/// A helper class for commands that track some continious user operation
class QRGUI_CONTROLLER_EXPORT TrackingEntity
{
public:
	virtual ~TrackingEntity() {}

	/// Executed when user starts some operation
	virtual void startTracking();

	/// Executed when user has finished some operation
	virtual void stopTracking();

	/// Executed when for some reason this command mustn`t handle user`s operation
	virtual void rejectTracking();

protected:
	bool mTrackStarted;
	bool mTrackStopped;
};

}
}
