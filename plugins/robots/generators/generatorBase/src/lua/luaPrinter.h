#pragma once

#include <QtCore/QMap>

#include <qrtext/lua/ast/node.h>

#include "reservedFunctionsConverter.h"
#include "generatorBase/lua/precedenceConverterInterface.h"
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace qrtext {
class LanguageToolboxInterface;
}

namespace generatorBase {
namespace lua {

class LuaPrinter : public qrtext::lua::LuaAstVisitorInterface
		, public TemplateParametrizedEntity
{
public:
	/// Takes ownership on converters.
	LuaPrinter(const QString &pathToTemplates
			, qrtext::LanguageToolboxInterface const &textLanguage
			, PrecedenceConverterInterface &precedeceTable
			, const simple::Binding::ConverterInterface *reservedVariablesConverter);

	~LuaPrinter();

	/// Prints the given AST to the code using a set of templates placed in the given in the constructor directory.
	virtual QString print(const QSharedPointer<qrtext::lua::ast::Node> &node);

	/// Prints the given AST to the code on the target language and casts it to string.
	virtual QString castToString(const QSharedPointer<qrtext::lua::ast::Node> &node);

private:
	void visit(const qrtext::lua::ast::Number &node) override;
	void visit(const qrtext::lua::ast::UnaryMinus &node) override;
	void visit(qrtext::lua::ast::Not const &node) override;
	void visit(const qrtext::lua::ast::BitwiseNegation &node) override;
	void visit(const qrtext::lua::ast::Length &node) override;
	void visit(const qrtext::lua::ast::LogicalAnd &node) override;
	void visit(const qrtext::lua::ast::LogicalOr &node) override;
	void visit(const qrtext::lua::ast::Addition &node) override;
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
	virtual bool needBrackets(const qrtext::lua::ast::Node &parent
			, const qrtext::lua::ast::Node &child
			, qrtext::core::Associativity childAssociativity = qrtext::core::Associativity::right) const;

	void processUnary(qrtext::core::ast::UnaryOperator const &node, const QString &templateFileName);
	void processBinary(qrtext::core::ast::BinaryOperator const &node, const QString &templateFileName);
	void processTemplate(const qrtext::lua::ast::Node &node, const QString &templateFileName
			, QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> const &bindings);

	void pushResult(const qrtext::lua::ast::Node &node, const QString &generatedCode);
	QString popResult(const qrtext::lua::ast::Node &node, bool wrapIntoBrackets = false);
	QStringList popResults(const QList<QSharedPointer<qrtext::lua::ast::Node>> &nodes);

	bool printWithoutPop(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString toString(const QSharedPointer<qrtext::lua::ast::Node> &node);

	qrtext::LanguageToolboxInterface const &mTextLanguage;
	QMap<const qrtext::lua::ast::Node *, QString> mGeneratedCode;
	PrecedenceConverterInterface &mPrecedenceTable;
	const simple::Binding::ConverterInterface *mReservedVariablesConverter;  // Takes ownership
	ReservedFunctionsConverter mReservedFunctionsConverter;
};

}
}
