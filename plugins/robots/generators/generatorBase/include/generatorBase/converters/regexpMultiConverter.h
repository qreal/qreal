#pragma once

#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace converters {

/// A multiconverter that splits the given input by the given regexp and applies given simple conveter to each part
class ROBOTS_GENERATOR_EXPORT RegexpMultiConverter : public simple::Binding::MultiConverterInterface
{
public:
	RegexpMultiConverter(const QString &splitRegexp, const simple::Binding::ConverterInterface * const converter);
	~RegexpMultiConverter() override;

	QStringList convert(const QString &data) const override;

private:
	const QString mSplitRegexp;
	const simple::Binding::ConverterInterface * const mSimpleConverter;
};

}
}
