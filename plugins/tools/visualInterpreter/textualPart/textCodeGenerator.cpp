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

#include "textCodeGenerator.h"

using namespace qReal;

QString const TextCodeGenerator::delimeter = "_visint_";

TextCodeGenerator::TextCodeGenerator(LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mRule(Id::rootId())
{
}

void TextCodeGenerator::setRule(Id const &rule)
{
	mRule = rule;
	mRuleElements = mGraphicalModelApi.graphicalRepoApi().children(mRule);
}

void TextCodeGenerator::setMatch(QHash<Id, Id> const &match)
{
	mMatch = match;
}

QString TextCodeGenerator::generateScript(bool const isApplicationCondition)
{
	QString code = property(mRule, isApplicationCondition ? "applicationCondition" : "procedure");
	collectPropertiesUsageAndMethodsInvocation(code);

	code = replacePropertiesUsage(code);
	code = substituteElementProperties(replaceMethodsInvocation(code));

	collectPropertiesUsageAndMethodsInvocation(code);

	QString const script = createProperInitAndOutput(replacePropertiesUsage(code), isApplicationCondition);

	mPropertiesUsage.clear();
	mMethodsInvocation.clear();

	return script;
}

bool TextCodeGenerator::hasElementName(QString const &name) const
{
	foreach (Id const &id, mRuleElements) {
		if (property(id, "name") == name) {
			return true;
		}
	}
	return false;
}

Id TextCodeGenerator::idByName(QString const &name) const
{
	foreach (Id const &id, mRuleElements) {
		if (property(id, "name") == name) {
			return id;
		}
	}
	return Id::rootId();
}

bool TextCodeGenerator::hasProperty(Id const &element, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(element)) {
		return mLogicalModelApi.logicalRepoApi().hasProperty(element, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().hasProperty(
				mGraphicalModelApi.logicalId(element), propertyName);
	}
}

QString TextCodeGenerator::property(Id const &element, QString const &propertyName) const
{
	return propertyVariant(element, propertyName).toString();
}

QVariant TextCodeGenerator::propertyVariant(Id const &element, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(element)) {
		return mLogicalModelApi.logicalRepoApi().property(element, propertyName);
	}
	return mLogicalModelApi.logicalRepoApi().property(mGraphicalModelApi.logicalId(element), propertyName);
}

bool TextCodeGenerator::isStringProperty(Id const &element, QString const &propertyName) const
{
	QVariant result = propertyVariant(element, propertyName);

	bool isInt = false;
	result.toInt(&isInt);

	QString const boolResult = result.toString().toLower();
	bool isBool = boolResult == "true" || boolResult == "false";

	return !isInt && !isBool;
}

void TextCodeGenerator::collectPropertiesUsageAndMethodsInvocation(QString const &code)
{
	int pos = 0;
	while (pos < code.length()) {
		int index = code.indexOf(".", pos);
		if (index == -1) {
			break;
		}

		QString const elemName = parseIdentifier(code, index, false);
		if (hasElementName(elemName)) {
			QString const propertyName = parseIdentifier(code, index, true);

			if (index + propertyName.length() + 1 < code.length()
					&& code.at(index + propertyName.length() + 1) == '(')
			{
				if (hasProperty(idByName(elemName), propertyName)) {
					if (!mMethodsInvocation.contains(elemName)) {
						mMethodsInvocation.insert(elemName, new QSet<QString>());
					}
					mMethodsInvocation.value(elemName)->insert(propertyName);
				}
				pos = index + 1;
				continue;
			}

			if (hasProperty(idByName(elemName), propertyName)) {
				if (!mPropertiesUsage.contains(elemName)) {
					mPropertiesUsage.insert(elemName, new QSet<QString>());
				}
				mPropertiesUsage.value(elemName)->insert(propertyName);
			}
		}
		pos = index + 1;
	}
}

QString TextCodeGenerator::replacePropertiesUsage(QString const &code) const
{
	QString res = code;
	foreach (QString const &elemName, mPropertiesUsage.keys()) {
		foreach (QString const &propertyName, *mPropertiesUsage.value(elemName)) {
			res = res.replace(elemName + "." + propertyName, elemName + delimeter + propertyName);
		}
	}
	return res;
}

QString TextCodeGenerator::replaceMethodsInvocation(QString const &code) const
{
	QString result = code;
	QString funcDefs = "";
	foreach (QString const &elemName, mMethodsInvocation.keys()) {
		foreach (QString const &propertyName, *mMethodsInvocation.value(elemName)) {
			result.replace(elemName + "." + propertyName + "()", elemName + delimeter + propertyName + "()");
			funcDefs += createBehaviourFunction(elemName, propertyName);
		}
	}

	return funcDefs + result;
}

QString TextCodeGenerator::substituteElementProperties(QString const &code) const
{
	QString result = code;

	int pos = result.indexOf("@");
	while (pos != -1) {
		QString const elemName = parseIdentifier(result, pos, false);
		if (hasElementName(elemName)) {
			QString const propertyName = parseIdentifier(result, pos, true);
			if (hasProperty(idByName(elemName), propertyName)) {
				result.replace(elemName + "@" + propertyName, property(mMatch.value(idByName(elemName)), propertyName));
			}
		}
		pos = result.indexOf("@", pos + 1);
	}

	return result;
}

QString TextCodeGenerator::parseIdentifier(QString const &stream, int pos, bool leftToRight) const
{
	int curPos = leftToRight ? pos + 1 : pos - 1;

	while (curPos >= 0 && curPos < stream.length() && isCorrectIdentifierSymbol(stream.at(curPos))) {
		curPos = leftToRight ? curPos + 1 : curPos - 1;
	}

	return leftToRight ? stream.mid(pos + 1, curPos - pos - 1) : stream.mid(curPos + 1, pos - curPos - 1);
}

bool TextCodeGenerator::isCorrectIdentifierSymbol(QChar const c) const
{
	return ('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a'<= c && c <= 'z') || c == '_';
}

QString TextCodeGenerator::escape(QString const &string) const
{
	QString result = string;

	QList<QString> escapedSymbols;
	escapedSymbols << "\\" << "'" << "\"" ;
	foreach (QString e, escapedSymbols) {
		result.replace(e, "\\" + e);
	}

	result.replace("\n", "\\n");
	result.replace("\t", "\\t");
	result.replace("\r", "\\r");

	return result;
}
