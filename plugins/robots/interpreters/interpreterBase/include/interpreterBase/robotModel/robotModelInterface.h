#pragma once

#include <QtCore/QObject>

namespace interpreterBase {
namespace robotModel {

class RobotModelInterface : public QObject
{
	Q_OBJECT

public:
	virtual void init() = 0;
	virtual void stopRobot() = 0;
	virtual void disconnectFromRobot() = 0;

	virtual bool needsConnection() const = 0;

signals:
	void sensorsConfigured();
	void connected(bool success);

	/// Emitted when robot is disconnected.
	void disconnected();
};

}
}
