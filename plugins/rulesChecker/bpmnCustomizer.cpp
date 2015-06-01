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

#include "bpmnCustomizer.h"

bool BpmnCustomizer::showInterpeterButton() const
{
	return false;
}

QString BpmnCustomizer::windowTitle() const
{
	return QObject::tr("QReal:BP") + " " + productVersion();
}

QString BpmnCustomizer::productVersion() const
{
	return "0.4.0";
}

bool BpmnCustomizer::showConnectionRelatedMenus() const
{
	return true;
}

QString BpmnCustomizer::addExplosionMenuName() const
{
	return QObject::tr("Set subprocess");
}

QString BpmnCustomizer::changeExplosionMenuName() const
{
	return QObject::tr("Change subprocess");
}

QString BpmnCustomizer::deleteExplosionMenuName() const
{
	return QObject::tr("Remove subprocess");
}

QString BpmnCustomizer::goToConnectedMenuName() const
{
	return QObject::tr("Edit subprocess");
}

bool BpmnCustomizer::showExpandConnectionAction() const
{
	return true;
}

QString BpmnCustomizer::expandExplosionActionText() const
{
	return QObject::tr("Expand subprocess");
}

QString BpmnCustomizer::collapseExplosionActionText() const
{
	return QObject::tr("Collapse subprocess");
}
