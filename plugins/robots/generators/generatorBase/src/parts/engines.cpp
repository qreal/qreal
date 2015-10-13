/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "generatorBase/parts/engines.h"

using namespace generatorBase::parts;
using namespace qReal;

Engines::Engines(const QStringList &pathsToTemplates
		, const simple::Binding::ConverterInterface *outputPortConverter
		, const simple::Binding::MultiConverterInterface *outputPortsConverter)
	: InitTerminateCodeGenerator(pathsToTemplates)
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
