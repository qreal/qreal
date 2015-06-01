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

#include "trikQtsControlFlowValidator.h"

#include "threadsValidator.h"

using namespace generatorBase;
using namespace trik::qts;

TrikQtsControlFlowValidator::TrikQtsControlFlowValidator(const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, QObject *parent)
	: PrimaryControlFlowValidator(repo, errorReporter, customizer, parent)
{
}

bool TrikQtsControlFlowValidator::validate(const qReal::Id &diagramId, const QString &threadId)
{
	mDiagram = diagramId;
	findInitialNode();
	if (mInitialNode.isNull()) {
		error(QObject::tr("There is nothing to generate, diagram doesn't have Initial Node"), mDiagram);
		return false;
	}

	ThreadsValidator threadsValidator(mRepo, mCustomizer, mErrorReporter);
	return threadsValidator.validate(mInitialNode, threadId)
			&& PrimaryControlFlowValidator::validate(diagramId, threadId);
}

PrimaryControlFlowValidator *TrikQtsControlFlowValidator::clone()
{
	return new TrikQtsControlFlowValidator(mRepo, mErrorReporter, mCustomizer, parent());
}
