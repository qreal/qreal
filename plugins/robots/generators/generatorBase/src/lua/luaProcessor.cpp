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

#include "generatorBase/lua/luaProcessor.h"

#include <qrtext/languageToolboxInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "src/lua/luaPrinter.h"

using namespace generatorBase::lua;
using namespace qReal;

LuaProcessor::LuaProcessor(qReal::ErrorReporterInterface &errorReporter
		, qrtext::LanguageToolboxInterface &textLanguage
		, const utils::ParserErrorReporter &parserErrorReporter
		, QObject *parent)
	: QObject(parent)
	, TemplateParametrizedEntity()
	, mErrorReporter(errorReporter)
	, mTextLanguage(textLanguage)
	, mParserErrorReporter(parserErrorReporter)
{
}

QString LuaProcessor::translate(const QString &data
		, const Id &id
		, const QString &propertyName
		, const simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	return lua::LuaPrinter(pathsToRoot(), mTextLanguage
			, precedenceConverter(), reservedVariablesConverter).print(tree);
}


QString LuaProcessor::castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
		, const QString &data
		, const Id &id
		, const QString &propertyName
		, const simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	const QSharedPointer<qrtext::core::ast::Node> tree = parse(data, id, propertyName);
	return lua::LuaPrinter(pathsToRoot(), mTextLanguage
			, precedenceConverter(), reservedVariablesConverter).castTo(type, tree);
}

QSharedPointer<qrtext::core::ast::Node> LuaProcessor::parse(const QString &data
		, const qReal::Id &id
		, const QString &propertyName) const
{
	const QSharedPointer<qrtext::core::ast::Node> tree = mTextLanguage.parse(id, propertyName, data);
	if (!mTextLanguage.errors().isEmpty()) {
		mParserErrorReporter.reportErrors(id, propertyName);
		return qrtext::wrap(nullptr);
	}

	return tree;
}

qrtext::LanguageToolboxInterface &LuaProcessor::toolbox() const
{
	return mTextLanguage;
}

PrecedenceConverterInterface &LuaProcessor::precedenceConverter()
{
	return mPrecedenceConverter;
}
