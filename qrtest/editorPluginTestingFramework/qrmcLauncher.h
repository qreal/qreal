/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

namespace editorPluginTestingFramework {

/// Helper class that launches QRMC and generates code by metamodel.
class QrmcLauncher
{
public:
	QrmcLauncher() = delete;

	/// Launches QRMC on a given .qrs file with metamodel and generates plugin code into given target directory.
	static void launchQrmc(const QString &fileName, const QString &targetDirectory);
};

}
