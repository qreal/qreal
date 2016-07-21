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

#include "generatorBase/parts/variables.h"

#include <QtCore/QFileInfo>

#include <qrtext/lua/luaToolbox.h>
#include <qrtext/core/ast/node.h>
#include <qrtext/lua/types/integer.h>
#include <qrtext/lua/types/float.h>
#include <qrtext/lua/types/boolean.h>
#include <qrtext/lua/types/string.h>
#include <qrtext/lua/types/table.h>

using namespace generatorBase;
using namespace parts;
using namespace qReal;

Variables::Variables(const QStringList &pathsToTemplates
		, const kitBase::robotModel::RobotModelInterface &robotModel
		, qrtext::LanguageToolboxInterface &luaToolbox)
	: TemplateParametrizedEntity(pathsToTemplates)
	, mRobotModel(robotModel)
	, mLuaToolbox(luaToolbox)
{
}

QString Variables::generateVariableString() const
{
	QString result;
	const QMap<QString, QSharedPointer<qrtext::core::types::TypeExpression>> variables = mLuaToolbox.variableTypes();
	const QStringList reservedNames = mLuaToolbox.specialIdentifiers();

	for (const QString &constantName : mLuaToolbox.specialConstants()) {
		const QString value = mLuaToolbox.variableTypes()[constantName]->is<qrtext::lua::types::Float>()
				? readTemplateIfExists("floatFormat.t", "@@VALUE@@")
						.replace("@@VALUE@@", mLuaToolbox.value<QString>(constantName))
				: mLuaToolbox.value<QString>(constantName);
		result += QString(constantDeclaration(variables[constantName])).replace("@@NAME@@", constantName)
				.replace("@@VALUE@@", value);
	}

	for (const QString &curVariable : variables.keys()) {
		if (reservedNames.contains(curVariable)) {
			continue;
		}

		result += variableDeclaration(variables[curVariable]).replace("@@NAME@@", curVariable);
	}

	result += mManualDeclarations.join('\n');

	return result;
}

QString Variables::typeExpression(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const
{
	const QString typeName = this->typeName(type);
	QString typeTemplate = readTemplate(QString("types/%1.t").arg(typeName));
	if (type->is<qrtext::lua::types::Table>()) {
		const auto elementType = qrtext::as<qrtext::lua::types::Table>(type)->elementType();
		return typeTemplate.replace("@@ELEMENT_TYPE@@", typeExpression(elementType));
	}

	return typeTemplate;
}

QString Variables::typeName(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const
{
	if (type->is<qrtext::lua::types::Integer>()) {
		return "int";
	} else if (type->is<qrtext::lua::types::Float>()) {
		return "float";
	} else if (type->is<qrtext::lua::types::Boolean>()) {
		return "bool";
	} else if (type->is<qrtext::lua::types::String>()) {
		return "string";
	} else if (type->is<qrtext::lua::types::Table>()) {
		return "array";
	}

	/// @todo: Add error reporting?
	return "int";
}

QString Variables::constantDeclaration(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const
{
	return readTemplate("variables/constantDeclaration.t").replace("@@TYPE@@", typeExpression(type));
}

QString Variables::variableDeclaration(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const
{
	const QString universalTemplate = readTemplate("variables/variableDeclaration.t");
	QString concreteTemplate = readTemplateIfExists(QString("variables/%1VariableDeclaration.t")
			.arg(typeName(type)), universalTemplate);
	return concreteTemplate.replace("@@TYPE@@", typeExpression(type));
}

QSharedPointer<qrtext::core::types::TypeExpression> Variables::expressionType(const QString &expression) const
{
	const QSharedPointer<qrtext::core::ast::Node> &ast = mLuaToolbox.parse(Id(), QString(), expression);
	return mLuaToolbox.type(ast);
}

void Variables::appendManualDeclaration(const QString &variables)
{
	if (!mManualDeclarations.contains(variables) && !variables.isEmpty()) {
		mManualDeclarations << variables;
	}
}
