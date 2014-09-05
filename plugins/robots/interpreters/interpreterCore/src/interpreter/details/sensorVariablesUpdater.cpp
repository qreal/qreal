#include "sensorVariablesUpdater.h"

#include <utils/tracer.h>
#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>
#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

static int const unrealUpdateInterval = 20;
static int const realUpdateInterval = 200;

using namespace interpreterCore::interpreter::details;
using namespace interpreterBase::robotModel;

SensorVariablesUpdater::SensorVariablesUpdater(RobotModelManagerInterface const &robotModelManager
		, qrtext::DebuggerInterface &newParser
		)
	: mUpdateTimer(nullptr)
	, mRobotModelManager(robotModelManager)
	, mParser(newParser)
{
}

void SensorVariablesUpdater::run()
{
	delete mUpdateTimer;
	mUpdateTimer = mRobotModelManager.model().timeline().produceTimer();
	connect(mUpdateTimer, &utils::AbstractTimer::timeout, this, &SensorVariablesUpdater::onTimerTimeout);
	resetVariables();

	for (robotParts::Device * const device : mRobotModelManager.model().configuration().devices()) {
		/// @todo: Works only with scalar sensors
		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor && !scalarSensor->port().reservedVariable().isEmpty()) {

			if (!scalarSensor->ready()) {
				/// @todo Error reporting
				continue;
			}

			connect(
					scalarSensor
					, &robotParts::ScalarSensor::newData
					, this
					, &SensorVariablesUpdater::onScalarSensorResponse
					, Qt::UniqueConnection
					);

			connect(
					scalarSensor
					, &robotParts::AbstractSensor::failure
					, this
					, &SensorVariablesUpdater::onFailure
					, Qt::UniqueConnection
					);

			scalarSensor->read();
		}
	}

	mUpdateTimer->start(updateInterval());
}

void SensorVariablesUpdater::suspend()
{
	if (mUpdateTimer) {
		mUpdateTimer->stop();
	}
}

void SensorVariablesUpdater::onScalarSensorResponse(int reading)
{
	robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(sender());
	if (!scalarSensor) {
		/// @todo Error reporting.
		return;
	}

	updateScalarSensorVariables(scalarSensor->port(), reading);
}

void SensorVariablesUpdater::onTimerTimeout()
{
	for (robotParts::Device * const device : mRobotModelManager.model().configuration().devices()) {
		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor && !scalarSensor->port().reservedVariable().isEmpty()) {

			if (!scalarSensor->ready()) {
				/// @todo Error reporting
				continue;
			}

			scalarSensor->read();
		}
	}

	mUpdateTimer->start(updateInterval());
}

int SensorVariablesUpdater::updateInterval() const
{
	return mRobotModelManager.model().needsConnection() ? realUpdateInterval : unrealUpdateInterval;
}

void SensorVariablesUpdater::onFailure()
{
	utils::Tracer::debug(utils::Tracer::autoupdatedSensorValues, "Interpreter::slotFailure", "");
}

void SensorVariablesUpdater::updateScalarSensorVariables(PortInfo const &sensorPortInfo, int reading)
{
	updateScalarSensorVariable(sensorPortInfo.reservedVariable(), reading);
}

void SensorVariablesUpdater::updateScalarSensorVariable(QString const &variable, int reading)
{
	utils::Tracer::debug(
			utils::Tracer::autoupdatedSensorValues
			, "SensorVariablesUpdater::updateScalarSensorVariable"
			, variable + QString::number(reading)
			);

	mParser.setVariableValue(variable, reading);
}

void SensorVariablesUpdater::resetVariables()
{
	for (robotParts::Device * const device : mRobotModelManager.model().configuration().devices()) {
		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor) {
			updateScalarSensorVariables(scalarSensor->port(), 0);
		}
	}
}
