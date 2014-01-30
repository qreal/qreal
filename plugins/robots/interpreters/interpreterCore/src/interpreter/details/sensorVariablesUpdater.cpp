#include "sensorVariablesUpdater.h"

#include <interpreterBase/robotModel/robotParts/scalarSensor.h>

static int const variableUpdateIntervalMs = 200;

using namespace interpreterCore::interpreter::details;
using namespace interpreterBase::robotModel;

SensorVariablesUpdater::SensorVariablesUpdater(
		RobotModelInterface const &robotModel
		, utils::ExpressionsParser &parser
		)
	: mModel(robotModel)
	, mParser(parser)
{
	mUpdateTimer.setInterval(variableUpdateIntervalMs);
	connect(&mUpdateTimer, &QTimer::timeout, this, &SensorVariablesUpdater::onTimerTimeout);
}

void SensorVariablesUpdater::run()
{
	resetVariables();

	for (robotParts::PluggableDevice * const device
		 : mModel.configuration().pluggableDevices(ConfigurationInterface::input))
	{
		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor) {

			if (!scalarSensor->ready()) {
				// TODO: Error reporting
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
		// TODO: Error reporting.
		return;
	}

	updateScalarSensorVariables(scalarSensor->port(), reading);
}

void SensorVariablesUpdater::onTimerTimeout()
{
	for (robotParts::PluggableDevice * const device
		 : mModel.configuration().pluggableDevices(ConfigurationInterface::input))
	{
		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor) {

			if (!scalarSensor->ready()) {
				// TODO: Error reporting
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
	updateScalarSensorVariable("Sensor" + sensorPortInfo.name(), reading);
	updateScalarSensorVariable("sensor" + sensorPortInfo.name(), reading);

	for (QString const &alias : sensorPortInfo.nameAliases()) {
		updateScalarSensorVariable("Sensor" + alias, reading);
		updateScalarSensorVariable("sensor" + alias, reading);
	}
}

void SensorVariablesUpdater::updateScalarSensorVariable(QString const &variable, int reading)
{
	(*(mParser.getVariables()))[variable] = utils::Number(reading, utils::Number::intType);
//	Tracer::debug(
//			tracer::enums::autoupdatedSensorValues
//			, "SensorVariablesUpdater::updateScalarSensorVariable"
//			, variable + QString::number(reading)
//			);
}

void SensorVariablesUpdater::resetVariables()
{
	for (robotParts::PluggableDevice * const device
		 : mModel.configuration().pluggableDevices(ConfigurationInterface::input))
	{
		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor) {
			updateScalarSensorVariables(scalarSensor->port(), 0);
		}
	}
}
