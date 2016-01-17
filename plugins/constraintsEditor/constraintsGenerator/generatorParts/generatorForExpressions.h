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
