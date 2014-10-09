#pragma once

#include "precedenceConverter.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace qrtext {
class LanguageToolboxInterface;
}

namespace generatorBase {
namespace lua {

/// A toolbox for reengineering lua code. Prints lua code into the target language,
/// replaces reserved variables with code specified in templates from "sensors" folder
/// and function invocations with code specified in templates from "functions" folder.
class LuaProcessor : public QObject, public TemplateParametrizedEntity
{
	Q_OBJECT

public:
	LuaProcessor(qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguage
			, QObject *parent = 0);

	/// Converts the given Lua code into the target language and substitues all
	/// reserved variables and functions code.
	/// Takes ownership on @arg reservedVariablesConverter.
	QString translate(QString const &luaCode
			, qReal::Id const &id
			, simple::Binding::ConverterInterface const *reservedVariablesConverter);

	/// Returns facade object for manipulating text language.
	qrtext::LanguageToolboxInterface &toolbox() const;

protected:
	/// Can be overrided to return precedence precedence table for the target language.
	/// By default C precedence table is returned (because most languages have same precedences).
	virtual PrecedenceConverterInterface &precedenceConverter();

	qReal::ErrorReporterInterface &mErrorReporter;
	qrtext::LanguageToolboxInterface &mTextLanguage;

private:
	PrecedenceConverter mPrecedenceConverter;
};

}
}
