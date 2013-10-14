#pragma once

#include <converters/templateParametrizedMultiConverter.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class TrikEnginePortsConverter : public TemplateParametrizedMultiConverter
{
public:
	TrikEnginePortsConverter(QString const &pathToTemplates);

	virtual QStringList convert(QString const &data) const;
};

}
}
}
}
