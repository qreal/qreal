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
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h"

namespace editorPluginTestingFramework {

class QrxcLauncher
{
public:

	/// launches qrxc (gets fileName, generates .xml-file and code)
	void launchQrxc(const QString &fileName, const QString &pathToQRealSources, const QString &pathToGeneratedCode);

private:

	/// generates path to plugin from .pro
	static QString generatePathToPlugin(const QString &pathToGeneratedCode);

	qReal::gui::MainWindowInterpretersInterface *mMainWindowInterface;
};

}
