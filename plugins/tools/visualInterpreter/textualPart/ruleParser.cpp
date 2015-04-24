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

#include "ruleParser.h"

using namespace qReal;
using namespace utils;

RuleParser::RuleParser(LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, ErrorReporterInterface* errorReporter)
		: ExpressionsParser(errorReporter)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
{
}

void RuleParser::setRuleId(Id const &id)
{
	mRuleId = id;
}

QString RuleParser::parseElemName(QString const &stream, int &pos)
{
	skip(stream, pos);

	QString const name = parseIdentifier(stream, pos);

	if (!isPoint(stream.at(pos).toLatin1())) {
		error(unexpectedSymbol, QString::number(pos), ".", QString(stream.at(pos).toLatin1()));
		return "";
	}

	return name;
}

QString RuleParser::parseElemProperty(QString const &stream, int &pos)
{
	pos++;
	skip(stream, pos);
	return parseIdentifier(stream, pos);
}

void RuleParser::checkForClosingBracketAndColon(QString const &stream, int &pos)
{
	skip(stream, pos);
	if (!checkForClosingBracket(stream, pos)) {
		return;
	}

	pos++;
	skip(stream, pos);
	if (!checkForColon(stream, pos)) {
		return;
	}
	pos++;
}

void RuleParser::parseRunFunction(QString const &stream, int &pos, QHash<Id, Id> const &mMatch)
{
	pos += 4;

	QString const name = parseElemName(stream, pos);
	if (hasErrors()) {
		return;
	}

	QString const prop = parseElemProperty(stream, pos);
	if (hasErrors()) {
		return;
	}

	checkForClosingBracketAndColon(stream, pos);

	Id const elem = elementByName(name, mMatch);
	if (elem == Id::rootId()) {
		error(unknownElementName);
		return;
	}

	QString const process = property(elem, prop).toString();
	int position = 0;
	parseProcess(process, position, elem);
	mCurrentId = mRuleId;
}

// TODO: refactor this!
void RuleParser::parsePropertyChange(QString const &stream, int &pos,
		QHash<Id, Id> const &mMatch)
{
	QString const name = parseElemName(stream, pos);
	if (hasErrors()) {
		return;
	}

	QString const prop = parseElemProperty(stream, pos);
	if (hasErrors()) {
		return;
	}

	pos++;
	skip(stream, pos);

	if (!checkForEqual(stream, pos)) {
		return;
	}

	pos++;
	skip(stream, pos);

	if (stream.mid(pos, 5) == "cond(") {
		pos += 5;
		skip(stream, pos);

		QString const condName = parseElemName(stream, pos);
		if (hasErrors()) {
			return;
		}

		QString const condProperty = parseElemProperty(stream, pos);
		if (hasErrors()) {
			return;
		}

		checkForClosingBracketAndColon(stream, pos);

		Id const condId = elementByName(condName, mMatch);
		if (condId == Id::rootId()) {
			error(unknownElementName);
			return;
		}

		int position = 0;

		bool value = parseCondition(property(condId, condProperty).toString(),
				position, condId);

		Id const id = elementByName(name, mMatch);
		if (id == Id::rootId()) {
			error(unknownElementName);
			return;
		}

		setProperty(id, prop, QString(value ? "true" : "false"));
		mCurrentId = mRuleId;

		return;
	}

	if (stream.at(pos).toLatin1() == '"') {
		pos++;
		QString const value = stream.mid(pos, stream.indexOf("\"", pos) - pos);
		pos += value.length();

		if (stream.at(pos).toLatin1() != '"') {
			error(unexpectedSymbol, QString::number(pos)
					, "\"", QString(stream.at(pos).toLatin1()));
			return;
		}

		Id const id = elementByName(name, mMatch);
		if (id == Id::rootId()) {
			error(unknownElementName);
			return;
		}

		setProperty(id, prop, value);

		pos++;
		skip(stream, pos);

		if (!checkForColon(stream, pos)) {
			return;
		}
		pos++;

		return;
	}

	if (checkForLogicalConst(stream, pos)) {
		int len = 0;
		QString value = "";

		if (stream.mid(pos, 4) == "true") {
			len = 4;
			value = "true";
		}
		if (stream.mid(pos, 5) == "false") {
			len = 5;
			value = "false";
		}

		QChar c = stream.at(pos + len).toLatin1();
		if (!isDigit(c) && !isLetter(c)) {
			pos += len;
			skip(stream, pos);

			if (!checkForColon(stream, pos)) {
				return;
			}
			pos++;

			Id const id = elementByName(name, mMatch);
			if (id == Id::rootId()) {
				error(unknownElementName);
				return;
			}

			setProperty(id, prop, value);
			return;
		}
	}

	if (stream.mid(pos, 4) == "null") {
		pos += 4;
		skip(stream, pos);

		if (!checkForColon(stream, pos)) {
			return;
		}
		pos++;

		Id const elem = elementByName(name, mMatch);
		if (elem == Id::rootId()) {
			error(unknownElementName);
			return;
		}
		setProperty(elem, prop, QVariant(property(elem, prop).type()));
		return;
	}

	if ((stream.indexOf("=", pos) > pos && stream.indexOf("=", pos) < stream.indexOf(";", pos))
		|| (stream.indexOf("<", pos) > pos && stream.indexOf("<", pos) < stream.indexOf(";", pos))
		|| (stream.indexOf(">", pos) > pos && stream.indexOf(">", pos) < stream.indexOf(";", pos)))
	{
		bool result = parseConditionHelper(stream, pos);
		if (hasErrors()) {
			return;
		}

		skip(stream, pos);
		if (!checkForColon(stream, pos)) {
			return;
		}
		pos++;

		Id const id = elementByName(name, mMatch);
		if (id == Id::rootId()) {
			error(unknownElementName);
			return;
		}

		setProperty(id, prop, QString(result ? "true" : "false"));
	} else {
		Number *result = parseExpression(stream, pos);
		if (hasErrors()) {
			return;
		}

		skip(stream, pos);
		if (!checkForColon(stream, pos)) {
			return;
		}
		pos++;

		Id const id = elementByName(name, mMatch);
		if (id == Id::rootId()) {
			error(unknownElementName);
			return;
		}

		setProperty(id, prop, result->toStringList().join(','));
	}

}

bool RuleParser::checkForLogicalConst(QString const &stream, int &pos)
{
	return (stream.mid(pos, 4) == "true" || stream.mid(pos, 5) == "false");
}

void RuleParser::parseRuleCommand(QString const &stream, int &pos, QHash<Id, Id> const &mMatch)
{
	if (stream.mid(pos, 4) == "run(") {
		parseRunFunction(stream, pos, mMatch);
		return;
	}

	if (stream.indexOf(".", pos) > 0 &&
			stream.indexOf(".", pos) < stream.indexOf("=", pos))
	{
		parsePropertyChange(stream, pos, mMatch);
	} else {
		parseCommand(stream, pos);
	}
}

bool RuleParser::parseRule(QString const &stream, QHash<Id, Id> const &mMatch)
{
	mCurrentId = mRuleId;
	int pos = 0;

	parseVarPart(stream, pos);
	if (hasErrors()) {
		return false;
	}

	while (pos < stream.length() && !hasErrors()) {
		parseRuleCommand(stream, pos, mMatch);
		skip(stream, pos);
	}

	return !hasErrors();
}

bool RuleParser::parseApplicationCondition(QString const &stream, QHash<Id, Id> const &mMatch)
{
	QString appCond = stream;
	int pos = appCond.indexOf("cond(");
	while (pos > -1) {
		int posBackup = pos;
		pos = pos + 5;
		QString const name = parseElemName(appCond, pos);
		QString const prop = parseElemProperty(appCond, pos);

		Id const elem = elementByName(name, mMatch);
		QString const cond = property(elem, prop).toString();

		appCond = appCond.mid(0, posBackup) + "(" + cond + appCond.mid(pos);

		pos = appCond.indexOf("cond(");
	}
	pos = 0;
	return parseConditionHelper(appCond, pos);
}

void RuleParser::parseStringCode(QString const &stream)
{
	int pos = 0;

	parseVarPart(stream, pos);

	while (pos < stream.length() && !hasErrors()) {
		parseCommand(stream, pos);
		skip(stream, pos);
	}
}

Id RuleParser::elementByName(QString const &name, QHash<Id, Id> const &mMatch)
{
	foreach (Id const &elem, mMatch.keys()) {
		if (mLogicalModelApi.logicalRepoApi().name(elem) == name) {
			return mMatch.value(elem);
		}
	}

	return Id::rootId();
}

void RuleParser::setProperty(Id const &id, QString const &propertyName, QVariant const &value)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		if (mLogicalModelApi.logicalRepoApi().hasProperty(id, propertyName)) {
			mLogicalModelApi.mutableLogicalRepoApi().setProperty(id, propertyName, value);
		} else {
			error(unknownElementProperty);
		}
	} else {
		Id const logId = mGraphicalModelApi.logicalId(id);
		if (mLogicalModelApi.logicalRepoApi().hasProperty(logId, propertyName)) {
			mLogicalModelApi.mutableLogicalRepoApi().setProperty(logId, propertyName, value);
		} else {
			error(unknownElementProperty);
		}
	}
}

QVariant RuleParser::property(Id const &id, QString const &propertyName)
{
	if (mLogicalModelApi.isLogicalId(id)) {
		if (mLogicalModelApi.logicalRepoApi().hasProperty(id, propertyName)) {
			return mLogicalModelApi.logicalRepoApi().property(id, propertyName);
		} else {
			error(unknownElementProperty);
		}
	} else {
		Id const logId = mGraphicalModelApi.logicalId(id);
		if (mLogicalModelApi.logicalRepoApi().hasProperty(logId, propertyName)) {
			return mLogicalModelApi.logicalRepoApi().property(logId, propertyName);
		} else {
			error(unknownElementProperty);
		}
	}

	return QVariant();
}

// TODO: refactor this!
void RuleParser::parseVarPart(QString const &stream, int &pos)
{
	skip(stream, pos);
	if (stream.mid(pos, 4).compare("var ") == 0) {
		pos += 4;
		skip(stream, pos);
		if (!isEndOfStream(stream, pos) &&
				stream.mid(pos, 4).compare("int ") != 0 &&
				stream.mid(pos, 7).compare("double ") != 0)
		{
			error(unexpectedSymbol, QString::number(pos + 1),
					tr("int\' or \'double"), stream.at(pos));
			return;
		}

		while (pos < stream.length() &&
				(stream.mid(pos, 4).compare("int ") == 0 ||
				stream.mid(pos, 7).compare("double ") == 0) )
		{
			Number::Type curType;
			if (stream.mid(pos, 4).compare("int ") == 0) {
				curType = Number::intType;
				pos += 4;
			} else {
				curType = Number::doubleType;
				pos += 7;
			}
			skip(stream, pos);
			while (pos < stream.length() && stream.at(pos).toLatin1() != ';') {
				skip(stream, pos);
				QString const variable = parseIdentifier(stream, pos);
				if (hasErrors()) {
					return;
				}
				skip(stream, pos);

				if (isEndOfStream(stream, pos)) {
					return;
				}
				switch (stream.at(pos).toLatin1()) {
				case '=':
					{
						pos++;
						skip(stream, pos);
						Number *temp = parseExpression(stream, pos);
						temp->setType(curType);
						mVariables[variable] = temp;
						break;
					}
				case ',':
					{
						pos++;
						mVariables[variable] = new Number();
						skip(stream, pos);
						if (pos == stream.length()) {
							error(unexpectedEndOfStream, QString::number(pos + 1));
							return;
						}
						if (stream.at(pos).toLatin1() == ';') {
							error(unexpectedSymbol, QString::number(pos + 1),
									tr("\'letter"),
									QString(stream.at(pos).toLatin1())
							);
							return;
						}
						break;
					}
				default:
					{
						if (!checkForColon(stream, pos)) {
							return;
						}

						mVariables[variable] = new Number();
						break;
					}
				}
				skip(stream, pos);
			}

			if (hasErrors()) {
				return;
			}
			pos++;
			skip(stream, pos);
		}
	}
}

void RuleParser::checkForVariable(QString const &nameOfVariable, int &index)
{
	if (!mVariables.contains(nameOfVariable)) {
		error(unknownIdentifier, QString::number(index + 1), "", nameOfVariable);
	}
}
