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

#include "generatorBase/parts/sensors.h"

#include <qrkernel/settingsManager.h>

#include <kitBase/robotModel/robotParts/touchSensor.h>
#include <kitBase/robotModel/robotParts/colorSensorFull.h>
#include <kitBase/robotModel/robotParts/colorSensorRed.h>
#include <kitBase/robotModel/robotParts/colorSensorGreen.h>
#include <kitBase/robotModel/robotParts/colorSensorBlue.h>
#include <kitBase/robotModel/robotParts/colorSensorPassive.h>
#include <kitBase/robotModel/robotParts/rangeSensor.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>
#include <kitBase/robotModel/robotParts/soundSensor.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include <kitBase/robotModel/robotParts/encoderSensor.h>

using namespace generatorBase::parts;
using namespace qReal;

Sensors::Sensors(const QStringList &pathsToTemplates
		, const simple::Binding::ConverterInterface *inputPortConverter)
	: InitTerminateCodeGenerator(pathsToTemplates)
	, mInputPortConverter(inputPortConverter)
{
}

Sensors::~Sensors()
{
	delete mInputPortConverter;
}

QString Sensors::code(const QString &directory
		, const kitBase::robotModel::PortInfo &port
		, const kitBase::robotModel::DeviceInfo &device)
{
	const QString portString = mInputPortConverter->convert(port.name());
	const QString templatePath = QString("%1/%2.t").arg(directory, device.name());
	return readTemplateIfExists(templatePath).replace("@@PORT@@", portString);
}

void Sensors::reinit(const QMap<kitBase::robotModel::PortInfo
		, kitBase::robotModel::DeviceInfo> &devices)
{
	mInitCode.clear();
	mTerminateCode.clear();
	mIsrHooksCode.clear();

	for (const kitBase::robotModel::PortInfo &port : devices.keys()) {
		reinitPort(port, devices[port]);
	}
}

void Sensors::reinitPort(const kitBase::robotModel::PortInfo &port
		, const kitBase::robotModel::DeviceInfo &device)
{
	mInitCode << code("initialization", port, device);
	mTerminateCode << code("termination", port, device);
	const QString isrHooksCode = code("isrHooks", port, device);
	if (!mIsrHooksCode.contains(isrHooksCode)) {
		mIsrHooksCode << isrHooksCode;
	}
}

QString Sensors::initCode()
{
	return mInitCode.join('\n');
}

QString Sensors::terminateCode()
{
	return mTerminateCode.join('\n');
}

QString Sensors::isrHooksCode()
{
	return mIsrHooksCode.join('\n');
}
