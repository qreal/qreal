#pragma once

#include <QtCore/QObject>

#include "interpreterBase/robotModel/configurationInterface.h"
#include "interpreterBase/robotModel/portInfo.h"
#include "interpreterBase/robotModel/robotParts/pluggableDevice.h"
#include "interpreterBase/robotModel/robotParts/brick.h"
#include "interpreterBase/robotModel/robotParts/display.h"

#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {

class ROBOTS_INTERPRETER_BASE_EXPORT RobotModelInterface : public QObject
{
	Q_OBJECT

public:
	virtual void init() = 0;
	virtual void stopRobot() = 0;
	virtual void disconnectFromRobot() = 0;

	virtual bool needsConnection() const = 0;

	virtual ConfigurationInterface &configuration() = 0;  // TODO: Don't like this, breaks incapsulation.

	virtual robotParts::Brick &brick() = 0;
	virtual robotParts::Display &display() = 0;

signals:
	void connected(bool success);

	/// Emitted when robot is disconnected.
	void disconnected();
};

}
}
