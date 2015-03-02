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

QStringList ConvertingMethods::convertExplosionListIntoStringList(QList<Explosion> explosionList)
{
	QStringList result;
	foreach (Explosion const &explosion, explosionList) {
		QString const &target = explosion.target().toString();
		result.append(target);
	}
	return result;
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

	for (auto &outputElement : output) {
		result.append(outputElement + ",");
	}

	return result;
}
QString ConvertingMethods::transformateOutputOther(
		QList<QPair<QString, QString>> const &output
		, Id const &id
		, QString const &name
		)
{
	return "";
}


QSet<QString> ConvertingMethods::resultToCompare(QString const &method)
{
	QStringList methodOutput = method.split("|");

	QStringList result;
	foreach (QString const &string, methodOutput) {
		QString output = string.split("-").last();
		QStringList outputToList = output.split(",");

		result.append(outputToList);
	}

	QSet<QString> methodParsed = result.toSet();
	return methodParsed;
}

