/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "interpreterCore/interpreter/details/sensorVariablesUpdater.h"

#include <utils/timelineInterface.h>
#include <utils/abstractTimer.h>
#include <kitBase/robotModel/robotParts/scalarSensor.h>
#include <kitBase/robotModel/robotParts/vectorSensor.h>

static const int unrealUpdateInterval = 20;
static const int realUpdateInterval = 200;

using namespace interpreterCore::interpreter::details;
using namespace kitBase::robotModel;

SensorVariablesUpdater::SensorVariablesUpdater(const RobotModelManagerInterface &robotModelManager
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
		robotParts::AbstractSensor * const sensor = dynamic_cast<robotParts::AbstractSensor *>(device);
		if (sensor && !sensor->port().reservedVariable().isEmpty()) {

			if (!sensor->ready() || sensor->isLocked()) {
				/// @todo Error reporting
				continue;
			}

			sensor->read();
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
}

void SensorVariablesUpdater::updateScalarSensorVariables(const PortInfo &sensorPortInfo, int reading)
{
	updateScalarSensorVariable(sensorPortInfo.reservedVariable(), reading);
}

void SensorVariablesUpdater::updateScalarSensorVariable(const QString &variable, int reading)
{
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
		robotParts::AbstractSensor * const sensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (!sensor) {
			return;
		}

		// Sensor state must be unlocked before interpretation starts even if it was not unlocked previous session.
		sensor->setLocked(false);

		robotParts::ScalarSensor * const scalarSensor = dynamic_cast<robotParts::ScalarSensor *>(device);
		if (scalarSensor) {
			updateScalarSensorVariables(scalarSensor->port(), 0);
		}

		robotParts::VectorSensor * const vectorSensor = dynamic_cast<robotParts::VectorSensor *>(device);
		if (vectorSensor) {
			scalarSensor->setLocked(false);
			updateVectorSensorVariables(vectorSensor->port(), {});
		}
	}
}
