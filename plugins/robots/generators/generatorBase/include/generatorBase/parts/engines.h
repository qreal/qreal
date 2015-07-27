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
	Engines(const QStringList &pathsToTemplates
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
