#pragma once

#include "../simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

class NameNormalizerConverter : public simple::Binding::ConverterInterface
{
public:
	virtual QString convert(QString const &data) const;
};

}
}
}
}
