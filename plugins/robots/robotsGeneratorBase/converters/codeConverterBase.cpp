#include "codeConverterBase.h"

#include <qrkernel/settingsManager.h>

using namespace qReal::robots::generators::converters;

CodeConverterBase::CodeConverterBase(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: TemplateParametrizedConverter(pathToTemplates)
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
		result.replace("Sensor" + stringSensor, sensorExpression(port));
	}

	result.replace("EncoderA", encoderExpression("A"));
	result.replace("EncoderB", encoderExpression("B"));
	result.replace("EncoderC", encoderExpression("C"));

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

QString CodeConverterBase::readSensorTemplatePath(
		qReal::interpreters::robots::enums::sensorType::SensorTypeEnum sensorType) const
{
	switch (sensorType) {
	case qReal::interpreters::robots::enums::sensorType::colorRed:
	case qReal::interpreters::robots::enums::sensorType::colorGreen:
	case qReal::interpreters::robots::enums::sensorType::colorBlue:
	case qReal::interpreters::robots::enums::sensorType::colorFull:
	case qReal::interpreters::robots::enums::sensorType::colorNone:
		return "sensors/readColor.t";
	case qReal::interpreters::robots::enums::sensorType::sonar:
		return "sensors/readSonar.t";
	case qReal::interpreters::robots::enums::sensorType::light:
		return "sensors/readLight.t";
	case qReal::interpreters::robots::enums::sensorType::sound:
		return "sensors/readSound.t";
	case qReal::interpreters::robots::enums::sensorType::gyroscope:
		return "sensors/readGyroscope.t";
	default:
		// TODO: display error when no sensor specified
		return "sensors/readTouch.t";
	}
}

QString CodeConverterBase::sensorExpression(int port) const
{
	QString const portString = QString::number(port);

	int const portValue = SettingsManager::value("port" + portString + "SensorType").toInt();

	QString const templatePath = readSensorTemplatePath(
			static_cast<qReal::interpreters::robots::enums::sensorType::SensorTypeEnum>(portValue));
	QString result = readTemplate(templatePath);

	// Converter must take a string like "1" or "2" (and etc) and return correct value
	result.replace("@@PORT@@", mInputConverter->convert(portString));
	return result;
}

QString CodeConverterBase::encoderExpression(QString const &port) const
{
	QString result = readTemplate("sensors/readEncoder.t");
	// Converter must take a string like "A" or "B" (and etc) and return correct value
	// TODO: rewrite for arbitary case
	result.replace("@@PORT@@", mOutputConverter->convert(port));
	return result;
}

QString CodeConverterBase::timelineExpression() const
{
	return readTemplate("whatTime.t");
}
