#pragma once

#include "simpleGenerators/binding.h"
#include "robotsGeneratorDeclSpec.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Replaces all inappropriate symbols in input string for valid usage in
/// code as string literal (for example replaces quotes into escaped ones)
class ROBOTS_GENERATOR_EXPORT StringPropertyConverter : public simple::Binding::ConverterInterface
{
public:
	virtual QString convert(QString const &data) const;
};

}
}
}
}
