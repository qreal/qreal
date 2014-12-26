#include "qtScriptGenerator.h"

using namespace qReal;

QtScriptGenerator::QtScriptGenerator(LogicalModelAssistInterface &logicalModelApi
		, GraphicalModelAssistInterface &graphicalModelApi
		, gui::MainWindowInterpretersInterface &interpretersInterface)
		: TextCodeGenerator(logicalModelApi, graphicalModelApi, interpretersInterface)
{
}

QString QtScriptGenerator::createProperInitAndOutput(QString const &code, bool const isApplicationCondition) const
{
	QString init = "";
	QString output = "''";
	foreach (QString const &elemName, mPropertiesUsage.keys()) {
		foreach (QString const &propertyName, *mPropertiesUsage.value(elemName)) {
			QString const variable = elemName + delimeter + propertyName;
			QString const curPropertyValue = property(mMatch.value(idByName(elemName)), propertyName);

			bool isStringProp = isStringProperty(mMatch.value(idByName(elemName)), propertyName);
			QString const propertyValue = isStringProp ? "'" + escape(curPropertyValue) + "'" : curPropertyValue;
			QString const representationOfProperty = "'\\'' + String(" + variable + ") + '\\''";

			init += variable + "=" + propertyValue + "; ";
			output += " + '" + variable + "=' + " + representationOfProperty + " + ';'";
		}
	}
	if (!isApplicationCondition) {
		return init + "\n\n" + code + "\n\n" + output;
	} else {
		return init + "\n\n" + code;
	}
}

QString QtScriptGenerator::createBehaviourFunction(QString const &elementName, QString const &propertyName) const
{
	QString result = properElementProperty(elementName, propertyName);
	result = "function " + elementName + delimeter + propertyName + "() {\n" + result + "}\n\n";

	return result;
}

QString QtScriptGenerator::properElementProperty(QString const &elementName, QString const &propertyName) const
{
	QString propertyValue = property(mMatch.value(idByName(elementName)), propertyName);

	propertyValue.replace("this.", elementName + ".");
	propertyValue.replace("this@", elementName + "@");
	propertyValue.replace("\n", "\n  ");

	return propertyValue;
}
