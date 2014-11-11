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
	QString typeExpression(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const;
	QString constantDeclaration(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const;
	QString variableDeclaration(QSharedPointer<qrtext::core::types::TypeExpression> const &type) const;

private:
	interpreterBase::robotModel::RobotModelInterface const &mRobotModel;
	qrtext::LanguageToolboxInterface &mLuaToolbox;
	QStringList mManualDeclarations;
};

}
}
