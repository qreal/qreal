#include "generatorBase/parts/sensors.h"

#include <qrkernel/settingsManager.h>

using namespace qReal::robots::generators::parts;

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

// TODO: make implementation more modular

//QString Sensors::initCode(QString const &port
//		, interpreters::robots::enums::sensorType::SensorTypeEnum type)
//{
//	switch (type) {
//	case qReal::interpreters::robots::enums::sensorType::sonar:
//		return readTemplate("initialization/sonar.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::light:
//		return readTemplate("initialization/light.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorFull:
//		return readTemplate("initialization/colorFull.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorRed:
//		return readTemplate("initialization/colorRed.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorGreen:
//		return readTemplate("initialization/colorGreen.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorBlue:
//		return readTemplate("initialization/colorBlue.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorNone:
//		return readTemplate("initialization/colorNone.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	default:
//		return QString();
//	}
//}

//QString Sensors::terminateCode(QString const &port
//		, interpreters::robots::enums::sensorType::SensorTypeEnum type)
//{
//	switch (type) {
//	case qReal::interpreters::robots::enums::sensorType::sonar:
//		return readTemplate("termination/sonar.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::light:
//		return readTemplate("termination/light.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorFull:
//		return readTemplate("termination/colorFull.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorRed:
//		return readTemplate("termination/colorRed.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorGreen:
//		return readTemplate("termination/colorGreen.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorBlue:
//		return readTemplate("termination/colorBlue.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorNone:
//		return readTemplate("termination/colorNone.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	default:
//		return QString();
//	}
//}

//QString Sensors::isrHooks(QString const &port
//		, interpreters::robots::enums::sensorType::SensorTypeEnum type)
//{
//	switch (type) {
//	case qReal::interpreters::robots::enums::sensorType::colorFull:
//		return readTemplate("isrHooks/colorFull.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorRed:
//		return readTemplate("isrHooks/colorRed.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorGreen:
//		return readTemplate("isrHooks/colorGreen.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorBlue:
//		return readTemplate("isrHooks/colorBlue.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	case qReal::interpreters::robots::enums::sensorType::colorNone:
//		return readTemplate("isrHooks/colorNone.t").replace("@@PORT@@", mInputPortConverter->convert(port));
//		break;
//	default:
//		return QString();
//	}
//}

void Sensors::reinit()
{
	mInitCode.clear();
	mTerminateCode.clear();
	mIsrHooksCode.clear();

	for (int i = 0; i < 4; ++i) {
		reinitPort(i);
	}
}

void Sensors::reinitPort(int port)
{
//	QString const portString = QString::number(port);
//	int const portValue = SettingsManager::value("port" + portString + "SensorType").toInt();
//	interpreters::robots::enums::sensorType::SensorTypeEnum portType =
//			static_cast<interpreters::robots::enums::sensorType::SensorTypeEnum>(portValue);
//	mInitCode << initCode(portString, portType);
//	mTerminateCode << terminateCode(portString, portType);
//	QString const isrHooksCode = isrHooks(portString, portType);
//	if (!mIsrHooksCode.contains(isrHooksCode)) {
//		mIsrHooksCode << isrHooksCode;
//	}
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
