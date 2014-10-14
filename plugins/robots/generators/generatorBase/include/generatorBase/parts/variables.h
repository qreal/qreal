#pragma once

#include <QtCore/QMap>
#include <QtCore/QStringList>

#include <qrtext/core/types/typeExpression.h>
#include <qrrepo/repoApi.h>
#include <interpreterBase/robotModel/robotModelInterface.h>

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
	Variables(QString const &pathToTemplates
			, interpreterBase::robotModel::RobotModelInterface const &robotModel
			, qrtext::LanguageToolboxInterface &luaToolbox);

	/// Returns global variables declarations string
	QString generateVariableString() const;

	/// Returns given expression type using inner type inference algorithm
	QSharedPointer<qrtext::core::types::TypeExpression> expressionType(QString const &expression) const;

	/// Adds given string to variables declaration section.
	/// This method can be called multiple times with the same string but it
	/// will be deduplicated.
	void appendManualDeclaration(QString const &variables);

protected:
	virtual QMap<QString, qrtext::core::types::TypeExpression> nonGenerableReservedVariables() const;
	virtual QMap<QString, int> intConstants() const;
	virtual QMap<QString, float> floatConstants() const;

	virtual QString typeExpression(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const;
	virtual QString constantDeclaration(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const;
	virtual QString variableDeclaration(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const;

	/// Returns a list of variable initialization expressions in the model.
	virtual QStringList expressions(qrRepo::RepoApi const &api) const;

private:
	QMap<QString, qrtext::core::types::TypeExpression> reservedVariables() const;

	interpreterBase::robotModel::RobotModelInterface const &mRobotModel;
	qrtext::LanguageToolboxInterface &mLuaToolbox;
	QStringList mManualDeclarations;
};

}
}
