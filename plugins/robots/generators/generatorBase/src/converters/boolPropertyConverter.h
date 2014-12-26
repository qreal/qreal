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
	BoolPropertyConverter(QString const &pathToTemplates
			, lua::LuaProcessor &luaTranslator
			, qReal::Id const &id
			, QString const &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter
			, bool needInverting);

	QString convert(QString const &data) const override;

private:
	QString invert(QString const &expression) const;

	bool mNeedInverting;
};

}
}
