#pragma once

#include <simpleGenerators/binding.h>

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class PowerMotorPortConverter : public simple::Binding::MultiConverterInterface
{
public:
	QStringList convert(QString const &data) const override;
};

}
}
}
}
