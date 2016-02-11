/* Copyright 2013-2016 Dmitry Mordvinov
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

#include "createElementsCommand.h"

#include "models/models.h"

using namespace qReal::commands;

CreateElementsCommand::CreateElementsCommand(const models::Models &models, const QList<ElementInfo> &elements)
	: mModels(models)
	, mLogicalApi(models.logicalModelAssistApi())
	, mGraphicalApi(models.graphicalModelAssistApi())
	, mExploser(models.exploser())
	, mImpl(models, elements)
{
	addExplosionCommands(elements);
}

const QList<qReal::ElementInfo> &CreateElementsCommand::results() const
{
	return mImpl.results();
}

void CreateElementsCommand::setElements(const QList<ElementInfo> &elements)
{
	mImpl.setElements(elements);
	addExplosionCommands(elements);
}

void CreateElementsCommand::addExplosionCommands(const QList<qReal::ElementInfo> &elements)
{
	for (const ElementInfo &element : elements) {
		if (element.explosionTarget().isNull() || mGraphicalApi.graphicalRepoApi().exist(element.explosionTarget())) {
			mExploser.handleCreationWithExplosion(this, mModels, element.id(), element.explosionTarget());
		}
	}
}

void CreateElementsCommand::setNewPosition(const QPointF &position)
{
	mImpl.setNewPosition(position);
}

bool CreateElementsCommand::execute()
{
	mImpl.create();
	return true;
}

bool CreateElementsCommand::restoreState()
{
	mImpl.remove();
	return true;
}
