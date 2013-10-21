#include "floatPropertyConverter.h"

using namespace qReal::robots::generators::converters;

FloatPropertyConverter::FloatPropertyConverter(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *inputPortConverter
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::ConverterInterface const *functionInvocationsConverter)
	: CodeConverterBase(pathToTemplates
			, inputPortConverter
			, outputPortConverter
			, functionInvocationsConverter)
{
}
