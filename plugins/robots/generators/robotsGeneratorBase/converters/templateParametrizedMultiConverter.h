#pragma once

#include "../simpleGenerators/binding.h"
#include "../templateParametrizedEntity.h"
#include "robotsGeneratorDeclSpec.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// A base class for all multi-converters that use external templates
class ROBOTS_GENERATOR_EXPORT TemplateParametrizedMultiConverter : public simple::Binding::MultiConverterInterface
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
