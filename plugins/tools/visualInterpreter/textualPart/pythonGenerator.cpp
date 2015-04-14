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

#include "pythonGenerator.h"
#include <qrutils/outFile.h>

using namespace qReal;

PythonGenerator::PythonGenerator(LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface)
		: TextCodeGenerator(logicalModelApi, graphicalModelApi, interpretersInterface)
{
}

QString PythonGenerator::generateScript(bool const isApplicationCondition)
{
	QString const script = "#!/usr/bin/python\n# -*- coding: utf-8 -*-\n\n" 
			+ TextCodeGenerator::generateScript(isApplicationCondition);
	return script;
}

QString PythonGenerator::property(Id const &element, QString const &propertyName) const
{
	QString res = TextCodeGenerator::property(element, propertyName);

	if (res == "true") {
		res = "True";
	}
	if (res == "false") {
		res = "False";
	}

	return res;
}

QString PythonGenerator::createProperInitAndOutput(QString const &code, bool const isApplicationCondition) const
{
	QString init = "";
	QString output = "print ''";

	if (mPropertiesUsage.keys().isEmpty()) {
		output += " + 'empty reaction'";
	} else {
		foreach (QString const &elemName, mPropertiesUsage.keys()) {
			foreach (QString const &propertyName, *mPropertiesUsage.value(elemName)) {
				QString const variable = elemName + delimeter + propertyName;
				QString const curPropertyValue = property(mMatch.value(idByName(elemName)), propertyName);
	
				bool isStringProp = isStringProperty(mMatch.value(idByName(elemName)), propertyName);
				QString const propertyValue = isStringProp ? "'" + escape(curPropertyValue) + "'" : curPropertyValue;
				QString const representationOfProperty = "'\\'' + str(" + variable + ") + '\\''";
	
				init += variable + "=" + propertyValue + "; ";
				output += " + '" + variable + "=' + " + representationOfProperty + " + ';'";
			}
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
