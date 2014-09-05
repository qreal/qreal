#pragma once

#include <functional>

#include <qrkernel/ids.h>

#include "qrtext/debuggerInterface.h"
#include "qrtext/core/error.h"
#include "qrtext/core/ast/node.h"
#include "qrtext/core/types/typeExpression.h"

namespace qrtext {

/// Interface for language toolboxes (toolbox is a facade that integrates lexer, parser, semantic analyzer and
/// interpreter, and provides convenient methods to use them).
class LanguageToolboxInterface : public DebuggerInterface {
public:
	virtual ~LanguageToolboxInterface() {}

	template<typename T>
	T interpret(qReal::Id const &id, QString const &propertyName, QString const &code)
	{
		auto root = parse(id, propertyName, code);
		return interpret(root).value<T>();
	}

	template<typename T>
	T interpret(QString const &code)
	{
		auto root = parse(qReal::Id(), "", code);
		return interpret(root).value<T>();
	}

	virtual void interpret(qReal::Id const &id, QString const &propertyName, QString const &code) = 0;

	virtual void interpret(QString const &code) = 0;

	virtual QSharedPointer<core::ast::Node> const &parse(qReal::Id const &id
			, QString const &propertyName
			, QString const &code) = 0;

	virtual QSharedPointer<core::ast::Node> ast(qReal::Id const &id, QString const &propertyName) const = 0;

	virtual QSharedPointer<core::types::TypeExpression> type(
			QSharedPointer<core::ast::Node> const &expression) const = 0;

	virtual QList<core::Error> const &errors() const = 0;

	/// Register intrinsic function.
	/// @param name - name of a function.
	/// @param returnType - function return type, as type expression. Takes ownership.
	/// @param parameterTypes - a list of types of function parameters. Takes ownership.
	/// @param semantic - a function that will be called by interpreter to actually get a result.
	virtual void addIntrinsicFunction(QString const &name
			, core::types::TypeExpression * const returnType
			, const QList<core::types::TypeExpression *> &parameterTypes
			, std::function<QVariant(QList<QVariant> const &)> const &semantic) = 0;

private:
	virtual QVariant interpret(QSharedPointer<core::ast::Node> const &root) = 0;
};

}

