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

#include <generatorBase/primaryControlFlowValidator.h>

namespace trik {
namespace qts {

/// Ensures that a diagram may be correctly generated into Qt Script code.
class TrikQtsControlFlowValidator : public generatorBase::PrimaryControlFlowValidator
{
public:
	TrikQtsControlFlowValidator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, generatorBase::GeneratorCustomizer &customizer
			, QObject *parent = 0);

	bool validate(const qReal::Id &diagramId, const QString &threadId) override;

	PrimaryControlFlowValidator *clone() override;
};

}
}
