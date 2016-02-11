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

#include "ev3LuaPrinter.h"

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
#include <qrtext/languageToolboxInterface.h>
#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>
#include <qrtext/lua/types/boolean.h>
#include <qrtext/lua/types/string.h>
#include <qrtext/lua/types/table.h>

using namespace ev3::rbf::lua;

const QMap<Ev3RbfType, QString> registerNames = {
	{ Ev3RbfType::data8, "_bool_temp_result_" }
	, { Ev3RbfType::data16, "_small_int_temp_result_" }
	, { Ev3RbfType::data32, "_int_temp_result_" }
	, { Ev3RbfType::dataF, "_float_temp_result_" }
	, { Ev3RbfType::dataS, "_string_temp_result_" }
	, { Ev3RbfType::array8, "_array8_temp_result_" }
	, { Ev3RbfType::array16, "_array16_temp_result_" }
	, { Ev3RbfType::array32, "_array32_temp_result_" }
	, { Ev3RbfType::arrayF, "_arrayF_temp_result_" }
};

const QMap<Ev3RbfType, QString> typeNames = {
	{ Ev3RbfType::data8, "8" }
	, { Ev3RbfType::data16, "16" }
	, { Ev3RbfType::data32, "32" }
	, { Ev3RbfType::dataF, "F" }
	, { Ev3RbfType::dataS, "S" }
};

Ev3LuaPrinter::Ev3LuaPrinter(const QStringList &pathsToTemplates
		, const qrtext::LanguageToolboxInterface &textLanguage
		, generatorBase::parts::Variables &variables)
	: generatorBase::TemplateParametrizedEntity(addSuffix(pathsToTemplates))
	, mTextLanguage(textLanguage)
	, mVariables(variables)
	, mReservedFunctionsConverter(pathsToTemplates)
{
}

Ev3LuaPrinter::~Ev3LuaPrinter()
{
}

void Ev3LuaPrinter::configure(const generatorBase::simple::Binding::ConverterInterface *reservedVariablesConverter)
{
	mReservedVariablesConverter.reset(reservedVariablesConverter);
}

QStringList Ev3LuaPrinter::addSuffix(const QStringList &list)
{
	QStringList result;
	for (const QString &path : list) {
		result << path + "/luaPrinting";
	}

	return result;
}

Ev3RbfType Ev3LuaPrinter::toEv3Type(const QSharedPointer<qrtext::core::types::TypeExpression> &type)
{
	if (type->is<qrtext::lua::types::Boolean>()) {
		return Ev3RbfType::data8;
	}

	if (type->is<qrtext::lua::types::Integer>()) {
		return Ev3RbfType::data32;
	}

	if (type->is<qrtext::lua::types::Float>()) {
		return Ev3RbfType::dataF;
	}

	if (type->is<qrtext::lua::types::String>()) {
		return Ev3RbfType::dataS;
	}

	if (type->is<qrtext::lua::types::Table>()) {
		const QSharedPointer<qrtext::core::types::TypeExpression> &tableType
				= static_cast<qrtext::lua::types::Table *>(type.data())->elementType();

		if (tableType->is<qrtext::lua::types::Boolean>()) {
			return Ev3RbfType::array8;
		}

		if (tableType->is<qrtext::lua::types::Integer>()) {
			return Ev3RbfType::array32;
		}

		if (tableType->is<qrtext::lua::types::Float>()) {
			return Ev3RbfType::arrayF;
		}
	}

	qWarning() << "Ev3LuaPrinter::typeOf: Unsupported type" << qUtf8Printable(type->toString());
	return Ev3RbfType::other;
}

Ev3RbfType Ev3LuaPrinter::typeOf(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	return toEv3Type(mTextLanguage.type(node));
}

bool Ev3LuaPrinter::isArray(Ev3RbfType type) const
{
	return type == Ev3RbfType::array8
			|| type == Ev3RbfType::array16
			|| type == Ev3RbfType::array32
			|| type == Ev3RbfType::arrayF;
}

Ev3RbfType Ev3LuaPrinter::elementType(Ev3RbfType arrayType) const
{
	switch (arrayType) {
	case Ev3RbfType::array8:
		return Ev3RbfType::data8;
	case Ev3RbfType::array16:
		return Ev3RbfType::data16;
	case Ev3RbfType::array32:
		return Ev3RbfType::data32;
	case Ev3RbfType::arrayF:
		return Ev3RbfType::dataF;
	default:
		return Ev3RbfType::other;
	}
}

QString Ev3LuaPrinter::newRegister(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	return newRegister(typeOf(node));
}

QString Ev3LuaPrinter::newRegister(Ev3RbfType type)
{
	if (type == Ev3RbfType::other) {
		return QString();
	}

	const QString result = registerNames[type] + QString::number(++mRegistersCount[mId][type]);
	const QString declarationTemplate = (type == Ev3RbfType::dataS)
			? "DATA%1 %2 255"
			: (isArray(type) ? QString("ARRAY%1 %2 255").arg(typeNames[elementType(type)], result) : "DATA%1 %2");
	mVariables.appendManualDeclaration(declarationTemplate.arg(typeNames[type], result));
	return result;
}

QString Ev3LuaPrinter::print(const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id)
{
	mId = id;
	mAdditionalCode.clear();
	return printWithoutPop(node) ? popResult(node) : QString();
}

QString Ev3LuaPrinter::castTo(const QSharedPointer<qrtext::core::types::TypeExpression> &type
		, const QSharedPointer<qrtext::lua::ast::Node> &node, const qReal::Id &id)
{
	mId = id;
	mAdditionalCode.clear();
	return printWithoutPop(node) ? castTo(toEv3Type(type), node) : QString();
}

QStringList Ev3LuaPrinter::additionalCode(const QSharedPointer<qrtext::core::ast::Node> &node) const
{
	return mAdditionalCode[node.data()];
}

QString Ev3LuaPrinter::constantsEvaluation() const
{
	QStringList code;
	for (const QString &constantName : mTextLanguage.specialConstants()) {
		const QString value = mTextLanguage.value<QString>(constantName);
		/// @todo: Rewrite this sh..t
		const Ev3RbfType type = value.contains(".") ? Ev3RbfType::dataF : Ev3RbfType::data32;
		const QString typeName = typeNames[type];
		code << readTemplate("assignment.t")
				.replace("@@TYPE1@@", typeName)
				.replace("@@TYPE2@@", typeName)
				.replace("@@VARIABLE@@", constantName)
				.replace("@@VALUE@@", type == Ev3RbfType::dataF ? value + "F" : value);
	}

	return code.join("\n");
}

void Ev3LuaPrinter::pushResult(const QSharedPointer<qrtext::lua::ast::Node> &node
		, const QString &generatedCode, const QString &additionalCode)
{
	mGeneratedCode[node.data()] = generatedCode;
	pushChildrensAdditionalCode(node);
	if (!additionalCode.isEmpty()) {
		mAdditionalCode[node.data()] << additionalCode;
	}
}

QString Ev3LuaPrinter::popResult(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	return mGeneratedCode.take(node.data());
}

QStringList Ev3LuaPrinter::popResults(const QList<QSharedPointer<qrtext::lua::ast::Node>> &nodes)
{
	QStringList result;
	for (const QSharedPointer<qrtext::lua::ast::Node> &node : nodes) {
		result << popResult(node);
	}

	return result;
}

void Ev3LuaPrinter::pushChildrensAdditionalCode(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	for (const auto &child : node->children()) {
		mAdditionalCode[node.data()] += mAdditionalCode.take(child.data());
	}
}

bool Ev3LuaPrinter::printWithoutPop(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	if (!node) {
		return false;
	}

	node->acceptRecursively(*this, node, qrtext::wrap(nullptr));
	if (mGeneratedCode.keys().count() != 1 || mGeneratedCode.keys().first() != node.data()) {
		QLOG_WARN() << "Lua printer got into the inconsistent state during printing."
				<< mGeneratedCode.keys().count() << "pieces of code:";
		for (const QString &code : mGeneratedCode.values()) {
			QLOG_INFO() << code;
		}

		mGeneratedCode.clear();
		return false;
	}

	return true;
}

void Ev3LuaPrinter::processTemplate(const QSharedPointer<qrtext::lua::ast::Node> &node
		, const QString &templateFileName
		, QMap<QString, QSharedPointer<qrtext::lua::ast::Node>> const &bindings
		, const QMap<QString, QString> &staticBindings)
{
	QString computation = readTemplate(templateFileName);
	const bool typedComputation = computation.contains("@@RESULT@@");
	QString result;
	if (typedComputation) {
		result = newRegister(node);
		computation.replace("@@RESULT@@", result);
	}

	for (const QString &toReplace : bindings.keys()) {
		computation.replace(toReplace, popResult(bindings[toReplace]));
	}

	for (const QString &toReplace : staticBindings.keys()) {
		computation.replace(toReplace, staticBindings[toReplace]);
	}

	pushResult(node, result, computation);
}

void Ev3LuaPrinter::processUnary(const QSharedPointer<qrtext::core::ast::UnaryOperator> &node
		, const QString &templateFileName)
{
	const Ev3RbfType type = typeOf(node);
	QString templateCode = readTemplate(templateFileName);
	const bool hasAdditionalCode = templateCode.contains("@@RESULT@@");
	const QString result = hasAdditionalCode ? newRegister(type) : QString();
	const QString code = templateCode
			.replace("@@TYPE@@", typeNames[type])
			.replace("@@OPERAND@@", popResult(node->operand()))
			.replace("@@RESULT@@", result);
	pushResult(node, hasAdditionalCode ? result : code, hasAdditionalCode ? code : QString());
}

void Ev3LuaPrinter::processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node
		, Ev3RbfType operandsType, Ev3RbfType resultType, const QString &templateFileName)
{
	const QString result = newRegister(resultType);
	pushResult(node, result, readTemplate(templateFileName)
			.replace("@@TYPE@@", typeNames[operandsType])
			.replace("@@RESULT@@", result)
			.replace("@@LEFT@@", castTo(operandsType, node->leftOperand()))
			.replace("@@RIGHT@@", castTo(operandsType, node->rightOperand())));
}

void Ev3LuaPrinter::processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node
		, Ev3RbfType resultType, const QString &templateFileName)
{
	const Ev3RbfType operandsType = moreCommon(typeOf(node->leftOperand()), typeOf(node->rightOperand()));
	processBinary(node, operandsType, resultType, templateFileName);
}

void Ev3LuaPrinter::processBinary(const QSharedPointer<qrtext::core::ast::BinaryOperator> &node
		, const QString &templateFileName)
{
	const Ev3RbfType type = moreCommon(typeOf(node->leftOperand()), typeOf(node->rightOperand()));
	processBinary(node, type, type, templateFileName);
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Number> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, node->stringRepresentation(), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::UnaryMinus> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "unaryMinus.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Not> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "not.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseNegation> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "bitwiseNegation.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Length> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processUnary(node, "length.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LogicalAnd> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, Ev3RbfType::data8, "logicalAnd.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LogicalOr> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, Ev3RbfType::data8, "logicalOr.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Addition> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, "addition.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Subtraction> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, "subtraction.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Multiplication> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, "multiplication.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Division> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::dataF, Ev3RbfType::dataF, "division.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::IntegerDivision> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data32, Ev3RbfType::data32, "integerDivision.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Modulo> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data32, Ev3RbfType::data32, "modulo.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Exponentiation> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::dataF, Ev3RbfType::dataF, "exponentiation.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseAnd> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data32, Ev3RbfType::data32, "bitwiseAnd.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseOr> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data32, Ev3RbfType::data32, "bitwiseOr.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseXor> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data32, Ev3RbfType::data32, "bitwiseXor.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseLeftShift> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data32, Ev3RbfType::data32, "bitwiseLeftShift.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::BitwiseRightShift> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data32, Ev3RbfType::data32, "bitwiseRightShift.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Concatenation> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QString result = newRegister(node);
	pushResult(node, result, readTemplate("concatenation.t")
			.replace("@@RESULT@@", result)
			.replace("@@LEFT@@", toString(node->leftOperand()))
			.replace("@@RIGHT@@", toString(node->rightOperand())));
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Equality> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, "equality.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LessThan> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, "lessThan.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::LessOrEqual> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, "lessOrEqual.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Inequality> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, "inequality.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::GreaterThan> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, "greaterThan.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::GreaterOrEqual> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	processBinary(node, Ev3RbfType::data8, "greaterOrEqual.t");
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::IntegerNumber> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, node->stringRepresentation(), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::FloatNumber> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, node->stringRepresentation() + "F", QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::FieldInitialization> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QString initializer = readTemplate("writeIndexer.t")
			.replace("@@INDEX@@", node->key() ? popResult(node->key()) : QString::number(++mTableInitializersCount))
			.replace("@@VALUE@@", popResult(node->value()));
	pushResult(node, initializer, QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::TableConstructor> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const auto &type = mTextLanguage.type(qrtext::as<qrtext::core::ast::Node>(node));
	const auto &elementType = static_cast<qrtext::lua::types::Table *>(type.data())->elementType();
	const QString elementTypeName = typeNames[toEv3Type(elementType)];
	mTableInitializersCount = -1;
	QStringList initializers = popResults(qrtext::as<qrtext::lua::ast::Node>(node->initializers()));
	const QString result = newRegister(node);
	for (int i = 0; i < initializers.count(); ++i) {
		initializers[i].replace("@@TABLE@@", result);
	}

	pushResult(node, result, readTemplate("tableConstructor.t")
			.replace("@@TYPE@@", elementTypeName)
			.replace("@@RESULT@@", result) + initializers.join("\n"));
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::String> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("string.t").replace("@@VALUE@@", node->string()), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::True> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("true.t"), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::False> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("false.t"), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Nil> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	pushResult(node, readTemplate("nil.t"), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Identifier> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	QString additionalCode;
	QString result = mReservedVariablesConverter->convert(node->name());
	if (result != node->name()) {
		const QString registerName = newRegister(node);
		additionalCode = result.replace("@@RESULT@@", registerName);
		result = registerName;
	}

	pushResult(node, result, additionalCode);
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::FunctionCall> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QString expression = popResult(node->function());

	const QStringList unused = {"print"};
	const QStringList int32Functions = {"time", "sgn"};
	const QStringList shouldCastToIntAfterFunctions = {"ceil", "floor"};
	bool shouldCastToIntAfter = false;

	QStringList arguments;
	QString reservedFunctionCall;
	Ev3RbfType type = Ev3RbfType::dataF;

	if (auto idNode = dynamic_cast<qrtext::lua::ast::Identifier *>(node->function().data())) {
		if (unused.contains(idNode->name())) {
			arguments = popResults(qrtext::as<qrtext::lua::ast::Node>(node->arguments()));
		} else {
			shouldCastToIntAfter = shouldCastToIntAfterFunctions.contains(idNode->name());
			if (int32Functions.contains(idNode->name())) {
				type = Ev3RbfType::data32;
			}

			// All arguments are considered float here due to bytecode spec.
			for (auto &argument : node->arguments()) {
				arguments << castTo(Ev3RbfType::dataF, qrtext::as<qrtext::lua::ast::Node>(argument));
			}

			reservedFunctionCall = addRandomIds(mReservedFunctionsConverter.convert(idNode->name(), arguments));
		}
	}

	const QString functionResult = newRegister(type);
	QString result = shouldCastToIntAfter ? newRegister(Ev3RbfType::data32) : functionResult;

	if (reservedFunctionCall.isEmpty()) {
		pushResult(node, result, readTemplate("functionCall.t")
				.replace("@@FUNCTION@@", expression)
				.replace("@@ARGUMENTS@@", arguments.join(readTemplate("argumentsSeparator.t")))
				.replace("@@RESULT@@", functionResult));
	} else {
		pushResult(node, result, reservedFunctionCall.replace("@@RESULT@@", functionResult));
		if (shouldCastToIntAfter) {
			mAdditionalCode[node.data()] << QString("MOVEF_32(%1, %2)").arg(functionResult, result);
		}
	}
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::MethodCall> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QString object = popResult(node->object());
	const QString method = popResult(node->methodName());
	const QStringList arguments = popResults(qrtext::as<qrtext::lua::ast::Node>(node->arguments()));
	pushResult(node, readTemplate("methodCall.t")
			.replace("@@OBJECT@@", object)
			.replace("@@METHOD@@", method)
			.replace("@@ARGUMENTS@@", arguments.join(readTemplate("argumentsSeparator.t"))), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Assignment> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	if (dynamic_cast<qrtext::lua::ast::IndexingExpression *>(node->variable().data())) {
		// We are dealing with write-indexer here. Template for result is already ready,
		// we just need to replace @@VALUE@@ there.
		QString writeTemplate = popResult(node->variable());
		pushResult(node, writeTemplate.replace("@@VALUE@@", popResult(node->value())), QString());
		return;
	}

	const Ev3RbfType type = typeOf(node->variable());
	const bool isString = type == Ev3RbfType::dataS;
	const bool isArray = this->isArray(type);
	processTemplate(node, isString ? "assignmentStrings.t" : (isArray ? "assignmentArrays.t" : "assignment.t")
			, { {"@@VARIABLE@@", node->variable()}, {"@@VALUE@@", node->value()} }
			, { {"@@TYPE1@@", typeNames[typeOf(node->variable())]}
			,   {"@@TYPE2@@", typeNames[typeOf(node->value())]} }
	);
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::Block> &node
		, const QSharedPointer<qrtext::core::ast::Node> &)
{
	const QStringList expressions = popResults(node->children());
	pushResult(node, expressions.join(readTemplate("statementsSeparator.t")), QString());
}

void Ev3LuaPrinter::visit(const QSharedPointer<qrtext::lua::ast::IndexingExpression> &node
		, const QSharedPointer<qrtext::core::ast::Node> &parent)
{
	bool isWriteIndexer = false;
	if (auto assignment = dynamic_cast<qrtext::lua::ast::Assignment *>(parent.data())) {
		isWriteIndexer = assignment->variable() == node;
	}

	const QString indexerTemplate = isWriteIndexer ? "writeIndexer.t" : "readIndexer.t";
	processTemplate(node, indexerTemplate, { {"@@TABLE@@", node->table()}, {"@@INDEX@@", node->indexer()} });
}

QString Ev3LuaPrinter::toString(const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	const QSharedPointer<qrtext::core::types::TypeExpression> type = mTextLanguage.type(node);
	const QString value = popResult(node);
	if (type->is<qrtext::lua::types::String>()) {
		return value;
	}

	QString code;
	if (type->is<qrtext::lua::types::Integer>()) {
		code = readTemplate("intToString.t").replace("@@VALUE@@", value);
	} else if (type->is<qrtext::lua::types::Float>()) {
		code = readTemplate("floatToString.t").replace("@@VALUE@@", value);
	} else if (type->is<qrtext::lua::types::Integer>()) {
		code = readTemplate("boolToString.t").replace("@@VALUE@@", value);
	} else {
		code = readTemplate("otherToString.t").replace("@@VALUE@@", value);
	}

	const QString result = newRegister(Ev3RbfType::dataS);
	pushChildrensAdditionalCode(node);
	mAdditionalCode[node.data()] << code.replace("@@RESULT@@", result);
	return result;
}

QString Ev3LuaPrinter::castTo(Ev3RbfType targetType, const QSharedPointer<qrtext::lua::ast::Node> &node)
{
	const Ev3RbfType actualType = typeOf(node);
	if (targetType == Ev3RbfType::dataS) {
		return toString(node);
	}

	const QString value = popResult(node);
	if (actualType == targetType) {
		return value;
	}

	if (actualType == Ev3RbfType::dataS) {
		return QObject::tr("/* Warning: cast from string to numeric type is not supported */ 0");
	}

	if (isArray(actualType)) {
		return QObject::tr("/* Warning: autocast from array to other type is not supported */ 0");
	}

	if (targetType == Ev3RbfType::other || actualType == Ev3RbfType::other) {
		return QObject::tr("/* Warning: autocast is supported only for numeric types */ 0");
	}

	const QString result = newRegister(targetType);
	pushChildrensAdditionalCode(node);
	mAdditionalCode[node.data()] << QString("MOVE%1_%2(%3, %4)")
			.arg(typeNames[actualType], typeNames[targetType], value, result);
	return result;
}

Ev3RbfType Ev3LuaPrinter::moreCommon(Ev3RbfType type1, Ev3RbfType type2) const
{
	return static_cast<Ev3RbfType>(qMax(static_cast<Ev3RbfType>(type1), static_cast<Ev3RbfType>(type2)));
}
