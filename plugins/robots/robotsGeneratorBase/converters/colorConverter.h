#pragma once

#include "templateParametrizedConverter.h"

namespace qReal {
namespace robots {
namespace generators {
namespace converters {

/// Converts 'Color' enum in WaitForColor blocks into generator-specific code.
/// Resulting code must be specified in templates from "colors" folder.
class ColorConverter : public TemplateParametrizedConverter
{
public:
	explicit ColorConverter(QString const &pathToTemplates);

	QString convert(QString const &data) const override;
};

}
}
}
}
