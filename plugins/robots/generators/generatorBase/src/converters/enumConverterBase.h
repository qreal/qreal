#pragma once

#include "generatorBase/converters/templateParametrizedConverter.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace converters {

/// A base class for converters that have finite input cases number and
/// each of them must be mapped to some template
class ROBOTS_GENERATOR_EXPORT EnumConverterBase : public TemplateParametrizedConverter
{
public:
	EnumConverterBase(QString const &pathToTemplatesDir
			, QMap<QString, QString> const &valuesToTemplatePaths = QMap<QString, QString>());

	virtual QString convert(QString const &data) const;

protected:
	void addMapping(QString const &enumValue, QString const &templatePath);

private:
	QMap<QString, QString> mValuesToTemplates;
};

}
}
