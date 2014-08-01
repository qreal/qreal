#pragma once

#include <generatorBase/simpleGenerators/binding.h>

namespace fSharp {
namespace converters {

class EncoderV4PortConverter : public generatorBase::simple::Binding::ConverterInterface
{
public:
	QString convert(QString const &data) const override;
};

}
}
