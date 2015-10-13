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

#include "metaEditorSupportPlugin.h"

#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>

#include <qrkernel/settingsManager.h>
#include <qrmc/metaCompiler.h>

#include "editorGenerator.h"
#include "xmlParser.h"

using namespace qReal;
using namespace metaEditor;

MetaEditorSupportPlugin::MetaEditorSupportPlugin()
		: mGenerateEditorForQrxcAction(nullptr)
		, mGenerateEditorWithQrmcAction(nullptr)
		, mParseEditorXmlAction(nullptr)
		, mRepoControlApi(nullptr)
		, mCompilerSettingsPage(new PreferencesCompilerPage())
{
}

MetaEditorSupportPlugin::~MetaEditorSupportPlugin()
{
}

void MetaEditorSupportPlugin::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalRepoApi = &configurator.logicalModelApi().mutableLogicalRepoApi();
	mRepoControlApi = &configurator.repoControlInterface();
}

QList<ActionInfo> MetaEditorSupportPlugin::actions()
{
	mGenerateEditorForQrxcAction.setText(tr("Generate editor"));
	ActionInfo generateEditorForQrxcActionInfo(&mGenerateEditorForQrxcAction, "generators", "tools");
	connect(&mGenerateEditorForQrxcAction, SIGNAL(triggered()), this, SLOT(generateEditorForQrxc()));

	mGenerateEditorWithQrmcAction.setText(tr("Generate editor (qrmc)"));
	ActionInfo generateEditorWithQrmcActionInfo(&mGenerateEditorWithQrmcAction, "generators", "tools");
	connect(&mGenerateEditorWithQrmcAction, SIGNAL(triggered()), this, SLOT(generateEditorWithQrmc()));

	/*
	mParseEditorXmlAction.setText(tr("Parse editor xml")); // button for parsing xml, doesn't work
	ActionInfo parseEditorXmlActionInfo(&mParseEditorXmlAction, "generators", "tools");
	connect(&mParseEditorXmlAction, SIGNAL(triggered()), this, SLOT(parseEditorXml()));
	*/

	return QList<ActionInfo>() << generateEditorForQrxcActionInfo
	<< generateEditorWithQrmcActionInfo;
	//<< parseEditorXmlActionInfo;
}

QPair<QString, gui::PreferencesPage *> MetaEditorSupportPlugin::preferencesPage()
{
	return qMakePair(QObject::tr("Compiler"), static_cast<gui::PreferencesPage *>(mCompilerSettingsPage));
}

void MetaEditorSupportPlugin::generateEditorForQrxc()
{
	EditorGenerator editorGenerator(*mLogicalRepoApi, *mMainWindowInterface->errorReporter());

	QDir dir(".");

	QHash<Id, QPair<QString, QString> > metamodelList = editorGenerator.getMetamodelList();
	foreach (Id const &key, metamodelList.keys()) {
		QString const nameOfTheDirectory = metamodelList[key].first;
		QString const pathToQRealRoot = metamodelList[key].second;
		dir.mkpath(nameOfTheDirectory);
		QPair<QString, QString> const metamodelNames = editorGenerator.generateEditor(key, nameOfTheDirectory
				, pathToQRealRoot);

		if (!mMainWindowInterface->errorReporter()->wereErrors()) {
			if (QMessageBox::question(mMainWindowInterface->windowWidget()
					, tr("loading..")
					, QString(tr("Do you want to load generated editor %1?"))
							.arg(metamodelNames.first)
					, QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
				return;
			}

			loadNewEditor(nameOfTheDirectory, metamodelNames
					, SettingsManager::value("pathToQmake").toString()
					, SettingsManager::value("pathToMake").toString()
					, SettingsManager::value("pluginExtension").toString()
					, SettingsManager::value("prefix").toString()
					, mLogicalRepoApi->stringProperty(key, "buildConfiguration"));
		}
	}

	if (metamodelList.isEmpty()) {
		mMainWindowInterface->errorReporter()->addError(tr("There is nothing to generate"));
	}
}

void MetaEditorSupportPlugin::generateEditorWithQrmc()
{
	qrmc::MetaCompiler metaCompiler(qApp->applicationDirPath() + "/../../qrmc", mLogicalRepoApi);

	IdList const metamodels = mLogicalRepoApi->children(Id::rootId());

	QProgressBar *progress = new QProgressBar(mMainWindowInterface->windowWidget());
	progress->show();
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	QApplication::processEvents();
	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);

	int forEditor = 60 / metamodels.size();

	foreach (Id const &key, metamodels) {
		QString const objectType = key.element();
		if (objectType == "MetamodelDiagram" && mLogicalRepoApi->isLogicalElement(key)) {
			QString nameOfTheDirectory = mLogicalRepoApi->stringProperty(key, "name of the directory");
			QString nameOfMetamodel = mLogicalRepoApi->stringProperty(key, "name");
			QString nameOfPlugin = nameOfTheDirectory.split("/").last();

			if (QMessageBox::question(mMainWindowInterface->windowWidget()
					, tr("loading..")
					, QString(tr("Do you want to compile and load editor %1?")).arg(nameOfPlugin)
					, QMessageBox::Yes, QMessageBox::No)
					== QMessageBox::No)
			{
				continue;
			}

			progress->setValue(5);

			const QString normalizedName = nameOfMetamodel.at(0).toUpper() + nameOfMetamodel.mid(1);
			const bool stateOfLoad = mMainWindowInterface->pluginLoaded(normalizedName);
			if (!mMainWindowInterface->unloadPlugin(normalizedName)) {
				progress->close();
				delete progress;
				return;
			}

			if (!metaCompiler.compile(nameOfMetamodel)) { // generating source code for all metamodels
				QMessageBox::warning(mMainWindowInterface->windowWidget()
						, tr("error")
						, tr("Cannot generate source code for editor ") + nameOfPlugin);
				continue;
			}
			progress->setValue(20);

			QStringList qmakeArgs;
			qmakeArgs.append("CONFIG+=" + mLogicalRepoApi->stringProperty(key, "buildConfiguration"));
			qmakeArgs.append(nameOfMetamodel + ".pro");

			QProcess builder;
			builder.setWorkingDirectory(nameOfTheDirectory);
			const QStringList environment = QProcess::systemEnvironment();
			builder.setEnvironment(environment);
			builder.start(SettingsManager::value("pathToQmake").toString(), qmakeArgs);

			qDebug()  << "qmake";
			if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
				progress->setValue(40);
				builder.start(SettingsManager::value("pathToMake").toString());

				bool finished = builder.waitForFinished(100000);
				qDebug()  << "make";

				if (finished && (builder.exitCode() == 0)) {
					if (stateOfLoad) {
						QMessageBox::warning(mMainWindowInterface->windowWidget()
								, tr("Attention!"), tr("Please restart QReal."));
						progress->close();
						delete progress;
						return;
					}
					qDebug()  << "make ok";
					progress->setValue(progress->value() + forEditor / 2);

					if (!nameOfMetamodel.isEmpty()) {
						if (!mMainWindowInterface->unloadPlugin(normalizedName)) {
							QMessageBox::warning(mMainWindowInterface->windowWidget()
									, tr("error")
									, tr("cannot unload plugin ") + normalizedName);
							progress->close();
							delete progress;
							continue;
						}
					}

					QString suffix = "";
					if (mLogicalRepoApi->stringProperty(key, "buildConfiguration") == "debug") {
						suffix = "-d";
					}

					QString const generatedPluginFileName = SettingsManager::value("prefix").toString()
							+ nameOfMetamodel
							+ suffix
							+ "."
							+ SettingsManager::value("pluginExtension").toString()
							;

					if (mMainWindowInterface->loadPlugin(generatedPluginFileName, normalizedName)) {
						progress->setValue(progress->value() + forEditor / 2);
					}
				}
				progress->setValue(100);
			}
		}
	}
	if (progress->value() != 100) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot load new editor"));
	}
	progress->setValue(100);
	progress->close();
	delete progress;
}

void MetaEditorSupportPlugin::parseEditorXml()
{
	if (!mMainWindowInterface->pluginLoaded("MetaEditor")) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error")
				, tr("required plugin (MetaEditor) is not loaded"));

		return;
	}
	QDir dir(".");
	QString directoryName = ".";
	while (dir.cdUp()) {
		QFileInfoList const infoList = dir.entryInfoList(QDir::Dirs);
		foreach (QFileInfo const &directory, infoList){
			if (directory.baseName() == "qrxml") {
				directoryName = directory.absolutePath() + "/qrxml";
			}
		}
	}
	QString const fileName = QFileDialog::getOpenFileName(mMainWindowInterface->windowWidget()
			, tr("Select xml file to parse")
			, directoryName
			, "XML files (*.xml)");

	if (fileName.isEmpty())
		return;

	XmlParser parser(*mLogicalRepoApi);

	parser.parseFile(fileName);

	parser.loadIncludeList(fileName);

	mMainWindowInterface->reinitModels();
}

void MetaEditorSupportPlugin::loadNewEditor(QString const &directoryName
		, QPair<QString, QString> const &metamodelNames
		, QString const &commandFirst
		, QString const &commandSecond
		, QString const &extension
		, QString const &prefix
		, QString const &buildConfiguration
		)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	QString const metamodelName = metamodelNames.first;
	QString const normalizerMetamodelName = metamodelNames.second;

	if ((commandFirst == "") || (commandSecond == "") || (extension == "")) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("please, fill compiler settings"));
		return;
	}

	QString const normalizeDirName = metamodelName.at(0).toUpper() + metamodelName.mid(1);

	QProgressBar * const progress = new QProgressBar(mMainWindowInterface->windowWidget());
	progress->show();

	QApplication::processEvents();

	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);
	progress->setValue(5);

	const bool stateOfLoad = mMainWindowInterface->pluginLoaded(normalizeDirName);
	if (!mMainWindowInterface->unloadPlugin(normalizeDirName)) {
		progress->close();
		delete progress;
		return;
	}

	progress->setValue(20);
	QStringList qmakeArgs;
	qmakeArgs.append("CONFIG+=" + buildConfiguration);
	qmakeArgs.append(metamodelName + ".pro");

	QProcess builder;
	builder.setWorkingDirectory(directoryName);
	const QStringList environment = QProcess::systemEnvironment();
	builder.setEnvironment(environment);
	builder.start(commandFirst, qmakeArgs);

	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
		progress->setValue(60);
		builder.start(commandSecond);

		if (builder.waitForFinished(60000) && (builder.exitCode() == 0)) {
			progress->setValue(80);

			if (stateOfLoad) {
				QMessageBox::warning(mMainWindowInterface->windowWidget()
						, tr("Attention!"), tr("Please restart QReal."));
				progress->close();
				delete progress;
				return;
			} else if (buildConfiguration == "debug") {
				if (mMainWindowInterface->loadPlugin(prefix + metamodelName 
						+ "-d"+ "." + extension, normalizeDirName)) 
				{
					progress->setValue(100);
				}
			} else {
				if (mMainWindowInterface->loadPlugin(prefix + metamodelName + "." + extension, normalizeDirName)) {
					progress->setValue(100);
				}
			}
		}
	}

	if (progress->value() == 20) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot qmake new editor"));
	} else if (progress->value() == 60) {
		QMessageBox::warning(mMainWindowInterface->windowWidget(), tr("error"), tr("cannot make new editor"));
	}

	progress->setValue(100);
	progress->close();
	delete progress;
}
