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

#include <qrgui/models/models.h>
#include <qrutils/versioningUtils/briefVersioningInterface.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/toolPluginInterface/pluginInterface.h>
#include <qrgui/mainWindow/projectManager/projectManagerWrapper.h>

#include "versioningPluginInterface.h"

#include "versioningPluginsManagerDeclSpec.h"


namespace qReal
{

class QRGUI_VERSIONING_PLUGINS_MANAGER_EXPORT VersioningPluginsManager
		: public BriefVersioningInterface
		, public qrRepo::WorkingCopyInspectionInterface
{
	Q_OBJECT

public:
	/// Inits plugin list using loaded by plugin manager ones
	VersioningPluginsManager(
		qrRepo::RepoControlInterface *repoApi
		, qReal::ErrorReporterInterface *errorReporter
		, qReal::ProjectManagerWrapper *projectManager
	);

	void initFromToolPlugins(
		QListIterator<ToolPluginInterface *> iterator
		, EditorManagerInterface *editorManager
		, QWidget *parent
	);

	BriefVersioningInterface *activeClient(const QString &workingDir);

	bool onFileAdded(const QList<QString> &list, QString const &workingDir);
	bool onFileRemoved(const QList<QString> &list, QString const &workingDir);
	bool onFileChanged(const QList<QString> &list, QString const &workingDir);

public slots:
	void beginWorkingCopyDownloading(
			QString const &repoAddress
			, QString const &targetProject
			, QString commitId = "-1"
			, bool quiet = false
	);
	void beginWorkingCopyUpdating(QString const &targetProject = QString());
	void beginChangesSubmitting(
		QString const &description
		, QString const &targetProject = QString()
		, bool quiet = false
	);
	bool reinitWorkingCopy(QString const &targetProject = QString());
	QString information(QString const &targetProject = QString());
	QString commitId(QString const &targetProject = QString());
	QString remoteRepositoryUrl(QString const &targetProject = QString());
	bool isMyWorkingCopy(
		QString const &directory = QString()
		, bool quiet = false
		, bool prepareAndProcess = false
	);
	QString friendlyName();
	bool clientExist();

private slots:
	void onWorkingCopyDownloaded(bool success, QString const &targetProject);
	void onWorkingCopyUpdated(bool success);
	void onChangesSubmitted(bool success);

private:
	QString tempFolder() const;
	void prepareWorkingCopy();
	VersioningPluginInterface *activePlugin(
		bool needPreparation = true
		, QString const &workingDir = ""
		, bool quiet = true
	);
	WorkingCopyInspectionInterface *activeWorkingCopyInspector(QString const &workingDir = "");

	void reportError(QString const &message);
	void reportErrors(QStringList const &messages);
	void reportWarning(QString const &message);
	void reportWarnings(QStringList const &messages);

	qrRepo::RepoControlInterface *mRepoApi;
	QList<VersioningPluginInterface *> mPlugins;
	qReal::ErrorReporterInterface *mErrorReporter;
	DiffPluginInterface *mDiffInterface;
	qReal::ProjectManagerWrapper *mProjectManager;
	QString mTempDir;
};

}
