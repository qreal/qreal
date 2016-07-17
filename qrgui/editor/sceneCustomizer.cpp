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

#include "sceneCustomizer.h"

#include <QtCore/QObject>

using namespace qReal;
using namespace qReal::gui::editor;

SceneCustomizer::SceneCustomizer()
	: mAddExplosionMenuName(QObject::tr("Add connection"))
	, mChangeExplosionMenuName(QObject::tr("Connect to other"))
	, mDeleteExplosionMenuName(QObject::tr("Disconnect"))
	, mGoToConnectedMenuName(QObject::tr("Go to connected element"))
	, mShowExpandConnectionAction(false)
	, mExpandExplosionActionText(QObject::tr("Expand explosion"))
	, mCollapseExplosionActionText(QObject::tr("Collapse explosion"))
{
}

QString SceneCustomizer::addExplosionMenuName() const
{
	return mAddExplosionMenuName;
}

QString SceneCustomizer::changeExplosionMenuName() const
{
	return mChangeExplosionMenuName;
}

QString SceneCustomizer::deleteExplosionMenuName() const
{
	return mDeleteExplosionMenuName;
}

QString SceneCustomizer::goToConnectedMenuName() const
{
	return mGoToConnectedMenuName;
}

bool SceneCustomizer::showExpandConnectionAction() const
{
	return mShowExpandConnectionAction;
}

QString SceneCustomizer::expandExplosionActionText() const
{
	return mExpandExplosionActionText;
}

QString SceneCustomizer::collapseExplosionActionText() const
{
	return mCollapseExplosionActionText;
}

void SceneCustomizer::setAddExplosionMenuName(const QString &text)
{
	mAddExplosionMenuName = text;
}

void SceneCustomizer::setChangeExplosionMenuName(const QString &text)
{
	mChangeExplosionMenuName = text;
}

void SceneCustomizer::setDeleteExplosionMenuName(const QString &text)
{
	mDeleteExplosionMenuName = text;
}

void SceneCustomizer::setGoToConnectedMenuName(const QString &text)
{
	mGoToConnectedMenuName = text;
}

void SceneCustomizer::setShowExpandConnectionAction(bool show)
{
	mShowExpandConnectionAction = show;
}

void SceneCustomizer::setExpandExplosionActionText(const QString &text)
{
	mExpandExplosionActionText = text;
}

void SceneCustomizer::setCollapseExplosionActionText(const QString &text)
{
	mCollapseExplosionActionText = text;
}
