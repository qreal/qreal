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
	QStringList const reservedNames = reservedVariables().keys();
	QMap<QString, int> const intConsts = intConstants();
	QMap<QString, float> const floatConsts = floatConstants();
	QString result = "\n";

	QSharedPointer<qrtext::core::types::TypeExpression> intType(new qrtext::lua::types::Integer());
	QSharedPointer<qrtext::core::types::TypeExpression> floatType(new qrtext::lua::types::Float());

	for (QString const &intConst : intConsts.keys()) {
		result += QString(constantDeclaration(intType)).replace("@@NAME@@", intConst)
				.replace("@@VALUE@@", QString::number(intConsts[intConst]));
	}

	for (QString const &floatConst : floatConsts.keys()) {
		result += QString(constantDeclaration(floatType)).replace("@@NAME@@", floatConst)
				.replace("@@VALUE@@", QString::number(floatConsts[floatConst]));
	}


	QMap<QString, QSharedPointer<qrtext::core::types::TypeExpression>> const variables = mLuaToolbox.variableTypes();
	for (QString const &curVariable : variables.keys()) {
		if (reservedNames.contains(curVariable)) {
			continue;
		}

		result += variableDeclaration(variables[curVariable]).replace("@@NAME@@", curVariable);
	}

	result += mManualDeclarations.join('\n');

	return result;
}

QMap<QString, qrtext::core::types::TypeExpression> Variables::nonGenerableReservedVariables() const
{
	/// @todo: Ask for it toolbox, remove thus copy-paste from RobotsBlockParser.
	QMap<QString, qrtext::core::types::TypeExpression> result;
	for (interpreterBase::robotModel::PortInfo const &port : mRobotModel.availablePorts()) {
		result.insert(port.name(), qrtext::lua::types::String());

		for (QString const &alias : port.nameAliases()) {
			result.insert(alias, qrtext::lua::types::String());
		}

		if (!port.reservedVariable().isEmpty()) {
			result.insert(port.reservedVariable(), qrtext::lua::types::Integer());
		}
	}

	return result;
}

QMap<QString, int> Variables::intConstants() const
{
	return QMap<QString, int>();
}

QMap<QString, float> Variables::floatConstants() const
{
	QMap<QString, float> result;
	result.insert("pi", 3.14159265);
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

QMap<QString, qrtext::core::types::TypeExpression> Variables::reservedVariables() const
{
	QMap<QString, qrtext::core::types::TypeExpression> result(nonGenerableReservedVariables());
	QMap<QString, int> const intVars = intConstants();
	QMap<QString, float> const floatVars = floatConstants();
	for (QString const &intVar : intVars.keys()) {
		result.insert(intVar, qrtext::lua::types::Integer());
	}

	for (QString const &floatVar : floatVars.keys()) {
		result.insert(floatVar, qrtext::lua::types::Float());
	}

	return result;
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
