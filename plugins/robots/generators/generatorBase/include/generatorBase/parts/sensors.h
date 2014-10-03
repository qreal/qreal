#pragma once

#include <interpreterBase/robotModel/portInfo.h>
#include <interpreterBase/robotModel/deviceInfo.h>

#include "initTerminateCodeGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {
namespace parts {

/// Inspects sensors usage and generates code for their initialization and termination.
class Sensors : public InitTerminateCodeGenerator
{
public:
	Sensors(QString const &pathToTemplates, simple::Binding::ConverterInterface const *inputPortConverter);
	virtual ~Sensors();

	virtual void reinit(QMap<interpreterBase::robotModel::PortInfo
			, interpreterBase::robotModel::DeviceInfo> const &devices);
	virtual QString initCode();
	virtual QString terminateCode();
	virtual QString isrHooksCode();

protected:
	void reinitPort(interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &device);

	virtual QString code(QString const &directory
			, interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &device);

private:
	simple::Binding::ConverterInterface const *mInputPortConverter;  // Takes ownership
	QStringList mInitCode;
	QStringList mTerminateCode;
	QStringList mIsrHooksCode;
};

}
}
