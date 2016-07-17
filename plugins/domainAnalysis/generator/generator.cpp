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

#include "generator.h"
#include <QtCore/QProcess>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDesktopWidget>
#include <QtCore/QTextStream>
#include <QtCore/QFile>

#include <qrkernel/settingsManager.h>
#include <qrkernel/roles.h>
#include <qrkernel/settingsManager.h>
#include <qrutils/outFile.h>
#include <qrutils/nameNormalizer.h>
#include <qrrepo/repoApi.h>
#include <qrmc/metaCompiler.h>
#include <qrgui/plugins/pluginManager/editorManager.h>

using namespace qReal;
using namespace domainAnalysis;
using namespace utils;
using namespace qrRepo;

Generator::Generator()
	: mGenerateMetamodelAction(nullptr)
{
}

Generator::~Generator()
{
}

void Generator::init(PluginConfigurator const &configurator)
{
	mMainWindowInterface = &configurator.mainWindowInterpretersInterface();
	mLogicalRepoApi = &configurator.logicalModelApi().mutableLogicalRepoApi();
}

//The control's connection with method "onGenerateMethod"
QList<ActionInfo> Generator::actions()
{
	mGenerateMetamodelAction.setText(tr("Generate metamodel"));
	ActionInfo generateMetaModelActionInfo(&mGenerateMetamodelAction, "generators", "tools");
	QObject::connect(&mGenerateMetamodelAction, &QAction::triggered, this, &Generator::onGenerateMetamodel);
	return { generateMetaModelActionInfo };
}

void Generator::onGenerateMetamodel()
{
	QFile testFile("test.txt");
	qrRepo::RepoApi repo("testEditor.qrs");
	//testId - editorNode
	Id const editorTestId("MetaEditor", "MetaEditor", "MetamodelDiagram", QUuid::createUuid().toString());
	repo.addChild(Id::rootId(), editorTestId);
	repo.setProperty(editorTestId, "name", "Editor");
	repo.setProperty(editorTestId, "displayedName", "Editor");
	//DiagramNode
	Id const diagramNode("MetaEditor", "MetaEditor", "MetaEditorDiagramNode", QUuid::createUuid().toString());
	repo.addChild(editorTestId, diagramNode);
	repo.setProperty(diagramNode, "name", "Diagram");
	repo.setProperty(diagramNode, "displayedName", "Diagram");
	repo.setProperty(diagramNode, "nodeName", "Diagram");
	if (testFile.open(QIODevice::WriteOnly)) {
		QTextStream stream(&testFile);
		IdList const metamodels = mLogicalRepoApi->children(Id::rootId());
		for (Id const key : metamodels) {
			QString const objectType = key.element();
			if (objectType == "DetailedFeature" && mLogicalRepoApi->isLogicalElement(key)) {
				stream << mLogicalRepoApi->name(key) << "\r\n";
				Id const nodeId("MetaEditor", "MetaEditor", "MetaEntityNode", QUuid::createUuid().toString());
				repo.addChild(diagramNode, nodeId);
				QString const &name = mLogicalRepoApi->name(key);
				repo.setProperty(nodeId, "name", name);
				repo.setProperty(nodeId, "displayedName", name);
			}
		}
		Id const edgeId("MetaEditor", "MetaEditor", "MetaEntityEdge", QUuid::createUuid().toString());
		repo.addChild(diagramNode, edgeId);
		repo.setProperty(edgeId, "name", "connection");
		repo.setProperty(edgeId, "displayedName", "Стрелка следования");
		repo.setProperty(edgeId, "shape", "broken");
		repo.setProperty(edgeId, "lineType", "solidLine");
		repo.saveAll();
		testFile.close();
		if (stream.status() != QTextStream::Ok) {
			qDebug() << "File writing erorr";
		}
	}
}
