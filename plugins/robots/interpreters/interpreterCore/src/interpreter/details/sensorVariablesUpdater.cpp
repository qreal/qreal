#include "sensorVariablesUpdater.h"

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

static int const variableUpdateIntervalMs = 200;

using namespace interpreterCore::interpreter::details;
using namespace interpreterBase::robotModel;

SensorVariablesUpdater::SensorVariablesUpdater(
		RobotModelManagerInterface const &robotModelManager
		, utils::ExpressionsParser &parser
		)
	: mRobotModelManager(robotModelManager)
	, mParser(parser)
{
	mUpdateTimer.setInterval(variableUpdateIntervalMs);
	connect(&mUpdateTimer, &QTimer::timeout, this, &SensorVariablesUpdater::onTimerTimeout);
}

void SensorVariablesUpdater::run()
{
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

	mUpdateTimer.start();
}

void SensorVariablesUpdater::suspend()
{
	mUpdateTimer.stop();
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
}

void SensorVariablesUpdater::onFailure()
{
//	Tracer::debug(tracer::enums::autoupdatedSensorValues, "Interpreter::slotFailure", "");
}

void SensorVariablesUpdater::updateScalarSensorVariables(PortInfo const &sensorPortInfo, int reading)
{
	updateScalarSensorVariable(sensorPortInfo.reservedVariable(), reading);
}

void SensorVariablesUpdater::updateScalarSensorVariable(QString const &variable, int reading)
{
	mParser.mutableVariables()[variable] = new utils::Number(reading, utils::Number::intType);
//	Tracer::debug(
//			tracer::enums::autoupdatedSensorValues
//			, "SensorVariablesUpdater::updateScalarSensorVariable"
//			, variable + QString::number(reading)
//			);
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
