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

#include "uxInfoPlugin.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMainWindow>

#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>
#include <qrkernel/logging.h>

#include "uxInfo.h"

using namespace uxInfo;

UXInfoPlugin::UXInfoPlugin()
	: mUsabilityTestingToolbar(new QToolBar(tr("Usability")))
	, mStartTest(new QAction(tr("Start test"), nullptr))
	, mFinishTest(new QAction(tr("Finish test"), nullptr))
{
	mStartTimestamp = QDateTime::currentMSecsSinceEpoch();

	qReal::SettingsListener::listen("usabilityTestingMode", mUsabilityTestingToolbar, &QWidget::setVisible);

	mStartTest->setEnabled(true);
	mFinishTest->setEnabled(false);
	connect(mStartTest, &QAction::triggered, this, &UXInfoPlugin::startUsabilityTest);
	connect(mFinishTest, &QAction::triggered, this, &UXInfoPlugin::finishUsabilityTest);
	mUsabilityTestingToolbar->addAction(mStartTest);
	mUsabilityTestingToolbar->addAction(mFinishTest);

	connect(qReal::SettingsManager::instance(), &qReal::SettingsManager::settingsChanged
			, UXInfo::instance(), &UXInfo::reportSettingsChanges);
}

void UXInfoPlugin::init(qReal::PluginConfigurator const &configurator)
{
	initSettingsUi(*configurator.mainWindowInterpretersInterface().preferencesPages()["preferencesBehaviourPage"]);

	connect(&configurator.systemEvents(), &qReal::SystemEvents::lowLevelEvent, this, &UXInfoPlugin::processEvent);

	connect(&configurator.systemEvents(), &qReal::SystemEvents::closedMainWindow, [=]() {
		UXInfo::instance()->closeUXInfo();
		int const finishTimestamp = QDateTime::currentMSecsSinceEpoch();
		QString const totalTime = QString::number((finishTimestamp - mStartTimestamp) / 1000);
		UXInfo::reportTotalTime(totalTime);
	});

	connect(&configurator.systemEvents(), &qReal::SystemEvents::graphicalElementAdded
			, [](qReal::Id const &id) { UXInfo::reportCreation(id.editor(), id.element()); });

	connect(&configurator.systemEvents(), &qReal::SystemEvents::informationAdded
			, [](QString const &message, qReal::Id const &position) {
		UXInfo::reportErrors("information", position.editor(), position.element(), message);
	});
	connect(&configurator.systemEvents(), &qReal::SystemEvents::warningAdded
			, [](QString const &message, qReal::Id const &position) {
		UXInfo::reportErrors("warning", position.editor(), position.element(), message);
	});
	connect(&configurator.systemEvents(), &qReal::SystemEvents::errorAdded
			, [](QString const &message, qReal::Id const &position) {
		UXInfo::reportErrors("error", position.editor(), position.element(), message);
	});
	connect(&configurator.systemEvents(), &qReal::SystemEvents::criticalAdded
			, [](QString const &message, qReal::Id const &position) {
		UXInfo::reportErrors("critical", position.editor(), position.element(), message);
	});

	QWidget * const windowWidget = configurator.mainWindowInterpretersInterface().windowWidget();
	static_cast<QMainWindow *>(windowWidget)->addToolBar(Qt::TopToolBarArea, mUsabilityTestingToolbar);
	mUsabilityTestingToolbar->setVisible(qReal::SettingsManager::value("usabilityTestingMode").toBool());

	for (QAction const *action : windowWidget->actions()) {
		if (action->isCheckable()) {
			connect(action, &QAction::triggered, &mFilterObject, &FilterObject::toggledActionActivated);
		} else {
			connect(action, &QAction::triggered, &mFilterObject, &FilterObject::triggeredActionActivated);
		}
	}
}

QStringList UXInfoPlugin::defaultSettingsFiles()
{
	return { ":/uxInfoDefaultSettings.ini" };
}

void UXInfoPlugin::initSettingsUi(qReal::gui::PreferencesPage &behaviourPage)
{
	QGridLayout * const automaticsLayout = behaviourPage.findChild<QGridLayout *>("automaticsFrameLayout");
	if (!automaticsLayout) {
		QLOG_ERROR() << "Could not find 'automaticsFrameLayout' on preferences behaviour page";
		return;
	}

	QCheckBox * const ergonomicBox = new QCheckBox(tr("Collect ergonomic values"), automaticsLayout->widget());
	QCheckBox * const usabilityModeBox = new QCheckBox(tr("Usability testing mode"), automaticsLayout->widget());
	automaticsLayout->addWidget(ergonomicBox, automaticsLayout->rowCount(), 0, 1, automaticsLayout->columnCount());
	automaticsLayout->addWidget(usabilityModeBox, automaticsLayout->rowCount(), 0, 1, automaticsLayout->columnCount());

	connect(ergonomicBox, &QAbstractButton::clicked, [](bool status) { UXInfo::setStatus(status); });

	connect(&behaviourPage, &qReal::gui::PreferencesPage::saved, [=]() {
		bool const usabilityTestingMode = usabilityModeBox->isChecked();
		qReal::SettingsManager::setValue("usabilityTestingMode", usabilityTestingMode);
		qReal::SettingsManager::setValue("collectErgonomicValues", ergonomicBox->isChecked() || usabilityTestingMode);
		if (mUsabilityTestingMode != usabilityTestingMode) {
			if (usabilityTestingMode) {
				ergonomicBox->setChecked(true);
			}

			mUsabilityTestingMode = usabilityTestingMode;
		}
	});

	connect(&behaviourPage, &qReal::gui::PreferencesPage::restored, [=]() {
		ergonomicBox->setChecked(qReal::SettingsManager::value("collectErgonomicValues").toBool());
		mUsabilityTestingMode = qReal::SettingsManager::value("usabilityTestingMode").toBool();
		usabilityModeBox->setChecked(mUsabilityTestingMode);
		UXInfo::setStatus(ergonomicBox->isChecked());
	});
}

void UXInfoPlugin::processEvent(QObject *obj, QEvent *e)
{
	QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent*>(e);
	QWidget *widget = dynamic_cast<QWidget*>(obj);
	QPoint pos;
	if (mouseEvent && widget && mouseEvent->type() == QMouseEvent::MouseButtonPress) {
		pos = widget->pos();
		for (; widget; widget = dynamic_cast<QWidget*>(widget->parent())) {
			pos += widget->pos();
		}

		UXInfo::reportMouseClick(mouseEvent->pos() + pos);
	}
}

void UXInfoPlugin::startUsabilityTest()
{
	mStartTest->setEnabled(false);
	mFinishTest->setEnabled(true);
	mFilterObject.reportTestStarted();
}

void UXInfoPlugin::finishUsabilityTest()
{
	mFinishTest->setEnabled(false);
	mStartTest->setEnabled(true);
	mFilterObject.reportTestFinished();
}
