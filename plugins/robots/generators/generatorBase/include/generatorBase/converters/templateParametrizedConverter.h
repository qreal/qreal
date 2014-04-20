#pragma once

#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/templateParametrizedEntity.h"

namespace generatorBase {
namespace converters {

/// A base class for all converters that use external templates
class TemplateParametrizedConverter : public simple::Binding::ConverterInterface
		, public TemplateParametrizedEntity
{
public:
	explicit TemplateParametrizedConverter(QString const &pathToTemplates);
	virtual ~TemplateParametrizedConverter();
};

}
}
