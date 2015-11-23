/* Copyright 2007-2015 QReal Research Group, Yurii Litvinov
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

#include "qrtext/src/lua/luaInterpreter.h"

#include <QtCore/QtMath>

#include "qrtext/lua/types/string.h"
#include "qrtext/lua/types/integer.h"

#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/floatNumber.h"
#include "qrtext/lua/ast/functionCall.h"
#include "qrtext/lua/ast/identifier.h"
#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/string.h"
#include "qrtext/lua/ast/true.h"
#include "qrtext/lua/ast/false.h"
#include "qrtext/lua/ast/nil.h"
#include "qrtext/lua/ast/tableConstructor.h"
#include "qrtext/lua/ast/indexingExpression.h"
#include "qrtext/lua/ast/block.h"

#include "qrtext/lua/ast/unaryMinus.h"
#include "qrtext/lua/ast/not.h"
#include "qrtext/lua/ast/length.h"
#include "qrtext/lua/ast/bitwiseNegation.h"

#include "qrtext/lua/ast/addition.h"
#include "qrtext/lua/ast/subtraction.h"
#include "qrtext/lua/ast/multiplication.h"
#include "qrtext/lua/ast/division.h"
#include "qrtext/lua/ast/integerDivision.h"
#include "qrtext/lua/ast/exponentiation.h"
#include "qrtext/lua/ast/modulo.h"
#include "qrtext/lua/ast/bitwiseAnd.h"
#include "qrtext/lua/ast/bitwiseXor.h"
#include "qrtext/lua/ast/bitwiseOr.h"
#include "qrtext/lua/ast/bitwiseRightShift.h"
#include "qrtext/lua/ast/bitwiseLeftShift.h"
#include "qrtext/lua/ast/concatenation.h"
#include "qrtext/lua/ast/lessThan.h"
#include "qrtext/lua/ast/greaterThan.h"
#include "qrtext/lua/ast/lessOrEqual.h"
#include "qrtext/lua/ast/greaterOrEqual.h"
#include "qrtext/lua/ast/equality.h"
#include "qrtext/lua/ast/inequality.h"
#include "qrtext/lua/ast/logicalAnd.h"
#include "qrtext/lua/ast/logicalOr.h"

using namespace qrtext::lua::details;
using namespace qrtext;

LuaInterpreter::LuaInterpreter(QList<core::Error> &errors)
	: mErrors(errors)
{
}

QVariant LuaInterpreter::interpret(const QSharedPointer<core::ast::Node> &root
		, const core::SemanticAnalyzer &semanticAnalyzer)
{
	Q_UNUSED(semanticAnalyzer);

	if (!root) {
		return QVariant();
	}

	if (root->is<ast::Block>()) {
		auto statements = as<ast::Block>(root)->children();
		for (auto statement : statements) {
			if (statement != statements.last()) {
				interpret(statement, semanticAnalyzer);
			}
		}

		return !statements.isEmpty() ? interpret(statements.last(), semanticAnalyzer) : QVariant();
	} else if (root->is<ast::IntegerNumber>()) {
		/// @todo Integer and float literals may differ from those recognized in toInt() and toDouble().
		bool ok = false;
		return as<ast::IntegerNumber>(root)->stringRepresentation().toInt(&ok, 0);
	} else if (root->is<ast::FloatNumber>()) {
		return as<ast::FloatNumber>(root)->stringRepresentation().toDouble();
	} else if (root->is<ast::String>()) {
		return as<ast::String>(root)->string();
	} else if (root->is<ast::TableConstructor>()) {
		return constructTable(root, semanticAnalyzer);
	} else if (root->is<ast::Assignment>()) {
		auto variable = as<ast::Assignment>(root)->variable();
		auto value = as<ast::Assignment>(root)->value();
		auto interpretedValue = interpret(value, semanticAnalyzer);

		if (variable->is<ast::Identifier>()) {
			const auto name = as<ast::Identifier>(variable)->name();

			if (mReadOnlyVariables.contains(name)) {
				mErrors.append(core::Error(root->start(), QObject::tr("Variable %1 is read-only")
						, core::ErrorType::runtimeError, core::Severity::error));

				return QVariant();
			}

			mIdentifierValues.insert(name, interpretedValue);
			return QVariant();
		} else if (variable->is<ast::IndexingExpression>()) {
			assignToTableElement(variable, interpretedValue, semanticAnalyzer);
			return QVariant();
		} else {
			mErrors.append(core::Error(root->start(), QObject::tr("This construction is not supported by interpreter")
					, core::ErrorType::runtimeError, core::Severity::error));
		}

		return QVariant();

	} else if (root->is<ast::Identifier>()) {
		return mIdentifierValues.value(as<ast::Identifier>(root)->name());
	} else if (root->is<ast::FunctionCall>()) {
		auto function = as<ast::FunctionCall>(root)->function();
		auto name = as<ast::Identifier>(function)->name();
		auto parameters = as<ast::FunctionCall>(root)->arguments();

		QList<QVariant> actualParameters;
		for (auto parameter : parameters) {
			actualParameters << interpret(parameter, semanticAnalyzer);
		}

		return mIntrinsicFunctions[name](actualParameters);
	} else if (root->is<ast::IndexingExpression>()) {
		return slice(root, semanticAnalyzer);
	} else if (root->is<ast::UnaryOperator>()) {
		return interpretUnaryOperator(root, semanticAnalyzer);
	} else if (root->is<ast::BinaryOperator>()) {
		return interpretBinaryOperator(root, semanticAnalyzer);
	} else if (root->is<ast::True>()) {
		return true;
	} else if (root->is<ast::False>()) {
		return false;
	} else if (root->is<ast::Nil>()) {
		return QVariant();
	} else {
		mErrors.append(core::Error(root->start(), QObject::tr("This construction is not supported by interpreter")
				, core::ErrorType::runtimeError, core::Severity::error));

		return QVariant();
	}
}

void LuaInterpreter::addIntrinsicFunction(const QString &name
		, std::function<QVariant(const QList<QVariant> &)> const &semantic)
{
	mIntrinsicFunctions.insert(name, semantic);
}

QStringList LuaInterpreter::identifiers() const
{
	return mIdentifierValues.keys();
}

QVariant LuaInterpreter::value(const QString &identifier) const
{
	return mIdentifierValues.value(identifier);
}

void LuaInterpreter::setVariableValue(const QString &name, const QVariant &value)
{
	QString valueString = value.toString();
	if (!valueString.isEmpty()
			&& (valueString[0] == '\'' || valueString[0] == '\"')
			&& (valueString[valueString.size() - 1] == '\'' || valueString[valueString.size() - 1] == '\"')
			)
	{
		// It is a string variable, chop off quotes.
		valueString.remove(0, 1);
		valueString.chop(1);
		mIdentifierValues.insert(name, valueString);
	} else {
		mIdentifierValues.insert(name, value);
	}
}

void LuaInterpreter::addReadOnlyVariable(const QString &name)
{
	mReadOnlyVariables.insert(name);
}

void LuaInterpreter::clear()
{
	mIdentifierValues.clear();
	mReadOnlyVariables.clear();
}

QVariant LuaInterpreter::interpretUnaryOperator(const QSharedPointer<core::ast::Node> &root
		, const core::SemanticAnalyzer &semanticAnalyzer)
{
	auto operand = as<ast::UnaryOperator>(root)->operand();
	if (root->is<ast::UnaryMinus>()) {
		return -interpret(operand, semanticAnalyzer).toFloat();
	} else if (root->is<ast::Not>()) {
		const QVariant operandResult = interpret(operand, semanticAnalyzer);
		/// @todo Code 'nil' more adequately.
		if (operandResult.isNull()) {
			return true;
		}

		return !operandResult.toBool();
	} else if (root->is<ast::Length>()) {
		if (semanticAnalyzer.type(operand)->is<types::String>()) {
			/// @todo Well, in Lua '#' returns bytes in a string, not symbols.
			return interpret(operand, semanticAnalyzer).toString().length();
		}
		/// @todo Support everything else.
	} else if (root->is<ast::BitwiseNegation>()) {
		return ~(interpret(operand, semanticAnalyzer).toInt());
	}

	return QVariant();
}

QVariant LuaInterpreter::interpretBinaryOperator(const QSharedPointer<core::ast::Node> &root
		, const core::SemanticAnalyzer &semanticAnalyzer)
{
	auto leftOperand = as<ast::BinaryOperator>(root)->leftOperand();
	auto rightOperand = as<ast::BinaryOperator>(root)->rightOperand();

	if (root->is<ast::Addition>()) {
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				+ interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::Subtraction>()) {
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				- interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::Multiplication>()) {
		QVariant leftOperandValue = interpret(leftOperand, semanticAnalyzer);
		return leftOperandValue.toDouble()
				* interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::Division>()) {
		const auto leftOperandValue = interpret(leftOperand, semanticAnalyzer).toDouble();
		const auto rightOperandValue = interpret(rightOperand, semanticAnalyzer).toDouble();
		if (rightOperandValue != 0) {
			return leftOperandValue / rightOperandValue;
		} else {
			mErrors.append(core::Error(root->start(), QObject::tr("Division by zero")
					, core::ErrorType::runtimeError, core::Severity::error));
			return 0;
		}
	} else if (root->is<ast::IntegerDivision>()) {
		const auto leftOperandValue = interpret(leftOperand, semanticAnalyzer).toInt();
		const auto rightOperandValue = interpret(rightOperand, semanticAnalyzer).toInt();
		if (rightOperandValue != 0) {
			return leftOperandValue / rightOperandValue;
		} else {
			mErrors.append(core::Error(root->start(), QObject::tr("Division by zero")
					, core::ErrorType::runtimeError, core::Severity::error));
			return 0;
		}
	} else if (root->is<ast::Exponentiation>()) {
		return qPow(interpret(leftOperand, semanticAnalyzer).toDouble()
				, interpret(rightOperand, semanticAnalyzer).toDouble());
	} else if (root->is<ast::Modulo>()) {
		const auto leftOperandValue = interpret(leftOperand, semanticAnalyzer).toInt();
		const auto rightOperandValue = interpret(rightOperand, semanticAnalyzer).toInt();
		if (rightOperandValue != 0) {
			return leftOperandValue % rightOperandValue;
		} else {
			mErrors.append(core::Error(root->start(), QObject::tr("Division by zero")
					, core::ErrorType::runtimeError, core::Severity::error));
			return 0;
		}
	} else if (root->is<ast::BitwiseAnd>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				& interpret(rightOperand, semanticAnalyzer).toInt();
	} else if (root->is<ast::BitwiseOr>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				| interpret(rightOperand, semanticAnalyzer).toInt();
	} else if (root->is<ast::BitwiseXor>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				^ interpret(rightOperand, semanticAnalyzer).toInt();
	} else if (root->is<ast::BitwiseLeftShift>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				<< interpret(rightOperand, semanticAnalyzer).toInt();
	} else if (root->is<ast::BitwiseRightShift>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				>> interpret(rightOperand, semanticAnalyzer).toInt();

	} else if (root->is<ast::Concatenation>()) {
		return interpret(leftOperand, semanticAnalyzer).toString()
				+ interpret(rightOperand, semanticAnalyzer).toString();

	/// @todo String comparison.
	} else if (root->is<ast::LessThan>()) {
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				< interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::LessOrEqual>()) {
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				<= interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::GreaterThan>()) {
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				> interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::GreaterOrEqual>()) {
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				>= interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::Equality>()) {
		return interpret(leftOperand, semanticAnalyzer) == interpret(rightOperand, semanticAnalyzer);
	} else if (root->is<ast::Inequality>()) {
		return interpret(leftOperand, semanticAnalyzer) != interpret(rightOperand, semanticAnalyzer);
	} else if (root->is<ast::LogicalAnd>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				&& interpret(rightOperand, semanticAnalyzer).toInt();
	} else if (root->is<ast::LogicalOr>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				|| interpret(rightOperand, semanticAnalyzer).toInt();
	}

	return QVariant();
}


QVariant LuaInterpreter::operateOnIndexingExpression(const QSharedPointer<core::ast::Node> &indexingExpression
		, const core::SemanticAnalyzer &semanticAnalyzer
		, const std::function<QVariant(const QString &
				, const QVariantList &
				, const QVector<int> &
				, const core::Connection &)> &action)
{
	return operateOnIndexingExpressionRecursive(indexingExpression, {}, semanticAnalyzer, action);
}

QVariant LuaInterpreter::operateOnIndexingExpressionRecursive(const QSharedPointer<core::ast::Node> &indexingExpression
		, const QVector<int> &currentIndex, const core::SemanticAnalyzer &semanticAnalyzer
		, const std::function<QVariant(const QString &
				, const QVariantList &
				, const QVector<int> &
				, const core::Connection &)> &action)
{
	const auto node = as<ast::IndexingExpression>(indexingExpression);

	const auto reportError = [this, &node](){
		mErrors.append(core::Error(node->start()
				, QObject::tr("Currently interpreter allows only tables denoted by identifier and "
						"by integer expression index, as in 'a[1 + 2][3]'")
				, core::ErrorType::runtimeError, core::Severity::error));
	};

	if (node->table()->is<ast::Identifier>()) {
		const auto name = as<ast::Identifier>(node->table())->name();
		if (semanticAnalyzer.type(node->indexer())->is<types::Number>()) {
			const auto index = interpret(node->indexer(), semanticAnalyzer).toInt();
			const auto table = mIdentifierValues.value(name).value<QVariantList>();

			return action(name, table, QVector<int>{index} + currentIndex, node->start());
		}

		reportError();
		return QVariant();
	} else if (node->table()->is<ast::IndexingExpression>()) {
		if (semanticAnalyzer.type(node->indexer())->is<types::Number>()) {
			const auto index = interpret(node->indexer(), semanticAnalyzer).toInt();
			return operateOnIndexingExpressionRecursive(node->table()
					, QVector<int>{index} + currentIndex, semanticAnalyzer, action);
		}

		reportError();
		return QVariant();
	}

	/// @todo Support more complex cases of table slice, like
	///       "f(x)['a'] = 1". Note that field access in form of "a.x = 1" is parsed as "a['x'] = 1", so
	///       no special handling is needed for that case.
	mErrors.append(core::Error(node->start()
			, QObject::tr("Tables denoted by something other than identifier (like f(x)[0]) are not allowed")
			, core::ErrorType::runtimeError, core::Severity::error));

	return QVariant();
}

QVariant LuaInterpreter::constructTable(const QSharedPointer<core::ast::Node> &tableConstructor
		, const core::SemanticAnalyzer &semanticAnalyzer)
{
	QVariantList temp;
	for (const auto &node : as<ast::TableConstructor>(tableConstructor)->initializers()) {
		if (node->implicitKey()) {
			temp << interpret(node->value(), semanticAnalyzer);
		} else {
			if (semanticAnalyzer.type(node->key())->is<types::Number>()) {
				const auto index = interpret(node->key(), semanticAnalyzer).toInt();
				if (temp.size() <= index) {
					for (int i = 0; index >= temp.size(); ++i) {
						/// @todo: add proper "nil" value.
						temp.append("");
					}
				}

				temp[index] = interpret(node->value(), semanticAnalyzer).value<QString>();
			} else {
				mErrors.append(core::Error(tableConstructor->start()
						, QObject::tr("Explicit table indexes of non-integer type are not supported")
						, core::ErrorType::runtimeError, core::Severity::error));
			}
		}
	}

	return QVariant(temp);
}

void LuaInterpreter::assignToTableElement(const QSharedPointer<core::ast::Node> &variable
		, const QVariant &interpretedValue, const core::SemanticAnalyzer &semanticAnalyzer)
{
	const auto action = [this, &interpretedValue] (
			const QString &name
			, const QVariantList &table
			, const QVector<int> &index
			, const core::Connection &connection)
	{
		mIdentifierValues.insert(name, doAssignToTableElement(table, interpretedValue, index, connection));
		return QVariant();
	};

	operateOnIndexingExpression(variable, semanticAnalyzer, action);
}

QVariantList LuaInterpreter::doAssignToTableElement(const QVariantList &table
		, const QVariant &value
		, const QVector<int> &index
		, const core::Connection &connection)
{
	QVariantList result;
	int i = 0;
	const int currentIndex = index.first();
	if (currentIndex < 0) {
		mErrors.append(core::Error(connection
				, QObject::tr("Negative index for a table")
				, core::ErrorType::runtimeError, core::Severity::error));
		return table;
	}

	const QVector<int> remainingIndex = index.mid(1);

	if (remainingIndex.isEmpty()) {
		result = table;
		for (int i = 0; currentIndex >= result.size(); ++i) {
			result << QVariant();
		}

		if (currentIndex >= 0) {
			result[currentIndex] = value;
		} else {
			mErrors.append(core::Error(connection
					, QObject::tr("Negative index for a table")
					, core::ErrorType::runtimeError, core::Severity::error));
		}

		return result;
	}

	for (const auto &element : table) {
		if (i == currentIndex) {
			result << QVariant(doAssignToTableElement(element.value<QVariantList>()
					, value
					, remainingIndex
					, connection));
		} else {
			result << element;
		}

		++i;
	}

	if (currentIndex >= result.size()) {
		for (int i = 0; currentIndex >= result.size() + 1; ++i) {
			result << QVariant();
		}

		result << QVariant(doAssignToTableElement({}, value, remainingIndex, connection));
	}

	return result;
}

QVariant LuaInterpreter::slice(const QSharedPointer<core::ast::Node> &indexingExpression
		, const core::SemanticAnalyzer &semanticAnalyzer)
{
	const auto action = [this] (const QString &name
			, const QVariantList &table
			, const QVector<int> &index
			, const core::Connection &connection)
	{
		Q_UNUSED(name);

		QVariantList slice = table;

		QVector<int> actualIndex = index;
		const int lastIndex = index.last();
		actualIndex.removeLast();

		for (int i : actualIndex) {
			if (slice.size() <= i) {
				return QVariant();
			}

			if (i < 0) {
				mErrors.append(core::Error(connection
						, QObject::tr("Negative index for a table")
						, core::ErrorType::runtimeError, core::Severity::error));
				return QVariant();
			}

			slice = slice[i].value<QVariantList>();
		}

		if (slice.size() <= lastIndex) {
			return QVariant();
		}

		if (lastIndex < 0) {
			mErrors.append(core::Error(connection
					, QObject::tr("Negative index for a table")
					, core::ErrorType::runtimeError, core::Severity::error));
			return QVariant();
		}

		return slice[lastIndex];
	};

	return operateOnIndexingExpression(indexingExpression, semanticAnalyzer, action);
}
