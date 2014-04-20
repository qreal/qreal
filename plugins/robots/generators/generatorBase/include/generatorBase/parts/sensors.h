#pragma once

#include "initTerminateCodeGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"
//#include "plugins/robots/robotsInterpreter/sensorConstants.h"

namespace generatorBase {
namespace parts {

/// Inspects sensors usage and generates code for their initialization and termination.
class Sensors : public InitTerminateCodeGenerator
{
public:
	Sensors(QString const &pathToTemplates, simple::Binding::ConverterInterface const *inputPortConverter);
	virtual ~Sensors();

	virtual void reinit();
	virtual QString initCode();
	virtual QString terminateCode();
	virtual QString isrHooksCode();

protected:
	void reinitPort(int port);

//	virtual QString initCode(QString const &port
//			, interpreters::robots::enums::sensorType::SensorTypeEnum type);

//	virtual QString terminateCode(QString const &port
//			, interpreters::robots::enums::sensorType::SensorTypeEnum type);

//	virtual QString isrHooks(QString const &port
//			, interpreters::robots::enums::sensorType::SensorTypeEnum type);

private:
	simple::Binding::ConverterInterface const *mInputPortConverter;  // Takes ownership
	QStringList mInitCode;
	QStringList mTerminateCode;
	QStringList mIsrHooksCode;
};

}
}
