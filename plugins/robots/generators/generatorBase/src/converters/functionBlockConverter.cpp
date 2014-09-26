#include "functionBlockConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FunctionBlockConverter::FunctionBlockConverter(QString const &pathToTemplates
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, qrtext::LanguageToolboxInterface &textLanguage
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter
		, parts::DeviceVariables const &deviceVariables
		)
	: CodeConverterBase(pathToTemplates
			, errorReporter
			, robotModel
			, textLanguage
			, devices
			, inputPortConverter
			, functionInvocationsConverter
			, deviceVariables)
{
}

QString FunctionBlockConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);

	QStringList const lines = preparedCode.split(';', QString::SkipEmptyParts);
	QStringList result;
	foreach (QString const &line, lines) {
		result.append(line.trimmed() + ";");
	}

	return result.join('\n');
}
