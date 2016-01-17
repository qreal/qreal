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

#include "generatorForElements.h"
#include "generatorForEdgeElements.h"
#include "generatorForNodeElements.h"
#include "generatorForExpressions.h"

using namespace constraints::generator::generatorParts;
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
			.arg(GeneratorForExpressions::conjunctionExpression(resBool));

	return resString;
}
