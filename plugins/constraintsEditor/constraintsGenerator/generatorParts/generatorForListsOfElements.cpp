#include "generatorForListsOfElements.h"

#include "generatorUtils/auxiliaryMethods.h"

using namespace qReal;
using namespace constraints::generator::generatorParts;
using namespace constraints::generator::generatorUtils;

QPair<QString, QList<QString> > GeneratorForListsOfElements::countConstraintForChildrens(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "Children"
			, "children"
			, "node"
			, depth
			, additionalString
			, api
			, errorReporter);
}

QPair<QString, QList<QString> > GeneratorForListsOfElements::countConstraintForOutgoingLinks(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "OutgoingLinks"
			, "outgoingLinks"
			, "edge"
			, depth
			, additionalString
			, api
			, errorReporter);
}

QPair<QString, QList<QString> > GeneratorForListsOfElements::countConstraintForIncomingLinks(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "IncomingLinks"
			, "incomingLinks"
			, "edge"
			, depth
			, additionalString
			, api
			, errorReporter);
}

QPair<QString, QList<QString> > GeneratorForListsOfElements::countConstraintForOutgoingNodes(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "OutgoingNodes"
			, "outgoingNodes"
			, "node"
			, depth
			, additionalString
			, api
			, errorReporter);
}

QPair<QString, QList<QString> > GeneratorForListsOfElements::countConstraintForIncomingNodes(
		const Id &constraint
		, const QString &elementName
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter)
{
	return countConstraintForListOfElements(
			constraint
			, elementName
			, "IncomingNodes"
			, "incomingNodes"
			, "node"
			, depth
			, additionalString
			, api
			, errorReporter);
}


QPair<QString, QList<QString> > GeneratorForListsOfElements::countConstraintForListOfElements(
		const Id &constraint
		, const QString &elementName
		, const QString &resElementName
		, const QString &functionName
		, const QString &resType
		, const int depth
		, const QString &additionalString
		, const qrRepo::LogicalRepoApi &api
		, ErrorReporterInterface &errorReporter)
{
	QString resultString = "";
	QList<QString> resBool;
	QList<QString> allResultBool;

	QString count = api.property(constraint, "count").toString();
	bool neededCount  = (!count.isEmpty());

	if (neededCount) {
		if (QRegExp("(([<>=]|&lt;|&rt;)=?)\\s*(\\d+)").exactMatch(count)) {
			count.replace(QRegExp("([<>=&rlt;]+)(\\d+)"), "\\1 \\2");
		} else {
			errorReporter.addCritical(QObject::tr("Value \"%1\" is not correct for property 'Count'.").arg(count)
					, constraint);
			neededCount = false;
		}
	}

	if (neededCount) {
		resultString += QString("%1int tempCount%2_%3 = 0;\n")
				.arg(additionalString)
				.arg(resElementName)
				.arg(depth);
	}

	resultString += QString("%1qReal::IdList new%2NamesList_%3 = logicalApi.%4(%5);\n"
			"%1bool main%2Res_%6 = true;\n")
			.arg(additionalString)
			.arg(resElementName)
			.arg(depth)
			.arg(functionName)
			.arg(elementName)
			.arg(depth + 1);

	const QString curElementOfList = resType
			+ resElementName
			+ "_"
			+ QString::number(depth);

	resultString += QString("%1foreach (qReal::Id const &%2, new%3NamesList_%4) {\n")
			.arg(additionalString)
			.arg(curElementOfList)
			.arg(resElementName)
			.arg(depth);

	const QString selection = api.property(constraint, "selection").toString();
	const bool neededSelection  = (!selection.isEmpty()) && (selection.compare("all", Qt::CaseInsensitive) != 0);

	if (neededSelection) {
		QList<QString> selectionList = selection.split(" ");
//		QPair<QString, QString> selectionRes = countPropertyCharacteristicForConstraintElement(
//				constraint
//				, resElementName.at(0).toLower() + resElementName.mid(1) + "Selection"
//				, "false"
//				, selectionList.at(0)
//				, selectionList.at(1)
//				, selectionList.at(2)
//				, curElementOfList
//				, depth
//				, additionalString + "	");

//		resultString += QString("%1%2	if (%3) {\n")
//				.arg(selectionRes.first)
//				.arg(additionalString)
//				.arg(selectionRes.second);
	}

//	if (resType == "node") {
//		resultString += countRealConstraintForNodeElement(
//				constraint
//				, curElementOfList
//				, functionName + "Res"
//				, depth + 1
//				, additionalString + "	");
//	} else if (resType == "edge") {
//		resultString += countRealConstraintForEdgeElement(
//				constraint
//				, curElementOfList
//				, functionName + "Res"
//				, depth + 1
//				, additionalString + "	");
//	}

	resultString += QString("%1	main%2Res_%3 = main%2Res_%3 && %4Res_%3;\n")
			.arg(additionalString)
			.arg(resElementName)
			.arg(depth + 1)
			.arg(functionName);

	if (neededCount) {
		resultString += QString("%1	tempCount%2_%3++;\n")
				.arg(resElementName)
				.arg(additionalString)
				.arg(depth);
	}

	if (neededSelection) {
		resultString += QString("%1	}\n").arg(additionalString);
	}

	resultString += additionalString + "}\n";
	resBool.push_back("main" + resElementName + "Res_" + QString::number(depth + 1));

	if (neededCount) {
		QList<QString> countList = count.split(" ");
		QString sign = countList.at(0);

		if (sign == "=") {
			sign += "=";
		}

		sign.replace("&lt;", "<");

		resultString += QString("%1bool count%2Res_%3 = (tempCount%2_%3 %4 %5);\n")
				.arg(additionalString)
				.arg(resElementName)
				.arg(depth)
				.arg(sign)
				.arg(countList.at(1));
		resBool.push_back("count" + resElementName + "Res_" + QString::number(depth));
	}

	allResultBool.append(AuxiliaryMethods::conjunctionExpression(resBool));

	return QPair<QString, QList<QString> >(resultString, allResultBool);
}
