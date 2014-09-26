#include "boolPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

BoolPropertyConverter::BoolPropertyConverter(QString const &pathToTemplates
		, qReal::ErrorReporterInterface &errorReporter
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, qrtext::LanguageToolboxInterface &textLanguage
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter
		, parts::DeviceVariables const &deviceVariables
		, bool needInverting)
	: CodeConverterBase(pathToTemplates
			, errorReporter
			, robotModel
			, textLanguage
			, devices
			, inputPortConverter
			, functionInvocationsConverter
			, deviceVariables
			)
	, mNeedInverting(needInverting)
{
}

QString BoolPropertyConverter::convert(QString const &data) const
{
	QString const preparedCode = CodeConverterBase::convert(data);
	return mNeedInverting ? invert(preparedCode) : preparedCode;
}

QString BoolPropertyConverter::invert(QString const &expression) const
{
	QString invertTemplate = readTemplate("conditional/negation.t");
	invertTemplate.replace("@@CONDITION@@", expression);
	return invertTemplate;
}
