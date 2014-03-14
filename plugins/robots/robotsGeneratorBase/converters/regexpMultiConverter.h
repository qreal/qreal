#pragma once

#include "simpleGenerators/binding.h"
#include "robotsGeneratorDeclSpec.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// A multiconverter that splits the given input by the given regexp and applies given simple conveter to each part
class ROBOTS_GENERATOR_EXPORT RegexpMultiConverter : public simple::Binding::MultiConverterInterface
{
public:
	RegexpMultiConverter(QString const &splitRegexp, simple::Binding::ConverterInterface const * const converter);
	~RegexpMultiConverter() override;

	QStringList convert(QString const &data) const override;

private:
	QString const mSplitRegexp;
	simple::Binding::ConverterInterface const * const mSimpleConverter;
};

}
}
}
}
