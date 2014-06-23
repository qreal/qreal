#pragma once

#include <QtCore/QTimer>
#include <QtCore/QObject>

#include <interpreterBase/robotModel/robotModelManagerInterface.h>
#include <qrutils/expressionsParser/expressionsParser.h>

namespace interpreterCore {
namespace interpreter {
namespace details {

/// Keeps sensor variables available from math expressions in a program up to date, by querying robot model.
/// Expects that model does not change and sensors are not reconfigured when updating is active.
class SensorVariablesUpdater : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param robotModelManager - has reference to current robot model.
	/// @param parser - contains sensor variables and is needed here to update them.
	SensorVariablesUpdater(
			interpreterBase::robotModel::RobotModelManagerInterface const &robotModelManager
			, utils::ExpressionsParser &parser  /// @todo direct dependency from ExpressionsParser shall be removed.
			);

	/// Starts background polling process.
	void run();

	/// Stops background polling process.
	void suspend();

private slots:
	void onTimerTimeout();
	void onScalarSensorResponse(int reading);
	void onFailure();

private:
	void updateScalarSensorVariables(interpreterBase::robotModel::PortInfo const &sensorPortInfo, int reading);
	void updateScalarSensorVariable(QString const &variable, int reading);
	void resetVariables();

	QTimer mUpdateTimer;
	interpreterBase::robotModel::RobotModelManagerInterface const &mRobotModelManager;
	utils::ExpressionsParser &mParser;
};

}
}
}
