#pragma once

#include <QtCore/QMap>

#include "../visualDebugger/number.h"
#include "../mainwindow/errorReporter.h"
#include "../mainwindow/propertyEditorProxyModel.h" // TODO: ?!

namespace qReal {

class ExpressionsParser
{

public:
	ExpressionsParser(gui::ErrorReporter* errorReporter);

	Number parseExpression(QString const &stream, int &pos);
	void parseProcess(QString const &stream, int& pos, Id const &curId);
	bool parseConditionHelper(QString const &stream, int &pos);
	bool parseCondition(QString const &stream, int& pos, Id const &curId);
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
		noExpression,
		incorrectVariableDeclaration,
		unexpectedSymbolAfterTheEndOfExpression
	};

protected:
	virtual bool isLetter(QChar const &c);

	bool isDigit(QChar const &c);
	bool isSign(QChar const &c);
	bool isExp(QChar const &c);
	bool isPoint(QChar const &c);
	bool isRoundBracket(QChar const &c);
	bool isDisjunction(QChar const &c);
	bool isConjunction(QChar const &c);
	bool isComparison(QChar const &c);
	bool isArithmeticalMinusOrPlus(QChar const &c);
	bool isMultiplicationOrDivision(QChar const &c);
	bool isDelimiter(QChar const &c);
	bool isAssignment(QChar const &c);

	bool isHtmlBrTag(QString const &stream, int &pos);

	QString parseIdentifier(QString const &stream, int &pos);
	Number parseNumber(QString const &stream, int &pos);
	void skip(QString const &stream, int &pos);

	Number parseTerm(QString const &stream, int &pos);
	Number parseMult(QString const &stream, int&pos);

	virtual void parseVarPart(QString const &stream, int &pos);
	void parseCommand(QString const &stream, int &pos);

	bool parseSingleComprasion(QString const &stream, int &pos);
	bool parseConjunction(QString const &stream, int &pos);
	bool parseDisjunction(QString const &stream, int &pos);

	void error(ParseErrorType const &type, QString const &pos = "", QString const &expected = "", QString const &got = "");
	bool isEndOfStream(QString const &stream, int &pos);
	bool checkForLetter(QString const &stream, int &pos);
	bool checkForDigit(QString const &stream, int &pos);
	bool checkForOpeningBracket(QString const &stream, int &pos);
	bool checkForClosingBracket(QString const &stream, int &pos);
	bool checkForColon(QString const &stream, int &pos);
	bool isEmpty(QString const &stream, int &pos);
	bool checkForEqual(QString const &stream, int pos);

	virtual bool checkForUsingReservedVariables(QString const &nameOfVariable);
	virtual void checkForVariable(QString const &nameOfVariable, int &index);

	bool isFunction(QString const &variable);
	Number applyFunction(QString const &variable, Number value);

	QMap<QString, Number> mVariables;
	bool mHasParseErrors;
	gui::ErrorReporter* mErrorReporter;
	Id mCurrentId;

};
}
