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

QString Sensors::initCode(interpreterBase::robotModel::PortInfo const &port
		, interpreterBase::robotModel::DeviceInfo const &device)
{
	QString const portString = mInputPortConverter->convert(port.name());

	if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorFull>()) {
		return readTemplate("initialization/colorFull.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorRed>()) {
		return readTemplate("initialization/colorRed.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorGreen>()) {
		return readTemplate("initialization/colorGreen.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorBlue>()) {
		return readTemplate("initialization/colorBlue.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorPassive>()) {
		return readTemplate("initialization/colorNone.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::RangeSensor>()) {
		return readTemplate("initialization/sonar.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::LightSensor>()) {
		return readTemplate("initialization/light.t").replace("@@PORT@@", portString);
	}

	return QString();
}

QString Sensors::terminateCode(interpreterBase::robotModel::PortInfo const &port
		, interpreterBase::robotModel::DeviceInfo const &device)
{
	QString const portString = mInputPortConverter->convert(port.name());

	if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorFull>()) {
		return readTemplate("termination/colorFull.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorRed>()) {
		return readTemplate("termination/colorRed.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorGreen>()) {
		return readTemplate("termination/colorGreen.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorBlue>()) {
		return readTemplate("termination/colorBlue.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorPassive>()) {
		return readTemplate("termination/colorNone.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::RangeSensor>()) {
		return readTemplate("termination/sonar.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::LightSensor>()) {
		return readTemplate("termination/light.t").replace("@@PORT@@", portString);
	}

	return QString();
}

QString Sensors::isrHooks(interpreterBase::robotModel::PortInfo const &port
		, interpreterBase::robotModel::DeviceInfo const &device)
{
	QString const portString = mInputPortConverter->convert(port.name());

	if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorFull>()) {
		return readTemplate("isrHooks/colorFull.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorRed>()) {
		return readTemplate("isrHooks/colorRed.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorGreen>()) {
		return readTemplate("isrHooks/colorGreen.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorBlue>()) {
		return readTemplate("isrHooks/colorBlue.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorPassive>()) {
		return readTemplate("isrHooks/colorNone.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::RangeSensor>()) {
		return readTemplate("isrHooks/sonar.t").replace("@@PORT@@", portString);
	} else if (device.isA<interpreterBase::robotModel::robotParts::LightSensor>()) {
		return readTemplate("isrHooks/light.t").replace("@@PORT@@", portString);
	}

	return QString();
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
	mInitCode << initCode(port, device);
	mTerminateCode << terminateCode(port, device);
	QString const isrHooksCode = isrHooks(port, device);
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
