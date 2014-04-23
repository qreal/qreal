#pragma once

#include <generatorBase/simpleGenerators/binding.h>

namespace trik {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class ServoMotorPortConverter : public generatorBase::simple::Binding::ConverterInterface
{
public:
	static QString splitRegexp();

	QString convert(QString const &data) const override;
};

}
}
