#include "generatorBase/parts/variables.h"

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

Variables::Variables(QString const &pathToTemplates
		, interpreterBase::robotModel::RobotModelInterface const &robotModel
		, qrtext::LanguageToolboxInterface &luaToolbox)
	: TemplateParametrizedEntity(pathToTemplates)
	, mRobotModel(robotModel)
	, mLuaToolbox(luaToolbox)
{
}

QStringList Variables::expressions(qrRepo::RepoApi const &api) const
{
	QStringList result;
	IdList const funtionBlocks = api.elementsByType("Function");
	for (Id const &block : funtionBlocks) {
		if (api.hasProperty(block, "Body")) {
			result << api.stringProperty(block, "Body");
		}
	}

	IdList const initializationBlocks = api.elementsByType("VariableInit");
	for (Id const &block : initializationBlocks) {
		if (api.hasProperty(block, "variable") && api.hasProperty(block, "value")) {
			result << api.stringProperty(block, "variable") + " = " + api.stringProperty(block, "value");
		}
	}

	return result;
}

QString Variables::generateVariableString() const
{
	QString result;
	QMap<QString, QSharedPointer<qrtext::core::types::TypeExpression>> const variables = mLuaToolbox.variableTypes();
	QStringList const reservedNames = mLuaToolbox.specialIdentifiers();

	for (QString const &constantName : mLuaToolbox.specialConstants()) {
		result += QString(constantDeclaration(variables[constantName])).replace("@@NAME@@", constantName)
				.replace("@@VALUE@@", mLuaToolbox.value<QString>(constantName));
	}

	for (QString const &curVariable : variables.keys()) {
		if (reservedNames.contains(curVariable)) {
			continue;
		}

		result += variableDeclaration(variables[curVariable]).replace("@@NAME@@", curVariable);
	}

	result += mManualDeclarations.join('\n');

	return result;
}

QString Variables::typeExpression(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const
{
	if (type->is<qrtext::lua::types::Integer>()) {
		return readTemplate("types/int.t");
	} else if (type->is<qrtext::lua::types::Float>()) {
		return readTemplate("types/float.t");
	} else if (type->is<qrtext::lua::types::Boolean>()) {
		return readTemplate("types/bool.t");
	} else if (type->is<qrtext::lua::types::String>()) {
		return readTemplate("types/string.t");
	} else if (type->is<qrtext::lua::types::Table>()) {
		auto const elementType = qrtext::as<qrtext::lua::types::Table>(type)->elementType();
		return readTemplate("types/array.t").replace("@@ELEMENT_TYPE@@", typeExpression(elementType));
	}

	return readTemplate("<unknown_type!>");
}

QString Variables::constantDeclaration(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const
{
	return readTemplate("variables/constantDeclaration.t").replace("@@TYPE@@", typeExpression(type));
}

QString Variables::variableDeclaration(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const
{
	return readTemplate("variables/variableDeclaration.t").replace("@@TYPE@@", typeExpression(type));
}

QSharedPointer<qrtext::core::types::TypeExpression> Variables::expressionType(QString const &expression) const
{
	QSharedPointer<qrtext::core::ast::Node> const &ast = mLuaToolbox.parse(Id(), QString(), expression);
	return mLuaToolbox.type(ast);
}

void Variables::appendManualDeclaration(QString const &variables)
{
	if (!mManualDeclarations.contains(variables) && !variables.isEmpty()) {
		mManualDeclarations << variables;
	}
}
