#include "blockParser.h"

#include <QMessageBox>

using namespace qReal;

blockParser::blockParser() {
	hasWrongParsedBracket = false;
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

number blockParser::parseNumber(QString stream, int* pos) {
	int beginPos = *pos;
	bool isDouble = false;
	if (*pos <= stream.length() && isSign(stream.at(*pos))) {
		(*pos)++;
	}
	while (*pos <= stream.length() && isDigit(stream.at(*pos))) {
		(*pos)++;
	}
	if (*pos <= stream.length() && isPoint(stream.at(*pos))) {
		isDouble = true;
		(*pos)++;
		while (*pos <= stream.length() && isDigit(stream.at(*pos))) {
			(*pos)++;
		}
	}
	if (*pos <= stream.length() && isExp(stream.at(*pos))) {
		isDouble = true;
		(*pos)++;
		if (*pos <= stream.length() && isSign(stream.at(*pos))) {
			(*pos)++;
		}
		while (*pos <= stream.length() && isDigit(stream.at(*pos))) {
			(*pos)++;
		}
	}
	if (isDouble) {
		return *(new number(stream.mid(beginPos, *pos - beginPos).toDouble(), number::Double));
	} else {
		return *(new number(stream.mid(beginPos, *pos - beginPos).toInt(), number::Int));
	}
}

QString blockParser::parseIdentifier(QString stream, int* pos) {
	int beginPos = *pos;
	if (*pos <= stream.length() && isLetter(stream.at(*pos))) {
		(*pos)++;
		while (*pos <= stream.length() && (isDigit(stream.at(*pos)) || isLetter(stream.at(*pos)))) {
			(*pos)++;
		}
		return stream.mid(beginPos, *pos - beginPos);
	}
	return "";
}

void blockParser::skip(QString stream, int* pos) {
	while (*pos <= stream.length() && 
		(isUseless(stream.at(*pos)) || stream.at(*pos).toAscii() == '<'))
	{
		if (isHtmlBpTag(stream, pos)) {
			(*pos)+=4;
			return;
		}
		(*pos)++;
	}
}

bool blockParser::isHtmlBpTag(QString stream, int *pos) {
	return stream.at(*pos).toAscii() == '<' &&
		stream.at(*pos + 1).toAscii() == 'b' &&
		stream.at(*pos + 2).toAscii() == 'p' &&
		stream.at(*pos + 3).toAscii() == '>';
}

number blockParser::parseTerm(QString stream, int* pos) {
	number res;
	skip(stream, pos);
	switch (stream.at(*pos).toAscii()) {
		case '+':
			(*pos)++;
			skip(stream, pos);
			res = parseTerm(stream, pos);
			break;
		case '-':
			(*pos)++;
			skip(stream, pos);
			res = - (parseTerm(stream, pos));
			break;
		case '(':
			(*pos)++;
			skip(stream, pos);
			res = parseExpression(stream, pos);
			skip(stream, pos);
			(*pos)++;
			break;
		default:
			if (isDigit(stream.at(*pos))) {
				res = parseNumber(stream, pos);
			} else if (isLetter(stream.at(*pos))) {
				res = mVariables[parseIdentifier(stream, pos)];
			}
			break;
	}
	skip(stream, pos);
	return res;
}

number blockParser::parseMult(QString stream, int* pos) {
	number res = parseTerm(stream, pos);
	while (*pos <= stream.length() && isArOpHP(stream.at(*pos))) {
		(*pos)++;
		switch (stream.at(*pos - 1).toAscii()) {
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

number blockParser::parseExpression(QString stream, int* pos) {
	number res = parseMult(stream, pos);
	while (*pos <= stream.length() && isArOpLP(stream.at(*pos))) {
		(*pos)++;
		switch (stream.at(*pos - 1).toAscii()) {
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

void blockParser::parseVarPart(QString stream, int* pos) {
	skip(stream, pos);
	if (stream.mid(*pos,4).compare("var ") == 0) {
		(*pos)+=4;
		skip(stream, pos);
		while (*pos <= stream.length() && 
			(stream.mid(*pos,4).compare("int ") == 0 || stream.mid(*pos,7).compare("double ") == 0))
		{
			if (stream.mid(*pos,4).compare("int ") == 0) {
				(*pos)+=4;
			} else {
				(*pos)+=7;
			}
			skip(stream, pos);
			while (*pos <= stream.length() && stream.at(*pos).toAscii() != ';') {
				skip(stream, pos);
				QString variable = parseIdentifier(stream, pos);
				skip(stream, pos);
				number n;
				switch (stream.at(*pos).toAscii()) {
					case '=':
						(*pos)++;
						skip(stream, pos);
						n = parseExpression(stream, pos);
						if (stream.mid(*pos,4).compare("int ") == 0) {
							n.setProperty("Type", number::Int);
						} else {
							n.setProperty("Type", number::Double);
						}
						mVariables[variable] = n;
						break;
					case ',':
						(*pos)++;
						mVariables[variable] = n;
						break;
				}
				skip(stream, pos);
			}
			(*pos)++;
			skip(stream, pos);
		}
	}
}

void blockParser::parseCommand(QString stream, int* pos) {
	skip(stream, pos);
	QString variable = parseIdentifier(stream, pos);
	skip(stream, pos);
	if (isAssignment(stream.at(*pos))) {
		(*pos)++;
		number n = parseExpression(stream, pos);
		mVariables[variable] = n;
	}
	if (stream.at(*pos).toAscii() == ';') {
		(*pos)++;
	}
}

void blockParser::parseProcess(QString stream, int* pos) {
	parseVarPart(stream, pos);
	while (*pos < stream.length()) {
		parseCommand(stream, pos);
	}
}

bool blockParser::parseSingleComprasion(QString stream, int *pos) {
	number left = parseExpression(stream, pos);
	number right;
	switch (stream.at(*pos).toAscii()) {
		case '=':
			if (stream.at(*pos + 1).toAscii() == '=') {
				(*pos)+=2;
				right = parseExpression(stream, pos);
				return left == right;
			}
			break;
		case '!':
			if (stream.at(*pos + 1).toAscii() == '=') {
				(*pos)+=2;
				right = parseExpression(stream, pos);
				return left != right;
			}
			break;
		case '<':
			if (stream.at(*pos + 1).toAscii() == '=') {
				(*pos)+=2;
				right = parseExpression(stream, pos);
				return left <= right;
			} else {
				(*pos)++;
				right = parseExpression(stream, pos);
				return left < right;
			}
			break;
		case '>':
			if (stream.at(*pos + 1).toAscii() == '=') {
				(*pos)+=2;
				right = parseExpression(stream, pos);
				return left >= right;
			} else {
				(*pos)++;
				right = parseExpression(stream, pos);
				return left > right;
			}
			break;
	}
	hasWrongParsedBracket = true;
	return false;
}

bool blockParser::parseDisjunction(QString stream, int* pos) {
	bool res = false;
	skip(stream, pos);
	int backupPos = *pos;
	switch (stream.at(*pos).toAscii()) {
		case '(':
			(*pos)++;
			skip(stream, pos);
			res = parseCondition(stream, pos);
			skip(stream, pos);
			(*pos)++;
			if (hasWrongParsedBracket && 
				(stream.indexOf('(', backupPos + 1) == -1 ||
				stream.indexOf(')',backupPos) < stream.indexOf('(', backupPos + 1)))
			{
				*pos = backupPos;
				res = parseSingleComprasion(stream, pos);
			}
			break;
		default:
			if (isDigit(stream.at(*pos)) || isLetter(stream.at(*pos))) {
				res = parseSingleComprasion(stream, pos);
			}
			break;
	}
	skip(stream, pos);
	return res;
}

bool blockParser::parseConjunction(QString stream, int* pos) {
	bool res = parseDisjunction(stream, pos);
	while (*pos <= stream.length() && isLogCon(stream.at(*pos))) {
		if (isLogCon(stream.at(*pos + 1))) {
			(*pos)+=2;
			res = res && parseDisjunction(stream, pos);
		}
	}
	return res;
}

bool blockParser::parseCondition(QString stream, int* pos) {
	bool res = parseConjunction(stream, pos);
	while (*pos <= stream.length() && isLogDis(stream.at(*pos))) {
		if (isLogDis(stream.at(*pos + 1))) {
			(*pos)+=2;
			res = res || parseConjunction(stream, pos);
		}
	}
	return res;
}
