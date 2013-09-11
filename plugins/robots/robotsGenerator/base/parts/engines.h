#pragma once

#include <QtCore/QSet>

#include "initTerminateCodeGenerator.h"
#include "../simpleGenerators/binding.h"

namespace qReal {
namespace robots {
namespace generators {
namespace parts {

class Engines : public InitTerminateCodeGenerator
{
public:
	Engines(QString const &pathToTemplates
			, simple::Binding::ConverterInterface const *outputPortConverter
			, simple::Binding::MultiConverterInterface const *outputPortsConverter);
	virtual ~Engines();

	virtual void reinit();
	virtual QString initCode();
	virtual QString terminateCode();

	void registerUsageOnPort(QString const &outputPort);
	void registerUsageOnPorts(QString const &outputPorts);

private:
	QString readEngineTemplate(QString const &pathToTemplate);

	QSet<QString> mUsedPorts;
	simple::Binding::ConverterInterface const *mOutputPortConverter;  // Takes ownership
	simple::Binding::MultiConverterInterface const *mOutputPortsConverter;  // Takes ownership
};

}
}
}
}
