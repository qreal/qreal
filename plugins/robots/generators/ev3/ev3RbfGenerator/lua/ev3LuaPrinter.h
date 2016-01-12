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
#include <generatorBase/lua/reservedFunctionsConverter.h>
#include <generatorBase/templateParametrizedEntity.h>
#include <generatorBase/simpleGenerators/binding.h>
#include <generatorBase/parts/variables.h>

namespace qrtext {
class LanguageToolboxInterface;
}

namespace ev3 {
namespace rbf {
namespace lua {

/// Represents type of variables available in EV3 bytecode.
enum class Ev3RbfType
{
	/// 8-bit integer
	data8 = 0
	/// 16-bit integer
	, data16
	/// 32-bit integer
	, data32
	/// Float data
	, dataF
	/// String
	, dataS
	/// 8-bit integer array
	, array8
	/// 16-bit integer array
	, array16
	/// 32-bit integer array
	, array32
	/// Float array
	, arrayF
	/// Something unknown
	, other
};

class Ev3LuaPrinter : public qrtext::lua::LuaAstVisitorInterface, public generatorBase::TemplateParametrizedEntity
{
public:
	Ev3LuaPrinter(const QStringList &pathsToTemplates
			, const qrtext::LanguageToolboxInterface &textLanguage
			, generatorBase::parts::Variables &variables);

	~Ev3LuaPrinter() override;

	/// Cofigures current printer instance to use the given reserved variables converter.
	/// Takes ownership over \a reservedVariablesConverter.
	void configure(const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter);

	/// Prints the given AST to the code using a set of templates placed in the given in the constructor directory.
	QString print(const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id);

	/// Prints the given AST to the code on the target language and casts it to the given type.
	QString castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
			, const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id);

	/// Returns code that must be prepended to generated code correspondning to current block.
	/// Must be called after print() call or castTo() function call.
	QStringList additionalCode(const QSharedPointer<qrtext::core::ast::Node> &node) const;

	/// Returns code that initializes all known constants.
	QString constantsEvaluation() const;

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
	void pushChildrensAdditionalCode(const QSharedPointer<qrtext::lua::ast::Node> &node);

	bool printWithoutPop(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString toString(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString castTo(Ev3RbfType targetType, const QSharedPointer<qrtext::lua::ast::Node> &node);
	Ev3RbfType moreCommon(Ev3RbfType type1, Ev3RbfType type2) const;

	QStringList addSuffix(const QStringList &list);
	Ev3RbfType toEv3Type(const QSharedPointer<qrtext::core::types::TypeExpression> &type);
	Ev3RbfType typeOf(const QSharedPointer<qrtext::lua::ast::Node> &node);
	bool isArray(Ev3RbfType type) const;
	Ev3RbfType elementType(Ev3RbfType arrayType) const;
	QString newRegister(const QSharedPointer<qrtext::lua::ast::Node> &node);
	QString newRegister(Ev3RbfType type);

	const qrtext::LanguageToolboxInterface &mTextLanguage;
	generatorBase::parts::Variables &mVariables;
	QMap<qrtext::lua::ast::Node *, QString> mGeneratedCode;
	QMap<qrtext::lua::ast::Node *, QStringList> mAdditionalCode;
	qReal::Id mId;
	int mTableInitializersCount = -1;
	QMap<qReal::Id, QMap<Ev3RbfType, int>> mRegistersCount;
	QScopedPointer<const generatorBase::simple::Binding::ConverterInterface> mReservedVariablesConverter;
	generatorBase::lua::ReservedFunctionsConverter mReservedFunctionsConverter;
};

}
}
}
