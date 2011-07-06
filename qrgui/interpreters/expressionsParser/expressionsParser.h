#pragma once

#include <QMap>

#include "../visualDebugger/number.h"
#include "../mainwindow/errorReporter.h"
#include "propertyeditorproxymodel.h"

namespace qReal {
class ExpressionsParser
{

public:
	ExpressionsParser(gui::ErrorReporter* errorReporter);
	~ExpressionsParser();

	Number parseExpression(QString stream, int& pos);
	void parseProcess(QString stream, int& pos, Id curId);
	bool parseConditionHelper(QString stream, int& pos);
	bool parseCondition(QString stream, int& pos, Id curId);
	gui::ErrorReporter& getErrors();
	bool hasErrors();
	void setErrorReporter(gui::ErrorReporter* errorReporter);
	void clear();

	QMap<QString, Number>* getVariables(); //only for test using

protected:
	enum ParseErrorType {
		unexpectedEndOfStream,
		unexpectedSymbol,
		typesMismatch,
		unknownIdentifier,
		emptyProcess,
		emptyCondition,
		usingReservedVariable,
		noExpression
	};

protected:
	virtual bool isLetter(QChar c);

	bool isDigit(QChar c);
	bool isSign(QChar c);
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

	virtual void parseVarPart(QString stream, int& pos);
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

	virtual bool checkForUsingReservedVariables(QString nameOfVariable);
	virtual void checkForVariable(QString nameOfVariable, int& index);

	bool isFunction(QString const &variable);
	Number applyFunction(QString const &variable, Number value);

	QMap<QString, Number> mVariables;
	bool mHasParseErrors;
	gui::ErrorReporter* mErrorReporter;
	Id mCurrentId;

};
}
