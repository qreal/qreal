#pragma once

#include "templateParametrizedMultiConverter.h"
#include "enginePortConverter.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class EnginePortsConverter : public TemplateParametrizedMultiConverter
{
public:
	EnginePortsConverter(QString const &pathToTemplates, EnginePortConverter const *oneEngineConverter);
	virtual ~EnginePortsConverter();

	virtual QStringList convert(QString const &data) const;

private:
	EnginePortConverter const *mOneEngineConverter;  // Takes ownership
};

}
}
}
}
