#pragma once

#include <generatorBase/simpleGenerators/binding.h>

namespace trik {
namespace converters {

class EncoderV6PortConverter : public generatorBase::simple::Binding::ConverterInterface
{
public:
	QString convert(QString const &data) const override;
};

}
}
