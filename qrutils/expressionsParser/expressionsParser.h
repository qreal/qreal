#pragma once

#include <QtCore/QMap>

#include "number.h"
#include "../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../utilsDeclSpec.h"

namespace qReal {

class QRUTILS_EXPORT ExpressionsParser
{

public:
	ExpressionsParser(ErrorReporterInterface *errorReporter);

	Number parseExpression(QString const &stream, int &pos);
	void parseProcess(QString const &stream, int& pos, Id const &curId);
	bool parseConditionHelper(QString const &stream, int &pos);
	bool parseCondition(QString const &stream, int& pos, Id const &curId);
	ErrorReporterInterface& getErrors();
	bool hasErrors();
	void setErrorReporter(ErrorReporterInterface *errorReporter);
	void clear();

	QMap<QString, Number>* getVariables();
	QMap<QString, QString>* getVariablesForWatch() const;

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
	ErrorReporterInterface *mErrorReporter;
	Id mCurrentId;

};
}
