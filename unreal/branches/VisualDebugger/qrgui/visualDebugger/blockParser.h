#pragma once

#include <QMap>

#include "number.h"
#include "../mainwindow/errorReporter.h"
#include "propertyeditorproxymodel.h"

namespace qReal {
	class blockParser
	{
		
	public:
		blockParser(gui::ErrorReporter* errorReporter);
		~blockParser();
		number parseExpression(QString stream, int& pos);
		void parseProcess(QString stream, int& pos, Id curId);
		bool parseConditionPrivate(QString stream, int& pos);
		bool parseCondition(QString stream, int& pos, Id curId);
		gui::ErrorReporter& getErrors();
		bool hasErrors();
		void setErrorReporter(gui::ErrorReporter* errorReporter);
		void clear();
		
		QMap<QString, number>* getVariables(); //only for test using
		
	private:
		enum ParseErrorType {
			unexpectedEndOfStream,
			unexpectedSymbol,
			typesMismatch,
			unknownIdentifier,
			emptyProcess, 
			emptyCondition
		};

	private:
		QMap<QString, number> mVariables;
		bool hasParseErrors;
		gui::ErrorReporter* mErrorReporter;
		Id mCurrentId;
		
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
		
		bool isHtmlBrTag(QString stream, int& pos);
		
		QString parseIdentifier(QString stream, int& pos);
		number parseNumber(QString stream, int& pos);
		void skip(QString stream, int& pos);
		
		number parseTerm(QString stream, int& pos);
		number parseMult(QString stream, int& pos);
		
		void parseVarPart(QString stream, int& pos);
		void parseCommand(QString stream, int& pos);
		
		bool parseSingleComprasion(QString stream, int& pos);
		bool parseConjunction(QString stream, int& pos);
		bool parseDisjunction(QString stream, int& pos);
		
		void error(ParseErrorType type, QString pos = "", QString expected = "", QString got = "");
		bool checkForEndOfStream(QString stream, int& pos);
		bool checkForLetter(QString stream, int& pos);
		bool checkForDigit(QString stream, int& pos);
		bool checkForBeginBracket(QString stream, int& pos);
		bool checkForEndBracket(QString stream, int& pos);
		bool checkForColon(QString stream, int& pos);
		bool checkForEmptiness(QString stream, int& pos);
		bool checkForEqual(QString stream, int pos);
	};
}
