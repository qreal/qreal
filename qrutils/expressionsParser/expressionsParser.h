#pragma once

#include <QtCore/QMap>

#include "number.h"
#include "../../qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "../utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT ExpressionsParser
{

public:
	ExpressionsParser(qReal::ErrorReporterInterface *errorReporter);

	Number parseExpression(QString const &stream, int &pos);
	void parseProcess(QString const &stream, int& pos, qReal::Id const &curId);
	bool parseConditionHelper(QString const &stream, int &pos);
	bool parseCondition(QString const &stream, int& pos, qReal::Id const &curId);
	qReal::ErrorReporterInterface& getErrors();
	bool hasErrors();
	void setErrorReporter(qReal::ErrorReporterInterface *errorReporter);
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
		unexpectedSymbolAfterTheEndOfExpression,
		unknownElementProperty,
		unknownElementName
	};

protected:
	virtual bool isLetter(QChar const &c) const;

	bool isDigit(QChar const &c) const;
	bool isSign(QChar const &c) const;
	bool isExp(QChar const &c) const;
	bool isPoint(QChar const &c) const;
	bool isRoundBracket(QChar const &c) const;
	bool isDisjunction(QChar const &c) const;
	bool isConjunction(QChar const &c) const;
	bool isComparison(QChar const &c) const;
	bool isArithmeticalMinusOrPlus(QChar const &c) const;
	bool isMultiplicationOrDivision(QChar const &c) const;
	bool isDelimiter(QChar const &c) const;
	bool isAssignment(QChar const &c) const;

	bool isHtmlBrTag(QString const &stream, int &pos) const;

	QString parseIdentifier(QString const &stream, int &pos);
	Number parseNumber(QString const &stream, int &pos);
	void skip(QString const &stream, int &pos) const;

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
	bool isEmpty(QString const &stream, int &pos) const;
	bool checkForEqual(QString const &stream, int pos);

	virtual bool checkForUsingReservedVariables(QString const &nameOfVariable);
	virtual void checkForVariable(QString const &nameOfVariable, int &index);

	bool isFunction(QString const &variable);
	Number applyFunction(QString const &variable, Number value);

	QMap<QString, Number> mVariables;
	bool mHasParseErrors;
	qReal::ErrorReporterInterface *mErrorReporter;
	qReal::Id mCurrentId;

};
}
