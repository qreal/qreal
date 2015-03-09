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
	EnumConverterBase(const QString &pathToTemplatesDir
			, QMap<QString, QString> const &valuesToTemplatePaths = QMap<QString, QString>());

	QString convert(const QString &data) const override;

protected:
	void addMapping(const QString &enumValue, const QString &templatePath);

private:
	QMap<QString, QString> mValuesToTemplates;
};

}
}
