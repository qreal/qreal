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

#include <QtCore/QSharedPointer>
#include <QtCore/QScopedPointer>

#include "qrtext/languageToolboxInterface.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace lua {

namespace details {
class LuaLexer;
class LuaParser;
class LuaSemanticAnalyzer;
class LuaInterpreter;
}

typedef core::Error Error;

/// Facade that uses text language parsing/interpreting framework to provide services based on "QReal standard" text
/// language (Lua 5.3 subset).
///
/// Facade can be used for interpreting diagrams or for syntax analysis in generators. It allows to pass code fragments
/// as chunks (corresponding to Lua blocks) identified by Id of a node and name of the property where this code fragment
/// was taken from. It can interpret it immediately, returning result and storing syntax tree and interpreter state for
/// future uses, or can parse it and then return AST for node/property by request (note that it uses all ASTs
/// as a context for parsing next chunks).
///
/// Note that types of variables may change during parsing next chunks, so, for example, after parsing "a = 123" type
/// of "a" will be inferred as Integer, but after parsing "a = 1.0" it will be changed to Float. Generators may reliably
/// use type information only when all code in a program is parsed.
class QRTEXT_EXPORT LuaToolbox : public LanguageToolboxInterface
{
public:
	/// Constructor.
	LuaToolbox();

	~LuaToolbox() override;

	void interpret(const qReal::Id &id, const QString &propertyName, const QString &code) override;

	void interpret(const QString &code) override;

	/// Interprets given code string using given id and property name for connection. Returns result of an expression.
	/// Reintroduce this method in a namespace of this class, since otherwise C++ lookup will not find it without full
	/// qualification.
	template<typename T>
	T interpret(const qReal::Id &id, const QString &propertyName, const QString &code)
	{
		return LanguageToolboxInterface::interpret<T>(id, propertyName, code);
	}

	/// Interprets given code string without connection. Returns result of an expression.
	/// Reintroduce this method in a namespace of this class, since otherwise C++ lookup will not find it without full
	/// qualification.
	template<typename T>
	T interpret(const QString &code)
	{
		return LanguageToolboxInterface::interpret<T>(code);
	}

	const QSharedPointer<core::ast::Node> &parse(const qReal::Id &id
			, const QString &propertyName
			, const QString &code) override;

	QSharedPointer<core::ast::Node> ast(const qReal::Id &id, const QString &propertyName) const override;

	QSharedPointer<core::types::TypeExpression> type(const QSharedPointer<core::ast::Node> &expression) const override;

	QList<core::Error> const &errors() const override;

	void addIntrinsicFunction(const QString &name
			, core::types::TypeExpression * const returnType
			, const QList<core::types::TypeExpression *> &parameterTypes
			, std::function<QVariant(const QList<QVariant> &)> const &semantic) override;

	QStringList identifiers() const override;

	QMap<QString, QSharedPointer<core::types::TypeExpression>> variableTypes() const override;

	const QStringList &specialIdentifiers() const override;

	const QStringList &specialConstants() const override;

	/// Reintroduced method from DebuggerInterface, returns value of identifier with given name.
	template<typename T>
	T value(const QString &identifier) const
	{
		return DebuggerInterface::value<T>(identifier);
	}

	/// Reintroduced method from DebuggerInterface, sets a value of given identifier in interpreter to given value.
	template<typename T>
	void setVariableValue(const QString &name, T value)
	{
		DebuggerInterface::setVariableValue<T>(name, value);
	}

	void clear() override;

	bool isGeneralization(const QSharedPointer<core::types::TypeExpression> &specific
			, const QSharedPointer<core::types::TypeExpression> &general) const override;

protected:
	/// Tells that the given identifier is a constant and reserved by the system (like 'pi').
	void markAsSpecialConstant(const QString &identifier);

	/// Tells that the given identifier is reserved by the system.
	void markAsSpecial(const QString &identifier);

private:
	QVariant interpret(const QSharedPointer<core::ast::Node> &root) override;
	QVariant value(const QString &identifier) const override;

	void setVariableValue(const QString &name, const QString &initCode, const QVariant &value) override;

	void reportErrors();

	QList<core::Error> mErrors;

	QScopedPointer<details::LuaLexer> mLexer;
	QScopedPointer<details::LuaParser> mParser;
	QScopedPointer<details::LuaSemanticAnalyzer> mAnalyzer;
	QScopedPointer<details::LuaInterpreter> mInterpreter;

	QHash<qReal::Id, QHash<QString, QSharedPointer<core::ast::Node>>> mAstRoots;
	QHash<qReal::Id, QHash<QString, QString>> mParsedCache;

	QStringList mSpecialConstants;
	QStringList mSpecialIdentifiers;
};

}
}
