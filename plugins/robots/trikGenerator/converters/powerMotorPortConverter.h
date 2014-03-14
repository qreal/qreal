#pragma once

#include <simpleGenerators/binding.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class PowerMotorPortConverter : public simple::Binding::ConverterInterface
{
public:
	static QString splitRegexp();

	QString convert(QString const &data) const override;
};

}
}
}
}
