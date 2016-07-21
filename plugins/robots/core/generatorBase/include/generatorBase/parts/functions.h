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

namespace generatorBase {
namespace parts {

/// Inspects function blocks with initialization marker and generates code
/// for initialization section.
class Functions : public InitTerminateCodeGenerator
{
public:
	explicit Functions(const QStringList &pathsToTemplates);

	void reinit() override;
	QString initCode() override;

	/// Adds given function block body into resulting initialization code.
	void registerFunctionInInitialization(const QString &body);

private:
	QStringList mFunctions;
};

}
}
