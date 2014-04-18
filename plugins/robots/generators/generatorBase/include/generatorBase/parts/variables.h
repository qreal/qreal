#pragma once

#include <QtCore/QMap>
#include <QtCore/QStringList>

#include <qrrepo/repoApi.h>
#include "generatorBase/templateParametrizedEntity.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace qReal {
namespace robots {
namespace generators {

namespace enums {
namespace variableType {
enum VariableType
{
	unknown = 0
	, floatType
	, intType
};
}
}

namespace parts {
// TODO: make this class customizable in other generators

/// Manages everything about variables and their types
class ROBOTS_GENERATOR_EXPORT Variables : public TemplateParametrizedEntity
{
public:
	explicit Variables(QString const &pathToTemplates);

	/// Tries to infer types for all variables declared in all function blocks
	/// on the specified diagram
	void reinit(qrRepo::RepoApi const &api);

	/// Returns global variables declarations string
	QString generateVariableString() const;

	/// Returns given expression type using inner type inference algorithm
	enums::variableType::VariableType expressionType(QString const &expression) const;

	/// Adds given string to variables declaration section.
	/// This method can be called multiple times with the same string but it
	/// will be deduplicated.
	void appendManualDeclaration(QString const &variables);

protected:
	virtual QMap<QString, enums::variableType::VariableType> nonGenerableReservedVariables() const;
	virtual QMap<QString, int> intConstants() const;
	virtual QMap<QString, float> floatConstants() const;

	virtual QString intConstantDeclaration() const;
	virtual QString floatConstantDeclaration() const;
	virtual QString intVariableDeclaration() const;
	virtual QString floatVariableDeclaration() const;

private:
	QMap<QString, enums::variableType::VariableType> reservedVariables() const;

	void inferTypes(QStringList const &expressions);

	/// Groups given expressions by variable names
	QMap<QString, QStringList> variablesExpressionsMap(QStringList const &expressions) const;

	/// Assigns to the given variable specified type if it is wider than old one
	void assignType(QString const &name, enums::variableType::VariableType type);

	/// Adds to the target list all the variable names participating
	/// in the expression. Returns int or float type if expression has
	/// pre-determined type (for example, contains only int constants or one float)
	/// or unknown else
	enums::variableType::VariableType participatingVariables(QString const &expression
			, QStringList &currentNames) const;

	/// Invokes inference process
	void startDeepInference(QMap<QString, QStringList> &dependencies);
	QStringList dependentFrom(QMap<QString, QStringList> const &dependencies
			, QString const variable) const;
	bool removeDependenciesFrom(QMap<QString, QStringList> &dependencies
			, QString const variable) const;

	bool isIdentifier(QString const &token) const;

	QMap<QString, enums::variableType::VariableType> mVariables;
	QStringList mManualDeclarations;
};

}
}
}
}
