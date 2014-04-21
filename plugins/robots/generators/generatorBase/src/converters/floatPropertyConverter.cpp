#include "floatPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FloatPropertyConverter::FloatPropertyConverter(QString const &pathToTemplates
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: CodeConverterBase(pathToTemplates
			, robotModel
			, devices
			, inputPortConverter
			, outputPortConverter
			, functionInvocationsConverter)
{
}
