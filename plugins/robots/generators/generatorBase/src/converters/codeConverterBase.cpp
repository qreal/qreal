#include "codeConverterBase.h"

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

using namespace generatorBase::converters;
using namespace qReal;

CodeConverterBase::CodeConverterBase(QString const &pathToTemplates
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: TemplateParametrizedConverter(pathToTemplates)
	, mRobotModel(robotModel)
	, mDevices(devices)
	, mInputConverter(inputPortConverter)
	, mFunctionInvocationsConverter(functionInvocationsConverter)
{
}

CodeConverterBase::~CodeConverterBase()
{
	delete mInputConverter;
	delete mFunctionInvocationsConverter;
}

QString CodeConverterBase::convert(QString const &data) const
{
	return replaceFunctionInvocations(replaceSystemVariables(data)).trimmed();
}

QString CodeConverterBase::replaceSystemVariables(QString const &expression) const
{
	QString result = expression;

	for (interpreterBase::robotModel::PortInfo const &port : mRobotModel.availablePorts()) {
		QString const variable = port.reservedVariable();
		if (!variable.isEmpty()) {
			result.replace(variable, deviceExpression(port));
		}
	}

	result.replace("Time", timelineExpression());
	return result;
}

QString CodeConverterBase::replaceFunctionInvocations(QString const &expression) const
{
	QString result = expression;

	QString const randomTemplate = mFunctionInvocationsConverter->convert("random");

	QRegExp const randomFunctionInvocationRegEx("random\\((.*)\\)");
	int pos = randomFunctionInvocationRegEx.indexIn(result, 0);
	while (pos != -1) {
		QString const param = randomFunctionInvocationRegEx.cap(1);
		QString randomInvocation = randomTemplate;
		randomInvocation.replace("@@UPPER_BOUND@@", param);
		result.replace(randomFunctionInvocationRegEx, randomInvocation);
		pos += randomFunctionInvocationRegEx.matchedLength();
		pos = randomFunctionInvocationRegEx.indexIn(result, pos);
	}

	return result;
}

QString CodeConverterBase::deviceTemplatePath(interpreterBase::robotModel::DeviceInfo const &device) const
{
	if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorFull>()) {
		return "sensors/readColorRecognition.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorRed>()) {
		return "sensors/readColor.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorGreen>()) {
		return "sensors/readColor.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorBlue>()) {
		return "sensors/readColor.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::ColorSensorPassive>()) {
		return "sensors/readColor.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::RangeSensor>()) {
		return "sensors/readSonar.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::LightSensor>()) {
		return "sensors/readLight.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::SoundSensor>()) {
		return "sensors/readSound.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::GyroscopeSensor>()) {
		return "sensors/readGyroscope.t";
	} else if (device.isA<interpreterBase::robotModel::robotParts::EncoderSensor>()) {
		return "sensors/readEncoder.t";
	}

	return "sensors/readTouch.t";
}

QString CodeConverterBase::deviceExpression(interpreterBase::robotModel::PortInfo const &port) const
{
	QString const templatePath = deviceTemplatePath(mDevices[port]);
	// Converter must take a string like "1" or "2" (and etc) and return correct value
	return readTemplate(templatePath).replace("@@PORT@@", mInputConverter->convert(port.name()));
}

QString CodeConverterBase::timelineExpression() const
{
	/// @todo: generate timestamps code in nxt c when required
	return readTemplate("whatTime.t");
}
