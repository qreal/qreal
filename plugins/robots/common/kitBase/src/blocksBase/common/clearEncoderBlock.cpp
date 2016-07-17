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

#include "kitBase/blocksBase/common/clearEncoderBlock.h"

#include <kitBase/robotModel/robotParts/encoderSensor.h>
#include <kitBase/robotModel/robotModelUtils.h>

using namespace kitBase::blocksBase::common;
using namespace kitBase::robotModel;

ClearEncoderBlock::ClearEncoderBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
}

void ClearEncoderBlock::run()
{
	for (robotParts::EncoderSensor * const encoder : parsePorts()) {
		encoder->nullify();
	}

	emit done(mNextBlockId);
}

QMap<PortInfo, DeviceInfo> ClearEncoderBlock::usedDevices()
{
	QMap<PortInfo, DeviceInfo> result;
	for (robotParts::EncoderSensor * const encoder : parsePorts()) {
		result[encoder->port()] = encoder->deviceInfo();
	}

	return result;
}

QList<robotParts::EncoderSensor *> ClearEncoderBlock::parsePorts()
{
	QList<robotParts::EncoderSensor *> result;
	for (const QString &port : stringProperty("Ports").split(',', QString::SkipEmptyParts)) {
		robotParts::EncoderSensor * const encoder = RobotModelUtils::findDevice<robotParts::EncoderSensor>(
				mRobotModel, port.trimmed());

		if (encoder) {
			result << encoder;
		}
	}

	return result;
}
