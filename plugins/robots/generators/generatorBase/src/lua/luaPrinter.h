#pragma once

#include <QtCore/QMap>

#include <qrtext/lua/ast/node.h>

#include "reservedFunctionsConverter.h"
#include "generatorBase/lua/precedenceConverterInterface.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {
namespace lua {

class LuaPrinter : public qrtext::lua::LuaAstVisitorInterface
		, public TemplateParametrizedEntity
{
public:
	/// Takes ownership on converters.
	LuaPrinter(QString const &pathToTemplates
			, PrecedenceConverterInterface &precedeceTable
			, simple::Binding::ConverterInterface const *reservedVariablesConverter);

	/// Prints the given AST to the code using a set of templates placed in the given in the constructor directory.
	virtual QString print(QSharedPointer<qrtext::lua::ast::Node> node);

private:
	void visit(qrtext::lua::ast::Number const &node) override;
	void visit(qrtext::lua::ast::UnaryMinus const &node) override;
	void visit(qrtext::lua::ast::Not const &node) override;
	void visit(qrtext::lua::ast::BitwiseNegation const &node) override;
	void visit(qrtext::lua::ast::Length const &node) override;
	void visit(qrtext::lua::ast::LogicalAnd const &node) override;
	void visit(qrtext::lua::ast::LogicalOr const &node) override;
	void visit(qrtext::lua::ast::Addition const &node) override;
	void visit(qrtext::lua::ast::Subtraction const &node) override;
	void visit(qrtext::lua::ast::Multiplication const &node) override;
	void visit(qrtext::lua::ast::Division const &node) override;
	void visit(qrtext::lua::ast::IntegerDivision const &node) override;
	void visit(qrtext::lua::ast::Modulo const &node) override;
	void visit(qrtext::lua::ast::Exponentiation const &node) override;
	void visit(qrtext::lua::ast::BitwiseAnd const &node) override;
	void visit(qrtext::lua::ast::BitwiseOr const &node) override;
	void visit(qrtext::lua::ast::BitwiseXor const &node) override;
	void visit(qrtext::lua::ast::BitwiseLeftShift const &node) override;
	void visit(qrtext::lua::ast::BitwiseRightShift const &node) override;
	void visit(qrtext::lua::ast::Concatenation const &node) override;
	void visit(qrtext::lua::ast::Equality const &node) override;
	void visit(qrtext::lua::ast::LessThan const &node) override;
	void visit(qrtext::lua::ast::LessOrEqual const &node) override;
	void visit(qrtext::lua::ast::Inequality const &node) override;
	void visit(qrtext::lua::ast::GreaterThan const &node) override;
	void visit(qrtext::lua::ast::GreaterOrEqual const &node) override;
	void visit(qrtext::lua::ast::IntegerNumber const &node) override;
	void visit(qrtext::lua::ast::FloatNumber const &node) override;
	void visit(qrtext::lua::ast::FieldInitialization const &node) override;
	void visit(qrtext::lua::ast::TableConstructor const &node) override;
	void visit(qrtext::lua::ast::String const &node) override;
	void visit(qrtext::lua::ast::True const &node) override;
	void visit(qrtext::lua::ast::False const &node) override;
	void visit(qrtext::lua::ast::Nil const &node) override;
	void visit(qrtext::lua::ast::Identifier const &node) override;
	void visit(qrtext::lua::ast::FunctionCall const &node) override;
	void visit(qrtext::lua::ast::MethodCall const &node) override;
	void visit(qrtext::lua::ast::Assignment const &node) override;
	void visit(qrtext::lua::ast::Block const &node) override;
	void visit(qrtext::lua::ast::IndexingExpression const &node) override;

	/// Returns true if child expression must be wrapped into the brackets.
	/// The default implementation returns true if the parent operation has greater precedence or
	/// equal precedence and child is situated on the opposite side to operator`s associativity.
	/// May be redefined by printers to concrete language.
	virtual bool needBrackets(qrtext::lua::ast::Node const &parent
			, qrtext::lua::ast::Node const &child
			, qrtext::core::Associativity childAssociativity = qrtext::core::Associativity::right) const;

	void processUnary(qrtext::core::ast::UnaryOperator const &node, QString const &templateFileName);
	void processBinary(qrtext::core::ast::BinaryOperator const &node, QString const &templateFileName);
	void processTemplate(qrtext::lua::ast::Node const &node, QString const &templateFileName
			, QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> const &bindings);

	void pushResult(qrtext::lua::ast::Node const &node, QString const &generatedCode);
	QString popResult(qrtext::lua::ast::Node const &node, bool wrapIntoBrackets = false);
	QStringList popResults(QList<QSharedPointer<qrtext::lua::ast::Node>> const &nodes);

	QMap<qrtext::lua::ast::Node const *, QString> mGeneratedCode;
	PrecedenceConverterInterface &mPrecedenceTable;
	simple::Binding::ConverterInterface const *mReservedVariablesConverter;
	ReservedFunctionsConverter mReservedFunctionsConverter;
};

}
}
