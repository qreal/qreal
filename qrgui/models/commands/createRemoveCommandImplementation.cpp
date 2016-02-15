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

#include "createRemoveCommandImplementation.h"

#include <QtCore/QTimer>

#include "models/models.h"

using namespace qReal;
using namespace commands;

CreateRemoveCommandImplementation::CreateRemoveCommandImplementation(const models::Models &models
		, const QList<ElementInfo> &elements)
	: mLogicalApi(models.logicalModelAssistApi())
	, mGraphicalApi(models.graphicalModelAssistApi())
	, mExploser(models.exploser())
	, mElements(elements)
{
}

void CreateRemoveCommandImplementation::create()
{
	// Creating elements...
	mGraphicalApi.createElements(mElements);

	// ... and restoring their properties.
	for (const ElementInfo &element : mElements) {
		// Graphical properties were already setted in GraphicalModel.
		// Now for graphical elements logical properties should be setted.
		const Id logicalId = mGraphicalApi.logicalId(element.id());
		if (mLogicalApi.logicalRepoApi().exist(logicalId) && element.id() != logicalId) {
			for (const QString &property : element.logicalProperties()) {
				mLogicalApi.setPropertyByRoleName(logicalId, element.logicalProperty(property), property);
			}
		}
	}

	refreshAllPalettes();
}

void CreateRemoveCommandImplementation::remove()
{
	for (int i = mElements.size() - 1; i >= 0; --i) {
		removeOne(mElements[i]);
	}

	refreshAllPalettes();
}

void CreateRemoveCommandImplementation::setElements(const QList<ElementInfo> &elements)
{
	mElements = elements;
}

const QList<ElementInfo> &CreateRemoveCommandImplementation::results() const
{
	return mElements;
}

void CreateRemoveCommandImplementation::removeOne(ElementInfo &element)
{
	if (element.id() == element.logicalId()) {
		mLogicalApi.removeReferencesTo(element.id());
		mLogicalApi.removeReferencesFrom(element.id());
		mLogicalApi.removeElement(element.id());
		mGraphicalApi.removeElement(element.id());
	} else {
		element.setAllGraphicalProperties(mGraphicalApi.properties(element.id()));
		if (!mLogicalApi.logicalRepoApi().exist(element.logicalId())) {
			mGraphicalApi.removeElement(element.id());
			return;
		}

		element.setAllLogicalProperties(mGraphicalApi.properties(element.logicalId()));
		const IdList graphicalIds = mGraphicalApi.graphicalIdsByLogicalId(element.logicalId());
		mGraphicalApi.removeElement(element.id());
		// Checking that the only graphical part is our element itself
		// (bijection between graphical and logical parts)
		if (graphicalIds.count() == 1 && graphicalIds[0] == element.id()) {
			mLogicalApi.removeReferencesTo(element.logicalId());
			mLogicalApi.removeReferencesFrom(element.logicalId());
			mLogicalApi.removeElement(element.logicalId());
		}
	}
}

void CreateRemoveCommandImplementation::setNewPosition(const QPointF &position)
{
	mElements.first().setPos(position);
}

void CreateRemoveCommandImplementation::refreshAllPalettes()
{
	// Calling refreshing immediately may cause segfault because of deleting drag source
	QTimer::singleShot(0, &mExploser, SIGNAL(explosionsSetCouldChange()));
}
