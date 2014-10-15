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
	IntPropertyConverter(QString const &pathToTemplates
			, lua::LuaProcessor &luaTranslator
			, qReal::Id const &id
			, simple::Binding::ConverterInterface *reservedVariablesConverter
			, simple::Binding::ConverterInterface *typeConverter);

	~IntPropertyConverter() override;

	QString convert(QString const &data) const override;

private:
	simple::Binding::ConverterInterface const *mTypeConverter;  // Takes ownership
};

}
}
