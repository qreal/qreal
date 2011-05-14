#include "blockParser.h"

#include <QMessageBox>

using namespace qReal;

BlockParser::BlockParser(gui::ErrorReporter* errorReporter)
	: mHasParseErrors(false), mErrorReporter(errorReporter), mCurrentId (Id::rootId())
{
}

BlockParser::~BlockParser()
{
	delete &mVariables;
}

QMap<QString, Number>* BlockParser::getVariables()
{
	return &mVariables;
}

bool BlockParser::isDigit(QChar c)
{
	char symbol = c.toAscii();
	return '0' <= symbol && symbol <= '9';
}

bool BlockParser::isSign(QChar c)
{
	char symbol = c.toAscii();
	return symbol == '-' || symbol == '+';
}

bool BlockParser::isLetter(QChar c)
{
	char symbol = c.toAscii();
	return ('A'<= symbol && symbol <= 'Z') || ('a'<= symbol && symbol <='z');
}

bool BlockParser::isExp(QChar c)
{
	char symbol = c.toAscii();
	return symbol == 'e' || symbol == 'E';
}

bool BlockParser::isPoint(QChar c)
{
	return c.toAscii() == '.';
}

bool BlockParser::isRoundBracket(QChar c)
{
	char symbol = c.toAscii();
	return symbol == '(' || symbol == ')';
}

bool BlockParser::isDisjunction(QChar c)
{
	return c.toAscii() == '|';
}

bool BlockParser::isConjunction(QChar c)
{
	return c.toAscii() == '&';
}

bool BlockParser::isComparison(QChar c)
{
	char symbol = c.toAscii();
	return symbol == '<' || symbol == '>';
}

bool BlockParser::isArithmeticalMinusOrPlus(QChar c)
{
	char symbol = c.toAscii();
	return symbol == '-' || symbol == '+';
}

bool BlockParser::isMultiplicationOrDivision(QChar c)
{
	char symbol = c.toAscii();
	return symbol == '*' || symbol == '/';
}

bool BlockParser::isDelimiter(QChar c)
{
	char symbol = c.toAscii();
	return symbol == '\n' || symbol == '\r' || symbol == ' ' || symbol == '\t';
}

bool BlockParser::isAssignment(QChar c)
{
	return c.toAscii() == '=';
}

Number BlockParser::parseNumber(QString stream, int& pos)
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

QString BlockParser::parseIdentifier(QString stream, int& pos)
{
	int beginPos = pos;
	if (checkForLetter(stream, pos)) {
		pos++;
		while (pos < stream.length() && (isDigit(stream.at(pos)) || isLetter(stream.at(pos)))) {
			pos++;
		}
		return stream.mid(beginPos, pos - beginPos);
	}
	return "";
}

void BlockParser::skip(QString stream, int& pos)
{
	while (pos < stream.length() &&
		(isDelimiter(stream.at(pos)) || stream.at(pos).toAscii() == '<'))
	{
		if (isHtmlBrTag(stream, pos)) {
			pos += 4;
			return;
		}
		if (stream.at(pos).toAscii() == '<'){
			return;
		}
		pos++;
	}
}

bool BlockParser::isHtmlBrTag(QString stream, int& pos)
{
	if (pos + 3 < stream.length()) {
		return stream.at(pos).toAscii() == '<' &&
			stream.at(pos + 1).toAscii() == 'b' &&
			stream.at(pos + 2).toAscii() == 'r' &&
			stream.at(pos + 3).toAscii() == '>';
	} else {
		return false;
	}
}

Number BlockParser::parseTerm(QString stream, int& pos)
{
	Number res;
	skip(stream, pos);

	if (hasErrors() || isEndOfStream(stream, pos)) {
		return Number();
	}

	switch (stream.at(pos).toAscii()) {
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
				if (mVariables.contains(variable)) {
					res = mVariables[variable];
				} else {
					error(unknownIdentifier, QString::number(unknownIdentifierIndex + 1), "", variable);
				}
			} else {
				error(unexpectedSymbol, QString::number(pos+1),
					"digit\'' or \'letter\' or \'bracket\' or \'sign", QString(stream.at(pos)));
			}
			break;
	}
	skip(stream, pos);
	return res;
}

Number BlockParser::parseMult(QString stream, int& pos)
{
	Number res = parseTerm(stream, pos);
	while (pos < stream.length() && isMultiplicationOrDivision(stream.at(pos))) {
		pos++;
		switch (stream.at(pos - 1).toAscii()) {
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

Number BlockParser::parseExpression(QString stream, int& pos)
{
	Number res = parseMult(stream, pos);
	while (pos < stream.length() && isArithmeticalMinusOrPlus(stream.at(pos))) {
		pos++;
		switch (stream.at(pos - 1).toAscii()) {
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

void BlockParser::parseVarPart(QString stream, int& pos)
{
	skip(stream, pos);
	if (stream.mid(pos, 4).compare("var ") == 0) {
		pos += 4;
		skip(stream, pos);
		if (!isEndOfStream(stream, pos) &&
			stream.mid(pos, 4).compare("int ") != 0 && stream.mid(pos, 7).compare("double ") != 0)
		{
			error(unexpectedSymbol, QString::number(pos + 1), "int\' or \'double", stream.at(pos));
			return;
		}

		while (pos < stream.length() &&
			(stream.mid(pos, 4).compare("int ") == 0 || stream.mid(pos, 7).compare("double ") == 0))
		{
			Number::Type curType;
			if (stream.mid(pos, 4).compare("int ") == 0) {
				curType = Number::intType;
				pos += 4;
			} else {
				curType = Number::doubleType;
				pos += 7;
			}
			skip(stream, pos);
			while (pos < stream.length() && stream.at(pos).toAscii() != ';') {
				skip(stream, pos);
				QString variable = parseIdentifier(stream, pos);
				if (hasErrors()) {
					return;
				}
				skip(stream, pos);

				Number n;
				if (isEndOfStream(stream, pos)) {
					return;
				}
				switch (stream.at(pos).toAscii()) {
					case '=':
						pos++;
						skip(stream, pos);
						n = parseExpression(stream, pos);
						n.setProperty("Type", curType);
						mVariables[variable] = n;
						break;
					case ',':
						pos++;
						mVariables[variable] = n;
						skip(stream, pos);
						if (pos == stream.length()) {
							error(unexpectedEndOfStream, QString::number(pos+1));
							return;
						}
						if (stream.at(pos).toAscii() == ';') {
							error(unexpectedSymbol, QString::number(pos+1), "\'letter",
								QString(stream.at(pos).toAscii()));
							return;
						}
						break;
					default:
						if (!checkForColon(stream, pos)) {
							return;
						}
						mVariables[variable] = n;
						break;
				}
				skip(stream, pos);
			}

			if (hasErrors()) {
				return;
			}
			pos++;
			skip(stream, pos);
		}
	}
}

void BlockParser::parseCommand(QString stream, int& pos)
{
	int typesMismatchIndex = pos;
	QString variable = parseIdentifier(stream, pos);
	skip(stream, pos);
	if (hasErrors() || isEndOfStream(stream, pos)) {
		return;
	}
	if (!mVariables.contains(variable)) {
		error(unknownIdentifier, QString::number(typesMismatchIndex + 1), "", variable);
	}

	if (isAssignment(stream.at(pos))) {
		pos++;
		Number n = parseExpression(stream, pos);
		if (!hasErrors()) {
			Number::Type t1 = mVariables[variable].property("Type").toInt() ? Number::intType : Number::doubleType;
			Number::Type t2 = n.property("Type").toInt() ? Number::intType : Number::doubleType;
			if (t1==t2) {
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

void BlockParser::parseProcess(QString stream, int& pos, Id curId)
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

bool BlockParser::parseSingleComprasion(QString stream, int& pos)
{
	Number left = parseExpression(stream, pos);
	Number right;
	if (hasErrors() || isEndOfStream(stream, pos)) {
		return false;
	}

	switch (stream.at(pos).toAscii()) {
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
			if (pos < stream.length() && stream.at(pos).toAscii() == '=') {
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
			if (pos < stream.length() && stream.at(pos).toAscii() == '=') {
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

bool BlockParser::parseDisjunction(QString stream, int& pos)
{
	bool res = false;
	skip(stream, pos);
	int index = stream.indexOf(')', pos);

	switch (stream.at(pos).toAscii()) {
		case '(':
			if ((index < stream.indexOf('<', pos) || stream.indexOf('<', pos) == -1) &&
				(index < stream.indexOf('>', pos) || stream.indexOf('>', pos) == -1) &&
				(index < stream.indexOf('=', pos) || stream.indexOf('=', pos) == -1)
				)
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
					"digit\' or \'letter\' or \'sign", QString(stream.at(pos)));
			}
			break;
	}
	skip(stream, pos);
	return res;
}

bool BlockParser::parseConjunction(QString stream, int& pos)
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

bool BlockParser::parseConditionHelper(QString stream, int& pos)
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

bool BlockParser::parseCondition(QString stream, int& pos, Id curId)
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

gui::ErrorReporter& BlockParser::getErrors()
{
	return *mErrorReporter;
}

bool BlockParser::hasErrors()
{
	return mHasParseErrors;
}

bool BlockParser::isEndOfStream(QString stream, int& pos)
{
	if (pos == stream.length()) {
		error(unexpectedEndOfStream, QString::number(pos + 1));
		return true;
	}
	return false;
}

bool BlockParser::checkForLetter(QString stream, int &pos)
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

bool BlockParser::checkForDigit(QString stream, int &pos)
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

bool BlockParser::checkForOpeningBracket(QString stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != '(') {
		error(unexpectedSymbol, QString::number(pos + 1), "(", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForClosingBracket(QString stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != ')') {
		error(unexpectedSymbol, QString::number(pos + 1), ")", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForColon(QString stream, int &pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != ';') {
		error(unexpectedSymbol, QString::number(pos + 1), ";", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForEqual(QString stream, int pos)
{
	if (isEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != '=') {
		error(unexpectedSymbol, QString::number(pos + 1), "=", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::isEmpty(QString stream, int &pos)
{
	skip(stream, pos);
	return pos == stream.length();
}

void BlockParser::error(ParseErrorType type, QString pos, QString expected, QString got)
{
	switch (type) {
		case unexpectedEndOfStream:
			mHasParseErrors = true;
			mErrorReporter->addCritical(
				"Unexpected end of stream at " + pos + ". Mb you forget \';\'?",
				mCurrentId);
			break;
		case unexpectedSymbol:
			mHasParseErrors = true;
			mErrorReporter->addCritical("Unexpected symbol at " + pos +
				" : expected \'" + expected + "\', got \'" + got + "\'", mCurrentId);
			break;
		case typesMismatch:
			mErrorReporter->addWarning("Types mismatch at " + pos + ": " +
				expected + " = " + got + ". Possible loss of data", mCurrentId);
			break;
		case unknownIdentifier:
			mHasParseErrors = true;
			mErrorReporter->addCritical("Unknown identifier at " + pos + " \'" + got + "\'", mCurrentId);
			break;
		case emptyProcess:
			mErrorReporter->addWarning("Empty process is unnecessary", mCurrentId);
			break;
		case emptyCondition:
			mHasParseErrors = true;
			mErrorReporter->addCritical("Condition can\'t be empty", mCurrentId);
			break;
	}
}

void BlockParser::setErrorReporter(gui::ErrorReporter *errorReporter)
{
	mErrorReporter = errorReporter;
}

void BlockParser::clear()
{
	mHasParseErrors = false;
	mErrorReporter = NULL;
	mVariables.clear();
	mCurrentId = Id::rootId();
}
