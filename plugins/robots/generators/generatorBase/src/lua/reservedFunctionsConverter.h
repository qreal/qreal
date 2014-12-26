#pragma once

#include "generatorBase/templateParametrizedEntity.h"

namespace generatorBase {
namespace lua {

/// Converts the given Lua reserved function call to the target language one.
class ReservedFunctionsConverter : public TemplateParametrizedEntity
{
public:
	explicit ReservedFunctionsConverter(QString const &pathToTemplates);

	QString convert(QString const &name, QStringList const &args) const;
};

}
}
