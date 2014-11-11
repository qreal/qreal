#pragma once

#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
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
