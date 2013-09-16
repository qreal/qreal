#pragma once

#include "../simpleGenerators/binding.h"
#include "../templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// A base class for all multi-converters that use external templates
class TemplateParametrizedMultiConverter : public simple::Binding::MultiConverterInterface
		, public TemplateParametrizedEntity
{
public:
	explicit TemplateParametrizedMultiConverter(QString const &pathToTemplates);
	virtual ~TemplateParametrizedMultiConverter();
};

}
}
}
}
