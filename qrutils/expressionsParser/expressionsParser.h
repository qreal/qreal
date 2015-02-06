#pragma once

#include <QtCore/QMap>

#include "number.h"
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include "qrutils/utilsDeclSpec.h"

namespace utils {

class QRUTILS_EXPORT ExpressionsParser
{

public:
	explicit ExpressionsParser(qReal::ErrorReporterInterface *errorReporter);
	virtual ~ExpressionsParser();

	Number *parseExpression(const QString &stream, int &pos);
	void parseProcess(const QString &stream, int& pos, const qReal::Id &curId);
	bool parseConditionHelper(const QString &stream, int &pos);
	bool parseCondition(const QString &stream, int& pos, const qReal::Id &curId);
	qReal::ErrorReporterInterface& getErrors();
	bool hasErrors() const;
	void setErrorReporter(qReal::ErrorReporterInterface *errorReporter);
	void clear();

	QMap<QString, Number *> const &variables() const;
	QMap<QString, Number *> &mutableVariables();

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
		unknownElementName,
		divisionByZero
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

	bool isHtmlBrTag(const QString &stream, int &pos) const;

	QString parseIdentifier(const QString &stream, int &pos);
	Number *parseNumber(const QString &stream, int &pos);
	void skip(const QString &stream, int &pos) const;

	Number *parseTerm(const QString &stream, int &pos);
	Number *parseMult(const QString &stream, int&pos);

	virtual void parseVarPart(const QString &stream, int &pos);
	void parseCommand(const QString &stream, int &pos);

	bool parseSingleComprasion(const QString &stream, int &pos);
	bool parseConjunction(const QString &stream, int &pos);
	bool parseDisjunction(const QString &stream, int &pos);

	void error(ParseErrorType const &type, const QString &pos = "", const QString &expected = "", const QString &got = "");
	bool isEndOfStream(const QString &stream, int &pos);
	bool checkForLetter(const QString &stream, int &pos);
	bool checkForDigit(const QString &stream, int &pos);
	bool checkForOpeningBracket(const QString &stream, int &pos);
	bool checkForClosingBracket(const QString &stream, int &pos);
	bool checkForColon(const QString &stream, int &pos);
	bool isEmpty(const QString &stream, int &pos) const;
	bool checkForEqual(const QString &stream, int pos);

	virtual bool checkForUsingReservedVariables(const QString &nameOfVariable);
	virtual void checkForVariable(const QString &nameOfVariable, int &index);

	bool isFunction(const QString &variable);
	Number *applyFunction(const QString &variable, Number *value);

	QMap<QString, Number *> mVariables;  // Takes ownership
	bool mHasParseErrors;
	qReal::ErrorReporterInterface *mErrorReporter;  // Does not take ownership
	qReal::Id mCurrentId;

};
}
