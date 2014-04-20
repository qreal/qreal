#pragma once

#include <QtCore/QMap>

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {
namespace converters {

/// A base class for converters that have finite input cases number and
/// each of them must be mapped to some template
class EnumConverterBase : public TemplateParametrizedConverter
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
