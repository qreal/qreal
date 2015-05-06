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

#include "models/logicalModelAssistApi.h"
#include "plugins/toolPluginInterface/projectConverter.h"

namespace qReal {

class MainWindow;

/// Performs validation and convertion of models due to editor versions that created them.
/// @todo: This class is not implemented at the moment.
/// It only displays error message if versions mismatch detected.
class VersionsConverterManager
{
public:
	explicit VersionsConverterManager(MainWindow &mainWindow);

	/// Performs validation and convertion of models due to editor versions that created them.
	bool validateCurrentProject();

private:
	bool convertProject(const Version &enviromentVersion
			, const Version &saveVersion
			, QList<ProjectConverter> const &converters);

	void displayTooOldEnviromentError(const Version &saveVersion);
	void displayCannotConvertError();
	void displayTooOldSaveError(const Version &saveVersion);

	void showError(const QString &errorMessage);

	MainWindow &mMainWindow;
};

}
