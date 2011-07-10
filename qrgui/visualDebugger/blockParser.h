#pragma once

#include <QMap>

#include "number.h"
#include "../mainwindow/errorReporter.h"
#include "propertyEditorProxyModel.h"

namespace qReal {
class BlockParser
{

public:
	explicit BlockParser(gui::ErrorReporter* errorReporter);
	~BlockParser();

	Number parseExpression(QString stream, int& pos);
	void parseProcess(QString stream, int& pos, Id curId);
	bool parseConditionHelper(QString stream, int& pos);
	bool parseCondition(QString stream, int& pos, Id curId);
	gui::ErrorReporter& getErrors();
	bool hasErrors();
	void setErrorReporter(gui::ErrorReporter* errorReporter);
	void clear();

	QMap<QString, Number>* getVariables(); //only for test using

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
	bool isDigit(QChar c);
	bool isSign(QChar c);
	bool isLetter(QChar c);
	bool isExp(QChar c);
	bool isPoint(QChar c);
	bool isRoundBracket(QChar c);
	bool isDisjunction(QChar c);
	bool isConjunction(QChar c);
	bool isComparison(QChar c);
	bool isArithmeticalMinusOrPlus(QChar c);
	bool isMultiplicationOrDivision(QChar c);
	bool isDelimiter(QChar c);
	bool isAssignment(QChar c);

	bool isHtmlBrTag(QString stream, int& pos);

	QString parseIdentifier(QString stream, int& pos);
	Number parseNumber(QString stream, int& pos);
	void skip(QString stream, int& pos);

	Number parseTerm(QString stream, int& pos);
	Number parseMult(QString stream, int& pos);

	void parseVarPart(QString stream, int& pos);
	void parseCommand(QString stream, int& pos);

	bool parseSingleComprasion(QString stream, int& pos);
	bool parseConjunction(QString stream, int& pos);
	bool parseDisjunction(QString stream, int& pos);

	void error(ParseErrorType type, QString pos = "", QString expected = "", QString got = "");
	bool isEndOfStream(QString stream, int& pos);
	bool checkForLetter(QString stream, int& pos);
	bool checkForDigit(QString stream, int& pos);
	bool checkForOpeningBracket(QString stream, int& pos);
	bool checkForClosingBracket(QString stream, int& pos);
	bool checkForColon(QString stream, int& pos);
	bool isEmpty(QString stream, int& pos);
	bool checkForEqual(QString stream, int pos);

	QMap<QString, Number> mVariables;
	bool mHasParseErrors;
	gui::ErrorReporter* mErrorReporter;
	Id mCurrentId;

};
}
