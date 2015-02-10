#pragma once

#include "generatorBase/templateParametrizedEntity.h"

namespace generatorBase {
namespace lua {

/// Converts the given Lua reserved function call to the target language one.
class ReservedFunctionsConverter : public TemplateParametrizedEntity
{
public:
	explicit ReservedFunctionsConverter(const QString &pathToTemplates);

	QString convert(const QString &name, const QStringList &args) const;
};

}
}
