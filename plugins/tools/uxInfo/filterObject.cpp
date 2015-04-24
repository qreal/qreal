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

#include "filterObject.h"

#include <QtCore/QEvent>
#include <QtWidgets/QAction>

#include "uxInfo.h"

using namespace uxInfo;

FilterObject::FilterObject(QObject *parent)
	: QObject(parent)
{
}

bool FilterObject::eventFilter(QObject *object, QEvent *event)
{
	Q_UNUSED(object);
	Q_UNUSED(event);
	return false;
}

void FilterObject::triggeredActionActivated()
{
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		UXInfo::reportMenuElements(action->text());
	}
}

void FilterObject::toggledActionActivated(bool status)
{
	QString const statusText = status ? "checked" : "unchecked";
	QAction* const action = static_cast<QAction* const>(sender());
	if (action) {
		QString const statusForUStatistics = "status change on: " + statusText;
		UXInfo::reportMenuElements(action->text(), statusForUStatistics);
	}
}

void FilterObject::setStatusCollectUsabilityStatistics(bool status)
{
	UXInfo::setStatus(status);
}

void FilterObject::reportTestStarted()
{
	UXInfo::reportTestStarted();
}

void FilterObject::reportTestFinished()
{
	UXInfo::reportTestFinished();
}
