/* Copyright 2014-2016 Anastasia Semenova
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

#include <QtCore/QTranslator>
#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>

#include <qrutils/expressionsParser/expressionsParser.h>
#include <qrkernel/ids.h>
#include <qrgui/plugins/toolPluginInterface/pluginConfigurator.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/preferencesDialog/preferencesPages/behaviourPage.h>
#include <qrkernel/platformInfo.h>

#include "databasesPreferencesPage.h"
#include "databasesCustomizer.h"
#include "generator/databasesGenerator.h"
#include "generator/databasesReverseEngineer.h"
#include "generateSchemaWidget.h"

namespace qReal {
namespace databasesSupport {

/// Main class for QReal:databases
/// Provides cooperation between main window
/// and functionality
class DatabasesSupportPlugin : public QObject, public qReal::ToolPluginInterface
{
	Q_OBJECT
	Q_INTERFACES(qReal::ToolPluginInterface)
	Q_PLUGIN_METADATA(IID "qReal.databasesSupport.DatabasesSupportPlugin")

public:
	DatabasesSupportPlugin();
	~DatabasesSupportPlugin();
	virtual void init(qReal::PluginConfigurator const &configurator);

	virtual QPair<QString, gui::PreferencesPage *> preferencesPage();
	virtual qReal::Customizer* customizationInterface();
	virtual QList<qReal::ActionInfo> actions();

public slots:
	/// Checks simple rules of correctnes (redirection to generator)
	void checkCorrectness();
	/// Generates physical model from logical model (redirection to generator)
	void generateCode();
	/// Checks SQL-code from physical model(redirection to generator)
	void generatePhysicalModel();
	/// Generate physical model from file
	void generateSchema();

private:
	// Doesn't have ownership.
	DatabasesPreferencesPage *mPreferencesPage;
	// Have ownership.
	GenerateSchemaWidget *mGenerateSchemaWidget;

	DatabasesCustomizer mCustomizer;

	// Have ownership.
	DatabasesGenerator *mDatabasesGenerator;
	// Have ownership.
	DatabasesReverseEngineer *mDatabasesReverseEngineer;

	void initActions();

	// Doesn't have ownership.
	QAction *mGenerateCodeAction;
	QAction *mCheckCorectnessAction;
	QAction *mGeneratePhysicalModelAction;
	QAction *mGenerateSchemaAction;
	QMenu *mDatabasesMenu;

	QList<qReal::ActionInfo> mActionInfos;

	QTranslator mAppTranslator;
};

}
}
