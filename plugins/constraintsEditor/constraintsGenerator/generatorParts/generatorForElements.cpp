#include "generatorForElements.h"
#include "generatorForEdgeElements.h"
#include "generatorForNodeElements.h"
#include "generatorUtils/auxiliaryMethods.h"

using namespace constraints::generator::generatorParts;
using namespace constraints::generator::generatorUtils;
using namespace qReal;
using namespace qrRepo;

QString GeneratorForElements::countRealConstraintForElement(const Id &constraintElement
		, const metaType &type
		, const QString &elementName
		, const QString &resultName
		, const int depth
		, const QString &addStr
		, const LogicalRepoApi &api
		, QMap<QString, int> &countsOfConstraintElementsInOneConstraint
		, ErrorReporterInterface &errorReporter)
{
	IdList list = api.children(constraintElement);
	QString resString = "";
	QList<QString> resBool;
	IdList usedElements;

	for (const Id &constraint : list) {
		if (!usedElements.contains(constraint)) {
			QPair<QString, QList<QString> > resOneConstraint;
			if (type == edge) {
				resOneConstraint = GeneratorForEdgeElements::countRealConstraintForOneEdgeElement(
						constraint
						, usedElements
						, elementName
						, depth
						, addStr
						, api
						, countsOfConstraintElementsInOneConstraint
						, errorReporter);
			} else if (type == node) {
				resOneConstraint = GeneratorForNodeElements::countRealConstraintForOneNodeElement(
						constraint
						, usedElements
						, elementName
						, depth
						, addStr
						, api
						, countsOfConstraintElementsInOneConstraint
						, errorReporter);
			}

			resString += resOneConstraint.first;
			resBool.append(resOneConstraint.second);
		}
	}

	resString += additionalCommonPartForConstraint(resBool, resultName, depth, addStr); //сбрасываем resBool в resString

	return resString;
}

QString GeneratorForElements::additionalCommonPartForConstraint(
		const QList<QString> &resBool
		, const QString &resultName
		, const int depth
		, const QString &addStr)
{
	QString resString = "";

	resString += QString("%1	bool %2_%3 = %4;\n")
			.arg(addStr)
			.arg(resultName)
			.arg(depth)
			.arg(AuxiliaryMethods::conjunctionExpression(resBool));

	return resString;
}
