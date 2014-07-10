#pragma once

#include <generatorBase/simpleGenerators/binding.h>

namespace trik {
namespace converters {

class PowerV6MotorPortConverter : public generatorBase::simple::Binding::ConverterInterface
{
public:
	static QString splitRegexp();

	QString convert(QString const &data) const override;
};

}
}
