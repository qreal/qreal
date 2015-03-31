#pragma once

#include <kitBase/robotModel/portInfo.h>

#include "generatorBase/converters/templateParametrizedConverter.h"

namespace generatorBase {
namespace converters {

/// Converts an output port with given id into generator-dependent port representation.
/// If port alias obtained it will be converted into the original port name.
/// Then port name will be tried to convert into the language-specific represenation.
/// For this purpose converter will search for "ports/*.t" template where
/// * is a port name. If such template found its contents will be returned otherwise
/// the result will be raw port name.
class PortNameConverter : public TemplateParametrizedConverter
{
public:
	PortNameConverter(const QString &pathToTemplates
			, QList<kitBase::robotModel::PortInfo> const &ports);

	QString convert(const QString &portNameOrAlias) const override;

private:
	QList<kitBase::robotModel::PortInfo> const mPorts;
};

}
}
