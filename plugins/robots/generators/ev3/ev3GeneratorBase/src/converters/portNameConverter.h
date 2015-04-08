#pragma once

#include <generatorBase/converters/templateParametrizedConverter.h>

namespace ev3 {
namespace converters {

/// Converts an output port with given id into generator-dependent port representation.
/// If port alias obtained it will be converted into the original port name.
/// Then port name will be tried to convert into the language-specific represenation.
/// For this purpose converter will search for "ports/*.t" template where
/// * is a port name. If such template found its contents will be returned otherwise
/// the result will be raw port name.
class PortNameConverter : public generatorBase::converters::TemplateParametrizedConverter
{
public:
	PortNameConverter(QString const &pathToTemplates);

	QString convert(QString const &portNameOrAlias) const override;
};

}
}

