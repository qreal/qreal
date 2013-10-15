#pragma once

#include "../simpleGenerators/binding.h"
#include "../templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {
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
}
}
