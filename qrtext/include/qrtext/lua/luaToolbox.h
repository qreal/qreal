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

	void interpret(qReal::Id const &id, QString const &propertyName, QString const &code) override;

	void interpret(QString const &code) override;

	/// Interprets given code string using given id and property name for connection. Returns result of an expression.
	/// Reintroduce this method in a namespace of this class, since otherwise C++ lookup will not find it without full
	/// qualification.
	template<typename T>
	T interpret(qReal::Id const &id, QString const &propertyName, QString const &code)
	{
		return LanguageToolboxInterface::interpret<T>(id, propertyName, code);
	}

	/// Interprets given code string without connection. Returns result of an expression.
	/// Reintroduce this method in a namespace of this class, since otherwise C++ lookup will not find it without full
	/// qualification.
	template<typename T>
	T interpret(QString const &code)
	{
		return LanguageToolboxInterface::interpret<T>(code);
	}

	QSharedPointer<core::ast::Node> const &parse(qReal::Id const &id
			, QString const &propertyName
			, QString const &code) override;

	QSharedPointer<core::ast::Node> ast(qReal::Id const &id, QString const &propertyName) const override;

	QSharedPointer<core::types::TypeExpression> type(QSharedPointer<core::ast::Node> const &expression) const override;

	QList<core::Error> const &errors() const override;

	void addIntrinsicFunction(QString const &name
			, core::types::TypeExpression * const returnType
			, const QList<core::types::TypeExpression *> &parameterTypes
			, std::function<QVariant(QList<QVariant> const &)> const &semantic) override;

	QStringList identifiers() const override;

	/// Reintroduced method from DebuggerInterface, returns value of identifier with given name.
	template<typename T>
	T value(QString const &identifier) const
	{
		return DebuggerInterface::value<T>(identifier);
	}

	/// Reintroduced method from DebuggerInterface, sets a value of given identifier in interpreter to given value.
	template<typename T>
	void setVariableValue(QString const &name, T value)
	{
		DebuggerInterface::setVariableValue<T>(name, value);
	}

private:
	QVariant interpret(QSharedPointer<core::ast::Node> const &root) override;
	QVariant value(QString const &identifier) const override;

	void setVariableValue(QString const &name, QString const &initCode, QVariant const &value) override;

	void reportErrors();

	QList<core::Error> mErrors;

	QScopedPointer<details::LuaLexer> mLexer;
	QScopedPointer<details::LuaParser> mParser;
	QScopedPointer<details::LuaSemanticAnalyzer> mAnalyzer;
	QScopedPointer<details::LuaInterpreter> mInterpreter;

	QHash<qReal::Id, QHash<QString, QSharedPointer<core::ast::Node>>> mAstRoots;
	QHash<qReal::Id, QHash<QString, QString>> mParsedCache;
};

}
}
