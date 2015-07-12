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

#include <QtWidgets/QDialog>
#include <QtCore/QSettings>

#include <qrrepo/logicalRepoApi.h>

namespace Ui {
class SpecifyPathToGeneratedCodeDialog;
}

namespace qReal {
namespace gui {

/// In this window you can specify path to generated code.
class SpecifyPathToGeneratedCodeDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param metamodelRepoApi - metamodel data (paths are stored in metamodel).
	/// @param parent - parent of this dialog.
	explicit SpecifyPathToGeneratedCodeDialog(
			qrRepo::LogicalRepoApi *metamodelRepoApi
			, QWidget *parent = 0);
	~SpecifyPathToGeneratedCodeDialog() override;

	/// Returns current path to folder with generated code.
	QString currentPathToFolder() const;

	/// Returns current main file name.
	QString currentFileName() const;

signals:
	/// Emitted when ok button pushed and paths saved.
	void pathsSpecified();

private slots:
	void restoreSettings();

	void saveSettings();

	void specifyFolder();

private:
	bool directoryCreated(const QString &directory) const;

	bool directoryRemoved(const QString &directory) const;

	static void clearDir(const QString &path);

	qrRepo::LogicalRepoApi *mMetamodelRepoApi;  // doesnt' have ownership.

	Ui::SpecifyPathToGeneratedCodeDialog *mUi;  // doesn't have owhership.
};

}
}
