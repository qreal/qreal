#include "luaPrinter.h"

#include <qrkernel/logging.h>

#include <qrtext/lua/ast/number.h>
#include <qrtext/lua/ast/unaryMinus.h>
#include <qrtext/lua/ast/not.h>
#include <qrtext/lua/ast/bitwiseNegation.h>
#include <qrtext/lua/ast/length.h>
#include <qrtext/lua/ast/logicalAnd.h>
#include <qrtext/lua/ast/logicalOr.h>
#include <qrtext/lua/ast/addition.h>
#include <qrtext/lua/ast/subtraction.h>
#include <qrtext/lua/ast/multiplication.h>
#include <qrtext/lua/ast/division.h>
#include <qrtext/lua/ast/integerDivision.h>
#include <qrtext/lua/ast/modulo.h>
#include <qrtext/lua/ast/exponentiation.h>
#include <qrtext/lua/ast/bitwiseAnd.h>
#include <qrtext/lua/ast/bitwiseOr.h>
#include <qrtext/lua/ast/bitwiseXor.h>
#include <qrtext/lua/ast/bitwiseLeftShift.h>
#include <qrtext/lua/ast/bitwiseRightShift.h>
#include <qrtext/lua/ast/concatenation.h>
#include <qrtext/lua/ast/equality.h>
#include <qrtext/lua/ast/lessThan.h>
#include <qrtext/lua/ast/lessOrEqual.h>
#include <qrtext/lua/ast/inequality.h>
#include <qrtext/lua/ast/greaterThan.h>
#include <qrtext/lua/ast/greaterOrEqual.h>
#include <qrtext/lua/ast/integerNumber.h>
#include <qrtext/lua/ast/floatNumber.h>
#include <qrtext/lua/ast/fieldInitialization.h>
#include <qrtext/lua/ast/tableConstructor.h>
#include <qrtext/lua/ast/string.h>
#include <qrtext/lua/ast/true.h>
#include <qrtext/lua/ast/false.h>
#include <qrtext/lua/ast/nil.h>
#include <qrtext/lua/ast/identifier.h>
#include <qrtext/lua/ast/functionCall.h>
#include <qrtext/lua/ast/methodCall.h>
#include <qrtext/lua/ast/assignment.h>
#include <qrtext/lua/ast/block.h>
#include <qrtext/lua/ast/indexingExpression.h>

using namespace generatorBase::printing;

LuaPrinter::LuaPrinter(QString const &pathToTemplates)//, PrecedenceConverterInterface &precedeceTable)
	: TemplateParametrizedEntity(pathToTemplates + "/luaPrinting")
//	, mPrecedenceTable(precedeceTable)
{
	/// @todo: remove it
	setPathToTemplates("/home/dvvrd/dev/qreal/lua-printers/plugins/robots/generators/trikGenerator/templates/luaPrinting/");
}

LuaPrinter::~LuaPrinter()
{
}

QString LuaPrinter::print(QSharedPointer<qrtext::lua::ast::Node> node)
{
	node->acceptRecursively(*this);
	if (mGeneratedCode.keys().count() != 1 || mGeneratedCode.keys().first() != node.data()) {
		QLOG_WARN() << "Lua printer got into the inconsistent state during printing."
				<< mGeneratedCode.keys().count() <<"pieces of code:";
		qDebug() << "Lua printer got into the inconsistent state during printing."
				<< mGeneratedCode.keys().count() <<"pieces of code:";
		for (QString const &code : mGeneratedCode.values()) {
			QLOG_INFO() << code;
			qDebug() << code;
		}

		mGeneratedCode.clear();
		return QString();
	}

	return popResult(*node);
}

void LuaPrinter::pushResult(qrtext::lua::ast::Node const &node, QString const &generatedCode)
{
	qDebug() << "pushing" << generatedCode;
	mGeneratedCode[&node] = generatedCode;
}

QString LuaPrinter::popResult(qrtext::lua::ast::Node const &node, bool wrapIntoBrackets)
{
	QString const code = mGeneratedCode.take(&node);
	return wrapIntoBrackets ? "(" + code + ")" : code;
}

void LuaPrinter::processTemplate(qrtext::lua::ast::Node const &node
		, QString const &templateFileName
		, QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> const &bindings)
{
	QString result = readTemplate(templateFileName);
	for (QString const &toReplace : bindings.keys()) {
		result.replace(toReplace, popResult(*bindings[toReplace]));
	}

	pushResult(node, result);
}

void LuaPrinter::processUnary(qrtext::core::ast::UnaryOperator const &node, QString const &templateFileName)
{
	pushResult(node, readTemplate(templateFileName)
			.replace("@@OPERAND@@", popResult(*node.operand(), needBrackets(node, *node.operand()))));
}

void LuaPrinter::processBinary(qrtext::core::ast::BinaryOperator const &node, QString const &templateFileName)
{
	pushResult(node, readTemplate(templateFileName)
			.replace("@@LEFT@@", popResult(*node.leftOperand(), needBrackets(node, *node.leftOperand())))
			.replace("@@RIGHT@@", popResult(*node.rightOperand(), needBrackets(node, *node.rightOperand()))));
}

bool LuaPrinter::needBrackets(qrtext::lua::ast::Node const &parent, qrtext::lua::ast::Node const &child) const
{
	return true;
//	int const parentPrecedence = mPrecedenceTable.precedence(parent);
//	int const childPrecedence = mPrecedenceTable.precedence(child);
//	return parentPrecedence > childPrecedence || (parentPrecedence == childPrecedence
//			&& mPrecedenceTable.associativity(child) == qrtext::core::Associativity::right);
}

void LuaPrinter::visit(qrtext::lua::ast::Number const &node)
{
	pushResult(node, node.stringRepresentation());
}

void LuaPrinter::visit(qrtext::lua::ast::UnaryMinus const &node)
{
	processUnary(node, "unaryMinus.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Not const &node)
{
	processUnary(node, "not.t");
}

void LuaPrinter::visit(qrtext::lua::ast::BitwiseNegation const &node)
{
	processUnary(node, "bitwiseNegation.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Length const &node)
{
	processUnary(node, "length.t");
}

void LuaPrinter::visit(qrtext::lua::ast::LogicalAnd const &node)
{
	processBinary(node, "logicalAnd.t");
}

void LuaPrinter::visit(qrtext::lua::ast::LogicalOr const &node)
{
	processBinary(node, "logicalOr.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Addition const &node)
{
	processBinary(node, "addition.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Subtraction const &node)
{
	processBinary(node, "subtraction.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Multiplication const &node)
{
	processBinary(node, "multiplication.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Division const &node)
{
	processBinary(node, "division.t");
}

void LuaPrinter::visit(qrtext::lua::ast::IntegerDivision const &node)
{
	processBinary(node, "integerDivision.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Modulo const &node)
{
	processBinary(node, "modulo.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Exponentiation const &node)
{
	processBinary(node, "exponentiation.t");
}

void LuaPrinter::visit(qrtext::lua::ast::BitwiseAnd const &node)
{
	processBinary(node, "bitwiseAnd.t");
}

void LuaPrinter::visit(qrtext::lua::ast::BitwiseOr const &node)
{
	processBinary(node, "bitwiseOr.t");
}

void LuaPrinter::visit(qrtext::lua::ast::BitwiseXor const &node)
{
	processBinary(node, "bitwiseXor.t");
}

void LuaPrinter::visit(qrtext::lua::ast::BitwiseLeftShift const &node)
{
	processBinary(node, "bitwiseLeftShift.t");
}

void LuaPrinter::visit(qrtext::lua::ast::BitwiseRightShift const &node)
{
	processBinary(node, "bitwiseRightShift.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Concatenation const &node)
{
	processBinary(node, "concatenation.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Equality const &node)
{
	processBinary(node, "equality.t");
}

void LuaPrinter::visit(qrtext::lua::ast::LessThan const &node)
{
	processBinary(node, "lessThan.t");
}

void LuaPrinter::visit(qrtext::lua::ast::LessOrEqual const &node)
{
	processBinary(node, "lessOrEqual.t");
}

void LuaPrinter::visit(qrtext::lua::ast::Inequality const &node)
{
	processBinary(node, "inequality.t");
}

void LuaPrinter::visit(qrtext::lua::ast::GreaterThan const &node)
{
	processBinary(node, "greaterThan.t");
}

void LuaPrinter::visit(qrtext::lua::ast::GreaterOrEqual const &node)
{
	processBinary(node, "greaterOrEqual.t");
}

void LuaPrinter::visit(qrtext::lua::ast::IntegerNumber const &node)
{
	pushResult(node, node.stringRepresentation());
}

void LuaPrinter::visit(qrtext::lua::ast::FloatNumber const &node)
{
	pushResult(node, node.stringRepresentation());
}

void LuaPrinter::visit(qrtext::lua::ast::FieldInitialization const &node)
{
	processTemplate(node, "fieldInitialization.t", { {"@@KEY@@", node.key()}, {"@@VALUE@@", node.value()} });
}

void LuaPrinter::visit(qrtext::lua::ast::TableConstructor const &node)
{
	/// @todo: Implement it
	pushResult(node, "todo: implement TableConstructor");
}

void LuaPrinter::visit(qrtext::lua::ast::String const &node)
{
	pushResult(node, readTemplate("string.t").replace("@@VALUE@@", node.string()));
}

void LuaPrinter::visit(qrtext::lua::ast::True const &node)
{
	pushResult(node, readTemplate("true.t"));
}

void LuaPrinter::visit(qrtext::lua::ast::False const &node)
{
	pushResult(node, readTemplate("false.t"));
}

void LuaPrinter::visit(qrtext::lua::ast::Nil const &node)
{
	pushResult(node, readTemplate("nil.t"));
}

void LuaPrinter::visit(qrtext::lua::ast::Identifier const &node)
{
	/// @todo: implement system variables substitution
	pushResult(node, node.name());
}

void LuaPrinter::visit(qrtext::lua::ast::FunctionCall const &node)
{
	/// @todo: Implement it
	pushResult(node, "todo: implement FunctionCall");
}

void LuaPrinter::visit(qrtext::lua::ast::MethodCall const &node)
{
	/// @todo: Implement it
	pushResult(node, "todo: implement MethodCall");
}

void LuaPrinter::visit(qrtext::lua::ast::Assignment const &node)
{
	processTemplate(node, "assignment.t", { {"@@VARIABLE@@", node.variable()}, {"@@VALUE@@", node.value()} });
}

void LuaPrinter::visit(qrtext::lua::ast::Block const &node)
{
	/// @todo: Implement it
	pushResult(node, "todo: implement Block");
}

void LuaPrinter::visit(qrtext::lua::ast::IndexingExpression const &node)
{
	processTemplate(node, "indexingExpression.t", { {"@@TABLE@@", node.table()}, {"@@INDEXER@@", node.indexer()} });
}
