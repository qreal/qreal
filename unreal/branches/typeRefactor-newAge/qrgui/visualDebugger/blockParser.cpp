#include "blockParser.h"

#include <QMessageBox>

using namespace qReal;

BlockParser::BlockParser(gui::ErrorReporter* errorReporter) {
	hasParseErrors = false;
	mErrorReporter = errorReporter;
        mCurrentId = ROOT_ID;
}

BlockParser::~BlockParser() {
	delete &mVariables;
}

QMap<QString, Number>* BlockParser::getVariables() {
	return &mVariables;
}

bool BlockParser::isDigit(QChar c) {
	char c1 = c.toAscii();
	return '0'<=c1 && c1<='9';
}

bool BlockParser::isSign(QChar c) {
	char c1 = c.toAscii();
	return c1=='-' || c1=='+';
}

bool BlockParser::isLetter(QChar c) {
	char c1 = c.toAscii();
	return ('A'<=c1 && c1<='Z') || ('a'<=c1 && c1<='z');
}

bool BlockParser::isExp(QChar c) {
	char c1 = c.toAscii();
	return c1=='e' || c1=='E';
}

bool BlockParser::isPoint(QChar c) {
	char c1 = c.toAscii();
	return c1=='.';
}

bool BlockParser::isRoundBracket(QChar c) {
	char c1 = c.toAscii();
	return c1=='(' || c1==')';
}

bool BlockParser::isLogDis(QChar c) {
	char c1 = c.toAscii();
	return c1=='|';
}

bool BlockParser::isLogCon(QChar c) {
	char c1 = c.toAscii();
	return c1=='&';
}

bool BlockParser::isLogComp(QChar c) {
	char c1 = c.toAscii();
	return c1=='<' || c1=='>';
}

bool BlockParser::isArOpLP(QChar c) {
	char c1 = c.toAscii();
	return c1=='-' || c1=='+';
}

bool BlockParser::isArOpHP(QChar c) {
	char c1 = c.toAscii();
	return c1=='*' || c1=='/';
}

bool BlockParser::isUnOp(QChar c) {
	char c1 = c.toAscii();
	return c1=='-' || c1=='+';
}

bool BlockParser::isUseless(QChar c) {
	char c1 = c.toAscii();
	return c1=='\n' || c1=='\r' || c1==' ' || c1=='\t';
}

bool BlockParser::isAssignment(QChar c) {
	char c1 = c.toAscii();
	return c1=='=';
}

Number BlockParser::parseNumber(QString stream, int& pos) {
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

		if (checkForEndOfStream(stream, pos)) {
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

QString BlockParser::parseIdentifier(QString stream, int& pos) {
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

void BlockParser::skip(QString stream, int& pos) {
	while (pos < stream.length() &&
		(isUseless(stream.at(pos)) || stream.at(pos).toAscii() == '<'))
	{
		if (isHtmlBrTag(stream, pos)) {
			pos+=4;
			return;
		}
		if (stream.at(pos).toAscii() == '<'){
			return;
		}
		pos++;
	}
}

bool BlockParser::isHtmlBrTag(QString stream, int& pos) {
	if (pos + 3 < stream.length()) {
		return stream.at(pos).toAscii() == '<' &&
			stream.at(pos + 1).toAscii() == 'b' &&
			stream.at(pos + 2).toAscii() == 'r' &&
			stream.at(pos + 3).toAscii() == '>';
	} else {
		return false;
	}
}

Number BlockParser::parseTerm(QString stream, int& pos) {
	Number res;
	skip(stream, pos);

	if (hasParseErrors || checkForEndOfStream(stream, pos)) {
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
			if (!checkForEndBracket(stream, pos)) {
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

Number BlockParser::parseMult(QString stream, int& pos) {
	Number res = parseTerm(stream, pos);
	while (pos < stream.length() && isArOpHP(stream.at(pos))) {
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

Number BlockParser::parseExpression(QString stream, int& pos) {
	Number res = parseMult(stream, pos);
	while (pos < stream.length() && isArOpLP(stream.at(pos))) {
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

void BlockParser::parseVarPart(QString stream, int& pos) {
	skip(stream, pos);
	if (stream.mid(pos,4).compare("var ") == 0) {
		pos+=4;
		skip(stream, pos);
		if (!checkForEndOfStream(stream, pos) &&
			stream.mid(pos,4).compare("int ") != 0 && stream.mid(pos,7).compare("double ") != 0)
		{
			error(unexpectedSymbol, QString::number(pos+1), "int\' or \'double", stream.at(pos));
			return;
		}

		while (pos < stream.length() &&
			(stream.mid(pos,4).compare("int ") == 0 || stream.mid(pos,7).compare("double ") == 0))
		{
			Number::Type curType;
			if (stream.mid(pos,4).compare("int ") == 0) {
				curType = Number::intType;
				pos+=4;
			} else {
				curType = Number::doubleType;
				pos+=7;
			}
			skip(stream, pos);
			while (pos < stream.length() && stream.at(pos).toAscii() != ';') {
				skip(stream, pos);
				QString variable = parseIdentifier(stream, pos);
				if (hasParseErrors) {
					return;
				}
				skip(stream, pos);

				Number n;
				if (checkForEndOfStream(stream, pos)) {
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

			if (hasParseErrors) {
				return;
			}
			pos++;
			skip(stream, pos);
		}
	}
}

void BlockParser::parseCommand(QString stream, int& pos) {
	int typesMismatchIndex = pos;
	QString variable = parseIdentifier(stream, pos);
	skip(stream, pos);
	if (hasParseErrors || checkForEndOfStream(stream, pos)) {
		return;
	}
	if (!mVariables.contains(variable)) {
		error(unknownIdentifier, QString::number(typesMismatchIndex + 1), "", variable);
	}

	if (isAssignment(stream.at(pos))) {
		pos++;
		Number n = parseExpression(stream, pos);
		if (!hasParseErrors) {
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
	if (!hasParseErrors && checkForColon(stream, pos)) {
		pos++;
	}
}

void BlockParser::parseProcess(QString stream, int& pos, Id curId) {
	mCurrentId = curId;

	if (checkForEmptiness(stream, pos)) {
		error(emptyProcess);
		return;
	}
	parseVarPart(stream, pos);
	if (hasParseErrors) {
		return;
	}
	while (pos < stream.length() && !hasParseErrors) {
		parseCommand(stream, pos);
		skip(stream, pos);
	}
}

bool BlockParser::parseSingleComprasion(QString stream, int& pos) {
	Number left = parseExpression(stream, pos);
	Number right;
	if (hasParseErrors || checkForEndOfStream(stream, pos)) {
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

bool BlockParser::parseDisjunction(QString stream, int& pos) {
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
				res = parseConditionPrivate(stream, pos);
				skip(stream, pos);
				if (!hasParseErrors && checkForEndBracket(stream, pos)) {
					pos++;
				}
			}
			break;
		case '!':
			pos++;
			skip(stream, pos);
			if (hasParseErrors || !checkForBeginBracket(stream, pos)) {
				return res;
			}
			pos++;
			res = !(parseConditionPrivate(stream, pos));

			if (hasParseErrors || !checkForEndBracket(stream, pos)) {
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

bool BlockParser::parseConjunction(QString stream, int& pos) {
	bool res = parseDisjunction(stream, pos);
	while (pos < (stream.length()-1) && isLogCon(stream.at(pos))) {
		pos++;
		if (isLogCon(stream.at(pos))) {
			pos++;
			res = parseDisjunction(stream, pos) && res;
		} else {
			error(unexpectedSymbol, QString::number(pos + 1), "&", QString(stream.at(pos)));
			return res;
		}
	}
	return res;
}

bool BlockParser::parseConditionPrivate(QString stream, int& pos) {
	bool res = parseConjunction(stream, pos);
	while (pos < (stream.length()-1) && isLogDis(stream.at(pos))) {
		pos++;
		if (isLogDis(stream.at(pos))) {
			pos++;
			res = parseConjunction(stream, pos) || res;
		} else {
			error(unexpectedSymbol, QString::number(pos + 1), "|", QString(stream.at(pos)));
			return res;
		}
	}
	return res;
}

bool BlockParser::parseCondition(QString stream, int& pos, Id curId) {
	mCurrentId = curId;
	if (checkForEmptiness(stream, pos)) {
		error(emptyCondition);
		return false;
	}

	bool res = parseConditionPrivate(stream, pos);
	skip(stream, pos);
	if (!hasParseErrors && pos < stream.length()) {
		error(unexpectedSymbol, QString::number(pos), "null string", QString(stream.at(pos)));
	}
	return res;
}

gui::ErrorReporter& BlockParser::getErrors() {
	return *mErrorReporter;
}

bool BlockParser::hasErrors() {
	return hasParseErrors;
}

bool BlockParser::checkForEndOfStream(QString stream, int& pos) {
	if (pos == stream.length()) {
		error(unexpectedEndOfStream, QString::number(pos+1));
		return true;
	}
	return false;
}

bool BlockParser::checkForLetter(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (!isLetter(stream.at(pos))) {
		error(unexpectedSymbol, QString::number(pos+1), "letter", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForDigit(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (!isDigit(stream.at(pos))) {
		error(unexpectedSymbol, QString::number(pos+1), "digit", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForBeginBracket(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != '(') {
		error(unexpectedSymbol, QString::number(pos+1), "(", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForEndBracket(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != ')') {
		error(unexpectedSymbol, QString::number(pos+1), ")", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForColon(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != ';') {
		error(unexpectedSymbol, QString::number(pos+1), ";", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForEqual(QString stream, int pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != '=') {
		error(unexpectedSymbol, QString::number(pos+1), "=", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool BlockParser::checkForEmptiness(QString stream, int &pos) {
	skip(stream, pos);
	return pos == stream.length();
}

void BlockParser::error(ParseErrorType type, QString pos, QString expected, QString got) {
	switch (type) {
		case unexpectedEndOfStream:
			hasParseErrors = true;
			mErrorReporter->addCritical(
				"Unexpected end of stream at " + pos + ". Mb you forget \';\'?",
				mCurrentId);
			break;
		case unexpectedSymbol:
			hasParseErrors = true;
			mErrorReporter->addCritical("Unexpected symbol at " + pos +
				" : expected \'" + expected + "\', got \'" + got + "\'", mCurrentId);
			break;
		case typesMismatch:
			mErrorReporter->addWarning("Types mismatch at " + pos + ": " +
				expected + " = " + got + ". Possible loss of data", mCurrentId);
			break;
		case unknownIdentifier:
			hasParseErrors = true;
			mErrorReporter->addCritical("Unknown identifier at " + pos + " \'" + got + "\'", mCurrentId);
			break;
		case emptyProcess:
			mErrorReporter->addWarning("Empty process is unnecessary", mCurrentId);
			break;
		case emptyCondition:
			hasParseErrors = true;
			mErrorReporter->addCritical("Condition can\'t be empty", mCurrentId);
			break;
	}
}

void BlockParser::setErrorReporter(gui::ErrorReporter *errorReporter) {
	mErrorReporter = errorReporter;
}

void BlockParser::clear() {
	hasParseErrors = false;
	mErrorReporter = NULL;
	mVariables.clear();
        mCurrentId = ROOT_ID;
}
