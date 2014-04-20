#pragma once

#include <QtCore/QString>

#include "robotsGeneratorDeclSpec.h"

namespace generatorBase {

/// This class can be inherited by those entities who need to use generator templates
class ROBOTS_GENERATOR_EXPORT TemplateParametrizedEntity
{
public:
	TemplateParametrizedEntity();

	/// @param pathToTemplates A path to a folder containing all concrete generator templates
	explicit TemplateParametrizedEntity(QString const &pathToTemplates);

	virtual ~TemplateParametrizedEntity();

	/// Resets a path to a folder containing all concrete generator templates
	void setPathToTemplates(QString const &pathTemplates);

protected:
	/// @param pathFromRoot A path to a concrete template relatively to specified in
	/// constructor folder
	QString readTemplate(QString const &pathFromRoot) const;

private:
	QString mPathToRoot;
};

}
