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

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "mainWindow/projectManager/projectManagerWrapper.h"

namespace qReal {

class MainWindow;

/// @brief Start page dialog
/// Start page dialog that allows the user to either open an existing project (from list of recent
/// projects or not), or create a new one and did not appear in the main window of application,
/// where there is no single project.
class StartWidget : public QWidget
{
	Q_OBJECT

public:
	StartWidget(MainWindow *mainWindow, ProjectManager *projectManager);

	/// If false is passed the interpreters buttons instances are deleted.
	/// This method can change layout of the buttons on start tab.
	void setVisibleForInterpreterButton(const bool visible);

private slots:
	void openRecentProject(const QString &fileName);
	void openExistingProject();
	void createProjectWithDiagram(const QString &idString);
	void openInterpretedDiagram();
	void createInterpretedDiagram();

private:
	QWidget *createMainWidget();
	QWidget *createHeader();
	QWidget *createRecentProjectsWidget();
	QWidget *createProjectsManagementWidget();

	QLayout *createRecentProjectsList(const QString &recentProjects);
	QWidget *createPluginsList();
	QWidget *createPluginButton(const Id &editor, const Id &diagram, QWidget * const bindedImage);

	void centralizeButton(QPushButton * const styledButton);

	virtual void paintEvent(QPaintEvent *event) override;

	MainWindow *mMainWindow;  // Doesn't have ownership.
	ProjectManager *mProjectManager;  // Doesn't have ownership.
	int mProjectListSize;

	QBoxLayout *mProjectsManagementLayout;  // Has ownership.
	QWidget *mRecentProjectsWidget;  // Has ownership.
	QPushButton *mNewProjectButton;  // Has ownership.
	QPushButton *mOpenProjectButton;  // Has ownership.
	QPushButton *mOpenInterpreterButton;  // Has ownership.
	QPushButton *mCreateInterpreterButton;  // Has ownership.
};

}
