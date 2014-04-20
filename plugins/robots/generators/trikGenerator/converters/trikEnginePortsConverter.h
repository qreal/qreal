#pragma once

#include <generatorBase/converters/templateParametrizedMultiConverter.h>

namespace trik {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class TrikEnginePortsConverter : public generatorBase::converters::TemplateParametrizedMultiConverter
{
public:
	explicit TrikEnginePortsConverter(QString const &pathToTemplates);

	QStringList convert(QString const &data) const override;
};

}
}
