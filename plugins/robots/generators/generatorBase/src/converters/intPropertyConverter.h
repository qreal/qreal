#pragma once

#include "codeConverterBase.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/parts/variables.h"

namespace generatorBase {
namespace converters {

/// Processes input string treating it as an expression of the int type.
/// Casts this expression to int type if type inferer hints returns other one.
/// Casting code must be specified in "types/cast.t" template.
class IntPropertyConverter : public CodeConverterBase, public TemplateParametrizedEntity
{
public:
	IntPropertyConverter(const QString &pathToTemplates
			, lua::LuaProcessor &luaTranslator
			, const qReal::Id &id
			, const QString &propertyName
			, simple::Binding::ConverterInterface *reservedVariablesConverter
			, simple::Binding::ConverterInterface *typeConverter);

	~IntPropertyConverter() override;

	QString convert(const QString &data) const override;

private:
	const simple::Binding::ConverterInterface *mTypeConverter;  // Takes ownership
};

}
}
