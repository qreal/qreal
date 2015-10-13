/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
		unexpectedEndOfStream
		, unexpectedSymbol
		, typesMismatch
		, unknownIdentifier
		, emptyProcess
		, emptyCondition
		, usingReservedVariable
		, noExpression
		, incorrectVariableDeclaration
		, unexpectedSymbolAfterTheEndOfExpression
		, unknownElementProperty
		, unknownElementName
		, divisionByZero
	};

protected:
	virtual bool isLetter(const QChar &c) const;

	bool isDigit(const QChar &c) const;
	bool isSign(const QChar &c) const;
	bool isExp(const QChar &c) const;
	bool isPoint(const QChar &c) const;
	bool isRoundBracket(const QChar &c) const;
	bool isDisjunction(const QChar &c) const;
	bool isConjunction(const QChar &c) const;
	bool isComparison(const QChar &c) const;
	bool isArithmeticalMinusOrPlus(const QChar &c) const;
	bool isMultiplicationOrDivision(const QChar &c) const;
	bool isDelimiter(const QChar &c) const;
	bool isAssignment(const QChar &c) const;

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

	void error(const ParseErrorType &type, const QString &pos = "", const QString &expected = ""
			, const QString &got = "");

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
