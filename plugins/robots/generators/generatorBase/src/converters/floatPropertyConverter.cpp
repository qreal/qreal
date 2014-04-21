#include "floatPropertyConverter.h"

using namespace generatorBase::converters;
using namespace qReal;

FloatPropertyConverter::FloatPropertyConverter(QString const &pathToTemplates
		, QMap<interpreterBase::robotModel::PortInfo, interpreterBase::robotModel::DeviceInfo> const &devices
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: CodeConverterBase(pathToTemplates
			, devices
			, inputPortConverter
			, outputPortConverter
			, functionInvocationsConverter)
{
}
