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

#include "expressionsParser.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace utils;
using namespace qReal;

ExpressionsParser::ExpressionsParser(ErrorReporterInterface *errorReporter)
	: mHasParseErrors(false), mErrorReporter(errorReporter), mCurrentId (Id::rootId())
{
	srand(time(nullptr));
}

ExpressionsParser::~ExpressionsParser()
{
	qDeleteAll(mVariables);
}

QMap<QString, Number *> const &ExpressionsParser::variables() const
{
	return mVariables;
}

QMap<QString, Number *> &ExpressionsParser::mutableVariables()
{
	return mVariables;
}

bool ExpressionsParser::isDigit(const QChar &c) const
{
	char symbol = c.toLatin1();
	return '0' <= symbol && symbol <= '9';
}

bool ExpressionsParser::isSign(const QChar &c) const
{
	char symbol = c.toLatin1();
	return symbol == '-' || symbol == '+';
}

bool ExpressionsParser::isLetter(const QChar &c) const
{
	char symbol = c.toLatin1();
	return ('A' <= symbol && symbol <= 'Z') || ('a'<= symbol && symbol <= 'z');
}

bool ExpressionsParser::isExp(const QChar &c) const
{
	char symbol = c.toLatin1();
	return symbol == 'e' || symbol == 'E';
}

bool ExpressionsParser::isPoint(const QChar &c) const
{
	return c.toLatin1() == '.';
}

bool ExpressionsParser::isRoundBracket(const QChar &c) const
{
	char symbol = c.toLatin1();
	return symbol == '(' || symbol == ')';
}

bool ExpressionsParser::isDisjunction(const QChar &c) const
{
	return c.toLatin1() == '|';
}

bool ExpressionsParser::isConjunction(const QChar &c) const
{
	return c.toLatin1() == '&';
}

bool ExpressionsParser::isComparison(const QChar &c) const
{
	char symbol = c.toLatin1();
	return symbol == '<' || symbol == '>';
}

bool ExpressionsParser::isArithmeticalMinusOrPlus(const QChar &c) const
{
	char symbol = c.toLatin1();
	return symbol == '-' || symbol == '+';
}

bool ExpressionsParser::isMultiplicationOrDivision(const QChar &c) const
{
	char symbol = c.toLatin1();
	return symbol == '*' || symbol == '/';
}

bool ExpressionsParser::isDelimiter(const QChar &c) const
{
	char symbol = c.toLatin1();
	return symbol == '\n' || symbol == '\r' || symbol == ' ' || symbol == '\t';
}

bool ExpressionsParser::isAssignment(const QChar &c) const
{
	return c.toLatin1() == '=';
}

Number *ExpressionsParser::parseNumber(const QString &stream, int &pos)
{
	int beginPos = pos;
	bool isDouble = false;
	if (pos < stream.length() && isSign(stream.at(pos))) {
		pos++;
	}

	if (!checkForDigit(stream, pos)) {
		return new Number();
	}

	while (pos < stream.length() && isDigit(stream.at(pos))) {
		pos++;
	}
	if (pos < stream.length() && isPoint(stream.at(pos))) {
		isDouble = true;
		pos++;

		if (!checkForDigit(stream, pos)) {
			return new Number();
		}

		while (pos < stream.length() && isDigit(stream.at(pos))) {
			pos++;
		}
	}
	if (pos < stream.length() && isExp(stream.at(pos))) {
		isDouble = true;
		pos++;

		if (isEndOfStream(stream, pos)) {
			return new Number();
		}

		if (pos < stream.length() && isSign(stream.at(pos))) {
			pos++;
		}

		if (!checkForDigit(stream, pos)) {
			return new Number();
		}

		while (pos < stream.length() && isDigit(stream.at(pos))) {
			pos++;
		}
	}
	if (isDouble) {
		return new Number(stream.mid(beginPos, pos - beginPos).toDouble(), Number::doubleType);
	} else {
		return new Number(stream.mid(beginPos, pos - beginPos).toInt(), Number::intType);
	}
}

QString ExpressionsParser::parseIdentifier(const QString &stream, int &pos)
{
	int beginPos = pos;
	if (checkForLetter(stream, pos)) {
		pos++;
		while (pos < stream.length() && (isDigit(stream.at(pos)) || isLetter(stream.at(pos)))) {
			pos++;
		}
		return stream.mid(beginPos, pos - beginPos).trimmed();
	}
	return "";
}

void ExpressionsParser::skip(const QString &stream, int &pos) const
{
	while (pos < stream.length() &&
		   (isDelimiter(stream.at(pos)) || stream.at(pos).toLatin1() == '<' ))
	{
		if (isHtmlBrTag(stream, pos)) {
			pos += 4;
			return;
		}
		if (stream.at(pos).toLatin1() == '<') {
			return;
		}
		pos++;
	}
}

bool ExpressionsParser::isHtmlBrTag(const QString &stream, int &pos) const
{
	if (pos + 3 < stream.length()) {
		return stream.at(pos).toLatin1() == '<'
				&& stream.at(pos + 1).toLatin1() == 'b'
				&& stream.at(pos + 2).toLatin1() == 'r'
				&& stream.at(pos + 3).toLatin1() == '>';
	} else {
		return false;
	}
}

Number *ExpressionsParser::parseTerm(const QString &stream, int &pos)
{
	Number *res = nullptr;
	skip(stream, pos);

	if (hasErrors() || isEndOfStream(stream, pos)) {
		return new Number();
	}

	switch (stream.at(pos).toLatin1()) {
	case '+':
		pos++;
		skip(stream, pos);
		res = parseTerm(stream, pos);
		break;
	case '-':
		pos++;
		skip(stream, pos);
		res = new Number(-(*parseTerm(stream, pos)));
		break;
	case '(':
		pos++;
		skip(stream, pos);
		res = parseExpression(stream, pos);
		skip(stream, pos);
		if (!checkForClosingBracket(stream, pos)) {
			return res;
		}
		pos++;
		break;
	default:
		if (isDigit(stream.at(pos))) {
			res = parseNumber(stream, pos);
		} else if (isLetter(stream.at(pos))) {
			int unknownIdentifierIndex = pos;
			QString variable = parseIdentifier(stream, pos);
			if (isFunction(variable)) {
				skip(stream, pos);
				if (checkForOpeningBracket(stream, pos)) {
					pos++;
					Number *value = parseExpression(stream, pos);
					if (checkForClosingBracket(stream, pos)) {
						pos++;
						res = applyFunction(variable, value);
						delete value;
					}
				}
			} else if (mVariables.contains(variable)) {
				res = new Number(mVariables[variable]->value(), mVariables[variable]->type());
			} else {
				error(unknownIdentifier, QString::number(unknownIdentifierIndex + 1), "", variable);
			}
		} else {
			error(unexpectedSymbol, QString::number(pos + 1)
					, "\'digit\' or \'letter\' or \'bracket\' or \'sign\'", QString(stream.at(pos)));
		}
		break;
	}
	skip(stream, pos);
	if (!res) {
		res = new Number();
	}

	return res;
}

Number *ExpressionsParser::parseMult(const QString &stream, int &pos)
{
	Number *res = parseTerm(stream, pos);
	while (pos < stream.length() && isMultiplicationOrDivision(stream.at(pos))) {
		pos++;
		switch (stream.at(pos - 1).toLatin1()) {
		case '*':
			*res *= *parseTerm(stream, pos);
			break;
		case '/':
			{
				Number *divisor = parseTerm(stream, pos);
				if (divisor->type() == Number::intType && divisor->value().toInt() == 0) {
					error(divisionByZero);
				} else {
					*res /= *divisor;
				}

				break;
			}
		}
	}

	return res;
}

Number *ExpressionsParser::parseExpression(const QString &stream, int &pos)
{
	Number *res = parseMult(stream, pos);
	while (pos < stream.length() && isArithmeticalMinusOrPlus(stream.at(pos))) {
		pos++;
		switch (stream.at(pos - 1).toLatin1()) {
		case '+':
			*res += *parseMult(stream, pos);
			break;
		case '-':
			*res -= *parseMult(stream, pos);
			break;
		}
	}

	return res;
}

void ExpressionsParser::parseVarPart(const QString &stream, int &pos)
{
	Q_UNUSED(stream);
	Q_UNUSED(pos);
}

void ExpressionsParser::parseCommand(const QString &stream, int &pos)
{
	int typesMismatchIndex = pos;
	QString variable = parseIdentifier(stream, pos);
	skip(stream, pos);
	if (hasErrors() || isEndOfStream(stream, pos)) {
		return;
	}
	checkForVariable(variable, typesMismatchIndex);
	if (checkForUsingReservedVariables(variable))
		return;

	if (isAssignment(stream.at(pos))) {
		pos++;
		Number *n = parseExpression(stream, pos);
		if (!hasErrors()) {
			const bool containsVariable = mVariables.keys().contains(variable);
			const Number::Type t1 = containsVariable ? mVariables[variable]->type() : Number::intType;
			const Number::Type t2 = n->type();
			if (!containsVariable || t1 == t2) {
				mVariables[variable] = n;
			} else {
				if (t1 == Number::intType) {
					mVariables[variable]->setValue(n->value().toInt());
					error(typesMismatch, QString::number(typesMismatchIndex + 1), "\'int\'", "\'double\'");
				} else {
					mVariables[variable]->setValue(n->value().toDouble());
				}

				delete n;
			}
		}
	} else {
		error(unexpectedSymbol, QString::number(pos+1), "=", QString(stream.at(pos)));
		return;
	}
	if (!hasErrors() && checkForColon(stream, pos)) {
		pos++;
	}
}

void ExpressionsParser::parseProcess(const QString &stream, int &pos, const Id &curId)
{
	mCurrentId = curId;

	if (isEmpty(stream, pos)) {
		error(emptyProcess);
		return;
	}
	parseVarPart(stream, pos);
	if (hasErrors()) {
		return;
	}
	while (pos < stream.length() && !hasErrors()) {
		parseCommand(stream, pos);
		skip(stream, pos);
	}
}

bool ExpressionsParser::parseSingleComprasion(const QString &stream, int &pos)
{
	Number *left = parseExpression(stream, pos);
	Number *right = nullptr;
	if (hasErrors() || isEndOfStream(stream, pos)) {
		return false;
	}

	switch (stream.at(pos).toLatin1()) {
	case '=':
		pos++;
		if (checkForEqual(stream, pos)) {
			pos++;
			right = parseExpression(stream, pos);
			return *left == *right;
		} else {
			return false;
		}
		break;
	case '!':
		pos++;
		if (checkForEqual(stream, pos)) {
			pos++;
			right = parseExpression(stream, pos);
			return *left != *right;
		} else {
			return false;
		}
		break;
	case '<':
		pos++;
		if (pos < stream.length() && stream.at(pos).toLatin1() == '=') {
			pos++;
			right = parseExpression(stream, pos);
			return *left <= *right;
		} else {
			right = parseExpression(stream, pos);
			return *left < *right;
		}
		break;
	case '>':
		pos++;
		if (pos < stream.length() && stream.at(pos).toLatin1() == '=') {
			pos++;
			right = parseExpression(stream, pos);
			return *left >= *right;
		} else {
			right = parseExpression(stream, pos);
			return *left > *right;
		}
		break;
	}

	error(unexpectedSymbol, QString::number(pos + 1), "=\',\'!\',\'>\',\'<"
			, QString(stream.at(pos)));
	return false;
}

bool ExpressionsParser::parseDisjunction(const QString &stream, int &pos)
{
	bool res = false;
	skip(stream, pos);
	int index = stream.indexOf(')', pos);

	switch (stream.at(pos).toLatin1()) {
	case '(':
		if ((index < stream.indexOf('<', pos) || stream.indexOf('<', pos) == -1) &&
				(index < stream.indexOf('>', pos) || stream.indexOf('>', pos) == -1) &&
				(index < stream.indexOf('=', pos) || stream.indexOf('=', pos) == -1))
		{
			res = parseSingleComprasion(stream, pos);
		} else {
			pos++;
			res = parseConditionHelper(stream, pos);
			skip(stream, pos);
			if (!hasErrors() && checkForClosingBracket(stream, pos)) {
				pos++;
			}
		}
		break;
	case '!':
		pos++;
		skip(stream, pos);
		if (hasErrors() || !checkForOpeningBracket(stream, pos)) {
			return res;
		}
		pos++;
		res = !(parseConditionHelper(stream, pos));

		if (hasErrors() || !checkForClosingBracket(stream, pos)) {
			return res;
		}
		pos++;
		break;
	default:
		if (isDigit(stream.at(pos)) || isLetter(stream.at(pos))) {
			res = parseSingleComprasion(stream, pos);
		} else {
			error(unexpectedSymbol, QString::number(pos + 1)
				, "\'digit\' or \'letter\' or \'sign\'", QString(stream.at(pos)));
		}
		break;
	}
	skip(stream, pos);
	return res;
}

bool ExpressionsParser::parseConjunction(const QString &stream, int &pos)
{
	bool res = parseDisjunction(stream, pos);
	while (pos < (stream.length()-1) && isConjunction(stream.at(pos))) {
		pos++;
		if (isConjunction(stream.at(pos))) {
			pos++;
			res = parseDisjunction(stream, pos) && res;
		} else {
			error(unexpectedSymbol, QString::number(pos + 1), "&", QString(stream.at(pos)));
			return res;
		}
	}
	return res;
}

bool ExpressionsParser::parseConditionHelper(const QString &stream, int &pos)
{
	bool res = parseConjunction(stream, pos);
	while (pos < (stream.length()-1) && isDisjunction(stream.at(pos))) {
		pos++;
		if (isDisjunction(stream.at(pos))) {
			pos++;
			res = parseConjunction(stream, pos) || res;
		} else {
			error(unexpectedSymbol, QString::number(pos + 1), "|", QString(stream.at(pos)));
			return res;
		}
	}
	return res;
}

bool ExpressionsParser::parseCondition(const QString &stream, int &pos, const Id &curId)
{
	mCurrentId = curId;
	if (isEmpty(stream, pos)) {
		error(emptyCondition);
		return false;
	}

	bool res = parseConditionHelper(stream, pos);
	skip(stream, pos);
	if (!hasErrors() && pos < stream.length()) {
		error(unexpectedSymbol, QString::number(pos), "null string", QString(stream.at(pos)));
	}
	return res;
}

ErrorReporterInterface& ExpressionsParser::getErrors()
{
	return *mErrorReporter;
}

bool ExpressionsParser::hasErrors() const
{
	return mHasParseErrors;
}

bool ExpressionsParser::isEndOfStream(const QString &stream, int &pos)
{
	if (pos == stream.length()) {
		error(unexpectedEndOfStream, QString::number(pos + 1));
		return true;
	}
	return false;
}

bool ExpressionsParser::checkForLetter(const QString &stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (!isLetter(stream.at(pos))) {
		error(unexpectedSymbol, QString::number(pos + 1), "letter", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool ExpressionsParser::checkForDigit(const QString &stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (!isDigit(stream.at(pos))) {
		error(unexpectedSymbol, QString::number(pos + 1), "digit", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool ExpressionsParser::checkForOpeningBracket(const QString &stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toLatin1() != '(') {
		error(unexpectedSymbol, QString::number(pos + 1), "(", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool ExpressionsParser::checkForClosingBracket(const QString &stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toLatin1() != ')') {
		error(unexpectedSymbol, QString::number(pos + 1), ")", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool ExpressionsParser::checkForColon(const QString &stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toLatin1() != ';') {
		error(unexpectedSymbol, QString::number(pos + 1), ";", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool ExpressionsParser::checkForEqual(const QString &stream, int pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toLatin1() != '=') {
		error(unexpectedSymbol, QString::number(pos + 1), "=", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool ExpressionsParser::isEmpty(const QString &stream, int &pos) const
{
	skip(stream, pos);
	return pos == stream.length();
}

void ExpressionsParser::error(const ParseErrorType &type, const QString &pos
		, const QString &expected, const QString &got)
{
	switch (type) {
	case unexpectedEndOfStream:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Unexpected end of stream at %1. Mb you forget \';\'?").arg(pos)
				, mCurrentId);
		break;
	case unexpectedSymbol:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Unexpected symbol at %1 : expected %2, got %3").arg(pos, expected, got)
				, mCurrentId);
		break;
	case typesMismatch:
		mErrorReporter->addWarning(QObject::tr("Types mismatch at %1: %2 = %3. Possible loss of data")
				.arg(pos, expected, got), mCurrentId);
		break;
	case unknownIdentifier:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Unknown identifier at %1 \' %2 \'").arg(pos, got), mCurrentId);
		break;
	case emptyProcess:
		mErrorReporter->addWarning(QObject::tr("Empty process is unnecessary"), mCurrentId);
		break;
	case emptyCondition:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Condition can\'t be empty"), mCurrentId);
		break;
	case usingReservedVariable:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Using reserved variable %1").arg(got), mCurrentId);
		break;
	case noExpression:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("No value of expression"), mCurrentId);
		break;
	case incorrectVariableDeclaration:
		mErrorReporter->addWarning(QObject::tr("Incorrect variable declaration: use function block for it")
				, mCurrentId);
		break;
	case unexpectedSymbolAfterTheEndOfExpression:
		mHasParseErrors = true;
		mErrorReporter->addWarning(QObject::tr("Unexpected symbol after the end of expression"), mCurrentId);
		break;
	case unknownElementProperty:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Unknown element property used"), mCurrentId);
		break;
	case unknownElementName:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Unknown element name used"), mCurrentId);
		break;
	case divisionByZero:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Integer division by zero"), mCurrentId);
	}
}

void ExpressionsParser::setErrorReporter(ErrorReporterInterface *errorReporter)
{
	mErrorReporter = errorReporter;
}

void ExpressionsParser::clear()
{
	mHasParseErrors = false;
	mErrorReporter = nullptr;
	qDeleteAll(mVariables);
	mVariables.clear();
	mCurrentId = Id::rootId();
}

bool ExpressionsParser::checkForUsingReservedVariables(const QString &nameOfVariable)
{
	Q_UNUSED(nameOfVariable)
	return false;
}

void ExpressionsParser::checkForVariable(const QString &nameOfVariable, int &index)
{
	Q_UNUSED(nameOfVariable);
	Q_UNUSED(index);
}

bool ExpressionsParser::isFunction(const QString &variable)
{
	return variable == "cos"
			|| variable == "sin"
			|| variable == "ln"
			|| variable == "exp"
			|| variable == "asin"
			|| variable == "acos"
			|| variable == "atan"
			|| variable == "sgn"
			|| variable == "sqrt"
			|| variable == "abs"
			|| variable == "random"
			;
}

Number *ExpressionsParser::applyFunction(const QString &variable, Number *value)
{
	Number *result = nullptr;
	qreal argument = value->value().toDouble();
	if (variable == "cos") {
		result = new Number(cos(argument), Number::doubleType);
	} else if (variable == "sin") {
		result = new Number(sin(argument), Number::doubleType);
	} else if (variable == "ln") {
		result = new Number(log(argument), Number::doubleType);
	} else if (variable == "exp") {
		result = new Number(exp(argument), Number::doubleType);
	} else if (variable == "sgn") {
		result = new Number((argument >= 0 ? 1 : -1), Number::intType);
	} else if (variable == "acos") {
		result = new Number(acos(argument), Number::doubleType);
	} else if (variable == "asin") {
		result = new Number(asin(argument), Number::doubleType);
	} else if (variable == "atan") {
		result = new Number(atan(argument), Number::doubleType);
	} else if (variable == "sqrt") {
		result = new Number(sqrt(argument), Number::doubleType);
	} else if (variable == "abs") {
		result = new Number(fabs(argument), Number::doubleType);
	} else if (variable == "random") {
		result = new Number(static_cast<int>(rand() % static_cast<int>(argument)), Number::intType);
	}

	return result;
}
