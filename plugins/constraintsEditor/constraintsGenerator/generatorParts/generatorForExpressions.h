#pragma once

#include <QtCore/QString>

#include <qrrepo/logicalRepoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

namespace constraints {
namespace generator {
namespace generatorParts {

/// Methods in this class generate concrete expressions (for example, conjunction or disjunction).
class GeneratorForExpressions
{
public:
	/// Forms disjunction expression given list of boolean expressions.
	/// @param listOfBooleanExpressions - list of expressions.
	/// @returns boolean expressions list connected by "||" symbol.
	static QString disjunctionExpression(QList<QString> listOfBooleanExpressions);

	/// Forms conjunction expression given list of boolean expressions.
	/// @param listOfBooleanExpressions - list of expressions.
	/// @returns boolean expressions list connected by "&&" symbol.
	static QString conjunctionExpression(QList<QString> listOfBooleanExpressions);

private:
	static QString formedExpression(
			QList<QString> listOfBooleanExpressions
			, const QString &firstElement
			, const QString &connectingSymbol);
};

}
}
}
