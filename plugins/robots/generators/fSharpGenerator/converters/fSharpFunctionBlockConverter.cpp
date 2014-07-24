#include "fSharpFunctionBlockConverter.h"

using namespace fSharp::converters;
using namespace qReal;

FSharpFunctionBlockConverter::FSharpFunctionBlockConverter(QString const &pathToTemplates
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, generatorBase::simple::Binding::ConverterInterface const *inputPortConverter
		, generatorBase::simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: CodeConverterBase(pathToTemplates
			, errorReporter
			, robotModel
			, devices
			, inputPortConverter
			, functionInvocationsConverter)
{
}

QString FSharpFunctionBlockConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);

	QStringList const lines = preparedCode.split(';', QString::SkipEmptyParts);
	QStringList result;

	foreach (QString const &line, lines) {
		QRegExp regExp("<=|>=|==");

		if(!line.trimmed().contains(regExp)) {
			result.append(line.trimmed().replace("=", "<-"));
		}
		else {
			result.append(line.trimmed());
		}
	}

	return result.join('\n');
}
