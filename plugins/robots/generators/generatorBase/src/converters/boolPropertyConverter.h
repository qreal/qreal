#pragma once

#include "codeConverterBase.h"
#include "generatorBase/templateParametrizedEntity.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the bool type.
/// Inverts this expression if it was specified in constructor. Inversion
/// code must be specified in "conditional/negation.t" template.
class BoolPropertyConverter : public CodeConverterBase, public TemplateParametrizedEntity
{
public:
	BoolPropertyConverter(const QString &pathToTemplates
			, lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QString &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter
			, bool needInverting);

	QString convert(const QString &data) const override;

private:
	QString invert(const QString &expression) const;

	bool mNeedInverting;
};

}
}
