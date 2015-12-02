/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrutils/parserErrorReporter.h>
#include <qrtext/core/types/typeExpression.h>

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
class ROBOTS_GENERATOR_EXPORT LuaProcessor : public QObject, public TemplateParametrizedEntity
{
	Q_OBJECT

public:
	LuaProcessor(qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguage
			, const utils::ParserErrorReporter &parserErrorReporter
			, QObject *parent = 0);

	/// Converts the given Lua code into the target language and substitues all
	/// reserved variables and functions code.
	/// Takes ownership on @arg reservedVariablesConverter.
	virtual QString translate(const QString &luaCode
			, const qReal::Id &id
			, const QString &propertyName
			, const simple::Binding::ConverterInterface *reservedVariablesConverter);

	/// Converts the given Lua code into the target language, substitues all
	/// reserved variables and functions code and casts the result to the given types.
	/// Takes ownership on @arg reservedVariablesConverter.
	virtual QString castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
			, const QString &luaCode
			, const qReal::Id &id
			, const QString &propertyName
			, const simple::Binding::ConverterInterface *reservedVariablesConverter);

	/// Returns facade object for manipulating text language.
	qrtext::LanguageToolboxInterface &toolbox() const;

protected:
	/// Can be overrided to return precedence precedence table for the target language.
	/// By default C precedence table is returned (because most languages have same precedences).
	virtual PrecedenceConverterInterface &precedenceConverter();

	qReal::ErrorReporterInterface &mErrorReporter;
	qrtext::LanguageToolboxInterface &mTextLanguage;

	QSharedPointer<qrtext::core::ast::Node> parse(const QString &data
			, const qReal::Id &id
			, const QString &propertyName) const;

private:
	PrecedenceConverter mPrecedenceConverter;
	const utils::ParserErrorReporter &mParserErrorReporter;
};

}
}
