#include "engines.h"

using namespace qReal::robots::generators::parts;

Engines::Engines(QString const &pathToTemplates
		, simple::Binding::ConverterInterface const *outputPortConverter
		, simple::Binding::MultiConverterInterface const *outputPortsConverter)
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
	return readEngineTemplate("initialization/engines.t");
}

QString Engines::terminateCode()
{
	return readEngineTemplate("termination/engines.t");
}

QString Engines::readEngineTemplate(QString const &pathToTemplate)
{
	QStringList result;
	foreach (QString const &port, mUsedPorts) {
		result << readTemplate(pathToTemplate).replace("@@PORT@@", port);
	}

	return result.join('\n');
}

void Engines::registerUsageOnPort(QString const &outputPort)
{
	mUsedPorts << mOutputPortConverter->convert(outputPort);
}

void Engines::registerUsageOnPorts(QString const &outputPorts)
{
	foreach (QString const &port, mOutputPortsConverter->convert(outputPorts)) {
		mUsedPorts << port;
	}
}
