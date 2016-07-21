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

#include "generatorBase/templateParametrizedEntity.h"

namespace generatorBase {
namespace parts {

/// A base for entity that inspects some robot part usage and generates code
/// for its correct initialization, deinitialization and work
class InitTerminateCodeGenerator : public TemplateParametrizedEntity
{
public:
	explicit InitTerminateCodeGenerator(const QStringList &pathsToTemplates);
	virtual ~InitTerminateCodeGenerator();

	/// Called each time before new generation process.
	/// Default implementation does nothing.
	virtual void reinit();

	/// Override must return code for initialization some robot part.
	/// Default implementation returns empty string.
	virtual QString initCode();

	/// Override must return code for deinitialization some robot part.
	/// Default implementation returns empty string.
	virtual QString terminateCode();

	/// Override must return code that will be invoked 1 time per 1 ms.
	/// Default implementation returns empty string.
	virtual QString isrHooksCode();
};

}
}
