/* Copyright 2017 QReal Research Group
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

#include <generatorBase/parts/initTerminateCodeGenerator.h>

namespace pioneer {
namespace lua {

/// Inspects magnet usage and initializes magnet GPIO if needed.
class MagnetPart : public generatorBase::parts::InitTerminateCodeGenerator
{
public:
	MagnetPart(const QStringList &pathsToTemplates);

	void reinit() override;
	QString initCode() override;
	QString terminateCode() override;

	/// Marks magnet as used so it shall be initialized.
	void registerUsage();

private:
	/// True, if magnet is used in a program.
	bool mIsUsed = false;
};

}
}
