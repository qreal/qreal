#pragma once

#include <QtCore/QString>

#include <qrkernel/ids.h>

#include "defs.h"
#include "concreteGenerator.h"

namespace constraints {
namespace generator {
namespace generatorUtils {

class CountRealConstraintsMethods
{
public:
	QString countRealConstraintOfDiagramElement(const qReal::Id &constraintElement);

	QString countRealConstraintForElement(
			const qReal::Id &constraintElement
			, const metaType &type
			, QString elementName
			, QString resultName
			, int depth
			, QString addStr) const;

	QString countRealConstraintForEdgeElement(
			const qReal::Id &constraintElement
			, QString elementName
			, QString resultName
			, int depth
			, QString addStr) const;

	QString countRealConstraintForNodeElement(
			const qReal::Id &constraintElement
			, QString elementName
			, QString resultName
			, int depth
			, QString addStr) const;

	QPair<QString, QList<QString> > countRealConstraintForOneEdgeElement(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, QString elementName
			, int depth
			, QString addStr
			, bool isMultiOr = false) const;

	QPair<QString, QList<QString> > countRealConstraintForOneNodeElement(
			const qReal::Id &constraint
			, qReal::IdList &usedElements
			, QString elementName
			, int depth
			, QString addStr
			, bool isMultiOr = false) const;

private:
	QMap<QString, int> mCountsOfConstraintElementsInOneConstraint;
};

}
}
}
