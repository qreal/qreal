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

#include <QtCore/QMap>

#include <qrtext/lua/ast/node.h>
#include <qrtext/core/types/typeExpression.h>

#include "generatorBase/lua/reservedFunctionsConverter.h"
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
	LuaPrinter(const QStringList &pathsToTemplates
			, const qrtext::LanguageToolboxInterface &textLanguage
			, PrecedenceConverterInterface &precedeceTable
			, const simple::Binding::ConverterInterface *reservedVariablesConverter);

	~LuaPrinter() override;

	/// Prints the given AST to the code using a set of templates placed in the given in the constructor directory.
	virtual QString print(const QSharedPointer<qrtext::lua::ast::Node> &node);

	/// Prints the given AST to the code on the target language and casts it to the given types.
	virtual QString castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
			, const QSharedPointer<qrtext::lua::ast::Node> &node);

private:
	void visit(const QSharedPointer<qrtext::lua::ast::Number> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::UnaryMinus> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Not> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseNegation> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Length> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LogicalAnd> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LogicalOr> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Addition> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Subtraction> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Multiplication> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Division> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::IntegerDivision> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Modulo> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Exponentiation> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseAnd> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseOr> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseXor> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseLeftShift> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseRightShift> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Concatenation> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Equality> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LessThan> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LessOrEqual> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Inequality> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::GreaterThan> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::GreaterOrEqual> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::IntegerNumber> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::FloatNumber> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::FieldInitialization> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::TableConstructor> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::String> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::True> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::False> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Nil> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Identifier> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::FunctionCall> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::MethodCall> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Assignment> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Block> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;
	void visit(const QSharedPointer<qrtext::lua::ast::IndexingExpression> &node
			, const QSharedPointer<qrtext::core::ast::Node> &parent) override;

	/// Returns true if child expression must be wrapped into the brackets.
	/// The default implementation returns true if the parent operation has greater precedence or
	/// equal precedence and child is situated on the opposite side to operator`s associativity.
	/// May be redefined by printers to concrete language.
	virtual bool needBrackets(const QSharedPointer<qrtext::lua::ast::Node> &parent
			, const QSharedPointer<qrtext::lua::ast::Node> &child
			, qrtext::core::Associativity childAssociativity = qrtext::core::Associativity::right) const;

	void processUnary(const QSharedPointer<qrtext::core::ast::UnaryOperator> &node, const QString &templateFileName);
	void processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node, const QString &templateFileName);
	void processTemplate(const QSharedPointer<qrtext::lua::ast::Node> &node, const QString &templateFileName
			, QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> const &bindings);

	void pushResult(const QSharedPointer<qrtext::lua::ast::Node> &node, const QString &generatedCode);
	QString popResult(const QSharedPointer<qrtext::lua::ast::Node> &node, bool wrapIntoBrackets = false);
	QStringList popResults(const QList<QSharedPointer<qrtext::lua::ast::Node>> &nodes);

	bool printWithoutPop(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString to(const QSharedPointer<qrtext::core::types::TypeExpression> &type
			, const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString toString(const QSharedPointer<qrtext::lua::ast::Node> &node);

	QStringList addSuffix(const QStringList &list);

	const qrtext::LanguageToolboxInterface &mTextLanguage;
	QMap<qrtext::lua::ast::Node *, QString> mGeneratedCode;
	PrecedenceConverterInterface &mPrecedenceTable;
	const simple::Binding::ConverterInterface *mReservedVariablesConverter;  // Takes ownership
	ReservedFunctionsConverter mReservedFunctionsConverter;
};

}
}
