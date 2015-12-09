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
	, mPrinter(nullptr)
{
}

Ev3LuaProcessor::~Ev3LuaProcessor()
{
}

void Ev3LuaProcessor::configure(generatorBase::parts::Variables *variables, Ev3RbfGeneratorFactory *factory)
{
	mVariables = variables;
	mFactory = factory;
	mPrinter.reset(new Ev3LuaPrinter(pathsToRoot(), mTextLanguage, *mVariables));
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
	mPrinter->configure(reservedVariablesConverter);
	const QString result = mPrinter->print(tree, id);
	mFactory->addAdditionalCode(id, mPrinter->additionalCode(tree));
	return result;
}


QString Ev3LuaProcessor::castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
		, const QString &data
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
	mPrinter->configure(reservedVariablesConverter);
	const QString result = mPrinter->castTo(type, tree, id);
	mFactory->addAdditionalCode(id, mPrinter->additionalCode(tree));
	return result;
}

QString Ev3LuaProcessor::constantsValuation() const
{
	return mPrinter->constantsEvaluation();
}
