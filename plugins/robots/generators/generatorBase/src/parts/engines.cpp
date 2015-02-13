#include "generatorBase/parts/engines.h"

using namespace generatorBase::parts;
using namespace qReal;

Engines::Engines(const QString &pathToTemplates
		, const simple::Binding::ConverterInterface *outputPortConverter
		, const simple::Binding::MultiConverterInterface *outputPortsConverter)
	: InitTerminateCodeGenerator(pathToTemplates)
	, mOutputPortConverter(outputPortConverter)
	, mOutputPortsConverter(outputPortsConverter)
{
}

Engines::~Engines()
{
	delete mOutputPortConverter;
	delete mOutputPortsConverter;
}

void Engines::reinit()
{
	mUsedPorts.clear();
}

QString Engines::initCode()
{
	return readEngineTemplate("initialization/motor.t");
}

QString Engines::terminateCode()
{
	return readEngineTemplate("termination/motor.t");
}

QString Engines::readEngineTemplate(const QString &pathToTemplate)
{
	QStringList result;
	foreach (const QString &port, mUsedPorts) {
		result << readTemplateIfExists(pathToTemplate).replace("@@PORT@@", port);
	}

	return result.join('\n');
}

void Engines::registerUsageOnPort(const QString &outputPort)
{
	mUsedPorts << mOutputPortConverter->convert(outputPort);
}

void Engines::registerUsageOnPorts(const QString &outputPorts)
{
	foreach (const QString &port, mOutputPortsConverter->convert(outputPorts)) {
		mUsedPorts << port;
	}
}
