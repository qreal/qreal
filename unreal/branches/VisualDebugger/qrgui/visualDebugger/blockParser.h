#pragma once

#include <QMap>

#include "number.h"

namespace qReal {
	class blockParser
	{
		
	public:
		blockParser();
		~blockParser();
		number parseExpression(QString stream, int* pos);
		void parseProcess(QString stream, int* pos);
		bool parseCondition(QString stream, int* pos);
		
		QMap<QString, number>* getVariables(); //only for test using
		
	private:
		QMap<QString, number> mVariables;
		bool hasWrongParsedBracket;
		
		bool isDigit(QChar c);
		bool isSign(QChar c);
		bool isLetter(QChar c);
		bool isExp(QChar c);
		bool isPoint(QChar c);
		bool isRoundBracket(QChar c);
		bool isLogDis(QChar c);
		bool isLogCon(QChar c);
		bool isLogComp(QChar c);
		bool isArOpLP(QChar c);
		bool isArOpHP(QChar c);
		bool isUnOp(QChar c);
		bool isUseless(QChar c);
		bool isAssignment(QChar c);
		
		bool isHtmlBpTag(QString stream, int* pos);
		
		QString parseIdentifier(QString stream, int* pos);
		number parseNumber(QString stream, int* pos);
		void skip(QString stream, int* pos);
		
		number parseTerm(QString stream, int* pos);
		number parseMult(QString stream, int* pos);
		
		void parseVarPart(QString stream, int* pos);
		void parseCommand(QString stream, int* pos);
		
		bool parseSingleComprasion(QString stream, int* pos);
		bool parseConjunction(QString stream, int* pos);
		bool parseDisjunction(QString stream, int* pos);
	};
}
