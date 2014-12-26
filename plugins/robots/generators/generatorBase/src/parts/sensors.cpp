#include "generatorBase/parts/sensors.h"

#include <qrkernel/settingsManager.h>

#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensorFull.h>
#include <interpreterBase/robotModel/robotParts/colorSensorRed.h>
#include <interpreterBase/robotModel/robotParts/colorSensorGreen.h>
#include <interpreterBase/robotModel/robotParts/colorSensorBlue.h>
#include <interpreterBase/robotModel/robotParts/colorSensorPassive.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/soundSensor.h>
#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>

using namespace generatorBase::parts;
using namespace qReal;

Sensors::Sensors(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter)
	: InitTerminateCodeGenerator(pathToTemplates)
	, mInputPortConverter(inputPortConverter)
{
}

Sensors::~Sensors()
{
	delete mInputPortConverter;
}

QString Sensors::code(QString const &directory
		, interpreterBase::robotModel::PortInfo const &port
		, interpreterBase::robotModel::DeviceInfo const &device)
{
	QString const portString = mInputPortConverter->convert(port.name());
	QString const templatePath = QString("%1/%2.t").arg(directory, device.name());
	return readTemplateIfExists(templatePath).replace("@@PORT@@", portString);
}

void Sensors::reinit(QMap<interpreterBase::robotModel::PortInfo
		, interpreterBase::robotModel::DeviceInfo> const &devices)
{
	mInitCode.clear();
	mTerminateCode.clear();
	mIsrHooksCode.clear();

	for (interpreterBase::robotModel::PortInfo const &port : devices.keys()) {
		reinitPort(port, devices[port]);
	}
}

void Sensors::reinitPort(interpreterBase::robotModel::PortInfo const &port
		, interpreterBase::robotModel::DeviceInfo const &device)
{
	mInitCode << code("initialization", port, device);
	mTerminateCode << code("termination", port, device);
	QString const isrHooksCode = code("isrHooks", port, device);
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
