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
			, const qrtext::LanguageToolboxInterface &textLanguage
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
	void visit(const qrtext::lua::ast::Not &node) override;
	void visit(const qrtext::lua::ast::BitwiseNegation &node) override;
	void visit(const qrtext::lua::ast::Length &node) override;
	void visit(const qrtext::lua::ast::LogicalAnd &node) override;
	void visit(const qrtext::lua::ast::LogicalOr &node) override;
	void visit(const qrtext::lua::ast::Addition &node) override;
	void visit(const qrtext::lua::ast::Subtraction &node) override;
	void visit(const qrtext::lua::ast::Multiplication &node) override;
	void visit(const qrtext::lua::ast::Division &node) override;
	void visit(const qrtext::lua::ast::IntegerDivision &node) override;
	void visit(const qrtext::lua::ast::Modulo &node) override;
	void visit(const qrtext::lua::ast::Exponentiation &node) override;
	void visit(const qrtext::lua::ast::BitwiseAnd &node) override;
	void visit(const qrtext::lua::ast::BitwiseOr &node) override;
	void visit(const qrtext::lua::ast::BitwiseXor &node) override;
	void visit(const qrtext::lua::ast::BitwiseLeftShift &node) override;
	void visit(const qrtext::lua::ast::BitwiseRightShift &node) override;
	void visit(const qrtext::lua::ast::Concatenation &node) override;
	void visit(const qrtext::lua::ast::Equality &node) override;
	void visit(const qrtext::lua::ast::LessThan &node) override;
	void visit(const qrtext::lua::ast::LessOrEqual &node) override;
	void visit(const qrtext::lua::ast::Inequality &node) override;
	void visit(const qrtext::lua::ast::GreaterThan &node) override;
	void visit(const qrtext::lua::ast::GreaterOrEqual &node) override;
	void visit(const qrtext::lua::ast::IntegerNumber &node) override;
	void visit(const qrtext::lua::ast::FloatNumber &node) override;
	void visit(const qrtext::lua::ast::FieldInitialization &node) override;
	void visit(const qrtext::lua::ast::TableConstructor &node) override;
	void visit(const qrtext::lua::ast::String &node) override;
	void visit(const qrtext::lua::ast::True &node) override;
	void visit(const qrtext::lua::ast::False &node) override;
	void visit(const qrtext::lua::ast::Nil &node) override;
	void visit(const qrtext::lua::ast::Identifier &node) override;
	void visit(const qrtext::lua::ast::FunctionCall &node) override;
	void visit(const qrtext::lua::ast::MethodCall &node) override;
	void visit(const qrtext::lua::ast::Assignment &node) override;
	void visit(const qrtext::lua::ast::Block &node) override;
	void visit(const qrtext::lua::ast::IndexingExpression &node) override;

	/// Returns true if child expression must be wrapped into the brackets.
	/// The default implementation returns true if the parent operation has greater precedence or
	/// equal precedence and child is situated on the opposite side to operator`s associativity.
	/// May be redefined by printers to concrete language.
	virtual bool needBrackets(const qrtext::lua::ast::Node &parent
			, const qrtext::lua::ast::Node &child
			, qrtext::core::Associativity childAssociativity = qrtext::core::Associativity::right) const;

	void processUnary(const qrtext::core::ast::UnaryOperator &node, const QString &templateFileName);
	void processBinary(const qrtext::core::ast::BinaryOperator &node, const QString &templateFileName);
	void processTemplate(const qrtext::lua::ast::Node &node, const QString &templateFileName
			, QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> const &bindings);

	void pushResult(const qrtext::lua::ast::Node &node, const QString &generatedCode);
	QString popResult(const qrtext::lua::ast::Node &node, bool wrapIntoBrackets = false);
	QStringList popResults(const QList<QSharedPointer<qrtext::lua::ast::Node>> &nodes);

	bool printWithoutPop(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString toString(const QSharedPointer<qrtext::lua::ast::Node> &node);

	const qrtext::LanguageToolboxInterface &mTextLanguage;
	QMap<const qrtext::lua::ast::Node *, QString> mGeneratedCode;
	PrecedenceConverterInterface &mPrecedenceTable;
	const simple::Binding::ConverterInterface *mReservedVariablesConverter;  // Takes ownership
	ReservedFunctionsConverter mReservedFunctionsConverter;
};

}
}
