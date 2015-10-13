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

#include "rulesBPMNPlugin.h"

#include <QtWidgets/QApplication>

using namespace qReal;
using namespace qReal::rulesChecker;

RulesPlugin::RulesPlugin()
	: mChecker(nullptr)
	, mRunAllDiagram(nullptr)
	, mRunCurrentDiagram(nullptr)
	, mExportToXml(nullptr)
{
	initAction();
}

RulesPlugin::~RulesPlugin()
{
	delete mChecker;
}

void RulesPlugin::init(PluginConfigurator const &configurator)
{
	mChecker = new RulesChecker(configurator.graphicalModelApi().graphicalRepoApi()
			, configurator.mainWindowInterpretersInterface());

	QObject::connect(mRunAllDiagram, SIGNAL(triggered()), mChecker, SLOT(checkAllDiagrams()));
	QObject::connect(mRunCurrentDiagram, SIGNAL(triggered()), mChecker, SLOT(checkCurrentDiagram()));
	QObject::connect(mExportToXml, SIGNAL(triggered()), mChecker, SLOT(exportToXml()));
	configurator.mainWindowDockInterface().logicalModelDock()->hide();
}

QList<ActionInfo> RulesPlugin::actions()
{
	return mActionInfos;
}

void RulesPlugin::initAction()
{
	mRunAllDiagram = new QAction(QObject::tr("Diagram Validation"), nullptr);
	mRunAllDiagram->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	ActionInfo runAllInfo(mRunAllDiagram, "generators", "tools");
	mActionInfos << runAllInfo;

	mRunCurrentDiagram = new QAction(QObject::tr("Validate active diagram"), nullptr);
	ActionInfo runCurrentInfo(mRunCurrentDiagram, "generators", "tools");
	mActionInfos << runCurrentInfo;

	mExportToXml = new QAction(QObject::tr("Generate XML"), nullptr);
	ActionInfo exportXmlInfo(mExportToXml, "generators", "tools");
	mActionInfos << exportXmlInfo;

}

qReal::Customizer * RulesPlugin::customizationInterface()
{
	return &mCustomizer;
}
