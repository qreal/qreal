#include "qrtext/src/lua/luaInterpreter.h"

#include "qrtext/lua/types/string.h"
#include "qrtext/lua/types/integer.h"

#include "qrtext/lua/ast/assignment.h"
#include "qrtext/lua/ast/floatNumber.h"
#include "qrtext/lua/ast/functionCall.h"
#include "qrtext/lua/ast/identifier.h"
#include "qrtext/lua/ast/integerNumber.h"
#include "qrtext/lua/ast/string.h"
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

QVariant LuaInterpreter::interpret(QSharedPointer<core::ast::Node> const &root
		, core::SemanticAnalyzer const &semanticAnalyzer)
{
	Q_UNUSED(semanticAnalyzer);

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
		return as<ast::IntegerNumber>(root)->stringRepresentation().toInt();
	} else if (root->is<ast::FloatNumber>()) {
		return as<ast::FloatNumber>(root)->stringRepresentation().toDouble();
	} else if (root->is<ast::String>()) {
		return as<ast::String>(root)->string();
	} else if (root->is<ast::TableConstructor>()) {
		QStringList temp;
		for (auto node : as<ast::TableConstructor>(root)->initializers()) {
			temp << interpret(node->value(), semanticAnalyzer).value<QString>();
		}

		return QVariant(temp);
	} else if (root->is<ast::Assignment>()) {
		auto variable = as<ast::Assignment>(root)->variable();
		auto value = as<ast::Assignment>(root)->value();
		auto interpretedValue = interpret(value, semanticAnalyzer);

		if (variable->is<ast::Identifier>()) {
			auto name = as<ast::Identifier>(variable)->name();
			mIdentifierValues.insert(name, interpretedValue);
			return QVariant();
		} else if (variable->is<ast::IndexingExpression>()) {
			operateOnIndexingExpression(variable
					, semanticAnalyzer
					, [this, &value, &semanticAnalyzer] (QString const &name, QStringList &table, int index) {
						table[index] = interpret(value, semanticAnalyzer).toString();
						mIdentifierValues.insert(name, table);
						return QVariant();
					});
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
		return operateOnIndexingExpression(root
				, semanticAnalyzer
				, [] (QString const &name, QStringList &table, int index) {
					Q_UNUSED(name)
					return table[index];
				});
	} else if (root->is<ast::UnaryOperator>()) {
		return interpretUnaryOperator(root, semanticAnalyzer);
	} else if (root->is<ast::BinaryOperator>()) {
		return interpretBinaryOperator(root, semanticAnalyzer);
	} else {
		mErrors.append(core::Error(root->start(), QObject::tr("This construction is not supported by interpreter")
				, core::ErrorType::runtimeError, core::Severity::error));

		return QVariant();
	}
}

QVariant LuaInterpreter::operateOnIndexingExpression(const QSharedPointer<core::ast::Node> &indexingExpression
		, core::SemanticAnalyzer const &semanticAnalyzer
		, std::function<QVariant(QString const &, QStringList &, int)> const &action)
{
	if (as<ast::IndexingExpression>(indexingExpression)->table()->is<ast::Identifier>()) {
		auto name = as<ast::Identifier>(as<ast::IndexingExpression>(indexingExpression)->table())->name();
		if (semanticAnalyzer.type(as<ast::IndexingExpression>(indexingExpression)->indexer())->is<types::Number>()) {
			auto index = interpret(as<ast::IndexingExpression>(indexingExpression)->indexer(), semanticAnalyzer).toInt();

			auto table = mIdentifierValues.value(name).value<QStringList>();
			if (table.size() <= index) {
				for (int i = 0; index >= table.size(); ++i) {
					table.append("");
				}
			}

			return action(name, table, index);
		}
	}

	mErrors.append(core::Error(indexingExpression->start()
			, QObject::tr("Currently interpreter allows only tables denoted by identifier and "
					"by integer expression index, as in 'a[1 + 2] = 3'")
			, core::ErrorType::runtimeError, core::Severity::error));

	/// @todo Support more complex cases of table assignment, like
	///       "f(x)['a'] = 1". Note that field access in form of "a.x = 1" is parsed as "a['x'] = 1", so
	///       no special handling is needed for that case.
	return QVariant();
}

void LuaInterpreter::addIntrinsicFunction(QString const &name
		, std::function<QVariant(QList<QVariant> const &)> const &semantic)
{
	mIntrinsicFunctions.insert(name, semantic);
}

QStringList LuaInterpreter::identifiers() const
{
	return mIdentifierValues.keys();
}

QVariant LuaInterpreter::value(QString const &identifier) const
{
	return mIdentifierValues.value(identifier);
}

void LuaInterpreter::setVariableValue(QString const &name, QVariant const &value)
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

QVariant LuaInterpreter::interpretUnaryOperator(QSharedPointer<core::ast::Node> const &root
		, core::SemanticAnalyzer const &semanticAnalyzer)
{
	auto operand = as<ast::UnaryOperator>(root)->operand();
	if (root->is<ast::UnaryMinus>()) {
		return -interpret(operand, semanticAnalyzer).toFloat();
	} else if (root->is<ast::Not>()) {
		QVariant const operandResult = interpret(operand, semanticAnalyzer);
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

QVariant LuaInterpreter::interpretBinaryOperator(QSharedPointer<core::ast::Node> const &root
		, core::SemanticAnalyzer const &semanticAnalyzer)
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
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				/ interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::IntegerDivision>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				/ interpret(rightOperand, semanticAnalyzer).toInt();
	} else if (root->is<ast::Exponentiation>()) {
		return pow(interpret(leftOperand, semanticAnalyzer).toDouble()
				, interpret(rightOperand, semanticAnalyzer).toDouble());
	} else if (root->is<ast::Modulo>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				% interpret(rightOperand, semanticAnalyzer).toInt();

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
				<< interpret(rightOperand, semanticAnalyzer).toInt();

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
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				== interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::Inequality>()) {
		return interpret(leftOperand, semanticAnalyzer).toDouble()
				!= interpret(rightOperand, semanticAnalyzer).toDouble();
	} else if (root->is<ast::LogicalAnd>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				&& interpret(rightOperand, semanticAnalyzer).toInt();
	} else if (root->is<ast::LogicalOr>()) {
		return interpret(leftOperand, semanticAnalyzer).toInt()
				|| interpret(rightOperand, semanticAnalyzer).toInt();
	}

	return QVariant();
}

