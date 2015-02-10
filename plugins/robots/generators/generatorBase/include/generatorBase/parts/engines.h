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
	Engines(const QString &pathToTemplates
			, const simple::Binding::ConverterInterface *outputPortConverter
			, const simple::Binding::MultiConverterInterface *outputPortsConverter);
	virtual ~Engines();

	virtual void reinit();
	virtual QString initCode();
	virtual QString terminateCode();

	/// Marks given output port used in resulting program. Adds init/termnate code for it.
	void registerUsageOnPort(const QString &outputPort);

	/// Marks given output ports used in resulting program. Adds init/termnate code for them.
	/// Output ports are splitted with system engines multiconverter.
	void registerUsageOnPorts(const QString &outputPorts);

private:
	QString readEngineTemplate(const QString &pathToTemplate);

	QSet<QString> mUsedPorts;
	const simple::Binding::ConverterInterface *mOutputPortConverter;  // Takes ownership
	const simple::Binding::MultiConverterInterface *mOutputPortsConverter;  // Takes ownership
};

}
}
