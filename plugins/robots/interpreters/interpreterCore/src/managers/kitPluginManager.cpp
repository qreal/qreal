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

#include "interpreterCore/managers/kitPluginManager.h"

#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>

#include <qrkernel/platformInfo.h>
#include <qrkernel/exception/exception.h>

using namespace interpreterCore;
using namespace qReal;

KitPluginManager::KitPluginManager(const QString &pluginsDirectory)
	: mPluginManager(pluginsDirectory)
{
	tryToLoadKitPlugins();
}

KitPluginManager::~KitPluginManager()
{
}

QList<QString> KitPluginManager::kitIds() const
{
	return mPluginInterfaces.uniqueKeys();
}

QList<kitBase::KitPluginInterface *> KitPluginManager::kitsById(const QString &kitId) const
{
	if (!mPluginInterfaces.contains(kitId)) {
		throw qReal::Exception("Requesting non-existing kit plugin");
	}

	return mPluginInterfaces.values(kitId);
}

QList<kitBase::robotModel::RobotModelInterface *> KitPluginManager::allRobotModels() const
{
	QList<kitBase::robotModel::RobotModelInterface *> result;
	for (kitBase::KitPluginInterface * const kit : mPluginInterfaces) {
		result += kit->robotModels();
	}

	return result;
}

int KitPluginManager::priority(const QString &kitId) const
{
	int result = 0;
	for (kitBase::KitPluginInterface * const kit : kitsById(kitId)) {
		result = qMax(result, kit->priority());
	}

	return result;
}

void KitPluginManager::tryToLoadKitPlugins()
{
	QList<kitBase::KitPluginInterface *> const loadedInterpreterPlugins =
			mPluginManager.loadAllPlugins<kitBase::KitPluginInterface>();

	for (kitBase::KitPluginInterface * const kitPlugin : loadedInterpreterPlugins) {
		mPluginInterfaces.insertMulti(kitPlugin->kitId(), kitPlugin);
	}
}
