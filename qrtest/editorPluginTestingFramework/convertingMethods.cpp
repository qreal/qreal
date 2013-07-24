#include "convertingMethods.h"

using namespace editorPluginTestingFramework;
using namespace qReal;

QStringList ConvertingMethods::convertIdListIntoStringList(IdList const idList)
{
	QStringList resultList;

	foreach (Id const &id, idList) {
		resultList += id.toString();
	}

	return resultList;
}

QStringList ConvertingMethods::convertStringIntoStringList(QString const &string)
{
	QStringList result;
	result.append(string);
	return result;
}

QStringList ConvertingMethods::convertBoolIntoStringList(bool const &boolValue)
{
	QString stringRepresentation = (boolValue) ? "true" : "false";
	return convertStringIntoStringList(stringRepresentation);
}

QStringList ConvertingMethods::convertIdIntoStringList(Id const &id)
{
	QString stringRepresentation = id.toString();
	return convertStringIntoStringList(stringRepresentation);
}

QStringList ConvertingMethods::convertIntIntoStringList(int const &integer)
{
	QString stringRepresentation = (integer == 1) ? "1" : "0";
	return convertStringIntoStringList(stringRepresentation);
}

QString ConvertingMethods::transformateOutput(
		QStringList const &output
		, Id const &id
		, QString const &name
		)
{
	QString result;
	if (name == "") {
		result.append(id.toString() + "-");
	} else {
		result.append(name + "-");
	}

	foreach (QString const &outputElement, output) {
		result.append(outputElement + ",");
	}

	return result;
}

