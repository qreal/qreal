#include "variables.h"

using namespace qReal;
using namespace robots::generator;

Variables::Variables()
{
}

void Variables::reinit(qrRepo::RepoApi *api)
{
	clear();
	QMap<QString, VariableType> const reservedVars = reservedVariables();
	foreach (QString const &var, reservedVars.keys()) {
		insert(var, reservedVars[var]);
	}
	QStringList expressions;
	IdList const blocks = api->elementsByType("Function");
	foreach (Id const &block, blocks) {
		if (api->hasProperty(block, "Body")) {
			expressions << api->stringProperty(block, "Body");
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

	foreach (QString const &intConst, intConsts.keys()) {
		result += QString("static const int %1 = %2;\n").arg(intConst, QString::number(intConsts[intConst]));
	}
	foreach (QString const &floatConst, floatConsts.keys()) {
		result += QString("static const float %1 = %2;\n").arg(floatConst, QString::number(floatConsts[floatConst]));
	}

	foreach (QString const &curVariable, keys()) {
		if (reservedNames.contains(curVariable)) {
			continue;
		}
		// If any of code pathes decided that this variable has int type
		// then it has int one. Unknown types are maximal ones (float)
		QString const type = value(curVariable) == intType ? "int" : "float";
		result += QString("static %1 %2;\n").arg(type, curVariable);
	}
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
		insert(variable, unknown);
		variableGroups.insert(variable, QStringList());
		QStringList const initializationList = rawGroups.value(variable);
		bool allInts = true;
		foreach (QString const &initialization, initializationList) {
			VariableType const inferredType = participatingVariables(initialization
					, variableGroups[variable]);
			// If we met at least one float expression then variable automaticly
			// becomes float. Else we have only int and unknown expressions.
			// If we met at least one unknown one than we can`t say that our
			// variable has int type so it stays unknown
			if (inferredType == floatType) {
				allInts = false;
				earlyFloats << variable;
				break;
			}
			if (inferredType == unknown) {
				allInts = false;
			}
		}
		if (allInts) {
			earlyInts << variable;
		}
	}

	foreach (QString const &intVariable, earlyInts) {
		assignType(intVariable, intType);
		// The type is already known, it must not participate in further inference
		variableGroups.remove(intVariable);
	}
	foreach (QString const &floatVariable, earlyFloats) {
		assignType(floatVariable, floatType);
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

QMap<QString, VariableType> Variables::nonGenerableReservedVariables() const
{
	QMap<QString, VariableType> result;
	result.insert("Sensor1", intType);
	result.insert("Sensor2", intType);
	result.insert("Sensor3", intType);
	result.insert("Sensor4", intType);
	result.insert("EncoderA", intType);
	result.insert("EncoderB", intType);
	result.insert("EncoderC", intType);
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

QMap<QString, VariableType> Variables::reservedVariables() const
{
	QMap<QString, VariableType> result(nonGenerableReservedVariables());
	QMap<QString, int> const intVars = intConstants();
	QMap<QString, float> const floatVars = floatConstants();
	foreach (QString const &intVar, intVars.keys()) {
		result.insert(intVar, intType);
	}
	foreach (QString const &floatVar, floatVars.keys()) {
		result.insert(floatVar, floatType);
	}
	return result;
}

void Variables::assignType(QString const &name, VariableType type)
{
	if (!contains(name)) {
		insert(name, type);
		return;
	}
	VariableType const oldType = value(name);
	switch (oldType) {
	case unknown:
		insert(name, type);
		break;
	case intType:
		if (type == floatType) {
			insert(name, floatType);
		}
		break;
	default:
		// float type is the widest one, do nothing
		break;
	}
}

VariableType Variables::participatingVariables(QString const &expression, QStringList &currentNames)
{
	// Performing quick processing of the expression, no parsing with syntax checking.
	// So syntax erros may cause incorrect inferrer work
	QStringList const tokens = expression.split(QRegExp("[\\s\\+\\-\\*/\\(\\)\\%]+"));
	bool metVariables = false;

	foreach (QString const &token, tokens) {
		bool ok;
		token.toInt(&ok);
		if (ok) {
			continue;
		}
		token.toFloat(&ok);
		if (ok) {
			// Met float constant, all the expression has float type
			return floatType;
		}
		if (isIdentifier(token) && !currentNames.contains(token)) {
			currentNames << token;
			metVariables = true;
		} else {
			// Syntax error
			return floatType;
		}
	}

	return metVariables ? unknown : intType;
}

bool Variables::isIdentifier(QString const &token)
{
	if (token.isEmpty() || !isLetter(token[0])) {
		return false;
	}
	foreach (QChar const symbol, token) {
		if (!isLetter(symbol) && !isDigit(symbol)) {
			return false;
		}
	}
	return true;
}

bool Variables::isDigit(QChar const &c) const
{
	char symbol = c.toLatin1();
	return '0' <= symbol && symbol <= '9';
}

bool Variables::isLetter(QChar const &c) const
{
	char symbol = c.toLatin1();
	return ('A' <= symbol && symbol <= 'Z') || ('a'<= symbol && symbol <= 'z');
}

void Variables::startDeepInference(QMap<QString, QStringList> &dependencies)
{
	bool somethingChanged = true;
	while (!dependencies.isEmpty() && somethingChanged) {
		somethingChanged = false;
		// Stage I: substituting all known types
		foreach (QString const &varName, keys()) {
			VariableType const currentType = value(varName);
			switch (currentType) {
			case intType:
				removeDependenciesFrom(dependencies, varName);
				break;
			case floatType: {
				QStringList const inferredToFloat = dependentFrom(dependencies, varName);
				somethingChanged |= !inferredToFloat.isEmpty();
				foreach (QString const &floatVar, inferredToFloat) {
					assignType(floatVar, floatType);
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
				assignType(varName, intType);
			}
		}
	}
	// Stage III: we may have some uninferred variables in result.
	// This may be caused by cyclic depenencies or undeclared variables usage.
	// Assigning for all of them float types for a while
	foreach (QString const &varName, dependencies.keys()) {
		assignType(varName, floatType);
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
		, QString const variable)
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
