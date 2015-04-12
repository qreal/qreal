#pragma once

#include <kitBase/robotModel/portInfo.h>
#include <kitBase/robotModel/deviceInfo.h>

#include "initTerminateCodeGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {
namespace parts {

/// Inspects sensors usage and generates code for their initialization and termination.
class Sensors : public InitTerminateCodeGenerator
{
public:
	Sensors(const QString &pathToTemplates, const simple::Binding::ConverterInterface *inputPortConverter);
	virtual ~Sensors();

	virtual void reinit(const QMap<kitBase::robotModel::PortInfo
			, kitBase::robotModel::DeviceInfo> &devices);

	virtual QString initCode();
	virtual QString terminateCode();
	virtual QString isrHooksCode();

protected:
	void reinitPort(const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &device);

	virtual QString code(const QString &directory
			, const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &device);

private:
	const simple::Binding::ConverterInterface *mInputPortConverter;  // Takes ownership
	QStringList mInitCode;
	QStringList mTerminateCode;
	QStringList mIsrHooksCode;
};

}
}
