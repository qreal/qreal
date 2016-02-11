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

#include <QtCore/QMap>
#include <QtCore/QStringList>

#include <qrtext/core/types/typeExpression.h>
#include <qrrepo/repoApi.h>
#include <kitBase/robotModel/robotModelInterface.h>

#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace qrtext {
class LanguageToolboxInterface;
}

namespace generatorBase {
namespace parts {
// TODO: make this class customizable in other generators

/// Manages everything about variables and their types
class ROBOTS_GENERATOR_EXPORT Variables : public TemplateParametrizedEntity
{
public:
	Variables(const QStringList &pathsToTemplates
			, const kitBase::robotModel::RobotModelInterface &robotModel
			, qrtext::LanguageToolboxInterface &luaToolbox);

	/// Returns global variables declarations string
	QString generateVariableString() const;

	/// Returns given expression type using inner type inference algorithm
	QSharedPointer<qrtext::core::types::TypeExpression> expressionType(const QString &expression) const;

	/// Adds given string to variables declaration section.
	/// This method can be called multiple times with the same string but it
	/// will be deduplicated.
	void appendManualDeclaration(const QString &variables);

protected:
	QString typeExpression(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const;
	QString typeName(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const;
	QString constantDeclaration(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const;
	QString variableDeclaration(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const;

private:
	const kitBase::robotModel::RobotModelInterface &mRobotModel;
	qrtext::LanguageToolboxInterface &mLuaToolbox;
	QStringList mManualDeclarations;
};

}
}
