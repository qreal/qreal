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
	Variables(const QString &pathToTemplates
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
	QString constantDeclaration(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const;
	QString variableDeclaration(const QSharedPointer<qrtext::core::types::TypeExpression> &type) const;

private:
	const kitBase::robotModel::RobotModelInterface &mRobotModel;
	qrtext::LanguageToolboxInterface &mLuaToolbox;
	QStringList mManualDeclarations;
};

}
}
