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
	/// Reads the given file contents. A path to file must be relative to templates folder root.
	/// @param pathFromRoot A path to a concrete template relatively to specified in constructor folder.
	QString readTemplate(QString const &pathFromRoot) const;

	/// Reads the given file contents if such exists or returns @arg fallback string otherwise.
	/// Same as readTemplate() but does not display 'file not found' messages in debug output.
	/// A path to file must be relative to templates folder root.
	/// @param pathFromRoot A path to a concrete template relatively to specified in constructor folder.
	QString readTemplateIfExists(QString const &pathFromRoot, QString const &fallback = QString()) const;

	/// Returns a given in constructor path to tempates root.
	QString pathToRoot() const;

private:
	QString mPathToRoot;
};

}
