#pragma once

#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace converters {

/// A base class for all converters that use external templates
class ROBOTS_GENERATOR_EXPORT TemplateParametrizedConverter : public simple::Binding::ConverterInterface
		, public TemplateParametrizedEntity
{
public:
	explicit TemplateParametrizedConverter(const QString &pathToTemplates);
};

}
}
