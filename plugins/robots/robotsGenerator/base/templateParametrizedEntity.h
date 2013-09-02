#pragma once

#include <QtCore/QString>

namespace qReal {
namespace robots {
namespace generators {

/// This class can be inherited by those entities who need to use generator templates
class TemplateParametrizedEntity
{
public:
	/// @param pathToTemplates A path to a folder containing all concrete generator templates
	explicit TemplateParametrizedEntity(QString const &pathToTemplates);
	virtual ~TemplateParametrizedEntity();

protected:
	/// @param pathFromRoot A path to a concrete template relatively to specified in
	/// constructor folder
	QString readTemplate(QString const &pathFromRoot) const;

private:
	QString const mPathToRoot;
};

}
}
}
