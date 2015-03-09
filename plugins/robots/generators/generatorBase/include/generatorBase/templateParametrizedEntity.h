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
	explicit TemplateParametrizedEntity(const QString &pathToTemplates);

	virtual ~TemplateParametrizedEntity();

	/// Resets a path to a folder containing all concrete generator templates
	void setPathToTemplates(const QString &pathTemplates);

protected:
	/// Reads the given file contents. A path to file must be relative to templates folder root.
	/// @param pathFromRoot A path to a concrete template relatively to specified in constructor folder.
	QString readTemplate(const QString &pathFromRoot) const;

	/// Reads the given file contents if such exists or returns @arg fallback string otherwise.
	/// Same as readTemplate() but does not display 'file not found' messages in debug output.
	/// A path to file must be relative to templates folder root.
	/// @param pathFromRoot A path to a concrete template relatively to specified in constructor folder.
	QString readTemplateIfExists(const QString &pathFromRoot, const QString &fallback = QString()) const;

	/// Returns a given in constructor path to tempates root.
	QString pathToRoot() const;

private:
	QString mPathToRoot;
};

}
