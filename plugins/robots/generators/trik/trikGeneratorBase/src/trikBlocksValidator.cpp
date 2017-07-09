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

#include "trikGeneratorBase/trikBlocksValidator.h"

using namespace trik;

TrikBlocksValidator::TrikBlocksValidator(const qrRepo::RepoApi &repo
		, generatorBase::GeneratorCustomizer &customizer
		, qReal::ErrorReporterInterface &errorReporter)
	: RobotsDiagramVisitor(repo, customizer)
	, mRepo(repo)
	, mErrorReporter(errorReporter)
{
}

bool TrikBlocksValidator::validate(const qReal::Id &startNode)
{
	mNoErrors = true;
	startSearch(startNode);
	return mNoErrors;
}

void TrikBlocksValidator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void TrikBlocksValidator::visitFinal(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void TrikBlocksValidator::visitConditional(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void TrikBlocksValidator::visitLoop(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void TrikBlocksValidator::visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void TrikBlocksValidator::visitUnknown(const qReal::Id &id, const QList<LinkInfo> &links)
{
	visitGeneral(id, links);
}

void TrikBlocksValidator::visitFork(const qReal::Id &id, QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void TrikBlocksValidator::visitJoin(const qReal::Id &id, QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void TrikBlocksValidator::visitGeneral(const qReal::Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	QSet<QString> blocksWithPossiblyEmptyProperties{
		"CommentBlock"
	};

	if (!blocksWithPossiblyEmptyProperties.contains(id.element())) {
		checkStringPropertiesNotEmpty(id);
	}
}

void TrikBlocksValidator::checkStringPropertiesNotEmpty(const qReal::Id &id)
{
	const auto properties = mRepo.properties(id);
	for (auto property : properties.keys()) {
		if (properties[property].type() == QVariant::String &&  properties[property].toString().isEmpty()) {
			error(QObject::tr("Property should not be empty."), id);
		}
	}
}

void TrikBlocksValidator::error(const QString &message, const qReal::Id &id)
{
	mErrorReporter.addError(message, id);
	mNoErrors = false;
}
