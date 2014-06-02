#pragma once

#include "templateParametrizedConverter.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Converts wait block comparison property into generator-denendent inequality
/// infix operator.
class InequalitySignConverter : public TemplateParametrizedConverter
{
public:
	explicit InequalitySignConverter(QString const &pathToTemplates);

	QString convert(QString const &data) const override;
};

}
}
}
}
