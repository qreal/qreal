#pragma once

#include <QtCore/QString>

#include <qrrepo/logicalRepoApi.h>

namespace constraints {
namespace generator {
namespace generatorUtils {

class AuxiliaryMethods
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

	/// Returns list of neighbor nodes for given element with given type.
	/// @param element - element id.
	/// @param type - type of node.
	/// @param api - information about model.
	/// @returns list of neighbor nodes ids.
	static qReal::IdList neighborNodesWithGivenType(
			const qReal::Id &element
			, const QString &type
			, const qrRepo::LogicalRepoApi &api);

	/// Returns true if link with given type and connected to given element exists.
	/// @param element - element id.
	/// @param type - needed type.
	/// @param api - information about model.
	/// @returns true if needed link exists, otherwise false.
	static bool linkWithGivenTypeExists(
			const qReal::Id &element
			, const QString &type
			, const qrRepo::LogicalRepoApi &api);

	static QString generateExistsProperty(
			const QString &resElementName
			, const QString &elementName
			, const qReal::Id &constraint
			, const int depth
			, const QString &additionalString
			, const qrRepo::LogicalRepoApi &api);

private:
	static QString formedExpression(
			QList<QString> listOfBooleanExpressions
			, const QString &firstElement
			, const QString &connectingSymbol);
};

}
}
}
