#include "codeConverterBase.h"

#include <qrkernel/settingsManager.h>

using namespace generatorBase::converters;
using namespace qReal;

CodeConverterBase::CodeConverterBase(QString const &pathToTemplates
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: TemplateParametrizedConverter(pathToTemplates)
	, mDevices(devices)
	, mInputConverter(inputPortConverter)
	, mOutputConverter(outputPortConverter)
	, mFunctionInvocationsConverter(functionInvocationsConverter)
{
}

CodeConverterBase::~CodeConverterBase()
{
	delete mInputConverter;
	delete mOutputConverter;
	delete mFunctionInvocationsConverter;
}

QString CodeConverterBase::convert(QString const &data) const
{
	return replaceFunctionInvocations(replaceSystemVariables(data)).trimmed();
}

QString CodeConverterBase::replaceSystemVariables(QString const &expression) const
{
	QString result = expression;
	for (int port = 1; port <= 4; ++port) {
		QString const stringSensor = QString::number(port);
//		result.replace("sensor" + stringSensor, sensorExpression(port));
	}

	result.replace("encoderA", encoderExpression("A"));
	result.replace("encoderB", encoderExpression("B"));
	result.replace("encoderC", encoderExpression("C"));

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

QString CodeConverterBase::readSensorTemplatePath(interpreterBase::robotModel::DeviceInfo const &sensor) const
{
//	switch (sensorType) {
//	case qReal::interpreters::robots::enums::sensorType::colorFull:
//		return "sensors/readColorRecognition.t";
//	case qReal::interpreters::robots::enums::sensorType::colorRed:
//	case qReal::interpreters::robots::enums::sensorType::colorGreen:
//	case qReal::interpreters::robots::enums::sensorType::colorBlue:
//	case qReal::interpreters::robots::enums::sensorType::colorNone:
//		return "sensors/readColor.t";
//	case qReal::interpreters::robots::enums::sensorType::sonar:
//		return "sensors/readSonar.t";
//	case qReal::interpreters::robots::enums::sensorType::light:
//		return "sensors/readLight.t";
//	case qReal::interpreters::robots::enums::sensorType::sound:
//		return "sensors/readSound.t";
//	case qReal::interpreters::robots::enums::sensorType::gyroscope:
//		return "sensors/readGyroscope.t";
//	default:
//		// TODO: display error when no sensor specified
//		return "sensors/readTouch.t";
//	}
}

QString CodeConverterBase::sensorExpression(interpreterBase::robotModel::PortInfo const &port) const
{
	QString const templatePath = readSensorTemplatePath(mDevices[port]);
	// Converter must take a string like "1" or "2" (and etc) and return correct value
	return readTemplate(templatePath).replace("@@PORT@@", mInputConverter->convert(port.name()));
}

QString CodeConverterBase::encoderExpression(QString const &port) const
{
	// Converter must take a string like "A" or "B" (and etc) and return correct value
	// TODO: rewrite for arbitary case
	return readTemplate("sensors/readEncoder.t").replace("@@PORT@@", mOutputConverter->convert(port));
}

QString CodeConverterBase::timelineExpression() const
{
	/// @todo: generate timestamps code in nxt c when required
	return readTemplate("whatTime.t");
}
