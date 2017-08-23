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

/// Inspects random variable init usage and initializes random number generator if needed.
class RandomGeneratorPart : public generatorBase::parts::InitTerminateCodeGenerator
{
public:
	RandomGeneratorPart(const QStringList &pathsToTemplates);

	void reinit() override;
	QString initCode() override;
	QString terminateCode() override;

	/// Marks random number generator as used so it shall be initialized.
	void registerUsage();

	/// Returns current RNG usage status.
	bool isUsed() const;

private:
	/// True, if random number generator is used in a program.
	bool mIsRngUsed = false;
};

}
}
