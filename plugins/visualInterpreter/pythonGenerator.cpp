#include "pythonGenerator.h"
#include "../../qrutils/outFile.h"

using namespace qReal;

QString const PythonGenerator::delimeter = "_visint_";

PythonGenerator::PythonGenerator(LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface
		, QString const &reactionScriptPath
		, QString const &applicationConditionScriptPath)
		: mInterpretersInterface(interpretersInterface)
		, mLogicalModelApi(logicalModelApi)
		, mGraphicalModelApi(graphicalModelApi)
		, mRule(Id::rootId())
		, mReactionScriptPath(reactionScriptPath)
		, mApplicationConditionScriptPath(applicationConditionScriptPath)
{
}

void PythonGenerator::setRule(Id const &rule)
{
	mRule = rule;
	mRuleElements = mGraphicalModelApi.graphicalRepoApi().children(mRule);
}

void PythonGenerator::setMatch(QHash<Id, Id> const &match)
{
	mMatch = match;
}

void PythonGenerator::setReactionScriptPath(QString const &path)
{
	mReactionScriptPath = path;
}

void PythonGenerator::setApplicationConditionScriptPath(QString const &path)
{
	mApplicationConditionScriptPath = path;
}

void PythonGenerator::generateScript(bool const isApplicationCondition)
{
	QString code = property(mRule, isApplicationCondition ? "applicationCondition" : "procedure");
	collectPropertiesUsageAndMethodsInvocation(code);

	code = replacePropertiesUsage(code);
	code = substituteElementProperties(replaceMethodsInvocation(code));

	collectPropertiesUsageAndMethodsInvocation(code);

	QString const script = "#!/usr/bin/python\n# -*- coding: utf-8 -*-\n"
			+ createProperInitAndOutput(replacePropertiesUsage(code), isApplicationCondition);
	createScriptFile(script, isApplicationCondition);

	mPropertiesUsage.clear();
	mMethodsInvocation.clear();
}

void PythonGenerator::createScriptFile(QString const &script, bool const isApplicationCondition) const
{
	utils::OutFile out(isApplicationCondition ? mApplicationConditionScriptPath : mReactionScriptPath);
	out() << script;
	out().flush();
}

bool PythonGenerator::hasElementName(QString const &name) const
{
	foreach (Id const &id, mRuleElements) {
		if (property(id, "name") == name) {
			return true;
		}
	}
	return false;
}

Id PythonGenerator::idByName(QString const &name) const
{
	foreach (Id const &id, mRuleElements) {
		if (property(id, "name") == name) {
			return id;
		}
	}
	return Id::rootId();
}

bool PythonGenerator::hasProperty(Id const &element, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(element)) {
		return mLogicalModelApi.logicalRepoApi().hasProperty(element, propertyName);
	} else {
		return mLogicalModelApi.logicalRepoApi().hasProperty(
				mGraphicalModelApi.logicalId(element), propertyName);
	}
}

QString PythonGenerator::property(Id const &element, QString const &propertyName) const
{
	QVariant result = propertyVariant(element, propertyName);

	QString res = result.toString();
	if (res == "true") {
		res = "True";
	}
	if (res == "false") {
		res = "False";
	}

	return res;
}

QVariant PythonGenerator::propertyVariant(Id const &element, QString const &propertyName) const
{
	if (mLogicalModelApi.isLogicalId(element)) {
		return mLogicalModelApi.logicalRepoApi().property(element, propertyName);
	}
	return mLogicalModelApi.logicalRepoApi().property(mGraphicalModelApi.logicalId(element), propertyName);
}

bool PythonGenerator::isStringProperty(Id const &element, QString const &propertyName) const
{
	QVariant result = propertyVariant(element, propertyName);
	return result.type() == QVariant::String && result.toString() != "false" && result.toString() != "true";
}

void PythonGenerator::collectPropertiesUsageAndMethodsInvocation(QString const &code)
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

QString PythonGenerator::replacePropertiesUsage(QString const &code) const
{
	QString res = code;
	foreach (QString const &elemName, mPropertiesUsage.keys()) {
		foreach (QString const &propertyName, *mPropertiesUsage.value(elemName)) {
			res = res.replace(elemName + "." + propertyName, elemName + delimeter + propertyName);
		}
	}
	return res;
}

QString PythonGenerator::replaceMethodsInvocation(QString const &code) const
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


QString PythonGenerator::substituteElementProperties(QString const &code) const
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

QString PythonGenerator::createProperInitAndOutput(QString const &code, bool const isApplicationCondition) const
{
	QString init = "";
	QString output = "print ''";
	foreach (QString const &elemName, mPropertiesUsage.keys()) {
		foreach (QString const &propertyName, *mPropertiesUsage.value(elemName)) {
			QString const variable = elemName + delimeter + propertyName;
			QString const curPropertyValue = property(mMatch.value(idByName(elemName)), propertyName);

			bool isStringProp = isStringProperty(idByName(elemName), propertyName);
			QString const propertyValue = isStringProp ? "'" + escape(curPropertyValue) + "'" : curPropertyValue;
			QString const representationOfProperty = "'\\'' + str(" + variable + ") + '\\''";

			init += variable + "=" + propertyValue + "; ";
			output += " + '" + variable + "=' + " + representationOfProperty + " + ';'";
		}
	}
	if (!isApplicationCondition) {
		return init + "\n\n" + code + "\n\n" + output;
	} else {
		return init + "\n\nprint " + code;
	}
}

QString PythonGenerator::createBehaviourFunction(QString const &elementName, QString const &propertyName) const
{
	QString result = properElementProperty(elementName, propertyName);
	result = "def " + elementName + delimeter + propertyName + "():\n" + result + "\n\n";

	return result;
}

QString PythonGenerator::properElementProperty(QString const &elementName, QString const &propertyName) const
{
	QString propertyValue = property(mMatch.value(idByName(elementName)), propertyName);

	int pos = propertyValue.indexOf("this.");
	QString globalVariableDef = "  ";
	while (pos != -1) {
		pos += 4;
		QString const propName = parseIdentifier(propertyValue, pos, true);
		if (hasProperty(idByName(elementName), propName)) {
			globalVariableDef += ("global " + elementName + delimeter + propName + ";\n  ");
		}
		pos = propertyValue.indexOf("this.", pos);
	}

	propertyValue.replace("this.", elementName + ".");
	propertyValue.replace("this@", elementName + "@");
	propertyValue.replace("\n", "\n  ");

	return globalVariableDef + propertyValue;
}

QString PythonGenerator::parseIdentifier(QString const &stream, int pos, bool leftToRight) const
{
	int curPos = leftToRight ? pos + 1 : pos - 1;

	while (curPos >= 0 && curPos < stream.length() && isCorrectIdentifierSymbol(stream.at(curPos))) {
		curPos = leftToRight ? curPos + 1 : curPos - 1;
	}

	return leftToRight ? stream.mid(pos + 1, curPos - pos - 1) : stream.mid(curPos + 1, pos - curPos - 1);
}

bool PythonGenerator::isCorrectIdentifierSymbol(QChar const c) const
{
	return ('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a'<= c && c <= 'z') || c == '_';
}

QString PythonGenerator::escape(QString const &string) const
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
