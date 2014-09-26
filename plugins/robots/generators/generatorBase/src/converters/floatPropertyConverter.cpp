#include "floatPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FloatPropertyConverter::FloatPropertyConverter(QString const &pathToTemplates
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
