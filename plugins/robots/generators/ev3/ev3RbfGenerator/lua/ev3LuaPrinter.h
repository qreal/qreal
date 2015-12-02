/* Copyright 2015 CyberTech Labs Ltd.
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
#include <generatorBase/lua/precedenceConverterInterface.h>
#include <generatorBase/templateParametrizedEntity.h>
#include <generatorBase/simpleGenerators/binding.h>
#include <generatorBase/parts/variables.h>

#include "ev3ReservedFunctionsConverter.h"

namespace qrtext {
class LanguageToolboxInterface;
}

namespace ev3 {
namespace rbf {
namespace lua {

enum class Ev3RbfType {
	data8 = 0
	, data16
	, data32
	, dataF
	, dataS
	, other
};

class Ev3LuaPrinter : public qrtext::lua::LuaAstVisitorInterface, public generatorBase::TemplateParametrizedEntity
{
public:
	/// Takes ownership on converters.
	Ev3LuaPrinter(const QStringList &pathsToTemplates
			, const qrtext::LanguageToolboxInterface &textLanguage
			, generatorBase::parts::Variables &variables
			, const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter);

	~Ev3LuaPrinter() override;

	/// Prints the given AST to the code using a set of templates placed in the given in the constructor directory.
	QString print(const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id);

	/// Prints the given AST to the code on the target language and casts it to the given type.
	QString castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
			, const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id);

	/// Returns code that must be prepended to generated code correspondning to current block.
	/// Must be called after print() call or any cast function call.
	QStringList additionalCode(const qReal::Id &id) const;

private:
	void visit(const QSharedPointer<qrtext::lua::ast::Number> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::UnaryMinus> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Not> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseNegation> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Length> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LogicalAnd> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LogicalOr> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Addition> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Subtraction> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Multiplication> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Division> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::IntegerDivision> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Modulo> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Exponentiation> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseAnd> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseOr> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseXor> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseLeftShift> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::BitwiseRightShift> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Concatenation> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Equality> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LessThan> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::LessOrEqual> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Inequality> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::GreaterThan> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::GreaterOrEqual> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::IntegerNumber> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::FloatNumber> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::FieldInitialization> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::TableConstructor> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::String> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::True> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::False> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Nil> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Identifier> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::FunctionCall> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::MethodCall> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Assignment> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::Block> &node) override;
	void visit(const QSharedPointer<qrtext::lua::ast::IndexingExpression> &node) override;

	void processUnary(const QSharedPointer<qrtext::core::ast::UnaryOperator> &node, const QString &templateFileName);
	void processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node, Ev3RbfType operandsType
			, Ev3RbfType resultType, const QString &templateFileName);
	void processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node
			, Ev3RbfType resultType, const QString &templateFileName);
	void processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node, const QString &templateFileName);
	void processTemplate(const QSharedPointer<qrtext::lua::ast::Node> &node, const QString &templateFileName
			, const QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> &bindings
			, const QMap<QString, QString> &staticBindings = {{}});

	void pushResult(const QSharedPointer<qrtext::lua::ast::Node> &node
			, const QString &generatedCode, const QString &additionalCode);
	QString popResult(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QStringList popResults(const QList<QSharedPointer<qrtext::lua::ast::Node>> &nodes);

	bool printWithoutPop(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString toString(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString castTo(Ev3RbfType targetType, const QSharedPointer<qrtext::lua::ast::Node> &node);
	Ev3RbfType moreCommon(Ev3RbfType type1, Ev3RbfType type2) const;

	QStringList addSuffix(const QStringList &list);
	Ev3RbfType toEv3Type(const QSharedPointer<qrtext::core::types::TypeExpression> &type);
	Ev3RbfType typeOf(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString newRegister(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString newRegister(Ev3RbfType type);

	const qrtext::LanguageToolboxInterface &mTextLanguage;
	generatorBase::parts::Variables &mVariables;
	QMap<qrtext::lua::ast::Node *, QString> mGeneratedCode;
	qReal::Id mId;
	QMap<qReal::Id, QStringList> mAdditionalCode;
	QMap<qReal::Id, QMap<Ev3RbfType, int>> mRegistersCount;
	const generatorBase::simple::Binding::ConverterInterface *mReservedVariablesConverter;  // Takes ownership
	Ev3ReservedFunctionsConverter mReservedFunctionsConverter;
};

}
}
}
