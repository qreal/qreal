#include "interpreterCore/interpreter/details/sensorVariablesUpdater.h"

#include <utils/tracer.h>
#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>
#include <interpreterBase/robotModel/robotParts/scalarSensor.h>
#include <interpreterBase/robotModel/robotParts/vectorSensor.h>

static const int unrealUpdateInterval = 20;
static const int realUpdateInterval = 200;

using namespace interpreterCore::interpreter::details;
using namespace interpreterBase::robotModel;

SensorVariablesUpdater::SensorVariablesUpdater(RobotModelManagerInterface const &robotModelManager
		, qrtext::DebuggerInterface &textLanguageToolbox
		)
	: mRobotModelManager(robotModelManager)
	, mParser(textLanguageToolbox)
{
}

SensorVariablesUpdater::~SensorVariablesUpdater()
{
}

void SensorVariablesUpdater::run()
{
	mUpdateTimer.reset(mRobotModelManager.model().timeline().produceTimer());
	connect(mUpdateTimer.data(), &utils::AbstractTimer::timeout, this, &SensorVariablesUpdater::onTimerTimeout);
	resetVariables();

	for (robotParts::Device * const device : mRobotModelManager.model().configuration().devices()) {
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

			continue;
		}

		robotParts::VectorSensor * const vectorSensor = dynamic_cast<robotParts::VectorSensor *>(device);
		if (vectorSensor && !vectorSensor->port().reservedVariable().isEmpty()) {

			if (!vectorSensor->ready()) {
				/// @todo Error reporting
				continue;
			}

			connect(
					vectorSensor
					, &robotParts::VectorSensor::newData
					, this
					, &SensorVariablesUpdater::onVectorSensorResponse
					, Qt::UniqueConnection
					);

			connect(
					vectorSensor
					, &robotParts::AbstractSensor::failure
					, this
					, &SensorVariablesUpdater::onFailure
					, Qt::UniqueConnection
					);

			vectorSensor->read();

			continue;
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

void SensorVariablesUpdater::onVectorSensorResponse(const QVector<int> &reading)
{
	robotParts::VectorSensor * const vectorSensor = dynamic_cast<robotParts::VectorSensor *>(sender());
	if (!vectorSensor) {
		/// @todo Error reporting.
		return;
	}

	updateVectorSensorVariables(vectorSensor->port(), reading);
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

		robotParts::VectorSensor * const vectorSensor = dynamic_cast<robotParts::VectorSensor *>(device);
		if (vectorSensor && !vectorSensor->port().reservedVariable().isEmpty()) {

			if (!vectorSensor->ready()) {
				/// @todo Error reporting
				continue;
			}

			vectorSensor->read();
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

void SensorVariablesUpdater::updateScalarSensorVariables(const PortInfo &sensorPortInfo, int reading)
{
	updateScalarSensorVariable(sensorPortInfo.reservedVariable(), reading);
}

void SensorVariablesUpdater::updateScalarSensorVariable(const QString &variable, int reading)
{
	utils::Tracer::debug(
			utils::Tracer::autoupdatedSensorValues
			, "SensorVariablesUpdater::updateScalarSensorVariable"
			, variable + QString::number(reading)
			);

	mParser.setVariableValue(variable, reading);
}

void SensorVariablesUpdater::updateVectorSensorVariables(const PortInfo &sensorPortInfo, const QVector<int> &reading)
{
	updateVectorSensorVariable(sensorPortInfo.reservedVariable(), reading);
}

void SensorVariablesUpdater::updateVectorSensorVariable(const QString &variable, const QVector<int> &reading)
{
	mParser.setVectorVariableValue(variable, reading);
}

void SensorVariablesUpdater::resetVariables()
{
	for (robotParts::Device * const device : mRobotModelManager.model().configuration().devices()) {
		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor) {
			updateScalarSensorVariables(scalarSensor->port(), 0);
		}

		robotParts::VectorSensor * const vectorSensor = dynamic_cast<robotParts::VectorSensor *>(device);
		if (vectorSensor) {
			updateVectorSensorVariables(vectorSensor->port(), {});
		}
	}
}
