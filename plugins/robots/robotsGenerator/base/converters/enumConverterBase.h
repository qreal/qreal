#pragma once

#include <QtCore/QMap>

#include "templateParametrizedConverter.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

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
}
}
