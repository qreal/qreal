#pragma once

#include "templateParametrizedMultiConverter.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Splits given string selecting just 'A', 'B' and 'C' symbols and returns
/// converted output port values.
class EnginePortsConverter : public TemplateParametrizedMultiConverter
{
public:
	EnginePortsConverter(QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *oneEngineConverter);
	virtual ~EnginePortsConverter();

	virtual QStringList convert(QString const &data) const;

private:
	simple::Binding::ConverterInterface const *mOneEngineConverter;  // Takes ownership
};

}
}
}
}
