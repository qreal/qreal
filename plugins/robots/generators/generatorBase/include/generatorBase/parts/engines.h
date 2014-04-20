#pragma once

#include <QtCore/QSet>

#include "initTerminateCodeGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"
#include "generatorBase/robotsGeneratorDeclSpec.h"

namespace generatorBase {
namespace parts {

/// Inspects engines usage and generates code for their initialization and termination.
class ROBOTS_GENERATOR_EXPORT Engines : public InitTerminateCodeGenerator
{
public:
	Engines(QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *outputPortConverter
			, simple::Binding::MultiConverterInterface const *outputPortsConverter);
	virtual ~Engines();

	virtual void reinit();
	virtual QString initCode();
	virtual QString terminateCode();

	/// Marks given output port used in resulting program. Adds init/termnate code for it.
	void registerUsageOnPort(QString const &outputPort);

	/// Marks given output ports used in resulting program. Adds init/termnate code for them.
	/// Output ports are splitted with system engines multiconverter.
	void registerUsageOnPorts(QString const &outputPorts);

private:
	QString readEngineTemplate(QString const &pathToTemplate);

	QSet<QString> mUsedPorts;
	simple::Binding::ConverterInterface const *mOutputPortConverter;  // Takes ownership
	simple::Binding::MultiConverterInterface const *mOutputPortsConverter;  // Takes ownership
};

}
}
