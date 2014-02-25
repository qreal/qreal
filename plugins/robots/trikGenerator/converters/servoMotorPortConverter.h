#pragma once

#include <simpleGenerators/binding.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Splits given string selecting just numbers and returns converted output port values
class ServoMotorPortConverter : public simple::Binding::MultiConverterInterface
{
public:
	QStringList convert(QString const &data) const override;
};

}
}
}
}
