#include "functionBlockConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FunctionBlockConverter::FunctionBlockConverter(QString const &pathToTemplates
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: CodeConverterBase(pathToTemplates
			, robotModel
			, devices
			, inputPortConverter
			, functionInvocationsConverter)
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
