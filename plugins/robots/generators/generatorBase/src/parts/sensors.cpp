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

Sensors::Sensors(const QString &pathToTemplates
		, const simple::Binding::ConverterInterface *inputPortConverter)
	: InitTerminateCodeGenerator(pathToTemplates)
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

void Sensors::reinit(QMap<kitBase::robotModel::PortInfo
		, kitBase::robotModel::DeviceInfo> const &devices)
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
