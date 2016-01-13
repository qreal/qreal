#include "generatorForProperties.h"

using namespace constraints::generator::generatorParts;
using namespace qReal;
using namespace qrRepo;

QPair<QString, QString> GeneratorForProperties::countPropertyCharacteristicForConstraintElement(
		const Id &constraint
		, const QString &characteristicName
		, const QString &defaultValue
		, const QString &property
		, QString sign
		, QString value
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, ErrorReporterInterface &errorReporter)
{
	QString resString = "";

	if (sign == "=") {
		sign += "=";
	}

	sign.replace("&lt;", "<");

	resString += QString("%1bool %2Res_%3 = %4;\n")
			.arg(additionalString)
			.arg(characteristicName)
			.arg(depth)
			.arg(defaultValue);

	if (property.compare("TYPE", Qt::CaseSensitive) == 0) {
		resString += QString("%1if (%2 != qReal::Id::rootId()) {\n"
				"%1	%3Res_%4 = (%2.element() %5 \"%6\");\n"
				"%1}\n")
				.arg(additionalString)
				.arg(elementName)
				.arg(characteristicName)
				.arg(depth)
				.arg(sign)
				.arg(value);
	} else if (property.compare("METATYPE", Qt::CaseInsensitive) == 0) {
		resString += QString("%1if (%2 != qReal::Id::rootId()) {\n"
				"%1	qReal::EditorManagerInterface::MetaType isNodeOrEdge = mEditorManager->metaTypeOfElement(%2);\n")
				.arg(additionalString)
				.arg(elementName);

		if (value.compare("node", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::node";
		} else if (value.compare("edge", Qt::CaseInsensitive) == 0) {
			value = "qReal::EditorManagerInterface::edge";
		} else {
			errorReporter.addCritical(QObject::tr("Metatype \"%1\" does not exist. Select \"node\" or \"edge\".")
					.arg(value), constraint);
		}

		resString += QString("%1	%2Res_%3 = (isNodeOrEdge %4 %5);\n"
				"%1}\n")
				.arg(additionalString)
				.arg(characteristicName)
				.arg(depth)
				.arg(sign)
				.arg(value);
	} else {
		bool valueIsInt = false;
		int intValue = value.toInt(&valueIsInt);
		Q_UNUSED(intValue);

		resString += QString("%1if (logicalApi.hasProperty(%2, \"%3\")) {\n"
				"%1	%4Res_%5 = (logicalApi.property(%2, \"%3\").")
				.arg(additionalString)
				.arg(elementName)
				.arg(property)
				.arg(characteristicName)
				.arg(depth);

		if (!valueIsInt) {
			resString += QString("toString() %1  \"%2\");\n")
					.arg(sign)
					.arg(value);
		} else {
			resString += QString("toInt() %1 %2);\n")
					.arg(sign)
					.arg(value);
		}

		resString += additionalString + "}\n";
	}

	const QString resBool = QString("%1Res_%2")
			.arg(characteristicName)
			.arg(depth);

	return QPair<QString, QString>(resString, resBool);
}

QString GeneratorForProperties::generateExistsProperty(
		const QString &resElementName
		, const QString &elementName
		, const Id &constraint
		, const int depth
		, const QString &additionalString
		, const LogicalRepoApi &api)
{
	QString resultString = "";
	const QString exists = api.property(constraint, "exists").toString();

	resultString += additionalString + "bool " + resElementName + "_" + QString::number(depth) + " = ";

	if (exists == "true" || exists == "false") {
		const QString comparisonSymbol = (exists == "true") ? "!=" : "==";
		resultString += "(" + elementName + "_" + QString::number(depth) + " " + comparisonSymbol + " qReal::Id::rootId());\n";
	} else {
		resultString += "true;\n";
	}

	return resultString;
}
