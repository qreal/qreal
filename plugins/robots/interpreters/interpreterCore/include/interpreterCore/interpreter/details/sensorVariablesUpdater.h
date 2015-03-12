#pragma once

#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>

#include <kitBase/robotModel/robotModelManagerInterface.h>

#include "interpreterCore/textLanguage/robotsBlockParser.h"

namespace utils {
class AbstractTimer;
}

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
	SensorVariablesUpdater(const kitBase::robotModel::RobotModelManagerInterface &robotModelManager
			, qrtext::DebuggerInterface &textLanguageToolbox
			);

	~SensorVariablesUpdater();

	/// Starts background polling process.
	void run();

	/// Stops background polling process.
	void suspend();

private slots:
	void onTimerTimeout();
	void onScalarSensorResponse(int reading);
	void onVectorSensorResponse(const QVector<int> &reading);
	void onFailure();

private:
	int updateInterval() const;

	void updateScalarSensorVariables(const kitBase::robotModel::PortInfo &sensorPortInfo, int reading);
	void updateScalarSensorVariable(const QString &variable, int reading);

	void updateVectorSensorVariables(const kitBase::robotModel::PortInfo &sensorPortInfo
			, const QVector<int> &reading);

	void updateVectorSensorVariable(const QString &variable, const QVector<int> &reading);

	void resetVariables();

	QScopedPointer<utils::AbstractTimer> mUpdateTimer;
	const kitBase::robotModel::RobotModelManagerInterface &mRobotModelManager;
	qrtext::DebuggerInterface &mParser;
};

}
}
}
