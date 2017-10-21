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

#include <generatorBase/robotsDiagramVisitor.h>

#include "trikGeneratorBase/trikGeneratorBaseDeclSpec.h"

typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

namespace trik {

/// Checks TRIK blocks on a diagram for some common errors.
class ROBOTS_TRIK_GENERATOR_BASE_EXPORT TrikBlocksValidator : public generatorBase::RobotsDiagramVisitor
{
public:
	TrikBlocksValidator(const qrRepo::RepoApi &repo
			, generatorBase::GeneratorCustomizer &customizer
			, qReal::ErrorReporterInterface &errorReporter);

	bool validate(const qReal::Id &startNode);

private:
	void visitRegular(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFinal(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitConditional(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitLoop(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFork(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitJoin(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitUnknown(const qReal::Id &id, const QList<LinkInfo> &links) override;

	void visitGeneral(const qReal::Id &id, const QList<LinkInfo> &links);

	void checkStringPropertiesNotEmpty(const qReal::Id &id);

	void error(const QString &message, const qReal::Id &id);

	const qrRepo::RepoApi &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	bool mNoErrors = true;
};

}
