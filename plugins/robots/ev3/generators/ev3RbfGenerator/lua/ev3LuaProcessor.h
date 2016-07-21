/* Copyright 2015 CyberTech Labs Ltd.
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

#include <generatorBase/lua/luaProcessor.h>

namespace generatorBase {
namespace parts {
class Variables;
}
}

namespace ev3 {
namespace rbf {
class Ev3RbfGeneratorFactory;
namespace lua {
class Ev3LuaPrinter;

/// A toolbox for reengineering lua code. Prints lua code into the EV3 bytecode.
class Ev3LuaProcessor : public generatorBase::lua::LuaProcessor
{
	Q_OBJECT

public:
	Ev3LuaProcessor(qReal::ErrorReporterInterface &errorReporter
			, qrtext::LanguageToolboxInterface &textLanguage
			, const utils::ParserErrorReporter &parserErrorReporter
			, QObject *parent = 0);

	~Ev3LuaProcessor() override;

	/// Configures this processor with variables instance. Variables cannot be passed to constructor because it will
	/// cause cyclic dependencies between Ev3LuaProcessor and ev3GenratorBase::Ev3GeneratorCustomizer dueing the
	/// initialization.
	void configure(generatorBase::parts::Variables *variables, Ev3RbfGeneratorFactory *factory);

	/// Converts the given Lua code into the EV3 bytecode and substitues all
	/// reserved variables and functions code.
	/// Takes ownership on @arg reservedVariablesConverter.
	QString translate(const QString &luaCode
			, const qReal::Id &id
			, const QString &propertyName
			, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter) override;

	/// Converts the given Lua code into the EV3 bytecode, substitues all
	/// reserved variables and functions code and casts the result to string.
	/// Takes ownership on @arg reservedVariablesConverter.
	QString castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
			, const QString &luaCode
			, const qReal::Id &id
			, const QString &propertyName
			, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter) override;

	/// Returns code that initializes all known constants.
	QString constantsValuation() const;

private:
	void preparePrinter();

	generatorBase::parts::Variables *mVariables;
	Ev3RbfGeneratorFactory *mFactory;
	QScopedPointer<Ev3LuaPrinter> mPrinter;
};

}
}
}
