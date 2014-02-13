#include "variables.h"

using namespace qReal;
using namespace robots::generators;
using namespace parts;

Variables::Variables(QString const &pathToTemplates)
	: TemplateParametrizedEntity(pathToTemplates)
{
}

void Variables::reinit(qrRepo::RepoApi const &api)
{
	mVariables.clear();
	QMap<QString, enums::variableType::VariableType> const reservedVars = reservedVariables();
	foreach (QString const &var, reservedVars.keys()) {
		mVariables.insert(var, reservedVars[var]);
	}

	QStringList expressions;
	IdList const blocks = api.elementsByType("Function");
	foreach (Id const &block, blocks) {
		if (api.hasProperty(block, "Body")) {
			expressions << api.stringProperty(block, "Body");
		}
	}

	inferTypes(expressions);
}

QString Variables::generateVariableString() const
{
	QStringList const reservedNames = reservedVariables().keys();
	QMap<QString, int> const intConsts = intConstants();
	QMap<QString, float> const floatConsts = floatConstants();
	QString result = "\n";

	// TODO: read it from template
	foreach (QString const &intConst, intConsts.keys()) {
		result += QString(intConstantDeclaration()).replace("@@NAME@@", intConst)
				.replace("@@VALUE@@", QString::number(intConsts[intConst]));
	}

	foreach (QString const &floatConst, floatConsts.keys()) {
		result += QString(floatConstantDeclaration()).replace("@@NAME@@", floatConst)
				.replace("@@VALUE@@", QString::number(floatConsts[floatConst]));
	}

	foreach (QString const &curVariable, mVariables.keys()) {
		if (reservedNames.contains(curVariable)) {
			continue;
		}
		// If every code path decided that this variable has int type
		// then it has int one. Unknown types are maximal ones (float)
		QString pattern = mVariables.value(curVariable) == enums::variableType::intType
				? intVariableDeclaration() : floatVariableDeclaration();
		result += pattern.replace("@@NAME@@", curVariable);
	}

	result += mManualDeclarations.join('\n');

	return result;
}

void Variables::inferTypes(QStringList const &expressions)
{
	QMap<QString, QStringList> rawGroups(variablesExpressionsMap(expressions));
	QMap<QString, QStringList> variableGroups;
	QStringList variableNames = rawGroups.keys();

	QStringList earlyFloats;
	QStringList earlyInts;

	foreach (QString const &variable, variableNames) {
		if (reservedVariables().contains(variable)) {
			// TODO: report error
			continue;
		}
		// Marking that we have this variable initializes among given expressions
		mVariables.insert(variable, enums::variableType::unknown);
		variableGroups.insert(variable, QStringList());
		QStringList const initializationList = rawGroups.value(variable);
		bool allInts = true;
		foreach (QString const &initialization, initializationList) {
			enums::variableType::VariableType const inferredType = participatingVariables(initialization
					, variableGroups[variable]);
			// If we met at least one float expression then variable automaticly
			// becomes float. Else we have only int and unknown expressions.
			// If we met at least one unknown one than we can`t say that our
			// variable has int type so it stays unknown
			if (inferredType == enums::variableType::floatType) {
				allInts = false;
				earlyFloats << variable;
				break;
			}
			if (inferredType == enums::variableType::unknown) {
				allInts = false;
			}
		}
		if (allInts) {
			earlyInts << variable;
		}
	}

	foreach (QString const &intVariable, earlyInts) {
		assignType(intVariable, enums::variableType::intType);
		// The type is already known, it must not participate in further inference
		variableGroups.remove(intVariable);
	}
	foreach (QString const &floatVariable, earlyFloats) {
		assignType(floatVariable, enums::variableType::floatType);
		// The type is already known, it must not participate in further inference
		variableGroups.remove(floatVariable);
	}

	startDeepInference(variableGroups);
}

QMap<QString, QStringList> Variables::variablesExpressionsMap(QStringList const &expressions) const
{
	QMap<QString, QStringList> result;
	QString const joinedExpressions = expressions.join("; ");
	QStringList const standaloneExpressions = joinedExpressions.split(";", QString::SkipEmptyParts);
	foreach (QString const &expression, standaloneExpressions) {
		QStringList const parts = expression.split("=", QString::SkipEmptyParts);
		if (parts.count() != 2) {
			// TODO: Do something
			continue;
		}
		QString const name = parts[0].trimmed();
		QString const initialization = parts[1].trimmed();
		QStringList const allInitializations(result[name] << initialization);
		result.insert(name, allInitializations);
	}
	return result;
}

QMap<QString, enums::variableType::VariableType> Variables::nonGenerableReservedVariables() const
{
	QMap<QString, enums::variableType::VariableType> result;
	result.insert("Sensor1", enums::variableType::intType);
	result.insert("Sensor2", enums::variableType::intType);
	result.insert("Sensor3", enums::variableType::intType);
	result.insert("Sensor4", enums::variableType::intType);
	result.insert("EncoderA", enums::variableType::intType);
	result.insert("EncoderB", enums::variableType::intType);
	result.insert("EncoderC", enums::variableType::intType);
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

QString Variables::intConstantDeclaration() const
{
	return readTemplate("variables/intConstantDeclaration.t");
}

QString Variables::floatConstantDeclaration() const
{
	return readTemplate("variables/floatConstantDeclaration.t");
}

QString Variables::intVariableDeclaration() const
{
	return readTemplate("variables/intVariableDeclaration.t");
}

QString Variables::floatVariableDeclaration() const
{
	return readTemplate("variables/floatVariableDeclaration.t");
}

QMap<QString, enums::variableType::VariableType> Variables::reservedVariables() const
{
	QMap<QString, enums::variableType::VariableType> result(nonGenerableReservedVariables());
	QMap<QString, int> const intVars = intConstants();
	QMap<QString, float> const floatVars = floatConstants();
	foreach (QString const &intVar, intVars.keys()) {
		result.insert(intVar, enums::variableType::intType);
	}

	foreach (QString const &floatVar, floatVars.keys()) {
		result.insert(floatVar, enums::variableType::floatType);
	}

	return result;
}

void Variables::assignType(QString const &name, enums::variableType::VariableType type)
{
	if (!mVariables.contains(name)) {
		mVariables.insert(name, type);
		return;
	}

	enums::variableType::VariableType const oldType = mVariables.value(name);
	switch (oldType) {
	case enums::variableType::unknown:
		mVariables.insert(name, type);
		break;
	case enums::variableType::intType:
		if (type == enums::variableType::floatType) {
			mVariables.insert(name, enums::variableType::floatType);
		}
		break;
	default:
		// float type is the widest one, do nothing
		break;
	}
}

enums::variableType::VariableType Variables::participatingVariables(QString const &expression
		, QStringList &currentNames) const
{
	// Performing quick processing of the expression, no parsing with syntax checking.
	// So syntax erros may cause incorrect inferrer work
	QStringList const tokens = expression.split(QRegExp("[\\s\\+\\-\\*/\\(\\)\\%]+"), QString::SkipEmptyParts);
	bool metVariables = false;

	foreach (QString const &token, tokens) {
		bool ok = false;
		token.toInt(&ok);
		if (ok) {
			continue;
		}

		token.toFloat(&ok);
		if (ok) {
			// Met float constant, all the expression has float type
			return enums::variableType::floatType;
		}

		if (isIdentifier(token) && !currentNames.contains(token)) {
			currentNames << token;
			metVariables = true;
		} else {
			// Syntax error
			return enums::variableType::floatType;
		}
	}

	return metVariables ? enums::variableType::unknown : enums::variableType::intType;
}

bool Variables::isIdentifier(QString const &token) const
{
	if (token.isEmpty() || !token[0].isLetter()) {
		return false;
	}

	foreach (QChar const symbol, token) {
		if (!symbol.isLetter() && !symbol.isDigit()) {
			return false;
		}
	}

	return true;
}

void Variables::startDeepInference(QMap<QString, QStringList> &dependencies)
{
	bool somethingChanged = true;
	while (!dependencies.isEmpty() && somethingChanged) {
		somethingChanged = false;
		// Stage I: substituting all known types
		foreach (QString const &varName, mVariables.keys()) {
			enums::variableType::VariableType const currentType = mVariables.value(varName);
			switch (currentType) {
			case enums::variableType::intType:
				removeDependenciesFrom(dependencies, varName);
				break;
			case enums::variableType::floatType: {
				QStringList const inferredToFloat = dependentFrom(dependencies, varName);
				somethingChanged |= !inferredToFloat.isEmpty();
				foreach (QString const &floatVar, inferredToFloat) {
					assignType(floatVar, enums::variableType::floatType);
					dependencies.remove(floatVar);
				}
				break;
			}
			default:
				// Do nothing
				break;
			}
		}
		// Stage II: removing new known types from inference list
		foreach (QString const &varName, dependencies.keys()) {
			if (dependencies[varName].isEmpty()) {
				somethingChanged = true;
				dependencies.remove(varName);
				assignType(varName, enums::variableType::intType);
			}
		}
	}
	// Stage III: we may have some uninferred variables in result.
	// This may be caused by cyclic depenencies or undeclared variables usage.
	// Assigning for all of them float types for a while
	foreach (QString const &varName, dependencies.keys()) {
		assignType(varName, enums::variableType::floatType);
	}
}

QStringList Variables::dependentFrom(QMap<QString, QStringList> const &dependencies
		, QString const variable) const
{
	QStringList result;
	foreach (QString const &key, dependencies.keys()) {
		if (dependencies.value(key).contains(variable)) {
			result << key;
		}
	}
	return result;
}

bool Variables::removeDependenciesFrom(QMap<QString, QStringList> &dependencies
		, QString const variable) const
{
	bool somethingChanged = false;
	foreach (QString const &key, dependencies.keys()) {
		QStringList values = dependencies.value(key);
		if (values.contains(variable)) {
			somethingChanged = true;
			values.removeAll(variable);
			dependencies.insert(key, values);
		}
	}
	return somethingChanged;
}

enums::variableType::VariableType Variables::expressionType(QString const &expression) const
{
	if (expression.isEmpty()) {
		return enums::variableType::intType;
	}

	QStringList variables;
	enums::variableType::VariableType const type = participatingVariables(expression, variables);

	if (type != enums::variableType::unknown) {
		return type;
	}

	foreach (QString const &variable, variables) {
		if (!mVariables.contains(variable)) {
			return enums::variableType::unknown;
		}

		if (mVariables.value(variable) != enums::variableType::intType) {
			return enums::variableType::floatType;
		}
	}

	// All constants and variables in expression have int type => expression too
	return enums::variableType::intType;
}

void Variables::appendManualDeclaration(QString const &variables)
{
	if (!mManualDeclarations.contains(variables) && !variables.isEmpty()) {
		mManualDeclarations << variables;
	}
}
