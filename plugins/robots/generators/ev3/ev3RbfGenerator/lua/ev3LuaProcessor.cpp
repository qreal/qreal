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

#include "ev3LuaProcessor.h"

#include <qrkernel/logging.h>
#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "ev3RbfGeneratorFactory.h"
#include "ev3LuaPrinter.h"

using namespace ev3::rbf::lua;
using namespace qReal;

Ev3LuaProcessor::Ev3LuaProcessor(qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguage
		, const utils::ParserErrorReporter &parserErrorReporter
		, QObject *parent)
	: generatorBase::lua::LuaProcessor(errorReporter, textLanguage, parserErrorReporter, parent)
	, mVariables(nullptr)
	, mFactory(nullptr)
{
}

void Ev3LuaProcessor::configure(generatorBase::parts::Variables *variables, Ev3RbfGeneratorFactory *factory)
{
	mVariables = variables;
	mFactory = factory;
}

QString Ev3LuaProcessor::translate(const QString &data
		, const Id &id
		, const QString &propertyName
		, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	if (!mVariables) {
		qWarning() << "Variables instance is null in Ev3LuaProcessor. Something went wrong.";
		QLOG_ERROR() << "Variables instance is null in Ev3LuaProcessor. Something went wrong.";
		return QString();
	}

	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	lua::Ev3LuaPrinter printer(pathsToRoot(), mTextLanguage, *mVariables, reservedVariablesConverter);
	const QString result = printer.print(tree);
	mFactory->addAdditionalCode(id, printer.additionalCode());
	return result;
}


QString Ev3LuaProcessor::castToString(const QString &data
		, const Id &id
		, const QString &propertyName
		, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	if (!mVariables) {
		qWarning() << "Variables instance is null in Ev3LuaProcessor. Something went wrong.";
		QLOG_ERROR() << "Variables instance is null in Ev3LuaProcessor. Something went wrong.";
		return QString();
	}

	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	lua::Ev3LuaPrinter printer(pathsToRoot(), mTextLanguage, *mVariables, reservedVariablesConverter);
	const QString result = printer.castToString(tree);
	mFactory->addAdditionalCode(id, printer.additionalCode());
	return result;
}
