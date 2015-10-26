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

#include <QtCore/QString>
#include <QtCore/QPair>

#include <QtWidgets/QProgressBar>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {

class QRUTILS_EXPORT PluginLoader: public QObject
{
	Q_OBJECT

public:
	PluginLoader(gui::MainWindowInterpretersInterface &interpretersInterface);

	void loadNewEditor(const QString &directoryName
			, const QString &pluginsName
			, const QString &pluginId
			, const QString &commandFirst
			, const QString &commandSecond
			, const QString &extension
			, const QString &prefix
			, const QString &buildConfiguration);

private:
	QString fullPluginName(const QString &pluginName
			, const QString &buildConfiguration
			, const QString &extension
			, const QString &prefix) const;

	QProgressBar *progressBar() const;

	void deleteGeneratedFiles(const QString &directoryName
			, const QString &pluginName);

	gui::MainWindowInterpretersInterface &mMainWindowInterpretersInterface;
};

}
