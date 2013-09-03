#pragma once

#include "../simpleGenerators/binding.h"
#include "../templateParametrizedEntity.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

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
