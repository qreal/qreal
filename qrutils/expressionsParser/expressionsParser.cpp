#include "expressionsParser.h"

#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace utils;
using namespace qReal;

ExpressionsParser::ExpressionsParser(ErrorReporterInterface *errorReporter)
	: mHasParseErrors(false), mErrorReporter(errorReporter), mCurrentId (Id::rootId())
{
	srand(time(NULL));
}

QMap<QString, Number>* ExpressionsParser::getVariables()
{
	return &mVariables;
}

QMap<QString, QString>* ExpressionsParser::getVariablesForWatch() const
{
	QMap<QString, QString>* result = new QMap<QString, QString>();
	foreach (QString const &variable, mVariables.keys()) {
		result->insert(variable, mVariables.value(variable).toString());
	}

	return result;
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

Number ExpressionsParser::parseNumber(QString const &stream, int &pos)
{
	int beginPos = pos;
	bool isDouble = false;
	if (pos < stream.length() && isSign(stream.at(pos))) {
		pos++;
	}

	if (!checkForDigit(stream, pos)) {
		return Number();
	}

	while (pos < stream.length() && isDigit(stream.at(pos))) {
		pos++;
	}
	if (pos < stream.length() && isPoint(stream.at(pos))) {
		isDouble = true;
		pos++;

		if (!checkForDigit(stream, pos)) {
			return Number();
		}

		while (pos < stream.length() && isDigit(stream.at(pos))) {
			pos++;
		}
	}
	if (pos < stream.length() && isExp(stream.at(pos))) {
		isDouble = true;
		pos++;

		if (isEndOfStream(stream, pos)) {
			return Number();
		}

		if (pos < stream.length() && isSign(stream.at(pos))) {
			pos++;
		}

		if (!checkForDigit(stream, pos)) {
			return Number();
		}

		while (pos < stream.length() && isDigit(stream.at(pos))) {
			pos++;
		}
	}
	if (isDouble) {
		return Number(stream.mid(beginPos, pos - beginPos).toDouble(), Number::doubleType);
	} else {
		return Number(stream.mid(beginPos, pos - beginPos).toInt(), Number::intType);
	}
}

QString ExpressionsParser::parseIdentifier(QString const &stream, int &pos)
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

void ExpressionsParser::skip(QString const &stream, int &pos) const
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

bool ExpressionsParser::isHtmlBrTag(QString const &stream, int &pos) const
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

Number ExpressionsParser::parseTerm(QString const &stream, int &pos)
{
	Number res;
	skip(stream, pos);

	if (hasErrors() || isEndOfStream(stream, pos)) {
		return Number();
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
		res = - (parseTerm(stream, pos));
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
				Number value;
				if (checkForOpeningBracket(stream, pos)) {
					pos++;
					value = parseExpression(stream, pos);
					if (checkForClosingBracket(stream, pos)) {
						pos++;
						res = applyFunction(variable, value);
					}
				}
			} else if (mVariables.contains(variable)) {
				res = mVariables[variable];
			} else {
				error(unknownIdentifier, QString::number(unknownIdentifierIndex + 1), "", variable);
			}
		} else {
			error(unexpectedSymbol, QString::number(pos+1),
				  "\'digit\' or \'letter\' or \'bracket\' or \'sign\'", QString(stream.at(pos)));
		}
		break;
	}
	skip(stream, pos);
	return res;
}

Number ExpressionsParser::parseMult(QString const &stream, int &pos)
{
	Number res = parseTerm(stream, pos);
	while (pos < stream.length() && isMultiplicationOrDivision(stream.at(pos))) {
		pos++;
		switch (stream.at(pos - 1).toLatin1()) {
		case '*':
			res *= parseTerm(stream, pos);
			break;
		case '/':
			res /= parseTerm(stream, pos);
			break;
		}
	}
	return res;
}

Number ExpressionsParser::parseExpression(QString const &stream, int &pos)
{
	Number res = parseMult(stream, pos);
	while (pos < stream.length() && isArithmeticalMinusOrPlus(stream.at(pos))) {
		pos++;
		switch (stream.at(pos - 1).toLatin1()) {
		case '+':
			res += parseMult(stream, pos);
			break;
		case '-':
			res -= parseMult(stream, pos);
			break;
		}
	}
	return res;
}

void ExpressionsParser::parseVarPart(QString const &stream, int &pos)
{
	Q_UNUSED(stream);
	Q_UNUSED(pos);
}

void ExpressionsParser::parseCommand(QString const &stream, int &pos)
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
		Number n = parseExpression(stream, pos);
		if (!hasErrors()) {
			bool const containsVariable = mVariables.keys().contains(variable);
			Number::Type const t1 = containsVariable
					? (mVariables[variable].property("Type").toInt() ? Number::intType : Number::doubleType)
					: Number::intType;
			Number::Type const t2 = n.property("Type").toInt() ? Number::intType : Number::doubleType;
			if (!containsVariable || t1 == t2) {
				mVariables[variable] = n;
			} else {
				if (t1 == Number::intType) {
					mVariables[variable].setProperty("Number", n.property("Number").toInt());
					error(typesMismatch, QString::number(typesMismatchIndex + 1), "\'int\'", "\'double\'");
				} else {
					mVariables[variable].setProperty("Number", n.property("Number").toDouble());
				}
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

void ExpressionsParser::parseProcess(QString const &stream, int &pos, const Id &curId)
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

bool ExpressionsParser::parseSingleComprasion(QString const &stream, int &pos)
{
	Number left = parseExpression(stream, pos);
	Number right;
	if (hasErrors() || isEndOfStream(stream, pos)) {
		return false;
	}

	switch (stream.at(pos).toLatin1()) {
	case '=':
		pos++;
		if (checkForEqual(stream, pos)) {
			pos++;
			right = parseExpression(stream, pos);
			return left == right;
		} else {
			return false;
		}
		break;
	case '!':
		pos++;
		if (checkForEqual(stream, pos)) {
			pos++;
			right = parseExpression(stream, pos);
			return left != right;
		} else {
			return false;
		}
		break;
	case '<':
		pos++;
		if (pos < stream.length() && stream.at(pos).toLatin1() == '=') {
			pos++;
			right = parseExpression(stream, pos);
			return left <= right;
		} else {
			right = parseExpression(stream, pos);
			return left < right;
		}
		break;
	case '>':
		pos++;
		if (pos < stream.length() && stream.at(pos).toLatin1() == '=') {
			pos++;
			right = parseExpression(stream, pos);
			return left >= right;
		} else {
			right = parseExpression(stream, pos);
			return left > right;
		}
		break;
	}
	error(unexpectedSymbol, QString::number(pos+1), "=\',\'!\',\'>\',\'<",
		  QString(stream.at(pos)));
	return false;
}

bool ExpressionsParser::parseDisjunction(QString const &stream, int &pos)
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
			error(unexpectedSymbol, QString::number(pos+1),
				  "\'digit\' or \'letter\' or \'sign\'", QString(stream.at(pos)));
		}
		break;
	}
	skip(stream, pos);
	return res;
}

bool ExpressionsParser::parseConjunction(QString const &stream, int &pos)
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

bool ExpressionsParser::parseConditionHelper(QString const &stream, int &pos)
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

bool ExpressionsParser::parseCondition(QString const &stream, int &pos, const Id &curId)
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

bool ExpressionsParser::hasErrors()
{
	return mHasParseErrors;
}

bool ExpressionsParser::isEndOfStream(QString const &stream, int &pos)
{
	if (pos == stream.length()) {
		error(unexpectedEndOfStream, QString::number(pos + 1));
		return true;
	}
	return false;
}

bool ExpressionsParser::checkForLetter(QString const &stream, int &pos)
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

bool ExpressionsParser::checkForDigit(QString const &stream, int &pos)
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

bool ExpressionsParser::checkForOpeningBracket(QString const &stream, int &pos)
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

bool ExpressionsParser::checkForClosingBracket(QString const &stream, int &pos)
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

bool ExpressionsParser::checkForColon(QString const &stream, int &pos)
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

bool ExpressionsParser::checkForEqual(QString const &stream, int pos)
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

bool ExpressionsParser::isEmpty(QString const &stream, int &pos) const
{
	skip(stream, pos);
	return pos == stream.length();
}

void ExpressionsParser::error(const ParseErrorType &type, QString const &pos, QString const &expected, QString const &got)
{
	switch (type) {
	case unexpectedEndOfStream:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Unexpected end of stream at %1. Mb you forget \';\'?").arg(pos), mCurrentId);
		break;
	case unexpectedSymbol:
		mHasParseErrors = true;
		mErrorReporter->addCritical(QObject::tr("Unexpected symbol at %1 : expected %2, got %3").arg(pos, expected, got), mCurrentId);
		break;
	case typesMismatch:
		mErrorReporter->addWarning(QObject::tr("Types mismatch at %1: %2 = %3. Possible loss of data").arg(pos, expected, got), mCurrentId);
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
		mErrorReporter->addWarning(QObject::tr("Incorrect variable declaration: use function block for it"), mCurrentId);
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
	}
}

void ExpressionsParser::setErrorReporter(ErrorReporterInterface *errorReporter)
{
	mErrorReporter = errorReporter;
}

void ExpressionsParser::clear()
{
	mHasParseErrors = false;
	mErrorReporter = NULL;
	mVariables.clear();
	mCurrentId = Id::rootId();
}

bool ExpressionsParser::checkForUsingReservedVariables(QString const &nameOfVariable)
{
	Q_UNUSED(nameOfVariable)
	return false;
}

void ExpressionsParser::checkForVariable(QString const &nameOfVariable, int &index)
{
	Q_UNUSED(nameOfVariable);
	Q_UNUSED(index);
}

bool ExpressionsParser::isFunction(QString const &variable)
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
			|| variable == "abs"
			|| variable == "random"
			;
}

Number ExpressionsParser::applyFunction(QString const &variable, Number value)
{
	Number result;
	double argument = value.property("Number").toDouble();
	if (variable == "cos") {
		result = Number(cos(argument), Number::doubleType);
	} else if (variable == "sin") {
		result = Number(sin(argument), Number::doubleType);
	} else if (variable == "ln") {
		result = Number(log(argument), Number::doubleType);
	} else if (variable == "exp") {
		result = Number(exp(argument), Number::doubleType);
	} else if (variable == "sgn") {
		result = Number((argument >= 0 ? 1 : -1), Number::intType);
	} else if (variable == "acos") {
		result = Number(acos(argument), Number::doubleType);
	} else if (variable == "asin") {
		result = Number(asin(argument), Number::doubleType);
	} else if (variable == "atan") {
		result = Number(atan(argument), Number::doubleType);
	} else if (variable == "sqrt") {
		result = Number(sqrt(argument), Number::doubleType);
	} else if (variable == "abs") {
		result = Number(fabs(argument), Number::doubleType);
	} else if (variable == "random") {
		result = Number(static_cast<int>(rand() % static_cast<int>(argument)), Number::intType);
	}
	return result;
}
