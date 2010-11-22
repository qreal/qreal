#include "blockParser.h"

#include <QMessageBox>

using namespace qReal;

blockParser::blockParser(gui::ErrorReporter* errorReporter) {
	hasParseErrors = false;
	mErrorReporter = errorReporter;
	mCurrentId = mCurrentId.getRootId();
}

QMap<QString, number>* blockParser::getVariables() {
	return &mVariables;
}

bool blockParser::isDigit(QChar c) {
	char c1 = c.toAscii();
	if ('0'<=c1 && c1<='9') {
		return true;
	}
	return false;
}

bool blockParser::isSign(QChar c) {
	char c1 = c.toAscii();
	if (c1=='-' || c1=='+') {
		return true;
	}
	return false;
}

bool blockParser::isLetter(QChar c) {
	char c1 = c.toAscii();
	if (('A'<=c1 && c1<='Z') || ('a'<=c1 && c1<='z')) {
		return true;
	}
	return false;
}

bool blockParser::isExp(QChar c) {
	char c1 = c.toAscii();
	if (c1=='e' || c1=='E') {
		return true;
	}
	return false;
}

bool blockParser::isPoint(QChar c) {
	char c1 = c.toAscii();
	if (c1=='.') {
		return true;
	}
	return false;
}

bool blockParser::isRoundBracket(QChar c) {
	char c1 = c.toAscii();
	if (c1=='(' || c1==')') {
		return true;
	}
	return false;
}

bool blockParser::isLogDis(QChar c) {
	char c1 = c.toAscii();
	if (c1=='|') {
		return true;
	}
	return false;
}

bool blockParser::isLogCon(QChar c) {
	char c1 = c.toAscii();
	if (c1=='&') {
		return true;
	}
	return false;
}

bool blockParser::isLogComp(QChar c) {
	char c1 = c.toAscii();
	if (c1=='<' || c1=='>') {
		return true;
	}
	return false;
}

bool blockParser::isArOpLP(QChar c) {
	char c1 = c.toAscii();
	if (c1=='-' || c1=='+') {
		return true;
	}
	return false;
}

bool blockParser::isArOpHP(QChar c) {
	char c1 = c.toAscii();
	if (c1=='*' || c1=='/') {
		return true;
	}
	return false;
}

bool blockParser::isUnOp(QChar c) {
	char c1 = c.toAscii();
	if (c1=='-' || c1=='+') {
		return true;
	}
	return false;
}

bool blockParser::isUseless(QChar c) {
	char c1 = c.toAscii();
	if (c1=='\n' || c1=='\r' || c1==' ' || c1=='\t') {
		return true;
	}
	return false;
}

bool blockParser::isAssignment(QChar c) {
	char c1 = c.toAscii();
	if (c1=='=') {
		return true;
	}
	return false;
}

number blockParser::parseNumber(QString stream, int& pos) {
	int beginPos = pos;
	bool isDouble = false;
	if (pos < stream.length() && isSign(stream.at(pos))) {
		pos++;
	}
	
	if (!checkForDigit(stream, pos)) {
		return number();
	}
	
	while (pos < stream.length() && isDigit(stream.at(pos))) {
		pos++;
	}
	if (pos < stream.length() && isPoint(stream.at(pos))) {
		isDouble = true;
		pos++;
		
		if (!checkForDigit(stream, pos)) {
			return number();
		}
		
		while (pos < stream.length() && isDigit(stream.at(pos))) {
			pos++;
		}
	}
	if (pos < stream.length() && isExp(stream.at(pos))) {
		isDouble = true;
		pos++;
		
		if (checkForEndOfStream(stream, pos)) {
			return number();
		}
		
		if (pos < stream.length() && isSign(stream.at(pos))) {
			pos++;
		}
		
		if (!checkForDigit(stream, pos)) {
			return number();
		}
		
		while (pos < stream.length() && isDigit(stream.at(pos))) {
			pos++;
		}
	}
	if (isDouble) {
		return *(new number(stream.mid(beginPos, pos - beginPos).toDouble(), number::Double));
	} else {
		return *(new number(stream.mid(beginPos, pos - beginPos).toInt(), number::Int));
	}
}

QString blockParser::parseIdentifier(QString stream, int& pos) {
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

void blockParser::skip(QString stream, int& pos) {
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

bool blockParser::isHtmlBrTag(QString stream, int& pos) {
	if (pos + 3 < stream.length()) {
		return stream.at(pos).toAscii() == '<' &&
			stream.at(pos + 1).toAscii() == 'b' &&
			stream.at(pos + 2).toAscii() == 'r' &&
			stream.at(pos + 3).toAscii() == '>';
	} else {
		return false;
	}
}

number blockParser::parseTerm(QString stream, int& pos) {
	number res;
	skip(stream, pos);
	
	if (hasParseErrors || checkForEndOfStream(stream, pos)) {
		return number();
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

number blockParser::parseMult(QString stream, int& pos) {
	number res = parseTerm(stream, pos);
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

number blockParser::parseExpression(QString stream, int& pos) {
	number res = parseMult(stream, pos);
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

void blockParser::parseVarPart(QString stream, int& pos) {
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
			number::Type curType;
			if (stream.mid(pos,4).compare("int ") == 0) {
				curType = number::Int;
				pos+=4;
			} else {
				curType = number::Double;
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
				
				number n;
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

void blockParser::parseCommand(QString stream, int& pos) {
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
		number n = parseExpression(stream, pos);
		if (!hasParseErrors) {
			number::Type t1 = mVariables[variable].property("Type").toInt() ? number::Int : number::Double;
			number::Type t2 = n.property("Type").toInt() ? number::Int : number::Double;
			if (t1==t2) {
				mVariables[variable] = n;
			} else {
				if (t1 == number::Int) {
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

void blockParser::parseProcess(QString stream, int& pos, Id curId) {
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

bool blockParser::parseSingleComprasion(QString stream, int& pos) {
	number left = parseExpression(stream, pos);
	number right;
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

bool blockParser::parseDisjunction(QString stream, int& pos) {
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

bool blockParser::parseConjunction(QString stream, int& pos) {
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

bool blockParser::parseConditionPrivate(QString stream, int& pos) {
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

bool blockParser::parseCondition(QString stream, int& pos, Id curId) {
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

gui::ErrorReporter& blockParser::getErrors() {
	return *mErrorReporter;
}

bool blockParser::hasErrors() {
	return hasParseErrors;
}

bool blockParser::checkForEndOfStream(QString stream, int& pos) {
	if (pos == stream.length()) {
		error(unexpectedEndOfStream, QString::number(pos+1));
		return true;
	}
	return false;
}

bool blockParser::checkForLetter(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (!isLetter(stream.at(pos))) {
		error(unexpectedSymbol, QString::number(pos+1), "letter", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool blockParser::checkForDigit(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (!isDigit(stream.at(pos))) {
		error(unexpectedSymbol, QString::number(pos+1), "digit", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool blockParser::checkForBeginBracket(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != '(') {
		error(unexpectedSymbol, QString::number(pos+1), "(", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool blockParser::checkForEndBracket(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != ')') {
		error(unexpectedSymbol, QString::number(pos+1), ")", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool blockParser::checkForColon(QString stream, int &pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != ';') {
		error(unexpectedSymbol, QString::number(pos+1), ";", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool blockParser::checkForEqual(QString stream, int pos) {
	if (checkForEndOfStream(stream, pos)) {
		return false;
	}
	if (stream.at(pos).toAscii() != '=') {
		error(unexpectedSymbol, QString::number(pos+1), "=", QString(stream.at(pos)));
		return false;
	}
	return true;
}

bool blockParser::checkForEmptiness(QString stream, int &pos) {
	skip(stream, pos);
	return pos == stream.length();
}

void blockParser::error(ParseErrorType type, QString pos, QString expected, QString got) {
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

void blockParser::setErrorReporter(gui::ErrorReporter *errorReporter) {
	mErrorReporter = errorReporter;
}

void blockParser::clear() {
	hasParseErrors = false;
	mErrorReporter = NULL;
	mVariables.clear();
	mCurrentId = mCurrentId.getRootId();
}
